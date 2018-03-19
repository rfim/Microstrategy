//==============================================================================================
//	FILENAME	:	Path.h
//	AUTHOR		:	vovechkin
//	CREATION	:	2002-08-27
//	Copyright (C) MicroStrategy Incorporated 2002
//	All Rights Reserved
//==============================================================================================
#ifndef MFileSystem_Path_h
#define MFileSystem_Path_h

#include "FileSystem.h"

//Yuqiang Huang - 01/13/2010 - TQMS 447466, 448037
//To make consistent with GUI (Web/MSTRDesk), make file name length limit to 250
const int FILE_NAME_LENGTH_LIMIT = 250;
const int FILE_EXTENSION_LENGTH_LIMIT = 4; //Based common practice
//Though different platforms have different limit, we simply choose the common mininum.
const int COMPLETE_FILE_NAME_LENGTH_LIMIT = 255;

#if defined(WIN32)
	const int COMPLETE_FILE_PATH_LENGTH_LIMIT = 255;
#else
	const int COMPLETE_FILE_PATH_LENGTH_LIMIT = 2147483647;  //INT_MAX - Maximum value for a variable of type int.
#endif

//Yuqiang Huang - 09/27/2010
//i-Server runs on Unix and saves files to Windows file sever
typedef enum
{
	DssNCSFileDestWindows	= 0,
	DssNCSFileDestUnix		= 1
} EnumFileDestPlatforms;

namespace MDataType
{
	class StringCollection;
}

namespace MFileSystem
{
	/**
	 * Path contains functions operating upon paths.
	 */
	class DLL_FILE_SYSTEM_EXIM Path
	{
	public:

		/**
		 * Tests whether the path is absolute (or relative).
		 *
		 * On Windows, the path may contain back/forward slashed as 
		 * directory separators.
		 *
		 * On Unices, only forward slash is directory separator.
		 * 
		 * @return true if the path is absolute, false if the path is relative.
		 */
		static bool IsAbsolute( const char* ipPath );

		/**
		 * Doed nothing if path is absolute, otherwise
		 * prepends current working directory.
		 */
		static MBase::ReturnString ToAbsolute(const char* ipPath);

		/**
		 * Tests whether the path is a directory.
		 *
		 * @return true if the path is a directory, false otherwise.
		 */
		static bool IsDirectory( const char* ipPath );

		/**
		 * Tests whether the path is a file.
		 *
		 * @return true if the path is a file, false otherwise.
		 */
		static bool IsFile( const char* ipPath );

		/**
		 * Tests whether the path (file or directory) is readable.
		 *
		 * @return true if the path is readable, false otherwise.
		 */
		static bool IsReadable( const char* ipPath );

		/**
		 * Tests whether the path (file or directory) is writeable.
		 *
		 * @return true if the path is writeable, false otherwise.
		 */
		static bool IsWriteable( const char* ipPath );


		/**
		 * Tests from the root folder going to its child up to the last
		 * child folder that exists.  Non-existent child folders will not
		 * be checked.
		 *
		 * @return true if all the path examined is writable, false otherwise.
		 */
		//static bool AreExistingFoldersWriteable( const char* ipPath );

		/**
		 * Tests from the root folder going to its child up to the last
		 * child folder that exists.  Non-existent child folders will not
		 * be checked.
		 *
		 * @return true if all the path examined is writable, false otherwise.
		 */
		static MBase::ReturnString GetParent( const char* ipPath );

		/**
		 * Converts an external path representation to internal one (platform-specific).
		 *
		 * Use this method before using a path representation, which
		 * was stored externally.
		 *
		 * On Windows, this method will convert a UNIX absolute path to a UNC path,
		 * for example: "/server/share/dir" --> "\\server\share\dir"
		 *
		 * On UNIX, this method will convert a UNC path to a UNIX absolute path,
		 * for example: "\\server\share\dir" --> "/server/share/dir"
		 *
		 */
		static MBase::ReturnString Internalize(const char* ipPath);

		/**
		 * Combines two paths into one.
		 *
		 * This function performs simplification of paths as well, such as 
		 * collapsing "foo/.." -> "" and "/." -> "/", and multiple slashes 
		 * into a single one: "//" -> "/". Multiple slashes happen to be 
		 * equivalent to a single slash on both Windows and Unix.
		 *
		 * On Windows, the input paths may contain back/forward slashed as 
		 * directory separators. The output is always with back-slashes.
		 *
		 * On Unices, only forward slash is directory separator.
		 *
		 * If the ipPath parameter is an absolute path, the first parameter
		 * is ignored.
		 *
		 * @param iBasePath may be absolute or relative.
		 * @param ipPath assumed to be a relative path.
		 */
		static MBase::ReturnString Combine( const char* ipBasePath, const char* ipPath );

		/**
		 * Returns the directory portion of a path.
		 *
		 * @see man dirname
		 */
		static MBase::ReturnString GetDirname( const char* ipPath );

		/**
		 * Returns the name portion of a path.
		 *
		 * @see man basename
		 */
		static MBase::ReturnString GetBasename( const char* ipPathFileName );

		/**
		 * Returns the extension portion of a filename.
		 *
		 * @see Getbasename
		 */
		static MBase::ReturnString GetExtension(const char* ipFileName);

		/**
		 * Returns the Basename portion of a filename.without the extension
		 *
		 * @see GetExtension, Getbasename
		 */
		static MBase::ReturnString GetBasenameWithoutExtension(const char* ipFileName);

		/**
		 * Returns the list of file names matching the input pattern.
		 *
		 * @throw MFileSystem::Exception()
		 */
		static MDataType::StringCollection* GetFileNamesWithPattern( const char* ipPathPattern );

		//Yuqiang Huang - 09/27/2010
		//i-Server runs on Unix and saves files to Windows file sever
		static bool IsAbsoluteCrossPlatform( const char* ipPath, EnumFileDestPlatforms iFileDestPlatform );
		static MBase::ReturnString CombineCrossPlatform( const char* ipBasePath, const char* ipPath, EnumFileDestPlatforms iFileDestPlatform );
	};
}

#endif // MFileSystem_Path_h
