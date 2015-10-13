#ifndef CAMERASETTINGSDIALOG_H
#define CAMERASETTINGSDIALOG_H

#include <QDialog>
#include "camerasettings.h"

namespace Ui {
class CameraSettingsDialog;
}

class CameraSettingsDialog : public QDialog
{
        Q_OBJECT

    public:
        explicit CameraSettingsDialog(QWidget *parent = 0);
        ~CameraSettingsDialog();
        CameraSettings settings;
        void accept();

    private slots:

    private:
        Ui::CameraSettingsDialog *ui;
};

#endif // CAMERASETTINGSDIALOG_H
