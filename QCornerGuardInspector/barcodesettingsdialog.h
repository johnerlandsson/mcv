#ifndef BARCODESETTINGSDIALOG_H
#define BARCODESETTINGSDIALOG_H

#include <QDialog>
#include "barcodesettings.h"

namespace Ui {
class BarcodeSettingsDialog;
}

class BarcodeSettingsDialog : public QDialog
{
        Q_OBJECT

    public:
        explicit BarcodeSettingsDialog(QWidget *parent = 0);
        ~BarcodeSettingsDialog();
        BarcodeSettings settings;
        void accept();

    private slots:

    private:
        Ui::BarcodeSettingsDialog *ui;
};

#endif // BARCODESETTINGSDIALOG_H
