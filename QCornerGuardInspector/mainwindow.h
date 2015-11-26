#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <memory>
#include <mcv/AbstractImageProvider.h>
#include "imageprocessor.h"
#include "barcodesettings.h"
#include "camerasettings.h"
#include "generalsettings.h"
#include "holesettings.h"
#include <QStringListModel>
#include "alarmstablemodel.h"


namespace Ui {
class MainWindow;
}

typedef std::shared_ptr<mcv::AbstractImageProvider> P_ImgSrc;

class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        explicit MainWindow( QWidget *parent = 0, P_ImgSrc imgsrc = P_ImgSrc() );
        ~MainWindow();

    private slots:
        void setEnableWidgetsRunStop( bool stop );
        void everySecond();
        void startProcessing();
        void on_buttStartStop_toggled(bool checked);
        void updateImageDisplay();
        void closeEvent( QCloseEvent *e );
        void loadAlarmCheckboxStates();
        void saveAlarmCheckboxStates();
        void loadBarcodeTimeoutSpinBoxState();
        void saveBarcodeTimeoutSpinBoxState();
        void loadHsThresholdState();
        void resetBarcodeTimeoutCounter();
        void on_hsThreshold_valueChanged(int value);
        void on_actionBarcode_triggered();
        void on_actionCamera_triggered();
        void on_actionGeneral_triggered();
        void on_actionHole_triggered();
        void validateBarcode( const QString data );
        void tab_switched( int index );

        void on_actionShutdown_triggered();

    signals:
        void barcode_timeout_alarm();
        void invalid_barcode_alarm();

    private:
        Ui::MainWindow *ui;
        QTimer secondsTimer;
        QTimer imageDisplayTimer;
        int barcodeTimeoutCounter;
        ImageProcessor imgproc;
        BarcodeSettings barcode_settings;
        CameraSettings camera_settings;
        GeneralSettings general_settings;
        HoleSettings hole_settings;
        QStringListModel cmbValidBarcodeModel;
        AlarmsTableModel tvAlarmsModel;
        P_ImgSrc imageSource;
        bool barcodeTimeoutAlarmRaised;
};

#endif // MAINWINDOW_H
