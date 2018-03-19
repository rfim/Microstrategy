//==============================================================================================
//	FILENAME	:	DataStreamHeader.h
//	AUTHOR		:	Yi Luo
//	CREATION	:	2002-09-05
//
//	Copyright (C) MicroStrategy, Inc. 2002
//==============================================================================================

#ifndef Serialization_DataStreamHeader_H
#define Serialization_DataStreamHeader_H

#include "PDSerialization/PDSerialization/ReadByteStream.h"
#include "PDSerialization/PDSerialization/WriteByteStream.h"

namespace MSerialization
{

class DataStreamHeader
{
public:
	DataStreamHeader() throw();
	virtual ~DataStreamHeader() throw();

	void Load(unsigned char* ipReadMemory);
	void Load(ReadByteStream * ipReadStream);
	void Save(WriteByteStream * ipWriteStream) const;

	bool GetLongVersion() const throw();
	bool GetRawFormat() const throw();
	bool IsBigEndian() const throw();

	unsigned short GetHeaderValue() const throw();
	void SetHeaderValue(unsigned short iValue) throw();

	Int32 GetHeaderSize() throw();
#ifndef __ANDROID__
	MBase::ReturnString GetKey() const throw();
	void SetKey(const char* ipcKey) throw();
#endif
private:

	void Write(unsigned short iValue, WriteByteStream * ipoStream) const;
	unsigned short Read(unsigned char** ioppMemory);
	unsigned short Read(ReadByteStream * ipReadStream);

	unsigned short mValue;
	MDataType::BinaryPtr mKey;
};

};

#endif // Serialization_DataStreamHeader_H
