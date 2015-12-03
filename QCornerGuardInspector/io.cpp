#ifdef USE_IO
#include "io.h"
#include <iostream>

IO::IO( QObject *parent ) : QObject( parent ), hw{ 0 }
{

}

IO::~IO()
{
    hw.release();
}

void IO::setup()
{
    hw.init();
}

void IO::signalBarcodeAlarm()
{
    hw.setOutputValue( _alarm_output_pin, true );
    hw.setOutputValue( _external_alarm_2_pin, true );
    hw.setOutputValue( _red_lamp_pin, true );
    hw.writeOutputs();
}

void IO::signalHoleAlarm()
{
    hw.setOutputValue( _alarm_output_pin, true );
    hw.setOutputValue( _external_alarm_1_pin, true );
    hw.setOutputValue( _orange_lamp_pin, true );
    hw.writeOutputs();
}

void IO::update()
{
    hw.updateInputs();

    if( hw.input( _reset_alarm_pin ) )
    {
        hw.setOutputValue( _alarm_output_pin, false );
        hw.setOutputValue( _orange_lamp_pin, false );
        hw.setOutputValue( _red_lamp_pin, false );
        hw.setOutputValue( _external_alarm_1_pin, false );
        hw.setOutputValue( _external_alarm_2_pin, false );
        hw.writeOutputs();
    }
}

#endif
