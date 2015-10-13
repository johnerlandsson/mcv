#include "contour_operations.h"
#include <opencv2/imgproc/imgproc.hpp>

namespace mcv
{

cv::Point contourMassCenter( const std::vector<cv::Point> &contour )
{
	long avgX = 0;
	long avgY = 0;

	for( auto p : contour )
	{
		avgX += p.x;
		avgY += p.y;
	}

	return cv::Point( avgX / contour.size(), avgY / contour.size() );
}

std::vector<cv::Point> offsetContour( const std::vector<cv::Point> &in, const cv::Point offs )
{
	std::vector<cv::Point> ret;

	for( auto p : in )
		ret.push_back( cv::Point( p.x + offs.x, p.y + offs.y ) );

	return ret;
}

double contourCircularity( const std::vector<cv::Point> contour )
{
	cv::Rect bb = contourBoundingBox( contour );
	double r = std::min<int>( bb.width, bb.height ) / 2.0f;
	double a = M_PI * r * r;

	return cv::contourArea( contour ) / a;
}

cv::Rect contourBoundingBox( const std::vector<cv::Point> contour )
{
	cv::Point tl( std::numeric_limits<int>::max(), std::numeric_limits<int>::max() );
	cv::Point br( 0, 0 );

	for( auto p : contour )
	{
		if( p.x < tl.x )
			tl.x = p.x;
		if( p.y < tl.y )
			tl.y = p.y;
		if( p.x > br.x )
			br.x = p.x;
		if( p.y > br.y )
			br.y = p.y;
	}

	return cv::Rect( tl, br );
}
} //namespace mcv
