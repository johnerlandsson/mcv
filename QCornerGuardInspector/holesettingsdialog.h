#ifndef HOLESETTINGSDIALOG_H
#define HOLESETTINGSDIALOG_H

#include <QDialog>
#include "holesettings.h"

namespace Ui {
class HoleSettingsDialog;
}

class HoleSettingsDialog : public QDialog
{
        Q_OBJECT

    public:
        explicit HoleSettingsDialog(QWidget *parent = 0);
        ~HoleSettingsDialog();
        void accept();
        HoleSettings settings;

    private:
        Ui::HoleSettingsDialog *ui;
};

#endif // HOLESETTINGSDIALOG_H
