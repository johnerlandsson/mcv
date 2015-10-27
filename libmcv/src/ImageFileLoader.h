/*
 * ImageFileLoader.h
 *
 *  Created on: 27 Oct 2015
 *      Author: john
 */

#ifndef IMAGEFILELOADER_H_
#define IMAGEFILELOADER_H_

#include "AbstractImageProvider.h"

namespace mcv
{

class ImageFileLoader : public AbstractImageProvider
{
	public:
		ImageFileLoader();
		virtual ~ImageFileLoader();
};

} /* namespace mcv */

#endif /* IMAGEFILELOADER_H_ */
