#include "holesettingsdialog.h"
#include "ui_holesettingsdialog.h"

HoleSettingsDialog::HoleSettingsDialog(QWidget *parent) : QDialog( parent ), ui( new Ui::HoleSettingsDialog )
{
    ui->setupUi( this );
    connect( ui->buttSaveClose, SIGNAL( clicked() ), this, SLOT( accept() ) );
    settings.load();

    ui->dspMaxCCDeviation->setValue( settings.max_cc_deviation );
    ui->dspnMaxCircularity->setValue( settings.max_circularity );
    ui->dspnMaxYDev->setValue( settings.max_y_dev );
    ui->dspnMinCircularity->setValue( settings.min_circularity );
    ui->spnMinArea->setValue( settings.min_area );
    ui->spnNErrorFrames->setValue( settings.n_error_frames );
}

HoleSettingsDialog::~HoleSettingsDialog()
{
    delete ui;
}

void HoleSettingsDialog::accept()
{
    settings.max_cc_deviation = ui->dspMaxCCDeviation->value();
    settings.max_circularity = ui->dspnMaxCircularity->value();
    settings.max_y_dev = ui->dspnMaxYDev->value();
    settings.min_area = ui->spnMinArea->value();
    settings.min_circularity = ui->dspnMinCircularity->value();
    settings.n_error_frames = ui->spnNErrorFrames->value();
    settings.save();
    QDialog::accept();
}
