#include "barcodesettings.h"
#include <QSettings>
#include <QFile>
#include <iostream>
#include <QRegularExpression>
#include <QTextStream>

BarcodeSettings::BarcodeSettings() : timeout_s{ 10 }
{
    QSettings s;
    valid_barcodes_path = s.fileName();
    valid_barcodes_path.remove( QRegularExpression( "[^/]*$" ) );
    valid_barcodes_path.append( "valid_barcodes.txt" );
}

void BarcodeSettings::load()
{
    QSettings s;
    timeout_s = s.value( "BarcodeSettings/timeout_s", 10 ).toInt();
    QFile f( valid_barcodes_path );
    if( f.open( QIODevice::ReadOnly ) )
    {
        QTextStream ts( &f );
        valid_barcodes = ts.readAll().split( '\n' );
        f.close();
    }
}

void BarcodeSettings::save()
{
    QSettings s;
    s.setValue( "BarcodeSettings/timeout_s", timeout_s );

    QFile f( valid_barcodes_path );
    if( f.open( QIODevice::WriteOnly | QIODevice::Truncate ) )
    {
        QTextStream ts( &f );
        ts << valid_barcodes.join( '\n' );
        f.close();
    }
}
