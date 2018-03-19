//==============================================================================================
//	FILENAME	:	HexEncoder.h
//	AUTHOR		:	Yuan Ding
//	CREATION	:	11/16/2001
//	Copyright (C) MicroStrategy Incorporated 1999
//  All Rights Reserved
//==============================================================================================

#ifndef MEncryptor_HexEncoder_h
#define MEncryptor_HexEncoder_h

#include "Encryptor/Encryptor/Encryptor.h"
#include "Encryptor/Encryptor/Hash.h"
#include "Base/Base/ReturnString.h"

namespace MDataType {
	class Binary;
}

namespace MEncryptor {

	///HexEncoder contains functions that perform conversion between binary data and its Hex representation.
	
	/// Encode the given binary data into its Hex representation. It also hash the binary data to
	/// generate a signature. You can also associate a version with the encoded data. The encoded 
	/// data contains the version, the signature and the original data (in this order).
	/** @param ipcData is the binary data to be encoded
	  *	@param inVersion is the version that you'd like to associate with the data
	  * @param ibReverseHighLow indicates if the high and low part of a byte needs to be reversed
	  *        (default is not).
	  * @param ibUpperCase indicates whether to use upper case for the hexdecimal letters
	  *		   (default is upper case).
	  * @return Hex representation of the binary data, it contains the version, a signature and
	  *			the data (in this order)
	  */
	DLL_ENCRYPTOR_EXIM MBase::ReturnWString HexEncodeWithSignature(const MDataType::Binary* ipcData, 
													   unsigned Int32 inVersion,
													   bool ibReverseHighLow = false,
													   bool ibUpperCase = true);

	/// Decode the string encoded with HexEncodeWithSignature
	/** @param ipcData is the encoded data
	  * @param ibReverseHighLow indicates if the high and low part of a byte has been reversed
	  *        (default is not).
	  * @return The decoded data in binary format.
	  * @throw BadDataException indicating the data is corrupted
	  */
	DLL_ENCRYPTOR_EXIM MDataType::Binary* HexDecodeWithSignature(const WCHAR* ipcData,
																 bool ibReverseHighLow = false);

	/// Encode the given binary data into its Hex representation (without signature).
	/** @param ipcData is the binary data to be encoded
	  * @param ibReverseHighLow indicates if the high and low part of a byte needs to be reversed
	  *        (default is not).
	  * @param ibUpperCase indicates whether to use upper case for the hexdecimal letters
	  *		   (default is upper case).
	  * @return Hex representation of the binary data
	  */
	DLL_ENCRYPTOR_EXIM MBase::ReturnWString HexEncode(const MDataType::Binary* ipcData,
										  bool ibReverseHighLow = false,
										  bool ibUpperCase = true);

	/// Decode the string encoded with HexEncode
	/** @param ipcData is the string to be decoded
	  * @param ibReverseHighLow indicates if the high and low part of a byte has been reversed
	  *        (default is not).
	  * @return The decoded data in binary format.
	  * @throw BadDataException indicating the data is corrupted
	  */
	DLL_ENCRYPTOR_EXIM MDataType::Binary* HexDecode(const WCHAR* ipcData,
													bool ibReverseHighLow = false);

	/// Retrieve the version from the given encoded data (encoded with HexEncodeWithSignature)
	/** @param ipcEncodedData The encoded data
	  * @return The version associated with the data. It returns -2 if the data is corrupted.
	  */
	DLL_ENCRYPTOR_EXIM unsigned Int32 GetVersion(const WCHAR* ipcEncodedData);

	DLL_ENCRYPTOR_EXIM MDataType::Binary* AddHeader(const MDataType::Binary* ipcData, 
								 unsigned Int32 inVersion);

	DLL_ENCRYPTOR_EXIM MDataType::Binary* RemoveHeader(const MDataType::Binary* ipcData);

}//namespace

#endif //MEncryptor_HexEncoder_h
