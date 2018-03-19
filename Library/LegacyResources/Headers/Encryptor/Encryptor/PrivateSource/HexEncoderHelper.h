//==============================================================================================
//	FILENAME	:	HexEncoderHelper.h
//	AUTHOR		:	Yuan Ding
//	CREATION	:	09/12/2003
//	Copyright (C) MicroStrategy Incorporated 1999
//  All Rights Reserved
//==============================================================================================

#ifndef MEncryptor_HexEncoderHelper_h
#define MEncryptor_HexEncoderHelper_h

namespace MDataType {
	class Binary;
}

namespace MEncryptor {

	//xfan. 1/21/08. added for AES
WCHAR* hHexEncodeWithEncryptionLevel(const MDataType::Binary* ipcData, 
								 unsigned int inEncryptionLevel,
								 unsigned int inHashVersion,
								 bool ibReverseHighLow = false,
								 bool ibUpperCase = true);

MDataType::Binary* HexDecodeWithEncryptionLevel(const WCHAR* ipcData, bool ibReverseHighLow);

/// The actual implementation of the encoding algorithm.
WCHAR* hHexEncodeWithSignature(const MDataType::Binary* ipcData, 
								 unsigned int inVersion,
								 bool ibReverseHighLow = false,
								 bool ibUpperCase = true);

/// Converts a character to a number.
bool ctoi(unsigned char iDigitChar, unsigned char& orDigit);

/// Actually encode the given binary to its Hex representation.
WCHAR* hHexEncode(const MDataType::Binary* ipcBinaryData, 
								bool ibReverseHighLow = false,
								bool ibUpperCase = true);

}// namespace

#endif //MEncryptor_HexEncoderHelper_h