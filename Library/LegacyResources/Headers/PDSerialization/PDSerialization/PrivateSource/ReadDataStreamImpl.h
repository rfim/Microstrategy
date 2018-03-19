//==============================================================================================
//	FILENAME	:	ReadDataStreamImpl.h
//	AUTHOR		:	Yi Luo
//	CREATION	:	07/12/2002
//	Copyright (C) MicroStrategy, Inc. 2002
//==============================================================================================

#ifndef Serialization_ReadDataStreamImpl_H
#define Serialization_ReadDataStreamImpl_H

#include "PDSerialization/PDSerialization/PDSerialization.h"
#include "DataType/DataType/Binary.h"
#include "DataType/DataType/DateTime.h"
#include "DataType/DataType/CellFmtData.h"
#include "DataType/DataType/Variant.h"
#include "Base/Base/GUID.h"
#include "PDSerialization/PDSerialization/ReadDataStream.h"
#include "ReadByteStreamImpl.h"
#include "Facilities.h"
#ifndef __ANDROID__
	#include "ReadSharedMemByteStreamImpl.h"
	#include "Encryptor/Encryptor/EncryptorDecryptor.h"
#endif

namespace MSerialization
{

// interface for reading data from a stream.
/* the signature for reading a single element is the same
	as the one for reading an array. The difference is in 
	the value passed into the function. A value greater than 
	the sizeof the basic type is the signal that the caller wants 
	to read an array, and the number of element is cb/sizeof(dt).
*/

class ReadDataStreamImpl : public ReadDataStream
{
public:

	// read binary.
	// throw
	virtual ReadDataStream& operator >> (MDataType::Binary& iValue);

	// bool and bool array
	virtual ReadDataStream& operator >> (bool& iValue);
	virtual void Read(bool * iopValueArray, unsigned Int32 iElementCount);		
	// raw binary
	virtual void Read(unsigned char * iopValueArray, unsigned Int32 iElementCount);		
	
	// short and short array
	virtual ReadDataStream& operator >> (short & iValue);
	virtual ReadDataStream& operator >> (unsigned short & iValue);
	virtual void Read(short * iopValueArray, unsigned Int32 iElementCount);		
	// long and long array
	virtual ReadDataStream& operator >> (Int32& iValue);
	virtual ReadDataStream& operator >> (unsigned Int32 & iValue);
	virtual void Read(Int32* iopValueArray, unsigned Int32 iElementCount);		
	// 64-bit integer and array
	virtual ReadDataStream& operator >> (Int64& iValue);
	virtual void Read(Int64* iopValueArray, unsigned Int32 iElementCount);		

	// float and float array
	virtual ReadDataStream& operator >> (float& iValue);
	virtual void Read(float* iopValueArray, unsigned Int32 iElementCount);	

	// double and double array
	virtual ReadDataStream& operator >> (double& iValue);
	virtual void Read(double * iopValueArray, unsigned Int32 iElementCount);	
#ifndef NO_WCHAR_SUPPORT
	// wide string
	virtual MBase::ReturnWString ReadWString();
#endif
	virtual MBase::ReturnString ReadString();

	virtual char ReadByte();
#ifndef NO_WCHAR_SUPPORT
	virtual wchar_t ReadWChar();
#endif
	// serialization of complext data type 

	// GUID and GUID array
	virtual ReadDataStream& operator >> (GUID& iValue);
	virtual void Read(GUID * iopValueArray, unsigned Int32 iElementCount );		
	
	// Variant and Variant array
	// when reading Variant array, the caller must allocate the Variant array, 
	// If the Variants must use a Buffer, each element of the Array must be
	// be initialized with the Buffer prior to call Read().
	virtual ReadDataStream& operator >> (MDataType::Variant& iValue);
	virtual void Read(MDataType::Variant * iopValueArray, unsigned Int32 iElementCount);		


	virtual ReadDataStream& operator >> (SerializableObject& irValue);

	// special function for enhanced robustness. The original value and its 
	// complementary value are expected to exist in the stream, so the reader 
	// can compare the consecutive data to ensure the data is good.
	virtual Int32 ReadSafeInt();

	// rewind the read pointer to the beginning of the stream.
	virtual void Rewind() ;

	// get the physical size of the stream, including all the extra information for 
	// any internal purpose such as error checking, bookkeeping and padding, 
	virtual unsigned Int32 GetSize();
	virtual Int64 Get64BitSize() throw();
	
	// get the number of bytes unread in the stream
	virtual Int64 GetNumberOfUnreadBytes() throw();

	// after this call, the mStream contains no more data, and any further reading operation
	// will fail.
	virtual MDataType::BinaryCollection* GiveOutBinaryCollection();

	// after this call, the mStream contains the same data, and any further reading operation
	// will work as before.
	virtual MDataType::BinaryCollection* CopyOutBinaryCollection() const;

	// after this call, the mStream contains the same data, and any further reading operation
	// will work as before.
	// This call should not put the header data into the returned binary
	virtual MDataType::BinaryCollection* CopyOutContentBinaryCollection() const ;

	// delete this stream. This is the only way a stream can be deleted.
	virtual void Delete() throw() ;


	// DateTime and DateTime array
	virtual ReadDataStream& operator >> (MDataType::DateTime & v);
	virtual void Read(MDataType::DateTime * iopValueArray, unsigned Int32 iElementCount);

	// DynamicTime and its array
	virtual ReadDataStream& operator >> (MDataType::DynamicTime& orValue);
	virtual void Read(MDataType::DynamicTime * iopValueArray, unsigned Int32 iElementCount);		

	// CellFmtData and CellFmtData array
	virtual ReadDataStream& operator >> (MDataType::CellFmtData & v);
	virtual void Read(MDataType::CellFmtData * iopValueArray, unsigned Int32 iElementCount);

	ReadDataStreamImpl(MDataType::Binary* ipB, MBase::MemoryContract * ipMemCon = 0);
	ReadDataStreamImpl(MDataType::BinaryCollection* ipB, MBase::MemoryContract * ipMemCon = 0);
	ReadDataStreamImpl(MFileSystem::ReadableFile::Ptr& iFilePtr);

	ReadDataStreamImpl(MBase::MemoryContract* ipMemCon);
#ifndef __ANDROID__
	// This constructor is used to create a shared memory based read data stream
	ReadDataStreamImpl(MMultiProcess::SharedMemory* ipSharedMemPtr, MBase::MemoryContract* ipMemCon);
#endif
	// This constructor is used to create a shared memory based read data stream from a 
	// shared memory based read byte stream directly
	ReadDataStreamImpl(MSerialization::ReadByteStream * ipSharedMemByteStream, MBase::MemoryContract* ipMemCon);

	virtual ~ReadDataStreamImpl() throw();

	/// Constructor used to make a read data stream with a reference to a multi-read stream.
	ReadDataStreamImpl(MDataType::BinaryCollection* ipB,
					   MultiReadStream* ipMR,
					   MBase::MemoryContract * ipMemCon = 0);


	void StartCount();
	Int64 GetCount();
	void StopCount();

	void VerifyType(TYPE iType);

	// skip the spcified bytes and start the next reading from the new location.
	virtual void Skip(Int64 dwOffset);
	virtual void Skip(unsigned Int32 dwOffset);

	virtual TYPE GetItemType();

	virtual bool IsEncrypted();

	virtual bool WaitForData(unsigned Int32 iTimeOut);

	// set a timeout value for all subsequent read actions.
	virtual void SetTimeout(Int64 iTimeoutInMilliseconds);

private:

	void ConvertLEStreamToBEValue(Int32 byteCnt, unsigned char *pv, unsigned Int32 cb);
	void ConvertLEStreamToBEValue(Int32 byteCnt, unsigned char *pv, Int64 cb);
	unsigned short GetHeader();
	void VerifyType(TYPE iType, Int64 cb);
	void SkipHeader();
	void Init();

	// If the current read data stream is shared memory based
	// True: read data stream is shared memory based
	// False: read data stream is named pipe based
	bool mbSharedMemBased;

	// The pointer to a read byte steream
	MBase::StrongPtr<ReadByteStream>  mpBStream;

	// ReadSharedMemByteStreamImpl * mpSharedMemBStream;
	// ReadByteStreamImpl mStream;

	unsigned short mStreamHeader;
	bool mLongVer;
	bool mRawFormat;
	MBase::SmartPtrI<MBase::MemoryContract> mMemConPtr;
#ifndef __ANDROID__
	MEncryptor::EncryptorDecryptorPtr mEncryptor;
#endif
	std::string mEncryptKey;
	Int32 mHeaderSize;
};


};

#endif  // Serialization_ReadDataStreamImpl_H
