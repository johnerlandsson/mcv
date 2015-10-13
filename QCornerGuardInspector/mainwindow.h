#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <mcv/ICubeCamera.h>
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

class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        explicit MainWindow( QWidget *parent = 0 );
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
        void resetBarcodeTimeoutCounter();
        void on_hsThreshold_valueChanged(int value);
        void on_actionBarcode_triggered();
        void on_actionCamera_triggered();
        void on_actionGeneral_triggered();
        void on_actionHole_triggered();

    private:
        void setupImageSource();
        void initCamera();

        Ui::MainWindow *ui;
        QTimer secondsTimer;
        QTimer imageDisplayTimer;
        int barcodeTimeoutCounter;
        mcv::ICubeCamera cam;
        ImageProcessor imgproc;
        BarcodeSettings barcode_settings;
        CameraSettings camera_settings;
        GeneralSettings general_settings;
        HoleSettings hole_settings;
        QStringListModel cmbValidBarcodeModel;
        AlarmsTableModel tvAlarmsModel;
};

#endif // MAINWINDOW_H
