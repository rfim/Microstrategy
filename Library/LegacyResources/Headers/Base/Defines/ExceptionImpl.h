//==============================================================================================
//	FILENAME	:	ExceptionImpl.h
//	AUTHOR		:	Juan Pablo Muraira
//	CREATION	:	9/26/2001
//	Copyright (C) MicroStrategy Incorporated 2001
//	All Rights Reserved
//==============================================================================================
//
//   #####  ###### #####  #####  ######  #####  ####  ##### ###### #####
//   #    # #      #    # #    # #      #      #    #   #   #      #    #
//   #    # #####  #    # #    # #####  #      #    #   #   #####  #    #
//   #    # #      #####  #####  #      #      ######   #   #      #    #
//   #    # #      #      #   #  #      #      #    #   #   #      #    #
//   #####  ###### #      #    # ######  ##### #    #   #   ###### #####
//
#ifndef MBase_ExceptionImpl_h
#define MBase_ExceptionImpl_h

#include "PDCHeader/PDCstring"

#include "Asserte.h"
#include "../Base/LastError.h"
#include "../Base/UTFConverter.h"
#include "../Interfaces/BaseException.h"

#if defined(GetMessage)
#error Please make sure that you include <winbase.h>/<windows.h> properly (via Common/PDCHeader)
#endif

namespace MBase
{
	/**
	 * ExceptionImpl provides the basic implementation for your custom component
	 * exception.
	 * Notice that by using a template we avoid inheritance of implementation across components,
	 * and we still provide a way to re-use the common behavior.
	 *
	 * Declare your exception: ex...
	 *
	 *		class MyException: public MBase::BaseException
	 *
	 * Derive from MBase::Exception, so that all MSTR exception may be caught at the top
	 * and handled by a common interface.
	 * Provide only the interface for your exception, so that your
	 * clients are not poluted by the implementation of your exception.
	 * Derive your exception implementation from your exception, and put in the implementation details.
	 *
	 * If your exception doesn't have any significant behavior but storing a message, then you
	 * don't need to define the implementation, you may simply throw this template class (this
	 * is very common for exceptions that are only "place holders", where it is important to
	 * distinguish the type of the exception, but the exception doesn't have any specific
	 * behavior).
	 * In that case, there is no need to define your own exception, simply throw like this...
	 *
	 *		throw MBase::ExceptionImpl<MyException>(L"My message");
	 *
	 * This will create an exception that derives from your exception (and may be caught as such),
	 * and provides the basic functionality of storing an error message.
	 *
	 * If your exception has significant behavior, you can still re-use ExceptionImpl
	 * functionality by deriving from it.
	 * Define your exception implementation like this...
	 *
	 *		class MyExceptionImpl: public MBase::ExceptionImpl<MyException>
	 *		{
	 *			// MyException specific behavior goes here...
	 *		}
	 *
	 * and throw it like this...
	 *
	 *		throw MyExceptionImpl( <constructor params> );
	 */


// To compile ExecLibRubyBridge on HP-UX PA-RISC machine we have to clearly
// specify the parent class, not a template with default value. PA-RISC doesn't
// accept that. - hzhang 12/22/2005
	 template<class T = MBase::BaseException>
	 class ExceptionImpl:
#ifndef __hpux
		public T
#elif #cpu(parisc)
		public MBase::BaseException
#else
		public T
#endif
	{
	public:
#ifndef NO_WCHAR_SUPPORT
		explicit ExceptionImpl(const WCHAR* ipcMessage) throw()
		{
			//SetDomainAndID(MBase::MESSAGE_DOMAIN_UNKNOWN, -1);
			//SetMessage(ipcMessage != NULL ? ipcMessage : L"");
		}
#endif

		explicit ExceptionImpl(const char* ipcUTF8Message) throw()
		{
			//SetDomainAndID(MBase::MESSAGE_DOMAIN_UNKNOWN, -1);
			//SetMessage(MBase::UTF8ToWideChar(ipcUTF8Message != NULL ? ipcUTF8Message : "", true).c_str());
		}

		/**
		 * This constructor is used for "cascading" exceptions.
		 * Useful, when you need to catch an exception of one type,
		 * add more information, and throw again, possibly as another type.
		 * Ex:
		 *      try
		 *		{
		 *			// do X
		 *			...
		 *			// call some functions that may throw SomeException
		 *			...
		 *		}
		 *		catch( SomeException& e )
		 *		{
		 *			throw MBase::ExceptionImpl< MyException >( "Cannot do X.", e );
		 *		}
		 */
#ifndef NO_WCHAR_SUPPORT
		explicit ExceptionImpl(const WCHAR* ipcMessage, BaseException& irRelatedException) throw()
		{
			//SetDomainAndID(irRelatedException.GetDomain(), irRelatedException.GetID());
			//Concatenate( ipcMessage, irRelatedException.GetMessage() );
		}
#endif

		explicit ExceptionImpl(const char* ipcUTF8Message, BaseException& irRelatedException) throw()
		{
			//SetDomainAndID(irRelatedException.GetDomain(), irRelatedException.GetID());
			//Concatenate( MBase::UTF8ToWideChar( ipcUTF8Message, true ).c_str(), irRelatedException.GetMessage() );
		}

		/**
		 * This constructor is used for throwing an exception right after a system call, which failed.
		 * Ex:
		 *		// do X by calling some Operating System API functions
		 *		...
		 *		if( X failed )
		 *		{
		 *			throw MBase::ExceptionImpl< MyException >(
		 *				L"Cannot do X.", MBase::GetLastError() );
		 *		}
		 */
#ifndef NO_WCHAR_SUPPORT
		explicit ExceptionImpl(const WCHAR* ipcMessage, MBase::ErrorCode iErrorCode) throw()
		{
			//SetDomainAndID(MBase::MESSAGE_DOMAIN_UNKNOWN, iErrorCode);
			//Concatenate(ipcMessage, MBase::GetSystemErrorDescription(iErrorCode).c_str());
		}
#endif

		explicit ExceptionImpl(const char* ipUTF8Message, MBase::ErrorCode iErrorCode) throw()
		{
#ifndef __ANDROID__
			//SetDomainAndID(MBase::MESSAGE_DOMAIN_UNKNOWN, iErrorCode);
			Concatenate(
				MBase::UTF8ToWideChar( ipUTF8Message, true ).c_str(),
				MBase::GetSystemErrorDescription(iErrorCode).c_str());
#endif
		}

		/// @see MBase::BaseException::GetMessage
		const WCHAR* GetMessage() const throw()
		{
#ifdef __ANDROID__
			return "";
#else
			return Message::GetMessage();
#endif
		}

		/// @see MBase::BaseException::GetErrorCode
		MBase::ErrorCode GetErrorCode() const throw()
		{
			return T::GetErrorCode();
		}

		void SetErrorCode(MBase::ErrorCode iErrorCode) throw()
		{
			//SetDomainAndID(MBase::MESSAGE_DOMAIN_UNKNOWN, iErrorCode);
		}

	private:

		void Concatenate( const WCHAR* ipcMessage, const WCHAR* ipcRelatedMessage ) throw()
		{
			try
			{
#ifdef NO_WCHAR_SUPPORT
				_ASSERTE( std::string( ipcMessage ).length() > 0 );
				_ASSERTE( std::string( ipcRelatedMessage ).length() > 0 );

				std::string lMessage(ipcMessage);
#else
				_ASSERTE( std::wstring( ipcMessage ).length() > 0 );
				_ASSERTE( std::wstring( ipcRelatedMessage ).length() > 0 );

				std::wstring lMessage(ipcMessage);
#endif
				// 2002-05-10 vovechkin
				// The Pollux LogRouter handles multi-line messages correctly
				lMessage += _T("\n");
				lMessage += ipcRelatedMessage;

				//SetMessage(lMessage.c_str());
			}
			catch(...)
			{
				_ASSERT( false );
			}
		}
	};
}

#endif // MBase_ExceptionImpl_h

