//
//  DSSXTabRecursiveLookupTable.hpp
//  ReportServiceCore
//
//  Created by Sun, Mingmin on 3/16/16.
//  Copyright © 2016 MicroStrategy, Inc. All rights reserved.
//

#ifndef DSSXTabRecursiveLookupTable_h
#define DSSXTabRecursiveLookupTable_h

#include <stdio.h>

// CDSSRecursiveUnit
#include "DSSXTabRecursiveUnit.h"

class DSSBinaryDataReader;
class DSSXTabRecursiveLookupTable : public DSSXTabRecursiveUnit
{
public:
    DSSXTabRecursiveLookupTable();
    
    virtual int getArity();
    virtual int Count();
    virtual int Item(int iForm, DSSDataColumn **oppColumn);
    virtual int getWHNullPosition();
    virtual void setWHNullPosition(int iPosition);
    virtual int getBaseFormID(int iForm);
    //to support transaction data
    virtual int setData(int iBaseFormID, int iRow, char *ipData, int iSizeInByte);
    virtual int setData(int iBaseFormID, int iOffset, MBase::CString &irText);
    virtual int CountRows();
    virtual int FindFormWithLocale(int iAttribute, int iBaseFormID, int iLocale, int *opColumn);
    virtual int FindFormWithLocale(int iAttribute, int iBaseFormID, int iLocale, int &orColumn);
    virtual int	createElement(int key, DSSObjectContext *ipObjectContext, DSSBaseElementProxy **oppElement, int iGlobalKey=-1, int iGlobalKeyVer = -1);
};

#endif /* DSSXTabRecursiveLookupTable_h */
