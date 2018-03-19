//==============================================================================================
//	FILENAME	:	EncryptorDecryptor.h
//	AUTHOR		:	Yuan Ding
//	CREATION	:	11/15/2001
//	Copyright (C) MicroStrategy Incorporated 1999
//  All Rights Reserved
//==============================================================================================
#ifndef MEncryptor_EncryptorDecryptor_h
#define MEncryptor_EncryptorDecryptor_h

#include "Encryptor.h"
#include "Base/Interfaces/BaseException.h"
#include "Base/Base/ReturnString.h"
#include "Base/Defines/StrongPtr.h"
#include "Hash.h"

namespace MDataType
{
	class Binary;
}

namespace MEncryptor
{
	///Encryption algorithm. For details, do a google search of RC4 and/or TEA.
	typedef enum
	{
		
		SIMPLE = -2,
		RC4 = -1,	//RC4 encryption/decryption algorithm.
		TEA = 0,		//TEA encryption/decryption algorithm.
		AES = 1, // AES encryption/decryption algorithm.
	} EncryptionVersion;

	///EncryptorDecryptor takes a string or Data object and encrypts or decrypts
	/** The supported hashing algorithms are:  
	  *		use MD5 as the KeySeed hashing algorithm
	  *		use RIPEMD160 as the KeySeed hashing algorithm 
	  * the supported encryption/decryption algorithms are:
	  *		SIMPLE (version = -2)
	  *		RC4 (version = -1)
	  *		TEA (version = 0)		
	  *     AES (version = 1)
	  */
	class EncryptorDecryptor
	{
	public:

		// a virtual destructor is a must
		virtual ~EncryptorDecryptor() throw()
		{
		}

		///EncryptData takes a plain text wide char string and returns the
		///encrypted data as a wide char string.
		/** @param ipcPlainText is plain text string to be encrypted
		  * @param inVersion must be -2 = SIMPLE, -1 = RC4 or 0 = TEA
		  *		currently:
		  *			if the hashing algorithm is RIPEMD160 then
		  *				only inVersion = 0 (TEA) is supported.
		  *			if the hashing algorithm is MD5 then
		  *				only inVersion = -1 (RC4) is supported
		  * @return is a string that is encrypted
		  * @throw BadEncryptionException (When inVersion is not in the range)
		  */
		virtual MBase::ReturnWString EncryptData(
			const WCHAR* ipcPlainText,
			EncryptionVersion inVersion) const = 0;

		///DecryptData takes a encrypted data as wide char string and 
		///returns a plain text wide char string as the decrypted data.
		/** The encrypted message contains the version information
		  * so there is no need to provide it here
		  * @param ipcCipherText is string to be decrypted
		  * @return is a plain text string that was decrypted
		  * @throw BadDataException (When ipcCipherText is corrupted)
		  */	
		virtual MBase::ReturnWString DecryptData(
			const WCHAR* ipcCipherText) const = 0;

	virtual MDataType::Binary* EncryptData2(
		const WCHAR* ipcPlainText,
		EncryptionVersion inVersion) const = 0;

	virtual MBase::ReturnWString DecryptData2(			
		const void* ipcCipherText,
		size_t inCipherTextSize) const = 0;

		///EncryptData takes a plain text as a byte array and returns the
		///encrypted data as a byte array
		/** @param ipcPlainText is the plain text string to be encrypted
		  * @param inVersion must be -2 = SIMPLE, -1 = RC4 or 0 = TEA
		  *		currently:
		  *			if the hashing algorithm is RIPEMD160 then
		  *				only inVersion = 0 (TEA) is supported.
		  *			if the hashing algorithm is MD5 then
		  *				only inVersion = -1 (RC4) is supported
		  * @return is a byte array that is encrypted
		  * @throw BadEncryptionException (When inVersion is not in the range)
		  */
		virtual MDataType::Binary* EncryptData(
			const MDataType::Binary* ipcPlainText,
			EncryptionVersion inVersion) const = 0;

		///DecryptData takes a encrypted data as a byte array and 
		///returns a byte array as the decrypted data.
		/** The encrypted message contains the version information
		  * so there is no need to provide it here
		  * @param ipcCipherText is the binary data to be decrypted
		  * @return is a plain text data that was decrypted
		  * @throw BadDataException (When ipcCipherText is corrupted)
		  */
		virtual MDataType::Binary* DecryptData(
			const MDataType::Binary* ipcCipherText) const = 0;

		///EncryptData takes a plain text as a byte array and returns the
		///encrypted data as a byte array
		/** @param ipcPlainText is the plain text string to be encrypted
		  * @param inPlainTextSize is the size of the byte of array
		  * @param inVersion must be -2 = SIMPLE, -1 = RC4 or 0 = TEA
		  *		currently:
		  *			if the hashing algorithm is RIPEMD160 then
		  *				only inVersion = 0 (TEA) is supported.
		  *			if the hashing algorithm is MD5 then
		  *				only inVersion = -1 (RC4) is supported
		  * @return is a byte array that is encrypted
		  * @throw BadEncryptionException (When inVersion is not in the range)
		  */
		virtual MDataType::Binary* EncryptData(
			const void* ipcPlainText,
			size_t inPlainTextSize,
			EncryptionVersion inVersion) const = 0;

		///DecryptData takes a encrypted data as a byte array and 
		///returns a byte array as the decrypted data.
		/** The encrypted message contains the version information
		  * so there is no need to provide it here
		  * @param ipcCipherText is the binary data to be decrypted
		  * @param inCipherTextSize is the size of the byte of array
		  * @return is a plain text data that was decrypted
		  * @throw BadDataException (When ipcCipherText is corrupted)
		  */
		virtual MDataType::Binary* DecryptData(
			const void* ipcCipherText,
			size_t inCipherTextSize) const = 0;
	};

	class BadEncryptionException : public MBase::BaseException
	{
	};

	class BadDecryptionException : public MBase::BaseException
	{
	};

	typedef MBase::StrongPtr<EncryptorDecryptor> EncryptorDecryptorPtr;

	///Retrieve the highest encryption/decryption algorithm version 
	///for the encryptor/decryptor that uses RIPEMD160 to hash keys.
	/** The intended use is when the encryptor and the decryptor
	  * may have different versions. The decryptor can get the highest
	  * level of encyption it supports and relay that information
	  * to the encryptor. The encryptor can then down grade and
	  * encrypt the message the decryptor can decrypt.
	  */
	DLL_ENCRYPTOR_EXIM EncryptionVersion GetHighestVersionRIPEMD160();

	///Retrieve the highest encryption/decryption algorithm version 
	///for the encryptor/decryptor that uses MD5 to hash keys.
	/** The intended use is when the encryptor and the decryptor
	  * may have different versions. The decryptor can get the highest
	  * level of encyption it supports and relay that information
	  * to the encryptor. The encryptor can then down grade and
	  * encrypt the message the decryptor can decrypt.
	  */
	DLL_ENCRYPTOR_EXIM EncryptionVersion GetHighestVersionMD5();

	///RIPEMD160 KeySeed factory
	/**
	  * @parameter ipcKeySeed is hashed into a key using RIPEMD160
	  */
	DLL_ENCRYPTOR_EXIM EncryptorDecryptor* CreateRIPEMD160EncryptorDecryptor(
		const WCHAR* ipcKeySeed);

	///RIPEMD160 KeySeed factory
	/**
	  * @parameter ipcKeySeed is hashed into a key using RIPEMD160
	  */
	DLL_ENCRYPTOR_EXIM EncryptorDecryptor* CreateRIPEMD160EncryptorDecryptor(
		MDataType::Binary* ipKey);

	///MD5 KeySeed factory
	/**
	  * @parameter ipcKeySeed is hashed into a key using MD5
	  */
	DLL_ENCRYPTOR_EXIM EncryptorDecryptor* CreateMD5EncryptorDecryptor(
		const WCHAR* ipcKeySeed);

	///Simple Encryptor/Decryptor factory
	DLL_ENCRYPTOR_EXIM EncryptorDecryptor* CreateSIMPLEEncryptorDecryptor();

	///Platform Dependent (PD) Encryptor/Decryptor factory
	/**
	 * Strong encryption algorithms (like RC4) is prohibited from some platforms.
	 * Franch is one of the platforms that we are aware of where RC4 is prohibited.
	 * As a result, a weak encryption algorithm is used in its place. But on all
	 * other platforms, we still use RC4. CreatePDEncryptorDecryptor is a factory 
	 * to create an encryptor based on the locale ID. For those platforms where RC4
	 * is prohibited, an SIMPLE encryptor is created. Otherwise, we create an RC4
	 * encryptor (the key seed is hashed using MD5 hashing algorithm). The
	 * encryption version is carried out with parameter irEncryptionVersion.
	 *
	 * @Param ipcKeySeed is the key seed to create the strong encryptor
	 * @Param irEncryptionVersion is a MEncryptor::EncryptionVersion.
	 */
	DLL_ENCRYPTOR_EXIM EncryptorDecryptor* CreatePDEncryptorDecryptor(
		const WCHAR* ipcKeySeed,
		EncryptionVersion& irEncryptionVersion);
	
	///RIPEMD160 KeySeed factory
	/**
	  * @parameter pcKey : Its character array which represnts hexadecimal values of Key
	              EncryptDecrypt: to indicate whether this is for Encryption or Decryption as Key is created based on that differently
	  */
	//DLL_ENCRYPTOR_EXIM EncryptorDecryptor* CreateAESEncryptorDecryptor(
		//const char* pcKey, bool EncryptDecrypt);

}

#endif // MEncryptor_EncryptorDecryptor_h
