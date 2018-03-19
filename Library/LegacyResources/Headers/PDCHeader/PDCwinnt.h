//==============================================================================================
//	FILENAME	:	PDCwinnt.h
//	AUTHOR		:	Perl Script
//	CREATION	:	6/13/02
//	Copyright (C) MicroStrategy, Inc. 2002
//==============================================================================================
#ifndef PDCwinnt_h
#define PDCwinnt_h

// this must be the *first* file included
#include "ProtectedSource/Prolog.h"

#if defined(WIN32) && !defined(_USE_MSI_COM)
	#include <winnt.h>
#else

	//==============================================================================================
	//	FILENAME	:	winnt.h
	//	AUTHOR		:	Yuling Ma
	//	CREATION	:	10/9/2001
	//	Copyright (C) MicroStrategy Incorporated 2001
	//	All rights reserved
	//==============================================================================================

	// This header file stands in for the standard 'winnt.h' header file used in VC++
	// We make minimal effort to declare symbols and functions in the same file as
	// used in Windows.

	// We are using this file to hold symbols and declarations that seem to be part of
	// the windows environment.

	#ifndef __WINNT_H__
	#define __WINNT_H__

	//
	//  Language IDs.
	//
	//  The following two combinations of primary language ID and
	//  sublanguage ID have special semantics:
	//
	//    Primary Language ID   Sublanguage ID      Result
	//    -------------------   ---------------     ------------------------
	//    LANG_NEUTRAL          SUBLANG_NEUTRAL     Language neutral
	//    LANG_NEUTRAL          SUBLANG_DEFAULT     User default language
	//    LANG_NEUTRAL          SUBLANG_SYS_DEFAULT System default language
	//

	//
	//  Primary language IDs.
	//

	#define LANG_NEUTRAL                     0x00

	#define LANG_AFRIKAANS                   0x36
	#define LANG_ALBANIAN                    0x1c
	#define LANG_ARABIC                      0x01
	#define LANG_ARMENIAN                    0x2b
	#define LANG_ASSAMESE                    0x4d
	#define LANG_AZERI                       0x2c
	#define LANG_BASQUE                      0x2d
	#define LANG_BELARUSIAN                  0x23
	#define LANG_BENGALI                     0x45
	#define LANG_BULGARIAN                   0x02
	#define LANG_CATALAN                     0x03
	#define LANG_CHINESE                     0x04
	#define LANG_CROATIAN                    0x1a
	#define LANG_CZECH                       0x05
	#define LANG_DANISH                      0x06
	#define LANG_DUTCH                       0x13
	#define LANG_ENGLISH                     0x09
	#define LANG_ESTONIAN                    0x25
	#define LANG_FAEROESE                    0x38
	#define LANG_FARSI                       0x29
	#define LANG_FINNISH                     0x0b
	#define LANG_FRENCH                      0x0c
	#define LANG_GEORGIAN                    0x37
	#define LANG_GERMAN                      0x07
	#define LANG_GREEK                       0x08
	#define LANG_GUJARATI                    0x47
	#define LANG_HEBREW                      0x0d
	#define LANG_HINDI                       0x39
	#define LANG_HUNGARIAN                   0x0e
	#define LANG_ICELANDIC                   0x0f
	#define LANG_INDONESIAN                  0x21
	#define LANG_ITALIAN                     0x10
	#define LANG_JAPANESE                    0x11
	#define LANG_KANNADA                     0x4b
	#define LANG_KASHMIRI                    0x60
	#define LANG_KAZAK                       0x3f
	#define LANG_KONKANI                     0x57
	#define LANG_KOREAN                      0x12
	#define LANG_LATVIAN                     0x26
	#define LANG_LITHUANIAN                  0x27
	#define LANG_MACEDONIAN                  0x2f
	#define LANG_MALAY                       0x3e
	#define LANG_MALAYALAM                   0x4c
	#define LANG_MANIPURI                    0x58
	#define LANG_MARATHI                     0x4e
	#define LANG_NEPALI                      0x61
	#define LANG_NORWEGIAN                   0x14
	#define LANG_ORIYA                       0x48
	#define LANG_POLISH                      0x15
	#define LANG_PORTUGUESE                  0x16
	#define LANG_PUNJABI                     0x46
	#define LANG_ROMANIAN                    0x18
	#define LANG_RUSSIAN                     0x19
	#define LANG_SANSKRIT                    0x4f
	#define LANG_SERBIAN                     0x1a
	#define LANG_SINDHI                      0x59
	#define LANG_SLOVAK                      0x1b
	#define LANG_SLOVENIAN                   0x24
	#define LANG_SPANISH                     0x0a
	#define LANG_SWAHILI                     0x41
	#define LANG_SWEDISH                     0x1d
	#define LANG_TAMIL                       0x49
	#define LANG_TATAR                       0x44
	#define LANG_TELUGU                      0x4a
	#define LANG_THAI                        0x1e
	#define LANG_TURKISH                     0x1f
	#define LANG_UKRAINIAN                   0x22
	#define LANG_URDU                        0x20
	#define LANG_UZBEK                       0x43
	#define LANG_VIETNAMESE                  0x2a

	//
	//  Sublanguage IDs.
	//
	//  The name immediately following SUBLANG_ dictates which primary
	//  language ID that sublanguage ID can be combined with to form a
	//  valid language ID.
	//

	#define SUBLANG_NEUTRAL                  0x00    // language neutral
	#define SUBLANG_DEFAULT                  0x01    // user default
	#define SUBLANG_SYS_DEFAULT              0x02    // system default

	#define SUBLANG_ARABIC_SAUDI_ARABIA      0x01    // Arabic (Saudi Arabia)
	#define SUBLANG_ARABIC_IRAQ              0x02    // Arabic (Iraq)
	#define SUBLANG_ARABIC_EGYPT             0x03    // Arabic (Egypt)
	#define SUBLANG_ARABIC_LIBYA             0x04    // Arabic (Libya)
	#define SUBLANG_ARABIC_ALGERIA           0x05    // Arabic (Algeria)
	#define SUBLANG_ARABIC_MOROCCO           0x06    // Arabic (Morocco)
	#define SUBLANG_ARABIC_TUNISIA           0x07    // Arabic (Tunisia)
	#define SUBLANG_ARABIC_OMAN              0x08    // Arabic (Oman)
	#define SUBLANG_ARABIC_YEMEN             0x09    // Arabic (Yemen)
	#define SUBLANG_ARABIC_SYRIA             0x0a    // Arabic (Syria)
	#define SUBLANG_ARABIC_JORDAN            0x0b    // Arabic (Jordan)
	#define SUBLANG_ARABIC_LEBANON           0x0c    // Arabic (Lebanon)
	#define SUBLANG_ARABIC_KUWAIT            0x0d    // Arabic (Kuwait)
	#define SUBLANG_ARABIC_UAE               0x0e    // Arabic (U.A.E)
	#define SUBLANG_ARABIC_BAHRAIN           0x0f    // Arabic (Bahrain)
	#define SUBLANG_ARABIC_QATAR             0x10    // Arabic (Qatar)
	#define SUBLANG_AZERI_LATIN              0x01    // Azeri (Latin)
	#define SUBLANG_AZERI_CYRILLIC           0x02    // Azeri (Cyrillic)
	#define SUBLANG_CHINESE_TRADITIONAL      0x01    // Chinese (Taiwan Region)
	#define SUBLANG_CHINESE_SIMPLIFIED       0x02    // Chinese (PR China)
	#define SUBLANG_CHINESE_HONGKONG         0x03    // Chinese (Hong Kong)
	#define SUBLANG_CHINESE_SINGAPORE        0x04    // Chinese (Singapore)
	#define SUBLANG_CHINESE_MACAU            0x05    // Chinese (Macau)
	#define SUBLANG_DUTCH                    0x01    // Dutch
	#define SUBLANG_DUTCH_BELGIAN            0x02    // Dutch (Belgian)
	#define SUBLANG_ENGLISH_US               0x01    // English (USA)
	#define SUBLANG_ENGLISH_UK               0x02    // English (UK)
	#define SUBLANG_ENGLISH_AUS              0x03    // English (Australian)
	#define SUBLANG_ENGLISH_CAN              0x04    // English (Canadian)
	#define SUBLANG_ENGLISH_NZ               0x05    // English (New Zealand)
	#define SUBLANG_ENGLISH_EIRE             0x06    // English (Irish)
	#define SUBLANG_ENGLISH_SOUTH_AFRICA     0x07    // English (South Africa)
	#define SUBLANG_ENGLISH_JAMAICA          0x08    // English (Jamaica)
	#define SUBLANG_ENGLISH_CARIBBEAN        0x09    // English (Caribbean)
	#define SUBLANG_ENGLISH_BELIZE           0x0a    // English (Belize)
	#define SUBLANG_ENGLISH_TRINIDAD         0x0b    // English (Trinidad)
	#define SUBLANG_ENGLISH_ZIMBABWE         0x0c    // English (Zimbabwe)
	#define SUBLANG_ENGLISH_PHILIPPINES      0x0d    // English (Philippines)
	#define SUBLANG_FRENCH                   0x01    // French
	#define SUBLANG_FRENCH_BELGIAN           0x02    // French (Belgian)
	#define SUBLANG_FRENCH_CANADIAN          0x03    // French (Canadian)
	#define SUBLANG_FRENCH_SWISS             0x04    // French (Swiss)
	#define SUBLANG_FRENCH_LUXEMBOURG        0x05    // French (Luxembourg)
	#define SUBLANG_FRENCH_MONACO            0x06    // French (Monaco)
	#define SUBLANG_GERMAN                   0x01    // German
	#define SUBLANG_GERMAN_SWISS             0x02    // German (Swiss)
	#define SUBLANG_GERMAN_AUSTRIAN          0x03    // German (Austrian)
	#define SUBLANG_GERMAN_LUXEMBOURG        0x04    // German (Luxembourg)
	#define SUBLANG_GERMAN_LIECHTENSTEIN     0x05    // German (Liechtenstein)
	#define SUBLANG_ITALIAN                  0x01    // Italian
	#define SUBLANG_ITALIAN_SWISS            0x02    // Italian (Swiss)
	#define SUBLANG_KASHMIRI_INDIA           0x02    // Kashmiri (India)
	#define SUBLANG_KOREAN                   0x01    // Korean (Extended Wansung)
	#define SUBLANG_LITHUANIAN               0x01    // Lithuanian
	#define SUBLANG_LITHUANIAN_CLASSIC       0x02    // Lithuanian (Classic)
	#define SUBLANG_MALAY_MALAYSIA           0x01    // Malay (Malaysia)
	#define SUBLANG_MALAY_BRUNEI_DARUSSALAM  0x02    // Malay (Brunei Darussalam)
	#define SUBLANG_NEPALI_INDIA             0x02    // Nepali (India)
	#define SUBLANG_NORWEGIAN_BOKMAL         0x01    // Norwegian (Bokmal)
	#define SUBLANG_NORWEGIAN_NYNORSK        0x02    // Norwegian (Nynorsk)
	#define SUBLANG_PORTUGUESE               0x02    // Portuguese
	#define SUBLANG_PORTUGUESE_BRAZILIAN     0x01    // Portuguese (Brazilian)
	#define SUBLANG_SERBIAN_LATIN            0x02    // Serbian (Latin)
	#define SUBLANG_SERBIAN_CYRILLIC         0x03    // Serbian (Cyrillic)
	#define SUBLANG_SPANISH                  0x01    // Spanish (Castilian)
	#define SUBLANG_SPANISH_MEXICAN          0x02    // Spanish (Mexican)
	#define SUBLANG_SPANISH_MODERN           0x03    // Spanish (Modern)
	#define SUBLANG_SPANISH_GUATEMALA        0x04    // Spanish (Guatemala)
	#define SUBLANG_SPANISH_COSTA_RICA       0x05    // Spanish (Costa Rica)
	#define SUBLANG_SPANISH_PANAMA           0x06    // Spanish (Panama)
	#define SUBLANG_SPANISH_DOMINICAN_REPUBLIC 0x07  // Spanish (Dominican Republic)
	#define SUBLANG_SPANISH_VENEZUELA        0x08    // Spanish (Venezuela)
	#define SUBLANG_SPANISH_COLOMBIA         0x09    // Spanish (Colombia)
	#define SUBLANG_SPANISH_PERU             0x0a    // Spanish (Peru)
	#define SUBLANG_SPANISH_ARGENTINA        0x0b    // Spanish (Argentina)
	#define SUBLANG_SPANISH_ECUADOR          0x0c    // Spanish (Ecuador)
	#define SUBLANG_SPANISH_CHILE            0x0d    // Spanish (Chile)
	#define SUBLANG_SPANISH_URUGUAY          0x0e    // Spanish (Uruguay)
	#define SUBLANG_SPANISH_PARAGUAY         0x0f    // Spanish (Paraguay)
	#define SUBLANG_SPANISH_BOLIVIA          0x10    // Spanish (Bolivia)
	#define SUBLANG_SPANISH_EL_SALVADOR      0x11    // Spanish (El Salvador)
	#define SUBLANG_SPANISH_HONDURAS         0x12    // Spanish (Honduras)
	#define SUBLANG_SPANISH_NICARAGUA        0x13    // Spanish (Nicaragua)
	#define SUBLANG_SPANISH_PUERTO_RICO      0x14    // Spanish (Puerto Rico)
	#define SUBLANG_SWEDISH                  0x01    // Swedish
	#define SUBLANG_SWEDISH_FINLAND          0x02    // Swedish (Finland)
	#define SUBLANG_URDU_PAKISTAN            0x01    // Urdu (Pakistan)
	#define SUBLANG_URDU_INDIA               0x02    // Urdu (India)
	#define SUBLANG_UZBEK_LATIN              0x01    // Uzbek (Latin)
	#define SUBLANG_UZBEK_CYRILLIC           0x02    // Uzbek (Cyrillic)

	//
	//  Sorting IDs.
	//

	#define SORT_DEFAULT                     0x0     // sorting default

	#define SORT_JAPANESE_XJIS               0x0     // Japanese XJIS order
	#define SORT_JAPANESE_UNICODE            0x1     // Japanese Unicode order

	#define SORT_CHINESE_BIG5                0x0     // Chinese BIG5 order
	#define SORT_CHINESE_PRCP                0x0     // PRC Chinese Phonetic order
	#define SORT_CHINESE_UNICODE             0x1     // Chinese Unicode order
	#define SORT_CHINESE_PRC                 0x2     // PRC Chinese Stroke Count order
	#define SORT_CHINESE_BOPOMOFO            0x3     // Traditional Chinese Bopomofo order

	#define SORT_KOREAN_KSC                  0x0     // Korean KSC order
	#define SORT_KOREAN_UNICODE              0x1     // Korean Unicode order

	#define SORT_GERMAN_PHONE_BOOK           0x1     // German Phone Book order

	#define SORT_HUNGARIAN_DEFAULT           0x0     // Hungarian Default order
	#define SORT_HUNGARIAN_TECHNICAL         0x1     // Hungarian Technical order

	#define SORT_GEORGIAN_TRADITIONAL        0x0     // Georgian Traditional order
	#define SORT_GEORGIAN_MODERN             0x1     // Georgian Modern order

	// end_r_winnt

	//
	//  A language ID is a 16 bit value which is the combination of a
	//  primary language ID and a secondary language ID.  The bits are
	//  allocated as follows:
	//
	//       +-----------------------+-------------------------+
	//       |     Sublanguage ID    |   Primary Language ID   |
	//       +-----------------------+-------------------------+
	//        15                   10 9                       0   bit
	//
	//
	//  Language ID creation/extraction macros:
	//
	//    MAKELANGID    - construct language id from a primary language id and
	//                    a sublanguage id.
	//    PRIMARYLANGID - extract primary language id from a language id.
	//    SUBLANGID     - extract sublanguage id from a language id.
	//

	#define MAKELANGID(p, s)       ((((WORD  )(s)) << 10) | (WORD  )(p))
	#define PRIMARYLANGID(lgid)    ((WORD  )(lgid) & 0x3ff)
	#define SUBLANGID(lgid)        ((WORD  )(lgid) >> 10)


	//
	//  A locale ID is a 32 bit value which is the combination of a
	//  language ID, a sort ID, and a reserved area.  The bits are
	//  allocated as follows:
	//
	//       +-------------+---------+-------------------------+
	//       |   Reserved  | Sort ID |      Language ID        |
	//       +-------------+---------+-------------------------+
	//        31         20 19     16 15                      0   bit
	//
	//
	//  Locale ID creation/extraction macros:
	//
	//    MAKELCID            - construct the locale id from a language id and a sort id.
	//    MAKESORTLCID        - construct the locale id from a language id, sort id, and sort version.
	//    LANGIDFROMLCID      - extract the language id from a locale id.
	//    SORTIDFROMLCID      - extract the sort id from a locale id.
	//    SORTVERSIONFROMLCID - extract the sort version from a locale id.
	//

	#define NLS_VALID_LOCALE_MASK  0x000fffff

	#define MAKELCID(lgid, srtid)  ((DWORD)((((DWORD)((WORD  )(srtid))) << 16) |  \
											 ((DWORD)((WORD  )(lgid)))))
	#define MAKESORTLCID(lgid, srtid, ver)                                            \
								   ((DWORD)((MAKELCID(lgid, srtid)) |             \
										(((DWORD)((WORD  )(ver))) << 20)))
	#define LANGIDFROMLCID(lcid)   ((WORD  )(lcid))
	#define SORTIDFROMLCID(lcid)   ((WORD  )((((DWORD)(lcid)) >> 16) & 0xf))
	#define SORTVERSIONFROMLCID(lcid)  ((WORD  )((((DWORD)(lcid)) >> 20) & 0xf))

	//
	//  Default System and User IDs for language and locale.
	//

	#define LANG_SYSTEM_DEFAULT    (MAKELANGID(LANG_NEUTRAL, SUBLANG_SYS_DEFAULT))
	#define LANG_USER_DEFAULT      (MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT))

	#define LOCALE_SYSTEM_DEFAULT  (MAKELCID(LANG_SYSTEM_DEFAULT, SORT_DEFAULT))
	#define LOCALE_USER_DEFAULT    (MAKELCID(LANG_USER_DEFAULT, SORT_DEFAULT))

	#define LOCALE_NEUTRAL                                                        \
			  (MAKELCID(MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL), SORT_DEFAULT))

	#ifndef WIN32_NO_STATUS
	/*lint -save -e767 */
	#define STATUS_WAIT_0                    ((DWORD   )0x00000000L)
	#define STATUS_ABANDONED_WAIT_0          ((DWORD   )0x00000080L)
	#define STATUS_USER_APC                  ((DWORD   )0x000000C0L)
	#define STATUS_TIMEOUT                   ((DWORD   )0x00000102L)
	#define STATUS_PENDING                   ((DWORD   )0x00000103L)
	#define STATUS_SEGMENT_NOTIFICATION      ((DWORD   )0x40000005L)
	#define STATUS_GUARD_PAGE_VIOLATION      ((DWORD   )0x80000001L)
	#define STATUS_DATATYPE_MISALIGNMENT     ((DWORD   )0x80000002L)
	#define STATUS_BREAKPOINT                ((DWORD   )0x80000003L)
	#define STATUS_SINGLE_STEP               ((DWORD   )0x80000004L)
	#define STATUS_ACCESS_VIOLATION          ((DWORD   )0xC0000005L)
	#define STATUS_IN_PAGE_ERROR             ((DWORD   )0xC0000006L)
	#define STATUS_INVALID_HANDLE            ((DWORD   )0xC0000008L)
	#define STATUS_NO_MEMORY                 ((DWORD   )0xC0000017L)
	#define STATUS_ILLEGAL_INSTRUCTION       ((DWORD   )0xC000001DL)
	#define STATUS_NONCONTINUABLE_EXCEPTION  ((DWORD   )0xC0000025L)
	#define STATUS_INVALID_DISPOSITION       ((DWORD   )0xC0000026L)
	#define STATUS_ARRAY_BOUNDS_EXCEEDED     ((DWORD   )0xC000008CL)
	#define STATUS_FLOAT_DENORMAL_OPERAND    ((DWORD   )0xC000008DL)
	#define STATUS_FLOAT_DIVIDE_BY_ZERO      ((DWORD   )0xC000008EL)
	#define STATUS_FLOAT_INEXACT_RESULT      ((DWORD   )0xC000008FL)
	#define STATUS_FLOAT_INVALID_OPERATION   ((DWORD   )0xC0000090L)
	#define STATUS_FLOAT_OVERFLOW            ((DWORD   )0xC0000091L)
	#define STATUS_FLOAT_STACK_CHECK         ((DWORD   )0xC0000092L)
	#define STATUS_FLOAT_UNDERFLOW           ((DWORD   )0xC0000093L)
	#define STATUS_INTEGER_DIVIDE_BY_ZERO    ((DWORD   )0xC0000094L)
	#define STATUS_INTEGER_OVERFLOW          ((DWORD   )0xC0000095L)
	#define STATUS_PRIVILEGED_INSTRUCTION    ((DWORD   )0xC0000096L)
	#define STATUS_STACK_OVERFLOW            ((DWORD   )0xC00000FDL)
	#define STATUS_CONTROL_C_EXIT            ((DWORD   )0xC000013AL)
	#define STATUS_FLOAT_MULTIPLE_FAULTS     ((DWORD   )0xC00002B4L)
	#define STATUS_FLOAT_MULTIPLE_TRAPS      ((DWORD   )0xC00002B5L)
	#define STATUS_ILLEGAL_VLM_REFERENCE     ((DWORD   )0xC00002C0L)
	/*lint -restore */
	#endif

	//
	// The types of events that can be logged.
	//
	#define EVENTLOG_SUCCESS                0X0000
	#define EVENTLOG_ERROR_TYPE             0x0001
	#define EVENTLOG_WARNING_TYPE           0x0002
	#define EVENTLOG_INFORMATION_TYPE       0x0004
	#define EVENTLOG_AUDIT_SUCCESS          0x0008
	#define EVENTLOG_AUDIT_FAILURE          0x0010

	#define MEM_COMMIT           0x1000
	#define MEM_RESERVE          0x2000
	#define MEM_DECOMMIT         0x4000
	#define MEM_RELEASE          0x8000
	#define MEM_FREE            0x10000
	#define MEM_PRIVATE         0x20000
	#define MEM_MAPPED          0x40000
	#define MEM_RESET           0x80000
	#define MEM_TOP_DOWN       0x100000
	#define MEM_4MB_PAGES    0x80000000
	#define SEC_FILE           0x800000
	#define SEC_IMAGE         0x1000000
	#define SEC_VLM           0x2000000
	#define SEC_RESERVE       0x4000000
	#define SEC_COMMIT        0x8000000
	#define SEC_NOCACHE      0x10000000
	#define MEM_IMAGE         SEC_IMAGE

#ifdef WIN32		//kxiao add for security subsytem  on MSICOM Windows

typedef DWORD ACCESS_MASK;
typedef ACCESS_MASK *PACCESS_MASK;
// Registry Specific Access Rights.
//

#define KEY_QUERY_VALUE         (0x0001)
#define KEY_SET_VALUE           (0x0002)
#define KEY_CREATE_SUB_KEY      (0x0004)
#define KEY_ENUMERATE_SUB_KEYS  (0x0008)
#define KEY_NOTIFY              (0x0010)
#define KEY_CREATE_LINK         (0x0020)
#define KEY_WOW64_32KEY         (0x0200)
#define KEY_WOW64_64KEY         (0x0100)
#define KEY_WOW64_RES           (0x0300)

#define KEY_READ                ((STANDARD_RIGHTS_READ       |\
                                  KEY_QUERY_VALUE            |\
                                  KEY_ENUMERATE_SUB_KEYS     |\
                                  KEY_NOTIFY)                 \
                                  &                           \
                                 (~SYNCHRONIZE))


#define KEY_WRITE               ((STANDARD_RIGHTS_WRITE      |\
                                  KEY_SET_VALUE              |\
                                  KEY_CREATE_SUB_KEY)         \
                                  &                           \
                                 (~SYNCHRONIZE))

#define KEY_EXECUTE             ((KEY_READ)                   \
                                  &                           \
                                 (~SYNCHRONIZE))

#define KEY_ALL_ACCESS          ((STANDARD_RIGHTS_ALL        |\
                                  KEY_QUERY_VALUE            |\
                                  KEY_SET_VALUE              |\
                                  KEY_CREATE_SUB_KEY         |\
                                  KEY_ENUMERATE_SUB_KEYS     |\
                                  KEY_NOTIFY                 |\
                                  KEY_CREATE_LINK)            \
                                  &                           \
                                 (~SYNCHRONIZE))

//
// Open/Create Options
//

#define REG_OPTION_RESERVED         (0x00000000L)   // Parameter is reserved

#define REG_OPTION_NON_VOLATILE     (0x00000000L)   // Key is preserved
                                                    // when system is rebooted

#define REG_OPTION_VOLATILE         (0x00000001L)   // Key is not preserved
                                                    // when system is rebooted

#define REG_OPTION_CREATE_LINK      (0x00000002L)   // Created key is a
                                                    // symbolic link

#define REG_OPTION_BACKUP_RESTORE   (0x00000004L)   // open for backup or restore
                                                    // special access rules
                                                    // privilege required

#define REG_OPTION_OPEN_LINK        (0x00000008L)   // Open symbolic link

#define REG_LEGAL_OPTION            \
                (REG_OPTION_RESERVED            |\
                 REG_OPTION_NON_VOLATILE        |\
                 REG_OPTION_VOLATILE            |\
                 REG_OPTION_CREATE_LINK         |\
                 REG_OPTION_BACKUP_RESTORE      |\
                 REG_OPTION_OPEN_LINK)

//
// Key creation/open disposition
//

#define REG_CREATED_NEW_KEY         (0x00000001L)   // New Registry Key created
#define REG_OPENED_EXISTING_KEY     (0x00000002L)   // Existing Key opened

//
// hive format to be used by Reg(Nt)SaveKeyEx
//
#define REG_STANDARD_FORMAT     1
#define REG_LATEST_FORMAT       2
#define REG_NO_COMPRESSION      4

//
// Key restore flags
//

#define REG_WHOLE_HIVE_VOLATILE     (0x00000001L)   // Restore whole hive volatile
#define REG_REFRESH_HIVE            (0x00000002L)   // Unwind changes to last flush
#define REG_NO_LAZY_FLUSH           (0x00000004L)   // Never lazy flush this hive
#define REG_FORCE_RESTORE           (0x00000008L)   // Force the restore process even when we have open handles on subkeys

//
// Unload Flags
//
#define REG_FORCE_UNLOAD            1

// end_ntddk end_wdm end_nthal

//
// Notify filter values
//
#define REG_NOTIFY_CHANGE_NAME          (0x00000001L) // Create or delete (child)
#define REG_NOTIFY_CHANGE_ATTRIBUTES    (0x00000002L)
#define REG_NOTIFY_CHANGE_LAST_SET      (0x00000004L) // time stamp
#define REG_NOTIFY_CHANGE_SECURITY      (0x00000008L)

#define REG_LEGAL_CHANGE_FILTER                 \
                (REG_NOTIFY_CHANGE_NAME          |\
                 REG_NOTIFY_CHANGE_ATTRIBUTES    |\
                 REG_NOTIFY_CHANGE_LAST_SET      |\
                 REG_NOTIFY_CHANGE_SECURITY)

//
//
// Predefined Value Types.
//

#define REG_NONE                    ( 0 )   // No value type
#define REG_SZ                      ( 1 )   // Unicode nul terminated string
#define REG_EXPAND_SZ               ( 2 )   // Unicode nul terminated string
                                            // (with environment variable references)
#define REG_BINARY                  ( 3 )   // Free form binary
#define REG_DWORD                   ( 4 )   // 32-bit number
#define REG_DWORD_LITTLE_ENDIAN     ( 4 )   // 32-bit number (same as REG_DWORD)
#define REG_DWORD_BIG_ENDIAN        ( 5 )   // 32-bit number
#define REG_LINK                    ( 6 )   // Symbolic Link (unicode)
#define REG_MULTI_SZ                ( 7 )   // Multiple Unicode strings
#define REG_RESOURCE_LIST           ( 8 )   // Resource list in the resource map
#define REG_FULL_RESOURCE_DESCRIPTOR ( 9 )  // Resource list in the hardware description
#define REG_RESOURCE_REQUIREMENTS_LIST ( 10 )
#define REG_QWORD                   ( 11 )  // 64-bit number
#define REG_QWORD_LITTLE_ENDIAN     ( 11 )  // 64-bit number (same as REG_QWORD)

//
// The FILE_READ_DATA and FILE_WRITE_DATA constants are also defined in
// devioctl.h as FILE_READ_ACCESS and FILE_WRITE_ACCESS. The values for these
// constants *MUST* always be in sync.
// The values are redefined in devioctl.h because they must be available to
// both DOS and NT.
//

#define FILE_READ_DATA            ( 0x0001 )    // file & pipe
#define FILE_LIST_DIRECTORY       ( 0x0001 )    // directory

#define FILE_WRITE_DATA           ( 0x0002 )    // file & pipe
#define FILE_ADD_FILE             ( 0x0002 )    // directory

#define FILE_APPEND_DATA          ( 0x0004 )    // file
#define FILE_ADD_SUBDIRECTORY     ( 0x0004 )    // directory
#define FILE_CREATE_PIPE_INSTANCE ( 0x0004 )    // named pipe


#define FILE_READ_EA              ( 0x0008 )    // file & directory

#define FILE_WRITE_EA             ( 0x0010 )    // file & directory

#define FILE_EXECUTE              ( 0x0020 )    // file
#define FILE_TRAVERSE             ( 0x0020 )    // directory

#define FILE_DELETE_CHILD         ( 0x0040 )    // directory

#define FILE_READ_ATTRIBUTES      ( 0x0080 )    // all

#define FILE_WRITE_ATTRIBUTES     ( 0x0100 )    // all

#define FILE_ALL_ACCESS (STANDARD_RIGHTS_REQUIRED | SYNCHRONIZE | 0x1FF)

#define FILE_GENERIC_READ         (STANDARD_RIGHTS_READ     |\
                                   FILE_READ_DATA           |\
                                   FILE_READ_ATTRIBUTES     |\
                                   FILE_READ_EA             |\
                                   SYNCHRONIZE)


#define FILE_GENERIC_WRITE        (STANDARD_RIGHTS_WRITE    |\
                                   FILE_WRITE_DATA          |\
                                   FILE_WRITE_ATTRIBUTES    |\
                                   FILE_WRITE_EA            |\
                                   FILE_APPEND_DATA         |\
                                   SYNCHRONIZE)


#define FILE_GENERIC_EXECUTE      (STANDARD_RIGHTS_EXECUTE  |\
                                   FILE_READ_ATTRIBUTES     |\
                                   FILE_EXECUTE             |\
                                   SYNCHRONIZE)

#define FILE_SHARE_READ                 0x00000001
#define FILE_SHARE_WRITE                0x00000002
#define FILE_SHARE_DELETE               0x00000004
#define FILE_ATTRIBUTE_READONLY             0x00000001
#define FILE_ATTRIBUTE_HIDDEN               0x00000002
#define FILE_ATTRIBUTE_SYSTEM               0x00000004
#define FILE_ATTRIBUTE_DIRECTORY            0x00000010
#define FILE_ATTRIBUTE_ARCHIVE              0x00000020
#define FILE_ATTRIBUTE_DEVICE               0x00000040
#define FILE_ATTRIBUTE_NORMAL               0x00000080
#define FILE_ATTRIBUTE_TEMPORARY            0x00000100
#define FILE_ATTRIBUTE_SPARSE_FILE          0x00000200
#define FILE_ATTRIBUTE_REPARSE_POINT        0x00000400
#define FILE_ATTRIBUTE_COMPRESSED           0x00000800
#define FILE_ATTRIBUTE_OFFLINE              0x00001000
#define FILE_ATTRIBUTE_NOT_CONTENT_INDEXED  0x00002000
#define FILE_ATTRIBUTE_ENCRYPTED            0x00004000
#define FILE_NOTIFY_CHANGE_FILE_NAME    0x00000001
#define FILE_NOTIFY_CHANGE_DIR_NAME     0x00000002
#define FILE_NOTIFY_CHANGE_ATTRIBUTES   0x00000004
#define FILE_NOTIFY_CHANGE_SIZE         0x00000008
#define FILE_NOTIFY_CHANGE_LAST_WRITE   0x00000010
#define FILE_NOTIFY_CHANGE_LAST_ACCESS  0x00000020
#define FILE_NOTIFY_CHANGE_CREATION     0x00000040
#define FILE_NOTIFY_CHANGE_SECURITY     0x00000100
#define FILE_ACTION_ADDED                   0x00000001
#define FILE_ACTION_REMOVED                 0x00000002
#define FILE_ACTION_MODIFIED                0x00000003
#define FILE_ACTION_RENAMED_OLD_NAME        0x00000004
#define FILE_ACTION_RENAMED_NEW_NAME        0x00000005
#define MAILSLOT_NO_MESSAGE             ((DWORD)-1)
#define MAILSLOT_WAIT_FOREVER           ((DWORD)-1)
#define FILE_CASE_SENSITIVE_SEARCH      0x00000001
#define FILE_CASE_PRESERVED_NAMES       0x00000002
#define FILE_UNICODE_ON_DISK            0x00000004
#define FILE_PERSISTENT_ACLS            0x00000008
#define FILE_FILE_COMPRESSION           0x00000010
#define FILE_VOLUME_QUOTAS              0x00000020
#define FILE_SUPPORTS_SPARSE_FILES      0x00000040
#define FILE_SUPPORTS_REPARSE_POINTS    0x00000080
#define FILE_SUPPORTS_REMOTE_STORAGE    0x00000100
#define FILE_VOLUME_IS_COMPRESSED       0x00008000
#define FILE_SUPPORTS_OBJECT_IDS        0x00010000
#define FILE_SUPPORTS_ENCRYPTION        0x00020000
#define FILE_NAMED_STREAMS              0x00040000
#define FILE_READ_ONLY_VOLUME           0x00080000

#define GENERIC_READ                     (0x80000000L)
#define GENERIC_WRITE                    (0x40000000L)
#define GENERIC_EXECUTE                  (0x20000000L)
#define GENERIC_ALL                      (0x10000000L)


#define ANYSIZE_ARRAY 1
typedef struct _ACL {
    BYTE  AclRevision;
    BYTE  Sbz1;
    WORD   AclSize;
    WORD   AceCount;
    WORD   Sbz2;
} ACL;
typedef ACL *PACL;

//
// Locally Unique Identifier
//

typedef struct _LUID {
    DWORD LowPart;
    LONG HighPart;
} LUID, *PLUID;

typedef PVOID PACCESS_TOKEN;
typedef PVOID PSECURITY_DESCRIPTOR;
typedef PVOID PSID;
typedef WORD   SECURITY_DESCRIPTOR_CONTROL, *PSECURITY_DESCRIPTOR_CONTROL;
#define SECURITY_DESCRIPTOR_MIN_LENGTH   (sizeof(SECURITY_DESCRIPTOR))
typedef struct _SECURITY_DESCRIPTOR {
   BYTE  Revision;
   BYTE  Sbz1;
   SECURITY_DESCRIPTOR_CONTROL Control;
   PSID Owner;
   PSID Group;
   PACL Sacl;
   PACL Dacl;

   } SECURITY_DESCRIPTOR, *PISECURITY_DESCRIPTOR;

////////////////////////////////////////////////////////////////////////
//                                                                    //
//                        LUID_AND_ATTRIBUTES                         //
//                                                                    //
////////////////////////////////////////////////////////////////////////
//
//


#include <pshpack4.h>

typedef struct _LUID_AND_ATTRIBUTES {
    LUID Luid;
    DWORD Attributes;
    } LUID_AND_ATTRIBUTES, * PLUID_AND_ATTRIBUTES;
typedef LUID_AND_ATTRIBUTES LUID_AND_ATTRIBUTES_ARRAY[ANYSIZE_ARRAY];
typedef LUID_AND_ATTRIBUTES_ARRAY *PLUID_AND_ATTRIBUTES_ARRAY;

#include <poppack.h>

////////////////////////////////////////////////////////////////////////
//                                                                    //
//              Security Id     (SID)                                 //
//                                                                    //
////////////////////////////////////////////////////////////////////////
//
//
// Pictorially the structure of an SID is as follows:
//
//         1   1   1   1   1   1
//         5   4   3   2   1   0   9   8   7   6   5   4   3   2   1   0
//      +---------------------------------------------------------------+
//      |      SubAuthorityCount        |Reserved1 (SBZ)|   Revision    |
//      +---------------------------------------------------------------+
//      |                   IdentifierAuthority[0]                      |
//      +---------------------------------------------------------------+
//      |                   IdentifierAuthority[1]                      |
//      +---------------------------------------------------------------+
//      |                   IdentifierAuthority[2]                      |
//      +---------------------------------------------------------------+
//      |                                                               |
//      +- -  -  -  -  -  -  -  SubAuthority[]  -  -  -  -  -  -  -  - -+
//      |                                                               |
//      +---------------------------------------------------------------+
//
//


// begin_ntifs

#ifndef SID_IDENTIFIER_AUTHORITY_DEFINED
#define SID_IDENTIFIER_AUTHORITY_DEFINED
typedef struct _SID_IDENTIFIER_AUTHORITY {
    BYTE  Value[6];
} SID_IDENTIFIER_AUTHORITY, *PSID_IDENTIFIER_AUTHORITY;
#endif

#ifndef SID_DEFINED
#define SID_DEFINED
typedef struct _SID {
   BYTE  Revision;
   BYTE  SubAuthorityCount;
   SID_IDENTIFIER_AUTHORITY IdentifierAuthority;
#ifdef MIDL_PASS
   [size_is(SubAuthorityCount)] DWORD SubAuthority[*];
#else // MIDL_PASS
   DWORD SubAuthority[ANYSIZE_ARRAY];
#endif // MIDL_PASS
} SID, *PISID;
#endif

#define SID_REVISION                     (1)    // Current revision level
#define SID_MAX_SUB_AUTHORITIES          (15)
#define SID_RECOMMENDED_SUB_AUTHORITIES  (1)    // Will change to around 6
                                                // in a future release.

typedef enum _SID_NAME_USE {
    SidTypeUser = 1,
    SidTypeGroup,
    SidTypeDomain,
    SidTypeAlias,
    SidTypeWellKnownGroup,
    SidTypeDeletedAccount,
    SidTypeInvalid,
    SidTypeUnknown,
    SidTypeComputer
} SID_NAME_USE, *PSID_NAME_USE;

typedef struct _SID_AND_ATTRIBUTES {
    PSID Sid;
    DWORD Attributes;
    } SID_AND_ATTRIBUTES, * PSID_AND_ATTRIBUTES;

typedef SID_AND_ATTRIBUTES SID_AND_ATTRIBUTES_ARRAY[ANYSIZE_ARRAY];
typedef SID_AND_ATTRIBUTES_ARRAY *PSID_AND_ATTRIBUTES_ARRAY;

////////////////////////////////////////////////////////////////////
//                                                                //
//           Token Object Definitions                             //
//                                                                //
//                                                                //
////////////////////////////////////////////////////////////////////


//
// Token Specific Access Rights.
//

#define TOKEN_ASSIGN_PRIMARY    (0x0001)
#define TOKEN_DUPLICATE         (0x0002)
#define TOKEN_IMPERSONATE       (0x0004)
#define TOKEN_QUERY             (0x0008)
#define TOKEN_QUERY_SOURCE      (0x0010)
#define TOKEN_ADJUST_PRIVILEGES (0x0020)
#define TOKEN_ADJUST_GROUPS     (0x0040)
#define TOKEN_ADJUST_DEFAULT    (0x0080)
#define TOKEN_ADJUST_SESSIONID  (0x0100)

#define TOKEN_ALL_ACCESS (STANDARD_RIGHTS_REQUIRED  |\
                          TOKEN_ASSIGN_PRIMARY      |\
                          TOKEN_DUPLICATE           |\
                          TOKEN_IMPERSONATE         |\
                          TOKEN_QUERY               |\
                          TOKEN_QUERY_SOURCE        |\
                          TOKEN_ADJUST_PRIVILEGES   |\
                          TOKEN_ADJUST_GROUPS       |\
                          TOKEN_ADJUST_SESSIONID    |\
                          TOKEN_ADJUST_DEFAULT)


#define TOKEN_READ       (STANDARD_RIGHTS_READ      |\
                          TOKEN_QUERY)


#define TOKEN_WRITE      (STANDARD_RIGHTS_WRITE     |\
                          TOKEN_ADJUST_PRIVILEGES   |\
                          TOKEN_ADJUST_GROUPS       |\
                          TOKEN_ADJUST_DEFAULT)

#define TOKEN_EXECUTE    (STANDARD_RIGHTS_EXECUTE)


//
//
// Token Types
//

typedef enum _TOKEN_TYPE {
    TokenPrimary = 1,
    TokenImpersonation
    } TOKEN_TYPE;
typedef TOKEN_TYPE *PTOKEN_TYPE;


//
// Token Information Classes.
//


typedef enum _TOKEN_INFORMATION_CLASS {
    TokenUser = 1,
    TokenGroups,
    TokenPrivileges,
    TokenOwner,
    TokenPrimaryGroup,
    TokenDefaultDacl,
    TokenSource,
    TokenType,
    TokenImpersonationLevel,
    TokenStatistics,
    TokenRestrictedSids,
    TokenSessionId
} TOKEN_INFORMATION_CLASS, *PTOKEN_INFORMATION_CLASS;

//
// Token information class structures
//


typedef struct _TOKEN_USER {
    SID_AND_ATTRIBUTES User;
} TOKEN_USER, *PTOKEN_USER;

typedef struct _TOKEN_GROUPS {
    DWORD GroupCount;
    SID_AND_ATTRIBUTES Groups[ANYSIZE_ARRAY];
} TOKEN_GROUPS, *PTOKEN_GROUPS;


typedef struct _TOKEN_PRIVILEGES {
    DWORD PrivilegeCount;
    LUID_AND_ATTRIBUTES Privileges[ANYSIZE_ARRAY];
} TOKEN_PRIVILEGES, *PTOKEN_PRIVILEGES;


typedef struct _TOKEN_OWNER {
    PSID Owner;
} TOKEN_OWNER, *PTOKEN_OWNER;


typedef struct _TOKEN_PRIMARY_GROUP {
    PSID PrimaryGroup;
} TOKEN_PRIMARY_GROUP, *PTOKEN_PRIMARY_GROUP;


typedef struct _TOKEN_DEFAULT_DACL {
    PACL DefaultDacl;
} TOKEN_DEFAULT_DACL, *PTOKEN_DEFAULT_DACL;

// 2/13/2006 qxue: added for Windows MSICOM configuration
typedef struct _MEMORY_BASIC_INFORMATION {
    PVOID BaseAddress;
    PVOID AllocationBase;
    DWORD AllocationProtect;
    SIZE_T RegionSize;
    DWORD State;
    DWORD Protect;
    DWORD Type;
} MEMORY_BASIC_INFORMATION, *PMEMORY_BASIC_INFORMATION;

#define PAGE_NOACCESS          0x01
#define PAGE_READONLY          0x02
#define PAGE_READWRITE         0x04
#define PAGE_WRITECOPY         0x08
#define PAGE_EXECUTE           0x10
#define PAGE_EXECUTE_READ      0x20
#define PAGE_EXECUTE_READWRITE 0x40
#define PAGE_EXECUTE_WRITECOPY 0x80
#define PAGE_GUARD            0x100
#define PAGE_NOCACHE          0x200
#define PAGE_WRITECOMBINE     0x400
#define MEM_COMMIT           0x1000
#define MEM_RESERVE          0x2000
#define MEM_DECOMMIT         0x4000
#define MEM_RELEASE          0x8000
#define MEM_FREE            0x10000
#define MEM_PRIVATE         0x20000
#define MEM_MAPPED          0x40000
#define MEM_RESET           0x80000
#define MEM_TOP_DOWN       0x100000
#define MEM_WRITE_WATCH    0x200000
#define MEM_PHYSICAL       0x400000
#define MEM_LARGE_PAGES  0x20000000
#define MEM_4MB_PAGES    0x80000000
#define SEC_FILE           0x800000
#define SEC_IMAGE         0x1000000
#define SEC_RESERVE       0x4000000
#define SEC_COMMIT        0x8000000
#define SEC_NOCACHE      0x10000000
#define SEC_LARGE_PAGES  0x80000000
#define MEM_IMAGE         SEC_IMAGE
#define WRITE_WATCH_FLAG_RESET 0x01

// begin_ntddk begin_wdm begin_nthal begin_ntifs
//
//  The following are masks for the predefined standard access types
//

#define DELETE                           (0x00010000L)
#define READ_CONTROL                     (0x00020000L)
#define WRITE_DAC                        (0x00040000L)
#define WRITE_OWNER                      (0x00080000L)
#define SYNCHRONIZE                      (0x00100000L)

#define STANDARD_RIGHTS_REQUIRED         (0x000F0000L)

#define STANDARD_RIGHTS_READ             (READ_CONTROL)
#define STANDARD_RIGHTS_WRITE            (READ_CONTROL)
#define STANDARD_RIGHTS_EXECUTE          (READ_CONTROL)

#define STANDARD_RIGHTS_ALL              (0x001F0000L)

#define SPECIFIC_RIGHTS_ALL              (0x0000FFFFL)

#define SECTION_QUERY                0x0001
#define SECTION_MAP_WRITE            0x0002
#define SECTION_MAP_READ             0x0004
#define SECTION_MAP_EXECUTE          0x0008
#define SECTION_EXTEND_SIZE          0x0010
#define SECTION_MAP_EXECUTE_EXPLICIT 0x0020 // not included in SECTION_ALL_ACCESS

#define SECTION_ALL_ACCESS (STANDARD_RIGHTS_REQUIRED|SECTION_QUERY|\
                            SECTION_MAP_WRITE |      \
                            SECTION_MAP_READ |       \
                            SECTION_MAP_EXECUTE |    \
                            SECTION_EXTEND_SIZE)


#define SE_DEBUG_NAME                     L"SeDebugPrivilege"
// begin_wdm begin_ntddk begin_nthal
//
// Privilege attributes
//

#define SE_PRIVILEGE_ENABLED_BY_DEFAULT (0x00000001L)
#define SE_PRIVILEGE_ENABLED            (0x00000002L)
#define SE_PRIVILEGE_REMOVED            (0X00000004L)
#define SE_PRIVILEGE_USED_FOR_ACCESS    (0x80000000L)

#if defined(_M_MRX000) && !(defined(MIDL_PASS) || defined(RC_INVOKED)) && defined(ENABLE_RESTRICTED)
#define RESTRICTED_POINTER __restrict
#else
#define RESTRICTED_POINTER
#endif

typedef struct _LIST_ENTRY {
   struct _LIST_ENTRY *Flink;
   struct _LIST_ENTRY *Blink;
} LIST_ENTRY, *PLIST_ENTRY, *RESTRICTED_POINTER PRLIST_ENTRY;

typedef struct _RTL_CRITICAL_SECTION_DEBUG {
    WORD   Type;
    WORD   CreatorBackTraceIndex;
    struct _RTL_CRITICAL_SECTION *CriticalSection;
    LIST_ENTRY ProcessLocksList;
    DWORD EntryCount;
    DWORD ContentionCount;
    DWORD Spare[ 2 ];
} RTL_CRITICAL_SECTION_DEBUG, *PRTL_CRITICAL_SECTION_DEBUG, RTL_RESOURCE_DEBUG, *PRTL_RESOURCE_DEBUG;

typedef struct _RTL_CRITICAL_SECTION {
    PRTL_CRITICAL_SECTION_DEBUG DebugInfo;

    //
    //  The following three fields control entering and exiting the critical
    //  section for the resource
    //

    LONG LockCount;
    LONG RecursionCount;
    HANDLE OwningThread;        // from the thread's ClientId->UniqueThread
    HANDLE LockSemaphore;
    ULONG_PTR SpinCount;        // force size on 64-bit systems when packed
} RTL_CRITICAL_SECTION, *PRTL_CRITICAL_SECTION;

typedef RTL_CRITICAL_SECTION CRITICAL_SECTION;

#endif // WIN32

	#endif /* __WINNT_H__ */
#endif // WIN32 && !_USE_MSI_COM

// this must be the *last* file included
#include "ProtectedSource/Epilog.h"

#endif // PDCwinnt_h
