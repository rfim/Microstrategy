//==============================================================================================
//	FILENAME	:	RIPEMD160Hasher.cpp
//	AUTHOR		:	Yuan Ding
//	CREATION	:	11/16/2001
//	Copyright (C) MicroStrategy Incorporated 1999
//  All Rights Reserved
//==============================================================================================

#include "../StdAfx.h"
#include "RIPEMD160Hasher.h"
#include "rmd160.h"
#include "DataType/DataType/Binary.h"
#include "../Util/WCharConverter.h"

namespace MEncryptor 
{

const unsigned int RIPEMD160Hasher::HASH_SIZE = 20;
const unsigned int RIPEMD160Hasher::BLOCK_SIZE = 16;

typedef MBase::StrongArrayPtr<unsigned char> ByteArrayStrongPtr;

MDataType::Binary* RIPEMD160Hasher::HashStringData(const WCHAR* ipcData, unsigned int inMaxHashLength)
{
	_ASSERTE(ipcData);
	_ASSERTE(inMaxHashLength>0);

	// wchar_t needs to be converted from 4 bytes to 2 on some platforms.
	MDataType::BinaryPtr lpData(WCharConverter::Four2Two(ipcData));

	return HashBinaryData(lpData.Get(), inMaxHashLength);
}

MDataType::Binary* RIPEMD160Hasher::HashBinaryData(const MDataType::Binary* ipcData, unsigned int inMaxHashLength)
{
	_ASSERTE(ipcData);
	_ASSERTE(inMaxHashLength>0 && inMaxHashLength <= HASH_SIZE);

	unsigned int lnSourceDataLength = static_cast<unsigned int>(ipcData->GetSize());
	const unsigned int* lpSourcePos = NULL;
	if (lnSourceDataLength>0)
	{
		lpSourcePos = reinterpret_cast<const unsigned int*>(ipcData->c_ptr());
	}
	
	unsigned char lnHashBuffer[HASH_SIZE];

	// Init the hash buffer
	MDinit(reinterpret_cast<unsigned int*>(lnHashBuffer));

	// The hashing algoritym processes BLOCK_SIZE bytes in each cycle.
	unsigned int lnSteps = lnSourceDataLength / (BLOCK_SIZE*sizeof(int));
	unsigned int lnBytesRemaining = lnSourceDataLength;

	for (unsigned int i = 0; i < lnSteps; i++)
	{	
		compress(reinterpret_cast<unsigned int*>(lnHashBuffer), lpSourcePos);
		//Advance the source position and adjust the bytes remaining.
		lpSourcePos+=BLOCK_SIZE;
		lnBytesRemaining-=BLOCK_SIZE*sizeof(int);
	}

	MDfinish(reinterpret_cast<unsigned int*>(lnHashBuffer), 
			 reinterpret_cast<const unsigned char*>(lpSourcePos), lnBytesRemaining, lnSourceDataLength);
	
	return MDataType::CreateBinary(lnHashBuffer, (HASH_SIZE < inMaxHashLength)?HASH_SIZE:inMaxHashLength);
}

}//namespace
