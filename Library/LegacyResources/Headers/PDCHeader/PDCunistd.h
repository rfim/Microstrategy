//==============================================================================================
//	FILENAME	:	PDCunistd.h
//	AUTHOR		:	Perl Script
//	CREATION	:	6/13/02
//	Copyright (C) MicroStrategy, Inc. 2002
//==============================================================================================
#ifndef PDCunistd_h
#define PDCunistd_h

// this must be the *first* file included
#include "ProtectedSource/Prolog.h"
#ifdef __ANDROID__
#include "PDCselect.h"
#else
#include "PDCHeader/PDCselect.h"
#endif

#ifdef WIN32
	#define STDIN_FILENO    0
	#define STDOUT_FILENO   1
	#define STDERR_FILENO   2
#else
	#include <unistd.h>
#endif

#ifdef __hpux
// On HP-UX, seems that sys/time.h must be included before time.h to
// make select work.
#include<sys/time.h>
#endif

#ifndef WIN32
#include "PDCerrno.h"

#ifdef _AIX
#include <sys/select.h>
#else
#include "PDCtime.h"
#endif

namespace MPDCHeader
{
	// a signal-safe wrapper
	inline ssize_t read(int fildes, void *buf, size_t nbyte)
	{
		ssize_t lBytesRead = 0;

		for (;;)
		{
			const ssize_t lResult = ::read(fildes, buf, nbyte);

			if (lResult == -1)
			{
				if (errno == EINTR)
				{
					// interrupted by signal, retry
					continue;
				}

				return (lBytesRead > 0) ? lBytesRead : -1;
			}
			else if (lResult == 0)
			{
				// end-of-file
				return lBytesRead;
			}
			else if (lResult < nbyte)
			{
				// may be interrupted by signal, read more
				lBytesRead += lResult;
				buf = reinterpret_cast<char*>(buf) + lResult;
				nbyte -= lResult;
				continue;
			}

			// success
			return lBytesRead + lResult;
		}
	}

	// a signal-safe wrapper
	inline ssize_t write(int fildes, const void *buf, size_t nbyte)
	{
		ssize_t lBytesWritten = 0;

		for (;;)
		{
			const ssize_t lResult = ::write(fildes, buf, nbyte);

			if (lResult == -1)
			{
				if (errno == EINTR)
				{
					// interrupted by signal, retry
					continue;
				}

				// error
				return (lBytesWritten > 0) ? lBytesWritten : -1;
			}
			else if (lResult < nbyte)
			{
				// may be interrupted by signal, write more
				lBytesWritten += lResult;
				buf = reinterpret_cast<const char*>(buf) + lResult;
				nbyte -= lResult;
				continue;
			}

			// success
			return lBytesWritten + lResult;
		}
	}

	// a signal-safe wrapper
	inline int fsync(int fildes)
	{
		for (;;)
		{
			const ssize_t lResult = ::fsync(fildes);

			if (lResult == -1)
			{
				if (errno == EINTR)
				{
					// interrupted by signal, retry
					continue;
				}

				// error
				return -1;
			}

			return lResult;
		}
	}

	// a signal-safe wrapper
	inline int ftruncate(int fildes, off_t length)
	{
		for (;;)
		{
			const ssize_t lResult = ::ftruncate(fildes, length);

			if (lResult == -1)
			{
				if (errno == EINTR)
				{
					// interrupted by signal, retry
					continue;
				}

				// error
				return -1;
			}

			return lResult;
		}
	}

	// a signal-safe wrapper
	inline int close(int fildes)
	{
		const int RETRY = 10;
		int retry = 0;
		for (;;)
		{
			const ssize_t lResult = ::close(fildes);

			if (lResult == -1)
			{
				if (errno == EINTR)
				{
					retry++;
					if (retry==RETRY)
						break;
					// interrupted by signal, retry
					continue;
				}

				// error
				return -1;
			}

			return lResult;
		}

		return -1;
	}

	// a signal-safe wrapper
	inline int select(int nfds, fd_set* readfds, fd_set* writefds, fd_set* errorfds, struct timeval* timeout)
	{
		for (;;)
		{
			const int lResult = mstr_select(nfds, readfds, writefds, errorfds, timeout);

			if (lResult == -1)
			{
				if (errno == EINTR)
				{
					// interrupted by signal, retry
					continue;
				}

				// error
				return -1;
			}

			return lResult;
		}
	}

	// a signal-safe wrapper
	inline int fchown(int fildes, uid_t owner, gid_t group)
	{
		for (;;)
		{
			const int lResult = ::fchown(fildes, owner, group);

			if (lResult == -1)
			{
				if (errno == EINTR)
				{
					// interrupted by signal, retry
					continue;
				}

				// error
				return -1;
			}

			return lResult;
		}
	}

	// a signal-safe wrapper
	inline int dup(int fildes)
	{
		for (;;)
		{
			const ssize_t lResult = ::dup(fildes);

			if (lResult == -1)
			{
				if (errno == EINTR)
				{
					// interrupted by signal, retry
					continue;
				}

				// error
				return -1;
			}

			return lResult;
		}
	}
	// a signal-safe wrapper
	inline int dup2(int fildes, int fildes2)
	{
		for (;;)
		{
			const ssize_t lResult = ::dup2(fildes, fildes2);

			if (lResult == -1)
			{
				if (errno == EINTR)
				{
					// interrupted by signal, retry
					continue;
				}

				// error
				return -1;
			}

			return lResult;
		}
	}
}

// provide closesocket() like on Windows
inline int closesocket(int fildes)
{
	return MPDCHeader::close(fildes);
}

#endif // WIN32

// this must be the *last* file included
#include "ProtectedSource/Epilog.h"

#endif // PDCunistd_h
