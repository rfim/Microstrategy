//==============================================================================================
//	FILENAME	:	WriteBlockStreamImpl.h
//	AUTHOR		:	Yi Luo
//	CREATION	:	07/15/2002
//	Copyright (C) MicroStrategy, Inc. 2002
//==============================================================================================

#ifndef Serialization_WriteBlockStreamImpl_H
#define Serialization_WriteBlockStreamImpl_H

#include "PDSerialization/PDSerialization/WriteBlockStream.h"
#include "WriteDataStreamImpl.h"
#include "PDCstack"

namespace MSerialization
{

class WriteBlockStreamImpl : public WriteBlockStream
{
public:
	void WriteBlockHeader(unsigned short blockType);
	void FinishWriteBlock();
	void FinishWriteBlock(unsigned short iBlockType);

	// Start a new list
	void StartList();

	// continue a new list
	void ContinueList();

	WriteBlockStreamImpl(MBase::MemoryContract * ipMemCon, unsigned short header, const char* ipcKey=NULL);
	WriteBlockStreamImpl(MFileSystem::WriteableFile::Ptr& iFilePtr, unsigned short header, const char* ipcKey=NULL);
	WriteBlockStreamImpl(MFileSystem::UpdateableFile::Ptr& iFilePtr, unsigned short iHeader);
	WriteBlockStreamImpl(MDataType::BinaryCollection* ipBinaries,
						MBase::MemoryContract * ipMemCon);
	WriteBlockStreamImpl(unsigned short header);
	virtual ~WriteBlockStreamImpl() throw() ;

	// functions for WriteDataStream
	void SafeWrite(Int32 v);

	// write binary data
	WriteDataStream& operator <<(const MDataType::Binary&pv);

	// write Unicode string
	WriteDataStream& operator<<(const wchar_t * pv);

	// write char string.
	WriteDataStream& operator<<(const char * ipString);


	WriteDataStream& operator<<(float pv);
	WriteDataStream& operator<<(double pv);
	WriteDataStream& operator<<(bool pv);
	WriteDataStream& operator<<(short pv);
	WriteDataStream& operator<<(unsigned short pv);
	WriteDataStream& operator<<(Int32 pv);
	WriteDataStream& operator<<(unsigned Int32 pv);
	WriteDataStream& operator<<(Int64 pv);
	WriteDataStream& operator<<(const GUID& pv);
	WriteDataStream& operator<<(const MDataType::DateTime & pv);
	WriteDataStream& operator<<(const MDataType::DynamicTime & pv);
	WriteDataStream& operator<<(const MDataType::CellFmtData & pv);
	WriteDataStream& operator<<(const MDataType::Variant & pv);
	WriteDataStream& operator<<(const SerializableObject& ipObject);

	void Write(float const *pv, unsigned Int32 cb);	// write array of floats
	void Write(double const *pv, unsigned Int32 cb);	// write array of doubles
	void Write(bool const *pv, unsigned Int32 cb);	// write array of bools
	void Write(unsigned char const *pv, unsigned Int32 cb);	// write array of bytes (unsigned chars)
	void Write(short const *pv, unsigned Int32 cb);	// write array of shorts
	void Write(Int32 const *pv, unsigned Int32 cb);	// write array of ints
	void Write(Int64 const * pv, unsigned Int32 cb);	// write array of INT64s
	void Write(GUID const *pv, unsigned Int32 cb);	// write array of GUIDs
	void Write(MDataType::DateTime const *pv, unsigned Int32 cb);	// write array of DateTimes
	void Write(MDataType::DynamicTime const* ipValues, unsigned Int32 iElementCount);
	void Write(MDataType::CellFmtData const *pv, unsigned Int32 cb);	// write array of CellFmtDatas
	void Write(MDataType::Variant const *pv, unsigned Int32 cb);	// write array of Variants

	void WriteByte(char iByte);
	void WriteWChar(const wchar_t iChar);

	void WriteString(const wchar_t * ipString);

	/* reserve a space of 64 bit integer in the stream, so the caller can fill
		the space with special value, such as the length of a block. We reserve
		64-bit integer because we want to ensure the block can be larger than 4G.
	*/
	StreamInt64Reserve* ReserveInt64();
	StreamInt32Reserve* ReserveInt32();

	/* return the binary data containing the stream as a continuous block.
		This will cause the underlying binary data to be copied and compacted.
	*/
	MDataType::Binary* GiveOutBinary();

	/* return the binary data containing the stream as it is.
		This will NOT cause the underlying binary data to be copied and compacted.
	*/
	MDataType::BinaryCollection* GiveOutBinaryCollection();
	MDataType::BinaryCollection* CopyOutBinaryCollection();

	Int64 Get64BitSize() throw();
	unsigned Int32 GetSize();

	void Rewind();

	void CopyAppend(const ReadDataStream& irReadStream);

	void Delete() throw();

	StreamType GetStreamType();
	MFileSystem::WriteableFile::ReturnPtr ReleaseFilePtr();

	void CommitStream();
	void Abandon();

	void SetTimeout(Int64 iTimeoutInMilliseconds) { mpPDStreamStack->SetTimeout(iTimeoutInMilliseconds); };

private:
	void IncreaseListCount();

	/*--------- data members -----------*/
	std::stack<StreamInt64Reserve*> mBlockPtrStack;
	std::stack<StreamInt32Reserve*> mBlockPtr32Stack;
	WriteDataStreamImpl* mpPDStreamStack;
	std::stack<Int64> mBlockSizeStack;
	std::stack<unsigned short> mBlockTypeStack;

	MBase::MemoryContractPtr mpMemCon;

	unsigned short mHeader;
	StreamInt32Reserve* mpListCount;
	StreamInt32Reserve* mpCompListCount;
	unsigned Int32 mListCount;
	bool mContinueList;
	std::string mEncryptKey;
};

};

#endif // Serialization_WriteBlockStreamImpl_H
