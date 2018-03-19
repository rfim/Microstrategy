//
//  DSSSliceCorrelator.h
//  ReportServiceCore
//
//  Created by Taimin Liao on 2/27/13.
//  Copyright (c) 2013 MicroStrategy, Inc. All rights reserved.
//

#ifndef ReportServiceCore_DSSSliceCorrelator_h
#define ReportServiceCore_DSSSliceCorrelator_h

#include "DSSAEHelper.h"

class DSSTabularIndex;
class DSSCubeDataSlice;
class DSSDataColumn;
class DSSCube;

class DSSSliceCorrelator
{	
public:
	DSSSliceCorrelator();
	~DSSSliceCorrelator();
	int Init(DSSTabularIndex* ipLookupIndex, Int32 inInputs, DSSCubeDataSlice **ippInputSlices, DSSCubeDataSlice *ipOutputSlice, DSSCube* ipCube, bool iEnsureRowMap = false);//, MBase::Buffer *ipBuffer = NULL);
	int GetInputValue(Int32 iSlice, Int32 iRow, double *opValue, DSSData_Flags *opFlag);
	int SetOutputValue(Int32 iInputRow, double iValue, DSSData_Flags iFlag);
	int GetInputValue(Int32 iSlice, Int32 iRow, BYTE **pData, Int32 cbSize, Int32 *pcbWritten, DSSDataType_Type *pDataType, DSSData_Flags *pFlags);
    
	int SetOutputValue(Int32 iInputRow, BYTE *pData, Int32 cbSize, EnumDSSDataType Type, DSSData_Flags Flags);
private:
	vector<Int32*> mvRowMap;
	vector<DSSDataColumn*> mvColumn;
	vector<DSSTabularIndex*> mvIndex;	
	vector<Int32> mvRefUnit;		// if it's RefColumn, this vector stores the unit it references
    
	DSSTabularIndex* mpLookupIndex;
	Int32 *mpCurrentKeys;
	Int32 *mpTempKeys;
	Int32 mnTotalUnits;
	Int32 mCurrentRow;
	Int32 mOutputSlicePos;
    
	int GetRow(Int32 iSlice, Int32 iRow, Int32 &orRow);
};

#endif
