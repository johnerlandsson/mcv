#include "point_operations.h"

namespace mcv
{

cv::Point meanPoint( const std::vector<cv::Point> &points )
{
	cv::Point ret;

	for( auto p : points )
		ret += p;

	ret.x /= points.size();
	ret.y /= points.size();

	return ret;
}

/* meanPointDistance
 * returns the mean distance between points in their current order.
 *
 * points - Vector of points to evaluate
 */
cv::Point2f meanPointDistance( const std::vector<cv::Point> &points )
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

} //namespace mcv



