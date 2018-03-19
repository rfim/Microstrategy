//
//  DSSAttributeThresholdItems.h
//  ReportServiceCore
//
//  Created by Zeng, Jinliang on 3/22/16.
//  Copyright © 2016 MicroStrategy, Inc. All rights reserved.
//

#ifndef DSSAttributeThresholdItems_h
#define DSSAttributeThresholdItems_h

#include <map>
#include <vector>

class DSSTemplate;
class DSSXTabView;

class DSSAttributeThresholdItems {
public:
    int init(DSSTemplate *ipTemplate, int iStartRow = -1, int iNumRows = -1);
    int init(DSSXTabView *ipView, int iStartRow = -1, int iNumRows = -1);
    bool getAttributeThresholdItem(EnumDSSAxisName iAxis, int iOrdinal, const DSS_ID& irAttributeID, int& orThresholdID);
    // input the vector of the row, output the threshold id, return the attribute threshold row number
    int getMergedAttributeThresholdItem(EnumDSSAxisName iAxis, const std::vector<int>& iOrdinal, const DSS_ID& irAttributeID, int& orThresholdID);
    
    size_t generateItemsMapKey(EnumDSSAxisName iAxis, int iOrdinal, const DSS_ID& irAttributeID);
    size_t generateItemsMapKey(EnumDSSAxisName iAxis, int iOrdinal, int irAttributeIDIndex);
    void getInfoFromMapKey(size_t iKey, EnumDSSAxisName &oAxis, int &oOrdinal, int &orAttributeIDIndex);
    int getAttributeIDIndex(const DSS_ID& irAttributeID);
    bool hasThreshold(const DSS_ID& irAttributeID);
private:
    bool getAttributeThresholdItem(EnumDSSAxisName iAxis, int iOrdinal, int irAttributeIDIndex, int& orThresholdID, int iMinThresholdID = -1);
    typedef std::map<GUID, int> IDMap;
    IDMap mIDMap;
    // to better performance, don't use map::find to get the key, use std::lower_bound instead
    class RowToAttributeThreshold {
    public:
        void addItem(size_t iKey, int iThresholdID);
        // find the threshold ID based on the input key; iMinThresholdID is used to optimization, if not -1, ignore the same or bigger threshold
        int find(size_t iKey, int iMinThresholdID = -1);
        // find the least threshold ID from the input keys, and return the row key of that threshold
        int find(const std::set<size_t>& iOrderedKeys, size_t& oKey);
        void sort();
        void clear();
        bool empty();
    private:
        typedef std::vector<size_t> RowKeys;
        std::vector<RowKeys> mThresholdInfos;
        bool mIsSorted;
    } mItems;
};

#endif /* DSSAttributeThresholdItems_h */
