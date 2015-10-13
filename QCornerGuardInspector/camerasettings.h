#ifndef CAMERASETTINGS_H
#define CAMERASETTINGS_H

#include <opencv2/core/core.hpp>

class CameraSettings
{
    public:
        CameraSettings();
        cv::Rect roi;
        float exposure_time;
        void load();
        void save();

};

#endif // CAMERASETTINGS_H
