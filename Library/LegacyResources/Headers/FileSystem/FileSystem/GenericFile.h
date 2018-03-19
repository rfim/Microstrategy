//==============================================================================================
//	FILENAME	:	GenericFile.h
//	AUTHOR		:	vovechkin
//	CREATION	:	2004-09-29
//	Copyright (C) MicroStrategy Incorporated 2004
//	All Rights Reserved
//==============================================================================================
#ifndef MFileSystem_GenericFile_h
#define MFileSystem_GenericFile_h

#include "Base/Defines/BasicTypes.h"
#include "Base/Defines/StrongPtr.h"
#include "Base/Defines/ReturnPtr.h"

namespace MBase
{
	class Message;
}

namespace MFileSystem
{
	class GenericFile
	{
	public:

		typedef MBase::StrongPtr<GenericFile> Ptr;
		typedef MBase::ReturnPtr<GenericFile> ReturnPtr;

		// virtual destructor is a must
		virtual ~GenericFile() throw()
		{
		}

		/**
		 * Returns the filename.
		 */
		virtual const char* GetPath() const = 0;

		/**
		 * Returns whether the file is still accessible via its path.
		 * Note: file must be open when this method is invoked.
		 */
		virtual bool IsAccessibleViaPath() const = 0;

		/**
		 * Returns size of the file.
		 */
		virtual Int64 GetSize() const = 0;

		/**
		 * Returns the curent file position.
		 */
		virtual Int64 GetPosition() const = 0;

		/**
		 * Moves the file pointer relative to the current position.
		 */
		virtual void MovePosition(Int64 iOffset) = 0;

		/**
		 * Moves the file pointer to the absolute position.
		 *
		 * @param iOffset must be positive.
		 */
		virtual void SetPosition(Int64 iOffset) = 0;

		/**
		 * Moves the file pointer to the end of the file.
		 *
		 * @return the new position value (relative to the beginning of the file).
		 */
		virtual Int64 SetPositionToEnd() = 0;
	};
}

#endif // MFileSystem_GenericFile_h
