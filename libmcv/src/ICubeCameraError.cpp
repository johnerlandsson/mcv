/*
 * 
 * ICubeCameraError.cpp
 *
 *  Created on: Jun 17, 2015
 *      Author: john
 */

#include "ICubeCameraError.h"

namespace mcv
{
ICubeCameraError::ICubeCameraError( const std::string what, const int err_code ) : std::runtime_error( "ICubeCamera error: " + what ), _err_code{ err_code }
{
}

const char *ICubeCameraError::what() const throw()
{
	std::string ret = std::string( std::runtime_error::what() ) + " Error message: " + errmsg( _err_code );

	return ret.c_str();
}

std::string ICubeCameraError::errmsg( const int err_code ) const
{
	switch( err_code )
	{
	case ErrorCodes::ICSuccess:
		return "Success.";
	case ErrorCodes::CameraInterfaceIsNotOpen:
		return "Camera-interface is not open.";
	case ErrorCodes::ParameterOutOfRange:
		return "Parameter is out of range.";
	case ErrorCodes::OutOfMemory:
		return "Out of memory.";
	case ErrorCodes::AllreadyDone:
		return "Already done.";
	case ErrorCodes::WrongPLLValue:
		return "Wrong PLL value.";
	case ErrorCodes::AVIInitFailureCom:
		return "AVI init failure: COM_LIB_INIT.";
	case ErrorCodes::AVIInitFailureIF:
		return "AVI init failure: NOT_IF_STARTED.";
	case ErrorCodes::WrongROIID:
		return "Wrong ROI id number.";
	case ErrorCodes::ParameterNotEnabled:
		return "Parameter not enabled.";
	case ErrorCodes::OnlyForColor:
		return "Parameter is only for color cameras.";
	case ErrorCodes::VersionMissmatch:
		return "Version mismatch.";
	default:
		break;
	}

	return "Undefined error.";
}
} /* namespace mcv */
