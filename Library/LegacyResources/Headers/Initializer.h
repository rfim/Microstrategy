//==============================================================================================
//	FILENAME	:	Initializer.h
//	AUTHOR		:	Fangyao Ma
//	CREATION	:	2008-05-15
//	Copyright (C) MicroStrategy Incorporated 2008
//	All Rights Reserved
//==============================================================================================
#ifndef MsiChart_Initializer_h
#define MsiChart_Initializer_h 

#include "ChartEngine.h"

namespace MsiChart
{
	class CHARTENGINE_API Initializer
	{
	public:
		Initializer();
		Initializer(bool iYatiTest);
		~Initializer() throw();
	private:
		bool mYatiTest;

	};


}


#endif
