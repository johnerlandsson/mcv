#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <mcv/AbstractImageProcessor.h>
#include <QObject>
#include <zbar.h>
#include "generalsettings.h"
#include "holesettings.h"
#include <QMutex>
#include <QVector>
#include <mcv/Blob.h>

class ImageProcessor : public QObject, public mcv::AbstractImageProcessor
{
    Q_OBJECT
    public:
        enum class Mode { Normal, Threshold };

        ImageProcessor( QObject *parent = 0 );
        virtual ~ImageProcessor();
        void process( cv::Mat &input_frame );
        void setMode( Mode m );

    public slots:
        void setGeneralSettings( GeneralSettings s );
        void setHoleSettings( HoleSettings s );

   private:
        void checkBarcode( cv::Mat &cvframe );
        void processNormal(const cv::Mat &thresh_frame, const cv::Mat &input_frame, GeneralSettings gs , HoleSettings hs);
        bool checkDrawHoles( QVector<QVector<cv::Point>> &holes, cv::Mat &outFrame );
        mcv::Blob findProfile( const cv::Mat1b &threshFrame, GeneralSettings s );
        QVector<QVector<cv::Point>> findDrawHoles(const mcv::Blob &profile , cv::Mat outframe, HoleSettings s);
        QVector<cv::Point> findHoles(const mcv::Blob &profile , HoleSettings s);
        QVector<QVector<cv::Point>> splitHolesByRow( QVector<cv::Point> &points, const double max_y_dev );

        zbar::ImageScanner scanner;
        Mode mode;
        GeneralSettings general_settings;
        QMutex general_settings_mutex;
        HoleSettings hole_settings;
        QMutex hole_settings_mutex;

    signals:
        void foundBarcode( const QString data );
        void missingHole();
        void noHolesInProfile();
};

#endif // IMAGEPROCESSOR_H
