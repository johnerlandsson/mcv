#include "camerasettings.h"
#include <QSettings>

CameraSettings::CameraSettings()
{

}

void CameraSettings::load()
{
    QSettings s;
    exposure_time = s.value( "CameraSettings/exposure_time", 10.0f ).toFloat();
    roi.x = s.value( "CameraSettings/roi_x", 0 ).toInt();
    roi.y = s.value( "CameraSettings/roi_y", 0 ).toInt();
    roi.width = s.value( "CameraSettings/roi_width", 0 ).toInt();
    roi.height = s.value( "CameraSettings/roi_height", 0 ).toInt();
}

void CameraSettings::save()
{
    QSettings s;
    s.setValue( "CameraSettings/exposure_time", exposure_time );
    s.setValue( "CameraSettings/roi_x", roi.x );
    s.setValue( "CameraSettings/roi_y", roi.y );
    s.setValue( "CameraSettings/roi_width", roi.width );
    s.setValue( "CameraSettings/roi_height", roi.height );
}
