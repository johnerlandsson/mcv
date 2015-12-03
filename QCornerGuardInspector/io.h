#ifdef USE_IO
#ifndef IO_H
#define IO_H

#include <QObject>
#include "PCI7260.h"

class IO : public QObject
{
    Q_OBJECT
    public:
        IO( QObject *parent = 0 );
        virtual ~IO();
        void setup();

    public slots:
        void signalBarcodeAlarm();
        void signalHoleAlarm();
        void update();

    private:
        PCI7260 hw;
        const uint8_t _reset_alarm_pin = 0;
        const uint8_t _alarm_output_pin = 0;
        const uint8_t _orange_lamp_pin = 2;
        const uint8_t _red_lamp_pin = 1;
        const uint8_t _external_alarm_1_pin = 3;
        const uint8_t _external_alarm_2_pin = 4;

};

#endif // IO_H
#endif //USE_IO
