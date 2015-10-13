#ifndef GENERALSETTINGSDIALOG_H
#define GENERALSETTINGSDIALOG_H

#include <QDialog>
#include "generalsettings.h"

namespace Ui {
class GeneralSettingsDialog;
}

class GeneralSettingsDialog : public QDialog
{
        Q_OBJECT

    public:
        explicit GeneralSettingsDialog(QWidget *parent = 0);
        ~GeneralSettingsDialog();
        GeneralSettings settings;

    private slots:
        void accept();

    private:
        Ui::GeneralSettingsDialog *ui;
};

#endif // GENERALSETTINGSDIALOG_H
