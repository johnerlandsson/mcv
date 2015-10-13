/*
 * AbstractImageProcessor.cpp
 *
 *  Created on: 12 Oct 2015
 *      Author: john
 */

#include "AbstractImageProcessor.h"

namespace mcv {

AbstractImageProcessor::AbstractImageProcessor()
{
}

AbstractImageProcessor::~AbstractImageProcessor()
{
}

cv::Mat AbstractImageProcessor::outputFrame()
{
	std::lock_guard<std::mutex> lock( _output_frame_mutex );
	return _output_frame;
}

void AbstractImageProcessor::setOutputFrame( const cv::Mat& frame )
{
	std::lock_guard<std::mutex> lock( _output_frame_mutex );
	frame.copyTo( _output_frame );
}

} /* namespace mcv */
