#include "barcodesettings.h"
#include <QSettings>
#include <QFile>
#include <iostream>
#include <QRegularExpression>
#include <QTextStream>

BarcodeSettings::BarcodeSettings() : timeout_s{ 10 }, n_error_frames{ 2 }
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
    n_error_frames = s.value( "BarcodeSettings/n_error_frames" ).toInt();
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
    s.setValue( "BarcodeSettings/n_error_frames", n_error_frames );

    QFile f( valid_barcodes_path );
    if( f.open( QIODevice::WriteOnly | QIODevice::Truncate ) )
    {
        QTextStream ts( &f );
        ts << valid_barcodes.join( '\n' );
        f.close();
    }
}
