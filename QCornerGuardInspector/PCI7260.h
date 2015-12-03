/*
 * PCI7260.h
 *
 *  Created on: 1 Sep 2015
 *      Author: john
 */

#ifdef USE_IO
#ifndef SRC_PCI7260_H_
#define SRC_PCI7260_H_

namespace pci_dask
{
extern "C"
{
#include <pci_dask/dask.h>
}
}

#include <stdexcept>

class PCI7260Error : public std::runtime_error
{
	public:
		PCI7260Error( int error, std::string msg );
		virtual const char *what() const throw();

	private:
		int _error;
		std::string _msg;
};

class PCI7260
{
	public:
	PCI7260( pci_dask::U8 device_index );
	virtual ~PCI7260();
	void init();
	void setOutputValue( pci_dask::U8 output, bool value );
	void setOutputsValue( pci_dask::U32 outputs );
	void writeOutputs();
	bool input( pci_dask::U8 input );
	pci_dask::U32 inputs();
	void updateInputs();
	void release();

	private:
	pci_dask::U8 _device_index;
	pci_dask::U32 _input_status;
	pci_dask::U32 _output_status;
};


#endif /* SRC_PCI7260_H_ */
#endif /*USE_IO*/
