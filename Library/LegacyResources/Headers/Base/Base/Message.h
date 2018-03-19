//==============================================================================================
//	FILENAME	:	Message.h
//	AUTHOR		:	vovechkin
//	CREATION	:	2004-10-26
//	Copyright (C) MicroStrategy Incorporated 2004
//	All Rights Reserved
//==============================================================================================
#ifndef MBase_Message_h
#define MBase_Message_h

#include "Base.h"

#include "../Defines/BasicTypes.h"
#include "../Defines/Asserte.h"
#include "ReturnString.h"
#include "Language.h"
#include "MessageDomain.h"

namespace MBase
{
	typedef Int32 MessageID;

	/**
	 * MBase::Message can be used as a regular variable, return value, or a C++ exception object.
	 *
	 * The major features are:
	 *	- chaining (along two axes)
	 *	- inplace construction (as a single object or an array)
	 *	- localization support
	 *	- conservative memory usage
	 */
	class DLL_BASE_EXIM Message
	{
	public:

		typedef unsigned Int32 SizeType;

		/**
		 * Is the minimal size of a memory block, in which a Message can exist.
		 *
		 * @see MBase::Message::PlacementPtr
		 * @see MBase::Message::PlacementArrayPtr
		 */
		static const SizeType MIN_SIZE;

		///////////////////////////////////////////////////////////////////////
		// 
		// Construction, assignment, destruction
		//
		///////////////////////////////////////////////////////////////////////

		Message();
		Message(const Message& irOther);
		Message(MessageDomain iDomain, MessageID iID, const char* ipTemplate = NULL);

		// (deep) assigment operator
		Message& operator=(const Message& irOther);

		/**
		 * Swaps two message objects.
		 *
		 * @return false if memory allocation was required, and failed.
		 */
		bool Swap(Message& irOther) throw();

		// destructor
		~Message() throw();

		///////////////////////////////////////////////////////////////////////
		// 
		// Chaining
		//
		///////////////////////////////////////////////////////////////////////

		/**
		 * To chain the current message, so that it becomes the lower-level message of the parameter.
		 * Note: when the message is blank (IsEmpty returns true), WrapBy behaves like a constructor.
		 */
		Message& WrapBy(MessageDomain iDomain, MessageID iID, const char* ipTemplate = NULL);

		/**
		 * Returns a pointer to a Message instance that represents a
		 * previous exception of a lower level or NULL.
		 */
		const Message* GetLowerLevelMessageOrNULL() const;

		/**
		 * To chain the current message, so that it becomes the same-level message of the parameter.
		 * Note: when the message is blank (IsEmpty returns true), PrependBy behaves like a constructor.
		 */
		Message& PrependBy(MessageDomain iDomain, MessageID iID, const char* ipTemplate = NULL);

		/**
		 * To chain the current message, so that it becomes the same-level message of the parameter.
		 * Note: when the message is blank (IsEmpty returns true), PrependBy behaves like a constructor.
		 */
		Message& PrependBy(const Message& irMessage);

		/**
		 * Returns a pointer to a Message instance that represents a
		 * previous exception of the same level or NULL.
		 */
		const Message* GetSameLevelMessageOrNULL() const;

		///////////////////////////////////////////////////////////////////////
		// 
		// Placement construction/destruction
		//
		///////////////////////////////////////////////////////////////////////

		/**
		 * This pointer class constructs/destructs an instance of the Message class
		 * in a pre-allocated chunk of memory.
		 *
		 * Note: this pointer class does NOT take ownership of the ipBuffer memory!
		 */
		class DLL_BASE_EXIM PlacementPtr
		{
		public:

			PlacementPtr(
				void* ipBuffer,
				SizeType iBufferSizeInBytes);

			PlacementPtr(
				void* ipBuffer,
				SizeType iBufferSizeInBytes,
				const Message& irOther);

			PlacementPtr(
				void* ipBuffer,
				SizeType iBufferSizeInBytes,
				MessageDomain iDomain,
				MessageID iID,
				const char* ipTemplate = NULL);

			~PlacementPtr() throw();

			inline Message& operator*() const throw()
			{
				_ASSERT(mpMessage != NULL);
				return *mpMessage;
			}

			inline Message* operator->() const throw()
			{
				_ASSERT(mpMessage != NULL);
				return mpMessage;
			}

			inline Message* Get() const throw()
			{
				_ASSERT(mpMessage != NULL);
				return mpMessage;
			}

			inline bool IsNull() const
			{
				return (mpMessage == NULL);
			}

		private:

			// do not allow copying
			PlacementPtr(const PlacementPtr&);
			PlacementPtr& operator=(const PlacementPtr&);

		private:

			Message* mpMessage;
		};

		/**
		 * This pointer class constructs/destructs an array of instances of the Message class
		 * in a pre-allocated chunk of memory.
		 *
		 * Note: this pointer class does NOT take ownership of the ipBuffer memory!
		 */
		class DLL_BASE_EXIM PlacementArrayPtr
		{
		public:

			PlacementArrayPtr(
				void* ipBuffer,
				SizeType iBufferSizeInBytes,
				SizeType iMessageSizeInBytes);
			
			PlacementArrayPtr(
				void* ipBuffer,
				SizeType iBufferSizeInBytes,
				SizeType iMessageSizeInBytes, 
				const Message& irOther);

			PlacementArrayPtr(
				void* ipBuffer,
				SizeType iBufferSizeInBytes,
				SizeType iMessageSizeInBytes,
				MessageDomain iDomain,
				MessageID iID,
				const char* ipTemplate = NULL);

			~PlacementArrayPtr() throw();

			inline MBase::Message& operator [](unsigned int iIndex)
			{
				_ASSERT(mpMessage != NULL);
				_ASSERT(iIndex < (mBufferSizeInBytes / mMessageSizeInBytes));

				return *reinterpret_cast<MBase::Message*>(mpMessage + mMessageSizeInBytes * iIndex);
			}

			inline bool IsNull() const
			{
				return (mpMessage == NULL);
			}

		private:

			// do not allow copying
			PlacementArrayPtr(const PlacementArrayPtr&);
			PlacementArrayPtr& operator=(const PlacementArrayPtr&);

		private:

			unsigned char* mpMessage;
			const SizeType mBufferSizeInBytes;
			const SizeType mMessageSizeInBytes;
		};

		///////////////////////////////////////////////////////////////////////
		// 
		// Value getters/setters
		//
		///////////////////////////////////////////////////////////////////////

		/**
		 * Returns true if message contains some fields.
		 */
		bool IsEmpty() const;

		/**
		 * Erases all information, makes the message empty.
		 */
		void Clear();

		/**
		 * Returns the resource bundle name that corresponds to the message domain.
		 */
		MBase::MessageDomain GetDomain() const;

		/**
		 * Returns the message ID.
		 */
		MBase::MessageID GetID() const;

		/**
		 * Sets the message ID and domain.
		 */
		void SetDomainAndID(MBase::MessageDomain iDomain, MessageID iID);

		/**
		 * Returns the default message template.
		 */
		const char* GetTemplate() const;
		void SetTemplate(const char* ipTemplate);

		/**
		 * Used to iterate over a list of parameters.
		 * If parameter is not set, returns NULL.
		 */
		const char* GetParameter(unsigned int iIndex) const;

		/**
		 * Parameter formatting
		 */
		Message& operator << (bool iParameter);
		Message& operator << (int iParameter);
		Message& operator << (unsigned int iParameter);
		Message& operator << (long iParameter);
		Message& operator << (unsigned long iParameter);
		Message& operator << (const char* ipParameter); // assumed to be UTF-8 (or ASCII)
#ifndef NO_WCHAR_SUPPORT
		Message& operator << (const WCHAR* ipParameter);
#endif

		typedef void (*Manipulator)(Message& irMessage);
		Message& operator << (Manipulator ipManipulator);

		/**
		 * Format manipulators. Example:
		 *
		 * lMessage << MBase::Message::Hex << hr;
		 */
		static void Dec(Message& irMessage);
		static void Hex(Message& irMessage); // ex: 0x1
		static void WideHex(Message& irMessage); // ex: 0x00000001
		static void WideWideHex(Message& irMessage); // ex: 0x0000000000000001

		///////////////////////////////////////////////////////////////////////
		// 
		// Localization
		//
		///////////////////////////////////////////////////////////////////////

		/**
		 * Returns the message text for a particular language, or NULL if the text
		 * is not available.
		 */
		const char* GetLocalizedText(MBase::LanguageID iLanguageID) const;

		/**
		 * Stores the message text for a particular language. If ipText is NULL, 
		 * the stored text is erased.
		 */
		void SetLocalizedText(MBase::LanguageID iLanguageID, const char* ipText);

		/**
		 * The callback interface for getting all the data at once.
		 */
		class Localizer
		{
		public:

			virtual MBase::ReturnString Localize(
				MBase::LanguageID iLanguageID,
				MBase::MessageDomain iMessageDomain,
				MBase::MessageID iID,
				const char* ipTemplate,
				const char* ipParameters[]/*,
				ParameterType ipParameterTypes[]*/) = 0;
		};

		/**
		 * Localize a single message. If the message already contains localized text for iLanguageID,
		 * the irLocalizer will not be called, and the stored text will be returned.
		 */
//		const char* GetLocalizedText(Localizer& irLocalizer, MBase::LanguageID iLanguageID);

		/**
		 * Localize a single message. If the message already contains localized text for iLanguageID,
		 * the irLocalizer will not be called, and the stored text will be returned.
		 */
		MBase::ReturnString Localize(Localizer& irLocalizer, MBase::LanguageID iLanguageID) const;

		///////////////////////////////////////////////////////////////////////
		// 
		// Private declarations
		//
		///////////////////////////////////////////////////////////////////////

	private:

		// placement constructors
		friend class PlacementPtr;
		friend class PlacementArrayPtr;

		Message(SizeType iBufferSizeInBytes);
		Message(SizeType iBufferSizeInBytes, const Message& irOther);
		Message(SizeType iBufferSizeInBytes, MessageDomain iDomain, MessageID iID, const char* ipTemplate);

		/**
		 * @param iRadix can be 10 or 16
		 * @param iWidth the number width (padded with zeroes on the left)
		 */
		void SetNumberFormat(unsigned char iRadix, unsigned char iWidth);
		void GetNumberFormat(unsigned char& orRadix, unsigned char& orWidth) const;

	private:

		// the only data member, which determines the default Message size (as local/temp variable)
		unsigned char mBuffer[100];

	public:

		///////////////////////////////////////////////////////////////////////
		// 
		// Compatibility with MBase::BaseException
		//
		///////////////////////////////////////////////////////////////////////

#ifndef NO_BASE_EXCEPTION_COMPATIBILITY
		// for compatibility with MBase::Exception
		const WCHAR* GetMessage() const throw();
		void SetMessage(const WCHAR* ipMessage) throw();

		/// @see MBase::BaseException::GetErrorCode
		inline unsigned Int32 GetErrorCode() const throw()
		{
			return static_cast<unsigned Int32>(GetID());
		}
#endif // NO_BASE_EXCEPTION_COMPATIBILITY
	};
}

#endif // MBase_Message_h
