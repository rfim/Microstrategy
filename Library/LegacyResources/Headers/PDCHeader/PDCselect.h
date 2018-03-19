//==============================================================================================
//	FILENAME	:	PDCselect.h
//	AUTHOR		:	Ananth Kini
//	CREATION	:	06/25/2010
//	Copyright (C) MicroStrategy, Inc. 2010
//==============================================================================================
#ifndef PDCselect_h
#define PDCselect_h

// this must be the *first* file included
#include "ProtectedSource/Prolog.h"
#ifdef WIN32
	//#if !defined(_WINSOCKAPI_) && !defined(PDCwinsock_h)
	//#include "PDCHeader/PDCwinsock.h"
	//#endif
#else
#include <sys/select.h>
#endif

#ifdef ANDROID
	typedef unsigned long int __fd_mask;
#endif

#ifdef linux

	//custom extended mstr fd_set size
	#define MSTR_FD_SETSIZE 65536

	/* fd_set for select and pselect.  */
	typedef struct
	  {
		/* XPG4.2 requires this member name.  Otherwise avoid the name
		   from the global namespace.  */
	#ifdef __USE_XOPEN
		__fd_mask fds_bits[MSTR_FD_SETSIZE / __NFDBITS];
	# define __FDS_BITS(set) ((set)->fds_bits)
	#else
		__fd_mask __fds_bits[MSTR_FD_SETSIZE / __NFDBITS];
	# define __FDS_BITS(set) ((set)->__fds_bits)
	#endif
	  } mstr_fd_set;


	//definition for custom select call (wrapper for system select call)
	inline int mstr_select(int numfds, mstr_fd_set* readfds, mstr_fd_set* writefds, mstr_fd_set* exceptfds, struct timeval *timeout){
		return ::select(numfds, (fd_set*)readfds, (fd_set*)writefds, (fd_set*)exceptfds, timeout);
	}

	//replace all references to fd_set from here on with MSTR_fd_set
	#define fd_set mstr_fd_set

#else
	//for simplicity on other platforms, mstr_select is a macro
	#define mstr_select( A , B , C , D , E ) ::select( A , B , C , D , E )
#endif //linux

#endif //PDCselect_h
