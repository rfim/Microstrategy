//
//  DSSLikeFunc.h
//  ReportServiceCore
//
//  Created by Yang Zhuo on 7/26/11.
//  Copyright 2011 Microstrategy. All rights reserved.
//


#ifndef CDSSLikeFunc_h
#define CDSSLikeFunc_h 1

#import "CalculationEngine/DSSNNSimpleFunction.h"

class DSSLikeFunc : public DSSNNSimpleFunction  
{
public:
    DSSLikeFunc();
    ~DSSLikeFunc();
    int Calculate (Int32 nSize, CComVariant *pVec, CComVariant *opResult, DSSDataFlag *pFlag);
    virtual int setProperty (Int32 Index, CComVariant *pValue);
    
    // int Init (EnumDSSFunction iFunctionIndex);
    int GetOutputType (Int32 size, EnumDSSDataType *inputType, EnumDSSDataType *outputType);  
    
    //zhyang, 26/07/2011, the pure vitual function in DSSSimpleFunction, we need to impelment it.
    int Calculate(int nParam, double *ipParams, double *opResult, DSSDataFlag *opFlag);
private:
    class RegExpr
    {
    public:
        //zhyang, 26/07/2011, change the wchar_t to char
//        RegExpr(const wchar_t *pattern, MBase::Buffer *pBuffer);
        RegExpr(const char *pattern);
        ~RegExpr();
        
//        bool match(const wchar_t *str);
        bool match(const char *str);
        
        enum { next_char = 	-1};
        
        enum {wild_card = -1};
        
        
    private:
        struct State
        {
            State():the_char(0), next1(0), next2(0) {}
            State(Int32 c, Int32 n1, Int32 n2 ):the_char(c), next1(n1), next2(n2) {}
            
            Int32 the_char;
            Int32 next1, next2;
        };
        typedef vector<State *> StateVector;   
        StateVector *mpAutomaton;
        
    private:
        //Int32 simulate(const wchar_t *str, Int32 j);
        Int32 simulate(const char *str, Int32 j);
    };
    
    
};



#endif
