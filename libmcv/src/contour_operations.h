/*
 * contour_operations.h
 *
 *  Created on: Apr 27, 2015
 *      Author: john
 */

#ifndef SRC_CONTOUR_OPERATIONS_H_
#define SRC_CONTOUR_OPERATIONS_H_

#include <opencv2/core/core.hpp>

namespace mcv
{
/*! \brief calculate mass center of a contour
 * @param[in] contour The contour
 * \return A point representing the contours mass center
 *
 * This is calculated by finding the average point of the contour.
 */
cv::Point contourMassCenter( const std::vector<cv::Point> &contour );

/*! \brief Move an entire contour on the coordinate system
 * @param[in] in The contour
 * @param[in] offs The offset point
 * \return A copy of the newly offset contour
 */
std::vector<cv::Point> offsetContour( const std::vector<cv::Point> &in, const cv::Point offs );

/*! \brief Calculate a contours circularity
 * @param[in] contour The contour
 * \return Circularity value.
 *
 * Compares contours area to the area of a perfect circle contained in contours bounding box.
 * 1.0f is a perfect circle
 */
double contourCircularity( const std::vector<cv::Point> contour );

/*! \brief Find the smallest rectangle that can contain a contour
 * @param[in] contour The contour
 * \return A rectangle
 *
 */
cv::Rect contourBoundingBox( const std::vector<cv::Point> contour );

} //namespace mcv

#endif /* SRC_CONTOUR_OPERATIONS_H_ */
