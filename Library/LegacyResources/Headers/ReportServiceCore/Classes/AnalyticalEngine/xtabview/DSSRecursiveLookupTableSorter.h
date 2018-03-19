//
//  DSSRecursiveLookupTableSorter.hpp
//  ReportServiceCore
//
//  Created by Sun, Mingmin on 6/12/16.
//  Copyright © 2016 MicroStrategy, Inc. All rights reserved.
//

#ifndef DSSRecursiveLookupTableSorter_hpp
#define DSSRecursiveLookupTableSorter_hpp

#import "DSSAEHelper.h"
#include "DSSSorter.h"

class DSSSorter;
class DSSSortDefinition;
class DSSXTabHeader;
class DSSDataColumn;
class DSSTabularData;

/**
 * DSSAttributeFormSorter base class.
 *
 * @author Qiang Xue <qxue@microstrategy.com>
 */
class DSSRecursiveLookupTableSorter : public DSSSorter
{
public:
    DSSRecursiveLookupTableSorter(DSSSortDefinition* definition, EnumDSSSortType sortType);
    virtual ~DSSRecursiveLookupTableSorter();
    int sort(DSSXTabHeader* header, vector<int>& items);
    virtual bool sortUnitElements(vector<int>& elements);
    bool operator ()(int a, int b);
    //virtual int buildBucket(DSSXTabHeader* header);
    int displayRespectHierarchy(vector<int>& items);
private:
    int _unitID;
    int _formID;
    EnumDSSSubtotalsPosition _subtotalPosition;
    bool _isNullOnTop;
    bool _isAscending;
    
    DSSDataColumn* _column;
    DSSTabularData* _model;
    EnumDSSSortType _sortType;
    int _nSortedElem; //junwang, 2012/07/25, record number of sorted elements
    int _unitIndex;
    vector<int> mvUnitKeys;
    //for attribute default sort
    int _unitIDinContainer;
    int _bucketID;
    //zhyang, 777421
    bool OnlySortDisplay();
    int sortDisplay(DSSXTabHeader* header, vector<int>& items);
    int* mpGlobal2LocalRecursiveMap;
    int mGlobalRow;
};

#endif /* DSSRecursiveLookupTableSorter_hpp */
