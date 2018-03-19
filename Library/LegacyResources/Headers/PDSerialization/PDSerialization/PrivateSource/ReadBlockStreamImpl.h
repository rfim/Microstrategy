//==============================================================================================
//	FILENAME	:	ReadBlockStreamImpl.h
//	AUTHOR		:	Yi Luo
//	CREATION	:	07/15/2002
//	Copyright (C) MicroStrategy, Inc. 2002
//==============================================================================================

#ifndef Serialization_ReadBlockStreamImpl_H
#define Serialization_ReadBlockStreamImpl_H

#include "PDSerialization/PDSerialization/ReadBlockStream.h"
#include <stack>

namespace MDataType
{
	class BinaryCollection;
	class Binary;
};

namespace MSerialization
{

class ReadBlockStreamImpl: public ReadBlockStream
{
public:

	void FinishReadBlock();
	unsigned short ReadBlockHeader();

	bool MoreDataInBlock();


	// signal the start of a list
	void StartReadList();

	// check whether the list has more element for reading.
	bool MoreElement() throw() ;


	ReadBlockStreamImpl(MDataType::BinaryCollection * ip, MBase::MemoryContract * ipMemCon );
	ReadBlockStreamImpl(ReadDataStream* ipDataStream);
	ReadBlockStreamImpl(MFileSystem::ReadableFile::Ptr& iFilePtr);
	~ReadBlockStreamImpl();

	// read binary.
	// throw
	ReadDataStream& operator >> (MDataType::Binary& iValue);

	// bool and bool array
	ReadDataStream& operator >> (bool& iValue);
	void Read(bool * iopValueArray, unsigned Int32 iElementCount);		

	// raw binary array
	void Read(unsigned char * iopValueArray, unsigned Int32 iElementCount);		

	// short and short array
	ReadDataStream& operator >> (short & iValue);
	ReadDataStream& operator >> (unsigned short & iValue);
	void Read(short * iopValueArray, unsigned Int32 iElementCount);		
	// long and long array
	ReadDataStream& operator >> (Int32& iValue);
	ReadDataStream& operator >> (unsigned Int32 & iValue);
	void Read(Int32* iopValueArray, unsigned Int32 iElementCount);		
	// 64-bit integer and array
	ReadDataStream& operator >> (Int64& iValue);
	void Read(Int64* iopValueArray, unsigned Int32 iElementCount);		

	// float and float array
	ReadDataStream& operator >> (float& iValue);
	void Read(float* iopValueArray, unsigned Int32 iElementCount);	

	// double and double array
	ReadDataStream& operator >> (double& iValue);
	void Read(double * iopValueArray, unsigned Int32 iElementCount);	
#ifndef NO_WCHAR_SUPPORT
	// wide string
	MBase::ReturnWString ReadWString();
#endif
	MBase::ReturnString ReadString();

	char ReadByte();
#ifndef NO_WCHAR_SUPPORT
	wchar_t ReadWChar();
#endif
	// serialization of complext data type 

	// GUID and GUID array
	ReadDataStream& operator >> (GUID& iValue);
	void Read(GUID * iopValueArray, unsigned Int32 iElementCount );		
	
	// DateTime and DateTime array
	ReadDataStream& operator >> (MDataType::DateTime& orValue);
	void Read(MDataType::DateTime * iopValueArray, unsigned Int32 iElementCount);

	// DynamicTime and its array
	ReadDataStream& operator >> (MDataType::DynamicTime& orValue);
	void Read(MDataType::DynamicTime * iopValueArray, unsigned Int32 iElementCount);		

	// CellFmtData and CellFmtData array
	ReadDataStream& operator >> (MDataType::CellFmtData& orValue);
	void Read(MDataType::CellFmtData * iopValueArray, unsigned Int32 iElementCount);
	

	// Variant and Variant array
	// when reading Variant array, the caller must allocate the Variant array, 
	// If the Variants must use a Buffer, each element of the Array must be
	// be initialized with the Buffer prior to call Read().
	ReadDataStream& operator >> (MDataType::Variant& iValue);
	void Read(MDataType::Variant * iopValueArray, unsigned Int32 iElementCount);		

	ReadDataStream& operator >> (SerializableObject& irValue);

	// special function for enhanced robustness. The original value and its 
	// complementary value are expected to exist in the stream, so the reader 
	// can compare the consecutive data to ensure the data is good.
	Int32 ReadSafeInt();

	// rewind the read pointer to the beginning of the stream.
	void Rewind() ;

	// get the physical size of the stream, including all the extra information for 
	// any internal purpose such as error checking, bookkeeping and padding, 
	unsigned Int32 GetSize() ;
	Int64 Get64BitSize() throw();
	
	// get the number of bytes unread in the stream
	Int64 GetNumberOfUnreadBytes() throw();

	MDataType::BinaryCollection* GiveOutBinaryCollection();
	MDataType::BinaryCollection* CopyOutBinaryCollection() const;
	MDataType::BinaryCollection* CopyOutContentBinaryCollection() const;

	// delete this stream. This is the only way a stream can be deleted.
	void Delete() throw() ;

	TYPE GetItemType();

	virtual bool IsEncrypted();

	bool WaitForData(unsigned Int32 iTimeOut);

	void SetTimeout(Int64 iTimeoutInMilliseconds) { mpDStream->SetTimeout(iTimeoutInMilliseconds); };

protected:
	// skip the spcified bytes and start the next reading from the new location.
	void Skip(Int64 dwOffset);
	void Skip(unsigned Int32 dwOffset);
	void Init();
	
private:
	ReadDataStream * mpDStream;
	std::stack<Int64> mBlockSizeStack;

	unsigned Int32 mLeftListCnt;

};

};

#endif // Serialization_ReadBlockStreamImpl_H
