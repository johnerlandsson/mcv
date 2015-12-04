#include "barcodesettingsdialog.h"
#include "ui_barcodesettingsdialog.h"

BarcodeSettingsDialog::BarcodeSettingsDialog( QWidget *parent ) : QDialog( parent ), ui( new Ui::BarcodeSettingsDialog )
{
    ui->setupUi(this);
    settings.load();
    connect( ui->buttSaveClose, SIGNAL( clicked() ), this, SLOT( accept() ) );
    ui->spnTimeout->setValue( settings.timeout_s );
    ui->spnNErrorFrames->setValue( settings.n_error_frames );
    ui->txtValidBarcodes->setText( settings.valid_barcodes.join( '\n' ) );
}

BarcodeSettingsDialog::~BarcodeSettingsDialog()
{
    delete ui;
}

void BarcodeSettingsDialog::accept()
{
    settings.timeout_s = ui->spnTimeout->value();
    settings.n_error_frames = ui->spnNErrorFrames->value();
    settings.valid_barcodes = ui->txtValidBarcodes->toPlainText().split( '\n' );
    settings.save();
    QDialog::accept();
}

