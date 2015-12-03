#include "ICubeCamera.h"
#include <NETUSBCAM_API.h>
#include <iostream>
#include "ICubeCameraError.h"

namespace mcv
{
ICubeCamera::ICubeCamera() : _cam_index{ 0 }, _is_open{ false }
{

}

ICubeCamera::ICubeCamera( unsigned int index ) : _cam_index{ index }, _is_open{ false }
{

}

ICubeCamera::~ICubeCamera()
{
	close();

	int rv = NETUSBCAM_Destroy( _cam_index );
	if( rv != ICubeCameraError::ICSuccess )
		throw ICubeCameraError( "Failed to destroy camera-interface.", rv );
}

void ICubeCamera::init()
{
	if( (NETUSBCAM_Init()) < 1 )
		throw NoCamerasConnected();
}

void ICubeCamera::setup()
{
	int rv = NETUSBCAM_SetCallback( _cam_index, CALLBACK_RAW, &ICubeCamera::cb, this );
	if( rv != ICubeCameraError::ICSuccess )
		throw ICubeCameraError( "Failed to set callback function.", rv );
}

void ICubeCamera::open()
{
	int rv = NETUSBCAM_Open( _cam_index );
	if( rv != ICubeCameraError::ICSuccess )
		throw ICubeCameraError( "Failed to open camera interface.", rv );

}

void ICubeCamera::close()
{
	if( isOpen() )
	{
		int rv = NETUSBCAM_Close( _cam_index );
		if( rv != ICubeCameraError::ICSuccess )
			throw ICubeCameraError( "Failed to close camera-interface.", rv );
	}
}

void ICubeCamera::start()
{
	//Read image size from camera. Needed by callback function.
	_sz = imageSize();

	int rv = NETUSBCAM_Start( _cam_index );
	if( rv != ICubeCameraError::ICSuccess )
		throw ICubeCameraError( "Failed to start capture.", rv );
}

void ICubeCamera::stop()
{
	int rv = NETUSBCAM_Stop( _cam_index );
	if( rv != ICubeCameraError::ICSuccess )
		throw ICubeCameraError( "Failed to stop capture.", rv );
}

//TODO color camera
int ICubeCamera::cb( void *buffer, unsigned int lBufferSize, void *this_ptr )
{
	ICubeCamera *instance = static_cast<ICubeCamera *>(this_ptr);
	cv::Mat frame( instance->_sz, CV_8UC1, buffer, instance->_sz.width );

	if( frame.data )
		instance->_process_function( frame );
//		instance->_pa->exec( frame );

	return 0;
}

void ICubeCamera::setExposure( float value )
{
	int rv = NETUSBCAM_SetExposure( _cam_index, value );
	if( rv != ICubeCameraError::ICSuccess )
		throw ICubeCameraError( "Failed set exposure time.", rv );
}

float ICubeCamera::exposure() const
{
	float ret = -1.0f;
	int rv = NETUSBCAM_GetExposure( _cam_index, &ret );
	if( rv != ICubeCameraError::ICSuccess )
		throw ICubeCameraError( "Failed set exposure time.", rv );

	return ret;
}

cv::Size ICubeCamera::imageSize() const
{
	cv::Size ret;
	int rv = NETUSBCAM_GetSize( _cam_index, &ret.width, &ret.height );
	if( rv != ICubeCameraError::ICSuccess )
		throw ICubeCameraError( "Failed read image dimensions.", rv );

	return ret;
}

void ICubeCamera::setROI( cv::Rect roi )
{
	int rv = NETUSBCAM_SetResolution( _cam_index, roi.width, roi.height, roi.x, roi.y );
	if( rv != ICubeCameraError::ICSuccess )
		throw ICubeCameraError( "Failed to set region of interest.", rv );
}


cv::Rect ICubeCamera::roi() const
{
	cv::Rect ret;
	int rv = NETUSBCAM_GetResolution( _cam_index, &ret.width, &ret.height, &ret.x, &ret.y );
	if( rv != ICubeCameraError::ICSuccess )
		throw ICubeCameraError( "Failed to read region of interest from camera.", rv );

	return ret;
}

void ICubeCamera::setMode( enum ImageModes mode )
{
	int rv = NETUSBCAM_SetMode( _cam_index, (unsigned int)mode );
	if( rv != ICubeCameraError::ICSuccess )
		throw ICubeCameraError( "Failed to set mode.", rv );
}

cv::Size ICubeCamera::mode() const
{
	unsigned int mode;
	int rv = NETUSBCAM_GetMode( _cam_index, &mode );
	if( rv != ICubeCameraError::ICSuccess )
		throw ICubeCameraError( "Failed to read mode.", rv );

	return mode2size( static_cast<ImageModes>(mode) );
}
std::vector<cv::Size> ICubeCamera::modeList() const
{
	unsigned int *list = 0;
	unsigned int sz = 0;
	int rv = NETUSBCAM_GetModeList( _cam_index, list, &sz );
	if( rv != ICubeCameraError::ICSuccess )
		throw ICubeCameraError( "Failed to read mode list.", rv );

	std::vector<cv::Size> ret;
	for( unsigned int i = 0; i < sz; i++ )
		ret.push_back( mode2size( static_cast<ImageModes>(list[i]) ) );

	return ret;
}

std::string ICubeCamera::name() const
{
	char *ret = 0;
	unsigned int sz = 128;

	int rv = NETUSBCAM_GetName( _cam_index, ret, sz );
	if( rv != ICubeCameraError::ICSuccess )
		throw ICubeCameraError( "Failed read camera name.", rv );

	return std::string( ret );
}

int ICubeCamera::brokenFrames() const
{
	unsigned int ret = -1;
	int rv = NETUSBCAM_GetBrokenFrames( _cam_index, &ret );
	if( rv != ICubeCameraError::ICSuccess )
		throw ICubeCameraError( "Failed read broken frame count.", rv );

	return ret;
}

int ICubeCamera::goodFrames() const
{
	unsigned int ret = -1;
	int rv = NETUSBCAM_GetGoodFrames( _cam_index, &ret );
	if( rv != ICubeCameraError::ICSuccess )
		throw ICubeCameraError( "Failed read good frame count.", rv );

	return ret;
}

std::string ICubeCamera::apiVersion() const
{
	char *ret = 0;
	NETUSBCAM_GetApiVersion( ret, 128 );

	return std::string( ret );
}

std::string ICubeCamera::firmwareVersion() const
{
	char *ret = 0;
	int rv = NETUSBCAM_GetFWVersion( _cam_index, ret, 128 );
	if( rv != ICubeCameraError::ICSuccess )
		throw ICubeCameraError( "Failed read firmware version.", rv );

	return std::string( ret );
}

std::string ICubeCamera::serialNumber() const
{
	char *ret = 0;
	int rv = NETUSBCAM_GetSerialNum( _cam_index, ret, 128 );
	if( rv != ICubeCameraError::ICSuccess )
		throw ICubeCameraError( "Failed read serial number.", rv );

	return std::string( ret );
}

void ICubeCamera::setTriggerMode( const TriggerModes mode )
{
	int rv = NETUSBCAM_SetTrigger( _cam_index, (int)mode );
	if( rv != ICubeCameraError::ICSuccess )
		throw ICubeCameraError( "Failed to set trigger mode.", rv );
}

ICubeCamera::TriggerModes ICubeCamera::triggerMode() const
{
	int ret;
	int rv = NETUSBCAM_GetTrigger( _cam_index, &ret );
	if( rv != ICubeCameraError::ICSuccess )
		throw ICubeCameraError( "Failed to read trigger mode.", rv );

	return static_cast<TriggerModes>(ret);
}

bool ICubeCamera::isOpen()
{
	char dummy[20];
	int rv = NETUSBCAM_GetName( _cam_index, (char *)&dummy, 20 );

	if( rv == ICubeCameraError::CameraInterfaceIsNotOpen )
		return false;

	return true;
}

cv::Size ICubeCamera::mode2size( const ImageModes mode ) const
{
	switch( mode )
	{
		case Mode320x240:
			return cv::Size( 320, 240 );
		case Mode640x480:
			return cv::Size( 640, 480 );
		case Mode752x480:
			return cv::Size( 752, 480 );
		case Mode800x600:
			return cv::Size( 800, 600 );
		case Mode1024x768:
			return cv::Size( 1024, 768 );
		case Mode1280x1024:
			return cv::Size( 1280, 1024 );
		case Mode1600x1200:
			return cv::Size( 1600, 1200 );
		case Mode2048x1536:
			return cv::Size( 2048, 1536 );
		case Mode2592x1944:
			return cv::Size( 2592, 1944 );
		case Mode3840x2748:
			return cv::Size( 3840, 2748 );
		default:
			break;
	}

	return cv::Size();
}

} //namespace mcv
