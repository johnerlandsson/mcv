/*
 * AbstractImageProvider.h
 *
 *  Created on: 12 Oct 2015
 *      Author: john
 */

#ifndef ABSTRACTIMAGEPROVIDER_H_
#define ABSTRACTIMAGEPROVIDER_H_

#include <opencv2/core/core.hpp>
#include <functional>
#include "AbstractImageProcessor.h"

namespace mcv
{

typedef std::function<void( cv::Mat & )> ProcessFunction;

class AbstractImageProvider
{
	public:
		AbstractImageProvider();
		virtual ~AbstractImageProvider();
		virtual void open() = 0;
		virtual void close() = 0;
		virtual void start() = 0;
		virtual void stop() = 0;
		void setProcessFunction( ProcessFunction f );
		void setProcessFunction( AbstractImageProcessor *p );

	protected:
		ProcessFunction _process_function;

	private:
};

} /* namespace mcv */

#endif /* ABSTRACTIMAGEPROVIDER_H_ */
