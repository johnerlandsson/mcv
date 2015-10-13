#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <mcv/AbstractImageProcessor.h>
#include <QObject>

class ImageProcessor : public QObject, public mcv::AbstractImageProcessor
{
    Q_OBJECT
    public:
        ImageProcessor( QObject *parent = 0 );
        virtual ~ImageProcessor();
        void process( cv::Mat &input_frame );

    signals:
        void foundValidBarcode();
};

#endif // IMAGEPROCESSOR_H
