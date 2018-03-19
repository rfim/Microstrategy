//==============================================================================================
//	FILENAME	:	RIPEMD160Hasher.h
//	AUTHOR		:	Yuan Ding
//	CREATION	:	11/16/2001
//	Copyright (C) MicroStrategy Incorporated 1999
//  All Rights Reserved
//==============================================================================================

#ifndef MEncryptor_RIPEMD160Hasher_h
#define MEncryptor_RIPEMD160Hasher_h

namespace MDataType {
	class Binary;
}

namespace MEncryptor {

///RIPEMD160Hasher exposes a static method to hash binary data.
class RIPEMD160Hasher
{
public:

	/// Hash the string data using the RIPEMD160 hashing algorithm
	/** @param ipcData String data to be hashed
	  * @param inMaxHashLength Maximum length of the hashed data (in byte)
	  * @return is the hashed binary data
	  */
	static MDataType::Binary* HashStringData(const WCHAR* ipcData, unsigned int inMaxHashLength = 20);

	/// Hash the binary data using the RIPEMD160 hashing algorithm
	/** @param ipcData Binary data to be hashed
	  * @param inMaxHashLength Maximum length of the hashed data (in byte)
	  * @return is the hashed binary data
	  */
	static MDataType::Binary* HashBinaryData(const MDataType::Binary* ipcData, unsigned int inMaxHashLength = 20);

private:
	/// The size (in byte) of hasher
	const static unsigned int HASH_SIZE;

	/// The block size (in byte) that the hashing algorithm processes
	/// in each cycle
	const static unsigned int BLOCK_SIZE;
};

}//namespace

#endif //MEncryptor_RIPEMD160Hasher_h
