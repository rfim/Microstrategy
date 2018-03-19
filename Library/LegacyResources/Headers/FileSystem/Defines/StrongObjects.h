//==============================================================================================
//	FILENAME	:	StrongObjects.h
//	AUTHOR		:	vovechkin
//	CREATION	:	2002-08-28
//	Copyright (C) MicroStrategy Incorporated 2002
//	All Rights Reserved
//==============================================================================================
#ifndef MFileSystem_StrongObjects_h
#define MFileSystem_StrongObjects_h

#include <string>
#include <stdio.h>
#include <errno.h>

#include <unistd.h> // for ::close

#include "Base/Defines/StrongObj.h"
#include "../FileSystem/File.h"
#include "../FileSystem/Path.h"

namespace MFileSystem
{
	class CloseFileStream
	{
	public:
		void operator()( FILE* iFileStream ) throw()
		{
			if( iFileStream != NULL )
			{
				const int lResult = ::fclose(iFileStream);
				
				if (lResult != 0)
				{
					// 2004-01-02 vovechkin
					//
					// WHY:
					// If closing the file failed, it could be because 
					// the file was not explicitly flushed prior to closing, 
					// and an implicit flush encountered an error.
					//
					// HOW TO FIX:
					// If you get this assertion, make sure that your 
					// last file operation is MFileSystem::File::Flush(),
					// which will throw an exception in case of an error.
					//
					// 2004-08-18 vovechkin
					// Removing assertions because they fail in the 
					// course of stack unwinding when an exception is
					// thrown. Until we have a file object, we cannot
					// know at this point whether we should assert or not.
//					const int lErrorCode = errno;
//					_ASSERTE("::fclose() failed" && false);
				}
			}
		}
	};

	class CloseFileDescriptor
	{
	public:
		void operator()( int iFileDescriptor ) throw()
		{
			// valid descriptors are >= 0
			if (iFileDescriptor >= 0)
			{
#ifdef WIN32
				const int lResult = ::_close(iFileDescriptor);
#else
				const int lResult = ::close(iFileDescriptor);
#endif

				if (lResult != 0)
				{
					// 2004-01-02 vovechkin
					//
					// WHY:
					// If closing the file failed, it could be because 
					// the file was not explicitly flushed prior to closing, 
					// and an implicit flush encountered an error.
					//
					// HOW TO FIX:
					// If you get this assertion, make sure that your 
					// last file operation is MFileSystem::File::Flush(),
					// which will throw an exception in case of an error.
					//
					// 2004-08-18 vovechkin
					// Removing assertions because they fail in the 
					// course of stack unwinding when an exception is
					// thrown. Until we have a file object, we cannot
					// know at this point whether we should assert or not.
//					const int lErrorCode = errno;
//					_ASSERTE("::close() failed" && false);
				}
			}
		}
	};

	typedef MBase::StrongObj< FILE*, CloseFileStream > StrongFileStream;

	/**
	 * This class is an exception-safe holder of a file descriptor.
	 */
	class StrongFileDescriptor:
		protected MBase::StrongObj<int, CloseFileDescriptor>
	{
	public:

		StrongFileDescriptor():
			MBase::StrongObj<int, CloseFileDescriptor>(-1)
		{
		}

		StrongFileDescriptor(int iDescriptor):
			MBase::StrongObj<int, CloseFileDescriptor>(iDescriptor)
		{
		}

		int Get() const throw()
		{
			return static_cast<const MBase::StrongObj<int, CloseFileDescriptor>*>(this)->Get();
		}

		bool IsNull()
		{
			return (Get() < 0);
		}

		void Acquire(StrongFileDescriptor& iStrongObj) throw()
		{
			int iData = iStrongObj.GiveUp();
			Acquire(iData);
		}

		void Acquire(int& irData) throw()
		{
			static_cast<MBase::StrongObj<int, CloseFileDescriptor>*>(this)->Acquire(irData);
		}

		void Reset() throw()
		{
			static_cast<MBase::StrongObj<int, CloseFileDescriptor>*>(this)->Reset(-1);
		}
	private:

		int GiveUp() throw()
		{
			int lTemp = mData;
			mData = -1;
			return lTemp;
		}
	};

	/**
	 * This class holds a filename. The corresponding file will be 
	 * automatically deleted unless Dismiss has been called.
	 */
	class StrongFilename
	{
	public:
		explicit StrongFilename( const char* ipPath ):
			mPath( ipPath )
		{
		}
		
		StrongFilename( StrongFilename& iorOther )
		{
			mPath.swap( iorOther.mPath );
		}

		~StrongFilename() throw()
		{
			Reset();
		}
		
		StrongFilename& operator=( StrongFilename& iorOther )
		{
			mPath.swap( iorOther.mPath );
			return *this;
		}

		const char* c_str() const
		{
			return mPath.c_str();
		}

		void Dismiss() throw()
		{
			mPath.erase();
		}

		void Attach( const char* ipPath ) throw()
		{
			Reset();
			mPath = ipPath;
		}

	private:
		std::string mPath;

		void Reset() throw()
		{
			if( mPath.length() > 0 )
			{
				bool lDeleted = File::Delete( mPath.c_str() );

				// don't assert if there is no file
//				_ASSERT(lDeleted || !Path::IsFile(mPath.c_str()));

				mPath.erase();
			}
		}
	};
}

#endif // MFileSystem_StrongObjects_h
