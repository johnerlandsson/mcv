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

/* \brief Mechanism for processing an image sent from an ImageProvider
 *
 */
class ImageProcessor : public QObject, public mcv::AbstractImageProcessor
{
    Q_OBJECT
    public:
        enum class Mode { Normal, Threshold };

        /* \brief Default constructor
         * @param[in] parent
         *
         */
        ImageProcessor( QObject *parent = 0 );

        /* \brief Destructor
         *
         */
        virtual ~ImageProcessor();

        /* \brief Process frame
         * @param[in] input_frame Current frame from image provider
         *
         * This is the implementation of AbstractImageProcessor's pure virtual method.
         * process gets called every time the ImageProvider asociated with it receives
         * a new image.
         *
         */
        void process( cv::Mat &input_frame );

        /* \brief Setter for mode enum.
         * @param[in] m New mode
         *
         */
        void setMode( Mode m );

    public slots:
        /* \brief Setter for general settings object
         * @param[in] s New settings object
         *
         */
        void setGeneralSettings( GeneralSettings s );

        /* \brief Setter for hole settings object
         * @param[in] s New settings object
         *
         */
        void setHoleSettings( HoleSettings s );

   private:
        /* \brief Check if an image contains a EAN13 barcode
         * @param[in] cvframe Image in wich to look for the barcode
         *
         * The image gets converted to a zbar image and then run through a zbar scanner.
         * Signals are used to notify system about errors and found barcodes.
         *
         */
        void checkBarcode( cv::Mat &cvframe );

        /* \brief Process image.
         * @param[in] thresh_frame A binary image containing the profile
         * @param[in] input_frame A grayscale image used to create the output image.
         * @param[in] gs General settings object
         * @param[in] hs Hole settings object
         *
         * Detect and measure holes, paint hole info on the output image.
         *
         */
        void processNormal(const cv::Mat &thresh_frame, const cv::Mat &input_frame, GeneralSettings gs , HoleSettings hs);

        /* \brief Measure distance between holes and paint lines between hole centerpoints.
         * @param[in] holes. A vector containting rows of points describing hole centerpoints.
         * @param[in][out] outFrame. The image to paint onto.
         * @param[in] max_cc_deviation. Threshold for missing hole alarm.
         *
         * \return True if no holes are missing
         *
         */
        bool checkDrawHoles(QVector<QVector<cv::Point>> &holes, cv::Mat &outFrame , const double max_cc_deviation);

        /* \brief Find profile blob in image
         * @param[in] threshFrame A binary image.
         * @param[in] s General settings object containing min and max area for filtering the blobs.
         *
         * \return The largest blob in image.
         *
         */
        mcv::Blob findProfile( const cv::Mat1b &threshFrame, GeneralSettings s );

        /* Find hole centerpoints
         * @param[in] profile. A blob representing the profile.
         * @param[in][out] outFrame. Image to paint centerpoints onto.
         * @param[in] s Hole settings object.
         *
         * Iterates over all holes in the profile blob. Splits the holes into separate vectors representing
         * rows of holes. And then paints the hole centerpoints onto an output image.
         *
         * \return A vector containing the rows of points.
         *
         */
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
