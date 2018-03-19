//==============================================================================================
//	FILENAME	:	PDCstring.h
//	AUTHOR		:	Perl Script
//	CREATION	:	6/13/02
//	Copyright (C) MicroStrategy, Inc. 2002
//==============================================================================================
#ifndef PDCstring_h
#define PDCstring_h

// this must be the *first* file included
#include "ProtectedSource/Prolog.h"

#include <string.h>
#include "PDCerrno.h"

#if !defined(WIN32) && defined(WIN64)
/**
 * Security enhanced version of basic string-related functions for non-Windows platform.
 * @author Qiang Xue
 * @version $Revision: $  $Date: $
 */

/**
 * Security enhanced version of memcpy.
 * 
 * Copies count characters of src to dest. The count parameter is in bytes.
 * If the source and destination overlap, the behavior of memcpy_s is undefined. 
 * Use memmove_s to handle overlapping regions.
 *
 * If dest or src is a null pointer, or sizeInBytes is too small for the buffer, 
 * it raises assertion error if _DEBUG is defined.
 * If execution is allowed to continue, it returns EINVAL and sets errno to EINVAL.
 *
 * @param void* new buffer
 * @param size_t size of the destination buffer in bytes
 * @param const void* buffer to copy from
 * @param size_t number of characters to copy
 * @return errno_t EINVAL if parameter validation fails, 0 otherwise.
 */
inline errno_t memcpy_s(void *dest, size_t sizeInBytes, const void *src, size_t count)
{
	_VALIDATE_RETURN_ERRCODE(dest!=NULL,EINVAL);
	_VALIDATE_RETURN_ERRCODE(src!=NULL,EINVAL);
	_VALIDATE_RETURN_ERRCODE(count>=0 && sizeInBytes>=count,EINVAL);
	::memcpy(dest,src,count);
	return 0;
}

/**
 * Security enhanced version of memmove.
 * 
 * Copies count bytes of characters from src to dest. If some regions of the source
 * area and the destination overlap, memmove_s ensures that the original source bytes
 * in the overlapping region are copied before being overwritten.
 *
 * If dest or src is a null pointer, or sizeInBytes is too small for the buffer, 
 * it raises assertion error if _DEBUG is defined.
 * If execution is allowed to continue, it returns EINVAL and sets errno to EINVAL.
 *
 * @param void* new buffer
 * @param size_t size of the destination buffer in bytes
 * @param const void* buffer to copy from
 * @param size_t number of characters to copy
 * @return errno_t EINVAL if parameter validation fails, 0 otherwise.
 */
inline errno_t memmove_s(void *dest, size_t sizeInBytes, const void* src, size_t count)
{
	_VALIDATE_RETURN_ERRCODE(dest!=NULL,EINVAL);
	_VALIDATE_RETURN_ERRCODE(src!=NULL,EINVAL);
	_VALIDATE_RETURN_ERRCODE(count>=0 && sizeInBytes>=count,EINVAL);
	::memmove(dest,src,count);
	return 0;
}

/**
 * Security enhanced version of strcat.
 * 
 * Appends strSource to strDestination and terminates the resulting string with a null
 * character. The initial character of strSource overwrites the terminating null character
 * of strDestination. The behavior of strcat_s is undefined if the source and destination
 * strings overlap.
 *
 * Note that the second parameter is the total size of the buffer, not the remaining size.
 *
 * If strDestination is a null pointer, or is not null-terminated, or if strSource is 
 * a NULL pointer, or if the destination string is too small,  
 * it raises assertion error if _DEBUG is defined.
 * If execution is allowed to continue, it returns EINVAL and sets errno to EINVAL.
 *
 * @param char* Null-terminated destination string buffer
 * @param size_t Size of the destination string buffer in bytes
 * @param const char* Null-terminated source string buffer
 * @return errno_t EINVAL if parameter validation fails, 0 otherwise.
 */
inline errno_t strcat_s(char *strDestination, size_t sizeInBytes, const char *strSource)
{
	_VALIDATE_RETURN_ERRCODE(strDestination!=NULL,EINVAL);
	_VALIDATE_RETURN_ERRCODE(strSource!=NULL,EINVAL);
	_VALIDATE_RETURN_ERRCODE(sizeInBytes>strlen(strSource)+strlen(strDestination),EINVAL);
	::strcat(strDestination,strSource);
	return 0;
}

/**
 * Security enhanced version of strcpy.
 * 
 * Copies the contents in the address of strSource, including the terminating null character,
 * to the location specified by strDestination. The destination string must be large enough to
 * hold the source string, including the terminating null character. The behavior of strcpy_s
 * is undefined if the source and destination strings overlap.
 *
 * If strDestination or strSource is a null pointer, or if the destination string is too small,
 * it raises assertion error if _DEBUG is defined.
 * If execution is allowed to continue, it returns EINVAL and sets errno to EINVAL.
 *
 * @param char* Null-terminated destination string buffer
 * @param size_t Size of the destination string buffer in bytes
 * @param const char* Null-terminated source string buffer
 * @return errno_t EINVAL if parameter validation fails, 0 otherwise.
 */
inline errno_t strcpy_s(char *strDestination, size_t sizeInBytes, const char *strSource)
{
	_VALIDATE_RETURN_ERRCODE(strDestination!=NULL,EINVAL);
	_VALIDATE_RETURN_ERRCODE(strSource!=NULL,EINVAL);
	_VALIDATE_RETURN_ERRCODE(sizeInBytes>strlen(strSource),EINVAL);
	::strcpy(strDestination,strSource);
	return 0;
}

/**
 * Security enhanced version of strncat.
 * 
 * Copies up to count characters, including the terminating null character, from the string 
 * pointed to by strSource into the buffer pointed to by strDestination.
 * Once strncpy_s() has copied count characters to strDestination, it does not append a 
 * terminating null character. 
 *
 * Note that the second parameter is the total size of the destination buffer, not the remaining size.
 *
 * If strDestination or strSource is a null pointer, or if the destination string is too small,
 * it raises assertion error if _DEBUG is defined.
 * If execution is allowed to continue, it returns EINVAL and sets errno to EINVAL.
 *
 * @param char* Null-terminated destination string
 * @param size_t Size of the destination string buffer in bytes
 * @param const char* Null-terminated source string
 * @param size_t Number of characters to append
 * @return errno_t EINVAL if parameter validation fails, 0 otherwise.
 */
inline errno_t strncat_s(char *strDestination, size_t sizeInBytes, const char *strSource, size_t count)
{
	_VALIDATE_RETURN_ERRCODE(strDestination!=NULL,EINVAL);
	_VALIDATE_RETURN_ERRCODE(strSource!=NULL,EINVAL);
	_VALIDATE_RETURN_ERRCODE(sizeInBytes>strlen(strSource)+(count>strlen(strSource)?strlen(strSource):count),EINVAL);
	::strncat(strDestination,strSource,count);
	return 0;
}

/**
 * Security enhanced version of strncpy.
 * 
 * Copies the initial count characters of strSource to strDestination and returns strDestination.
 * NOTE, Unlike strncpy, if count is less than or equal to the length of strSource, a null character 
 * is appended automatically to the copied string, and if count is greater than the length of strSource,
 * the destination string is NOT padded with null characters up to length count. 
 * The behavior of strncpy_s is undefined if the source and destination strings overlap.
 *
 * If strDestination or strSource is a null pointer, or if the destination string is too small,
 * it raises assertion error if _DEBUG is defined.
 * If execution is allowed to continue, it returns EINVAL and sets errno to EINVAL.
 *
 * @param char* Null-terminated destination string buffer
 * @param size_t Size of the destination string buffer in bytes
 * @param const char* Null-terminated source string buffer
 * @param size_t Number of characters to copy
 * @return errno_t EINVAL if parameter validation fails, 0 otherwise.
 */
inline errno_t strncpy_s(char *strDestination, size_t sizeInBytes, const char *strSource, size_t count)
{
	_VALIDATE_RETURN_ERRCODE(strDestination!=NULL,EINVAL);
	_VALIDATE_RETURN_ERRCODE(strSource!=NULL,EINVAL);
	size_t len=(count>strlen(strSource)?strlen(strSource):count);
	_VALIDATE_RETURN_ERRCODE(sizeInBytes>len,EINVAL);
	::strncpy(strDestination,strSource,len);
	strDestination[len]=NULL;
	return 0;
}

#endif

// 2005-04-19 vovechkin
// HP-UX does not have the ::strsignal() function.
#if defined(__hpux)
#include <signal.h>

inline const char* strsignal(int n)
{
	switch (n)
	{
		// according to POSIX, all systems must define these signals:
		case SIGABRT: return "Process abort.";
		case SIGALRM: return "Alarm clock.";
		case SIGBUS:  return "Bus error.";
		case SIGCHLD: return "Child status changed.";
		case SIGCONT: return "Continued.";
		case SIGFPE:  return "Erroneous arithmetic operation.";
		case SIGHUP:  return "Hangup.";
		case SIGILL:  return "Illegal instruction.";
		case SIGINT:  return "Interrupt.";
		case SIGKILL: return "Kill.";
		case SIGPIPE: return "Broken pipe.";
		case SIGQUIT: return "Quit.";
		case SIGSEGV: return "Segmentation fault.";
		case SIGSTOP: return "Stopped.";
		case SIGTERM: return "Terminated.";
		case SIGTSTP: return "Stopped from terminal.";
		case SIGTTIN: return "Background process attempting to read from terminal.";
		case SIGTTOU: return "Background process attempting to write to terminal.";
		case SIGUSR1: return "User-defined signal 1.";
		case SIGUSR2: return "User-defined signal 2.";
		case SIGPOLL: return "Pollable event.";
//		case SIGPROF: return "Profiling timer expired.";
		case SIGSYS:  return "Bad system call.";
		case SIGTRAP: return "Trace or breakpoint trap.";
		case SIGURG:  return "High bandwidth data is available at a socket.";
		case SIGVTALRM: return "Virtual timer expired.";
		case SIGXCPU: return "CPU time limit exceeded.";
		case SIGXFSZ: return "File size limit exceeded.";

		// these are optional
		#ifdef SIGEMT
		case SIGEMT:  return "Emulation trap.";
		#endif

		#ifdef SIGPWR
		case SIGPWR:  return "Power fail or restart.";
		#endif

		#ifdef SIGWINCH
		case SIGWINCH:return "Window size change.";
		#endif

		#ifdef SIGPROF
		case SIGPROF: return "Profiling timer expired.";
		#endif

		#ifdef SIGFREEZE
		case SIGFREEZE: return "Checkpoint freeze.";
		#endif

		#ifdef SIGTHAW
		case SIGTHAW: return "Checkpoint thaw.";
		#endif

		#ifdef SIGLOST
		case SIGLOST: return "Resource lost.";
		#endif

		#ifdef SIGXRES
		case SIGXRES: return "Resource control exceeded.";
		#endif
	}

	return NULL;
}
#endif // __hpux

// this must be the *last* file included
#include "ProtectedSource/Epilog.h"

#endif // PDCstring_h
