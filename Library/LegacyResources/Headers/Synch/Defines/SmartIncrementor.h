//==============================================================================================
//	FILENAME	:	SmartIncrementor.h
//	AUTHOR		:	Yulin Wen
//	CREATION	:	7/1/2004
//	Copyright (C) MicroStrategy Incorporated 2004
//==============================================================================================
#ifndef MSynch_SmartIncrementor_h
#define MSynch_SmartIncrementor_h

namespace MSynch
{
	template<class T>
	class SmartIncrementor
	{
	public:

		inline SmartIncrementor(T& irInt):
			mrInt(irInt)
		{
			++mrInt;
		}

		inline ~SmartIncrementor() throw()
		{
			--mrInt;
		}

	private:

		T& mrInt;
	};
}

#endif // MSynch_SemaphoreImpl_h
