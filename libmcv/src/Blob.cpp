/*
 * Blob.cpp
 *
 *  Created on: Apr 9, 2015
 *      Author: john
 */

#include "Blob.h"
#include <iostream>
#include "contour_operations.h"

namespace mcv
{

/* Constructor
 *
 */
Blob::Blob() : _valid{ false }, _bb{ nullptr }, _br{ nullptr }, _a{ nullptr }
{
}

/* Copy constructor
 *
 */
Blob::Blob( const Blob &other ) : _contours{ other._contours },
								  _valid{ other._valid }
{
	if( other._bb )
		_bb = std::unique_ptr<cv::Rect>( new cv::Rect( *other._bb ) );
	else
		_bb = std::unique_ptr<cv::Rect>( nullptr );

	if( other._br )
		_br = std::unique_ptr<cv::RotatedRect>( new cv::RotatedRect( *other._br ) );
	else
		_br = std::unique_ptr<cv::RotatedRect>( nullptr );

	if( other._a )
		_a = std::unique_ptr<double>( new double( *other._a ) );
	else
		_a = std::unique_ptr<double>( nullptr );
}

Blob::Blob( Blob &&other ) : _contours{ std::move( other._contours ) },
							  _valid{ std::move( other._valid ) }
{
	if( other._bb )
		_bb = std::move( other._bb );
	else
		_bb = std::unique_ptr<cv::Rect>( nullptr );

	if( other._br )
		_br = std::move( other._br );
	else
		_br = std::unique_ptr<cv::RotatedRect>( nullptr );

	if( other._a )
		std::move( other._a );
	else
		_a = std::unique_ptr<double>( nullptr );
}

/* Destructor
 *
 */
Blob::~Blob()
{
	_bb.reset();
	_br.reset();
	_a.reset();
}

/* copy assignment
 *
 */
Blob &Blob::operator=( const Blob &other )
{
	assert( &other != this );

	if( other._bb )
		_bb = std::unique_ptr<cv::Rect>( new cv::Rect( *other._bb ) );
	else
		_bb = std::unique_ptr<cv::Rect>( nullptr );

	if( other._br )
		_br = std::unique_ptr<cv::RotatedRect>( new cv::RotatedRect( *other._br ) );
	else
		_br = std::unique_ptr<cv::RotatedRect>( nullptr );

	if( other._a )
		_a = std::unique_ptr<double>( new double( *other._a ) );
	else
		_a = std::unique_ptr<double>( nullptr );

	_contours = ContourArray( other._contours );

	_valid = other._valid;

	return *this;
}

/* move assignment
 *
 */
Blob &Blob::operator=( Blob &&other )
{
	assert( &other != this );

	if( other._bb )
		_bb = std::move( other._bb );
	else
		_bb = std::unique_ptr<cv::Rect>( nullptr );

	if( other._br )
		_br = std::move( other._br );
	else
		_br = std::unique_ptr<cv::RotatedRect>( nullptr );

	if( other._a )
		_a = std::move( other._a );
	else
		_a = std::unique_ptr<double>( nullptr );

	_contours = std::move( other._contours );

	_valid = other._valid;

	return *this;
}

/* splitMatIntoBlobs
 */
std::vector<Blob> Blob::splitMatIntoBlobs( const cv::Mat1b in, int minBlobArea, int minHoleArea, cv::Rect roi )
{
	std::vector<Blob> ret;

	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;

	cv::Mat contImg;
	bool use_roi = !(roi.width == 0 && roi.height == 0);

	if( !use_roi )
		contImg = in;
	else
		contImg = cv::Mat( in, roi );

	cv::findContours( contImg, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE, cv::Point() );

	if( contours.size() == hierarchy.size() && contours.size() > 0 )
	{
		for( size_t i = 0; i < contours.size(); i++ )
		{
			if( hierarchy[i][3] == -1 )
			{
				if( minBlobArea > 0 && cv::contourArea( contours[i], false ) < minBlobArea )
					continue;

				Blob nextBlob;

				if( use_roi )
					nextBlob._contours.push_back( offsetContour( contours[i], roi.tl() ) );
				else
					nextBlob._contours.push_back( contours[i] );

				for( size_t j = 0; j < hierarchy.size(); j++ )
				{
					if( hierarchy[j][3] == (int)i )
					{
						if( minHoleArea > 0 && contourArea( contours[j] ) < minHoleArea )
							continue;

						nextBlob._contours.push_back( contours[j] );
					}
				}

				nextBlob._valid = true;
				ret.push_back( nextBlob );
			}
		}
	}

	return ret;
}

/* toMat
 */
cv::Mat1b Blob::toMat( cv::Size sz, cv::Point offset ) const
{
	cv::Mat1b ret = cv::Mat1b::zeros( sz );

	if( _contours.size() <= 0 )
		return ret;

	cv::Scalar color( 255 );	//Blob color
	for( int i = 0; i < (int)_contours.size(); i++ )
	{
		if( i > 0 ) //Hole color
			color = cv::Scalar( 0 );

		cv::drawContours( ret, _contours, i, color, CV_FILLED, 8, cv::noArray(), 0, offset );
	}

	return ret;
}

/* toMat
 * Draw the blob on a Mat of the smallest size possible
 */
cv::Mat1b Blob::toMat() const
{
	cv::Point offset = cv::Point() - topLeft();
	return toMat( boundingBox().size(), offset );
}

/* area
 * Returns the blob's area
 */
double Blob::area() const
{
	if( _a )
	{
		double a = *_a.get();
		return a;
	}

	if( _contours.size() <= 0 )
	{
		_a = std::unique_ptr<double>( new double( 0.0f ) );
	}
	else
	{
		double holeArea = 0;

		for( auto c = _contours.begin() + 1; c != _contours.end(); c++ )
			holeArea += cv::contourArea( *c );

		double a = cv::contourArea( _contours[OUTER_CONTOUR_POS] ) - holeArea;

		_a = std::unique_ptr<double>( new double( a ) );
	}

	return *_a.get();
}

/* boundingBox
 * Find the top-left and bottom-right points and return a rectangle that encloses those points
 */
cv::Rect Blob::boundingBox() const
{
	if( _bb.get() != nullptr )
		return *_bb;

	if( _contours.size() <= 0 )
		_bb = std::unique_ptr<cv::Rect>( new cv::Rect );
	else
		_bb = std::unique_ptr<cv::Rect>( new cv::Rect( contourBoundingBox( _contours[0] ) ) );

	return *_bb;
}

/* boundingRect
 * Return the smallest rotated rect enclosing the outer contour
 */
cv::RotatedRect Blob::boundingRect() const
{
	if( _br.get() != nullptr )
		return *_br;

	if( _contours.size() <= 0 )
		_br = std::unique_ptr<cv::RotatedRect>();
	else
		_br = std::unique_ptr<cv::RotatedRect>( new cv::RotatedRect( cv::minAreaRect( _contours[0] ) ) );

	return *_br;
}

/* topLeft
 */
cv::Point Blob::topLeft() const
{
	if( _contours.size() <= 0 )
		return cv::Point();

	cv::Point ret( std::numeric_limits<int>::max(), std::numeric_limits<int>::max() );

	for( auto p : _contours[OUTER_CONTOUR_POS] )
	{
		if( p.x < ret.x )
			ret.x = p.x;
		if( p.y < ret.y )
			ret.y = p.y;
	}

	return ret;
}

/* size
 */
cv::Size Blob::size() const
{
	return boundingBox().size();
}

/* circularity
 */
double Blob::circularity() const
{
	if( _contours.size() <= 0 )
		return -1.0f;

	return contourCircularity( _contours[0] );
}

/* center
 */
cv::Point Blob::center() const
{
	if( _contours.size() <= 0 )
		return cv::Point();

	return contourMassCenter( _contours[0] );
}

/* holeCount
 */
size_t Blob::holeCount() const
{
	if( _contours.size() <= 1 )
		return 0;

	return _contours.size() - 1;
}

/* isValid
 */
bool Blob::isValid() const
{
	return _valid;
}

std::vector<cv::Point> Blob::outerContour() const
{
	if( _contours.size() <= 0 )
		return std::vector<cv::Point>();

	return _contours[0];
}
Blob::ContourArray Blob::contours() const
{
	return _contours;
}

Blob::ContourArray Blob::holes() const
{
	if( _contours.size() <= 1 )
		return ContourArray();

	return ContourArray( _contours.begin() + 1, _contours.end() );
}

void Blob::clear()
{
	_contours.clear();
	_bb.reset();
	_br.reset();
	_a.reset();
	_valid = false;
}

} //namespace
