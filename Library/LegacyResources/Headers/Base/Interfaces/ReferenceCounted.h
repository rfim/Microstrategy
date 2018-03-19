//==============================================================================================
//	FILENAME	:	ReferenceCounted.h
//	AUTHOR		:	Juan Pablo Muraira
//	CREATION	:	9/26/01
//	Copyright (C) MicroStrategy Incorporated 2001
//	All Rights Reserved
//==============================================================================================
#ifndef MBase_ReferenceCounted_h
#define MBase_ReferenceCounted_h

#ifdef _MSC_VER
#pragma warning(disable:4275) // non - DLL-interface classkey 'identifier' used as base for DLL-interface classkey 'identifier'
#endif

namespace MBase 
{
	/*
	 * Derive your class from MBase::ReferenceCounted to get the standard interface
	 * of reference counting. You may get the reference counted implementation
	 * by deriving your implementation class from MSynch::ReferenceCountedImpl<> 
	 * class. 
	 *
	 * Example:
	 *
	 *	// interface class
	 *	class My:
	 *		virtual public MBase::ReferenceCounted
	 *	{
	 *		...
	 *	}
	 *
	 *	// implementation class
	 *	class MyImpl:
	 *		public My,
	 *		virtual public MSynch::ReferenceCountedImpl<>
	 *
	 *	{
	 *		...
	 *	}
	 *
	 * If MyImpl is a single-threaded class, you may save a link dependency to Synch
	 * by using a simple integer instead of an MSynch::AtomicLong (default template 
	 * parameter of MSynch::ReferenceCountedImpl)
	 *
	 * Example:
	 *
	 *	// implementation class (single-threaded!)
	 *	class MyImpl:
	 *		public My,
	 *		virtual public MSynch::ReferenceCountedImpl<int>
	 *
	 *	{
	 *		...
	 *	}
	 *
	 * @see MSynch::ReferenceCountedImpl
	 */
	class ReferenceCounted
	{
	protected:

		virtual ~ReferenceCounted() throw()
		{
		};

	public:
		virtual void AddRef() const throw()=0;
		virtual void Release() const throw()=0;
	};
}

#endif // MBase_ReferenceCounted_h
