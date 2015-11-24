#include "mainwindow.h"
#include <QApplication>
#include <mcv/ICubeCamera.h>
#include <mcv/ImageFileLoader.h>
#include <QCommandLineParser>
#include <iostream>
#include <memory>

std::shared_ptr<mcv::AbstractImageProvider> selectCam()
{
    std::shared_ptr<mcv::ICubeCamera> ret( new mcv::ICubeCamera( 0 ) );
    ret->init();
    ret->setExposure( 1.0f );

    return ret;
}

std::shared_ptr<mcv::AbstractImageProvider> selectFiles()
{
    std::shared_ptr<mcv::ImageFileLoader> ret( new mcv::ImageFileLoader( "/home/john/dev/mcv_workspace/images", 100 ) );
    ret->setExtensions( { ".jpg", ".png" } );

    return ret;
}

std::shared_ptr<mcv::AbstractImageProvider> selectImageSource( QString src )
{
    if( src == "cam" )
        return selectCam();

    return selectFiles();
}

int main(int argc, char *argv[])
{
    //Setup core application
    QCoreApplication::setOrganizationName( "Lunatech Innovations" );
    QCoreApplication::setOrganizationDomain( "lunatech.se" );
    QCoreApplication::setApplicationName( "QCornerGuardInspector" );

    QApplication a( argc, argv );

    //Parse command line arguments
    QCommandLineParser parser;
    parser.setApplicationDescription( "QCornerGuardInspector" );
    parser.addVersionOption();

    QCommandLineOption inputSrcOption( QStringList() << "i" << "input-source", QCoreApplication::translate( "main", "Select image source (cam, files)"),
                                       QCoreApplication::translate( "main", "input source" ) );
    parser.addOption( inputSrcOption );

    parser.process( a );

    //Select image source
    QString inputSource = parser.value( inputSrcOption );
    if( inputSource != "cam" && inputSource != "files" )
    {
        std::cerr << "Invalid input source selected" << std::endl;
        return 1;
    }

    auto imgSrc = selectImageSource( inputSource );
    if( !imgSrc )
    {
        std::cerr << "Invalid image source. Exiting..." << std::endl;
        return 1;
    }


    imgSrc->open();

    MainWindow w( 0, imgSrc );
    w.show();

    int ret = a.exec();

    imgSrc->close();

    return ret;
}
