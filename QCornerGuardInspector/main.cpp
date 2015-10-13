#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName( "Lunatech Innovations" );
    QCoreApplication::setOrganizationDomain( "lunatech.se" );
    QCoreApplication::setApplicationName( "QCornerGuardInspector" );

    QApplication a( argc, argv );
    MainWindow w;
    //w.showFullScreen()
    w.show();

    return a.exec();
}
