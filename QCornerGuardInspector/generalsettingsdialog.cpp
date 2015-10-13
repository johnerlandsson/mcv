#include "generalsettingsdialog.h"
#include "ui_generalsettingsdialog.h"

GeneralSettingsDialog::GeneralSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GeneralSettingsDialog)
{
    ui->setupUi(this);
    connect( ui->buttSaveClose, SIGNAL( clicked() ), this, SLOT( accept() ) );
    settings.load();
    ui->spnThreshold->setValue( settings.threshold );
    ui->spnMinProfileArea->setValue( settings.min_profile_area );
    ui->spnMinProfileHoleArea->setValue( settings.min_profile_hole_area );
}

GeneralSettingsDialog::~GeneralSettingsDialog()
{
    delete ui;
}

void GeneralSettingsDialog::accept()
{
    settings.threshold = ui->spnThreshold->value();
    settings.min_profile_area = ui->spnMinProfileArea->value();
    settings.min_profile_hole_area = ui->spnMinProfileHoleArea->value();
    settings.save();

    QDialog::accept();
}
