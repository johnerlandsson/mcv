/*
 * 
 * ICubeCameraError.h
 *
 *  Created on: Jun 17, 2015
 *      Author: john
 */

#ifndef ICUBECAMERAERROR_H_
#define ICUBECAMERAERROR_H_

#include <stdexcept>

namespace mcv
{

class ICubeCameraError : public std::runtime_error
{
public:
	enum ErrorCodes
	{
		ICSuccess = 0,
		CameraInterfaceIsNotOpen = -1,
		ParameterOutOfRange = -2,
		OutOfMemory = -3,
		AllreadyDone = -4,
		WrongPLLValue = -5,
		AVIInitFailureCom = -6,
		AVIInitFailureIF = -7,
		WrongROIID = -8,
		ParameterNotEnabled = -9,
		OnlyForColor = -10,
		VersionMissmatch = -11
	};

	ICubeCameraError( const std::string what, const int err_code );
	virtual const char *what() const throw();

private:
	std::string errmsg( const int err_code ) const;
	int _err_code;
};

} /* namespace mcv */

#endif /* ICUBECAMERAERROR_H_ */
