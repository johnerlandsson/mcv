#include "mainwindow.h"
#include <QApplication>
#include <mcv/ICubeCamera.h>
#include <mcv/ImageFileLoader.h>
#include <QCommandLineParser>
#include <iostream>
#include <QSharedPointer>

QSharedPointer<mcv::AbstractImageProvider> selectCam()
{
    QSharedPointer<mcv::ICubeCamera> ret( new mcv::ICubeCamera( 0 ), &QObject::deleteLater );

    return ret;
}

QSharedPointer<mcv::AbstractImageProvider> selectFiles()
{
    QSharedPointer<mcv::ImageFileLoader> ret( new mcv::ImageFileLoader, &QObject::deleteLater );

    return ret;
}

QSharedPointer<mcv::AbstractImageProvider> selectImageSource( QString src )
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

    MainWindow w;
    w.show();

    return a.exec();
}
