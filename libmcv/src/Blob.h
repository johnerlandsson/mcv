/*
 * Blob.h
 *
 *  Created on: Apr 9, 2015
 *      Author: john
 */

#ifndef BLOB_H_
#define BLOB_H_

#include <opencv2/opencv.hpp>
#include <memory>

namespace mcv
{

#define OUTER_CONTOUR_POS 0

class Blob
{
	public:
		typedef std::vector<cv::Point> Contour;
		typedef std::vector<Contour> ContourArray;

		/*! \brief Default constructor
		 *
		 */
		Blob();

		/*! \brief Copy constructor
		 *
		 */
		Blob( const Blob &other );

		/*! \brief Move constructor
		 *
		 */
		Blob( Blob &&other );

		/*! \brief Assignment operator
		 *
		 */
		virtual ~Blob();

		/*! \brief Destructor
		 *
		 */
		Blob &operator=( const Blob &other );

		/*! \brief Move assignment operator
		 *
		 */
		Blob &operator=( Blob &&other );

		/*! \brief Converts the connected pixels in a binary image into separate blob objects.
		 * @param[in] in Binary image to find blobs in.
		 * @param[in] minBlobArea Blobs with an area smaller than this value gets ignored. Use -1 to ignore this parameter
		 * @param[in] minHoleArea Holes with an area smaller than this value gets ignored. Use -1 to ignore this parameter
		 * @param[in] roi Region of interest. Use cv::Rect() to ignore this parameter
		 *
		 * \return A vector of blob objects
		 */
		static std::vector<Blob> splitMatIntoBlobs( const cv::Mat1b in, int minBlobArea = -1, int minHoleArea = -1, cv::Rect roi = cv::Rect() );

		/*! \brief Converts the blob into a cv::Mat of specific size
		 * @param[in] sz Size of output image
		 * @param[in] offset Point to offset the blob with in output image.
		 *
		 * \return A binary image of the given size
		 */
		cv::Mat1b toMat( cv::Size sz, cv::Point offset = cv::Point() ) const;

		/*! \brief Converts the blob into a cv::Mat of the smallest possible size
		 * \return A binary image of the smallest possible size
		 */
		cv::Mat1b toMat() const;

		/*! \brief Calculate the blob area in pixels
		 * \return Contour area in pixels
		 *
		 * This method calculates the contourArea() of blobs outer contour, stores the value and returns.
		 * This way no calculation is needed the next time this method is called.
		 */
		double area() const;

		/*! \brief Find the smallest possible cv::Rect surrounding the blob
		 * \return Bounding rectangle
		 *
		 * This method calculates the contourBoundingBox() of blobs outer contour, stores the value and returns.
		 * This way no calculation is needed the next time this method is called.
		 */
		cv::Rect boundingBox() const;

		/*! \brief Find the smallest possible cv::RotatedRect surrounding the blob
		 * \return Bounding rotated rectangle
		 *
		 * This method calculates the cv::minAreaRect() of blobs outer contour, stores the value and returns.
		 * This way no calculation is needed the next time this method is called.
		 */
		cv::RotatedRect boundingRect() const;

		/*! \brief Finds the top left point of blobs bounding box.
		 * \return Top left point
		 */
		cv::Point topLeft() const;

		/*! \brief Finds the dimensions of blobs bounding box.
		 * \return Dimensions of blobs bounding box.
		 */
		cv::Size size() const;

		/*! \brief Calculate the circularity of blobs outer contour
		 * \return Circularity where 1.0 is a perfect circle
		 */
		double circularity() const;

		/*! \brief Calculate mass center of blobs outer contour.
		 * \return Mass center
		 */
		cv::Point center() const;

		/*! \brief Find the number of holes contained i the blob
		 * \return Number of holes
		 */
		size_t holeCount() const;

		/*! \brief Check if a valid blob has been created.
		 * \return true if valid
		 */
		bool isValid() const;

		/*! \brief Getter for blobs outer contour.
		 * \return A copy of blobs outer contour
		 */
		std::vector<cv::Point> outerContour() const;

		/*! \brief Getter for contour vector
		 * \return A vector of contours. [0] Is always outer contour.
		 */
		ContourArray contours() const;

		/*! \brief Getter for hole vector
		 * \return A vector of contours.
		 */
		ContourArray holes() const;

		/*! \brief Clears all members.
		 *
		 * Calls clear on contour vector, reset on all unique_ptr objects and sets valid flag to false
		 */
		void clear();

	private:
		ContourArray _contours; /*!< Vector holding the contours. First index is always outer contour */
		bool _valid; /*!< Holds information weather the blob is valid or not. Gets set by splitMatIntoBlobs */
		mutable std::unique_ptr<cv::Rect> _bb; /*!< Holds bounding box object if boundingBox() has been called*/
		mutable std::unique_ptr<cv::RotatedRect> _br; /*!< Holds bounding rectangle object if boundingRectangle() has been called*/
		mutable std::unique_ptr<double> _a; /*!< Holds the blobs area if area() has been called*/
};

} //namespace

#endif /* BLOB_H_ */
