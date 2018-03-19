//
//  DSSUCase.h
//  ReportServiceCore
//
//  Created by Yang Zhuo on 7/27/11.
//  Copyright 2011 Microstrategy. All rights reserved.
//

#ifndef CDSSUCase_h
#define CDSSUCase_h 1

#import "DSSNNSimpleFunction.h"

class DSSUCase : public DSSNNSimpleFunction  
{
public:
    DSSUCase();
    ~DSSUCase();
    int Calculate (Int32 nSize, CComVariant *pVec, CComVariant *opResult, DSSDataFlag *pFlag);
    virtual int setProperty (Int32 Index, CComVariant *pValue);
    int GetOutputType (Int32 size, EnumDSSDataType *inputType, EnumDSSDataType *outputType);
    int Calculate(int nParam, double *ipParams, double *opResult, DSSDataFlag *opFlag);
    
    // 11/19/2012 tehe
    int SetStringCompareOption(VARIANT_BOOL iOpt);
    //tliao, 899309, 5/26/2014, it's a Non-Numerical function only
    virtual bool IsNNFunctionOnly() {return true;}
private:
    //zhyang, 28/07/2011, This is done in the cpp file.
    //If use function call, I don't know why it doesn't work
	// This function converts a BSTR to all lower case IN PLACE
	/*inline void BSTRToLower(BSTR str)
	{
        //zhyang, 27/07/2011, In the client side, it is encoded with UTF-8
        //So I think we should use strlen to count the length currently
        //!!!!This need to be enhanced!!!!
//        Int32 len = (str) ? SysStringLen(str) : 0;
        Int32 len = str ? strlen((char *)str) : 0;
        //Int32 len = str ? strlen(str) : 0;
        for (Int32 i = 0; i < len; i++)
        {
            //str[i] = towlower(str[i]);
            str[i] = tolower(str[i]);
        }
	}
    
    inline void BSTRToUpper(BSTR& str)
    { 
        //Int32 len = (str) ? SysStringLen(str) : 0;
        char *lpStr = (char *)&str;
        Int32 len = str ? strlen(lpStr) : 0;
        for (Int32 i  = 0; i < len; i++)
        {
           // str[i] = towupper(str[i]);
            lpStr[i] = toupper(lpStr[i]);
        }
    }*/
    bool mfTrimString;
};

#endif
