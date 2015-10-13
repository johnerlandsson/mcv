/*
 * AbstractImageProvider.cpp
 *
 *  Created on: 12 Oct 2015
 *      Author: john
 */

#include "AbstractImageProvider.h"

namespace mcv
{

AbstractImageProvider::AbstractImageProvider()
{
}

AbstractImageProvider::~AbstractImageProvider()
{
}

void AbstractImageProvider::setProcessFunction( ProcessFunction f )
{
	_process_function = f;
}

void AbstractImageProvider::setProcessFunction( AbstractImageProcessor *p )
{
	if( p == nullptr )
		return;

	_process_function = std::bind( &AbstractImageProcessor::process, p, std::placeholders::_1 );
}

} /* namespace mcv */
