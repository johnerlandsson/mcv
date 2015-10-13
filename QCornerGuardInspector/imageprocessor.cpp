#include "imageprocessor.h"

ImageProcessor::ImageProcessor( QObject *parent ) : QObject( parent )
{

}

ImageProcessor::~ImageProcessor()
{

}

void ImageProcessor::process( cv::Mat &input_frame )
{
    setOutputFrame( input_frame );
}
