//==============================================================================================
//	FILENAME	:	Hash.h
//	AUTHOR		:	Yuan Ding
//	CREATION	:	11/15/2001
//	Copyright (C) MicroStrategy Incorporated 1999
//  All Rights Reserved
//==============================================================================================

#ifndef Encryptor_Hash_h
#define Encryptor_Hash_h

#include "Base/Interfaces/BaseException.h"
#include "Base/Base/ReturnString.h"
#include "Encryptor.h"
#include "DataType/DataType/Binary.h"

namespace MEncryptor {

typedef enum
{
	RIPEMD160 = 0,
	HashingVersionHighest	// The highest hashing version. Always insert new hashing version before this.
} HashingVersion;

///Hash is a one way encryption that uses RIPEMD160 hashing algorithm
class DLL_ENCRYPTOR_EXIM Hash
{
public:

	/// Hash the data
	/** @param ipcData is data to be hashed
	  * @param inVersion to use for hashing
	  *		currently supported:
	  *			0 (RIPEMD160)
	  * @return is a hex representation string
	  * @throw BadHashException
	  */
	static MBase::ReturnWString HashData(const WCHAR* ipcData, HashingVersion inVersion);
	//xfan. 1/15/08. hash with fixed length. used in AES
	//(static MDataType::Binary* HashData(const WCHAR* ipcData,HashingVersion inVersion, unsigned int iLen);
	/// Get the hashing version used to hash the data and create the signature
	/** @param ipcHashedData is a hashed string
	  * @throw BadDataException when input is corrupt
	  */
	static HashingVersion GetHashVersion(const WCHAR* ipcHashedData);
};

class BadHashException : public MBase::BaseException {};
class BadDataException : public MBase::BaseException {};

}//namespace

#endif //Encryptor_Hash_h
