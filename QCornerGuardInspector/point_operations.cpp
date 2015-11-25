#include "point_operations.h"


cv::Point2f meanPointDistance( const QVector<cv::Point> &points )
{
    if( points.size() < 2 )
        return cv::Point();

    cv::Point ret;

    for( auto p = points.begin() + 1; p != points.end(); p++ )
    {
        cv::Point dist = *p - *(p - 1);
        if( dist.x < 0 )
            dist.x *= -1;
        if( dist.y < 0 )
            dist.y *= -1;

        ret += dist;
    }

    ret.x /= points.size();
    ret.y /= points.size();

    return ret;
}

