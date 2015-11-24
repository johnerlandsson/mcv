#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <mcv/AbstractImageProcessor.h>
#include <QObject>
#include <zbar.h>

class ImageProcessor : public QObject, public mcv::AbstractImageProcessor
{
    Q_OBJECT
    public:
        ImageProcessor( QObject *parent = 0 );
        virtual ~ImageProcessor();
        void process( cv::Mat &input_frame );
        void checkBarcode( cv::Mat &cvframe );

   private:
        zbar::ImageScanner scanner;

    signals:
        void foundBarcode( const QString data );
};

#endif // IMAGEPROCESSOR_H
