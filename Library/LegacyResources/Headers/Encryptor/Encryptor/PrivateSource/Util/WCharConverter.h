//==============================================================================================
//	FILENAME	:	WCharConverter.h
//	AUTHOR		:	Yuan Ding
//	CREATION	:	06/20/2002
//	Copyright (C) MicroStrategy Incorporated 1999
//  All Rights Reserved
//==============================================================================================

#ifndef MEncryptor_WCharConverter_h
#define MEncryptor_WCharConverter_h

namespace MDataType {
	class Binary;
}

namespace MEncryptor {

///WCharConverter exposes two static methods to convert wchar_t strings back and forth 
///between 4 byte and 2 byte representation (for each wchar_t). It also performs endian
///conversion.
class WCharConverter
{
public:
	/// Convert wchar_t string from 4 bytes to 2 bytes.
	/** @param ipcWCBuf wchar_t string in 4 byte representation
	  * @return wchar_t string in 2 bytes representation
	  */
	static MDataType::Binary* Four2Two(const WCHAR* ipcWCBuf);
	
	/// Convert wchar_t string from 2 bytes to 4 bytes.
	/** @param ipcBuf wchar_t string in 2 byte representation
	  * @return wchar_t string in 4 bytes representation
	  */
	static WCHAR* Two2Four(const MDataType::Binary* ipcBuf);
};

} // namespace

#endif //MEncryptor_WCharConverter_h
