#include "generalsettings.h"
#include <QSettings>

GeneralSettings::GeneralSettings()
{

}

void GeneralSettings::load()
{
    QSettings s;
    threshold = s.value( "GeneralSettings/threshold", 0 ).toInt();
    min_profile_area = s.value( "GeneralSettings/min_profile_area", 0 ).toInt();
    min_profile_hole_area = s.value( "GeneralSettings/min_profile_hole_area", 0 ).toInt();
}

void GeneralSettings::save()
{
    QSettings s;
    s.setValue( "GeneralSettings/threshold", threshold );
    s.setValue( "GeneralSettings/min_profile_area", min_profile_area );
    s.setValue( "GeneralSettings/min_profile_hole_area", min_profile_hole_area );
}
