//==============================================================================================
//	FILENAME	:	NewOperator.h
//	AUTHOR		:	Juan Pablo Muraira
//	CREATION	:	9/26/01
//	Copyright (C) MicroStrategy Incorporated 2001
//	All Rights Reserved
//==============================================================================================
#ifndef MBase_NewOperator_h
#define MBase_NewOperator_h

#include "Base.h"	// export macros
#include "PDCHeader/PDCstddef.h"

namespace MBase 
{
	class Buffer;
}

#ifdef WIN32
	#define _BUFFER_NEW_OPERATOR_DEFINED_

	#ifdef _NEW_OPERATOR_UNDEFINED_
		#undef new
	#endif

	#ifdef _OBJECT_DUMP_NEW_OPERATOR_DEFINED_
		#undef new
	#endif // DEBUG
#endif
// redefine new and delete operators in the global namespace

///////////////////////////////////////////////////////////////////////////////////////////////
// IMPORTANT:	Compiler Error C2059 indicates that NewOperator.h was 
//				included before PlatformAPI_ObjectDump.h.
void DLL_BASE_EXIM * operator new(size_t size,MBase::Buffer* iBuffer);
void DLL_BASE_EXIM operator delete(void *iPtr,MBase::Buffer* iBuffer) throw();

//kxiao remove for VC8
//#ifndef WIN32
	void DLL_BASE_EXIM * operator new[](size_t size,MBase::Buffer* iBuffer);
	void DLL_BASE_EXIM operator delete[](void *iPtr,MBase::Buffer* iBuffer) throw();
//#endif


#endif // MBase_NewOperator_h
