/*
 * ImageFileLoader.h
 *
 *  Created on: 27 Oct 2015
 *      Author: john
 */

#ifndef IMAGEFILELOADER_H_
#define IMAGEFILELOADER_H_

#include "AbstractImageProvider.h"
#include <chrono>
#include <thread>

namespace mcv
{

class ImageFileLoader : public AbstractImageProvider
{
	public:
		ImageFileLoader( std::string folder_path, int delay_ms );
		virtual ~ImageFileLoader();
		void open();
		void close();
		void start();
		void stop();
		void setExtensions( const std::initializer_list<std::string> ext );

	private:
		std::chrono::milliseconds _delay;
		std::string _path;
		std::vector<std::string> _valid_file_extensions;
		std::vector<std::string> _filenames;
		std::vector<std::string>::iterator _current_filename;
		std::thread _caller_thread;
		bool _run;

		void caller();
		cv::Mat next_frame();
};

} /* namespace mcv */

#endif /* IMAGEFILELOADER_H_ */
