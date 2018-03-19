//==============================================================================================
//	FILENAME	:	WriteDataStreamImpl.cpp
//	AUTHOR		:	Yi Luo
//	CREATION	:	07/10/2002
//	Copyright (C) MicroStrategy, Inc. 2002
//==============================================================================================
#include "StdAfx.h"

#include "WriteDataStreamImpl.h"
#include "PDSerialization/PDSerialization/ReadDataStream.h"
#include "Facilities.h"
#include "StreamInt64ReserveImpl.h"
#include "Base/Base/UTFConverter.h"
#include "Base/Defines/ExceptionImplementation.h"
#include "DataType/DataType/Variant.h"
#include "DataType/DataType/DynamicTime.h"
#include "DataStreamHeader.h"

#include "WriteByteStreamFileImpl.h"

#define RET_BOOL_BY_HR(hr) 	if ( SUCCEEDED(hr) )  return true;	else return false;
#define RET_FALSE_ON_FAIL(hr) 		if ( FAILED( hr ) )	return false;
#define RET_ON_FALSE(ret) 		if ( ! ret  )	return;

namespace MSerialization
{

#define STREAM_BLOCK_SIZE 1024

/*------------------------
	public funtions
-------------------------*/

// factory functions

WriteDataStream*
CreateWriteDataStream(MBase::MemoryContract * ipMemCon, unsigned short header )
{
	_ASSERTE( header < 0x2000 );  // because we reserve bit above 0x2000 for special flags
	return new WriteDataStreamImpl(true, STREAM_BLOCK_SIZE, ipMemCon, header);
}

WriteDataStream*
CreateRawWriteDataStream(MBase::MemoryContract * ipMemCon, unsigned short header )
{
	_ASSERTE( header < 0x2000 );

#ifdef LITTLE_ENDIAN
	return new WriteDataStreamImpl(true, STREAM_BLOCK_SIZE, ipMemCon, header | PDS_RAW_STREAM );
#else
	return new WriteDataStreamImpl(true, STREAM_BLOCK_SIZE, ipMemCon, header | PDS_RAW_STREAM | PDS_BIG_ENDIAN_STREAM );
#endif
}

WriteDataStream*
CreateSafeWriteDataStream(MBase::MemoryContract * ipMemCon, unsigned short header )
{
	_ASSERTE( header < 0x2000 );
	return new WriteDataStreamImpl(true, STREAM_BLOCK_SIZE, ipMemCon, header | PDS_SAFE_STREAM );
}

WriteDataStream*
CreateSafeRawWriteDataStream(MBase::MemoryContract * ipMemCon, unsigned short header )
{
	_ASSERTE( header < 0x2000 );

#ifdef LITTLE_ENDIAN
	return new WriteDataStreamImpl(true, STREAM_BLOCK_SIZE, ipMemCon, header | PDS_SAFE_STREAM | PDS_RAW_STREAM);
#else
	return new WriteDataStreamImpl(true, STREAM_BLOCK_SIZE, ipMemCon, header | PDS_SAFE_STREAM | PDS_RAW_STREAM | PDS_BIG_ENDIAN_STREAM );
#endif
}

WriteDataStream*
ConvertBinaries2WriteDataStream(MDataType::BinaryCollection* ipBins,
								MBase::MemoryContract * ipMemCon)
{
	return new WriteDataStreamImpl(ipBins, STREAM_BLOCK_SIZE, ipMemCon);
}

WriteDataStream* CreateWriteDataStreamWithNoHeader(bool iRawFormat, unsigned short header, MBase::MemoryContract * ipMemCon)
{
	_ASSERTE( header < 0x2000 );

	if ( iRawFormat )
	{
#ifdef LITTLE_ENDIAN
		return new WriteDataStreamImpl(false, 256, ipMemCon, header | PDS_RAW_STREAM);
#else
		return new WriteDataStreamImpl(false, 256, ipMemCon, header | PDS_RAW_STREAM | PDS_BIG_ENDIAN_STREAM );
#endif
	}
	else
		return new WriteDataStreamImpl(false, 256, ipMemCon, header);
}

WriteDataStream*
CreateWriteDataStream(MFileSystem::WriteableFile::Ptr& iFilePtr, unsigned short header )
{
	_ASSERTE( header < 0x2000 );  // because we reserve bit above 0x2000 for special flags
	return new WriteDataStreamImpl(true, STREAM_BLOCK_SIZE, iFilePtr, header);
}

WriteDataStream*
CreateRawWriteDataStream(MFileSystem::WriteableFile::Ptr& iFilePtr, unsigned short header )
{
	_ASSERTE( header < 0x2000 );

#ifdef LITTLE_ENDIAN
	return new WriteDataStreamImpl(true, STREAM_BLOCK_SIZE, iFilePtr, header | PDS_RAW_STREAM );
#else
	return new WriteDataStreamImpl(true, STREAM_BLOCK_SIZE, iFilePtr, header | PDS_RAW_STREAM | PDS_BIG_ENDIAN_STREAM );
#endif
}

WriteDataStream*
CreateSafeWriteDataStream(MFileSystem::WriteableFile::Ptr& iFilePtr, unsigned short header )
{
	_ASSERTE( header < 0x2000 );
	return new WriteDataStreamImpl(true, STREAM_BLOCK_SIZE, iFilePtr, header | PDS_SAFE_STREAM );
}

WriteDataStream*
CreateSafeRawWriteDataStream(MFileSystem::WriteableFile::Ptr& iFilePtr, unsigned short header )
{
	_ASSERTE( header < 0x2000 );

#ifdef LITTLE_ENDIAN
	return new WriteDataStreamImpl(true, STREAM_BLOCK_SIZE, iFilePtr, header | PDS_SAFE_STREAM | PDS_RAW_STREAM);
#else
	return new WriteDataStreamImpl(true, STREAM_BLOCK_SIZE, iFilePtr, header | PDS_SAFE_STREAM | PDS_RAW_STREAM | PDS_BIG_ENDIAN_STREAM );
#endif
}

#if !defined(WIN32) && !defined(__ANDROID__)
WriteDataStream*
CreateWriteDataStreamToSharedMem(MMultiProcess::SharedMemory* ipSharedMemPtr, unsigned Int32 iBlockSize)
{
	return new WriteDataStreamImpl(ipSharedMemPtr, iBlockSize);
}
#endif

WriteDataStream*
CreateWriteDataStreamToSharedMemByteStream(
		MSerialization::WriteByteStream* ipSharedMemWriteByteStreamPtr)
{
	return new WriteDataStreamImpl(ipSharedMemWriteByteStreamPtr);
}

/*=====================================================
=====================================================
|| 			Serialization Functions					||
=====================================================
=====================================================*/

// unsigned char MSerialization::GN_Data_Stream_Flag = 0xBE;

/*-------- 1 byte / binary data ---------*/

WriteDataStream& WriteDataStreamImpl::operator << (const MDataType::Binary &v)
{
	WriteType(T_BINARY);

	// write length. We use 64-bit integer for expandability.
	Int64 lLen = v.GetSize();
	(*this) << lLen;

	// write complementary length for extra robustness
	Int64 lComp = ~ lLen;
	(*this) << lComp;

	mpBStream->Write((const unsigned char *) v.c_ptr(), lLen);

	if ( mLongVer )
	{
		(*this) << lLen;
	}
	return *this;
}

WriteDataStream& WriteDataStreamImpl::operator <<(bool v)
{
	WriteType(T_BOOL);

	mpBStream->Write((const unsigned char *)&v,(unsigned Int32) sizeof(char));

	return *this;
}

void WriteDataStreamImpl::Write(const bool *pv, unsigned Int32 cb)
{
	WriteType(T_BOOL, cb);

	mpBStream->Write((const unsigned char *)pv,(unsigned Int32)( cb * sizeof(bool) ) );
}

void WriteDataStreamImpl::Write(const unsigned char *pv, unsigned Int32 cb)
{
	WriteType(T_RAWBINARY, cb);
#ifndef __ANDROID__
	if (mHeader & (PDS_SIMPLE_ENCRYPT_STREAM | PDS_RC4_ENCRYPT_STREAM |PDS_TEA_ENCRYPT_STREAM |PDS_AES_ENCRYPT_STREAM))
	{
		MDataType::BinaryPtr lBinary(mEncryptor->EncryptData(pv, cb, mEncryptorVersion));
		unsigned Int32 lLen = static_cast<unsigned Int32>(lBinary->GetSize());
		unsigned char* lPtr = (unsigned char*)lBinary->c_ptr();
		(*this) << lLen;
		mpBStream->Write(lPtr,(unsigned Int32)(lLen * sizeof(unsigned char)));
	}
	else
#endif
	{
		mpBStream->Write((const unsigned char *)pv,(unsigned Int32)( cb * sizeof(unsigned char) ) );
	}
}


/*-------- 2 byte ---------*/

WriteDataStream& WriteDataStreamImpl::operator <<(short v)
{
	WriteType(T_SHORT);
	#ifdef LITTLE_ENDIAN
		mpBStream->Write((const unsigned char *) &v, (unsigned Int32)sizeof(v) );
	#else
		if ( mRawFormat)
			mpBStream->Write((const unsigned char *) &v, (unsigned Int32)sizeof(v) );
		else
			ConvertBigEndianToLittleEndian(2, (const unsigned char*)&v,(unsigned Int32) 1 );
	#endif

	return *this;
}

WriteDataStream& WriteDataStreamImpl::operator <<(unsigned short v)
{
	WriteType(T_USHORT);

	#ifdef LITTLE_ENDIAN
		mpBStream->Write((const unsigned char *) &v, (unsigned Int32)sizeof(v) );
	#else
		if ( mRawFormat)
			mpBStream->Write((const unsigned char *) &v, (unsigned Int32)sizeof(v) );
		else
			ConvertBigEndianToLittleEndian(2, (const unsigned char*)&v, (unsigned Int32) 1 );
	#endif

	return *this;
}

void WriteDataStreamImpl::Write(const short *pv, unsigned Int32 cb)
{
	WriteType(T_SHORT, cb);

	#ifdef LITTLE_ENDIAN
		mpBStream->Write((const unsigned char *) pv, (unsigned Int32)(cb * sizeof(short)) );
	#else
		if ( mRawFormat)
			mpBStream->Write((const unsigned char *) pv, (unsigned Int32)(cb * sizeof(short)) );
		else
			ConvertBigEndianToLittleEndian(2, (const unsigned char*)(pv), cb);
	#endif
}

/*-------- 4 byte ---------*/

WriteDataStream& WriteDataStreamImpl::operator <<(Int32 v)
{
	WriteType(T_INT);

	#ifdef LITTLE_ENDIAN
		mpBStream->Write((const unsigned char *) &v, (unsigned Int32)sizeof(Int32) );
	#else
		if ( mRawFormat)
			mpBStream->Write((const unsigned char *) &v, (unsigned Int32)sizeof(Int32) );
		else
			ConvertBigEndianToLittleEndian(4, ( const unsigned char*) &v, (unsigned Int32) 1 );
	#endif

	return *this;
}

WriteDataStream& WriteDataStreamImpl::operator <<(unsigned Int32 v)
{
	WriteType(T_UINT);

	#ifdef LITTLE_ENDIAN
		mpBStream->Write((const unsigned char *) &v, (unsigned Int32)sizeof(Int32) );
	#else
		if ( mRawFormat)
			mpBStream->Write((const unsigned char *) &v, (unsigned Int32)sizeof(Int32) );
		else
			ConvertBigEndianToLittleEndian(4, ( const unsigned char*)&v, (unsigned Int32) 1 );
	#endif

	return *this;
}

void WriteDataStreamImpl::Write(const Int32 *pv, unsigned Int32 cb)
{
	WriteType(T_INT, cb);

	#ifdef LITTLE_ENDIAN
		mpBStream->Write((const unsigned char *) pv, (unsigned Int32)(cb * sizeof(Int32)) );
	#else
		if ( mRawFormat)
			mpBStream->Write((const unsigned char *) pv, (unsigned Int32)(cb * sizeof(Int32)) );
		else
			ConvertBigEndianToLittleEndian(4, (const unsigned char*) pv, cb);
	#endif
}

/*-------- 8 byte ---------*/

WriteDataStream& WriteDataStreamImpl::operator <<(Int64 v)
{
	WriteType(T_INT64);

	#ifdef LITTLE_ENDIAN
		mpBStream->Write((const unsigned char *) &v, (unsigned Int32)sizeof(Int64) );
	#else
		if ( mRawFormat)
			mpBStream->Write((const unsigned char *) &v, (unsigned Int32)sizeof(Int64) );
		else
			ConvertBigEndianToLittleEndian(8, (const unsigned char*) &v, (unsigned Int32) 1 );
	#endif

	return *this;
}

void WriteDataStreamImpl::Write(const Int64 * pv, unsigned Int32 cb)
{
	WriteType(T_INT64, cb);

	#ifdef LITTLE_ENDIAN
		mpBStream->Write((const unsigned char *) pv, (unsigned Int32)(cb * sizeof(Int64)));
	#else
		if ( mRawFormat)
			mpBStream->Write((const unsigned char *) pv, (unsigned Int32)(cb * sizeof(Int64)));
		else
			ConvertBigEndianToLittleEndian(8, ( const unsigned char*) pv, cb);
	#endif
}

WriteDataStream& WriteDataStreamImpl::operator <<(float v)
{
	WriteType(T_FLOAT);

	#ifdef LITTLE_ENDIAN
		mpBStream->Write((const unsigned char *) &v, (unsigned Int32)sizeof(v) );
	#else
		if ( mRawFormat)
			mpBStream->Write((const unsigned char *) &v, (unsigned Int32)sizeof(v) );
		else
			ConvertBigEndianToLittleEndian(4, ( const unsigned char*) &v, (unsigned Int32) 1 );
	#endif

	return *this;
}

void WriteDataStreamImpl::Write(const float *pv, unsigned Int32 cb)
{
	WriteType(T_FLOAT, cb);

	#ifdef LITTLE_ENDIAN
		mpBStream->Write((const unsigned char *) pv, (unsigned Int32)(cb * sizeof(float)));
	#else
		if ( mRawFormat)
			mpBStream->Write((const unsigned char *) pv, (unsigned Int32)(cb * sizeof(float)));
		else
			ConvertBigEndianToLittleEndian(4, ( const unsigned char*) pv, cb);
	#endif
}

WriteDataStream& WriteDataStreamImpl::operator <<(double v)
{
	WriteType(T_DOUBLE);

	#ifdef LITTLE_ENDIAN
		mpBStream->Write((const unsigned char *) &v, (unsigned Int32)sizeof(v) );
	#else
		if ( mRawFormat)
			mpBStream->Write((const unsigned char *) &v, (unsigned Int32)sizeof(v) );
		else
			ConvertBigEndianToLittleEndian(8, ( const unsigned char*) &v, (unsigned Int32) 1 );
	#endif
	return *this;
}

void WriteDataStreamImpl::Write(const double *pv, unsigned Int32 cb)
{
	WriteType(T_DOUBLE, cb);

	#ifdef LITTLE_ENDIAN
		mpBStream->Write((const unsigned char *) pv, (unsigned Int32)(cb * sizeof(double)));
	#else
		if ( mRawFormat)
			mpBStream->Write((const unsigned char *) pv, (unsigned Int32)(cb * sizeof(double)));
		else
			ConvertBigEndianToLittleEndian(8, ( const unsigned char*) pv, cb);
	#endif
}

void WriteDataStreamImpl::WriteByte(char iByte)
{
	WriteType(T_BYTE);

	mpBStream->Write((unsigned char *)&iByte,(unsigned Int32)(sizeof(char) ) );
}

/*-------- complext type ---------*/
#ifndef NO_WCHAR_SUPPORT
void WriteDataStreamImpl::WriteWChar(const wchar_t iChar)
{
	WriteType(T_UTF8_CHAR);

	wchar_t lWStr[2];
	lWStr[0] = iChar;
	lWStr[1] = 0;

	try
	{
		const MBase::ReturnString lStr(MBase::WideCharToUTF8Governed(lWStr, false, mMemConPtr));

		Int64 lLen = lStr.GetLength() + 1; // including the terminal NULL
		_ASSERT( lLen > 0 );

		// write the length of the string
		(*this) << lLen;

		// write the complementary value of the length
		Int64 lNew = ~lLen;
		(*this) << lNew;

		mpBStream->Write((const unsigned char*)lStr.c_str() , lLen);
	}
	catch (MBase::StringConversionException& )
	{
		throw MBase::ExceptionImplementation<WriteStreamException>("WideCharToUTF8 conversion failure.");
	}
	catch (MBase::ContractManagerAllocException&) {
		throw MBase::ExceptionImplementation<WriteStreamException>("Memory contracting failed during WideCharToUTF8 conversion.");
	}
}
#endif

/*	typedef struct  _GUID
	{
		unsigned long Data1 = 0;
		unsigned short Data2;
		unsigned short Data3;
		unsigned char Data4[ 8 ];
	} GUID; */

WriteDataStream& WriteDataStreamImpl::operator <<(const GUID & v)
{
	WriteType(T_GUID);

	// long
	unsigned Int32 lD = v.Data1;
	(*this) << lD;

	// short
	(*this) << v.Data2;

	(*this) << v.Data3;

	mpBStream->Write( (const unsigned char*) v.Data4, (unsigned Int32) 8);

	return *this;
}

void WriteDataStreamImpl::Write(const GUID *pv, unsigned Int32 cb)
{
	WriteType(T_GUID, cb);

	for(Int64 i = 0; i < cb ; i ++)
	{	// for each GUID of the array

		// long
		unsigned Int32 lD = pv->Data1;
		(*this) << lD;

		// short
		(*this) << pv->Data2;

		(*this) << pv->Data3;

		mpBStream->Write(( const unsigned char*) pv->Data4, (unsigned Int32) 8);

		pv++;
	}
}

WriteDataStream& WriteDataStreamImpl::operator << (const MDataType::DateTime & v)
{
	WriteType(T_DATETIME);

	// fcamargo 12/18/02
	// Removed the code that serialized the DateTime as binary because it didn't work

	// NOTE: We don't need to serialize the week and dayofweek because they'll be calculated
	// from the month and day

	// int mnYear
	(*this) <<  v.GetYear();

	// 	int mnMonth;
	(*this) << v.GetMonth();

	// 	int mnDay;
	(*this) << v.GetDay();

	// 	int mnHour;
	(*this) << v.GetHour();

	// 	int mnMinute;
	(*this) << v.GetMinute();

	// 	int mnSecond;
	(*this) << v.GetSecond();

	// 	int mnMillisecond;
	(*this) << v.GetMillisecond();

	return *this;
}

void WriteDataStreamImpl::Write(const MDataType::DateTime *pv, unsigned Int32 cb)
{
	WriteType(T_DATETIME, cb);

	// fcamargo 12/18/02
	// Removed the code that serialized the DateTime as binary because it didn't work

	const MDataType::DateTime * lpD = pv;
	for(Int64 i = 0; i < cb ; i ++)
	{	// for each DateTime
		(*this) << *pv;

		lpD ++;
	}
}

WriteDataStream& WriteDataStreamImpl::operator << (const MDataType::DynamicTime & v)
{
	WriteType(T_DYNAMIC_TIME);

	// fcamargo 12/18/02
	// Removed the code that serialized the DateTime as binary because it didn't work

	// int mnYear
	(*this) <<  v.GetYear();

	// 	int mnMonth;
	(*this) << v.GetMonth();

	// 	int mnDay;
	(*this) << v.GetDay();

	// int mnWeek
	(*this) << v.GetWeek();

	//  int mnDayOfWeek;
	(*this) << v.GetDayOfWeek();

	// 	int mnHour;
	(*this) << v.GetHour();

	// 	int mnMinute;
	(*this) << v.GetMinute();

	// 	int mnSecond;
	(*this) << v.GetSecond();

	// Dynamic Day Offset
	(*this) << v.GetDynamicDayOffset();

	// Dynamic time
	Int32 lDynTime = v.GetDynamicTime();
	if (lDynTime!=MDataType::NOT_DYNAMIC_TIME)
	{
		short lDynMonOffset = v.GetDynamicMonthOffset();
		lDynTime |= lDynMonOffset << 16;
	}
	(*this) << lDynTime;

	return *this;
}

void WriteDataStreamImpl::Write(const MDataType::DynamicTime *pv, unsigned Int32 cb)
{
	WriteType(T_DYNAMIC_TIME, cb);

	// fcamargo 12/18/02
	// Removed the code that serialized the DateTime as binary because it didn't work

	const MDataType::DynamicTime * lpD = pv;
	for(Int64 i = 0; i < cb ; i ++)
	{	// for each DynamicTime
		(*this) << *pv;

		lpD ++;
	}
}

WriteDataStream& WriteDataStreamImpl::operator << (const MDataType::CellFmtData & v)
{
	WriteType(T_CELLFMTDATA);

	(*this) << v.GetValue();
	(*this) << (Int32)v.GetType();
	(*this) << v.GetFmtID();

	return *this;
}

void WriteDataStreamImpl::Write(const MDataType::CellFmtData *pv, unsigned Int32 cb)
{
	WriteType(T_CELLFMTDATA, cb);

	const MDataType::CellFmtData * lpD = pv;
	for(Int64 i = 0; i < cb ; i ++)
	{
		(*this) << *pv;
		lpD ++;
	}
}


WriteDataStream& WriteDataStreamImpl::operator << (const MDataType::Variant & pv)
{
	WriteType(T_VARIANT);

	MDataType::Variant::DATA_TYPE lDT = pv.GetVarType();

	(*this) << (Int32)lDT;

	switch ( lDT )
	{
	case MDataType::Variant::MSI_SHORT:
		(*this) << (short) pv;
		break;
	case MDataType::Variant::MSI_INTEGER:
		(*this) << (Int32) pv;
		break;
	case MDataType::Variant::MSI_LONG:
		(*this) << (Int32) pv;
		break;
	case MDataType::Variant::MSI_FLOAT:
		(*this) << (double)((float) pv);
		break;
	case MDataType::Variant::MSI_DOUBLE:
		(*this) << (double) pv;
		break;

	case MDataType::Variant::MSI_DATE:
	case MDataType::Variant::MSI_TIME:
	case MDataType::Variant::MSI_DATETIME:

#if defined(WIN32) || defined(sun)
		(*this) << ( ( const MDataType::DateTime ) pv );
#elif defined(_AIX) || defined(linux) || defined(__hpux) || defined(__ANDROID__)
		(*this) << (pv.operator MDataType::DateTime)();
#else
		#error UNKNOWN PLATFORM
#endif
		break;

	case MDataType::Variant::MSI_CELLFMTDATA:
#if defined(WIN32) || defined(sun) 
		(*this) << ( ( const MDataType::CellFmtData ) pv );
#elif defined(_AIX) || defined(linux) || defined(__hpux) || defined(__ANDROID__)
		(*this) << (pv.operator MDataType::CellFmtData)();
#else
		#error UNKNOWN PLATFORM
#endif
		break;

	case MDataType::Variant::MSI_BINARY:
		(*this) << *(pv.ToBinary());
		break;

	case MDataType::Variant::MSI_INVALID:
		// there is no content, and we already wrote datatype, so write nothing here.
		break;

	case MDataType::Variant::MSI_STRING:
	case MDataType::Variant::MSI_MBCS_STRING:
#ifdef NO_WCHAR_SUPPORT
		(*this) << pv.ToString().c_str();
#else
		(*this) << pv.ToWString().c_str();
#endif
		break;

	default:
		_ASSERT(false);
		throw MBase::ExceptionImplementation<WriteStreamException>("Unrecognized Variant Type.");
		break;
	}

	return *this;
}

void WriteDataStreamImpl::Write(const MDataType::Variant *pv, unsigned Int32 cb)
{
	WriteType(T_VARIANT, cb);

	const MDataType::Variant * lpV = pv;
	for(Int64 i = 0; i < cb ; i ++)
	{	// for each Variant

		MDataType::Variant::DATA_TYPE lDT = lpV->GetVarType();

		(*this) << (Int32)lDT;

		switch ( lDT )
		{
		case MDataType::Variant::MSI_SHORT:
			(*this) << (short) *lpV;
			break;
		case MDataType::Variant::MSI_INTEGER:
			(*this) << (Int32) *lpV;
			break;
		case MDataType::Variant::MSI_LONG:
			(*this) << (Int32) *lpV;
			break;
		case MDataType::Variant::MSI_FLOAT:
			(*this) << (double)((float) *lpV);
			break;
		case MDataType::Variant::MSI_DOUBLE:
			(*this) << (double) *lpV;
			break;

		case MDataType::Variant::MSI_DATE:
		case MDataType::Variant::MSI_TIME:
		case MDataType::Variant::MSI_DATETIME:

#if defined(WIN32) || defined(sun)
		  	(*this) << (const MDataType::DateTime) *lpV;
#elif defined(_AIX) || defined(linux) || defined(__hpux) || defined(__ANDROID__)
			(*this) << lpV->operator MDataType::DateTime();
#else
	#error UNKNOWN PLATFORM
#endif
			break;

		case MDataType::Variant::MSI_CELLFMTDATA:
#if defined(WIN32) || defined(sun)
		  	(*this) << (const MDataType::CellFmtData) *lpV;
#elif defined(_AIX) || defined(linux) || defined(__hpux) || defined(__ANDROID__)
			(*this) << lpV->operator MDataType::CellFmtData();
#else
	#error UNKNOWN PLATFORM
#endif
			break;

		case MDataType::Variant::MSI_BINARY:
			(*this) << * (lpV->ToBinary());
			break;

		case MDataType::Variant::MSI_INVALID:
			// there is no content, and we already wrote datatype, so write nothing here.
			break;

		case MDataType::Variant::MSI_STRING:
		case MDataType::Variant::MSI_MBCS_STRING:
#ifdef NO_WCHAR_SUPPORT
			(*this) << lpV->ToString().c_str();
#else
			(*this) << lpV->ToWString().c_str();
#endif			
			break;

		default:
			_ASSERT(false);
			throw MBase::ExceptionImplementation<WriteStreamException>("Unrecognized Variant Type.");
			break;
		}

		lpV++;
	}

}

WriteDataStream& WriteDataStreamImpl::operator<<(const SerializableObject& irObject)
{
	WriteType(T_OBJECT);

	irObject.Serialize(*this);

	return *this;
}


/*-------- unicode string ---------*/
#ifndef NO_WCHAR_SUPPORT
WriteDataStream& WriteDataStreamImpl::operator << (const wchar_t *pv)
{
	WriteType(T_UTF8);

	try
	{
		const MBase::ReturnString lStr(MBase::WideCharToUTF8Governed(pv, false, mMemConPtr));

		// strlen() AVs on NULL input
		Int64 lLen = lStr.GetLength() + 1; // including the terminal NULL
		const char* lPtr = lStr.c_str() ? lStr.c_str() : "";
		MDataType::BinaryPtr lBinary;
		if (mHeader & (PDS_SIMPLE_ENCRYPT_STREAM | PDS_RC4_ENCRYPT_STREAM |PDS_TEA_ENCRYPT_STREAM |PDS_AES_ENCRYPT_STREAM))
		{
			lBinary.Reset(mEncryptor->EncryptData(lPtr, lLen, mEncryptorVersion));
			lLen = lBinary->GetSize();
		}

		// write the length of the string
		(*this) << lLen;

		// write the complementary value of the length
		Int64 lNew = ~lLen;
		(*this) << lNew;

		if (mHeader & (PDS_SIMPLE_ENCRYPT_STREAM | PDS_RC4_ENCRYPT_STREAM |PDS_TEA_ENCRYPT_STREAM |PDS_AES_ENCRYPT_STREAM))
			mpBStream->Write((unsigned char*)lBinary->c_ptr(), lLen);
		else
			mpBStream->Write((unsigned char*)lPtr, lLen);
	}
	catch (MBase::StringConversionException& )
	{
		throw MBase::ExceptionImplementation<WriteStreamException>("WideCharToUTF8 conversion failure.");
	}
	catch (MBase::ContractManagerAllocException&) {
		throw MBase::ExceptionImplementation<WriteStreamException>("Memory contracting failed during WideCharToUTF8 conversion.");
	}

	return *this;
}
#endif

/* write char string.
*/
WriteDataStream& WriteDataStreamImpl::operator<<(const char * ipString)
{
	WriteType(T_CHAR);

	// strlen() AVs on NULL input
	Int64 lLen = ipString ? strlen(ipString) + 1 : 1; // including the terminal NULL

	const char* lPtr = ipString ? ipString : "";
	MDataType::BinaryPtr lBinary;
#ifndef __ANDROID__
	if (mHeader & (PDS_SIMPLE_ENCRYPT_STREAM | PDS_RC4_ENCRYPT_STREAM |PDS_TEA_ENCRYPT_STREAM |PDS_AES_ENCRYPT_STREAM))
	{
		lBinary.Reset(mEncryptor->EncryptData(lPtr, lLen, mEncryptorVersion));
		lLen = lBinary->GetSize();
	}
#endif
	// write the length of the string
	(*this) << lLen;

	// write the complementary value of the length
	Int64 lNew = ~lLen;
	(*this) << lNew;
#ifndef __ANDROID__
	if (mHeader & (PDS_SIMPLE_ENCRYPT_STREAM | PDS_RC4_ENCRYPT_STREAM |PDS_TEA_ENCRYPT_STREAM |PDS_AES_ENCRYPT_STREAM))
		mpBStream->Write((unsigned char*)lBinary->c_ptr(), lLen);
	else
#endif
		mpBStream->Write((unsigned char*)lPtr, lLen);

	return *this;
}


/* special function for enhanced robustness. The original value and its
 complementary value are both writen into the stream, so the reader can
 compare the consecutive data to ensure the data integrity
 */
void WriteDataStreamImpl::SafeWrite(Int32 pv)
{
	WriteType(T_SAFEINT);

	(*this) << pv;

	Int32 lCompData = ~ pv;
	(*this) << lCompData;
}

/*=====================================================
=====================================================
|| 			Utility Functions					||
=====================================================
=====================================================*/


/* reserve a space of 64 bit integer in the stream, so the caller can fill
	the space with special value, such as the length of a block. We reserve
	64-bit integer because we want to ensure the block can be larger than 4G.
*/
StreamInt64Reserve* WriteDataStreamImpl::ReserveInt64()
{
	_ASSERTE( GetStreamType() != ST_SHM);

	WriteType(T_INT64);

	ReservedPosition lResPos = mpBStream->ReserveBytes(8);
	return new StreamInt64ReserveImpl(mpBStream.Get(), lResPos, mRawFormat);
}

StreamInt32Reserve* WriteDataStreamImpl::ReserveInt32()
{
	_ASSERTE( GetStreamType() != ST_SHM);

	WriteType(T_INT);

	ReservedPosition lResPos = mpBStream->ReserveBytes(4);
	return new StreamInt32ReserveImpl(mpBStream.Get(),lResPos, mRawFormat);
}


/* return the binary data containing the stream, so other component can
	save it to file or send it over network.
*/
MDataType::Binary* WriteDataStreamImpl::GiveOutBinary()
{
	_ASSERTE(mpBStream->GetStreamType() == ST_MEMORY);

	mpBStream->Pack();

	MDataType::BinaryPtr lBinPtr( MDataType::CreateBinary( mBinCollectionPtr.Get(), mMemConPtr.Get() ));

	mBinCollectionPtr.Reset();	// release memory in binary collection

	Rewind();
	return lBinPtr.GiveUp();
}

/* return the binary data containing the stream, so other component can
	save it to file or send it over network.
*/
MDataType::BinaryCollection* WriteDataStreamImpl::GiveOutBinaryCollection()
{
	_ASSERTE(mpBStream->GetStreamType() == ST_MEMORY);

	mpBStream->Pack();

	MDataType::BinaryCollectionPtr lResult( mBinCollectionPtr.GiveUp() );

	Rewind();	// this will delete mpBStream

	return lResult.GiveUp();
}

/* return a copy of the binary data containing the stream, so other component can
	save it to file or send it over network.
*/
MDataType::BinaryCollection* WriteDataStreamImpl::CopyOutBinaryCollection()
{
	_ASSERTE(mpBStream->GetStreamType() == ST_MEMORY);

	mpBStream->Pack();

	MDataType::BinaryCollectionPtr lResultPtr( MDataType::CreateBinaryCollection() );

	// duplicate the contents in mBinCollectionPtr to lResultPtr
	unsigned Int32 lIdx = 0;
	unsigned Int32 lCount = mBinCollectionPtr->GetBinaryCount();
	for(; lIdx < lCount; lIdx ++ )
	{
		const MDataType::Binary *lBinary = mBinCollectionPtr->GetBinary(lIdx);
		lResultPtr->Insert(MDataType::CreateBinary(lBinary->c_ptr(), lBinary->GetSize()));
	}

	return lResultPtr.GiveUp();
}

void WriteDataStreamImpl::CommitStream()
{
	mpBStream->CommitStream();
}

Int64 WriteDataStreamImpl::Get64BitSize() throw()
{
	return mpBStream->GetSize();
}

unsigned Int32 WriteDataStreamImpl::GetSize()
{
	if ( MoreThan32Bit(mpBStream->GetSize()) )
	{
		throw MBase::ExceptionImplementation<WriteStreamException>("Parameter is too small to hold the value");
	}

	return (unsigned Int32) mpBStream->GetSize();
}

StreamType WriteDataStreamImpl::GetStreamType()
{
	return mpBStream->GetStreamType();
}

void WriteDataStreamImpl::Rewind()
{
	_ASSERTE( mpBStream.Get() );
	_ASSERTE( mpBStream->GetStreamType() != ST_SHM);

	if ( mpBStream->GetStreamType()== ST_FILE)
	{
		MFileSystem::WriteableFile::Ptr lFilePtr(static_cast<WriteByteStreamFileImpl*>(mpBStream.Get())->ReleaseFilePtr());
		mpBStream = CreateWriteByteStream( lFilePtr ) ;
	}
	else
	{
		mBinCollectionPtr.Reset(MDataType::CreateBinaryCollection() );
		mpBStream = CreateWriteByteStream(mBinCollectionPtr.Get(), mBlockSize, mMemConPtr.Get() ) ;
	}

	DataStreamHeader lHeader;
	lHeader.SetHeaderValue(mHeader);
#ifndef __ANDROID__
	if (mHeader & (PDS_RC4_ENCRYPT_STREAM |PDS_TEA_ENCRYPT_STREAM |PDS_AES_ENCRYPT_STREAM))
	{
		lHeader.SetKey(mEncryptKey.c_str());
	}
#endif
	lHeader.Save(mpBStream.Get());
}

void WriteDataStreamImpl::Abandon()
{
	mpBStream->Abandon();
}

void WriteDataStreamImpl::CopyAppend(const ReadDataStream& irReadStream)
{
	_ASSERTE(mpBStream->GetStreamType() == ST_MEMORY);

	MDataType::BinaryCollectionPtr lBinCollectionPtr(irReadStream.CopyOutContentBinaryCollection());
	static_cast<WriteByteStreamImpl*>(mpBStream.Get())->Merge(lBinCollectionPtr.Get());
};


void WriteDataStreamImpl::Delete() throw()
{
	delete this;
}

MFileSystem::WriteableFile::ReturnPtr WriteDataStreamImpl::ReleaseFilePtr()
{
	_ASSERTE(mpBStream->GetStreamType() == ST_FILE);

	WriteByteStreamFileImpl* lpTemp = static_cast<WriteByteStreamFileImpl*>(mpBStream.Get());
	return lpTemp->ReleaseFilePtr();
}


/*=====================================================
=====================================================
|| 			Constructor Functions					||
=====================================================
====================================================*/

// constructor for local memory based Stream
WriteDataStreamImpl::WriteDataStreamImpl(bool iNeedWriteHeader,
										 unsigned Int32 iBlockSize,
										 MBase::MemoryContract * ipMemCon,
										 unsigned short header,
										 const char* ipcKey)
: mBinCollectionPtr(MDataType::CreateBinaryCollection())
, mpBStream()
, mHeader(header)
, mLongVer(false)
, mBlockSize(iBlockSize)
, mMemConPtr(ipMemCon)
, mRawFormat(false)
#ifndef __ANDROID__
, mEncryptorVersion((MEncryptor::EncryptionVersion)-3)
#endif
{
	mpBStream = CreateWriteByteStream(mBinCollectionPtr.Get(), iBlockSize, ipMemCon) ;
	Init(iNeedWriteHeader, header, ipcKey);
}

// constructor for File-based stream
WriteDataStreamImpl::WriteDataStreamImpl(bool iNeedWriteHeader,
					 unsigned Int32 iBlockSize,
					 MFileSystem::WriteableFile::Ptr& iFilePtr,
					 unsigned short header,
					 const char* ipcKey)
: mpBStream( CreateWriteByteStream(iFilePtr) ),
 mHeader(header),
 mLongVer(false),
 mRawFormat(false),
 mBlockSize(iBlockSize)
 , mBinCollectionPtr(0)
{
	Init(iNeedWriteHeader, header, ipcKey);
}

// constructor for local memory-based stream
WriteDataStreamImpl::WriteDataStreamImpl(MDataType::BinaryCollection* ipBins,
										unsigned Int32 iBlockSize,
										MBase::MemoryContract * ipMemCon)
: mBinCollectionPtr(ipBins)
, mpBStream()
, mBlockSize(iBlockSize)
, mMemConPtr(ipMemCon)
, mRawFormat(false)
{
	mpBStream = CreateWriteByteStream( mBinCollectionPtr.Get(), iBlockSize, ipMemCon) ;

	_ASSERT( mpBStream.Get());

	DataStreamHeader lHeader;
	unsigned char* lpMemory = (unsigned char*)ipBins->GetBinary(0)->c_ptr();
	lHeader.Load(lpMemory);

	mHeader = lHeader.GetHeaderValue();
	mLongVer = lHeader.GetLongVersion();
	mRawFormat = lHeader.GetRawFormat();
	if ( mRawFormat )
	{
#ifdef LITTLE_ENDIAN
		if ( lHeader.IsBigEndian()  )
			throw MBase::ExceptionImplementation<WriteStreamException>("Little Endian machine cannot handle Big Endian raw data");
#else
		if ( ! lHeader.IsBigEndian()  )
			throw MBase::ExceptionImplementation<WriteStreamException>("Big Endian machine cannot handle Little Endian raw data");
#endif
	}
}
#ifndef __ANDROID__
// constructor for SHM-based stream
WriteDataStreamImpl::WriteDataStreamImpl(MMultiProcess::SharedMemory* ipSharedMemPtr, unsigned Int32 iBlockSize)
: mpBStream( CreateWriteByteStream(ipSharedMemPtr, iBlockSize))
, mBlockSize(iBlockSize)
, mBinCollectionPtr(0)
{
	// The current shared memory based write data stream does not support encryption and other settings.
	// If needed, a new constructor taking header configurations should be added and the majority of the
	// code in this file should function correctly. The settings below are the simplest default settings.
	// -  ywei Jan, 2008
	_ASSERT(mpBStream.Get() );
	mLongVer = false;
	mRawFormat = true;
	mHeader = 0;
	// mEncryptor and mEncryptorVersion are not initialized. The current code does not use them.
	mEncryptKey = "";
}
#endif

// constructor for SHM-based stream
WriteDataStreamImpl::WriteDataStreamImpl(MSerialization::WriteByteStream* ipSharedMemWriteByteStreamPtr)
: mpBStream(ipSharedMemWriteByteStreamPtr),
	mBlockSize(0)
, mBinCollectionPtr(0)
{
	// The current shared memory based write data stream does not support encryption and other settings.
	// If needed, a new constructor taking header configurations should be added and the majority of the
	// code in this file should function correctly. The settings below are the simplest default settings.
	// -  ywei Jan, 2008
	_ASSERT(mpBStream.Get() );
	mLongVer = false;
	mRawFormat = true;
	mHeader = 0;
	// mEncryptor and mEncryptorVersion are not initialized. The current code does not use them.
	mEncryptKey = "";
}

WriteDataStreamImpl::~WriteDataStreamImpl() throw()
{
}

void WriteDataStreamImpl::Merge(WriteDataStreamImpl& irStream)
{
	_ASSERTE(mpBStream->GetStreamType() == ST_MEMORY);
	_ASSERTE( irStream.mpBStream->GetStreamType() == ST_MEMORY);
	if ( irStream.mpBStream->GetStreamType() != ST_MEMORY )
	{
		throw MBase::ExceptionImplementation<WriteStreamException>("Coding error. Cannot merge with non-memory ByteStream.");
	}

	irStream.mpBStream->Pack();

	static_cast<WriteByteStreamImpl*>(mpBStream.Get())->Merge(irStream.mBinCollectionPtr.Get());
}


void WriteDataStreamImpl::WriteType(TYPE iType)
{
	if ( mLongVer )
	{
		unsigned char lType = (unsigned char) iType;
		mpBStream->Write(&lType,(unsigned Int32) sizeof(lType));
	}
}

void WriteDataStreamImpl::WriteType(TYPE iType, Int64 cnt)
{
	if ( mLongVer )
	{
		if ( MoreThan32Bit(cnt) )
		{
			unsigned char lType = (unsigned char) (iType | T_ARRAY );

			mpBStream->Write(&lType, (unsigned Int32)sizeof(lType));

			#ifdef LITTLE_ENDIAN
				mpBStream->Write((unsigned char*)&cnt,(unsigned Int32) sizeof(cnt));
			#else
				if ( mRawFormat)
					mpBStream->Write((unsigned char*)&cnt,(unsigned Int32) sizeof(cnt));
				else
					ConvertBigEndianToLittleEndian(sizeof(cnt), (const unsigned char*)&cnt, (unsigned Int32)1);
			#endif
		}
		else
		{
			unsigned char lType = (unsigned char) (iType | T_SMALLARRAY );

			mpBStream->Write(&lType, (unsigned Int32)sizeof(lType));

			unsigned Int32 lCnt = cnt;
			#ifdef LITTLE_ENDIAN
				mpBStream->Write((unsigned char*)&lCnt,(unsigned Int32) sizeof(lCnt));
			#else
				if ( mRawFormat)
					mpBStream->Write((unsigned char*)&lCnt,(unsigned Int32) sizeof(lCnt));
				else
					ConvertBigEndianToLittleEndian(sizeof(lCnt), (const unsigned char*)&lCnt, (unsigned Int32)1);
			#endif
		}
	}
}


void WriteDataStreamImpl::SetTimeout(Int64 iTimeoutInMilliSeconds)
{
	mpBStream->SetTimeout(iTimeoutInMilliSeconds);
}

/*-------------------------
	private functions
--------------------------*/

template<class A, class B>
void TempFastWrite(B cb, const unsigned char* pv, WriteByteStream* ipStream)
{
	A lCache[2000/sizeof(A)];	// 2000 bytes

	A * lpCache = lCache;
	MBase::StrongArrayPtr<A> lSPtrA;
	if ( cb > 2000/sizeof(A))
	{
		A * lpACache = new A[cb];
		lpCache = lpACache;
		lSPtrA.Acquire(lpACache);
	}

	const A* lpTempMem = (const A*) pv;
	A* lpTempCache = lpCache;

	switch (sizeof(A) )
	{
	case 2:
		{
			for (B i = 0; i < cb; i ++)
			{	// for each element of the array
				register unsigned short lpShort = *lpTempMem++;
				*lpTempCache++ = const_swap2(lpShort);
			}
		}
		break;
	case 4:
		{
			for (B i = 0; i < cb; i ++)
			{	// for each element of the array
				register unsigned Int32 lpShort = *lpTempMem++;
				*lpTempCache++ = const_swap4(lpShort);
			}
		}
		break;
	case 8:
		{
			for (B i = 0; i < cb; i ++)
			{	// for each element of the array
				register unsigned Int64 lpShort = *lpTempMem++;
				*lpTempCache++ = const_swap8(lpShort);
			}
		}
		break;
	default:
		_ASSERTE(false);	// this should be used by only three types
		throw  MBase::ExceptionImplementation<ReadStreamException>("Wrong conversion size.");
		break;
	}

	ipStream->Write((unsigned char*) lpCache, (Int64) (cb * sizeof(A)) );
}


void WriteDataStreamImpl::ConvertBigEndianToLittleEndian(Int32 byteCnt,
														 const unsigned char *pv,
														 unsigned Int32 cb)
{
	switch (byteCnt)
	{
	case 2:
		TempFastWrite<unsigned short, unsigned Int32>(cb, pv, mpBStream.Get());
		break;
	case 4:
		TempFastWrite<unsigned Int32, unsigned Int32>(cb, pv, mpBStream.Get());
		break;
	case 8:
		TempFastWrite<unsigned Int64, unsigned Int32>(cb, pv, mpBStream.Get());
		break;
	default:
		{
			const unsigned char * lpSrc = pv;
			for(unsigned Int32 i = 0; i < cb; i ++)
			{	// for each element of the array

				for(Int32 j = byteCnt - 1; j >= 0; j --)
				{	// for each byte of the single value
					mpBStream->Write(lpSrc + j, (unsigned Int32)1);
				}

				lpSrc += byteCnt;
			}
		}
		break;
	}
}

void WriteDataStreamImpl::ConvertBigEndianToLittleEndian(Int32 byteCnt,
														 const unsigned char *pv,
														 Int64 cb)
{
	switch (byteCnt)
	{
	case 2:
		TempFastWrite<unsigned short, Int64>(cb, pv, mpBStream.Get());
		break;
	case 4:
		TempFastWrite<unsigned Int32, Int64>(cb, pv, mpBStream.Get());
		break;
	case 8:
		TempFastWrite<unsigned Int64, Int64>(cb, pv, mpBStream.Get());
		break;
	default:
		{
			const unsigned char * lpSrc = pv;
			for(Int64 i = 0; i < cb; i ++)
			{	// for each element of the array

				for(Int32 j = byteCnt - 1; j >= 0; j --)
				{	// for each byte of the single value
					mpBStream->Write(lpSrc + j, (unsigned Int32) 1);
				}

				lpSrc += byteCnt;
			}
		}
		break;
	}
}

void WriteDataStreamImpl::Init(bool iNeedWriteHeader, unsigned short header, const char* ipcKey)
{
	_ASSERT(mpBStream.Get());

	DataStreamHeader lHeader;
	lHeader.SetHeaderValue(header);
#ifndef __ANDROID__
	if (header & (PDS_RC4_ENCRYPT_STREAM |PDS_TEA_ENCRYPT_STREAM |PDS_AES_ENCRYPT_STREAM))
	{
		mEncryptKey = ipcKey ? ipcKey : "";
		lHeader.SetKey(mEncryptKey.c_str());
	}
#endif
	if ( iNeedWriteHeader)
	{
		lHeader.Save(mpBStream.Get());
	}

	mLongVer = lHeader.GetLongVersion();
	mRawFormat = lHeader.GetRawFormat();
#ifndef __ANDROID__
	if (header & PDS_SIMPLE_ENCRYPT_STREAM)
	{
		mEncryptor.Reset(MEncryptor::CreateSIMPLEEncryptorDecryptor());
		mEncryptorVersion = MEncryptor::SIMPLE;
	}
	else if (header & PDS_RC4_ENCRYPT_STREAM)
	{
		MBase::ReturnWString lKey(MBase::UTF8ToWideChar(ipcKey).c_str());
		mEncryptor.Reset(MEncryptor::CreateMD5EncryptorDecryptor(lKey.c_str()));
		mEncryptorVersion = MEncryptor::RC4;
	}
	else if (header & PDS_TEA_ENCRYPT_STREAM)
	{
		MBase::ReturnWString lKey(MBase::UTF8ToWideChar(ipcKey).c_str());
		mEncryptor.Reset(MEncryptor::CreateRIPEMD160EncryptorDecryptor(lKey.c_str()));
		mEncryptorVersion = MEncryptor::TEA;
	}
	else if (header & PDS_AES_ENCRYPT_STREAM)
	{
		MBase::ReturnWString lKey(MBase::UTF8ToWideChar(ipcKey).c_str());
		mEncryptor.Reset(MEncryptor::CreateRIPEMD160EncryptorDecryptor(lKey.c_str()));
		mEncryptorVersion = MEncryptor::AES;
	}
#endif
}

#ifndef NO_WCHAR_SUPPORT
void WriteDataStreamImpl::WriteString(const wchar_t *pv)
{
	WriteType(T_UTF8);

	try
	{
		const MBase::ReturnString lStr(MBase::WideCharToUTF8Governed(pv, false, mMemConPtr));

		// strlen() AVs on NULL input
		unsigned Int32 lLen = static_cast<unsigned Int32>(lStr.GetLength()) + 1; // including the terminal NULL
		const char* lPtr = lStr.c_str() ? lStr.c_str() : "";
		MDataType::BinaryPtr lBinary;
		if (mHeader & (PDS_SIMPLE_ENCRYPT_STREAM | PDS_RC4_ENCRYPT_STREAM |PDS_TEA_ENCRYPT_STREAM |PDS_AES_ENCRYPT_STREAM))
		{
			lBinary.Reset(mEncryptor->EncryptData(lPtr, lLen, mEncryptorVersion));
			lLen = static_cast<unsigned Int32>(lBinary->GetSize());
		}

		// write the length of the string
		(*this) << lLen;

		if (mHeader & (PDS_SIMPLE_ENCRYPT_STREAM | PDS_RC4_ENCRYPT_STREAM |PDS_TEA_ENCRYPT_STREAM |PDS_AES_ENCRYPT_STREAM))
			mpBStream->Write((unsigned char*)lBinary->c_ptr(), lLen);
		else
			mpBStream->Write((unsigned char*)lPtr, lLen);
	}
	catch (MBase::StringConversionException& )
	{
		throw MBase::ExceptionImplementation<WriteStreamException>("WideCharToUTF8 conversion failure.");
	}
	catch (MBase::ContractManagerAllocException&) {
		throw MBase::ExceptionImplementation<WriteStreamException>("Memory contracting failed during WideCharToUTF8 conversion.");
	}
}
#endif

}
