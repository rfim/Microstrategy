//==============================================================================================
//	FILENAME	:	ReadDataStreamImpl.cpp
//	AUTHOR		:	Yi Luo
//	CREATION	:	07/12/2002
//	Copyright (C) MicroStrategy, Inc. 2002
//==============================================================================================

#include "StdAfx.h"

#include "ReadDataStreamImpl.h"
#include "PDSerialization/PDSerialization/WriteDataStream.h"
#include "Facilities.h"
#include "Base/Defines/ExceptionImplementation.h"
#include "DataStreamHeader.h"
#include "DataType/DataType/DynamicTime.h"

#define RET_ON_FALSE(ret)   if ( ! ret ) return;

namespace MSerialization
{


ReadDataStream* ConvertBinary2ReadDataStream(MDataType::Binary* iB, MBase::MemoryContract * ipMemCon )
{
	return new ReadDataStreamImpl(iB, ipMemCon);
}

ReadDataStream* ConvertWriteDataStream2ReadDataStream(WriteDataStream * ipWriteStream,
													   MBase::MemoryContract * ipMemCon )
{
	return new ReadDataStreamImpl(ipWriteStream->GiveOutBinaryCollection(), ipMemCon);
}

ReadDataStream* GetReadDataStreamFromFile(MFileSystem::ReadableFile::Ptr& iFilePtr)
{
	return new ReadDataStreamImpl(iFilePtr);
}
#if !defined(WIN32) && !defined(__ANDROID__)
ReadDataStream* CreateReadDataStreamFromSharedMem(MMultiProcess::SharedMemory* ipSharedMemPtr,
												  MBase::MemoryContract * ipMemCon)
{
	return new ReadDataStreamImpl(ipSharedMemPtr, ipMemCon);
}
#endif

ReadDataStream* CreateReadDataStreamFromSharedMemByteStream(MSerialization::ReadByteStream * ipSharedMemReadBStreamPtr,
													MBase::MemoryContract * ipMemCon)
{
	return new ReadDataStreamImpl(ipSharedMemReadBStreamPtr, ipMemCon);
}

/*-----------------
	public:
-----------------*/

/*------- 1 byte ----------*/

// binary
ReadDataStream& ReadDataStreamImpl::operator >> (MDataType::Binary& v)
{
	VerifyType(T_BINARY);

	// at first read the length and its complementary
	Int64 lSize = 0;
	(*this) >> lSize;

	Int64 lComp = 0;
	(*this) >> lComp;

	if ( lComp != ~lSize)
	{
		throw MBase::ExceptionImplementation<ReadStreamException>("Corrupted Stream: Wrong binary size.");
	}

	MCMRESULT lReq = MCM_OK;
	if ( ! mMemConPtr.IsNull() && (lReq = mMemConPtr->Alloc((unsigned int)lSize)) != MCM_OK )
	{
		throw MBase::ExceptionImplementation<ReadStreamException>("Not enough memory for this binary.", lReq);
	}

	MBase::StrongArrayPtr<unsigned char> lMemPtr(new unsigned char [lSize]);
	if ( ! mMemConPtr.IsNull() )
	{
		mMemConPtr->Free((unsigned int)lSize);
	}

	if ( lMemPtr.Get() == 0 )
	{
		throw MBase::ExceptionImplementation<ReadStreamException>("Memory allocation failure: NULL pointer");
	}

	// read the binary data
	mpBStream->Read( lMemPtr.Get(), lSize );

	if ( mLongVer )
	{
		Int64 lSizeCopy = 0;
		(*this) >> lSizeCopy;
		if ( lSizeCopy != lSize )
			throw MBase::ExceptionImplementation<ReadStreamException>("Corrupted Stream: binary corrupted.");
	}
	v.SetBinary( lMemPtr.Get(), lSize);

	return (*this);
}

// raw binary
void ReadDataStreamImpl::Read(unsigned char * pv, unsigned Int32 cb)
{
	VerifyType(T_RAWBINARY, cb);
#ifndef __ANDROID__
	if (mStreamHeader & (PDS_SIMPLE_ENCRYPT_STREAM | PDS_RC4_ENCRYPT_STREAM |PDS_TEA_ENCRYPT_STREAM |PDS_AES_ENCRYPT_STREAM))
	{
		unsigned Int32 lLen = 0;
		(*this) >> lLen;

		MBase::StrongArrayPtr<unsigned char> lBuffer;
		unsigned char* lPtr = pv;
		if (lLen>cb)
		{
			lBuffer.Reset(new unsigned char[lLen]);
			lPtr = (unsigned char*)lBuffer.Get();
		}

		mpBStream->Read(lPtr, (unsigned Int32)(lLen * sizeof(unsigned char)));

		MDataType::BinaryPtr lBinary(mEncryptor->DecryptData(lPtr, lLen));
		_ASSERTE(lBinary->GetSize()==cb);
		memmove(pv, lBinary->c_ptr(), cb);
	}
	else
#endif
	{
		mpBStream->Read(pv, (unsigned Int32)(cb * sizeof(unsigned char)));
	}
}

// bool and bool array
ReadDataStream& ReadDataStreamImpl::operator >> (bool& v)
{
	VerifyType(T_BOOL);

	bool lv = false;

	mpBStream->Read((unsigned char*)&lv,(unsigned Int32) sizeof(lv));

	v = lv;
	return (*this);
}

void ReadDataStreamImpl::Read(bool * pv, unsigned Int32 cb)
{
	VerifyType(T_BOOL, cb);

	mpBStream->Read((unsigned char *)pv,(unsigned Int32)( cb * sizeof(bool)));
}

/*------- 2 byte ----------*/

// short and short array
ReadDataStream& ReadDataStreamImpl::operator >> (short& v)
{
	VerifyType(T_SHORT);

	short lv = 0;

	#ifdef LITTLE_ENDIAN
		mpBStream->Read((unsigned char *)&lv, (unsigned Int32)sizeof(lv));
	#else
		if (mRawFormat)
			mpBStream->Read((unsigned char *)&lv, (unsigned Int32)sizeof(lv));
		else
			ConvertLEStreamToBEValue(2, (unsigned char *)&lv, (unsigned Int32) 1);
	#endif

	v = lv;

	return (*this);
}

ReadDataStream& ReadDataStreamImpl::operator >> (unsigned short& v)
{
	VerifyType(T_USHORT);

	unsigned short lv = 0;

	#ifdef LITTLE_ENDIAN
		mpBStream->Read((unsigned char *)&lv, (unsigned Int32)sizeof(lv));
	#else
		if (mRawFormat)
			mpBStream->Read((unsigned char *)&lv, (unsigned Int32)sizeof(lv));
		else
			ConvertLEStreamToBEValue(2, (unsigned char *)&lv, (unsigned Int32) 1);
	#endif

	v = lv;
	return (*this);
}

void ReadDataStreamImpl::Read(short * pv, unsigned Int32 cb)
{
	VerifyType(T_SHORT, cb);

	#ifdef LITTLE_ENDIAN
		mpBStream->Read((unsigned char *)pv, (unsigned Int32)(sizeof(short) * cb));
	#else
		if (mRawFormat)
			mpBStream->Read((unsigned char *)pv, (unsigned Int32)(sizeof(short) * cb));
		else
		{
			if ( MoreThan32Bit(cb) )
			{
				ConvertLEStreamToBEValue(2, (unsigned char *)pv, cb);
			}
			else
			{
				ConvertLEStreamToBEValue(2, (unsigned char *)pv, (unsigned Int32) cb );
			}
		}
	#endif
}

/*------- 4 byte ----------*/

// long and long array
ReadDataStream& ReadDataStreamImpl::operator >> (Int32& v)
{
	VerifyType(T_INT);

	Int32 lv = 0;

	#ifdef LITTLE_ENDIAN
		mpBStream->Read((unsigned char *)&lv, (unsigned Int32)sizeof(Int32)) ;
	#else
		if (mRawFormat)
			mpBStream->Read((unsigned char *)&lv, (unsigned Int32)sizeof(Int32)) ;
		else
			ConvertLEStreamToBEValue(4, (unsigned char *)&lv, (unsigned Int32) 1);
	#endif

	v = lv;
	return (*this);
}

ReadDataStream& ReadDataStreamImpl::operator >> (unsigned Int32& v)
{
	VerifyType(T_UINT);

	unsigned Int32 lv = 0;

	#ifdef LITTLE_ENDIAN
		mpBStream->Read((unsigned char *)&lv, (unsigned Int32)sizeof(Int32)) ;
	#else
		if (mRawFormat)
			mpBStream->Read((unsigned char *)&lv, (unsigned Int32)sizeof(Int32)) ;
		else
			ConvertLEStreamToBEValue(4, (unsigned char *)&lv, (unsigned Int32) 1);
	#endif

	v = lv;
	return (*this);
}


void ReadDataStreamImpl::Read(Int32* pv, unsigned Int32 cb)
{
	VerifyType(T_INT, cb);

	#ifdef LITTLE_ENDIAN
		mpBStream->Read((unsigned char *)pv, (unsigned Int32)(sizeof(Int32) * cb));
	#else
		if (mRawFormat)
			mpBStream->Read((unsigned char *)pv, (unsigned Int32)(sizeof(Int32) * cb));
		else
		{
			if ( MoreThan32Bit(cb) )
			{
				ConvertLEStreamToBEValue(4, (unsigned char *)pv, cb);
			}
			else
			{
				ConvertLEStreamToBEValue(4, (unsigned char *)pv, (unsigned Int32) cb );
			}
		}
	#endif
}

/*------- 8 byte ----------*/

// 64-bit integer and array
ReadDataStream& ReadDataStreamImpl::operator >> (Int64& v)
{
	VerifyType(T_INT64);

	Int64 lv = 0;

	#ifdef LITTLE_ENDIAN
		mpBStream->Read((unsigned char *)&lv, (unsigned Int32)sizeof(lv));
	#else
		if (mRawFormat)
			mpBStream->Read((unsigned char *)&lv, (unsigned Int32)sizeof(lv));
		else
			ConvertLEStreamToBEValue(8, (unsigned char *)&lv, (unsigned Int32) 1);
	#endif

	v = lv;
	return (*this);
}


void ReadDataStreamImpl::Read(Int64* pv, unsigned Int32 cb)
{
	VerifyType(T_INT64, cb);

	#ifdef LITTLE_ENDIAN
		mpBStream->Read((unsigned char *)pv, (unsigned Int32)(sizeof(Int64) * cb));
	#else
		if (mRawFormat)
			mpBStream->Read((unsigned char *)pv, (unsigned Int32)(sizeof(Int64) * cb));
		else
		{
			if ( MoreThan32Bit(cb) )
			{
				ConvertLEStreamToBEValue(8, (unsigned char *)pv, cb);
			}
			else
			{
				ConvertLEStreamToBEValue(8, (unsigned char *)pv, (unsigned Int32) cb );
			}
		}
	#endif
}

// float and float array
ReadDataStream& ReadDataStreamImpl::operator >> (float & v)
{
	VerifyType(T_FLOAT);

	float lv = 0;

	#ifdef LITTLE_ENDIAN
		mpBStream->Read((unsigned char *)&lv, (unsigned Int32)sizeof(lv));
	#else
		if (mRawFormat)
			mpBStream->Read((unsigned char *)&lv, (unsigned Int32)sizeof(lv));
		else
			ConvertLEStreamToBEValue(4, (unsigned char *)&lv, (unsigned Int32) 1);
	#endif

	v = lv;
	return (*this);
}


void ReadDataStreamImpl::Read(float* pv, unsigned Int32 cb)
{
	VerifyType(T_FLOAT, cb);

	#ifdef LITTLE_ENDIAN
		mpBStream->Read((unsigned char *)pv, (unsigned Int32)(sizeof(float) * cb));
	#else
		if (mRawFormat)
			mpBStream->Read((unsigned char *)pv, (unsigned Int32)(sizeof(float) * cb));
		else
		{
			if ( MoreThan32Bit(cb) )
			{
				ConvertLEStreamToBEValue(4, (unsigned char *)pv, cb);
			}
			else
			{
				ConvertLEStreamToBEValue(4, (unsigned char *)pv, (unsigned Int32) cb );
			}
		}
	#endif
}

// double and double array
ReadDataStream& ReadDataStreamImpl::operator >> (double & v)
{
	VerifyType(T_DOUBLE);

	double lv = 0;

	#ifdef LITTLE_ENDIAN
		mpBStream->Read((unsigned char *)&lv, (unsigned Int32)sizeof(lv));
	#else
		if (mRawFormat)
			mpBStream->Read((unsigned char *)&lv, (unsigned Int32)sizeof(lv));
		else
			ConvertLEStreamToBEValue(8, (unsigned char *)&lv, (unsigned Int32) 1);
	#endif

	v = lv;
	return (*this);
}


void ReadDataStreamImpl::Read(double * pv, unsigned Int32 cb)
{
	VerifyType(T_DOUBLE, cb);

	#ifdef LITTLE_ENDIAN
		mpBStream->Read((unsigned char *)pv, (unsigned Int32)(sizeof(double) * cb));
	#else
		if (mRawFormat)
			mpBStream->Read((unsigned char *)pv, (unsigned Int32)(sizeof(double) * cb));
		else
		{
			if ( MoreThan32Bit(cb) )
			{
				ConvertLEStreamToBEValue(8, (unsigned char *)pv, cb);
			}
			else
			{
				ConvertLEStreamToBEValue(8, (unsigned char *)pv, (unsigned Int32) cb );
			}
		}
	#endif
}

/*------- Unicdoe string ----------*/
#ifndef NO_WCHAR_SUPPORT
// wide char and wide string
MBase::ReturnWString ReadDataStreamImpl::ReadWString()
{
	VerifyType(T_UTF8);

	// read the length of the string
	Int64 lLen = 0;
	(*this) >> lLen;

	Int64 lNewLen = 0;
	(*this) >> lNewLen;

	if ( lNewLen != ~lLen )
	{
		throw MBase::ExceptionImplementation<ReadStreamException>("String length is corrupted.");
	}

	// notice that this length includes the terminal NULL
	MCMRESULT lReq = MCM_OK;
	if ( ! mMemConPtr.IsNull() && (lReq = mMemConPtr->Alloc((unsigned int)lLen)) != MCM_OK )
	{
		throw MBase::ExceptionImplementation<ReadStreamException>("Not enough memory for this string.", lReq);
	}

	MBase::StrongArrayPtr<char> lMBStr(new char[lLen]);
	if ( ! mMemConPtr.IsNull() )
	{
		mMemConPtr->Free((unsigned int)lLen);
	}

	if ( lMBStr.Get() == 0 )
	{
		throw MBase::ExceptionImplementation<ReadStreamException>("Memory allocation failure.");
	}

	// read the UTF-8 string
	mpBStream->Read((unsigned char*)lMBStr.Get(), lLen);

	if (mStreamHeader & (PDS_SIMPLE_ENCRYPT_STREAM | PDS_RC4_ENCRYPT_STREAM |PDS_TEA_ENCRYPT_STREAM |PDS_AES_ENCRYPT_STREAM))
	{
		MDataType::BinaryPtr lBinary(mEncryptor->DecryptData(lMBStr.Get(), lLen));
		lMBStr.Reset(new char[lBinary->GetSize()]);
		memmove(lMBStr.Get(), lBinary->c_ptr(), lBinary->GetSize());
	}

	try
	{
		return MBase::UTF8ToWideChar(lMBStr.Get() );
	}
	catch (MBase::StringConversionException&)
	{
		throw MBase::ExceptionImplementation<ReadStreamException>("UTF8ToWideChar conversion failure.");
	}
}
#endif
MBase::ReturnString ReadDataStreamImpl::ReadString()
{
	VerifyType(T_CHAR);

	// read the length of the string
	Int64 lLen = 0;
	(*this) >> lLen;

	Int64 lNewLen = 0;
	(*this) >> lNewLen;

	if ( lNewLen != ~lLen )
	{
		throw MBase::ExceptionImplementation<ReadStreamException>("String length is corrupted.");
	}

	// notice that this length includes the terminal NULL
	MCMRESULT lReq = MCM_OK;
	if ( ! mMemConPtr.IsNull() && (lReq = mMemConPtr->Alloc((unsigned int)lLen)) != MCM_OK )
	{
		throw MBase::ExceptionImplementation<ReadStreamException>("Not enough memory for this string.", lReq);
	}

	MBase::StrongArrayPtr<char> lMBStr(new char[lLen]);
	if ( ! mMemConPtr.IsNull() )
	{
		mMemConPtr->Free((unsigned int)lLen);
	}

	if ( lMBStr.Get() == 0 )
	{
		throw MBase::ExceptionImplementation<ReadStreamException>("Memory allocation failure.");
	}

	// read the UTF-8 string
	mpBStream->Read((unsigned char*)lMBStr.Get(), lLen);
#ifndef __ANDROID__
	if (mStreamHeader & (PDS_SIMPLE_ENCRYPT_STREAM | PDS_RC4_ENCRYPT_STREAM |PDS_TEA_ENCRYPT_STREAM |PDS_AES_ENCRYPT_STREAM))
	{
		MDataType::BinaryPtr lBinary(mEncryptor->DecryptData(lMBStr.Get(), lLen));
		lMBStr.Reset(new char[lBinary->GetSize()]);
		memmove(lMBStr.Get(), lBinary->c_ptr(), lBinary->GetSize());
	}
#endif
	return lMBStr.Get();
}


char ReadDataStreamImpl::ReadByte()
{
	char lChar = 0;

	mpBStream->Read((unsigned char *)&lChar, (unsigned Int32) sizeof(char));

	return lChar;
}
#ifndef NO_WCHAR_SUPPORT
wchar_t ReadDataStreamImpl::ReadWChar()
{
	VerifyType(T_UTF8_CHAR);

	// read the length of the string
	Int64 lLen = 0;
	(*this) >> lLen;

	Int64 lNewLen = 0;
	(*this) >> lNewLen;

	if ( lNewLen != ~lLen )
	{
		throw MBase::ExceptionImplementation<ReadStreamException>("String length is corrupted.");
	}

	// notice that this length includes the terminal NULL
	MCMRESULT lReq = MCM_OK;
	if ( ! mMemConPtr.IsNull() && (lReq = mMemConPtr->Alloc((unsigned int)lLen)) != MCM_OK )
	{
		throw MBase::ExceptionImplementation<ReadStreamException>("Not enough memory for this string.", lReq);
	}

	MBase::StrongArrayPtr<char> lMBStr(new char[lLen]);
	if ( ! mMemConPtr.IsNull() )
	{
		mMemConPtr->Free((unsigned int)lLen);
	}

	if ( lMBStr.Get() == 0 )
	{
		throw MBase::ExceptionImplementation<ReadStreamException>("Memory allocation failure.");
	}

	// read the UTF-8 string
	mpBStream->Read((unsigned char*)lMBStr.Get(), lLen);

	try
	{
		MBase::ReturnWString lRes(MBase::UTF8ToWideChar(lMBStr.Get() ) );
		_ASSERT( wcslen(lRes.c_str()) == 1 );
		wchar_t lChar = * lRes.c_str();
		return lChar;
	}
	catch (MBase::StringConversionException& )
	{
		throw MBase::ExceptionImplementation<ReadStreamException>("UTF8ToWideChar conversion failure.");
	}

}
#endif

/*------- Complext type ----------*/

// serialization of derived data type
// GUID and GUID array
ReadDataStream& ReadDataStreamImpl::operator >> (GUID& v)
{
	VerifyType(T_GUID);

	// long
	unsigned Int32 lD = 0;
	(*this) >> lD;
	v.Data1 = lD;

	// short
	(*this) >> v.Data2;

	(*this) >> v.Data3;

	// char[8]
	mpBStream->Read((unsigned char*) v.Data4, (unsigned Int32) 8);

	return (*this);
}

void ReadDataStreamImpl::Read(GUID * pv, unsigned Int32 cb)
{
	VerifyType(T_GUID, cb);

	GUID * lpv = pv;
	for(Int64 i = 0; i < cb; i ++)
	{	// for each GUID in the array

		// long
		unsigned Int32 lD = 0;
		(*this) >> lD;
		lpv->Data1 = lD;

		// short
		(*this) >> lpv->Data2;

		(*this) >> lpv->Data3;

		// char[8]
		mpBStream->Read((unsigned char*) lpv->Data4, (unsigned Int32) 8);

		lpv ++;
	}
}

// DateTime and DateTime array
ReadDataStream& ReadDataStreamImpl::operator >> (MDataType::DateTime& v)
{
	VerifyType(T_DATETIME);

	// fcamargo 12/18/02
	// Removed the code that serialized the DateTime as binary because it didn't work

	// NOTE: We don't need to serialize the week and dayofweek because they'll be calculated
	// from the month and day

	Int32 lnYear=0;
	(*this) >> lnYear;

	Int32 lnMonth = 0;
	(*this) >> lnMonth ;

	Int32 lnDay = 0;
	(*this) >> lnDay;

	Int32 lnHour = 0;
	(*this) >> lnHour;

	Int32 lnMinute = 0;
	(*this) >> lnMinute;

	Int32 lnSecond = 0;
	(*this) >> lnSecond;

	Int32 lnMillisecond = 0;
	(*this) >> lnMillisecond;

	bool lRes = true;
	lRes = v.PutYear(lnYear) && lRes;
	lRes = v.PutMonth(lnMonth) && lRes;
	lRes = v.PutDay(lnDay)&& lRes;
	lRes = v.PutHour(lnHour)&& lRes;
	lRes = v.PutMinute(lnMinute)&& lRes;
	lRes = v.PutSecond(lnSecond)&& lRes;
	lRes = v.PutMillisecond(lnMillisecond)&& lRes;

	_ASSERTE(lRes);

	return (*this);
}

void ReadDataStreamImpl::Read(MDataType::DateTime * pv, unsigned Int32 cb)
{
	VerifyType(T_DATETIME, cb);

	// fcamargo 12/18/02
	// Removed the code that serialized the DateTime as binary because it didn't work

	MDataType::DateTime * lpv = pv;
	for(Int64 i = 0; i < cb; i ++)
	{	// for each MDataType::DateTime in the array
		(*this) >> *lpv;

		lpv ++;
	}
}


// CellFmtData and CellFmtData array
ReadDataStream& ReadDataStreamImpl::operator >> (MDataType::CellFmtData& v)
{
	VerifyType(T_CELLFMTDATA);

	double lValue = 0;
	(*this) >> lValue;

	Int32 lType;
	(*this) >> lType;

	GUID lFmtID;
	(*this) >> lFmtID;

	v.PutValue(lValue);
	v.PutType((EnumCellFmtDataTypes)lType);
	v.PutFmtID(lFmtID);

	return (*this);
}

void ReadDataStreamImpl::Read(MDataType::CellFmtData * pv, unsigned Int32 cb)
{
	VerifyType(T_CELLFMTDATA, cb);

	MDataType::CellFmtData * lpv = pv;
	for(Int64 i = 0; i < cb; i ++)
	{
		(*this) >> *lpv;
		lpv ++;
	}
}


ReadDataStream& ReadDataStreamImpl::operator >> (MDataType::DynamicTime& orValue)
{
	VerifyType(T_DYNAMIC_TIME);

	// fcamargo 12/18/02
	// Removed the code that serialized the DynamicTime as binary because it didn't work

	Int32 lnYear=0;
	(*this) >> lnYear;

	Int32 lnMonth = 0;
	(*this) >> lnMonth;

	Int32 lnDay = 0;
	(*this) >> lnDay;

	Int32 lnWeek = 0;
	(*this) >> lnWeek;

	Int32 lnDayOfWeek = 0;
	(*this) >> lnDayOfWeek;

	Int32 lnHour = 0;
	(*this) >> lnHour;

	Int32 lnMinute = 0;
	(*this) >> lnMinute;

	Int32 lnSecond = 0;
	(*this) >> lnSecond;

	Int32	lnDayOffset = 0;
	(*this) >> lnDayOffset;

	Int32 lnDyn = 0;
	(*this) >> lnDyn;

	short lDynMonOffset = 0;
	MDataType::DYNAMIC_TIME lnDynamicTime = static_cast<MDataType::DYNAMIC_TIME>(lnDyn);
	if (lnDyn!=MDataType::NOT_DYNAMIC_TIME)
	{
		lDynMonOffset = lnDyn >> 16;
		lnDynamicTime = static_cast<MDataType::DYNAMIC_TIME>(lnDyn & 0xFFFF);
	}

	orValue.PutYear(lnYear) ;
	orValue.PutMonth(lnMonth);
	orValue.PutDay(lnDay);
	orValue.PutWeek(lnWeek);
	orValue.PutDayOfWeek(lnDayOfWeek);
	orValue.PutHour(lnHour);
	orValue.PutMinute(lnMinute);
	orValue.PutSecond(lnSecond);
	orValue.PutDynamicDayOffset(lnDayOffset);
	orValue.PutDynamicTime(lnDynamicTime);
	orValue.PutDynamicMonthOffset(lDynMonOffset);

	return (*this);
}

void ReadDataStreamImpl::Read(MDataType::DynamicTime * iopValueArray, unsigned Int32 iElementCount)
{
	VerifyType(T_DYNAMIC_TIME, iElementCount);

	// fcamargo 12/18/02
	// Removed the code that serialized the DynamicTime as binary because it didn't work

	MDataType::DynamicTime * lpv = iopValueArray;
	for(Int64 i = 0; i < iElementCount; i ++)
	{	// for each MDataType::DynamicTime in the array
		(*this) >> *lpv;
		lpv ++;
	}
}

ReadDataStream& ReadDataStreamImpl::operator >> (SerializableObject& irValue)
{
	VerifyType(T_OBJECT);
	irValue.DeSerialize(*this);
	return *this;
}


// Variant and Variant array
ReadDataStream& ReadDataStreamImpl::operator >> (MDataType::Variant& v)
{
	VerifyType(T_VARIANT);

	// get Variant type
	Int32 lTemp = 0;
	(*this) >> lTemp; ;

	MDataType::Variant::DATA_TYPE lDT = ( MDataType::Variant::DATA_TYPE) lTemp;

	switch ( lDT )
	{
	case MDataType::Variant::MSI_SHORT:
		{
			short lVal = 0;
			(*this) >> lVal;
			MDataType::Variant lvar(lVal);
			lvar.swap(v);
		}
		break;
	case MDataType::Variant::MSI_INTEGER:
	case MDataType::Variant::MSI_LONG:
		{
			Int32 lVal = 0;
			(*this) >> lVal;
			MDataType::Variant lvar(lVal);
			lvar.swap(v);
		}
		break;
	case MDataType::Variant::MSI_FLOAT:
		{
			double lVal = 0;
			(*this) >> lVal;
			MDataType::Variant lvar( (float)lVal);
			lvar.swap(v);
		}
		break;
	case MDataType::Variant::MSI_DOUBLE:
		{
			double lVal = 0;
			(*this) >> lVal;
			MDataType::Variant lvar( lVal);
			lvar.swap(v);
		}
		break;

	case MDataType::Variant::MSI_DATE:
	case MDataType::Variant::MSI_TIME:
	case MDataType::Variant::MSI_DATETIME:
		{
			MDataType::DateTime lS;
			(*this) >> lS;
			MDataType::Variant lvar( lS);
			lvar.swap(v);
		}
		break;

	case MDataType::Variant::MSI_CELLFMTDATA:
		{
			MDataType::CellFmtData lCellFmtData;
			(*this) >> lCellFmtData;
			MDataType::Variant lvar(lCellFmtData);
			lvar.swap(v);
		}
		break;

	case MDataType::Variant::MSI_BINARY:
		{
			// read length. We use 64-bit integer for expandability.
			Int64 lLen = 0;
			(*this) >> lLen;

			// read complementary length
			Int64 lComp = 0;
			(*this) >> lComp;

			if ( lComp != ~ lLen )
			{
				throw MBase::ExceptionImplementation<ReadStreamException>(
								"Binary length is corrupted.");
			}

			MBase::StrongArrayPtr<unsigned char> lMemPtr(new unsigned char [lLen]);

			mpBStream->Read(lMemPtr.Get(), lLen);

			MDataType::BinaryPtr lBP(MDataType::CreateBinary(lMemPtr.Get(), lLen));

			MDataType::Variant lVar( lBP.Get() );
			lVar.swap(v);
		}
		break;

	case MDataType::Variant::MSI_INVALID:
		// there is no content, and we already wrote datatype, so write nothing here.
		break;

	case MDataType::Variant::MSI_STRING:
	case MDataType::Variant::MSI_MBCS_STRING:
		{
#ifndef NO_WCHAR_SUPPORT
			MBase::ReturnWString lStr( ReadWString() );
#else
			MBase::ReturnString lStr( ReadString() );
#endif
			MDataType::Variant lvar( lStr.c_str());
			lvar.swap(v);
		}
		break;

	default:
		_ASSERT(false);
		break;
	}

	return (*this);
}

void ReadDataStreamImpl::Read(MDataType::Variant * pv, unsigned Int32 cb)
{
	VerifyType(T_VARIANT, cb);

	MDataType::Variant * lpv = pv;
	for(Int64 i = 0; i < cb; i ++)
	{	// for each Variant in the array

		// get Variant type
		Int32 lTemp = 0;
		(*this) >> lTemp; ;

		MDataType::Variant::DATA_TYPE lDT = ( MDataType::Variant::DATA_TYPE) lTemp;

		switch ( lDT )
		{
		case MDataType::Variant::MSI_SHORT:
			{
				short lVal = 0;
				(*this) >> lVal;
				MDataType::Variant lvar(lVal);
				lpv->swap(lvar);
			}
			break;
		case MDataType::Variant::MSI_INTEGER:
		case MDataType::Variant::MSI_LONG:
			{
				Int32 lVal = 0;
				(*this) >> lVal;
				MDataType::Variant lvar(lVal);
				lpv->swap(lvar);
			}
			break;
		case MDataType::Variant::MSI_FLOAT:
			{
				double lVal = 0;
				(*this) >> lVal;
				MDataType::Variant lvar( (float)lVal);
				lpv->swap(lvar);
			}
			break;
		case MDataType::Variant::MSI_DOUBLE:
			{
				double lVal = 0;
				(*this) >> lVal;
				MDataType::Variant lvar( lVal);
				lpv->swap(lvar);
			}
			break;

		case MDataType::Variant::MSI_DATE:
		case MDataType::Variant::MSI_TIME:
		case MDataType::Variant::MSI_DATETIME:
			{
				MDataType::DateTime lS;
				(*this) >> lS;
				MDataType::Variant lvar( lS);
				lpv->swap(lvar);
			}
			break;

		case MDataType::Variant::MSI_CELLFMTDATA:
			{
				MDataType::CellFmtData lCellFmtData;
				(*this) >> lCellFmtData;
				MDataType::Variant lvar(lCellFmtData);
				lpv->swap(lvar);
			}
			break;

		case MDataType::Variant::MSI_BINARY:
			{
				// read length. We use 64-bit integer for expandability.
				Int64 lLen = 0;
				(*this) >> lLen;

				// read complementary length
				Int64 lComp = 0;
				(*this) >> lComp;

				if ( lComp != ~ lLen )
				{
					throw MBase::ExceptionImplementation<ReadStreamException>(
									"Binary length is corrupted.");
				}

				// then read the real binary data
				MBase::StrongArrayPtr<unsigned char> lMemPtr(new unsigned char [lLen]);

				mpBStream->Read(lMemPtr.Get(), lLen);

				MDataType::BinaryPtr lBP(MDataType::CreateBinary(lMemPtr.Get(), lLen));

				MDataType::Variant lvar( lBP.Get() );
				lpv->swap(lvar);
			}
			break;

		case MDataType::Variant::MSI_INVALID:
			// there is no content, and we already wrote datatype, so write nothing here.
			break;

		case MDataType::Variant::MSI_STRING:
		case MDataType::Variant::MSI_MBCS_STRING:
			{
#ifndef NO_WCHAR_SUPPORT
				MBase::ReturnWString lStr( ReadWString() );
#else
				MBase::ReturnString lStr( ReadString() );
#endif
				MDataType::Variant lvar( lStr.c_str());
				lpv->swap(lvar);
			}
			break;

		default:
			_ASSERT(false);
			break;
		}
		lpv ++;
	}
}


// special function for enhanced robustness. The original value
// and its complementary value are expected to exist in the stream,
// so the reader can compare the consecutive data to ensure the data is good.
Int32 ReadDataStreamImpl::ReadSafeInt()
{
	VerifyType(T_SAFEINT);

	Int32 lVal = 0;
	(*this) >> lVal;

	Int32 lCompVal = 0;
	(*this) >> lCompVal;

	if ( lCompVal != ~ lVal )
	{
		throw MBase::ExceptionImplementation<ReadStreamException>("SafeRead fails due to unmatched data,");
	}

	return lVal;
}

void ReadDataStreamImpl::Rewind()
{
	mpBStream->Rewind();

	// Skip header only when it is not shared mem based.
	// The shared mem based stream does not support header for encryption etc.
	if(!mbSharedMemBased) {
		SkipHeader();
	}
}

void ReadDataStreamImpl::Skip(Int64 dwOffset)
{
	mpBStream->Skip(dwOffset);
}

void ReadDataStreamImpl::Skip(unsigned Int32 dwOffset)
{
	mpBStream->Skip(dwOffset);
}

Int64 ReadDataStreamImpl::Get64BitSize() throw()
{
	_ASSERTE(!mbSharedMemBased);
	return mpBStream->Get64BitSize();
}

unsigned Int32 ReadDataStreamImpl::GetSize()
{
	//_ASSERTE(!mbSharedMemBased);
	// This function should not be called when shared memory is used.
	// However to improve the buffer management performance, we allow this function to be called, but 
	// it returns special value "0" when shared memory is used.
	if(mbSharedMemBased) {
		return 0;
	}

	Int64 lSize = mpBStream->Get64BitSize();
	if ( MoreThan32Bit(lSize))
	{
		throw  MBase::ExceptionImplementation<ReadStreamException>("Stream size is bigger than 4G.");
	}
	return (unsigned Int32) lSize;
}

Int64 ReadDataStreamImpl::GetNumberOfUnreadBytes() throw()
{
	_ASSERTE(!mbSharedMemBased);
	return mpBStream->GetNumberOfUnreadBytes();
}

MDataType::BinaryCollection* ReadDataStreamImpl::GiveOutBinaryCollection()
{
	_ASSERTE(!mbSharedMemBased);
	return mpBStream->GiveOutBinaryCollection();
}

MDataType::BinaryCollection* ReadDataStreamImpl::CopyOutBinaryCollection() const
{
	_ASSERTE(!mbSharedMemBased);
	return mpBStream->CopyOutBinaryCollection();
}

// This call should not put the header data into the returned binary
MDataType::BinaryCollection* ReadDataStreamImpl::CopyOutContentBinaryCollection() const
{
	_ASSERTE(!mbSharedMemBased);
	return mpBStream->CopyOutBinaryCollection(mHeaderSize);
}

void ReadDataStreamImpl::Delete() throw()
{
	delete this;
}

bool ReadDataStreamImpl::WaitForData(unsigned Int32 iTimeOut)
{
	_ASSERTE(mbSharedMemBased);
	return mpBStream->WaitForData(iTimeOut);
}

ReadDataStreamImpl::ReadDataStreamImpl(MDataType::Binary* ipB, MBase::MemoryContract * ipMemCon )
: mpBStream(new ReadByteStreamImpl(ipB))
, mLongVer(false)
, mRawFormat(false)
, mHeaderSize(0)
, mMemConPtr(ipMemCon)
, mbSharedMemBased(false)
{
	Init();
}

ReadDataStreamImpl::ReadDataStreamImpl(MDataType::BinaryCollection* ipB, MBase::MemoryContract * ipMemCon )
: mpBStream(new ReadByteStreamImpl(ipB))
, mLongVer(false)
, mHeaderSize(0)
, mMemConPtr(ipMemCon)
, mbSharedMemBased(false)
{
	Init();
}

ReadDataStreamImpl::ReadDataStreamImpl(MDataType::BinaryCollection* ipB,
									   MultiReadStream* ipMR,
									   MBase::MemoryContract * ipMemCon)
: mpBStream(new ReadByteStreamImpl(ipB, ipMR))
, mLongVer(false)
, mMemConPtr(ipMemCon)
, mbSharedMemBased(false)
{
	Init();
}

ReadDataStreamImpl::ReadDataStreamImpl(MFileSystem::ReadableFile::Ptr& iFilePtr)
: mLongVer(false)
, mpBStream(new ReadByteStreamImpl(iFilePtr))
, mbSharedMemBased(false)
{
	Init();
}


ReadDataStreamImpl::ReadDataStreamImpl(MBase::MemoryContract* ipMemCon)
:mbSharedMemBased(false)
, mpBStream(NULL)
{
	Init();
}
#ifndef __ANDROID__
ReadDataStreamImpl::ReadDataStreamImpl(MMultiProcess::SharedMemory* ipSharedMemPtr, MBase::MemoryContract* ipMemCon)
: mpBStream(new ReadSharedMemByteStreamImpl(ipSharedMemPtr))
, mbSharedMemBased(true)
, mMemConPtr(ipMemCon)
, mLongVer(false)
, mRawFormat(true)
{
	_ASSERTE(mpBStream.Get());

	mHeaderSize = 0;
	mStreamHeader = 0;
	mEncryptKey = "";
	//Init(); //The shared member based serialization does not support header yet.
}
#endif
ReadDataStreamImpl::ReadDataStreamImpl(MSerialization::ReadByteStream * ipSharedMemByteStream, MBase::MemoryContract* ipMemCon)
: mpBStream(ipSharedMemByteStream)
, mbSharedMemBased(true)
, mMemConPtr(ipMemCon)
, mLongVer(false)
, mRawFormat(true)
{
	_ASSERTE(mpBStream.Get());

	mHeaderSize = 0;
	mStreamHeader = 0;
	mEncryptKey = "";
	//Init(); //The shared member based serialization does not support header yet.
}

void ReadDataStreamImpl::Init()
{
	// Initialization code, common to each constructor
	DataStreamHeader lHeader;
	lHeader.Load(mpBStream.Get());

	mStreamHeader = lHeader.GetHeaderValue();
	mLongVer = lHeader.GetLongVersion();
	mRawFormat = lHeader.GetRawFormat();
	if ( mRawFormat )
	{
#ifdef LITTLE_ENDIAN
		if ( lHeader.IsBigEndian())
			throw  MBase::ExceptionImplementation<ReadStreamException>("Cannot read data: Little Endian machine cannot read Big Endian raw data.");
#else
		if ( ! lHeader.IsBigEndian())
			throw  MBase::ExceptionImplementation<ReadStreamException>("Cannot read data: Big Endian machine cannot read Little Endian raw data.");
#endif
	}

	mHeaderSize = lHeader.GetHeaderSize();
#ifndef __ANDROID__
	if (mStreamHeader & (PDS_RC4_ENCRYPT_STREAM |PDS_TEA_ENCRYPT_STREAM |PDS_AES_ENCRYPT_STREAM))
	{
		mEncryptKey = lHeader.GetKey().c_str();
	}

	if (mStreamHeader & PDS_SIMPLE_ENCRYPT_STREAM)
	{
		mEncryptor.Reset(MEncryptor::CreateSIMPLEEncryptorDecryptor());
	}
	else if (mStreamHeader & PDS_RC4_ENCRYPT_STREAM)
	{
		MBase::ReturnWString lKey(MBase::UTF8ToWideChar(mEncryptKey.c_str()).c_str());
		mEncryptor.Reset(MEncryptor::CreateMD5EncryptorDecryptor(lKey.c_str()));
	}
	else if (mStreamHeader & PDS_TEA_ENCRYPT_STREAM)
	{
		MBase::ReturnWString lKey(MBase::UTF8ToWideChar(mEncryptKey.c_str()).c_str());
		mEncryptor.Reset(MEncryptor::CreateRIPEMD160EncryptorDecryptor(lKey.c_str()));
	}
	else if (mStreamHeader & PDS_AES_ENCRYPT_STREAM)
	{
		MBase::ReturnWString lKey(MBase::UTF8ToWideChar(mEncryptKey.c_str()).c_str());
		mEncryptor.Reset(MEncryptor::CreateRIPEMD160EncryptorDecryptor(lKey.c_str()));
	}
#endif
}

ReadDataStreamImpl::~ReadDataStreamImpl() throw()
{
/*	if(mpBStream) {
		delete mpBStream;
		mpBStream = 0;
	}
*/
}

void ReadDataStreamImpl::StartCount()
{
	_ASSERTE(!mbSharedMemBased);
	mpBStream->StartCount();
}

Int64 ReadDataStreamImpl::GetCount()
{
	_ASSERTE(!mbSharedMemBased);
	return mpBStream->GetCount();
}

void ReadDataStreamImpl::StopCount()
{
	_ASSERTE(!mbSharedMemBased);
	mpBStream->StopCount();
}

void ReadDataStreamImpl::VerifyType(TYPE iType)
{
	if ( mLongVer )
	{
		unsigned char lType = T_INVALID;

		mpBStream->Read(&lType, (unsigned Int32)sizeof(lType));

		if ( iType != (TYPE) lType )
		{
			throw  MBase::ExceptionImplementation<ReadStreamException>("Cannot read data: wrong data type.");
		}
	}
}

// set a timeout value for all subsequent read actions.
void ReadDataStreamImpl::SetTimeout(Int64 iTimeoutInMilliseconds)
{
	mpBStream->SetTimeout(iTimeoutInMilliseconds);
}


/*--------------------------
	private
----------------------------*/
/*
void ReadDataStreamImpl::ConvertLEStreamToBEValue(Int32 byteCnt,
												 unsigned char *pv,
												 unsigned Int32 cb)
{
	unsigned char * lpSrc = pv;

	for(Int32 i = 0; i < cb; i ++)
	{	// for each element of the array

		for(Int32 j = byteCnt - 1; j >= 0; j --)
		{	// for each byte of the single value
			mpBStream->Read(lpSrc + j, (unsigned Int32) 1);
		}

		lpSrc += byteCnt;
	}
}
*/

template<class A, class B>
void TempFastRead(B cb, unsigned char* pv, ReadByteStream * ipStream)
{
	A lCache[2000/sizeof(A)];	// 2000 bytes

	A * lpCache = lCache;
	MBase::StrongArrayPtr<A> lSPtrA;
	if ( cb > 2000/sizeof(A))
	{
		A* lpACache = new A[cb];
		lpCache = lpACache;
		lSPtrA.Acquire(lpACache);
	}

	ipStream->Read((unsigned char*) lpCache, (Int64) (cb * sizeof(A)) );

	A* lpTempCache=lpCache;
	A* lpTempMem = (A*) pv;

	switch (sizeof(A) )
	{
	case 2:
		{
			for (B i = 0; i < cb; i ++)
			{	// for each element of the array
				register unsigned short lpShort = *lpTempCache++;
				*lpTempMem++ = const_swap2(lpShort);
			}
		}
		break;
	case 4:
		{
			for (B i = 0; i < cb; i ++)
			{	// for each element of the array
				register unsigned Int32 lpShort = *lpTempCache++;
				*lpTempMem++ = const_swap4(lpShort);
			}
		}
		break;
	case 8:
		{
			for (B i = 0; i < cb; i ++)
			{	// for each element of the array
				register unsigned Int64 lpShort = *lpTempCache++;
				*lpTempMem++ = const_swap8(lpShort);
			}
		}
		break;
	default:
		_ASSERTE(false);	// this should be used by only three types
		throw  MBase::ExceptionImplementation<ReadStreamException>("Wrong conversion size.");
		break;
	}
}


void ReadDataStreamImpl::ConvertLEStreamToBEValue(Int32 byteCnt,
												 unsigned char *pv,
												 unsigned Int32 cb)
{
	switch (byteCnt)
	{
	case 2:
		TempFastRead<unsigned short, unsigned Int32>(cb, pv, mpBStream.Get());
		break;
	case 4:
		TempFastRead<unsigned Int32, unsigned Int32>(cb, pv, mpBStream.Get());
		break;
	case 8:
		TempFastRead<unsigned Int64, unsigned Int32>(cb, pv, mpBStream.Get());
		break;
	default:
		{
			unsigned char * lpSrc = pv;

			for(Int32 i = 0; i < (Int32)cb; i ++)
			{	// for each element of the array
				for(Int32 j = byteCnt - 1; j >= 0; j --)
				{	// for each byte of the single value
					mpBStream->Read(lpSrc + j, (unsigned Int32) 1);
				}

				lpSrc += byteCnt;
			}
		}
		break;
	}

}


void ReadDataStreamImpl::ConvertLEStreamToBEValue(Int32 byteCnt,
												 unsigned char *pv,
												 Int64 cb)
{
	switch (byteCnt)
	{
	case 2:
		TempFastRead<unsigned short, Int64>(cb, pv, mpBStream.Get());
		break;
	case 4:
		TempFastRead<unsigned Int32, Int64>(cb, pv, mpBStream.Get());
		break;
	case 8:
		TempFastRead<unsigned Int64, Int64>(cb, pv, mpBStream.Get());
		break;
	default:
		{
			unsigned char * lpSrc = pv;

			for(Int64 i = 0; i < cb; i ++)
			{	// for each element of the array

				for(Int32 j = byteCnt - 1; j >= 0; j --)
				{	// for each byte of the single value
					mpBStream->Read(lpSrc + j, (unsigned Int32) 1);
				}

				lpSrc += byteCnt;
			}
		}
		break;
	}
}

unsigned short ReadDataStreamImpl::GetHeader()
{
	_ASSERTE(!mbSharedMemBased);
	short lHeader=0;
/*	(*this) >> lHeader;

	unsigned char lFlag = 0;
	mpBStream->Read(&lFlag, (unsigned int) sizeof(lFlag));
	if ( lFlag != GN_Data_Stream_Flag )
	{
		throw  MBase::ExceptionImplementation<ReadStreamException>("Corrupted stream: wrong header flag.");
	}

	short lComp = 0;
	(*this) >> lComp;

	if ( lComp != ~lHeader)
	{
		throw  MBase::ExceptionImplementation<ReadStreamException>("Corrupted stream: wrong header data.");
	}

*/	return lHeader;
}

void ReadDataStreamImpl::SkipHeader()
{
	_ASSERTE(!mbSharedMemBased);
	mpBStream->Skip((unsigned Int32)mHeaderSize);
}

void ReadDataStreamImpl::VerifyType(TYPE iType, Int64 cb)
{
	if ( mLongVer )
	{
		unsigned char lType = T_INVALID;
		mpBStream->Read(&lType, (unsigned Int32) sizeof(lType) );
		if ( ( lType & T_ARRAY ) == 0 && (lType&T_SMALLARRAY) == 0 )
		{
			_ASSERT(false);
			throw  MBase::ExceptionImplementation<ReadStreamException>("Cannot read data: data is not in array.");
		}


		if ( lType & T_ARRAY)
		{
			unsigned char lArr = iType | T_ARRAY ;

			if ( lArr != lType )
			{
				_ASSERT(false);
				throw  MBase::ExceptionImplementation<ReadStreamException>("Cannot read data: wrong data type.");
			}

			Int64 lcnt = 0;

#ifdef LITTLE_ENDIAN
			mpBStream->Read( (unsigned char*) &lcnt, (unsigned Int32) sizeof(lcnt));
#else
			if (mRawFormat)
				mpBStream->Read( (unsigned char*) &lcnt, (unsigned Int32) sizeof(lcnt));
			else
				ConvertLEStreamToBEValue(sizeof(lcnt), (unsigned char*) &lcnt, (unsigned Int32)1);
#endif

			if ( lcnt != cb)
			{
				_ASSERT(false);
				throw  MBase::ExceptionImplementation<ReadStreamException>("Cannot read data: wrong array size.");
			}
		}
		else
		{
			unsigned char lArr = iType | T_SMALLARRAY ;

			if ( lArr != lType )
			{
				_ASSERT(false);
				throw  MBase::ExceptionImplementation<ReadStreamException>("Cannot read data: wrong data type.");
			}

			unsigned Int32 lcnt = 0;

#ifdef LITTLE_ENDIAN
			mpBStream->Read( (unsigned char*) &lcnt, (unsigned Int32)sizeof(lcnt));
#else
			if (mRawFormat)
				mpBStream->Read( (unsigned char*) &lcnt, (unsigned Int32)sizeof(lcnt));
			else
				ConvertLEStreamToBEValue(sizeof(lcnt), (unsigned char*) &lcnt, (unsigned Int32)1);
#endif

			if ( lcnt != cb)
			{
				_ASSERT(false);
				throw  MBase::ExceptionImplementation<ReadStreamException>("Cannot read data: wrong array size.");
			}
		}
	}
}

TYPE ReadDataStreamImpl::GetItemType()
{
	_ASSERTE(!mbSharedMemBased);

	if ( mLongVer )
	{
		unsigned char lType = T_INVALID;
		mpBStream->Peek(&lType, (unsigned Int32)sizeof(lType));
		return (TYPE)lType;
	}

	_ASSERTE(false);
	throw  MBase::ExceptionImplementation<ReadStreamException>("method supported for safe stream only");
}

bool ReadDataStreamImpl::IsEncrypted()
{
#ifndef __ANDROID__
	return (mStreamHeader & (PDS_SIMPLE_ENCRYPT_STREAM | PDS_RC4_ENCRYPT_STREAM |PDS_TEA_ENCRYPT_STREAM |PDS_AES_ENCRYPT_STREAM)) ? true : false;
#else
	return false;
#endif
}

}
