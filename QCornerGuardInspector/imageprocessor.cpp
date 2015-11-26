#include "imageprocessor.h"
#include <zbar.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <QtAlgorithms>
#include <point_operations.h>
#include <mcv/contour_operations.h>

ImageProcessor::ImageProcessor( QObject *parent ) : QObject( parent ), mode( Mode::Normal )
{
    if( (scanner.set_config( zbar::ZBAR_EAN13, zbar::ZBAR_CFG_ENABLE, 1 )) != 0 )
        throw std::runtime_error( "Failed to setup zbar image scanner" );
}

ImageProcessor::~ImageProcessor()
{

}

void ImageProcessor::process( cv::Mat &input_frame )
{
    //Copy settings for thread safety
    general_settings_mutex.lock();
    GeneralSettings gs = general_settings;
    general_settings_mutex.unlock();

    hole_settings_mutex.lock();
    HoleSettings hs = hole_settings;
    hole_settings_mutex.unlock();

    checkBarcode( input_frame );

    //Threshold image
    cv::Mat1b thresh;
    cv::threshold( input_frame, thresh, gs.threshold, 255, CV_THRESH_BINARY_INV );

    //Process image
    if( mode == Mode::Normal )
        processNormal( thresh, input_frame, gs, hs );
    else if( mode == Mode::Threshold )
        setOutputFrame( thresh );
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

void ImageProcessor::setMode( Mode m )
{
    mode = m;
}

void ImageProcessor::setHoleSettings( HoleSettings s )
{
    QMutexLocker lock( &general_settings_mutex );
    hole_settings = s;
}

void ImageProcessor::setGeneralSettings( GeneralSettings s )
{
    QMutexLocker lock( &hole_settings_mutex );
    general_settings = s;
}

    void ImageProcessor::processNormal( const cv::Mat &thresh_frame, const cv::Mat &input_frame, GeneralSettings gs, HoleSettings hs )
    {
        cv::Mat of;
        cv::cvtColor( input_frame, of, CV_GRAY2BGR );

        //Find largest blob in image
        auto profile = findProfile( thresh_frame, gs );

        //Locate the centerpoints of all holes in blob and draw points on output image
        auto sortedPoints = findDrawHoles( profile, of, hs );
        if( sortedPoints.size() <= 0 )
        {
            emit noHolesInProfile();
            setOutputFrame( of );
            return;
        }

        //Check distances and draw lines between holes
        if( !checkDrawHoles( sortedPoints, of, hs.max_cc_deviation ) )
    {
        emit missingHole();
        setOutputFrame( of );
        return;
    }

    setOutputFrame( of );
}

bool ImageProcessor::checkDrawHoles( QVector<QVector<cv::Point>> &holes, cv::Mat &outFrame, const double max_cc_deviation )
{
    double maxDist = 0;
    double avgDist = 0;

    for( auto r : holes )
    {
        qSort( r.begin(), r.end(), [](const cv::Point &a, const cv::Point &b) -> bool
        {
            return a.x > b.x;
        } );

        for( auto p = r.begin() + 1; p != r.end(); p++ )
        {
            double dist = sqrt( pow( (p->x - (p - 1)->x) + (p->y - (p - 1)->y), 2 ) );
            if( dist > maxDist )
                maxDist = dist;
            avgDist += dist;

            cv::Point p1 = *(p - 1);
            cv::Point p2 = *p;

            line( outFrame, p1, p2, cv::Scalar( 200, 255, 0 ), 2, 8 );
        }
        avgDist /= r.size();

        if( maxDist > avgDist * max_cc_deviation )
            return false;
    }

    return true;
}

mcv::Blob ImageProcessor::findProfile( const cv::Mat1b &threshFrame, GeneralSettings s )
{
    auto blobs = mcv::Blob::splitMatIntoBlobs( threshFrame, s.min_profile_area, s.min_profile_hole_area );

    if( blobs.size() <= 0 )
        return mcv::Blob();

    int maxArea = 0;
    unsigned int maxAreaIndex = 0;

    for( unsigned int i = 0; i < blobs.size(); i++ )
    {
        int current_area = blobs[i].area();
        if( current_area > maxArea )
        {
            maxArea = current_area;
            maxAreaIndex = i;
        }
    }

    return blobs[maxAreaIndex];
}

QVector<QVector<cv::Point> > ImageProcessor::findDrawHoles( const mcv::Blob &profile, cv::Mat outframe, HoleSettings s )
{
    auto holes = findHoles( profile, s );
    if( holes.size() <= 0 )
    {
        emit noHolesInProfile();
        return QVector<QVector<cv::Point>>();
    }

    QVector<QVector<cv::Point>> ret = splitHolesByRow( holes, s.max_y_dev );

    for( auto p : holes )
        cv::circle( outframe, p, 5, cv::Scalar( 0, 255, 10 ), -1 );

    return ret;
}

QVector<cv::Point> ImageProcessor::findHoles( const mcv::Blob &profile, HoleSettings s )
{
    QVector<cv::Point> ret;
    if( !profile.isValid() )
        return ret;

    auto holes = profile.holes();
    for( auto h : holes )
    {
        double circ = mcv::contourCircularity( h );
        if( circ < s.max_circularity && circ > s.min_circularity )
            ret.push_back( mcv::contourMassCenter( h ) );
    }

    return ret;
}

QVector<QVector<cv::Point>> ImageProcessor::splitHolesByRow( QVector<cv::Point> &points, const double max_y_dev )
{
    if( points.size() < 1 )
        return QVector<QVector<cv::Point>>();

    //Sort points by Y-axis
    qSort( points.begin(), points.end(), [](const cv::Point &a, const cv::Point &b) -> bool
    {
        return a.y > b.y;
    } );

    //Setup return vector
    QVector<QVector<cv::Point>> ret;
    ret.push_back( QVector<cv::Point>() );
    ret.back().push_back( *points.begin() );

    double mean_y_diff = meanPointDistance( points ).y;

    for( auto p = points.begin() + 1; p != points.end(); p++ )
    {
        double ydiff = (p - 1)->y - p->y ;
        if( ydiff >= mean_y_diff * max_y_dev )
            ret.push_back( QVector<cv::Point>() );

        ret.back().push_back( *p );
    }


    return ret;
}
