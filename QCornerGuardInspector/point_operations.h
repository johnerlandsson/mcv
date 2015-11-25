#ifndef POINT_OPERATIONS_H
#define POINT_OPERATIONS_H

#include <opencv2/core/core.hpp>
#include <QVector>

cv::Point2f meanPointDistance( const QVector<cv::Point> &points );

#endif // POINT_OPERATIONS_H

