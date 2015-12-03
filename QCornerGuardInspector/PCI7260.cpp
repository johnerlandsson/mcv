/*
 * PCI7260.cpp
 *
 *  Created on: 1 Sep 2015
 *      Author: john
 */

#ifdef USE_IO
#include "PCI7260.h"

PCI7260Error::PCI7260Error( int error, std::string msg ) : std::runtime_error{ msg }, _error{ error }, _msg{ msg }
{
}

const char *PCI7260Error::what() const throw()
{
	std::string errmsg;
	switch( _error )
	{
		case ErrorUnknownCardType:
			errmsg = "Unknown card type";
			break;
		case ErrorInvalidCardNumber:
			errmsg = "Invalid card number";
			break;
		case ErrorTooManyCardRegistered:
			errmsg = "Too many cards registered";
			break;
		case ErrorCardNotRegistered:
			errmsg = "Card not registered";
			break;
		case ErrorFuncNotSupport:
			errmsg = "Function is not supported";
			break;
		case ErrorInvalidIoChannel:
			errmsg = "Invalid IO channel";
			break;
		case ErrorInvalidAdRange:
			errmsg = "Invalid AD range";
			break;
		case ErrorContIoNotAllowed:
			errmsg = "Continuous IO not allowed";
			break;
		case ErrorDiffRangeNotSupport:
			errmsg = "Diff range not supported";
			break;
		case ErrorLastChannelNotZero:
			errmsg = "Last channel not zero";
			break;
		case ErrorChannelNotDescending:
			errmsg = "Channel not descending";
			break;
		case ErrorChannelNotAscending:
			errmsg = "Channel not ascending";
			break;
		case ErrorOpenDriverFailed:
			errmsg = "Failed to open driver";
			break;
		case ErrorOpenEventFailed:
			errmsg = "Open event failed";
			break;
		case ErrorTransferCountTooLarge:
			errmsg = "Transfer count too large";
			break;
		case ErrorNotDoubleBufferMode:
			errmsg = "Not in double buffer mode";
			break;
		case ErrorInvalidSampleRate:
			errmsg = "Invalid sample rate";
			break;
		case ErrorInvalidCounterMode:
			errmsg = "Invalid counter mode";
			break;
		case ErrorInvalidCounter:
			errmsg = "Invalid counter";
			break;
		case ErrorInvalidCounterState:
			errmsg = "Invalid counter state";
			break;
		case ErrorInvalidBinBcdParam:
			errmsg = "Invalid bin bcd parameter";
			break;
		case ErrorBadCardType:
			errmsg = "Bad card type";
			break;
		case ErrorInvalidDaRefVoltage:
			errmsg = "Invalid DA reference voltage";
			break;
		case ErrorAdTimeOut:
			errmsg = "AD timeout";
			break;
		case ErrorNoAsyncAI:
			errmsg = "No asynchronous analog input";
			break;
		case ErrorNoAsyncAO:
			errmsg = "No asynchronous analog output";
			break;
		case ErrorNoAsyncDI:
			errmsg = "No asynchronous digital input";
			break;
		case ErrorNoAsyncDO:
			errmsg = "No asynchronous digital output";
			break;
		case ErrorNotInputPort:
			errmsg = "Not an input port";
			break;
		case ErrorNotOutputPort:
			errmsg = "Not an output port";
			break;
		case ErrorInvalidDioPort:
			errmsg = "Invalid digital IO port";
			break;
		case ErrorInvalidDioLine:
			errmsg = "Invalid digital IO line";
			break;
		case ErrorContIoActive:
			errmsg = "Continuous IO active";
			break;
		case ErrorDblBufModeNotAllowed:
			errmsg = "Double buffer mode not allowed";
			break;
		case ErrorConfigFailed:
			errmsg = "Config failed";
			break;
		case ErrorInvalidPortDirection:
			errmsg = "Invalid port direction";
			break;
		case ErrorBeginThreadError:
			errmsg = "Begin thread error";
			break;
		case ErrorInvalidPortWidth:
			errmsg = "Invalid port width";
			break;
		case ErrorInvalidCtrSource:
			errmsg = "Invalid control source";
			break;
		case ErrorOpenFile:
			errmsg = "Error opening file";
			break;
		case ErrorAllocateMemory:
			errmsg = "Could not allocate memory";
			break;
		case ErrorDaVoltageOutOfRange:
			errmsg = "DA voltage out of range";
			break;
		case ErrorLockMemory:
			errmsg = "Error while locking memory";
			break;
		case ErrorDIODataWidthError:
			errmsg = "Digital IO data width error";
			break;
		case ErrorTaskCodeError:
			errmsg = "Task code error";
			break;
		case ErrortriggercountError:
			errmsg = "Trigger count error";
			break;
		case ErrorInvalidTriggerMode:
			errmsg = "Invalid trigger mode";
			break;
		case ErrorInvalidTriggerType:
			errmsg = "Invalid trigger type";
			break;
		case ErrorInvalidCounterValue:
			errmsg = "Invalid counter value";
			break;
		case ErrorInvalidEventHandle:
			errmsg = "Invalid event handle";
			break;
		case ErrorNoMessageAvailable:
			errmsg = "No message available";
			break;
		case ErrorEventMessgaeNotAdded:
			errmsg = "Event message not added";
			break;
		case ErrorCalibrationTimeOut:
			errmsg = "Calibration timeout";
			break;
		case ErrorUndefinedParameter:
			errmsg = "Undefined parameter";
			break;
		case ErrorInvalidBufferID:
			errmsg = "Invalid buffer ID";
			break;
		case ErrorInvalidSampledClock:
			errmsg = "Invalid sampled clock";
			break;
		case ErrorInvalisOperationMode:
			errmsg = "Invalid operation mode";
			break;
		default:
			errmsg = "Undefined";
			break;
	}

	return std::string( _msg + "\n" + errmsg ).c_str();
}

PCI7260::PCI7260( pci_dask::U8 device_index ) : _device_index{ device_index }, _input_status{ 0 }, _output_status{ 0 }
{
}

PCI7260::~PCI7260()
{
}

void PCI7260::init()
{
	if( (pci_dask::Register_Card( PCI_7260, _device_index )) < 0 )
		throw std::runtime_error( "Error while registering PCI7260." );
}

void PCI7260::setOutputValue( pci_dask::U8 output, bool value )
{
	if( value )
		_output_status |= 1 << output;
	else
		_output_status &= ~(1 << output);
}

void PCI7260::setOutputsValue( pci_dask::U32 outputs )
{
	_output_status = outputs;
}

void PCI7260::writeOutputs()
{
	int res = pci_dask::DO_WritePort( _device_index, 0, _output_status );
	if( res != NoError )
		throw PCI7260Error( res, "Error while writing outputs" );
}

bool PCI7260::input( pci_dask::U8 input )
{
	return (bool)(_input_status & (1 << input)) >> input;
}

pci_dask::U32 PCI7260::inputs()
{
	return _input_status;
}

void PCI7260::updateInputs()
{
	int res = pci_dask::DI_ReadPort( _device_index, 0, &_input_status );
	if( res != NoError )
		throw PCI7260Error( res, "Error while writing outputs" );
}

void PCI7260::release()
{
	setOutputsValue( 0 );
	writeOutputs();
	pci_dask::Release_Card( _device_index );
}
#endif
