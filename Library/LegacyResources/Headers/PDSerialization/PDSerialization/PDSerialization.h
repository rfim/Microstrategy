//==============================================================================================
//	FILENAME	:	PDSerilization.h
//	AUTHOR		:	Yi Luo
//	CREATION	:	2002-07-10
//	Copyright (C) MicroStrategy, Inc. 2002
//==============================================================================================
#ifndef Serialization_PDSerialization_H
#define Serialization_PDSerialization_H

#ifdef WIN32

	#ifdef PDSERIALIZATION_DLL_EXPORT
		#define	DLL_PDSERIALIZATION_EXIM	_declspec(dllexport)
	#else
		#define	DLL_PDSERIALIZATION_EXIM	_declspec(dllimport)
	#endif

#pragma warning( disable: 4244)

#else
	#define	DLL_PDSERIALIZATION_EXIM
#endif

// Endianess is determined by CPU architecture
#if defined(WIN32) || defined(__i386) || defined(__x86_64) || defined(_MAC) || defined(__ANDROID__) // The last is for Android
	#ifndef LITTLE_ENDIAN
	#define LITTLE_ENDIAN
	#endif
#elif defined(__sparc) || defined(_POWER) || defined(__hpux)
//	#define BIG_ENDIAN
	#undef LITTLE_ENDIAN
#else
#error Unknown Platform
#endif


#include "Base/Interfaces/BaseException.h"

namespace MSerialization
{

	class ReadDataStream;
	class WriteDataStream;

	/// the root class for all exceptions in Serialization namespace
	class SerializationException:public MBase::BaseException {};

	/// the exception during read stream operations
	class ReadStreamException : public SerializationException 
	{
	public:
		ReadStreamException();
	};

	/// the exception during write stream operations
	class WriteStreamException : public SerializationException
	{
	public:
		WriteStreamException();	
	};


	class SerializableObject
	{
	public:
		virtual void Serialize(WriteDataStream&) const = 0;
		virtual void DeSerialize(ReadDataStream&) = 0;

		virtual ~SerializableObject() throw(){}
	};

	/**
	 * PolymorphicSerializableObject should be used instead of SerializableObject
	 * when the class will be used polymorphically. Derive the base class from
	 * PolymorphicSerializableObject, provide a factory function that takes one
	 * Int32 as a parameter, and then use POLYMORPHIC_SERIALIZATION_CONSTRUCTOR_FACTORY
	 * to set up the insertion (<<) and extraction (>>) operators
	 */
	class PolymorphicSerializableObject :
		virtual public SerializableObject
	{
		public:
			virtual Int32 GetType() const = 0;

			virtual ~PolymorphicSerializableObject() throw(){}
	};
	enum TYPE
	{
		T_INVALID	=0,
		T_UTF8		=1,
		T_DOUBLE	=2,
		T_BOOL		=3,
		T_SHORT		=4,
		T_USHORT	=5,
		T_INT		=6,
		T_UINT		=7,
		T_INT64		=8,
		T_GUID		=9,
		T_DATETIME	=10,
		T_VARIANT	=11,
		T_SAFEINT	=12,
		T_BINARY	=13,
		T_BLOCK		=14,
		T_LIST		=15,
		T_UTF8_CHAR	=16,
		T_DYNAMIC_TIME =17,
		T_BYTE		=18,
		T_RAWBINARY	=19,
		T_FLOAT		=20,
		T_CHAR		=21,
		T_OBJECT	=22,
		T_CELLFMTDATA = 23,


		T_ARRAY		=0x80,
		T_SMALLARRAY=0x40
	};
}


#define PDS_SAFE_STREAM			0x8000
#define PDS_RAW_STREAM			0x4000
#define PDS_BIG_ENDIAN_STREAM   0x2000
#define PDS_SIMPLE_ENCRYPT_STREAM		0x0100
#define PDS_RC4_ENCRYPT_STREAM			0x0200
#define PDS_TEA_ENCRYPT_STREAM			0x0400
#define PDS_AES_ENCRYPT_STREAM			0x0800

#endif  // Serialization_PDSerialization_H
