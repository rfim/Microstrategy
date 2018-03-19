//==============================================================================================
//	FILENAME	:	RIPEMD160EncryptorDecryptor.h
//	AUTHOR		:	Yuan Ding
//	CREATION	:	11/20/2001
//	Copyright (C) MicroStrategy Incorporated 1999
//  All Rights Reserved
//==============================================================================================

#ifndef MEncryptor_RIPEMD160EncryptorDecryptor_h
#define MEncryptor_RIPEMD160EncryptorDecryptor_h

#include "Encryptor/Encryptor/EncryptorDecryptor.h"
#include "DataType/DataType/Binary.h"
#include "aes.h"
#include "Synch/Synch/CriticalSection.h"
#include "Synch/Synch/Initializer.h"
//AES for Android
#include "Encryptor/Encryptor/PrivateSource/OpenSSL/aes.h"

//used in AES
#define KEY_LENGTH 16 //(in bytes for KEY)
#define BLOCK_LEN 16 // It will encrypt in blocks of 16 bytes

namespace MEncryptor {

///RIPEMD160EncryptorDecryptor is one implementation of the EncryptorDecryptor interface.
/** It uses the RIPEMD160 hashing algorithm to hash the given key seed to generate the 
  * session key for encryption/decryption. 
  * The only encryption/decryption algorithm version that it supports right now is 
  * TEA (version == 0).
  */
class RIPEMD160EncryptorDecryptor : public EncryptorDecryptor
{
public:
	///Constructor of this implementation
	RIPEMD160EncryptorDecryptor(const WCHAR* ipcKeySeed);

	/// We use this for Android. The KeyStore has already generated the key, so there is no need to generate. 
	/// We pass in the key directly to instantiate the EncryptorDecryptor.
	RIPEMD160EncryptorDecryptor(MDataType::Binary* ipKey);

	///EncryptData takes a plain text wide char string and returns the
	///encrypted data as a wide char string.
	/** @param ipcPlainText is plain text string to be encrypted
	  * @param inVersion must be TEA = 0
	  * @return is a string that is encrypted
	  * @throw BadEncryptionException (When inVersion is not in the range)
	  */
	virtual MBase::ReturnWString EncryptData(
		const WCHAR* ipcPlainText,
		EncryptionVersion inVersion) const;

	///DecryptData takes an encrypted data as wide char string and 
	///returns a plain text wide char string as the decrypted data.
	/** The encrypted message contains the version information
	  * so there is no need to provide it here
	  * @param ipcCipherText is string to be decrypted
	  * @return is a plain text string that was decrypted
	  * @throw BadDecryptionException (When ipcCipherText is corrupted)
	  */	
	virtual MBase::ReturnWString DecryptData(
		const WCHAR* ipcCipherText) const;

	virtual MDataType::Binary* EncryptData2(
													const WCHAR* ipcPlainText,
													EncryptionVersion inVersion) const;

	virtual MBase::ReturnWString DecryptData2(
		const void* ipcCipherText,
		size_t inCipherTextSize) const;

	///EncryptData takes a plain text as a byte array and returns the
	///encrypted data as a byte array
	/** @param ipcPlainText is the plain text string to be encrypted
	  * @param inVersion must be 0 = TEA
	  * @return is a byte array that is encrypted
	  * @throw BadEncryptionException (When inVersion is not in the range)
	  */
	virtual MDataType::Binary* EncryptData(
		const MDataType::Binary* ipcPlainText,
		EncryptionVersion inVersion) const;

	///DecryptData takes a encrypted data as a byte array and 
	///returns a byte array as the decrypted data.
	/** The encrypted message contains the version information
	  * so there is no need to provide it here
	  * @param ipcCipherText is the binary data to be decrypted
	  * @return is a plain text data that was decrypted
	  * @throw BadDataException (When ipcCipherText is corrupted)
	  */
	virtual MDataType::Binary* DecryptData(
		const MDataType::Binary* ipcCipherText) const;

	///EncryptData takes a plain text as a byte array and returns the
	///encrypted data as a byte array
	/** @param ipcPlainText is the plain text string to be encrypted
	  * @param inPlainTextSize is the size of the byte of array
	  * @param inVersion must be 0 = TEA
	  * @return is a byte array that is encrypted
	  * @throw BadEncryptionException (When inVersion is not in the range)
	  */
	virtual MDataType::Binary* EncryptData(
		const void* ipcPlainText,
		size_t inPlainTextSize,
		EncryptionVersion inVersion) const;

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
		size_t inCipherTextSize) const;

private:
	///Destructor
	virtual ~RIPEMD160EncryptorDecryptor() throw();

	///Helper the actually performs the encryption.
	///Both input and output are binary.
	MDataType::Binary* hEncryptData(
		const MDataType::Binary* ipcPlainText,
		EncryptionVersion inVersion) const;

	///Helper the actually performs the decryption.
	///Both input and output are binary.
	MDataType::Binary* hDecryptData(
		const MDataType::Binary* ipcCipherText,
		EncryptionVersion inVersion) const;


	// The TEA encryption algorythm, see the implememtation for details
	void encipher(const unsigned int *const v,unsigned int *const w,const unsigned int *const k) const;
	void decipher(const unsigned int *const v,unsigned int *const w,const unsigned int *const k) const;

	///Session Key used for encryption and decryption.
	MDataType::BinaryPtr mpSessionKey;

	//AES
		const MSynch::Initializer mSynchInitializer;
		mutable aes_ctx    mctx_enc; // For aes algorithm it needs schedule key to be generated from the key
		mutable aes_ctx    mctx_dec;
		mutable unsigned Int64 mRandomFill[2];
		MSynch::CriticalSectionPtr mCS;
		void fillrand(char *buf, const int len) const; //helper function for schedule key

		//for OpenSSL AES
		AES_KEY mEncKey;
		AES_KEY mDecKey;
	
	/* #ifdef WIN32
		void cycles(volatile unsigned __int64 *rtn)  const;   	// helper function for fillrand 
	#else
		void cycles(volatile unsigned long long *rtn)  const   ; 	// helper function for fillrand 
	#endif
	*/
		void cycles(volatile unsigned Int64 *rtn)  const;   	// helper function for fillrand 

	///The size of the session key in bytes (TEA).
	const static unsigned int KEY_SIZE;
	///The size of the block in bytes (TEA).
	const static unsigned int BLOCK_SIZE;
	///The size of the signature added to the ciphered text in bytes.
	const static unsigned int CRC_SIZE;

};

} //namespace

#endif //MEncryptor_RIPEMD160EncryptorDecryptor_h
