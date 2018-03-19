//==============================================================================================
//	FILENAME	:	StreamInt64Reserve.h
//	AUTHOR		:	Yi Luo
//	PROJECT		:	PDSerilization
//
//	CREATION	:	2002-07-10
//	Copyright (C) MicroStrategy, Inc. 2002
//==============================================================================================
#ifndef Serialization_StreamInt64Reserve_H
#define Serialization_StreamInt64Reserve_H

#include "PDSerialization.h"
#include "Base/Defines/StrongPtr.h"

namespace MSerialization
{
	/**
		This is the interface for writing to a reserved 64 bit space in a stream.
		For usage, refer to WriteDataStream
	*/
	class DLL_PDSERIALIZATION_EXIM StreamInt64Reserve
	{
	public:
		/// Write a 64 bit integer into the reserved space.
		virtual void Write(Int64 data) = 0;

		/// delete this object. Used by StrongPtr<>.
		virtual void Delete() throw()=0;
	};

	typedef MBase::StrongPtr<StreamInt64Reserve, MBase::DeleteOperatorGeneric<StreamInt64Reserve> > StreamInt64ReservePtr;

	/**
		This is the interface for writing to a reserved 32 bit space in a stream.
		For usage, refer to WriteDataStream
	*/
	class DLL_PDSERIALIZATION_EXIM StreamInt32Reserve
	{
	public:
		/// Write a 32 bit integer into the reserved space.
		virtual void Write(Int32 data) = 0;

		/// delete this object. Used by StrongPtr<>.
		virtual void Delete() throw()=0;
	};

	typedef MBase::StrongPtr<StreamInt32Reserve, MBase::DeleteOperatorGeneric<StreamInt32Reserve> > StreamInt32ReservePtr;
}

#endif // Serialization_StreamInt64Reserve_H

