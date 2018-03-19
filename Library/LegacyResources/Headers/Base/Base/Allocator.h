//==============================================================================================
//	FILENAME	:	Allocator.h
//	AUTHOR		:	Juan Pablo Muraira
//	CREATION	:	9/26/01
//	Copyright (C) MicroStrategy Incorporated 2001
//	All Rights Reserved
//==============================================================================================
#ifndef	MBase_Allocator_h
#define	MBase_Allocator_h

// the allocator here is a standard allocator, not an RW alternative allocator interface, need to define _RWSTD_ALLOCATOR for sun
#if defined(sun) && !defined(_RWSTD_ALLOCATOR)
// if you get this error make sure that _RWSTD_ALLOCATOR is defined AT THE BEGINNING OF YOUR PROJECT
// it should be defined automatically for you in the Makefile, because it is on the local.param rules for Solaris
#error _RWSTD_ALLOCATOR not defined
_RWSTD_ALLOCATOR not defined
#endif


#ifdef _AIX
#include <new>
#endif

#include "PDCHeader/PDClimits.h"
#include "PDCHeader/PDCcstdlib"
#include "PDCHeader/PDCutility"

#include "Buffer.h"

/////////////////////////////////////////////////////////////////
// IMPORTANT:
// Because allocator is a template and therefore expanded when the code is 
// instantiated, it may not become part of a constellation interface.
// This means that it may not be referenced as parameter, return value, etc
// on a constellation interface. It may not be a part of another template
// on the interface either (such as MsiString)
// Templates may be used in another constellation if and only if they do not
// "escape" that constellation.
////////////////////////////////////////////////////////////////
// HISTORY:
// This file was originally copied/pasted from Windows' <xmemory>
// for reasons unknown to us. After that some of the ancient macros were
// removed, such as _FARQ. The ultimate goal is to converge back to using
// std::allocator.
////////////////////////////////////////////////////////////////

namespace MBase 
{
	/**
	 * Allocator allows stl collections to use our Buffer. When creating a collection
	 * that must be in the buffer, create an Allocator and initializing it with the 
	 * desired Buffer. Then create the collection initializing with that Allocator
	 * instance.
	 * The interface of Allocator is as defined by std::allocator.
	 */
	template<class _Ty>
	class Allocator
	{
	public:
		typedef size_t		size_type;
		typedef ptrdiff_t	difference_type;
		typedef _Ty*		pointer;
		typedef const _Ty*	const_pointer;
		typedef _Ty&		reference;
		typedef const _Ty&	const_reference;
		typedef _Ty			value_type;

		Allocator():
			mMemPtr(NULL)
		{
			// Default Constructor.  We initialize the Buffer
			// pointer to NULL.  This means allocation will
			// now be done from the process heap
		}

		template<class _UT>
		Allocator(const Allocator<_UT>& irAllocator):
				mMemPtr(irAllocator.GetBuffer())
		{
		}

		Allocator(Buffer* iMan):
			mMemPtr(iMan)
		{
			// Initialize the buffer pointer.  This means allocation
			// is now always done from the buffer
		}

		template<class _UT>
		struct rebind
		{
				typedef Allocator<_UT> other;
		};

		pointer address(reference irX) const throw()
		{
			return (&irX); 
		}

		const_pointer address(const_reference irX) const throw()
		{
			return (&irX); 
		}

		pointer allocate(size_type inSize, const void *)
		{
			if (!mMemPtr)
			{
				// buffer pointer is NULL, do allocation
				// from process heap
				return reinterpret_cast<pointer>(malloc(sizeof(value_type) *inSize));
			}

			// else get memory from the buffer
			return reinterpret_cast<pointer>(mMemPtr->GetMem(inSize * sizeof(value_type)));
		}

		pointer allocate(size_type inSize)
		{
			return allocate(inSize, NULL);
		}

	#ifdef WIN32
		char*_Charalloc(size_type inSize)
		{
			if (!mMemPtr)
			{ 
				// buffer pointer is NULL, do allocation
				// from process heap
				return reinterpret_cast<char*>(new char[inSize]);
			}

			// else get memory from the buffer.
			return reinterpret_cast<char*>(mMemPtr->GetMem(inSize)); 
		}

		void deallocate(void* ipPtr, size_type) throw()
		{
			// If allocation is being done from process heap, the
			// deallocate
			if (!mMemPtr)
			{
			   delete[] ipPtr;
			} 
		}
	#endif

		void deallocate(pointer ipPtr, size_type)
		{
			// If allocation is being done from process heap, the
			// deallocate
			if (!mMemPtr)
			{
				free(ipPtr);
			}
		}


		void construct(pointer ipPtr, const_reference _V)
		{
			if (!ipPtr) return;

#ifdef WIN32
			std::_Construct(ipPtr, _V);
#else
			new ((void *)ipPtr) (_Ty)(_V);
#endif
		}

		void destroy(pointer ipPtr) throw()
		{
			// If allocation is being done from process heap, then
			// destroy
			if (!mMemPtr)
			{
#ifdef WIN32
				std::_Destroy(ipPtr);
#else
				ipPtr->~_Ty(); 
#endif
			}
		}


		size_type max_size() const throw()
		{
			size_type lnSize = static_cast<size_type>(UINT_MAX / sizeof(value_type));
			return (0 < lnSize ? lnSize : 1); 
		}

		// this function is to be used internally by allocator only
		// needs to be public to be accessed by another intantiation of the
		// template
		Buffer* GetBuffer() const
		{
			return mMemPtr;
		}

	private:
		Buffer* mMemPtr;
	};

	template<class _Ty, class _UT> inline
	bool operator==(const Allocator<_Ty>& left, const Allocator<_UT>& right)
	{	
		//nprabhudessai , 02/16/06, it was always returning true which was wrong
		return (left.GetBuffer() == right.GetBuffer());
	}

	template<class _Ty, class _UT> inline
	bool operator!=(const Allocator<_Ty>& left, const Allocator<_UT>& right)
	{
		//nprabhudessai , 02/16/06, it was always returning false which was wrong
		return !(left == right);
	}


// the following is only for Unix
#ifndef WIN32
		// CLASS allocator<void>
	template<> class Allocator<void> 
	{
	public:
		typedef void _Ty;
		typedef _Ty  *pointer;
		typedef const _Ty  *const_pointer;
		typedef _Ty value_type;

		template<class _UT>
		struct rebind 
		{
			typedef Allocator<_UT> other;
		};

		Allocator()
		{
		}

		Allocator(const Allocator<_Ty>& /*irAlloc*/)
		{
		}

		template<class _UT>
		Allocator(const Allocator<_UT>& /*irAlloc*/)
		{
		}

		template<class _UT>
		Allocator<_Ty>& operator=(const Allocator<_UT>& /*irAlloc*/)
		{
			return (*this);
		}
	};
#endif // WIN32
}

#endif // MBase_Allocator_h
