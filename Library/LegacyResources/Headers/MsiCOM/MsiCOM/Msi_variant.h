// File:	variant.h
// Date:	15/08/2001
// Author:	Yuling Ma
//
// Copyright (C) 2001 MicroStrategy Incorporated
// All rights reserved

// This header file is for defining the VARIANT related structures, symbols and functions.
// On NT platform, the these structures, symbols and functions are defined in the following
// header files: oaidl.h, oleauto.h, wtypes.h



#if !defined( _VARIANT_H_ )
#define _VARIANT_H_

//#include "oaidl.h"
#include "MsiCOM/MsiCOM/MsiCOM.h"
#include "PDCHeader/PDCwtypes.h"
#include "MsiCOM/MsiCOM/Msi_bstr.h"
#ifndef FARSTRUCT
#define FARSTRUCT  
#endif  // FARSTRUCT

#ifdef __ANDROID__
class IUnknown ;
class   IDispatch ;
struct tagSAFEARRAY;
typedef tagSAFEARRAY SAFEARRAY;
#endif

typedef unsigned short VARTYPE;
/*
 * VARENUM usage key,
 *
 * * [V] - may appear in a VARIANT
 * * [T] - may appear in a TYPEDESC
 * * [P] - may appear in an OLE property set
 * * [S] - may appear in a Safe Array
 *
 *
 *  VT_EMPTY            [V]   [P]     nothing
 *  VT_NULL             [V]   [P]     SQL style Null
 *  VT_I2               [V][T][P][S]  2 byte signed int
 *  VT_I4               [V][T][P][S]  4 byte signed int
 *  VT_R4               [V][T][P][S]  4 byte real
 *  VT_R8               [V][T][P][S]  8 byte real
 *  VT_CY               [V][T][P][S]  currency
 *  VT_DATE             [V][T][P][S]  date
 *  VT_BSTR             [V][T][P][S]  OLE Automation string
 *  VT_DISPATCH         [V][T][P][S]  IDispatch *
 *  VT_ERROR            [V][T][P][S]  SCODE
 *  VT_BOOL             [V][T][P][S]  True=-1, False=0
 *  VT_VARIANT          [V][T][P][S]  VARIANT *
 *  VT_UNKNOWN          [V][T]   [S]  IUnknown *
 *  VT_DECIMAL          [V][T]   [S]  16 byte fixed point
 *  VT_RECORD           [V]   [P][S]  user defined type
 *  VT_I1               [V][T][P][s]  signed char
 *  VT_UI1              [V][T][P][S]  unsigned char
 *  VT_UI2              [V][T][P][S]  unsigned short
 *  VT_UI4              [V][T][P][S]  unsigned short
 *  VT_I8                  [T][P]     signed 64-bit int
 *  VT_UI8                 [T][P]     unsigned 64-bit int
 *  VT_INT              [V][T][P][S]  signed machine int
 *  VT_UINT             [V][T]   [S]  unsigned machine int
 *  VT_VOID                [T]        C style void
 *  VT_HRESULT             [T]        Standard return type
 *  VT_PTR                 [T]        pointer type
 *  VT_SAFEARRAY           [T]        (use VT_ARRAY in VARIANT)
 *  VT_CARRAY              [T]        C style array
 *  VT_USERDEFINED         [T]        user defined type
 *  VT_LPSTR               [T][P]     null terminated string
 *  VT_LPWSTR              [T][P]     wide null terminated string
 *  VT_FILETIME               [P]     FILETIME
 *  VT_BLOB                   [P]     Length prefixed bytes
 *  VT_STREAM                 [P]     Name of the stream follows
 *  VT_STORAGE                [P]     Name of the storage follows
 *  VT_STREAMED_OBJECT        [P]     Stream contains an object
 *  VT_STORED_OBJECT          [P]     Storage contains an object
 *  VT_BLOB_OBJECT            [P]     Blob contains an object 
 *  VT_CF                     [P]     Clipboard format
 *  VT_CLSID                  [P]     A Class ID
 *  VT_VECTOR                 [P]     simple counted array
 *  VT_ARRAY            [V]           SAFEARRAY*
 *  VT_BYREF            [V]           void* for local use
 *  VT_BSTR_BLOB                      Reserved for system use
 */

enum EnumVariantType
    {	VT_EMPTY	= 0,
	VT_NULL	= 1,
	VT_I2	= 2,
	VT_I4	= 3,
	VT_R4	= 4,
	VT_R8	= 5,
	VT_CY	= 6,
	VT_DATE	= 7,
	VT_BSTR	= 8,
	VT_DISPATCH	= 9,
	VT_ERROR	= 10,
	VT_BOOL	= 11,
	VT_VARIANT	= 12,
	VT_UNKNOWN	= 13,
	VT_DECIMAL	= 14,
	VT_I1	= 16,
	VT_UI1	= 17,
	VT_UI2	= 18,
	VT_UI4	= 19,
	VT_I8	= 20,
	VT_UI8	= 21,
	VT_INT	= 22,
	VT_UINT	= 23,
	VT_VOID	= 24,
	VT_HRESULT	= 25,
	VT_PTR	= 26,
	VT_SAFEARRAY	= 27,
	VT_CARRAY	= 28,
	VT_USERDEFINED	= 29,
	VT_LPSTR	= 30,
	VT_LPWSTR	= 31,
	VT_RECORD	= 36,
	VT_FILETIME	= 64,
	VT_BLOB	= 65,
	VT_STREAM	= 66,
	VT_STORAGE	= 67,
	VT_STREAMED_OBJECT	= 68,
	VT_STORED_OBJECT	= 69,
	VT_BLOB_OBJECT	= 70,
	VT_CF	= 71,
	VT_CLSID	= 72,
	VT_BSTR_BLOB	= 0xfff,
	VT_VECTOR	= 0x1000,
	VT_ARRAY	= 0x2000,
	VT_BYREF	= 0x4000,
	VT_RESERVED	= 0x8000,
	VT_ILLEGAL	= 0xffff,
	VT_ILLEGALMASKED	= 0xfff,
	VT_TYPEMASK	= 0xfff
    };


typedef /* [wire_marshal] */ struct tagVARIANT VARIANT;
typedef VARIANT __RPC_FAR *LPVARIANT;

typedef VARIANT VARIANTARG;

typedef struct tagVARIANT  {
   VARTYPE vt;
   WORD wReserved1; // typedef unsigned short WORD;
   WORD wReserved2;
   WORD wReserved3;
   union {
      BYTE                    bVal;                 // VT_UI1.
      SHORT                   iVal;                 // VT_I2.
      LONG                    lVal;                 // VT_I4.
      LONGLONG                llVal;                // VT_I8.
      FLOAT                   fltVal;               // VT_R4.
      DOUBLE                  dblVal;               // VT_R8.
      VARIANT_BOOL            boolVal;              // VT_BOOL.
      SCODE                   scode;                // VT_ERROR.
      CY                      cyVal;                // VT_CY.
      DATE                    date;                 // VT_DATE.
      BSTR                    bstrVal;              // VT_BSTR.
      DECIMAL                 FAR* pdecVal;          // VT_BYREF|VT_DECIMAL.
      IUnknown                FAR* punkVal;         // VT_UNKNOWN.
      IDispatch               FAR* pdispVal;        // VT_DISPATCH.
      SAFEARRAY               FAR* parray;          // VT_ARRAY|*.
      BYTE                    FAR* pbVal;           // VT_BYREF|VT_UI1.
      SHORT                   FAR* piVal;           // VT_BYREF|VT_I2.
      LONG                    FAR* plVal;           // VT_BYREF|VT_I4.
      FLOAT                   FAR* pfltVal;         // VT_BYREF|VT_R4.
      DOUBLE                  FAR* pdblVal;         // VT_BYREF|VT_R8.
      VARIANT_BOOL            FAR* pboolVal;        // VT_BYREF|VT_BOOL.
      SCODE                   FAR* pscode;          // VT_BYREF|VT_ERROR.
      CY                      FAR* pcyVal;          // VT_BYREF|VT_CY.
      DATE                    FAR* pdate;           // VT_BYREF|VT_DATE.
      BSTR                    FAR* pbstrVal;        // VT_BYREF|VT_BSTR.
      IUnknown                FAR* FAR* ppunkVal;   // VT_BYREF|VT_UNKNOWN.
      IDispatch               FAR* FAR* ppdispVal;  // VT_BYREF|VT_DISPATCH.
      SAFEARRAY               FAR* FAR* pparray;    // VT_ARRAY|*.
      VARIANT                 FAR* pvarVal;         // VT_BYREF|VT_VARIANT.
      PVOID                   byref;           // Generic ByRef.
      CHAR                    cVal;                 // VT_I1.
      USHORT				  uiVal;                // VT_UI2.
      ULONG					  ulVal;                // VT_UI4.
      INT                     intVal;               // VT_INT.
      UINT					  uintVal;              // VT_UINT.
      CHAR			FAR *     pcVal;                // VT_BYREF|VT_I1.
      USHORT		FAR *     puiVal;               // VT_BYREF|VT_UI2.
      ULONG			FAR *     pulVal;               // VT_BYREF|VT_UI4.
      INT			FAR *     pintVal;              // VT_BYREF|VT_INT.
      UINT			FAR *     puintVal;             //VT_BYREF|VT_UINT.
   };
} tagVARIANT;
 

/*---------------------------------------------------------------------*/
/*                           VARIANT API                               */
/*---------------------------------------------------------------------*/

WINOLEAUTAPI_(void) DLL_MSICOM_EXIM VariantInit(VARIANTARG * pvarg);
WINOLEAUTAPI DLL_MSICOM_EXIM VariantClear(VARIANTARG * pvarg);
WINOLEAUTAPI DLL_MSICOM_EXIM VariantCopy(VARIANTARG * pvargDest, VARIANTARG * pvargSrc);
WINOLEAUTAPI DLL_MSICOM_EXIM VariantCopyInd(VARIANT * pvargDest, VARIANTARG * pvargSrc);
WINOLEAUTAPI DLL_MSICOM_EXIM VariantChangeType(VARIANTARG * pvargDest,
                VARIANTARG * pvarSrc, USHORT wFlags, VARTYPE vt);
WINOLEAUTAPI DLL_MSICOM_EXIM VariantChangeTypeEx(VARIANTARG * pvargDest, VARIANTARG * pvarSrc, LCID lcid, USHORT wFlags, VARTYPE vt);
WINOLEAUTAPI DLL_MSICOM_EXIM VarCmp(LPVARIANT left, LPVARIANT right, LCID lcid, DWORD flags);

// Flags for VariantChangeType/VariantChangeTypeEx
#define VARIANT_NOVALUEPROP      0x01
#define VARIANT_ALPHABOOL        0x02 // For VT_BOOL to VT_BSTR conversions,
                                      // convert to "True"/"False" instead of
                                      // "-1"/"0"
#define VARIANT_NOUSEROVERRIDE   0x04 // For conversions to/from VT_BSTR,
				      // passes LOCALE_NOUSEROVERRIDE
				      // to core coercion routines
#define VARIANT_CALENDAR_HIJRI   0x08
#define VARIANT_LOCALBOOL        0x10 // For VT_BOOL to VT_BSTR and back,
                                      // convert to local language rather than
                                      // English

// Compare results.  These are returned as a SUCCESS HResult.  Subtracting
// one gives the usual values of -1 for Less Than, 0 for Equal To, +1 for
// Greater Than.
//
#define VARCMP_LT   0
#define VARCMP_EQ   1
#define VARCMP_GT   2
#define VARCMP_NULL 3


#define V_UNION(X, Y)   ((X)->Y)
#define V_VT(X)         ((X)->vt)
#define V_RECORDINFO(X) ((X)->pRecInfo)
#define V_RECORD(X)     ((X)->pvRecord)


/* Variant access macros
 */
#define V_ISBYREF(X)     (V_VT(X)&VT_BYREF)
#define V_ISARRAY(X)     (V_VT(X)&VT_ARRAY)
#define V_ISVECTOR(X)    (V_VT(X)&VT_VECTOR)
#define V_NONE(X)        V_I2(X)

#define V_UI1(X)         V_UNION(X, bVal)
#define V_UI1REF(X)      V_UNION(X, pbVal)
#define V_I2(X)          V_UNION(X, iVal)
#define V_I2REF(X)       V_UNION(X, piVal)
#define V_I4(X)          V_UNION(X, lVal)
#define V_I4REF(X)       V_UNION(X, plVal)
#define V_I8(X)          V_UNION(X, llVal)
#define V_I8REF(X)       V_UNION(X, pllVal)
#define V_R4(X)          V_UNION(X, fltVal)
#define V_R4REF(X)       V_UNION(X, pfltVal)
#define V_R8(X)          V_UNION(X, dblVal)
#define V_R8REF(X)       V_UNION(X, pdblVal)
#define V_I1(X)          V_UNION(X, cVal)
#define V_I1REF(X)       V_UNION(X, pcVal)
#define V_UI2(X)         V_UNION(X, uiVal)
#define V_UI2REF(X)      V_UNION(X, puiVal)
#define V_UI4(X)         V_UNION(X, ulVal)
#define V_UI4REF(X)      V_UNION(X, pulVal)
#define V_UI8(X)         V_UNION(X, ullVal)
#define V_UI8REF(X)      V_UNION(X, pullVal)
#define V_INT(X)         V_UNION(X, intVal)
#define V_INTREF(X)      V_UNION(X, pintVal)
#define V_UINT(X)        V_UNION(X, uintVal)
#define V_UINTREF(X)     V_UNION(X, puintVal)

#define V_CY(X)          V_UNION(X, cyVal)
#define V_CYREF(X)       V_UNION(X, pcyVal)
#define V_DATE(X)        V_UNION(X, date)
#define V_DATEREF(X)     V_UNION(X, pdate)
#define V_BSTR(X)        V_UNION(X, bstrVal)
#define V_BSTRREF(X)     V_UNION(X, pbstrVal)
#define V_DISPATCH(X)    V_UNION(X, pdispVal)
#define V_DISPATCHREF(X) V_UNION(X, ppdispVal)
#define V_ERROR(X)       V_UNION(X, scode)
#define V_ERRORREF(X)    V_UNION(X, pscode)
#define V_BOOL(X)        V_UNION(X, boolVal)
#define V_BOOLREF(X)     V_UNION(X, pboolVal)
#define V_UNKNOWN(X)     V_UNION(X, punkVal)
#define V_UNKNOWNREF(X)  V_UNION(X, ppunkVal)
#define V_VARIANTREF(X)  V_UNION(X, pvarVal)
#define V_ARRAY(X)       V_UNION(X, parray)
#define V_ARRAYREF(X)    V_UNION(X, pparray)
#define V_BYREF(X)       V_UNION(X, byref)

#define V_DECIMAL(X)     V_UNION(X, decVal)
#define V_DECIMALREF(X)  V_UNION(X, pdecVal)

#define VTBIT_I1        (1 << VT_I1)
#define VTBIT_UI1       (1 << VT_UI1)
#define VTBIT_I2        (1 << VT_I2)
#define VTBIT_UI2       (1 << VT_UI2)
#define VTBIT_I4        (1 << VT_I4)
#define VTBIT_UI4       (1 << VT_UI4)
#define VTBIT_I8		(1 << VT_I8)
#define VTBIT_UI8		(1 << VT_UI8)
#define VTBIT_R4        (1 << VT_R4)
#define VTBIT_R8        (1 << VT_R8)
#define VTBIT_CY        (1 << VT_CY)
#define VTBIT_DECIMAL   (1 << VT_DECIMAL)

#define VTBIT_BOOL      (1 << VT_BOOL)
#define VTBIT_BSTR      (1 << VT_BSTR)
#define VTBIT_DATE      (1 << VT_DATE)
#define VTBIT_DISPATCH  (1 << VT_DISPATCH)
#define VTBIT_EMPTY     (1 << VT_EMPTY)
#define VTBIT_ERROR     (1 << VT_ERROR)
#define VTBIT_INT       (1 << VT_INT)
#define VTBIT_NULL      (1 << VT_NULL)
#define VTBIT_UINT      (1 << VT_UINT)
#define VTBIT_UNKNOWN   (1 << VT_UNKNOWN)
#define VTBIT_VARIANT   (1 << VT_VARIANT)
#define VTBIT_15        (1 << 15)        /* no variant type with this number */

#endif /* _VARIANT_H_ */



