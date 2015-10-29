#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <iostream>
#include "barcodesettingsdialog.h"
#include "camerasettingsdialog.h"
#include "generalsettingsdialog.h"
#include "holesettingsdialog.h"
#include <QMessageBox>

MainWindow::MainWindow( QWidget *parent, P_ImgSrc imgsrc ) : QMainWindow( parent ), ui( new Ui::MainWindow ), barcodeTimeoutCounter{ 0 }, imageSource{ imgsrc }
{
    ui->setupUi( this );

    //Setup seconds timer
    connect( &secondsTimer, SIGNAL( timeout() ),
             this, SLOT( everySecond() ) );
    secondsTimer.start( 1000 );

    //Timer to update glwidget
    connect( &imageDisplayTimer, SIGNAL( timeout() ),
              this, SLOT( updateImageDisplay() ) );
    imageDisplayTimer.start( 33 );

    //Adjust size proportions of left splitter
    QList<int> sizes;
    sizes.append( 1000 );
    sizes.append( 100 );
    ui->splitter->setSizes( sizes );

    //Load widget states
    loadAlarmCheckboxStates();
    loadBarcodeTimeoutSpinBoxState();

    connect( &imgproc, SIGNAL( foundValidBarcode() ),
             this, SLOT( resetBarcodeTimeoutCounter() ) );

    //Load settings
    barcode_settings.load();
    resetBarcodeTimeoutCounter();
    camera_settings.load();
    general_settings.load();
    on_hsThreshold_valueChanged( general_settings.threshold );
    hole_settings.load();

    //Valid barcodes combo box
    cmbValidBarcodeModel.setStringList( barcode_settings.valid_barcodes );
    ui->cmbValidBarcode->setModel( &cmbValidBarcodeModel );

    //Alarms
    ui->tvAlarms->setModel( &tvAlarmsModel );
    ui->tvAlarms->horizontalHeader()->setSectionResizeMode( 0, QHeaderView::ResizeToContents );
    ui->tvAlarms->horizontalHeader()->setSectionResizeMode( 1, QHeaderView::Stretch );
    ui->tvAlarms->horizontalHeader()->setSectionResizeMode( 2, QHeaderView::ResizeToContents );

    //Image source
    imageSource->setProcessFunction( &imgproc );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setEnableWidgetsRunStop( bool stop )
{
    //Active alarms check boxes
    ui->chkBarcodeTimeout->setEnabled( !stop );
    ui->chkInvalidBarcode->setEnabled( !stop );
    ui->chkInvalidProfile->setEnabled( !stop );
    ui->chkMissingHole->setEnabled( !stop );

    //Menu bar items
    ui->menuFile->setEnabled( !stop );
    ui->menuSettings->setEnabled( !stop );

    //tab widget
    ui->tabWidget->setEnabled( !stop );
}

void MainWindow::everySecond()
{
    if( ui->buttStartStop->isChecked() )
    {
        if( barcodeTimeoutCounter > 0 && ui->chkBarcodeTimeout->isChecked() )
            ui->lcdBarcodeTimeout->display( --barcodeTimeoutCounter );
    }
}

void MainWindow::startProcessing()
{
    resetBarcodeTimeoutCounter();
    ui->lcdBarcodeTimeout->display( barcodeTimeoutCounter );
}

void MainWindow::on_buttStartStop_toggled( bool checked )
{
    if( checked )
    {
        if( ui->cmbValidBarcode->currentIndex() < 0 )
        {
            ui->buttStartStop->setChecked( false );
            QMessageBox::critical( nullptr, "Error", "No barcode selected." );
            return;
        }

        startProcessing();
//        setupImageSource();
//        cam.start();
        imageSource->start();
    }
    else
    {
        imageSource->stop();
    }

    setEnableWidgetsRunStop( checked );
}

void MainWindow::updateImageDisplay()
{
    if( ui->buttStartStop->isChecked() )
    {
        static bool first_update = true;

        cv::Mat frame = imgproc.outputFrame();
        ui->glwdgImage->setImage( frame );

        //Resize glviewport on first valid frame. Will be 64x64 until resize otherwize
        if( first_update && frame.data )
        {
            first_update = false;
            ui->glwdgImage->resizeGL( ui->glwdgImage->width(), ui->glwdgImage->height() );
        }
    }
}

void MainWindow::closeEvent( QCloseEvent *e )
{
   if( ui->buttStartStop->isChecked() )
   {
       e->ignore();
   }
   else
   {
       saveAlarmCheckboxStates();
       saveBarcodeTimeoutSpinBoxState();
       e->accept();
   }
}

void MainWindow::loadAlarmCheckboxStates()
{
    QSettings s;
    if( s.value( "AlarmCheckboxStates/BarcodeTimeout", false ).toBool() )
        ui->chkBarcodeTimeout->setChecked( true );

    if( s.value( "AlarmCheckboxStates/InvalidBarcode", false ).toBool() )
        ui->chkInvalidBarcode->setChecked( true );

    if( s.value( "AlarmCheckboxStates/InvalidProfile", false ).toBool() )
        ui->chkInvalidProfile->setChecked( true );

    if( s.value( "AlarmCheckboxStates/MissingHole", false ).toBool() )
        ui->chkMissingHole->setChecked( true );
}

void MainWindow::saveAlarmCheckboxStates()
{
    QSettings s;

    s.setValue( "AlarmCheckboxStates/BarcodeTimeout", ui->chkBarcodeTimeout->isChecked() );
    s.setValue( "AlarmCheckboxStates/InvalidBarcode", ui->chkInvalidBarcode->isChecked() );
    s.setValue( "AlarmCheckboxStates/InvalidProfile", ui->chkInvalidProfile->isChecked() );
    s.setValue( "AlarmCheckboxStates/MissingHole", ui->chkMissingHole->isChecked() );
}

void MainWindow::loadBarcodeTimeoutSpinBoxState()
{
    QSettings s;
    ui->spnBarcodeTimeout->setValue( s.value( "BarcodeSpinBoxState/BarcodeTimeout", 10 ).toInt() );
}

void MainWindow::saveBarcodeTimeoutSpinBoxState()
{
    QSettings s;
    s.setValue( "BarcodeSpinBoxState/BarcodeTimeout", ui->spnBarcodeTimeout->value() );
}

void MainWindow::resetBarcodeTimeoutCounter()
{
    barcodeTimeoutCounter = barcode_settings.timeout_s;
    ui->lcdBarcodeTimeout->display( barcodeTimeoutCounter );
}

void MainWindow::on_hsThreshold_valueChanged( int value )
{
    ui->lblThreshold->setText( QString( "%1" ).arg( value ) );
}

void MainWindow::on_actionBarcode_triggered()
{
    BarcodeSettingsDialog d;
    if( d.exec() == QDialog::Accepted )
    {
        barcode_settings = d.settings;
        resetBarcodeTimeoutCounter();
        cmbValidBarcodeModel.setStringList( barcode_settings.valid_barcodes );
    }
}

void MainWindow::on_actionCamera_triggered()
{
    CameraSettingsDialog d;
    if( d.exec() == QDialog::Accepted )
        camera_settings = d.settings;
}

void MainWindow::on_actionGeneral_triggered()
{
    GeneralSettingsDialog d;
    if( d.exec() == QDialog::Accepted )
    {
        general_settings = d.settings;
        on_hsThreshold_valueChanged( general_settings.threshold );
    }
}

void MainWindow::on_actionHole_triggered()
{
    HoleSettingsDialog d;
    if( d.exec() == QDialog::Accepted )
        hole_settings = d.settings;
}
