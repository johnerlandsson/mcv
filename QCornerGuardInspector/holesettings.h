#ifndef HOLESETTINGS_H
#define HOLESETTINGS_H


class HoleSettings
{
    public:
        HoleSettings();
        void load();
        void save();

        double max_y_dev;
        double min_circularity;
        double max_circularity;
        double max_cc_deviation;
        int min_area;
        int n_error_frames;
};

#endif // HOLESETTINGS_H
