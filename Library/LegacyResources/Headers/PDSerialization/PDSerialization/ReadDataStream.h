//==============================================================================================
//	FILENAME	:	PDSerializatoin/PDSerialization/ReadDataStream.h
//  PROJECT		:	PDSerialization
//
//	AUTHOR		:	Yi Luo
//	CREATION	:	2002-07-10
//	Copyright (C) MicroStrategy, Inc. 2002
//==============================================================================================
#ifndef Serialization_ReadDataStream_H
#define Serialization_ReadDataStream_H

#include "PDSerialization.h"
#include "DataType/DataType/Binary.h"
#include "DataType/DataType/Variant.h"
#include "Base/Base/GUID.h"
#include "Base/Base/ReturnString.h"
#include "Base/Interfaces/BaseMemoryContract.h"
#include "Base/Defines/SmartPtrI.h"
#include "FileSystem/FileSystem/ReadableFile.h"
#include "Synch/Defines/SmartPtr.h"
#if !defined(WIN32) && !defined(__ANDROID__)
	#include "MultiProcess/SharedMemory/SharedMemory.h"
#endif
#include "PDSerialization/PDSerialization/ReadByteStream.h"

namespace MDataType
{
	class BinaryCollection;
	class DynamicTime;
}

namespace MSerialization
{

	class WriteDataStream;	// foward class declaration

	/**
		Interface for reading typed data from a stream.

		we use opeator >> to read one single element, and use Read() to read an array.
		When reading array, the caller must allocate enough memory to hold all the elements.
		Otherwise, ACCESS VIOLATION may occur.
		@par
		When reading array, the caller must provide the number of elements in the array.
		This number must equal to the number of elements in the array which is written into the stream.
		A different number will cause the function to throw ReadStreamException.
		@par
		For example, if an array of 5 integer is written into a stream, these 5 integers must be read
		out through a Read(ipArray, 5) call. Any other way to read these 5 integers will fail.
	*/
	class DLL_PDSERIALIZATION_EXIM ReadDataStream
	{
	public:

		/**
			Read binary data.
			@param orValue a reference of a Binary
			@return reference to the ReadDataStream itself.
			@throws ReadStreamException if stream is corrupted.
		*/
		virtual ReadDataStream& operator >> (MDataType::Binary& orValue)=0;

		/**
			Read bool value.
			@param orValue a reference of a bool
			@return reference to the ReadDataStream itself.
			@throws ReadStreamException if stream is corrupted.
		*/
		virtual ReadDataStream& operator >> (bool& orValue)=0;

		/**
			Read into bool array.
			@param iopValueArray a pointer to a bool array
			@param iElementCount the number of elements in the array.
					It must equals to the number of elements writen into the array.
			@throws ReadStreamException if stream is corrupted.
		*/
		virtual void Read(bool * iopValueArray, unsigned Int32 iElementCount)=0;

		/**
			Read into byte (unsigned char) array.
			@param iopValueArray a pointer to byte (unsigned char) array
			@param iElementCount the number of elements in the array.
					It must equals to the number of elements writen into the array.
			@throws ReadStreamException if stream is corrupted.
		*/
		virtual void Read(unsigned char * iopValueArray, unsigned Int32 iElementCount)=0;

		/**
			Read value of short.
			@param orValue a reference of a short.
			@return reference to the ReadDataStream itself.
			@throws ReadStreamException if stream is corrupted.
		*/
		virtual ReadDataStream& operator >> (short & orValue)=0;

		/**
			Read value of unsigned short .
			@param orValue a reference of an unsigned short.
			@return reference to the ReadDataStream itself.
			@throws ReadStreamException if stream is corrupted.
		*/
		virtual ReadDataStream& operator >> (unsigned short & orValue)=0;

		/**
			Read into short array.
			@param iopValueArray a pointer to a short array
			@param iElementCount the number of elements in the array.
					It must equals to the number of elements writen into the array.
			@throws ReadStreamException if stream is corrupted.
		*/
		virtual void Read(short * iopValueArray, unsigned Int32 iElementCount)=0;

		/**
			Read value of 32bit integer.
			@param orValue a reference of a 32 bit integer.
			@return reference to the ReadDataStream itself.
			@throws ReadStreamException if stream is corrupted.
		*/
		virtual ReadDataStream& operator >> (Int32& orValue)=0;

		/**
			Read value of 32bit unsigned integer.
			@param orValue a reference of a 32 bit unsigned integer.
			@return reference to the ReadDataStream itself.
			@throws ReadStreamException if stream is corrupted.
		*/
		virtual ReadDataStream& operator >> (unsigned Int32 & orValue)=0;

		/**
			Read into 32bit integer array.
			@param iopValueArray a pointer to a 32bit integer array
			@param iElementCount the number of elements in the array.
					It must equals to the number of elements writen into the array.
			@throws ReadStreamException if stream is corrupted.
		*/
		virtual void Read(Int32* iopValueArray, unsigned Int32 iElementCount)=0;

		/**
			Read value of 64bit integer.
			@param orValue a reference of a 64 bit integer.
			@return reference to the ReadDataStream itself.
			@throws ReadStreamException if stream is corrupted.
		*/
		virtual ReadDataStream& operator >> (Int64& orValue)=0;

		/**
			Read into 64bit integer array.
			@param iopValueArray a pointer to a 64bit integer array
			@param iElementCount the number of elements in the array.
					It must equals to the number of elements writen into the array.
			@throws ReadStreamException if stream is corrupted.
		*/
		virtual void Read(Int64* iopValueArray, unsigned Int32 iElementCount)=0;

		/**
			Read value of float.
			@param orValue a reference of a float.
			@return reference to the ReadDataStream itself.
			@throws ReadStreamException if stream is corrupted.
		*/
		virtual ReadDataStream& operator >> (float& orValue)=0;

		/**
			Read into float array.
			@param iopValueArray a pointer to a float array
			@param iElementCount the number of elements in the array.
					It must equals to the number of elements writen into the array.
			@throws ReadStreamException if stream is corrupted.
		*/
		virtual void Read(float* iopValueArray, unsigned Int32 iElementCount)=0;

		/**
			Read value of double.
			@param orValue a reference of a double.
			@return reference to the ReadDataStream itself.
			@throws ReadStreamException if stream is corrupted.
		*/
		virtual ReadDataStream& operator >> (double& orValue)=0;

		/**
			Read into double array.
			@param iopValueArray a pointer to a double array
			@param iElementCount the number of elements in the array.
					It must equals to the number of elements writen into the array.
			@throws ReadStreamException if stream is corrupted.
		*/
		virtual void Read(double* iopValueArray, unsigned Int32 iElementCount)=0;

#ifndef NO_WCHAR_SUPPORT
		/**
			Read a wchar string.
			@return a wstring.
			@throws ReadStreamException if stream is corrupted.
		*/
		virtual MBase::ReturnWString ReadWString()=0;
#endif
		/**
			Read a char string.
			@return a string.
			@throws ReadStreamException if stream is corrupted.
		*/
		virtual MBase::ReturnString ReadString()=0;

		/**
			Read a byte
			@return the byte.
			@throws ReadStreamException if stream is corrupted.
		*/
		virtual char ReadByte()=0;

#ifndef NO_WCHAR_SUPPORT
		/**
			Read a wchar character
			@param orValue the read char
			@return reference to the ReadDataStream itself.
			@throws ReadStreamException if stream is corrupted.
		*/
		virtual wchar_t ReadWChar()=0;
#endif

		// serialization of complext data type

		/**
			Read value of GUID.
			@param orValue a reference of a GUID.
			@return reference to the ReadDataStream itself.
			@throws ReadStreamException if stream is corrupted.
		*/
		virtual ReadDataStream& operator >> (GUID& orValue)=0;

		/**
			Read into GUID array.
			@param iopValueArray a pointer to a GUID array
			@param iElementCount the number of elements in the array.
					It must equals to the number of elements writen into the array.
			@throws ReadStreamException if stream is corrupted.
		*/
		virtual void Read(GUID * iopValueArray, unsigned Int32 iElementCount )=0;

		/**
			Read value of DateTime.
			@param orValue a reference of a DateTime.
			@return reference to the ReadDataStream itself.
			@throws ReadStreamException if stream is corrupted.
		*/
		virtual ReadDataStream& operator >> (MDataType::DateTime& orValue)=0;

		/**
			Read into DateTime array.
			@param iopValueArray a pointer to a DateTime array
			@param iElementCount the number of elements in the array.
					It must equals to the number of elements writen into the array.
			@throws ReadStreamException if stream is corrupted.
		*/
		virtual void Read(MDataType::DateTime * iopValueArray, unsigned Int32 iElementCount)=0;

		/**
			Read value of DynamicTime.
			@param orValue a reference of a DynamicTime.
			@return reference to the ReadDataStream itself.
			@throws ReadStreamException if stream is corrupted.
		*/
		virtual ReadDataStream& operator >> (MDataType::DynamicTime& orValue)=0;

		/**
			Read into DynamicTime array.
			@param iopValueArray a pointer to a DynamicTime array
			@param iElementCount the number of elements in the array.
					It must equals to the number of elements writen into the array.
			@throws ReadStreamException if stream is corrupted.
		*/
		virtual void Read(MDataType::DynamicTime * iopValueArray, unsigned Int32 iElementCount)=0;


		/**
			Read value of CellFmtData.
			@param orValue a reference of a CellFmtData.
			@return reference to the ReadDataStream itself.
			@throws ReadStreamException if stream is corrupted.
		*/
		virtual ReadDataStream& operator >> (MDataType::CellFmtData& orValue)=0;

		/**
			Read into CellFmtData array.
			@param iopValueArray a pointer to a CellFmtData array
			@param iElementCount the number of elements in the array.
					It must equals to the number of elements writen into the array.
			@throws ReadStreamException if stream is corrupted.
		*/
		virtual void Read(MDataType::CellFmtData * iopValueArray, unsigned Int32 iElementCount)=0;


		/**
			Read value of Variant.
			@param orValue a reference of a Variant.
			@return reference to the ReadDataStream itself.
			@throws ReadStreamException if stream is corrupted.
		*/
		virtual ReadDataStream& operator >> (MDataType::Variant& orValue)=0;

		/** Read Serializable object.
			Actually call the Deserialize() function of the object
			@param irValue the object to read to
			@throws WriteStreamException if writing fails.
		*/
		virtual ReadDataStream& operator >> (SerializableObject& irValue)=0;

		/**
			Read into Variant array.
			when reading Variant array, the caller must allocate the Variant array,
			If the Variants must use a Buffer, each element of the Array must be
			be initialized with the Buffer prior to call Read().
			@param iopValueArray a pointer to a Variant array
			@param iElementCount the number of elements in the array.
					It must equals to the number of elements writen into the array.
			@throws ReadStreamException if stream is corrupted.
		*/
		virtual void Read(MDataType::Variant * iopValueArray, unsigned Int32 iElementCount)=0;

		/**
			special function for enhanced robustness. The original value and its
			complementary value are expected to exist in the stream, so the reader
			can compare the consecutive data to ensure the data is good.
			@return the integer value stored in the stream
			@throws ReadStreamException if stream is corrupted.
		*/
		virtual Int32 ReadSafeInt()=0;

		/// rewind the read pointer to the beginning of the stream.
		virtual void Rewind() =0 ;

		/**
			get the physical size of the stream, including all the extra information for
			any internal purpose such as error checking, bookkeeping and padding,
			@return the size of the stream as unsigned int.
			@throws ReadStreamException if the size of stream is bigger than 4G.
		*/
		virtual unsigned Int32 GetSize() = 0;

		/**
			get the physical size of the stream, including all the extra information for
			any internal purpose such as error checking, bookkeeping and padding,
			@return the size of the stream as 64bit integer.
		*/
		virtual Int64 Get64BitSize() throw() = 0;

		/// get the number of bytes unread in the stream
		virtual Int64 GetNumberOfUnreadBytes() throw() = 0;

		/**
			Return the binary data containing the stream as a list of blocks.
			Other component can get the binary, then save it to file or send it over network.
			@par
			The ReadDataStream gives up the ownership of the data. So the caller must release the Binary
			approriately. After this call, no more reading from this ReadDataStream is possible.
			@par
			After this call, this Stream contains no more data..
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
			After this call, the caller can still read from this Stream whose contents are not disturbed.
			@return a pointer to the generated BinaryCollection.
			@throws WriteStreamException if the BinaryList cannot be created.
		*/
		virtual MDataType::BinaryCollection* CopyOutBinaryCollection() const =0;

		/**
			Return the binary data containing the content of stream as a list of blocks.
			The header of the Stream is not in the binary data.
			Other component can get the binary, then save it to file or send it over network.
			@par
			This call does not give up the ownership of the data, and in stead it copies the data.
			So the caller must release the Binary approriately.
			@par
			After this call, the caller can still read from this Stream whose contents are not disturbed.
			@return a pointer to the generated BinaryCollection.
			@throws WriteStreamException if the BinaryList cannot be created.
		*/
		virtual MDataType::BinaryCollection* CopyOutContentBinaryCollection() const =0;

		/// delete this stream. This is the only way a stream can be deleted.
		virtual void Delete() throw() =0;

		// return the type of next item
		virtual TYPE GetItemType() =0;

		// return whether the stream is encrypted
		virtual bool IsEncrypted() = 0;

		// Wait for data in read streams
		virtual bool WaitForData(unsigned Int32 iTimeOut) = 0;

		// set a timeout value for all subsequent read actions.
		virtual void SetTimeout(Int64 iTimeoutInMilliseconds) = 0;

	protected:

		/// skip the spcified bytes and start the next reading from the new location.
		virtual void Skip(Int64 dwOffset)=0;

		/// skip the spcified bytes and start the next reading from the new location.
		virtual void Skip(unsigned Int32 dwOffset)=0;

		virtual ~ReadDataStream(){};
	};

	/// Strong pointer to the ReadDataStream
	typedef MBase::StrongPtr<ReadDataStream, MBase::DeleteOperatorGeneric<ReadDataStream> > ReadDataStreamPtr;

	// factory function to create a ReadDataStream from a Binary.

	/**
		The factory function to create ReadDataStream from a Binary.
		The returned ReadDataStream TAKE OVER the ownership of the Binary, so if the caller want to retain
		the Binary for other purpose beyond the lifetime of the ReadDataStream, it must duplicate
		the Binary. This also means the caller should not try to delete the Binary once it is given
		to the ReadDataStream.
		@par
		If necessary, the caller can provide a MemoryContract to guard the memory allocation during
		the stream operation.
		@param ipB The pointer to the Binary.
		@param ipMemCon the pointer to the memeory contractor. Its default value is NULL.
		@return the pointer to the created ReadDataStream.
	*/
	DLL_PDSERIALIZATION_EXIM ReadDataStream* ConvertBinary2ReadDataStream(MDataType::Binary* ipB,
																		  MBase::MemoryContract * ipMemCon = 0);

	/**
		The factory function to create ReadDataStream from a WriteDataStream.
		The returned ReadDataStream TAKE OVER the ownership of the WriteDataStream.
		This also means the caller should not try to delete the WriteDataStream once it is given
		to the ReadDataStream.
		@par
		If necessary, the caller can provide a MemoryContract to guard the memory allocation during
		the stream operation.
		@param ipWriteStream The pointer to the WriteDataStream.
		@param ipMemCon the pointer to the memeory contractor. Its default value is NULL.
		@return the pointer to the created ReadDataStream.
	*/
	DLL_PDSERIALIZATION_EXIM ReadDataStream* ConvertWriteDataStream2ReadDataStream(WriteDataStream * ipWriteStream,
																				   MBase::MemoryContract * ipMemCon = 0);


	/**
		The following method is used for reading serialized data from a file.
 		@param iFilePtr Strong pointer to a Readable file that contains a data stream.
 		The caller opens a Readable file and gives up ownership of the file pointer to the
 		data stream.  When all is done the data stream commits and closes the file.
	 */
	DLL_PDSERIALIZATION_EXIM ReadDataStream* GetReadDataStreamFromFile(MFileSystem::ReadableFile::Ptr& iFilePtr);

#if !defined(WIN32) && !defined(__ANDROID__)
	/**
		The following method is used for reading serialized data from shared memory directly.
 		@param ipSharedMemPtr raw pointer to shared memory instance.
	 */
	DLL_PDSERIALIZATION_EXIM ReadDataStream* CreateReadDataStreamFromSharedMem(MMultiProcess::SharedMemory* ipSharedMemPtr,
																				MBase::MemoryContract * ipMemCon);
#endif
		/**
		The following method is used for reading serialized data from shared memory based byte stream directly.
 		@param ipSharedMemReadBStreamPtr raw pointer to shared memory based byte stream instance.
	 */
	DLL_PDSERIALIZATION_EXIM ReadDataStream* CreateReadDataStreamFromSharedMemByteStream(
						ReadByteStream * ipSharedMemReadBStreamPtr,
						MBase::MemoryContract * ipMemCon);
}

#endif  // Serialization_ReadDataStream_H
