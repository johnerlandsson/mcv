/*
 * AbstractImageProcessor.h
 *
 *  Created on: 12 Oct 2015
 *      Author: john
 */

#ifndef ABSTRACTIMAGEPROCESSOR_H_
#define ABSTRACTIMAGEPROCESSOR_H_

#include <opencv2/core/core.hpp>
#include <mutex>

namespace mcv {

class AbstractImageProcessor
{
	public:
		AbstractImageProcessor();
		virtual ~AbstractImageProcessor();
		virtual void process( cv::Mat &input_frame ) = 0;
		cv::Mat outputFrame();


	protected:
		void setOutputFrame( const cv::Mat &frame );

		cv::Mat _output_frame;
		std::mutex _output_frame_mutex;
};

} /* namespace mcv */

#endif /* ABSTRACTIMAGEPROCESSOR_H_ */
