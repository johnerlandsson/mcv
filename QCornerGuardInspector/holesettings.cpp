#include "holesettings.h"
#include <QSettings>

HoleSettings::HoleSettings()
{

}

void HoleSettings::load()
{
    QSettings s;
    max_y_dev = s.value( "HoleSettings/max_y_dev", 0.0f ).toDouble();
    min_circularity = s.value( "HoleSettings/min_circularity", 0.0f ).toDouble();
    max_circularity =  s.value( "HoleSettings/max_circularity", 0.0f ).toDouble();
    max_cc_deviation =  s.value( "HoleSettings/max_cc_deviation", 0.0f ).toDouble();
    min_area =  s.value( "HoleSettings/min_area", 0.0f ).toInt();
    n_error_frames =  s.value( "HoleSettings/n_error_frames", 0.0f ).toInt();
}

void HoleSettings::save()
{
    QSettings s;

    s.setValue( "HoleSettings/max_y_dev", max_y_dev );
    s.setValue( "HoleSettings/min_circularity", min_circularity );
    s.setValue( "HoleSettings/max_circularity", max_circularity );
    s.setValue( "HoleSettings/max_cc_deviation", max_cc_deviation );
    s.setValue( "HoleSettings/min_area", min_area );
    s.setValue( "HoleSettings/n_error_frames", n_error_frames );
}
