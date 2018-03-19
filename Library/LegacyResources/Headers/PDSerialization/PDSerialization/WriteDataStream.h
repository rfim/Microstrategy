//==============================================================================================
//	FILENAME	:	WriteDataStream.h
//	AUTHOR		:	Yi Luo
//	PROJECT		:	PDSerialization
//
//	CREATION	:	07/10/2002
//	Copyright (C) MicroStrategy, Inc. 2002
//==============================================================================================
#ifndef Serialization_WriteDataStream_H
#define Serialization_WriteDataStream_H

#include "PDSerialization.h"
#include "StreamInt64Reserve.h"
#include "DataType/DataType/Binary.h"
#include "DataType/DataType/BinaryCollection.h"
#include "DataType/DataType/DateTime.h"
#include "DataType/DataType/CellFmtData.h"
#include "DataType/DataType/Variant.h"
#include "Base/Base/GUID.h"
#include "Base/Defines/StrongPtr.h"
#include "Base/Base/Buffer.h"
#include "Base/Defines/SmartPtrI.h"
#include "FileSystem/FileSystem/WriteableFile.h"
#include "FileSystem/FileSystem/UpdateableFile.h"
#include "Synch/Defines/SmartPtr.h"
#if !defined(WIN32) && !defined(__ANDROID__)
#include "MultiProcess/SharedMemory/SharedMemory.h"
#endif
#include "PDSerialization/PDSerialization/WriteByteStream.h"

namespace MDataType
{
	class DynamicTime;
}

namespace MSerialization
{
	class ReadDataStream;

	/// const value for specifying the safe version of WriteDataStream
	const unsigned short LONG_VERSION = 0x8000;

	/**
		Interface for writing data into a stream.
		we use opeator << to write one single element, and use Write() to write an array.
		@par
		When writing array, the caller provides the start pointer of the array and
		the number of elements in the array.
		@par
		There are a safe version of the WriteDataStream which can be created using CreateSafeWriteDataStream().
		The safe stream will write extra type information into the stream, thus providing better
		error detection.

	*/
	class DLL_PDSERIALIZATION_EXIM WriteDataStream
	{
	public:

		/**
			Write integer value with enhanced robustness.
			The original value and its complementary value are both writen
			into the stream, so the reader can compare the consecutive data
			to ensure the data integrity.
			@param iValue the integer value.
			@throws WriteDataStreamException if the write fails.
		*/
		virtual void SafeWrite(Int32 iValue)=0;

		/** write binary data
			@param irBinValue the reference to the Binay
			@throws WriteStreamException if writing fails.
		*/
		virtual WriteDataStream& operator <<(const MDataType::Binary& irBinValue)=0;

#ifndef NO_WCHAR_SUPPORT
		/** write Unicode string.
			@param ipString the pointer to the wchar_t string
			@throws WriteStreamException if writing fails.
		*/
		virtual WriteDataStream& operator<<(const wchar_t * ipString)=0;
#endif

		/** write char string.
			@param ipString the pointer to the char string terminiated with '\0'
			@throws WriteStreamException if writing fails.
		*/
		virtual WriteDataStream& operator<<(const char * ipString)=0;

		/** write bool value.
			@param iValue the value to write
			@throws WriteStreamException if writing fails.
		*/
		virtual WriteDataStream& operator<<(bool iValue)=0;

		/** write short value.
			@param iValue the value to write
			@throws WriteStreamException if writing fails.
		*/
		virtual WriteDataStream& operator<<(short iValue)=0;

		/** write unsigned short value.
			@param iValue the value to write
			@throws WriteStreamException if writing fails.
		*/
		virtual WriteDataStream& operator<<(unsigned short iValue)=0;

		/** write 32 bit integer value.
			@param iValue the value to write
			@throws WriteStreamException if writing fails.
		*/
		virtual WriteDataStream& operator<<(Int32 iValue)=0;

		/** write unsigned 32 bit integer value.
			@param iValue the value to write
			@throws WriteStreamException if writing fails.
		*/
		virtual WriteDataStream& operator<<(unsigned Int32 iValue)=0;

		/** write 64 bit integer value.
			@param iValue the value to write
			@throws WriteStreamException if writing fails.
		*/
		virtual WriteDataStream& operator<<(Int64 iValue)=0;

		/** write float value.
			@param iValue the value to write
			@throws WriteStreamException if writing fails.
		*/
		virtual WriteDataStream& operator<<(float iValue)=0;

		/** write double value.
			@param iValue the value to write
			@throws WriteStreamException if writing fails.
		*/
		virtual WriteDataStream& operator<<(double iValue)=0;

		/** write GUID value.
			@param irValue the value to write
			@throws WriteStreamException if writing fails.
		*/
		virtual WriteDataStream& operator<<(const GUID& irValue)=0;

		/** write DateTime value.
			@param irValue the value to write
			@throws WriteStreamException if writing fails.
		*/
		virtual WriteDataStream& operator <<(const MDataType::DateTime & pv)=0;

		/** write DynamicTime value.
			@param irValue the value to write
			@throws WriteStreamException if writing fails.
		*/
		virtual WriteDataStream& operator <<(const MDataType::DynamicTime & pv)=0;

		/** write CellFmtData value
			@param pv is the value to write
			@throws WriteStreamException
		*/
		virtual WriteDataStream& operator <<(const MDataType::CellFmtData & pv)=0;

		/** Write Variant value.
			The variant type is written at first, then the value is written.
			@param irValue the value to write
			@throws WriteStreamException if writing fails.
		*/
		virtual WriteDataStream& operator<<(const MDataType::Variant & irValue)=0;

		/** Write Serializable object.
			Actually call the Serialize() function of the object
			@param irValue the object to serialize
			@throws WriteStreamException if writing fails.
		*/
		virtual WriteDataStream& operator<<(const SerializableObject& irValue)=0;


		/** write an array of bool values.
			@param ipValue the pointer to the start of the array.
			@param iElementCount the number of elements in the array to write.
			@throws WriteStreamException if writing fails.
		*/
		virtual void Write(bool const *ipValues, unsigned Int32 iElementCount)=0;

		/** write an array of bytes (unsigned char) values.
			@param ipValue the pointer to the start of the array.
			@param iElementCount the number of elements in the array to write.
			@throws WriteStreamException if writing fails.
		*/
		virtual void Write(const unsigned char *ipValues, unsigned Int32 iElementCount)=0;

		/** write an array of short values.
			@param ipValue the pointer to the start of the array.
			@param iElementCount the number of elements in the array to write.
			@throws WriteStreamException if writing fails.
		*/
		virtual void Write(short const *ipValues, unsigned Int32 iElementCount)=0;

		/** write an array of integer values.
			@param ipValue the pointer to the start of the array.
			@param iElementCount the number of elements in the array to write.
			@throws WriteStreamException if writing fails.
		*/
		virtual void Write(Int32 const *ipValues, unsigned Int32 iElementCount)=0;

		/** write an array of 64 bit integer values.
			@param ipValue the pointer to the start of the array.
			@param iElementCount the number of elements in the array to write.
			@throws WriteStreamException if writing fails.
		*/
		virtual void Write(Int64 const * ipValues, unsigned Int32 iElementCount)=0;

		/** write an array of float values.
			@param ipValue the pointer to the start of the array.
			@param iElementCount the number of elements in the array to write.
			@throws WriteStreamException if writing fails.
		*/
		virtual void Write(float const *ipValues, unsigned Int32 iElementCount)=0;

		/** write an array of double values.
			@param ipValue the pointer to the start of the array.
			@param iElementCount the number of elements in the array to write.
			@throws WriteStreamException if writing fails.
		*/
		virtual void Write(double const *ipValues, unsigned Int32 iElementCount)=0;

		/** write an array of GUID values.
			@param ipValue the pointer to the start of the array.
			@param iElementCount the number of elements in the array to write.
			@throws WriteStreamException if writing fails.
		*/
		virtual void Write(GUID const *ipValues, unsigned Int32 iElementCount)=0;

		/** write an array of DataTime values.
			@param ipValue the pointer to the start of the array.
			@param iElementCount the number of elements in the array to write.
			@throws WriteStreamException if writing fails.
		*/
		virtual void Write(MDataType::DateTime const* ipValues, unsigned Int32 iElementCount)=0;

		/** write an array of DynamicTime values.
			@param ipValue the pointer to the start of the array.
			@param iElementCount the number of elements in the array to write.
			@throws WriteStreamException if writing fails.
		*/
		virtual void Write(MDataType::DynamicTime const* ipValues, unsigned Int32 iElementCount)=0;

		/** write an array of CellFmtData values
			@param ipValues is the pointer to the start of the array.
			@param iElementCount is the number of elements in the array to write.
			@throws WriteStreamException if writing fails
		*/
		virtual void Write(MDataType::CellFmtData const* ipValues, unsigned Int32 iElementCount)=0;

		/** write an array of Variant values.
			@param ipValue the pointer to the start of the array.
			@param iElementCount the number of elements in the array to write.
			@throws WriteStreamException if writing fails.
		*/
		virtual void Write(MDataType::Variant const* ipValues, unsigned Int32 iElementCount)=0;

		/** write one byte.
			@param iChar the byte
			@throws WriteStreamException if writing fails.
		*/
		virtual void WriteByte(char iChar)=0;

#ifndef NO_WCHAR_SUPPORT
		/** write wide character.
			@param iChar the wchar_t character
			@throws WriteStreamException if writing fails.
		*/
		virtual void WriteWChar(const wchar_t iChar)=0;
#endif

		/**
			Reserve a space of 64 bit integer in the stream, so the caller can
			fill the space with special value.
			This function is usually used by BlockStrem to write the length of a block,
			but it can be used for other purposes too.
			@return a pointer to a ReserveInt64 inteface which can be used to write the data later on.
		*/
		virtual StreamInt64Reserve* ReserveInt64()=0;

		/**
			Reserve a space of 32 bit integer in the stream, so the caller can
			fill the space with special value.
			This function is usually used by BlockStrem to write the length of a block,
			but it can be used for other purposes too.
			@return a pointer to a ReserveInt64 inteface which can be used to write the data later on.
			@throws WriteStreamException if writing fails.
		*/
		virtual StreamInt32Reserve* ReserveInt32()=0;

		/**
			Return the binary data containing the stream as one continuous block..
			Other component can get the binary, then save it to file or send it over network.
			@par
			This call gives up the ownership of the data. So the caller must release the Binary
			approriately.
			@par
			After this call, the caller can still write into this Stream, but the newly written data
			is not in the returned Binary. To get those data, the caller must call the GiveoutBinary again.
			@return a pointer to the Binary data.
			@throws WriteStreamException if the Binary cannot be created.
		*/
		virtual MDataType::Binary* GiveOutBinary()=0;

		/**
			Return the binary data containing the stream as a list of blocks.
			Other component can get the binary, then save it to file or send it over network.
			@par
			This call gives up the ownership of the data. So the caller must release the Binary
			approriately.
			@par
			After this call, the caller can still write into this Stream, but the newly written data
			is not in the returned Binary. To get those data, the caller must call the GiveoutBinary again.
			@return a pointer to the generated BinaryCollection.
			@throws WriteStreamException if the BinaryList cannot be created.
		*/
		virtual MDataType::BinaryCollection* GiveOutBinaryCollection()=0;

		/**
			Return the binary data containing the stream as a list of blocks.
			Other component can get the binary, then save it to file or send it over network.
			@par
			This call does not give up the ownership of the data, and in stead it copies the data.
			So the caller must release the Binary approriately.
			@par
			After this call, the caller can still write into this Stream whose contents are not disturbed,
			but the newly written data is not in the returned Binary.
			@return a pointer to the generated BinaryCollection.
			@throws WriteStreamException if the BinaryList cannot be created.
		*/
		virtual MDataType::BinaryCollection* CopyOutBinaryCollection()=0;

		/// get the size of the stream as an Int64 value
		virtual Int64 Get64BitSize() throw() = 0;

		/// get the size of the stream as an int (32) value.
		/// @throws WriteStreamException if the size is bigger than the range of 32bit integer.
		virtual unsigned Int32 GetSize() = 0;

		/// Rewind the write pointer to the beginning of the stream.
		/// Destroy the old data.
		/// @throws WriteStreamException if cannot create the new stream.
		virtual void Rewind() = 0;

		/**
			Copy the content of the input ReadDataStream into the end of this stream
			and move the write pointer to the end of the new content.
			@par
			This call does not erase the content in the input stream
			@param ipReadStream a const reference to the input stream
			@throws WriteStreamException if the WriteDataStream cannot be expanded.
		*/
		virtual void CopyAppend(const ReadDataStream& irReadStream) = 0;

		/// delete this object. Used by StrongPtr.
		virtual void Delete() throw() =0;

		// Mark the end of a stream.
		virtual void CommitStream() = 0;

		// Abandon stream : freeze further actions on this stream
		virtual void Abandon() = 0;

		virtual void SetTimeout(Int64 iTimeoutInMilliSeconds) = 0;

#ifndef NO_WCHAR_SUPPORT
		/** write Unicode string. It is different from opertor<< one. It only writes
		    4-byte length before the string. 
			@param ipString the pointer to the wchar_t string
			@throws WriteStreamException if writing fails.
		*/
		virtual void WriteString(const wchar_t * ipString)=0;
#endif

	protected:
		virtual ~WriteDataStream() throw () {};
	};



	typedef MBase::StrongPtr<WriteDataStream, MBase::DeleteOperatorGeneric<WriteDataStream> > WriteDataStreamPtr;

	/**
		Create a WriteDataStream using the specified stream header.
		@param ipMemCon a pointer to MemoryContract. Its default value is NULL.
		@param header the header of the data stream. Any stream version information should be passed by it.
		Its default value is 1.
		@return a pointer to the created WriteDataStream.
		@throws WriteStreamException.
	*/
	DLL_PDSERIALIZATION_EXIM WriteDataStream* CreateWriteDataStream(MBase::MemoryContract * ipMemCon=0,
																	unsigned short header = 1);

	/**
		create a fast but platform-dependent WriteDataStream, using the specified stream header.
		This stream won't convert the data type into platform-neutral format. So its usage must
		be restricted in the same machine.
		@param ipMemCon a pointer to MemoryContract. Its default value is NULL.
		@param header the header of the data stream. Any stream version information should be passed by it.
		Its default value is 1.
		@return a pointer to the created WriteDataStream.
		@throws WriteStreamException.
	*/
	DLL_PDSERIALIZATION_EXIM WriteDataStream* CreateRawWriteDataStream(MBase::MemoryContract * ipMemCon=0,
																	unsigned short header = 1);

	/**
		Create a WriteDataStream using the specified stream header.
		This stream will put extra information into the stream to make the stream more robust.
		@param ipMemCon a pointer to MemoryContract. Its default value is NULL.
		@param header the header of the data stream. Any stream version information should be passed by it.
		Its default value is 1.
		@return a pointer to the created WriteDataStream.
		@throws WriteStreamException.
	*/
	DLL_PDSERIALIZATION_EXIM WriteDataStream* CreateSafeWriteDataStream(MBase::MemoryContract * ipMemCon=0,
																		unsigned short header = 1);

	/**
		Create a WriteDataStream using the specified stream header.
		This stream will put extra information into the stream to make the stream more robust.

		This is a fast but platform-dependent WriteDataStream, using the specified stream header.
		This stream won't convert the data type into platform-neutral format. So its usage must
		be restricted in the same machine.

		@param ipMemCon a pointer to MemoryContract. Its default value is NULL.
		@param header the header of the data stream. Any stream version information should be passed by it.
		Its default value is 1.
		@return a pointer to the created WriteDataStream.
		@throws WriteStreamException.
	*/
	DLL_PDSERIALIZATION_EXIM WriteDataStream* CreateSafeRawWriteDataStream(MBase::MemoryContract * ipMemCon=0,
																		unsigned short header = 1);

	/**
		Create a WriteDataStream from a BinaryCollection.
		The stream header will be decided by the data in the BinaryCollection.
		If the stream header has safe stream flag, the stream will be serialized
		using safe format.
		The new WriteDataStream will take the ownership of the BinaryCollection.
		@param ipBins the pointer to the BinaryCollection. This BinaryCollection must
			contain header data. Otherwise, serialization exception will be thrown.
		@param ipMemCon a pointer to MemoryContract. Its default value is NULL.
		@return a pointer to the created WriteDataStream.
		@throws WriteStreamException.
	*/
	DLL_PDSERIALIZATION_EXIM WriteDataStream* ConvertBinaries2WriteDataStream(
														MDataType::BinaryCollection* ipBins,
														MBase::MemoryContract * ipMemCon=0);
	/**
		The following are overloaded methods for writing data to file instead of memory.
		Useful when serializing large data.
		@param iFilePtr Strong pointer to a Writeable file. The caller opens a Writeable file and
		gives up ownership of the file pointer to the data stream.  When all is done the data stream
		commits and closes the file.
	 */
	DLL_PDSERIALIZATION_EXIM WriteDataStream* CreateWriteDataStream(MFileSystem::WriteableFile::Ptr& iFilePtr,
									    							unsigned short header=1);
	DLL_PDSERIALIZATION_EXIM WriteDataStream* CreateRawWriteDataStream(MFileSystem::WriteableFile::Ptr& iFilePtr,
																	   unsigned short header=1);
	DLL_PDSERIALIZATION_EXIM WriteDataStream* CreateSafeWriteDataStream(MFileSystem::WriteableFile::Ptr& iFilePtr,
																		unsigned short header=1);
	DLL_PDSERIALIZATION_EXIM WriteDataStream* CreateSafeRawWriteDataStream(MFileSystem::WriteableFile::Ptr& iFilePtr,
																		   unsigned short header=1);
#if !defined(WIN32) && !defined(__ANDROID__)
	/**
		Create a write data stream which writes directly to the shared memory
	 */
	DLL_PDSERIALIZATION_EXIM WriteDataStream* CreateWriteDataStreamToSharedMem(MMultiProcess::SharedMemory* ipSharedMemPtr, unsigned Int32 iBlockSize = 1024);
#endif

	/**
		Create a write data stream which writes directly to the shared memory based byte stream
	 */
	DLL_PDSERIALIZATION_EXIM WriteDataStream* CreateWriteDataStreamToSharedMemByteStream(
		MSerialization::WriteByteStream* ipSharedMemWriteByteStreamPtr);
}

#endif // Serialization_WriteDataStream_H
