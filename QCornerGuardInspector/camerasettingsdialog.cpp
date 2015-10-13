#include "camerasettingsdialog.h"
#include "ui_camerasettingsdialog.h"

CameraSettingsDialog::CameraSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CameraSettingsDialog)
{
    ui->setupUi( this );
    setFixedSize( 250, 250 );

    connect( ui->buttSaveClose, SIGNAL( clicked() ), this, SLOT( accept() ) );

    settings.load();
    ui->dspnExposureTime->setValue( settings.exposure_time );
    ui->spnROIX->setValue( settings.roi.x );
    ui->spnROIY->setValue( settings.roi.y );
    ui->spnROIWidth->setValue( settings.roi.width );
    ui->spnROIHeight->setValue( settings.roi.height );
}

CameraSettingsDialog::~CameraSettingsDialog()
{
    delete ui;
}

void CameraSettingsDialog::accept()
{
    settings.exposure_time = ui->dspnExposureTime->value();
    settings.roi.x = ui->spnROIX->value();
    settings.roi.y = ui->spnROIY->value();
    settings.roi.width = ui->spnROIWidth->value();
    settings.roi.height = ui->spnROIHeight->value();

    settings.save();

    QDialog::accept();
}
