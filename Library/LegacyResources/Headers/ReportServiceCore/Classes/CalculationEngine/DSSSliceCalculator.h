/*
 *  DSSSliceCalculator.h
 *  ReportServiceCore
 *
 *  Created by Xiaolong Wang on 1/25/11.
 *  Copyright 2011 MicroStrategy, Inc. All rights reserved.
 *
 */

#ifndef SliceCalculator_h
#define SliceCalculator_h 1

#import "CalculationEngine/DSSCalculationHelper.h"
#import "CalculationEngine/DSSLevelResolver.h"


#import "DSSAggregationFunction.h"
#import "PDCHeader/PDCwinnt.h"
#import "MsiCOM/MsiCOM/Msi_atldef.h"
#import "MsiCOM/MsiCOM/Msi_objbase.h"
#import "MsiCOM/MsiCOM/Msi_oleauto.h"
#import "MsiCOM/MsiCOM/Msi_ccomvariant.h"
#import "DSSXTabSortDefns.h"
#import "DSSXTabBitVector.h"

class DSSSimpleFunction;
class DSSNNSimpleFunction;
class DSSAggregationFunction;
class DSSNNAggregationFunction;
class DSSGenericFunction;
class DSSNNGenericFunction;

class DSSTabularIndex;
class DSSLevelResolver;

class DSSSliceCalculator
{
public:
	DSSSliceCalculator(CalculationContext *ipContext);
	
	~DSSSliceCalculator();
	
	int CalculateNewSlice(int *opResultSliceID);
	
private:
	typedef enum {
		eGenericUnknown = 0,
		eGenericType1,	// F(V1, V2, S) -> S'Breakby by level L.
		eGenericType2	// F(V1, V2, S) -> V' Breakby by level L. 
	} GenericFunctionType;
    
	typedef enum {
		eCountUnknown = 0,
		eCountID,	// count(A@ID)
		eCountStar,	// count("*")
		eCountOffsets // after the introduction of dictionary lookup approach for metric column, we should be able to count on the offsets instead of the value itself
	} CountType;
    
	int hApplySimpleFunc(int *opResultSliceID);
	
	int hApplyNumericalSimpleFunction(DSSSimpleFunction* pFunc, DSSTabularIndex* ipLookupIndex,
									 DSSTabularIndex *ipOutputIndex, int *opResultSliceID);
    
    int hApplyNonNumericalSimpleFunction(DSSNNSimpleFunction* pFunc, DSSTabularIndex* ipLookupIndex,
                                         DSSTabularIndex *ipOutputIndex,int *opResultSliceID,
                                         //tliao, 822148, 2014/09/23, support cell format data calculation
                                         DSSFunctionObject* pCellFmtFunc = NULL);
	
	int hApplyAggregationFunc(int *opResultSliceID);
	
	int hApplyNumericalAggregation(DSSAggregationFunction* pFunc, 
                                   DSSTabularIndex* ipLookupIndex,
                                   DSSTabularIndex *ipOutputIndex, 
                                   EnumNullCheckingOption iNullCheckingOption, 
                                   int *opResultSliceID,
                                   CountType iCountType);
	
	int hApplyNonNumericalAggregation(DSSNNAggregationFunction* pFunc,
                                      DSSTabularIndex* ipLookupIndex,
                                      DSSTabularIndex *ipOutputIndex, 
                                      EnumNullCheckingOption iNullCheckingOption, 
                                      int *opResultSliceID,
                                      CountType iCountType,
                                      //tliao, 822148, 2014/09/23, support cell format data calculation
                                      DSSFunctionObject* pCellFmtFunc = NULL);
	
    int hApplyGenericFunc(int *opResultSliceID);
    
	int hApplyNumericalGenericFunction (DSSGenericFunction *pFunc,
                                        GenericFunctionType iFuncType,
                                        DSSTabularIndex *ipLookupIndex, 
                                        DSSTabularIndex *ipBreakbyIndex,
                                        bool *ifIsVector, 
                                        vector<Int32> &ivResSliceLevel,
                                        EnumDSSDataType  iOutputDataType,
                                        int *pResultSliceId);
    
    int hApplyNonNumericalGenericFunction (DSSNNGenericFunction *pFunc,
                                           GenericFunctionType iFuncType,
                                           DSSTabularIndex *ipLookupIndex, 
                                           DSSTabularIndex *ipBreakbyIndex,
                                           bool *ifIsVector,
                                           vector<Int32> &ivResSliceLevel,
                                           EnumDSSDataType  iOutputDataType,
                                           int *opResultSliceID,
                                           //tliao, 822148, 2014/09/23, support cell format data calculation
                                           bool bSupportCellFormat);
    
    int hConvertFromVariant(CComVariant &iVariant, EnumDSSDataType iDataType,
                            unsigned char* &opResultBuf, int &oResultLen);
    
    int  hGetGenericLevels(DSSGenericFunction *ipFunc,
                           DSSNNGenericFunction *ipNNFunc,
                           vector<int> &ovTrueBreakby,
                           vector<int> &ovLookupUnits,
                           GenericFunctionType &oFuncType, bool *&ofIsVector,
                           EnumDSSDataType &oOutputDataType);
    
    int hTrueBreakByLevel(vector<int> &ivBreakbyUnit,
                          Int32 nSliceIn, DSSCubeDataSlice **ippSlices,
                          bool *ipvIsVector,
                          vector<int> &ovTrueBreakbyLevel);
    
    int hSliceLevelUnion(Int32 nSliceIn, DSSCubeDataSlice **ppSliceIn, vector<Int32> &ovUnits);
    
    int hResolveLookupUnits(vector<int> &ivInput1, vector<int> &ivInput2, bool ifHasSort, vector<int> &ovOutput);
    
	int hLoadInputDataSlices();
    
    int hGroupDataForGenericFunction(DSSTabularIndex* ipLookupIndex,
                                     DSSTabularIndex* ipBreakbyIndex,
                                     DSSXTabSortDefns* ipSortDefns,	
                                     GenericFunctionType iFuncType,
                                     vector<Int32> **& orvDataByGroup,
                                     MBase::Buffer* ipBuffer,
                                     Int32 &orBreakByRow);
                                     //DSSXTabBitVector* &orMask,
    
    int hGetTotalUnits(Int32& oTotalUnits);
	
	int hGroupData(DSSCubeDataSlice* ipSliceIn,
				   DSSTabularIndex* ipLookUpIndex,
				   DSSCubeDataSlice* ipSliceOut,
				   bool iUseStreamAgg,
				   int* &orBlockList, int &orInputRows,
				   int* &orGroupBounds, int &orOutputRows,
                   Int32* &orBlockAddress,
                   DSSXTabBitVector* &orMask,
                   bool ifUseLookupAsInput = false
				   );
    
    int hSortIndex(DSSTabularIndex *ipLookUpIndex, 
                   vector<DSSSortDefinition *> *pSortDefns,
                   DSSCubeDataSlice *pSliceIn);
    
    inline int SortedIndex(int index) { return (mpSortIndex) ? mpSortIndex[index] : index; }
    
    int hCheckDirectCalculate(DSSTabularIndex* ipLookUpIndex, 
                              bool& orUseDirectCalculate, 
                              DSSSimpleFunction* ipFuncObj);
    int hDirectCalculateForSimpleFunction(DSSTabularIndex* ipLookUpIndex, 
                                          DSSSimpleFunction* pFuncObj, 
                                          DSSCubeDataSlice* opResSlice,
                                          DSSXTabBitVector* ipMask,
                                          EnumNullCheckingOption iNullCheckingOption);
    int hCheckDirectCalculate(Int32 nSliceIn, 
                              DSSCubeDataSlice** ppSliceIn, 
                              DSSTabularIndex* ipLookUpIndex, 
                              DSSGenericFunction* ipFunc, 
                              EnumNullCheckingOption iNullCheckOption, 
                              bool& orUseDirectCalculate);
    int hDirectCalculateForGenericFunction(Int32 nSliceIn, DSSCubeDataSlice** ppSliceIn, DSSTabularIndex* ipLookUpIndex, DSSGenericFunction* ipFunc, 
                                       DSSCubeDataSlice* opResSlice, DSSXTabBitVector* ipMask, vector<Int32> ** ipvDataByGroup,
                                       Int32 inBreakbyRow, EnumNullCheckingOption iNullCheckingOption, GenericFunctionType iFuncType);
    int hGetCountType(DSSSliceCalculator::CountType &oCountType);
    
    int hCalculateFirstLast(DSSTabularIndex *ipInputIndex,
                            vector<DSSSortDefinition *> *ipSortDefns,
                            DSSCubeDataSlice *ipSlice,
                            DSSCubeDataSlice *opSlice,
                            bool iFirst);
    
    int hGetUnit(DSSSortDefinition *ipSortDefn, Int32 *opUnit);

    int hPropogateMissing(Int32 nSliceIn, DSSCubeDataSlice **ppSliceIn,
                          Int32 nUnit, Int32 *pUnit, DSSCubeDataSlice **ppSliceOut,
                          bool  &oMissingFound);
    
    int hProcessConditionalMetric(Int32 nSliceIn, DSSCubeDataSlice** ppSliceIn);
    //tliao, 822148, 2014/09/23, support cell format data calculation
    int hIsNumericalSlice(DSSCubeDataSlice *ipDataSlice, bool &obNumericalSlice, bool &obCellFormatSlice);
    
    static inline bool hIsNumerical(VARTYPE iVarType)
	{
		return (iVarType == VT_I2 || iVarType == VT_I4 || iVarType == VT_R4 || iVarType == VT_R8
                || iVarType == VT_BOOL || iVarType == VT_I1 || iVarType == VT_UI1 || iVarType == VT_UI2
                || iVarType == VT_UI4 || iVarType == VT_I8 || iVarType == VT_UI8 || iVarType == VT_INT || iVarType == VT_UINT);
	}
	//Execution evironment
	
	CalculationContext *mpContext;
	
	DSSLevelResolver mLevelResolver;    
    
    Int32 *mpSortIndex;
    
    vector<DSSCubeDataSlice*> mvSlice;
    
    Int32 mbOldRWD;
    
    bool mbDSTrx;
};



#endif
