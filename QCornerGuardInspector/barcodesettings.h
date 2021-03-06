#ifndef BARCODESETTINGS_H
#define BARCODESETTINGS_H

#include <QStringList>

class BarcodeSettings
{
    public:
        BarcodeSettings();
        void load();
        void save();

        QStringList valid_barcodes;
        int timeout_s;
        int n_error_frames;

    private:
        QString valid_barcodes_path;
};

#endif // BARCODESETTINGS_H
