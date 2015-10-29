/*
 * ImageFileLoader.cpp
 *
 *  Created on: 27 Oct 2015
 *      Author: john
 */

#include "ImageFileLoader.h"
#include <opencv2/opencv.hpp>

extern "C"
{
#include <dirent.h>
}

namespace mcv
{

ImageFileLoader::ImageFileLoader( std::string folder_path, int delay_ms ) : _delay{ delay_ms }, _path{ folder_path }, _run{ false }
{
}

ImageFileLoader::~ImageFileLoader()
{
}

void ImageFileLoader::open()
{
	_filenames.clear();

	if( _path.back() != '/' )
		_path.push_back( '/' );

	DIR *d = ::opendir( _path.c_str() );
	struct dirent *dir = nullptr;

	if( d )
	{
		while( (dir = ::readdir( d )) )
		{
			std::string filename{ dir->d_name };

			//Handle . and ..
			if( filename.size() < 3 )
				continue;

			//Extract extension part of filename
			auto pp = filename.end();
			for( auto c = pp; c != filename.begin(); c-- )
				if( *c == '.' )
					pp = c;
			if( pp == filename.begin() )
				continue;

			for( auto ext : _valid_file_extensions )
			{
				if( std::string( pp, filename.end() ) == ext )
				{
					_filenames.push_back( filename );
					break;
				}
			}
		}
		_current_filename = _filenames.begin();
		closedir( d );
	}
	else
	{
		throw std::runtime_error( "Failed to open dir: " + _path );
	}
}

void ImageFileLoader::close()
{
}

void ImageFileLoader::start()
{
	_run = true;
	caller();
}

void ImageFileLoader::stop()
{
	_run = false;
}

void ImageFileLoader::setExtensions( const std::initializer_list<std::string> ext )
{
	_valid_file_extensions = ext;
}

void ImageFileLoader::caller()
{
	//Delay
	std::this_thread::sleep_for( _delay );

	//Process
	auto f = next_frame();
	_process_function( f );


	//Call again
	if( _run )
	{
		std::thread t( std::bind( &ImageFileLoader::caller, this ) );
		t.detach();
	}
//	if( _run )
//		std::async( std::launch::async, &ImageFileLoader::caller, this );
}

cv::Mat ImageFileLoader::next_frame()
{
	if( _current_filename >= _filenames.end() )
		return cv::Mat();

	if( (++_current_filename) >= _filenames.end() )
		_current_filename = _filenames.begin();

	return cv::imread( _path + *_current_filename, CV_LOAD_IMAGE_UNCHANGED );
}

} /* namespace mcv */
