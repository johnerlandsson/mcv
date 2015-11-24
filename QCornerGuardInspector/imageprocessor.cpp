#include "imageprocessor.h"
#include <zbar.h>

ImageProcessor::ImageProcessor( QObject *parent ) : QObject( parent )
{
    if( (scanner.set_config( zbar::ZBAR_EAN13, zbar::ZBAR_CFG_ENABLE, 1 )) != 0 )
        throw std::runtime_error( "Failed to setup zbar image scanner" );
}

ImageProcessor::~ImageProcessor()
{

}

void ImageProcessor::process( cv::Mat &input_frame )
{
    checkBarcode( input_frame );
    setOutputFrame( input_frame );
}

void ImageProcessor::checkBarcode( cv::Mat &cvframe )
{
    zbar::Image zbimg( cvframe.cols, cvframe.rows, "Y800", cvframe.data, cvframe.cols * cvframe.rows );

    int res = scanner.scan( zbimg );

    if( res == 0 )
        return;
    else if( res > 0 )
    {
        auto data = zbimg.symbol_begin()->get_data();
        emit foundBarcode( QString::fromStdString( data ) );
    }
}
