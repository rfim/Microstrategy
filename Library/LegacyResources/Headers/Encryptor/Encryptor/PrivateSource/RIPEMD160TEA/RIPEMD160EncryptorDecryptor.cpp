//==============================================================================================
//	FILENAME	:	RIPEMD160EncryptorDecryptor.cpp
//	AUTHOR		:	Yuan Ding
//	CREATION	:	11/20/2001
//	Copyright (C) MicroStrategy Incorporated 1999
//  All Rights Reserved
//==============================================================================================

#include "../StdAfx.h"
#include "RIPEMD160EncryptorDecryptor.h"
#include "RIPEMD160Hasher.h"
#include "Base/Defines/ExceptionImpl.h"
#include "Encryptor/Encryptor/HexEncoder.h"
#include "Encryptor/Encryptor/PrivateSource/HexEncoderHelper.h"
#include "../Util/WCharConverter.h"
#include "Base/Defines/EndianConversion.h"
#include "Base/Base/UTFConverter.h"
#include <time.h>

#ifndef WIN32
#include <ctype.h>
#endif

#define RAND(a,b) (((a = 36969 * (a & 65535) + (a >> 16)) << 16) + \
                    (b = 18000 * (b & 65535) + (b >> 16))  )


namespace MEncryptor
{

const unsigned Int32 RIPEMD160EncryptorDecryptor::KEY_SIZE	= 16;
const unsigned Int32 RIPEMD160EncryptorDecryptor::BLOCK_SIZE	= 8;
const unsigned Int32 RIPEMD160EncryptorDecryptor::CRC_SIZE	= 4;

typedef MBase::StrongArrayPtr<unsigned char> ByteArrayStrongPtr;

RIPEMD160EncryptorDecryptor::RIPEMD160EncryptorDecryptor(const WCHAR* ipcKeySeed)
: mpSessionKey(RIPEMD160Hasher::HashStringData(ipcKeySeed, KEY_SIZE)),
mCS(MSynch::CreateCriticalSection())
{
	cycles(mRandomFill);
	char lActualKey[KEY_LENGTH];
	char* lpAESKey = (char*)((mpSessionKey.Get())->c_ptr());
	mctx_enc.n_rnd = 0; // This is for creating schedule table from the key(in hexadecimal but represented as characters)  ensure all flags are initially set to zero
	mctx_enc.n_blk = 0;
	mctx_dec.n_rnd = 0; // This is for creating schedule table from the key(in hexadecimal but represented as characters)  ensure all flags are initially set to zero
	mctx_dec.n_blk = 0;

	for(Int32 i =0;i<16;i++)
		lActualKey[i] = lpAESKey[i];

	aes_enc_key((unsigned char*)lActualKey , KEY_LENGTH, &mctx_enc); // Call this so that scheduling table is ready before encrypting  because it uses "ctx" rather than key

	aes_dec_key((unsigned char*)lActualKey , KEY_LENGTH, &mctx_dec); // Call this so that scheduling table is ready before decrypting  because it uses "ctx" rather than key

}

RIPEMD160EncryptorDecryptor::RIPEMD160EncryptorDecryptor(MDataType::Binary* ipKey)
: mpSessionKey(ipKey),
mCS(MSynch::CreateCriticalSection())
{
	/*
	mctx_enc.n_rnd = 0; // This is for creating schedule table from the key(in hexadecimal but represented as characters)  ensure all flags are initially set to zero
	mctx_enc.n_blk = 0;
	mctx_dec.n_rnd = 0; // This is for creating schedule table from the key(in hexadecimal but represented as characters)  ensure all flags are initially set to zero
	mctx_dec.n_blk = 0;

	aes_enc_key((unsigned char*)((mpSessionKey.Get())->c_ptr()), (mpSessionKey.Get()->GetSize()), &mctx_enc); // Call this so that scheduling table is ready before encrypting  because it uses "ctx" rather than key

	aes_dec_key((unsigned char*) ((mpSessionKey.Get())->c_ptr()), (mpSessionKey.Get()->GetSize()), &mctx_dec); // Call this so that scheduling table is ready before decrypting  because it uses "ctx" rather than key
	*/

	//cseng: we switch to OpenSSL AES implementation
	unsigned char* lKeyString = ((unsigned char*)(mpSessionKey.Get())->c_ptr());
	AES_set_encrypt_key(lKeyString, 128, &mEncKey);
	AES_set_decrypt_key(lKeyString, 128, &mDecKey);

}

RIPEMD160EncryptorDecryptor::~RIPEMD160EncryptorDecryptor() throw()
{
}


//helper function
Int32 GetHexValue(const WCHAR* ipcEncodedData, Int32 iStart)
{
	unsigned Int32 lnVersion = 0;
	unsigned Int32 lnHex = 1;
	for (Int32 i= iStart + 8 -1; i>=iStart; i--)
	{
		unsigned char lnLow = static_cast<unsigned char>(ipcEncodedData[i]);
		unsigned char lnDigit = 0;
		if (ctoi(lnLow, lnDigit))
		{
			lnVersion += lnHex*lnDigit;
			lnHex = lnHex*16;
		}
		else
		{
			return -1;
		}
	}
	return lnVersion;
}


// xfan. 1/22/08
// first 4 bytes, if 0, TEA. if 1, AES, otherwise, RC4
// return -3 in error
Int32 GetEncryptionLevel(const WCHAR* ipcEncodedData)
{
	_ASSERT(ipcEncodedData);

	if (::strlen(ipcEncodedData)>8)
	{
		Int32 lVersion = GetHexValue(ipcEncodedData, 0);
		switch(lVersion)
		{
		case 0:
			return TEA;
		case 1:
			return AES;
		case -1: //error case
			return -3;
		default: //other cases
			return RC4;
		}
	}
	else
	{
		return -3;
	}
}

// xfan. 3/9/08. for binary
Int32 GetEncryptionLevel(const void* ipcEncodedData)
{
	Int32 lVersion;
	::memmove(&lVersion, ipcEncodedData, sizeof(Int32));
	lVersion = MBase::ConvertEndian<unsigned Int32>(lVersion);
	
	switch(lVersion)
	{
	case 0:
		return TEA;
	case 1:
		return AES;
	default: //other cases
		return -3;
	}
}

// xfan. 5/27/09. add new interface. TQMS 162764
MDataType::Binary* RIPEMD160EncryptorDecryptor::EncryptData2(
													const WCHAR* ipcPlainText,
													EncryptionVersion inVersion) const
{
	_ASSERT(ipcPlainText);
	_ASSERT(inVersion == TEA|| inVersion == AES);
	if (!ipcPlainText)
		throw MBase::ExceptionImpl<BadEncryptionException>(_T("Encryptor: Invalid input parameter."));

	// wchar_t string needs to be converted from 4 bytes to 2 on some platforms.
	MDataType::BinaryPtr lpPlainText(WCharConverter::Four2Two(ipcPlainText));

	MDataType::BinaryPtr lpCipherText(hEncryptData(lpPlainText.Get(), inVersion));

	MDataType::BinaryPtr lEncryptededDataPtr(AddHeader
		(lpCipherText.Get(),
		static_cast<unsigned Int32>(inVersion)));
	return lEncryptededDataPtr.GiveUp();
}

MBase::ReturnWString RIPEMD160EncryptorDecryptor::DecryptData2(const void* ipcCipherText, size_t inCipherTextSize) const
{
	_ASSERT(ipcCipherText);
	if (!ipcCipherText)
		throw MBase::ExceptionImpl<BadDataException>(_T("Corrupted data."));

	MDataType::BinaryPtr lpCipherText(MDataType::CreateBinary(ipcCipherText, inCipherTextSize));
	Int32 lVersion = GetEncryptionLevel(lpCipherText->c_ptr());
	lpCipherText.Reset(RemoveHeader(lpCipherText.Get()));

	MDataType::BinaryPtr lpPlainTextBinary(hDecryptData(lpCipherText.Get(),static_cast<EncryptionVersion>(lVersion)));
	// wchar_t string needs to be converted back from 2 bytes to 4 on some platforms.
	MBase::StrongArrayPtr<WCHAR> lpPlainTextString(WCharConverter::Two2Four(lpPlainTextBinary.Get()));
	if (lpPlainTextString.IsNull())
	{
		return _T("");
	}
	else
	{
		return lpPlainTextString.Get();
	}
}

MBase::ReturnWString RIPEMD160EncryptorDecryptor::EncryptData(
													const WCHAR* ipcPlainText,
													EncryptionVersion inVersion) const
{
	_ASSERT(ipcPlainText);
	_ASSERT(inVersion == TEA|| inVersion == AES);
	if (!ipcPlainText)
		throw MBase::ExceptionImpl<BadEncryptionException>(_T("Encryptor: Invalid input parameter."));

	// wchar_t string needs to be converted from 4 bytes to 2 on some platforms.
	MDataType::BinaryPtr lpPlainText(WCharConverter::Four2Two(ipcPlainText));

	MDataType::BinaryPtr lpCipherText(hEncryptData(lpPlainText.Get(), inVersion));

	MBase::StrongArrayPtr<WCHAR> lEncryptededDataPtr(hHexEncodeWithSignature
		(lpCipherText.Get(),
		static_cast<unsigned Int32>(inVersion),
		true, false));
	return lEncryptededDataPtr.Get();
}

MBase::ReturnWString RIPEMD160EncryptorDecryptor::DecryptData(
													const WCHAR* ipcCipherText) const
{
	_ASSERT(ipcCipherText);
	if (!ipcCipherText)
		throw MBase::ExceptionImpl<BadDataException>(_T("Corrupted data."));

	Int32 lVersion = GetEncryptionLevel(ipcCipherText);
//	_ASSERT(lVersion ==	TEA || lVersion == AES);

	MDataType::BinaryPtr lpCipherText;
	lpCipherText.Reset(HexDecodeWithSignature(ipcCipherText, true));

	MDataType::BinaryPtr lpPlainTextBinary(hDecryptData(lpCipherText.Get(),static_cast<EncryptionVersion>(lVersion)));
	
	// wchar_t string needs to be converted back from 2 bytes to 4 on some platforms.
	MBase::StrongArrayPtr<WCHAR> lpPlainTextString(WCharConverter::Two2Four(lpPlainTextBinary.Get()));

	if (lpPlainTextString.IsNull())
	{
		return _T("");
	}
	else
	{
		return lpPlainTextString.Get();
	}
}

MDataType::Binary* RIPEMD160EncryptorDecryptor::EncryptData(
													const MDataType::Binary* ipcPlainText,
													EncryptionVersion inVersion) const
{
	// Check if we support the requested encryption version
	_ASSERT(ipcPlainText);
	_ASSERT(inVersion == TEA || inVersion == AES);
	if (!ipcPlainText)
		throw MBase::ExceptionImpl<BadEncryptionException>(_T("Encryptor: Invalid input parameter."));

	MDataType::BinaryPtr lpCipherText(hEncryptData(ipcPlainText, inVersion));

	MDataType::BinaryPtr lEncryptededDataPtr(AddHeader
		(lpCipherText.Get(),
		static_cast<unsigned Int32>(inVersion)));

	return lEncryptededDataPtr.GiveUp();
}

MDataType::Binary* RIPEMD160EncryptorDecryptor::DecryptData(
													const MDataType::Binary* ipcCipherText) const
{
	_ASSERT(ipcCipherText);
	if (!ipcCipherText)
		throw MBase::ExceptionImpl<BadDataException>(_T("Corrupted data."));

	Int32 lVersion = GetEncryptionLevel(ipcCipherText->c_ptr());
	//_ASSERT(lVersion ==	TEA || lVersion == AES);

	MDataType::BinaryPtr lpCipherText;
	lpCipherText.Reset(RemoveHeader(ipcCipherText));

	return hDecryptData(lpCipherText.Get(),static_cast<EncryptionVersion>(lVersion));
}

MDataType::Binary* RIPEMD160EncryptorDecryptor::EncryptData(
													const void* ipcPlainText,
													size_t inPlainTextSize,
													EncryptionVersion inVersion) const
{
	// Check if we support the requested encryption version
	_ASSERT(ipcPlainText);
	_ASSERT(inVersion == TEA || inVersion == AES);
	if (!ipcPlainText)
		throw MBase::ExceptionImpl<BadEncryptionException>(_T("Encryptor: Invalid input parameter."));

	MDataType::BinaryPtr lpPlainText(MDataType::CreateBinary(ipcPlainText, inPlainTextSize));
	MDataType::BinaryPtr lpCipherText(hEncryptData(lpPlainText.Get(), inVersion));

	MBase::StrongPtr<MDataType::Binary> lEncryptededDataPtr(AddHeader
		(lpCipherText.Get(),
		static_cast<unsigned Int32>(inVersion)));
	return lEncryptededDataPtr.GiveUp();
}

MDataType::Binary* RIPEMD160EncryptorDecryptor::DecryptData(
													const void* ipcCipherText,
													size_t inCipherTextSize) const
{
	_ASSERT(ipcCipherText);
	if (!ipcCipherText)
		throw MBase::ExceptionImpl<BadDataException>(_T("Corrupted data."));

	//_ASSERT(lVersion ==	TEA || lVersion == AES);

	MDataType::BinaryPtr lpCipherText(MDataType::CreateBinary(ipcCipherText, inCipherTextSize));
	Int32 lVersion = GetEncryptionLevel(lpCipherText->c_ptr());
	lpCipherText.Reset(RemoveHeader(lpCipherText.Get()));
	return hDecryptData(lpCipherText.Get(),static_cast<EncryptionVersion>(lVersion));
}

EncryptionVersion GetHighestVersionRIPEMD160()
{
	return AES;
}

EncryptorDecryptor* CreateRIPEMD160EncryptorDecryptor(
										const WCHAR* ipcKeySeed)
{
	return new RIPEMD160EncryptorDecryptor(ipcKeySeed);
}

EncryptorDecryptor* CreateRIPEMD160EncryptorDecryptor(
										MDataType::Binary* ipKey)
{
	return new RIPEMD160EncryptorDecryptor(ipKey);
}

MDataType::Binary* RIPEMD160EncryptorDecryptor::hEncryptData(
													const MDataType::Binary* ipcPlainText,
													EncryptionVersion inVersion) const
{
	// Check if we support the requested encryption version
	_ASSERT(ipcPlainText);
	_ASSERT(inVersion == TEA || inVersion == AES);

	if ((inVersion != TEA) && (inVersion != AES))
	{
		throw MBase::ExceptionImpl<BadEncryptionException>(_T("Encryptor: Invalid input parameter."));
	}

	unsigned Int32 lnSourceDataLength = static_cast<unsigned Int32>(ipcPlainText->GetSize());	// The size of the source data in bytes
	const unsigned Int32* lpSourcePos = NULL; // for TEA
	const unsigned char* lpSource; // for AES. pointing to the Source Text
	unsigned char lnLastBlockUsedBytes = 0;

	if (lnSourceDataLength>0)
	{
		if(inVersion == TEA)
		{
			lpSourcePos = reinterpret_cast<const unsigned Int32*>(ipcPlainText->c_ptr());	// A pointer to the data that we are encypting
			lnLastBlockUsedBytes = static_cast<unsigned char>(lnSourceDataLength % BLOCK_SIZE);
		}
		else  //AES
		{
			lpSource = reinterpret_cast<const unsigned char*>(ipcPlainText->c_ptr());	// A pointer to the data that we are encrypting
			lnLastBlockUsedBytes = static_cast<unsigned char>(lnSourceDataLength % AES_BLOCK_SIZE);
		}
	}

	// We have a block cipher of BLOCK_SIZE, this means that all data must be encrypted
	// in chunks of BLOCK_SIZE bytes. The only problem here is the last block, this is
	// because the last block of the plaintext doesn't have to be the size of the BLOCK_SIZE
	// if we only added zeroes to complete the last block, then when decrypting the data would
	// appear to be longer than the original plaintext (because of the ending zeroes). In order
	// to cope with this we allways assume that the last byte of the last block of ciphertext
	// indicates the number of used bytes in that block.
	// This leads to two possible scenarios:
	// 1) The last block of the plaintext is NOT as large as BLOCK_SIZE
	//		- In this case we use the last byte in the block to indicate
	//		  The number of bytes actually being used in that block
	// 2) The last block of the plaintext IS exactly BLOCK_SIZE
	//		- In this case we add another block to the ciphertext with the last byte set to zero
	//		  thus indicating that zero bytes in ths block are data from the plaintext

	// Since the ciphertext can be one block bigger that the original message we need to
	// account for this
	unsigned Int32 lnDestDataLength = lnSourceDataLength;
	// The ciphertext must be a multiple of BLOCK_SIZE
	if(inVersion == TEA)
	{
		if (lnLastBlockUsedBytes!=0)
		{
			lnDestDataLength += BLOCK_SIZE - lnLastBlockUsedBytes;
			_ASSERTE((lnDestDataLength % BLOCK_SIZE)==0);
		}
		else
		{
			lnDestDataLength += BLOCK_SIZE;
		}
	}
	else
		lnDestDataLength += AES_BLOCK_SIZE; // This is required for AES algorithm

	// Since we place a hash of the original message at the beginning,
	// we need some extra space
	lnDestDataLength += CRC_SIZE;

	// Allocate the output buffer, leave space for the hast at the beginning of the message
	unsigned char* lpTemp = new unsigned char[lnDestDataLength];
	_ASSERT(lpTemp);
	ByteArrayStrongPtr lpOutputData(lpTemp);

	// Create the hash
	MDataType::BinaryPtr lpMessageHash(RIPEMD160Hasher::HashBinaryData(ipcPlainText, CRC_SIZE));

	// Copy the hash to the encrypted message
	memmove(lpOutputData.Get(), lpMessageHash->c_ptr(), CRC_SIZE);

	if(inVersion == TEA)
	{
	// Set the destination position to the beginnig of the encrypted message, just after the hash
	unsigned Int32* lpDestPos = reinterpret_cast<unsigned Int32*>(lpOutputData.Get() + CRC_SIZE);

	// Calculate the number of steps to do (excluding the last block)
	unsigned Int32 lnSteps = lnSourceDataLength/BLOCK_SIZE;

	for (unsigned Int32 i = 0; i < lnSteps; i++)
	{
		encipher(lpSourcePos, lpDestPos, reinterpret_cast<const unsigned Int32*>(mpSessionKey->c_ptr()));
		// Advance the data pointers
		lpSourcePos += 2;
		lpDestPos += 2;
	}

	// Do the final block
	unsigned Int32 lnFinalBlock[2] = {0,0}; 	// Pad the final block with zeroes
	if (lpSourcePos!=NULL)
	{
		memmove(lnFinalBlock, lpSourcePos, lnLastBlockUsedBytes);
	}
	// Set the last byte to the number of blocks used.
	(reinterpret_cast<unsigned char*>(lnFinalBlock))[7] =
										static_cast<unsigned char>(lnSourceDataLength % BLOCK_SIZE);
	// Encrypt the final block
	encipher(lnFinalBlock, lpDestPos, reinterpret_cast<const unsigned Int32*>(mpSessionKey->c_ptr()));
	}
	else //AES
	{
		unsigned Int32 lSourcePos = 0;
		unsigned Int32 lDestPos = 0;
		lDestPos += CRC_SIZE; // Advance the pointer to next memory location by CRC_SIZE(Hash Size)

		char buf[BLOCK_LEN],  dbuf[2 * BLOCK_LEN];
		Int32 i = 0;
		 // set a random IV
	    fillrand(dbuf, BLOCK_LEN);
	/*	for(i=0;i<BLOCK_LEN;i++)
			dbuf[i] = 0;
	*/
		if(lnSourceDataLength <= BLOCK_LEN)
		{   // if the string length is less than or equal to 16 bytes

			// read the bytes of the string into the buffer
			memmove(dbuf+ BLOCK_LEN,lpSource ,lnSourceDataLength);
			lSourcePos += lnSourceDataLength;
			// pad the file bytes with zeroes
			for(i = lnSourceDataLength; i < BLOCK_LEN; ++i)
				dbuf[i + BLOCK_LEN] = 0;

			// xor the file bytes with the IV bytes
			for(i = 0; i < BLOCK_LEN; ++i)
				dbuf[i + BLOCK_LEN] ^= dbuf[i];

			// encrypt the top 16 bytes of the buffer
			AES_encrypt((unsigned char*)dbuf + BLOCK_LEN, (unsigned char*)dbuf + lnSourceDataLength, &mEncKey);

			Int32 len = lnSourceDataLength + BLOCK_LEN;
			// write the IV and the encrypted file bytes
			memmove(lpOutputData.Get() + lDestPos,dbuf,len);
			lDestPos = lDestPos + len;
		}
		else // length is more than 16 bytes
		{
			// write the IV
			memmove(lpOutputData.Get() + lDestPos,dbuf,BLOCK_LEN);
			lDestPos += BLOCK_LEN;

			while(lSourcePos < lnSourceDataLength) // till you reach the end of the input buffer
			{
				 // read a block and reduce the remaining byte count
				memmove(buf,lpSource + lSourcePos,BLOCK_LEN);

				lSourcePos += BLOCK_LEN;
				// do CBC chaining prior to encryption
				for(i = 0; i < BLOCK_LEN; ++i)
					buf[i] ^= dbuf[i];

				// encrypt the block
				AES_encrypt((unsigned char*)buf,(unsigned char*) dbuf, &mEncKey);
				if((lnSourceDataLength - lSourcePos) > 0 && (lnSourceDataLength - lSourcePos) < BLOCK_LEN) // if there is only one more block do ciphertext stealing
				{
					// move the previous ciphertext to top half of double buffer since rlen bytes of this are output last
					Int32 iRemaining = lnSourceDataLength - lSourcePos; // remianing no. of bytes
					for(i = 0; i < BLOCK_LEN; ++i)
						dbuf[i + BLOCK_LEN] = dbuf[i];
					// read last part of plaintext into bottom half of buffer
					memmove(dbuf,lpSource + lSourcePos,iRemaining);
					lSourcePos += iRemaining;
					// clear the remainder of the bottom half of buffer
					for(i = 0; i < BLOCK_LEN - iRemaining; ++i)
						dbuf[iRemaining + i] = 0;
					// do CBC chaining from previous ciphertext
					for(i = 0; i < BLOCK_LEN; ++i)
						dbuf[i] ^= dbuf[i + BLOCK_LEN];

					// encrypt the final block
					AES_encrypt((unsigned char*)dbuf, (unsigned char*)dbuf, &mEncKey);
                // set the length of the final write
					//len = iRemaining + BLOCK_LEN;
					memmove(lpOutputData.Get() + lDestPos,dbuf,iRemaining + BLOCK_LEN);
					lDestPos = lDestPos + BLOCK_LEN + iRemaining;
				}
				else
				{ // write the encrypted block
					memmove(lpOutputData.Get() + lDestPos,dbuf, BLOCK_LEN);
					lDestPos = lDestPos + BLOCK_LEN;
				}
			} // end of while (lSourcePos < lnSourceDataLength)

		}// end of if(lnSourceDataLength <= BLOCK_LEN)

	}
	// Return encrypted binary data
	return MDataType::CreateBinary(lpOutputData.Get(), lnDestDataLength);
}

MDataType::Binary* RIPEMD160EncryptorDecryptor::hDecryptData(
													const MDataType::Binary* ipcCipherText,
													EncryptionVersion inVersion) const
{

	_ASSERT(ipcCipherText);

	//need assert and exception?
//	_ASSERT(inVersion == TEA || inVersion == AES);

	
	if ((inVersion != TEA) && (inVersion != AES))
	{
		throw MBase::ExceptionImpl<BadEncryptionException>(_T("Encryptor: Invalid input parameter."));
	}

	if(inVersion == TEA)
	{
		// The minimum length of an ecrypted message is the starting hash and at least one block
		if (ipcCipherText != NULL && ipcCipherText->GetSize() >= CRC_SIZE + BLOCK_SIZE)
		{
			unsigned Int32 lnSourceDataLength = static_cast<unsigned Int32>(ipcCipherText->GetSize());	// The size of the source ciphertext in bytes

			// A pointer to the data that we are decypting, we need to skip the hash at the beginnig of the message
			const unsigned char* lpcData = reinterpret_cast<const unsigned char*>(ipcCipherText->c_ptr()) + CRC_SIZE;
			const unsigned Int32* lpSourcePos = reinterpret_cast<const unsigned Int32*>(lpcData);

			unsigned Int32 lnDestDataLength = lnSourceDataLength - CRC_SIZE;

			// Allocate the output buffer
			unsigned char* lpTemp = new unsigned char[lnDestDataLength];
			ByteArrayStrongPtr lpOutputData(lpTemp);
			unsigned Int32* lpDestPos = reinterpret_cast<unsigned Int32*>(lpOutputData.Get());

			unsigned Int32 lnSteps = lnDestDataLength/BLOCK_SIZE;

			for (unsigned Int32 i = 0; i < lnSteps; i++)
			{
				decipher(lpSourcePos, lpDestPos, reinterpret_cast<const unsigned Int32*>(mpSessionKey->c_ptr()));
				// Advance the data pointers
				lpSourcePos += 2;
				lpDestPos += 2;
			}

			unsigned char lnLastBlockUsedBytes = static_cast<unsigned char>(
				*(reinterpret_cast<unsigned char*>(lpDestPos)-1));
			// lnLastBlockUsedBytes > 7 indicate a corrupted data.
			if (lnLastBlockUsedBytes <= 7)
			{
				// Reduce the length of the destination data as indicated in the last byte of the last block
				lnDestDataLength = lnDestDataLength - (BLOCK_SIZE - lnLastBlockUsedBytes);

				// Create the output binary
				MDataType::BinaryPtr lpPlainText(MDataType::CreateBinary(lpOutputData.Get(), lnDestDataLength));

				// Obtain the original hash
				MDataType::BinaryPtr lpOriginalHash(MDataType::CreateBinary(ipcCipherText->c_ptr(), CRC_SIZE));
				MDataType::BinaryPtr lpDecryptedHash(RIPEMD160Hasher::HashBinaryData(lpPlainText.Get(), CRC_SIZE));

				if (*lpOriginalHash == *lpDecryptedHash)
				{
					return lpPlainText.GiveUp();
				}
			}
		}
	}
	else  //AES
	{
	
		Int32 i = 0;
		// The minimum length of an ecrypted message is the starting hash and at least one block
		if (ipcCipherText != NULL && ipcCipherText->GetSize() >= CRC_SIZE + AES_BLOCK_SIZE)
		{
			unsigned Int32 lnSourceDataLength = static_cast<unsigned Int32>(ipcCipherText->GetSize());	// The size of the source ciphertext in bytes

			// A pointer to the data that we are decypting, we need to skip the hash at the beginnig of the message
			const unsigned char* lpSource = reinterpret_cast<const unsigned char*>(ipcCipherText->c_ptr()) + CRC_SIZE;
			unsigned Int32 lnDestDataLength = lnSourceDataLength - CRC_SIZE - BLOCK_LEN; // Decrypted data size as in encryption we added BLOCK_LEN extra

			// Allocate the output buffer, leave space for the hast at the beginning of the message
			unsigned char* lpTemp = new unsigned char[lnDestDataLength];
			ByteArrayStrongPtr lpOutputData(lpTemp);
			unsigned Int32 lSourcePos = 0;
			unsigned Int32 lDestPos = 0;
			char            buf1[BLOCK_LEN], buf2[BLOCK_LEN], dbuf[2 * BLOCK_LEN];
			char            *b1, *b2, *bt;

			if( (lnSourceDataLength - CRC_SIZE) <= 2 * BLOCK_LEN) // if the original file length is less than or equal to 16 bytes
			{
			
				// read the bytes of the file and verify length
				memmove(dbuf,lpSource ,lnSourceDataLength-CRC_SIZE);
				lSourcePos += (lnSourceDataLength - CRC_SIZE);
				// decrypt from position len to position len + BLOCK_LEN
				AES_decrypt((unsigned char*)dbuf + (lnSourceDataLength - CRC_SIZE - BLOCK_LEN), (unsigned char*)dbuf + BLOCK_LEN, &mDecKey);

				// undo CBC chaining
				for(i = 0; i < (lnSourceDataLength - CRC_SIZE - BLOCK_LEN); ++i)
					dbuf[i] ^= dbuf[i + BLOCK_LEN];
				// output decrypted bytes
				memmove(lpOutputData.Get() + lDestPos,dbuf,(lnSourceDataLength - CRC_SIZE - BLOCK_LEN));
				lDestPos = lDestPos + lnSourceDataLength - CRC_SIZE - BLOCK_LEN;
			}
			else
			{
				// we need two input buffers because we have to keep the previous
				// ciphertext block - the pointers b1 and b2 are swapped once per
				// loop so that b2 points to new ciphertext block and b1 to the
				// last ciphertext block
				b1 = buf1; b2 = buf2;
				// input the IV
				memmove(b1,lpSource,BLOCK_LEN);
				lSourcePos = lSourcePos + BLOCK_LEN;

				while(lSourcePos < (lnSourceDataLength - CRC_SIZE)) // Till you read the end the input
				{
					// input a block and reduce the remaining byte count

					memmove(b2,lpSource + lSourcePos,BLOCK_LEN);
					lSourcePos = lSourcePos + BLOCK_LEN;

					// decrypt input buffer
					AES_decrypt((unsigned char*)b2, (unsigned char*)dbuf, &mDecKey);
					// if there is only one more block do ciphertext stealing
					if((lnSourceDataLength - CRC_SIZE - lSourcePos) > 0 && (lnSourceDataLength - CRC_SIZE - lSourcePos)< BLOCK_LEN)
					{

						// read last ciphertext block
						Int32 iRemaining = lnSourceDataLength - CRC_SIZE - lSourcePos;
						memmove(b2,lpSource + lSourcePos,iRemaining);
						lSourcePos = lSourcePos + iRemaining;
						// append high part of last decrypted block
						for(i = iRemaining ; i < BLOCK_LEN; ++i)
							b2[i] = dbuf[i];

						// decrypt last block of plaintext
						for(i = 0; i < iRemaining; ++i)
							dbuf[i + BLOCK_LEN] = dbuf[i] ^ b2[i];

						// decrypt last but one block of plaintext
						AES_decrypt((unsigned char*)b2, (unsigned char*)dbuf, &mDecKey);

						// unchain CBC using the last ciphertext block
						for(i = 0; i < BLOCK_LEN; ++i)
							dbuf[i] ^= b1[i];

						// write decrypted block
						memmove(lpOutputData.Get() + lDestPos,dbuf,(iRemaining + BLOCK_LEN));
						lDestPos = lDestPos + lnSourceDataLength - CRC_SIZE - lSourcePos + BLOCK_LEN;

					}
					else
					{
						// unchain CBC using the last ciphertext block
						for(i = 0; i < BLOCK_LEN; ++i)
							dbuf[i] ^= b1[i];

						// write decrypted block
						memmove(lpOutputData.Get() + lDestPos,dbuf, BLOCK_LEN);
						lDestPos = lDestPos + BLOCK_LEN;
						// swap the buffer pointers
						bt = b1, b1 = b2, b2 = bt;
					}
				} // end of While

			} // end of if-else

		
			// Checking for authentication i.e. hashing
			// Create the output binary
			MDataType::BinaryPtr lpPlainText(MDataType::CreateBinary(lpOutputData.Get(), lnDestDataLength));

			// Obtain the original hash
			MDataType::BinaryPtr lpOriginalHash(MDataType::CreateBinary(ipcCipherText->c_ptr(), CRC_SIZE));
			MDataType::BinaryPtr lpDecryptedHash(RIPEMD160Hasher::HashBinaryData(lpPlainText.Get(), CRC_SIZE));

			
			if (*lpOriginalHash == *lpDecryptedHash)
			{
				
				return lpPlainText.GiveUp();
			}
			

		} // end if(to check whether valid cipher text is there or not)
	}

	throw MBase::ExceptionImpl<BadDataException>(_T("Corrupted data."));
}

// Begin TEA encryption algorythm
/************************************************

The Tiny Encryption Algorithm (TEA) by
David Wheeler and Roger Needham of the
Cambridge Computer Laboratory.

Placed in the Public Domain by
David Wheeler and Roger Needham.

**** ANSI C VERSION (New Variant) ****

Notes:

TEA is a Feistel cipher with XOR and
and addition as the non-linear mixing
functions.

Takes 64 bits of data in v[0] and v[1].
Returns 64 bits of data in w[0] and w[1].
Takes 128 bits of key in k[0] - k[3].

TEA can be operated in any of the modes
of DES. Cipher Block Chaining is, for example,
simple to implement.

n is the number of iterations. 32 is ample,
16 is sufficient, as few as eight may be OK.
The algorithm achieves good dispersion after
six iterations. The iteration count can be
made variable if required.

Note this is optimised for 32-bit CPUs with
fast shift capabilities. It can very easily
be ported to assembly language on most CPUs.

delta is chosen to be the real part of (the
golden ratio Sqrt(5/4) - 1/2 ~ 0.618034
multiplied by 2^32).

This version has been amended to foil two
weaknesses identified by David A. Wagner
(daw@cs.berkeley.edu): 1) effective key
length of old-variant TEA was 126 not 128
bits 2) a related key attack was possible
although impractical.

************************************************/

void RIPEMD160EncryptorDecryptor::encipher(const unsigned Int32 *const v,unsigned Int32 *const w,
										   const unsigned Int32 *const k) const
{
	register unsigned Int32       y=v[0],z=v[1],sum=0,delta=0x9E3779B9,n=32;
	// Endian convert the given data
	y = MBase::ConvertEndian<unsigned Int32>(y);
	z = MBase::ConvertEndian<unsigned Int32>(z);
	// Endian convert the key
	unsigned Int32 key[KEY_SIZE/4];
	for (Int32 i=0; i<KEY_SIZE/4; i++)
	{
		key[i] = MBase::ConvertEndian<unsigned Int32>(k[i]);
	}

	while(n-->0)
	{
		y+= (z<<4 ^ z>>5) + z ^ sum + key[sum&3];
		sum += delta;
		z+= (y<<4 ^ y>>5) + y ^ sum + key[sum>>11 & 3];
	}

	// Conver the endian back
	y = MBase::ConvertEndian<unsigned Int32>(y);
	z = MBase::ConvertEndian<unsigned Int32>(z);
	w[0]=y; w[1]=z;
}

void RIPEMD160EncryptorDecryptor::decipher(const unsigned Int32 *const v,unsigned Int32 *const w,
										   const unsigned Int32 *const k) const
{
	register unsigned Int32       y=v[0],z=v[1],sum=0xC6EF3720,
		delta=0x9E3779B9,n=32;
	// Endian convert the given data
	y = MBase::ConvertEndian<unsigned Int32>(y);
	z = MBase::ConvertEndian<unsigned Int32>(z);
	// Endian convert the key
	unsigned Int32 key[KEY_SIZE/4];
	for (Int32 i=0; i<KEY_SIZE/4; i++)
	{
		key[i] = MBase::ConvertEndian<unsigned Int32>(k[i]);
	}

	/* sum = delta<<5, in general sum = delta * n */

	while(n-->0)
	{
		z-= (y<<4 ^ y>>5) + y ^ sum + key[sum>>11 & 3];
		sum -= delta;
		y-= (z<<4 ^ z>>5) + z ^ sum + key[sum&3];
	}

	// Conver the endian back
	y = MBase::ConvertEndian<unsigned Int32>(y);
	z = MBase::ConvertEndian<unsigned Int32>(z);
	w[0]=y; w[1]=z;
}

// AES helper function
void RIPEMD160EncryptorDecryptor::cycles(volatile unsigned Int64 *rtn)  const
{

  time_t tt;
  tt     = time(NULL);
  rtn[0] = tt;
  rtn[1] = tt & -36969l;
  return;

}

// AES helper function
void RIPEMD160EncryptorDecryptor::fillrand(char *buf, const Int32 len) const
{
	Int32 count = 4;
    char          r[sizeof(unsigned Int64)];
    Int32  i;

	MSynch::CriticalSection::SmartLock lLock(*mCS);

    for(i = 0; i < len; ++i)
    {
        if(count == 4)
        {
            *(unsigned Int64*)r = RAND(mRandomFill[0], mRandomFill[1]);
            count = 0;
        }

        buf[i] = r[count++];
    }
}

}//namespace
