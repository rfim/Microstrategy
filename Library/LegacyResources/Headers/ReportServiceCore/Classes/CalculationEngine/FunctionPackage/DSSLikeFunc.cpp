//
//  DSSLikeFunc.cpp
//  ReportServiceCore
//
//  Created by Yang Zhuo on 7/26/11.
//  Copyright 2011 Microstrategy. All rights reserved.
//

#include "DSSLikeFunc.h"
#import "DSSFunctionUtility.h"
#import "DFCDataConversion.h"
#import "list"
#import "vector"

DSSLikeFunc::DSSLikeFunc()
{
}


DSSLikeFunc::~DSSLikeFunc()
{
}


int DSSLikeFunc::Calculate (Int32 nParam, CComVariant *pParam, CComVariant *pResult, DSSDataFlag *pFlag)
{
  	if(!pParam || !pResult || !pFlag) 
		return E_POINTER;
    
	if ( (pParam->vt & (VT_ARRAY|VT_VARIANT)) != (VT_ARRAY|VT_VARIANT))
		return E_FAIL;
    
    
	*pFlag = DssDataOk;
	double result = 0;
	
//	wchar_t *pattern, *str;	
    char *pattern, *str;	
	int hr = S_OK;
    
	switch (mFunctionIndex) {
        case DssFuncLike:
        case DssFuncNotLike:
		{
			if(nParam != 2) 
            {
                return E_FAIL;
            }
            
			hr = getVariantString(pParam, 0, &str);
			CHECK_HR;
            
			hr = getVariantString(pParam, 1, &pattern);
			CHECK_HR;
            
	/*		trimSpace(str);
			trimSpace(pattern);
      */      
			RegExpr regExpr(pattern);
			bool tmp = regExpr.match(str);
            
			if (mFunctionIndex == DssFuncLike)
				result = tmp;
			else
				result = !tmp;
		}
            
            break;
        
        default:
            break;
            
	}
    
    
	hr = DFCDataConversion::ConvertToVariant((BYTE *)(&result), DssDataTypeDouble, 0, pResult);
	CHECK_HR;
    
	*pFlag = DssDataOk;
    
	return  S_OK; 
}

int DSSLikeFunc::setProperty (Int32 Index, CComVariant *pValue)
{
	return E_NOTIMPL;
}

//int DSSLikeFunc::Init (EnumDSSFunction iFunctionIndex)
//{
//    mFunctionIndex = iFunctionIndex;    
//	return S_OK;
//}

int DSSLikeFunc::GetOutputType (Int32 /*size*/, EnumDSSDataType * /*inputType*/,
                                                       EnumDSSDataType *outputType)
{
 	if(!outputType) 
		return E_POINTER;
    
	*outputType = DssDataTypeDouble;
	return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
//RegExpr
/////////////////////////////////////////////////////////////////////////////
DSSLikeFunc::RegExpr::RegExpr(const char *pattern)
{
    mpAutomaton = new StateVector();
	Int32 sLen = 0; 
    
	if (pattern)
    {
		//sLen = wcslen(pattern);
        sLen = strlen(pattern);
    }
    
	mpAutomaton->push_back( new State(0, 1, 1));
	Int32 lState = 1;
	for ( Int32 i = 0; i < sLen; i ++){
		switch (pattern[i]){
            case '%': //wildcard
                //empty node
                mpAutomaton->push_back( new State(0, lState + 1, lState + 2));
                
                mpAutomaton->push_back(new State(wild_card, lState, lState));
                
                lState += 2;
                
                
                break;
                
            case '*': //wildcard
                if (pattern[i + 1] == '*') { // ** is escaped as *
                    i ++;
                    mpAutomaton->push_back(new State(pattern[i], lState + 1, lState + 1));
                    
                    lState ++;
                } else { // * is a wildcard character
                    //empty node
                    mpAutomaton->push_back(new State(0, lState + 1, lState + 2));
                    
                    mpAutomaton->push_back(new State(wild_card, lState, lState));
                    
                    lState += 2;
                }
                
                break;
                
            case '_': //single wildcard
                
                mpAutomaton->push_back(new State(wild_card, lState + 1, lState + 1));
                
                lState ++;
                break;
                
            case '/': //escape
                if (pattern[i + 1] == '%' || pattern[i + 1] == '_' || pattern[i + 1] == '/') // jxie, TQMS 394022, 2010-4-29, should be i+1
                    // jxie, /% means %, /_ means _, // means /
                    i ++;
            default : //any	ss	other non-wildcard character
                mpAutomaton->push_back(new State(pattern[i], lState + 1, lState + 1));
                
                lState ++;
                break;
		}
        
	}
    
	mpAutomaton->push_back(new State(0, 0, 0));
    
}

DSSLikeFunc::RegExpr::~RegExpr(){
    
    for(vector<State *>::iterator it = mpAutomaton->begin(); it != mpAutomaton->end(); it++ )
    {
        delete *it;
    }
	mpAutomaton->clear();
}

bool DSSLikeFunc::RegExpr::match(const char *str){
    
//	Int32 m, slen = wcslen(str);
    Int32 m, slen = strlen(str);
	m = simulate(str, 0);
	if (m == slen - 1) // match finished
		return true;
	else 
		return false;
}

Int32 DSSLikeFunc::RegExpr::simulate(const char *str, Int32 j){
	Int32 lState = (*mpAutomaton)[0]->next1,
    last_match = j - 1,
//    len = wcslen(str);
    len = strlen(str);
    
	typedef list<Int32 > LongList;
    
//	LongList deque();
    LongList deque;
    
	if((*mpAutomaton)[0]->next1 != (*mpAutomaton)[0]->next2)
		deque.push_front((*mpAutomaton)[0]->next2);
    
	deque.push_back(next_char);
    
	do {
		if(lState == next_char) { //progress to next char
			if(str[j])
				j++;
			deque.push_back(next_char);
		} else if(((*mpAutomaton)[lState]->the_char == wild_card && str[j] != L'\0') ||
				  (*mpAutomaton)[lState]->the_char == str[j]) {
            
			deque.push_back((*mpAutomaton)[lState]->next1);
			if((*mpAutomaton)[lState]->next1 != (*mpAutomaton)[lState]->next2)
				deque.push_back((*mpAutomaton)[lState]->next2);
            
		} else if(!(*mpAutomaton)[lState]->the_char) {
            
			deque.push_front((*mpAutomaton)[lState]->next1);
			if((*mpAutomaton)[lState]->next1 != (*mpAutomaton)[lState]->next2)
				deque.push_front((*mpAutomaton)[lState]->next2);
            
		}
        
        
		lState = deque.front();
		deque.pop_front();
        
		if(lState == 0) {
			last_match = j - 1;	
			lState = deque.front();
			deque.pop_front();
            
		}
        
	} while(j <= len && !deque.empty());
    
	return last_match;
}

int DSSLikeFunc::Calculate(int nParam, double *ipParams, double *opResult, DSSDataFlag *opFlag)
{
    return E_NOTIMPL;
}