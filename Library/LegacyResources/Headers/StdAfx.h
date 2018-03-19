// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//
#ifndef _VE_CE
//#ifndef _UUID_T
//#define _UUID_T
//#endif

#ifndef _Msi_Variant
#define _Msi_Variant
#endif

#ifndef _Msi_WinError
#define _Msi_WinError
#endif
#import "GUID.h"

#include "PDCHeader/PDCwchar.h"

#include "PDCHeader/PDCrpcdce.h"
#include <CoreGraphics/CoreGraphics.h>
#include "PDCHeader/PDCatlbase.h"
#include "PDCHeader/PDCatlcom.h"

//#include "DSSCOMMaster/DSSCOMMaster_i.h"
#include "DSSIPhoneEnums.h"

// TODO: reference additional headers your program requires here
#else

#include "ATL/DSSAtl_Includes.h"
#include "DSSCOMMaster/DSSCOM_TLB_INC.H"
#include "XercesDOM/XercesDOM/MSXML3_i.h"

#define M_PI   3.14159265358979323846264338327950288

//#ifndef PORTING_DONE
//#define PORTING_DONE
//#endif
#endif
