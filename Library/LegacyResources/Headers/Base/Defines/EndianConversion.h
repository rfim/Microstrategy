//==============================================================================================
//	FILENAME	:	EndianConversion.h
//	AUTHOR		:	Yuan Ding
//	CREATION	:	07/16/2002
//	Copyright (C) MicroStrategy Incorporated 1999
//  All Rights Reserved
//==============================================================================================
#ifndef MBase_EndianConversion_h
#define MBase_EndianConversion_h

#include "Base/Defines/BasicTypes.h"
#include "Base/Base/System.h"

namespace MBase
{

	///Template function to conduct endian conversion on the given data type.
	///Since Intel and SUN/IBM have different endian architecture, when a block
	///of memory needs to be interpreted as some data type, the result will be 
	///different. Use this function to perform the conversion.
	///This is an inline function to reduce the cost of a function call, especially
	///for the case where an array of data needs to be converted.
	///To use it, please specify the data type to be converted. For example,
	///to convert long, call it like this:
	///MBase::ConvertEndian<long>(lnData).
	template<class T> inline T ConvertEndian(T iData)
	{
		if (IsBigEndian())
		{
			unsigned long lnDataSize = sizeof(T);
			unsigned char* lpByte = (unsigned char*)(&iData);
			unsigned char lpTemp = 0;
			for (unsigned Int32 idxByte = 0; idxByte < lnDataSize/2; idxByte++) 
			{
				lpTemp = *(lpByte+idxByte);
				*(lpByte+idxByte) = *(lpByte+lnDataSize-idxByte-1);
				*(lpByte+lnDataSize-idxByte-1) = lpTemp;
			}			
		}
		
		return iData;
	}

#ifdef GUID_DEFINED

	inline void EndianConvertGUID(::GUID &irGUID)
	{
		if (IsBigEndian())
		{	
			irGUID.Data1 = ConvertEndian<unsigned Int32>(irGUID.Data1);
			irGUID.Data2 = ConvertEndian<unsigned short>(irGUID.Data2);
			irGUID.Data3 = ConvertEndian<unsigned short>(irGUID.Data3);
		}	
	}

#endif // GUID_DEFINED
}

#endif // MBase_EndianConversion_h
