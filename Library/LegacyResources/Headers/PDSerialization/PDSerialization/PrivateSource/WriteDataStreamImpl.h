//==============================================================================================
//	FILENAME	:	WriteDataStreamImpl.h
//	AUTHOR		:	Yi Luo
//	CREATION	:	07/10/2002
//	Copyright (C) MicroStrategy, Inc. 2002
//==============================================================================================

#ifndef Serialization_WriteDataStreamImpl_H
#define Serialization_WriteDataStreamImpl_H

#include "PDSerialization/PDSerialization/WriteDataStream.h"
#include "WriteByteStreamImpl.h"
#include "Facilities.h"
#ifndef __ANDROID__
	#include "WriteSharedMemByteStreamImpl.h"
	#include "Encryptor/Encryptor/EncryptorDecryptor.h"
#endif

namespace MSerialization
{

// class for writing data into a stream
class WriteDataStreamImpl : public WriteDataStream
{
public:
	/* special function for enhanced robustness. The original value and its 
	 complementary value are both writen into the stream, so the reader can 
	 compare the consecutive data to ensure the data integrity
	 */
	void SafeWrite(Int32 v);

	// write binary data
	WriteDataStream& operator <<(const MDataType::Binary&pv);	

	// write Unicode string
#ifndef NO_WCHAR_SUPPORT
	WriteDataStream& operator <<(const wchar_t * pv);
#endif
	WriteDataStream& operator <<(const char * pv);	
	WriteDataStream& operator <<(float pv);
	WriteDataStream& operator <<(double pv);
	WriteDataStream& operator <<(bool pv);
	WriteDataStream& operator <<(short pv);
	WriteDataStream& operator <<(unsigned short pv);
	WriteDataStream& operator <<(Int32 pv);
	WriteDataStream& operator <<(unsigned Int32 pv);
	WriteDataStream& operator <<(Int64 pv);
	WriteDataStream& operator <<(const GUID& pv);
	WriteDataStream& operator <<(const MDataType::DateTime & pv);
	WriteDataStream& operator <<(const MDataType::DynamicTime & pv);
	WriteDataStream& operator <<(const MDataType::CellFmtData & pv);
	WriteDataStream& operator <<(const MDataType::Variant & pv);
	WriteDataStream& operator <<(const SerializableObject& ipObject);


	void Write(float const *pv, unsigned Int32 cb);	// write array of floats
	void Write(double const *pv, unsigned Int32 cb);	// write array of doubles
	void Write(bool const *pv, unsigned Int32 cb);	// write array of bools
	void Write(unsigned char const *pv, unsigned Int32 cb);	// write array of bytes (unsigned chars)
	void Write(short const *pv, unsigned Int32 cb);	// write array of shorts
	void Write(Int32 const *pv, unsigned Int32 cb);	// write array of ints
	void Write(Int64 const * pv, unsigned Int32 cb);	// write array of INT64s
	void Write(GUID const *pv, unsigned Int32 cb);	// write array of GUIDs
	void Write(MDataType::DateTime const *pv, unsigned Int32 cb);	// write array of DateTimes
	void Write(MDataType::CellFmtData const *pv, unsigned Int32 cb);	// write array of CellFmtDatas
	void Write(MDataType::DynamicTime const* ipValues, unsigned Int32 iElementCount);
	void Write(MDataType::Variant const *pv, unsigned Int32 cb);	// write array of Variants

	void WriteByte(char iChar);	
#ifndef NO_WCHAR_SUPPORT
	void WriteWChar(const wchar_t iChar);	
	void WriteString(const wchar_t * ipString);
#endif
	/* reserve a space of 64 bit integer in the stream, so the caller can fill 
		the space with special value, such as the length of a block. We reserve 
		64-bit integer because we want to ensure the block can be larger than 4G.
	*/
	StreamInt64Reserve* ReserveInt64();
	StreamInt32Reserve* ReserveInt32();

	/* return the binary data containing the stream, so other component can 
		save it to file or send it over network. 
	*/
	MDataType::Binary* GiveOutBinary();
	MDataType::BinaryCollection* GiveOutBinaryCollection();
	MDataType::BinaryCollection* CopyOutBinaryCollection();

	void CommitStream();

	Int64 Get64BitSize() throw();
	unsigned Int32 GetSize();

	void Rewind();

	void CopyAppend(const ReadDataStream& irReadStream);

	void Delete() throw();

	void Abandon();

	void SetTimeout(Int64 iTimeoutInMilliSeconds);

	WriteDataStreamImpl(bool iHasHeader, unsigned Int32 iBlockSize, 
						MBase::MemoryContract * ipMemCon, unsigned short header, const char* ipcKey=NULL);
	WriteDataStreamImpl(bool iHasHeader, unsigned Int32 iBlockSize, 
						MFileSystem::WriteableFile::Ptr& iFilePtr, unsigned short header, const char* ipcKey=NULL);
	WriteDataStreamImpl(MDataType::BinaryCollection* ipBins,
						unsigned Int32 iBlockSize, 
						MBase::MemoryContract * ipMemCon);
#ifndef __ANDROID__
	// This constructor is used to create a shared memory based write data stream
	WriteDataStreamImpl(MMultiProcess::SharedMemory* ipSharedMemPtr, unsigned Int32 iBlockSize = 1024);
#endif
	WriteDataStreamImpl(MSerialization::WriteByteStream* ipSharedMemWriteByteStreamPtr);
	
	virtual ~WriteDataStreamImpl() throw();

	void Merge(WriteDataStreamImpl& irStream);
	void WriteType(TYPE iType);
	void WriteType(TYPE iType, Int64 cnt);

	StreamType GetStreamType();
	MFileSystem::WriteableFile::ReturnPtr ReleaseFilePtr();
private:
	void ConvertBigEndianToLittleEndian(Int32 byteCnt, const unsigned char *pv, unsigned Int32 cb);
	void ConvertBigEndianToLittleEndian(Int32 byteCnt, const unsigned char *pv, Int64 cb);
	void Init(bool iHasHeader, unsigned short header, const char* ipcKey);

	//--------------------------------
	/*-------- data members ---------*/
	//--------------------------------

	WriteByteStream::Ptr mpBStream;
	unsigned short mHeader;
	bool mLongVer;
	bool mRawFormat;

	unsigned Int32 mBlockSize;
	MBase::SmartPtrI<MBase::MemoryContract> mMemConPtr;
#ifndef __ANDROID__
	MEncryptor::EncryptorDecryptorPtr mEncryptor;
	MEncryptor::EncryptionVersion mEncryptorVersion;
#endif
	std::string mEncryptKey;

	// fields only for local memory-based stream
	MDataType::BinaryCollectionPtr mBinCollectionPtr;
};

WriteDataStream* CreateWriteDataStreamWithNoHeader(bool iRawFormat, unsigned short = 1, MBase::MemoryContract * ipMemCon=0);

};

#endif // Serialization_WriteDataStreamImpl_H

