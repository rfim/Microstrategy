//==============================================================================================
//	FILENAME	:	Binary.h
//	AUTHOR		:	Yuxiao Xiao
//	CREATION	:	8/24/01
//	Copyright (C) MicroStrategy, Inc. 2001
//==============================================================================================
#ifndef MDataType_Binary_h
#define MDataType_Binary_h

#include "DataType.h"
#include "Base/Defines/StrongPtr.h"
#include "Base/Defines/ReturnPtr.h"
#include "PDCtypes.h"

namespace MBase
{
	class Buffer;
}

namespace MDataType
{
	/// MDataType::Binary represents binary data.
	/**
	 *	Use this class to hold and do simple operations on raw binary data.
	 *	Note: There is no GiveUp method to give up the ownership of the raw data. It is important that
	 *	such a method NEVER be added to this class. Since this class allocates the buffer, it needs
	 *	to also delete the buffer. If ownership was given up, the user would have no way of deleting
	 *	the buffer when done with it, because there would be no way to know how to delete it. If you
	 *	find you need to give up the data, then you need to consider implementing the object that needs
	 *	the ownership on top of MDataType::Binary. For an example of such a need see CDSSMsg, which was changed
	 *	to be implemented on top of MDataType::Binary.
	 */
	class Binary
	{
	public:
		
		// a virtual destructor is a must
		virtual ~Binary() throw()
		{
		}

		typedef MBase::StrongPtr<Binary> Ptr;
		typedef MBase::ReturnPtr<Binary> ReturnPtr;

		/// SetBinary allocates a new buffer.
		/**
		 *	This method allocates a new buffer of the specified size and copies data from the passed in
		 *	pointer into the newly allocated space
		 *	@param ipData Pointer to data to use as initialization for the newly created buffer. If NULL
		 *		then the newly created buffer is initialized with zeros.
		 *	@param iSize Size in bytes of the buffer to allocate.
		 */
		virtual void SetBinary(const void* ipData, size_t iSize) = 0;


		/// Operator == does an efficient deep comparison. Returns true if equal.
		virtual bool operator == (const Binary& irBinary) const = 0;

		/// Operator < does an efficient deep comparison. Returns true if first parameter is less
		/// than the second parameter
		virtual bool operator < (const Binary& irBinary) const = 0;

		/// GetSize returns the number of bytes of the internal buffer.
		virtual size_t GetSize() const = 0;

		/// c_ptr returns a weak reference to the internal buffer. Do not try to delete the pointer.
		virtual const void* c_ptr() const = 0;

		/**
		 * This method makes the last iHowManyBytes bytes "invisible".
		 * "Invisible" data is not taken into account by GetSize/c_ptr methods.
		 */
		virtual void ZapTail(size_t iHowManyBytes) = 0;

		/**
		 * This method makes the first iHowManyBytes bytes "invisible".
		 * "Invisible" data is not taken into account by GetSize/c_ptr methods.
		 */
		virtual void ZapHead(size_t iHowManyBytes) = 0;

		/**
		 * This method makes all data made "invisible" by ZapHead/ZapTail visible again.
		 */
		virtual void UnZap() = 0;

		/**
		 * If there is no buffer (mpBuffer is NULL), give up ownership and return the binary data, clean the rest data member.
		 * If there is buffer, return NULL.
		 */
		virtual char* GiveUpBinary() = 0;
	};

	// this typedef is obsolete, use Binary::Ptr instead
	typedef MBase::StrongPtr<Binary> BinaryPtr;

	/// CreateBinary instantiates a new MDataType::Binary object
	/**
	 *	This factory method creates a MDataType::Binary object that is empty. There is no allocated buffer.
	 *	To allocate a buffer use the SetBinary method.
	 *	@param ipBuffer Pointer to an optional MBase::Buffer. If NULL, then no MBase::Buffer is used.
	 *	@see #SetBinary
	 */
	DLL_DATATYPE_EXIM Binary* CreateBinary(MBase::Buffer* ipBuffer = NULL);

	/// CreateBinary instantiates a new MDataType::Binary object
	/**
	 *	This factory method creates a MDataType::Binary object of the specified size and copies data
	 *	from the passed pointer into the newly allocated space.
	 *	@param ipData Pointer to data to use as initialization for the newly created buffer.
	 *	@param iSize Size in bytes of the buffer to allocate.
	 *	@param ipBuffer Pointer to an optional MBase::Buffer. If NULL, then no MBase::Buffer is used.
	 */
	DLL_DATATYPE_EXIM Binary* CreateBinary(const void* ipData, size_t iSize, MBase::Buffer* ipBuffer = NULL);

	/// CreateBinary instantiates a new MDataType::Binary object
	/**
	 *	This factory method creates a MDataType::Binary object of the specified size and initialized
	 *	to zeros. In order to fill the object, use the c_ptr method.
	 *	@param iSize Size in bytes of the buffer to allocate.
	 *	@param ipBuffer Pointer to an optional MBase::Buffer. If NULL, then no MBase::Buffer is used.
	 *	@see #c_ptr
	 */
	DLL_DATATYPE_EXIM Binary* CreateBinary(size_t iSize, MBase::Buffer* ipBuffer = NULL);
}

#endif // MDataType_Binary_h
