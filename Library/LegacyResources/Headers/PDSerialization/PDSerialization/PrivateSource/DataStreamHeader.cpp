//==============================================================================================
//	FILENAME	:	DataStreamHeader.cpp
//	AUTHOR		:	Yi Luo
//	CREATION	:	2002-09-05
//
//	Copyright (C) MicroStrategy, Inc. 2002
//==============================================================================================

#include "StdAfx.h"
#include "PDSerialization/PDSerialization/PDSerialization.h"
#include "Facilities.h"
#include "Base/Defines/ExceptionImplementation.h"
#ifndef __ANDROID__
	#include "Encryptor/Encryptor/EncryptorDecryptor.h"
#endif
#include "DataStreamHeader.h"

namespace MSerialization
{

DataStreamHeader::DataStreamHeader() throw()
: mValue(1)
{
}

DataStreamHeader::~DataStreamHeader() throw()
{
}

void DataStreamHeader::Load(unsigned char* ipReadMemory)
{
	mValue = Read(&ipReadMemory);

	unsigned char lFlag = *ipReadMemory;
	ipReadMemory++;

	if ( lFlag != GN_Data_Stream_Flag )
	{
		throw  MBase::ExceptionImplementation<ReadStreamException>("Corrupted stream: wrong header flag.");
	}
	
	short lComp = Read(&ipReadMemory);

	if ( lComp != (short) ~mValue)
	{
		throw  MBase::ExceptionImplementation<ReadStreamException>("Corrupted stream: wrong header data.");
	}
}

void DataStreamHeader::Load(ReadByteStream * ipReadStream)
{
	mValue = Read(ipReadStream);

	unsigned char lFlag = 0;
	ipReadStream->Read(&lFlag, (unsigned Int32) 1);
	if ( lFlag != GN_Data_Stream_Flag )
	{
		throw  MBase::ExceptionImplementation<ReadStreamException>("Corrupted stream: wrong header flag.");
	}
	
	short lComp = Read(ipReadStream);

	if ( lComp != (short) ~mValue)
	{
		throw  MBase::ExceptionImplementation<ReadStreamException>("Corrupted stream: wrong header data.");
	}

	if (mValue& (PDS_RC4_ENCRYPT_STREAM |PDS_TEA_ENCRYPT_STREAM |PDS_AES_ENCRYPT_STREAM))
	{
		unsigned short lLen = Read(ipReadStream);
		MDataType::BinaryPtr lBinary(MDataType::CreateBinary(lLen));
		ipReadStream->Read((unsigned char*)lBinary->c_ptr(), (unsigned int)lLen);
		mKey.Reset(lBinary.GiveUp());
	}
}


void DataStreamHeader::Save(WriteByteStream * ipWriteStream) const
{
	try
	{
		Write( mValue, ipWriteStream);

		unsigned char ldata = GN_Data_Stream_Flag;
		ipWriteStream->Write(&ldata, (unsigned Int32)1);
		
		unsigned short lHeader = ~ mValue;
		Write( lHeader, ipWriteStream);

		if (mValue& (PDS_RC4_ENCRYPT_STREAM |PDS_TEA_ENCRYPT_STREAM |PDS_AES_ENCRYPT_STREAM))
		{
			unsigned short lLen = static_cast<unsigned short>(mKey->GetSize());
			Write(lLen, ipWriteStream);
			ipWriteStream->Write((const unsigned char*)mKey->c_ptr(), (unsigned int)lLen);
		}
	}
	catch ( WriteStreamException& )
	{
		throw MBase::ExceptionImplementation<WriteStreamException>("Cannot write data stream header");
	}
}

bool DataStreamHeader::GetLongVersion() const throw()
{
	return ( mValue & PDS_SAFE_STREAM ) != 0;
}

bool DataStreamHeader::GetRawFormat() const throw()
{
	return ( mValue & PDS_RAW_STREAM ) != 0;
}

bool DataStreamHeader::IsBigEndian() const throw()
{
	return ( mValue & PDS_BIG_ENDIAN_STREAM ) != 0;
}

unsigned short DataStreamHeader::GetHeaderValue() const throw()
{
	return mValue;
}

void DataStreamHeader::SetHeaderValue(unsigned short iValue) throw()
{
	mValue = iValue;
}

Int32 DataStreamHeader::GetHeaderSize() throw()
{
	Int32 lSize = sizeof(short) + sizeof(char) + sizeof(short);
	if (mValue& (PDS_RC4_ENCRYPT_STREAM |PDS_TEA_ENCRYPT_STREAM |PDS_AES_ENCRYPT_STREAM))
	{
		lSize += sizeof(short) + static_cast<Int32>(mKey->GetSize());
	}
	return lSize;
}

void DataStreamHeader::Write(unsigned short iValue, WriteByteStream * ipoStream) const
{
#ifdef LITTLE_ENDIAN
	ipoStream->Write((const unsigned char *) &iValue, (unsigned int)sizeof(iValue) );
#else
	unsigned char* lpPtr = reinterpret_cast<unsigned char*>(& iValue);
	ipoStream->Write(lpPtr+1, (unsigned Int32) 1);
	ipoStream->Write(lpPtr, (unsigned Int32) 1);
#endif
}

unsigned short DataStreamHeader::Read(unsigned char** ioppMemory)
{
	unsigned short lValue = 0;

#ifdef LITTLE_ENDIAN
	::memmove((void*) &lValue, *ioppMemory, sizeof(lValue) );
	(*ioppMemory)++;
	(*ioppMemory)++;
#else
	unsigned char* lpPtr = reinterpret_cast<unsigned char*>(& lValue);
	*(lpPtr+1) = **ioppMemory;
	(*ioppMemory)++;
	*lpPtr = **ioppMemory;
	(*ioppMemory)++;
#endif
	return lValue;
}

unsigned short DataStreamHeader::Read(ReadByteStream * ipReadStream)
{
	unsigned short lValue = 0;

#ifdef LITTLE_ENDIAN
	ipReadStream->Read((unsigned char*) &lValue, (unsigned int)sizeof(lValue));
#else
	unsigned char* lpPtr = reinterpret_cast<unsigned char*>(& lValue);
	ipReadStream->Read(lpPtr+1, (unsigned Int32) 1);
	ipReadStream->Read(lpPtr, (unsigned Int32) 1);
#endif
	return lValue;
}
#ifndef __ANDROID__
MBase::ReturnString DataStreamHeader::GetKey() const throw()
{
	const wchar_t* lKeyBase = L"30F9C67D-7125-4d18-9DA6-8B477E326A91";
	//std::string lHashed(MBase::WideCharToUTF8(MEncryptor::Hash::HashData(lKeyBase, MEncryptor::RIPEMD160).c_str()).c_str());
	//std::string lKey(lHashed.substr(8, 16));
	MEncryptor::EncryptorDecryptorPtr lEncryptor(MEncryptor::CreateRIPEMD160EncryptorDecryptor(lKeyBase));
	MDataType::BinaryPtr lBinary(lEncryptor->DecryptData(mKey->c_ptr(), mKey->GetSize()));
	return (char*)lBinary->c_ptr();
}

void DataStreamHeader::SetKey(const char* ipcKey) throw()
{
	const wchar_t* lKeyBase = L"30F9C67D-7125-4d18-9DA6-8B477E326A91";
	//std::string lHashed(MBase::WideCharToUTF8(MEncryptor::Hash::HashData(lKeyBase, MEncryptor::RIPEMD160).c_str()).c_str());
	//std::string lKey(lHashed.substr(8, 16));
	MEncryptor::EncryptorDecryptorPtr lEncryptor(MEncryptor::CreateRIPEMD160EncryptorDecryptor(lKeyBase));
	mKey.Reset(lEncryptor->EncryptData(ipcKey, strlen(ipcKey)+1, MEncryptor::AES));
}
#endif
}


