/*
 * point_operations.h
 *
 *  Created on: May 13, 2015
 *      Author: john
 */

#ifndef SRC_POINT_OPERATIONS_H_
#define SRC_POINT_OPERATIONS_H_

#include <opencv2/core/core.hpp>

namespace mcv
{
	/*! \brief Calculate the mean of a vector of points
	 * @param[in] points Vector of points
	 * \return Mean point
	 */
	cv::Point meanPoint( const std::vector<cv::Point> &points );


	/*! \brief Calculate the mean distance between points in a vector.
	 * @param[in] points Vector of points
	 * \return A point containing mean x and y distances
	 *
	 * This function iterates a vector, adds up the distance from previous point to the next and calculates the mean.
	 * This means that it only calculates the distance in the order that they were in when passed to the function.
	 */
	cv::Point2f meanPointDistance( const std::vector<cv::Point> &points );
} //namespace mcv



#endif /* SRC_POINT_OPERATIONS_H_ */
