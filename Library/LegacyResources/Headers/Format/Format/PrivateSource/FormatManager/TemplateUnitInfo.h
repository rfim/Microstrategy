//==============================================================================================
//	FILENAME	:	Dataset.h
//	AUTHOR		:	Dong Shi & Fangyao Ma
//	CREATION	:	2008-05-15
//	Copyright (C) MicroStrategy Incorporated 2008
//	All Rights Reserved
//==============================================================================================

// Updated by Li en, 2016/3/10
// formatmanager refactor, splite dataset out
// move used template units include MetricUnitInfo and CustomGroupUnitInfo into this new file

#ifndef MsiFormat_MetricUnitInfo_h
#define MsiFormat_MetricUnitInfo_h

#include "Base/Base/BaseString.h"
#include "Base/Defines/BasicTypes.h"
#include "Synch/Defines/SmartPtr.h"

class DSSTemplateMetric;
class DSSTemplate;
class DSSRWGridIterator;
class DSSTemplateCustomGroup;

namespace MsiFormat
{
    /////////////////////////////////
    
    //Unit Info Structure//
    
    ///////////////////////////// ////
    struct UnitInfo
    {
        UnitInfo()
        {
            mName = L"";
            mIndexInAxis = 0;
            mType = DssTemplateReserved;
            mIsValid = false;
        }
        
        UnitInfo(MBase::String& iName, GUID& iID, Int32 iIndex, Int32 iType) : mID(iID), mIndexInAxis(iIndex), mType(iType), mName(iName)
        {
            mIsValid = true;
        }
    public:
        MBase::GUID GetID() {return mID;}
        Int32 GetIndex() {return mIndexInAxis;}
        Int32 GetType() {return mType;}
        MBase::String GetName() {return mName;}
        Int32 mIndexInAxis;
        bool mIsValid;
    protected:
        MBase::GUID mID;
        Int32 mType;
        MBase::String mName;
    };
    
    typedef std::pair<MBase::GUID, MBase::String> ElementInfo;
    
    class DimTree;
    struct MetricUnitInfo: public UnitInfo
    {
        typedef MSynch::SmartPtr<MetricUnitInfo> Ptr;
        
        MetricUnitInfo(MBase::String& iName, MBase::GUID& iID, Int32 iIndex, Int32 iType) : UnitInfo(iName, iID, iIndex, iType)
        {
            mIsInRowAxis = false;
            mMetricNumber = 0;
        }
        
        MetricUnitInfo(DSSTemplate* ipTemplate)
        {
            mIsInRowAxis = false;
            mMetricNumber = 0;
            mIsValid = hPopulateMetricInfo(ipTemplate);
        }
        void AddOneElement(const MBase::GUID& iID, const MBase::String& iName) {mElements.push_back(std::make_pair(iID, iName));}
        void GetOneElement(MBase::GUID& orID, MBase::String& orName, Int32 iIndex)
        {
            _ASSERTE( iIndex >= 0 && iIndex < mElements.size());
            orID = mElements[iIndex].first;
            orName = mElements[iIndex].second;
        }
        GUID GetMetricID(Int32 iIndex)
        {
            _ASSERTE(iIndex >= 0 && iIndex < mElements.size());
            return mElements[iIndex].first;
        }
        DSSTemplateMetric* GetTemplateMetric(MBase::GUID& irID);
        MBase::String GetMetricName(Int32 iIndex)
        {
            return mElements[iIndex].second;
        }
        
        void GetElementNames(std::vector<MBase::String>& orElements)
        {
            orElements.clear();
            size_t lSize = mElements.size();
            for (size_t i = 0; i < lSize; i++)
                orElements.push_back(mElements[i].second);
        }
        Int32 GetElementNumber(){return (Int32)mElements.size();}
        Int32 MetricIDToTemaplateMetricIndex(GUID& irID);
        Int32 GetMemoryUsageEstimation()
        {
            Int32 lSize = sizeof(*this);
            
            Int32 lCount = (Int32)mOccupyPairSet.size();
            Int32 kSize = sizeof(std::pair<Int32, Int32>);
            lSize += lCount * kSize;
            
            lCount = (Int32)mIDToTemplateMetric.size();
            kSize = sizeof(MBase::GUID) + sizeof(DSSTemplateMetric*);
            lSize += lCount * kSize;
            
            lCount = (Int32)mElements.size();
            kSize = sizeof(ElementInfo);
            lSize += lCount * kSize;
            
            return lSize;
        }
        
        bool mIsInRowAxis;
        bool mHasMetrics;
        Int32 mMetricNumber;
    protected:
        std::vector<ElementInfo> mElements;
        std::vector<std::pair<Int32, Int32> > mOccupyPairSet; //<width, index>
        std::map<MBase::GUID, DSSTemplateMetric*> mIDToTemplateMetric;
    private:
        bool hPopulateMetricInfo(DSSTemplate* ipTemplate);
        
    };
    
    //CustomGroupUnitInfo
    typedef enum
    {//to identify custom group levels ()
        CUSTGRP_UNDEFINED = -1,
        CUSTGRP_ELEMENT = 5,
        CUSTGRP_BANDING = 7,
        CUSTGRP_INDIVIDUAL_ITEM = 11,
    }CUSTOM_GROUP_LEVEL;
    //CustomGroupUnitInfo impl
    struct CustomGroupUnitInfo: public UnitInfo
    {
        typedef MSynch::SmartPtr<CustomGroupUnitInfo> Ptr;
        
        Int32 mCustomGroupNum[2];//0 for rows, 1 for columns
        
        CustomGroupUnitInfo(MBase::String& iName, MBase::GUID& iID, Int32 iIndex, Int32 iType) : UnitInfo(iName, iID, iIndex, iType)
        {
            mCustomGroupNum[0] = mCustomGroupNum[1] = 0;
        }
        CustomGroupUnitInfo(DSSRWGridIterator* ipIterator)
        {
            mCustomGroupNum[0] = mCustomGroupNum[1] = 0;
            mIsValid = hPopulateCustomGroupUnitInfo(ipIterator);
        }
        //iAxis = 0(Row) or 1(Column)
        DSSTemplateCustomGroup* getCustomGroupInAxis(int iAxis, int iDepth, CUSTOM_GROUP_LEVEL& iLevel);//not fully implemented
        std::vector<std::pair<Int32, Int32> > getCustomGroupPositions(bool iIsRow);
        bool hasCustomGroupInRow(){return (mCustomGroupNum[0]>0);}
        bool hasCustomGroupInColumn(){return (mCustomGroupNum[1]>0);}
    protected:
        std::vector<std::pair<Int32, Int32> > mOccupyPairSet[2];				//<beginIndex endIndex> -- [bgin, end)
        std::map<Int32, MBase::GUID> mCustomGroupIndexToID[2];					//<CGIndex, ID>
        std::map<MBase::GUID, DSSTemplateCustomGroup*> mIDToTemplateCustomGroup;
        //format info pool
        std::map<Int32, std::pair<GUID, Int32> > mCustomGroupIndexToElement[2];
    private:
        bool hPopulateCustomGroupUnitInfo(DSSRWGridIterator* mpIterator);
    };
    //CustomGroupUnitInfo~
};

#endif

