/*
 * ICubeCamera.h
 *
 *  Created on: May 20, 2015
 *      Author: john
 */

#ifndef ICUBECAMERA_H_
#define ICUBECAMERA_H_

#include <mutex>
#include <stdexcept>

#include "AbstractImageProvider.h"

namespace mcv
{
class ICubeCamera : public AbstractImageProvider
{
public:
	enum class NoCamerasConnected{};
	enum ImageModes
	{
		Mode320x240 = 0,	//QVGA
		Mode640x480 = 1,	//VGA
		Mode752x480 = 2,	//WVGA
		Mode800x600 = 3,	//SVGA
		Mode1024x768 = 4,	//XGA
		Mode1280x1024 = 5,	//SXGA
		Mode1600x1200 = 6,	//UXGA
		Mode2048x1536 = 7,	//QXGA
		Mode2592x1944 = 8,	//QSXGA
		Mode3840x2748 = 9	//WQUXGA
	};

	enum BinSkipMode
	{
		ModeSkip = 0,
		ModeBin = 1
	};

	enum BinSkipNPixel
	{
		SkipOff = 0,
		Skip2NdPixel = 1,
		Skip4ThPixel = 2
	};

	enum TriggerModes
	{
		SwStart = 0,	//Start software trigger
		SwDo = 1,		//Do a software trigger pulse
		HwStart = 2,	//Start hardware trigger
		Stop = 3,		//Stop software || hardware trigger
		SwStart2 = 4,	//GlobalResetRelease shutter mode (1500CU/BU,1300CU)
		HwStart2 = 5	//GlobalResetRelease shutter mode (1500CU/BU,1300CU)
	};

	enum TriggerEdge
	{
		Falling = 0,
		Rising = 1
	};

	ICubeCamera();
	ICubeCamera( unsigned int index );
	ICubeCamera( const ICubeCamera & ) = default;
	ICubeCamera( ICubeCamera && ) = default;
	virtual ~ICubeCamera();
	ICubeCamera &operator=( const ICubeCamera & ) = default;
	ICubeCamera &operator=( ICubeCamera && ) = default;

	void init();
	void setup();
	void open();
	void close();
	void start();
	void stop();
	void setExposure( float value );
	float exposure() const;
	cv::Size imageSize() const;
	void setROI( cv::Rect roi );
	cv::Rect roi() const;
	void setMode( ImageModes mode );
	cv::Size mode() const;
	std::vector<cv::Size> modeList() const;
	std::string name() const;
	int brokenFrames() const;
	int goodFrames() const;
	std::string apiVersion() const;
	std::string firmwareVersion() const;
	std::string serialNumber() const;
	void setTriggerMode( const TriggerModes mode );
	TriggerModes triggerMode() const;
	bool isOpen();

private:
	static int cb( void *buffer, unsigned int lBufferSize, void *this_ptr );

	unsigned int _cam_index;
	cv::Size _sz;
	std::mutex _cf_mutex;
	cv::Size mode2size( const ImageModes mode ) const;
	bool _is_open;
};

} //namespace mcv
#endif /* ICUBECAMERA_H_ */
