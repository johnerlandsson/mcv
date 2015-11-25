#ifndef GENERALSETTINGS_H
#define GENERALSETTINGS_H


class GeneralSettings
{
    public:
        GeneralSettings();
//        GeneralSettings operator=( GeneralSettings &other ) = default;
        int threshold;
        int min_profile_area;
        int min_profile_hole_area;
        void load();
        void save();
};

#endif // GENERALSETTINGS_H
