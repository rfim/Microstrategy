//==============================================================================================
//	FILENAME	:	BinaryImpl.h
//	AUTHOR		:	Yuxiao Xiao
//	CREATION	:	8/24/01
//	Copyright (C) MicroStrategy, Inc. 2001
//==============================================================================================
#ifndef MDataType_BinaryImpl_h
#define MDataType_BinaryImpl_h

#include "../Binary.h"

namespace MDataType
{
	class BinaryImpl:
		public Binary
	{
	public:

		BinaryImpl(MBase::Buffer* ipBuffer = NULL);
		BinaryImpl(const void* ipData, size_t iSize, MBase::Buffer* ipBuffer = NULL);

		// a virtual destructor is a must
		virtual ~BinaryImpl() throw()
		{
		}

	protected:

		void SetBinary(const void* ipData, size_t iSize);

		bool operator == (const Binary& irBinary) const;
		bool operator < (const Binary& irBinary) const;

		size_t GetSize() const;
		const void* c_ptr() const;

		virtual void ZapTail(size_t iHowManyBytes);
		virtual void ZapHead(size_t iHowManyBytes);
		virtual void UnZap();

		char* GiveUpBinary();

	private:

		MBase::Buffer* const mpBuffer;

		MBase::StrongArrayPtr<char> mBinaryPtr;

		// "allocated" is the block of memory that was allocated
		size_t mAllocatedSize;
		void* mpAllocatedData;
		
		// "visible" is (part) of the "allocated" block, which is "seen" by c_ptr()/GetSize
		size_t mVisibleSize;
		void* mpVisibleData;
	};
}

#endif // MDataType_BinaryImpl_h
