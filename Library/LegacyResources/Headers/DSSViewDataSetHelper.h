//
//  DSSViewDataSetHelper.h
//  ReportServiceCore
//
//  Created by Yang Zhuo on 9/21/11.
//  Copyright 2011 Microstrategy. All rights reserved.
//
#ifndef DSSVIEWDATASETHELPER_H
#define DSSVIEWDATASETHELPER_H 1

#include "DSSBinaryDataReader.h"
#include "DSSDataColumn.h"
#include "DSSTabularIndex.h"
#import "DSSTabularIndexPool.h"
#include "DSSMemoryGovernorHelper.h"  //yimliu, 2014/06/25, class the call MEM_GOVERNOR_RELEASE when it deconstructs.
class DSSXTabBitVector;
class DSSCMNodeModel;


class DSSViewDataSetHelper
{
public:
    static int loadColumn(DSSBinaryDataReader * ipReadStream, DSSDataColumn *lpColumn, bool isOnDemandLoading = false, vector<int>* ipFilterList = NULL/*ctang: partial load*/); // 5/17/2012 tehe: add on demand flag
    static int loadStringVector(DSSBinaryDataReader * ipReadStream, vector<string> &ivStrs);
    static int loadIndex(DSSBinaryDataReader * ipReadStream, DSSTabularIndex **ippIndex, DSSTabularIndexPool *ipIndexPool = NULL, bool *opIndexExist = NULL, DSSCMNodeModel *ipCMNodeModel = NULL);
    static int str2GUID(string &irVal, GUID &iGUID);
    static size_t hSizeOf(EnumDSSDataType iDataType);
    
    // 5/21/2012 tehe: partial element loading
    static int PartialLoadColumn(DSSBinaryDataReader * ipReadStream, DSSDataColumn *lpColumn, vector<int> & ivPartialEleList);
    
    //zhyang, 722280, de-serialize a int array
    static int loadCompressedIntArray(DSSBinaryDataReader * ipReadStream, vector<int> &iovArray);
private:
    static int hLoadRawDataColumn(DSSBinaryDataReader *ipReadStream, DSSDataColumn *ipColumn, EnumDSSDataType iDataType, int nElements, vector<int> &irvElementList, bool isOnDemandLoading = false); //  tehe: delay loading
    static int hLoadStringDataColumn(DSSBinaryDataReader *ipReadStream, DSSDataColumn *ipColumn, EnumDSSDataType iDataType, int nElements, vector<int> &irvElementList, bool isOnDemandLoading = false); // tehe: delay loading

    static int hDecompressUnitKeys(int *ipKeys, int &iCurrentPos, int inUnits, int iUnit, int iStartPos, int iEndPos, vector<int>  &ivUnitCompressedKey, DSSXTabBitVector &iCompressedBitVector);
    static int hLoadShortArray(DSSBinaryDataReader *ipReadStream, vector<int> &lData);
    static int hDecompressRange(vector<int> &ivCompressedData, vector<int> &ovDecompressedData);
    
    // 5/22/2012 tehe: get partial data from the buffer
    static int hPartialLoadRawDataColumn(DSSBinaryDataReader * ipReadStream, DSSDataColumn *lpColumn, EnumDSSDataType iDataType, vector<int> &irvElementList);
    static int hPartialLoadStringDataColumn(DSSBinaryDataReader * ipReadStream, DSSDataColumn *lpColumn, EnumDSSDataType iDataType, vector<int> &irvElementList);
    
    // 11/12/2012 tehe
    static int hInitKeyValue(DSSTabularIndex *ipSourceIndex, int iUnitCount, int *ipUnits, int iRowCount, int *ipRowMap, int *iopKeys);
    static int hLoadIndexKeys(DSSBinaryDataReader *ipReadStream, DSSTabularIndexPool *ipIndexPool, int inRows, int inUnits, int *ipUnits, int *iopKeys, DSSCMNodeModel *ipCMNodeModel);
};

#endif