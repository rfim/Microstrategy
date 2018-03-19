/*
 *  DSSRWDataModel.h
 *  ReportServiceCore
 *
 *  Created by Xiaolong Wang on 9/26/11.
 *  Copyright 2011 MicroStrategy, Inc. All rights reserved.
 *
 */

#ifndef RWDataModel_H
#define RWDataModel_H

#import "DSSRWDHelper.h"
#import "DSSBaseString.h"
#include "Base/Base/Allocator.h"
#include <map>
#include <set>
class DSSBinaryDataReader;
class DSSRWNodeModel;
class DSSBaseElementProxy;
class DSSObjectContext;
class DSSGUnitContainer;
class DSSTabularUnit;
class DSSTabularAttribute;
class DSSRWDataModel;
class DSSRWNode;
class DSSDocumentInstance;
class DSSTabularIndex;
class DSSRelation;

class DSSFormat;
struct GroupbyElement;

class DSSRWIterator;
class DSSRWGridIterator;
class DSSRWSectionModelIterator;
class DSSCMDataModel;
class DSSXTabBitVector;
class MCESchema;
class DSSRWViewEvaluation;
class DSSViewDataSet;
class DSSTemplate;
class DSSTabularMetrics;
class DSSThreshold;
//xiazhou, offline UC
class DSSGroupByUnit;
class DSSMaskInfos;
using MBase::Buffer;
class IndexMask;
class DSSCube;
class NodeAndControls;

//zhyang, 10/13/2011
//This must be the same as the server side.
//We have to move it to a more suitable place later.
//zhyang, 9/27/2011
typedef enum EnumNewFlexBlockType
{
	EnumNewBlockReserve				=	0,
	EnumNewBlockGirdModel			=	101,
	EnumNewBlockFieldGroupModel		=	201,
	EnumNewBlockDataSet				=	301,
	EnumNewBlockDSSID				=	401,
	EnumNewBlockCube				=	501,
	EnumNewBlockCalculationPlan		=	601,
	EnumNewBlockEvents				=	701,
	EnumNewBlockEventLevels			=	801,
	EnumNewBlockEventMetrics		=	901,
	EnumNewBlockProperties			=	1001,
	EnumNewBlockIndexPool			=	1101,
	EnumNewBlockNGBSectionModel		=	1201,
	EnumNewBlockCGBSectionModel		=	1301,
	EnumNewBlockUCSectionModel		=	1401,
	EnumNewBlockControlModel		=	1501,
    EnumNewBlockSectionModel		=	1601,
    //ctang NGBPath block
    EnumNewBlockNGBPathInfo         =   1701,
    EnumNewBlockGUnitContainerHeader    =   1801,
    EnumNewBlockGlobalLookUpHeader  =   1901,
    EnumNewBlockGUnitMapHeader      =   2001,
    EnumNewBlockGUnitContainerData  =   2101,
    EnumNewBlockGlobalLookUpData    =   2201,
    EnumNewBlockGUnitMapData        =   2301,
    EnumNewBlockRefColumn           =   2401,
    //xzeng DE block
    EnumNewBlockDE                  =   2501,
    EnumNewBlockDERelated           =   2601,
    EnumNewBlockDataModelNullOnTop  =   2701,
    EnumNewBlockFieldGroupTotalDisplay  =   2801,
    EnumNewBlockFieldGroupNullDisplay   =   2901,
    //ctang: binary context, now only relations
    EnumNewBlockBinaryContext       =   3001,
    EnumNewBlockAutoText            =   3101,
    EnumNewBlockControlModelElementList = 3201,
    EnumNewBlockBaseDatasetNullReplacement = 3301,
    EnumNewBlockDDICModel           =  3401,

    //xzeng, 2013-04-22, hide null or zero
    EnumNewHideNullOrZero               = 3501,
    EnumNewBlockIndex                   = 3601,
    EnumNewBlockIndexKey                = 3701,
    EnumNewBlockNewIndicesInfo          = 3801,
    EnumNewBlockOptionalSelectElement   = 3901,
    EnumNewBlockOptionalSelectUnit      = 4001,

    //xzeng Input order block
    EnumBlockAttributeInputOrderV1      = 4101,
    EnumNewBlockGroupByElements         = 4201,

    EnumNewBlockPrimaryFlagOfDatasets = 4301,
    EnumNewBlockBaseDataSets        =  4401,
    EnumNewBlockBaseDataSet         =  4501,
    EnumNewBlockMCESchema           =  4601,
    EnumNewBlockTransactionCSI      =  4701,
    EnumNewBlockDSSIDChild1         =  4801,
    EnumNewBlockNewColumnInfo       =  4901,
    EnumNewBlockSliceColDivider     =  5001,
    EnumNewBlockAttrGMapData        =  5201,
    EnumNewBlockCSIs                =  5301,
    EnumNewBlockNewDADE             =  5401,
    EnumNewBlockCalculationPlanExtemeInfo = 5501,
    EnumNewBlockDynamicDimty        =  5701,
    EnumNewBlockAttributeRank       =  5901,
    EnumNewBlockLocalLookupTable	=  6001,
    EnumNewBlockControlModelElementAttributeList    = 6101,
    // mchen, for recursive attribute
    EnumNewBlockRecursiveUnit		=	6201,
    EnumNewBlockDisplayMainIndex	=	6301,
    // jzeng, for serialize attribute threshold
    EnumNewBlockAttributeThresholds	=   6401,
    // financial report
    EnumNewBlockControlModelParentElementList       = 6501,
    EnumNewBlockControlModelRALevels  = 6601,
    EnumNewBlockControlModelRAExpandStatuses    = 6701,
    EnumNewBlockControlModelRAChildrenStatuses      = 6801,
    EnumNewBlockParent4GLU = 6901,
    EnumNewBlockIndexHiddenMask = 7001,
    EnumNewBlockRecursiveElementIDs = 7101
}EnumNewFlexBlockType;

//zhyang, 9/27/2011
typedef enum EnumNewFlexObjectType
{
	NewFlexObjectReserve = 200,
	NewFlexObjectGridModel,
	NewFlexObjectFieldGroupModel,
	NewFlexObjectGroupbyModel,
	NewFlexObjectControlNodeModel,
	NewFlexObjectSectionModel,
	NewFlexObjectDataSet,
	NewFlexObjectCalculationPlan,
	NewFlexObjectCube,
	NewFlexObjectIndexPool,
	NewFlexObjectIndex,
	NewFlexObjectSlice,
	NewFlexObjectNGBSectionModel,
	NewFlexObjectCGBSectionModel,
	NewFlexObjectUCSectionModel,
	NewFlexObjectControlModel,
    NewFlexObjectRelation,
    NewFlexObjectGBTable,
    NewFlexObjectEmptyNodeModel,
	//more type here
    NewFlexObjetSeprateDDIC,
}EnumNewFlexObjectType;

typedef enum EnumNewRWDBlockType
{
    EnumBlockRWDBinary      = 20001,
    EnumBlockHeadInfo       = 30001,
    EnumBlockBlobalLU       = 30002,
    EnumBlockNewRWDDataModel = 30003,
    EnumBlockLUDetail       = 30004,
    EnumBlockCellFormat     = 30005,
    EnumBlockNewRWDNodeModels = 30006,
    EnumBlockBaseDataSets   = 30007
}EnumNewRWDBlockType;


//junwang, 10/13/2011
class NGBPathInfo
{

    typedef struct SpecialElement{
        MBase::CString displayText;
        DSSBaseElementProxy *element;
        // tehe: initialize element
        SpecialElement():element(NULL)
        {

        }

    }SpecialElement;

public:
    NGBPathInfo();
    ~NGBPathInfo();

    void buildNGBIndexHash();
    int  GetNGBKey(int iRow, int iLevel, int* opKey);
    //tliao, 05/28/2013
    bool containSameKey(int iRow1, int iRow2, int iLevel);
    int GetRowCountByNGBPath(int iRow, int iLevel, int &oRowCount);

    int  GetRowsByNGBPath(int iRow, int iLevel, vector<int>& ovRows);
    int  GetIndicesByNGBPath(int iRow, int iLevel, vector<int>& ovIndices);
    int GetAllText(MBase::CString &orText);//display text for all
	DSSBaseElementProxy *GetAllElement();

    int GetElements(const vector<int>& ivIndex, const int iUnit, vector<DSSBaseElementProxy*>& ovElements);
    int GetElementsByRows(const vector<int>& ivRows, const int iUnit, vector<DSSBaseElementProxy*>& ovElements);
    int GetElementTexts(vector<int>& irvIndices, int unit, vector<string> &ovTexts);
    void setDataModel(DSSRWDataModel* ipModel)
    {
        mpDataModel = ipModel;
    }

    int loadFromBinary(DSSBinaryDataReader *ipReadStream);
    string GetNGBNodeKey(int iUnit);

    int toString(std::ostream& out);

    //junwang,01/10/2012,
    int getNGBSelectedRow(int iLevel);
    void setNGBSelectedRow(int iLevel, int iRow);

    int GetNGBUnitCount(){return mNGBCount;}
    DSSBaseElementProxy* getSpecialElement(int i);
    int GetSpecialElementCount();
    std::string getSpecialElementText(int i);
    int GetElementCount(int iUnit);
    GroupbyElement* GetRootElement();

    //compare two GB paths with row index a & b
    bool operator() (int a, int b);

    // 11/7/2012 tehe
    int getNGBNodeIndex(string nodekey);

    //hyan,2/22/2012
    inline int getNGBIndexRowCount()
    {
        return mnNGBIndexRow;
    }

	int getMaxElementIndex(int iUnit);

    void clearUnitElements();

    // jimzhang, Jan/14/2014, TQMS 831468, this function is called by hRefreshAfterLoad() to remove rows which contains compound NULL.
    int RemoveFromNGBIndex(const vector<int> &irRemovedRows);

    vector<string> mvNGBNodeKey;
    vector<bool> mvHasTotal;
	int mNGBCount;
	int mnNGBIndexRow;
	vector<int> mNGBIndex;  //size = mNGBCount*mnNGBIndexRow;
	vector<int> mNGBIndexHash;

protected:

    vector<SpecialElement> mvSpecialElements;//like subtoal and NULL
    vector<int> mvMaxIndexForUnit;
    vector< vector<DSSBaseElementProxy *> > mvUnitElements;

    //junwang, 01/10/2012, cache parent NGB selection for query by child NGB
    vector<int> mvCurrentSelectedRow; // 1-based, 0 means all

    int mlAllElementOffset;
	DSSBaseElementProxy *mpAllElement;

    DSSRWDataModel* mpDataModel;

    GroupbyElement* mpRootElement;

};

struct NGBViewInfo
{
	DSSViewDataSet* mpViewDataSet;
	vector<IndexMask*> mvIndexMasks;
	bool mViewPrimary;
	Int32 mSourceIndex;

	DSSGUnitContainer* mpContainer;
};

class NGBViewsJoinOrderSort
{
private:
	Int32 hGetIndexWeight(const NGBViewInfo iView1);
	NGBViewInfo* mpViews;
public:
	NGBViewsJoinOrderSort(NGBViewInfo* ipViews) : mpViews(ipViews) {}
	bool operator() (const Int32 iIndex1, const Int32 iIndex2);
};

typedef std::map<int, int, less<int> > IntToIntMapType;

class IntVectorCompare
{
private:
	vector<bool>* mpbUseMap;
	vector< vector<Int32, MBase::Allocator<Int32> >* >* mpOrderVector;
	vector< IntToIntMapType* >* mpOrderMap;
public:
	void Init(vector<bool>* ipbUseMap,
			  vector< vector<Int32, MBase::Allocator<Int32> >* >* ipvOrderVector,
			  vector< IntToIntMapType* > * ipvOrderMap)
	{
		mpbUseMap = ipbUseMap;
		mpOrderVector = ipvOrderVector;
		mpOrderMap = ipvOrderMap;
	}
	bool operator()(const vector<Int32>& lv, const vector<Int32>& rv) const
	{
		assert(lv.size() == rv.size());

		for (Int32 i = 0; i < lv.size(); i++)
		{
			if (lv[i] != rv[i])
			{
				if (lv[i] < 0 || rv[i] < 0)
				{
					if (lv[i] == -100) return false;
					if (rv[i] == -100) return true;
					return false; // for any any elements
				}

				//if (lv[i] < rv[i]) return true;
				//else return false;
				Int32 left = -1;
				Int32 right = -1;
				if ((*mpbUseMap)[i])
				{
					left = (*mpOrderMap)[i]->find(lv[i])->second;
					right = (*mpOrderMap)[i]->find(rv[i])->second;
				}
				else
				{
					left = (*(*mpOrderVector)[i])[lv[i]];
					right = (*(*mpOrderVector)[i])[rv[i]];
				}
				return left < right;
			}
		}

		return false;
	}
};

//xiazhou, 09/04/2014
//A document level object that records auto-pick selections of "unset" CGB.
//Used when user needs to keep all targets of a CGB consistent.
//For a unset CGB that has multiple targets T1, T2, ... Tx, only one (Primary) target will do
//auto-pick according to selector's setting. The rest of targets just use Primary's selections.
class AutoSelectionMap{
    typedef std::pair<string, vector<int> > TargetSelectionPair;
public:
    int updateMap(string iSelectorKey, string iTargetKey, const vector<int> &ivSelections);
    bool isPrimaryTarget(string iSelectorKey, string iTargetKey);
    bool hasDefaultSelections(string iSelectorKey) {
        return !(mSelector2SelectionMap.find(iSelectorKey) == mSelector2SelectionMap.end());
    }
    vector<int> getDefaultSelections(string iSelectorKey);
    
private:
    std::map<string, TargetSelectionPair> mSelector2SelectionMap;
};

class DSSRWDataModel
{
public:

	DSSRWDataModel();

	~DSSRWDataModel();

	int Init(DSSDocumentInstance* ipDocumentInstance);

	/*!
	 @method
	 @abstract   the entry point to load all data node models
	 @discussion
	 */
	int LoadDataFromBinary(DSSBinaryDataReader *ipReadStream, int iFlag1, int iFlag2);
    int LoadNGBPathInfo(DSSBinaryDataReader *ipReadStream);
    int LoadDataModelProperties(DSSBinaryDataReader *ipReadStream);
    int LoadBaseDatasetNullDisplay(DSSBinaryDataReader *ipReadStream);
    int LoadBaseDatasetsFromBinary(DSSBinaryDataReader *ipReadStream);

	/*!
	 @method
	 @abstract   return the unit's index in global lookup table list
	 @discussion
	 */
	int FindUnit(GUID &irUnitID);

	/*!
	 @method
	 @abstract   look for ipElment from global lookup table; return the element key if it's found
     otherwise, return a negaive value;
	 @discussion
	 */
	int FindElementKey(GUID & irUnitID, DSSBaseElementProxy* ipElement);

    //junwang, the following interfaces are needed by sectionModelIterator
    DSSRWNodeModel* getNodeModel(const string& iKey);

    DSSRWNodeModel* getLatestNodeModel(const string &iKey);
    // 8/3/2012 tehe
    int getLatestNodeModel(const string &iKey, DSSRWNodeModel * &opNodeModel);

    int GetUnset()
    {
        return mfUnset;
    }

    /*
     @method
     @abstract look for ipElement in iGlobalKeys and store the local index in opLocalIndex;
     return S_OK if it's found and return S_FALSE otherwise;
     */
    int LookForElement(DSSBaseElementProxy* ipElement, GUID& iObjectID, EnumDSSObjectType iObjectType, const vector<int>& iGlobalKeys, int* opLocalIndex);

    //junwang, 04/19/2012
    int GetGBElementGlobalKey(GUID& iObjectID, EnumDSSObjectType iObjectType, DSSBaseElementProxy* ipElement, int* opKey);

    //old logic, to be removed
    int GetGBElementGlobalKey(GUID* ipID, DSSBaseElementProxy* ipElement, int* opKey);

    NGBPathInfo* getNGBPathInfo(const string& iNodeKey);

    int GetNGBKey(const string& iNodeKey, int iRow, int iLevel, int* opKey);

    int GetRowsByNGBPath(const string& iNodeKey, int iLevel, int iRow, vector<int>& ovRows);

    //tliao, 05/28/2013
    int SetNGBWindow(const string& iNodeKey, int iStart, int iEnd);
    int ClearNGBWindow(const string& iNodeKey);
    bool ContainsNGBWindow(const string& iNodeKey);

    int IsCompoundNull(GUID& iUnitID, int iGlobalKey, bool* opIsCompoundNull);

    int CompareElement(DSSBaseElementProxy* pElement, int iGlobalIndex, EnumDSSObjectType iObjectType, GUID& iObjectID, bool* opMatch);

    int get_PrimaryNodeIndex(int iLevel);

    bool get_RemoveNullProperty()
    {
        return  mfRemoveNullGroup;
    }

    DSSObjectContext* getObjectContext()
    {
        return mpObjectContext;
    }

    void clearGUnitContainer();
    //hyan, get the TabularAttribute from Global Lookup  by GUID
    DSSTabularAttribute* GetTabularAttributeByGUID(GUID iGUID);

    //junwang, get the lookup table from Global lookup by NGBNode Key, 12/07/2011
    DSSTabularAttribute* GetLookUpByNGBNodeKey(const string& iNGBNodeKey);



    int GetGlobalLookupContainer(DSSGUnitContainer ** oppGlobalLookupContainer);

    Int32 GetTrimEmptySections()
    {
        return mfTrimEmptySections;
    }

    int GetUnitIndexByGUID(GUID* ipGUID, Int32* opIndex);
    int GetUnitGUIDByIndex(int iIndex, GUID* opGUID);


    int BindModel();

    //junwang, get tabularUnit from GUnitContainer by GUID and global key
    int GetTabularUnit(GUID& iGUID, int iGlobalKey, DSSTabularUnit** oppUnit, int *oLocalKey,bool* opDE = NULL);

	const vector<MBase::CString> & getTotalDispForFG();
	const vector<MBase::CString> & getNullDispForFG();

    //junwang, 12/28/2011
    bool GetDisplayNullOnTop()
    {
        return mfDisplayNullOnTop;
    }

    DSSRelation* getSchemaRelation(GUID& iID1, GUID& iID2);
    DSSRelation* getDataSetRelation(GUID& iID1, GUID& iID2);

    // tehe 2/3/2012 get all schema relations
    int getSchemaRelations(vector<DSSRelation*> &ivRelaitons);

    //zhyang, 01/11/2012
    int updateNodeModel(const string &irkey, bool &orbUpdated);

    //zhyang, 02/08/2012
    int getAutoText(string iKey, string &ostr);

    inline bool getNullChecking() { return mfNullChecking; }


    //hyan,2/9/2012
    int getAutoText(GUID &iGUID,string iName,string &ostr);

    DSSDocumentInstance* getDocumentInstance()
    {
        return mpDocumentInstance;
    }




    //hyan,2012/03/07,add for get default formatting
    //2012/03/21,move from FieldGroupModelIterator to here
    DSSFormat* getFormatUsingFormatStr(string iFormatStr);


    //hyan,2012/03/26
    DSSFormat *getDefaultFormatOfCF()
    {
        return mpDefaultFormatOfCF;
    }

    //hyan,11/1/2012
    string getJoinNullDisplay()
    {
        return mJoinNullDisplay;
    }


    //hyan,2012/03/29
    map<int, string>* getDisplayTextMap(string iNodeKey);

    map<int,DSSBaseElementProxy*>* getElementProxyMap(string iNodeKey);

    // 5/17/2012 tehe
    int setDataSourcePath(const char *ipBinaryFilePath, MBase::CString iSubFolder);
    MBase::CString getDataSourcePath();

    //2012/07/24,hyan
    int getNullDisplay(GUID &iGUID,DSSDataFlag iNullFlag,string &oNullDisplayStr);

    inline const vector<vector<string> > & getNullDisplay() { return mvvNullDisplay; }

    inline bool getTrimString() { return mbTrimString; }

    // 11/22/2012 tehe
    inline MBase::CString getDocumentID() {return mDocumentID;}

    // 1/28/2012 tehe: support node key to iterator
    void addNodeKey2Iterator(MBase::CString iNodeKey, DSSRWIterator *ipIterator);
    void removeNodeKey2Iterator(MBase::CString iNodeKey, DSSRWIterator *ipIterator);

    inline MCESchema *getSchema() { return mpSchema; }

	/*!
	 @method    EvaluateAllViews
	 @abstract  For each view temlate in the document, do
	 @discussion
	 */
    int EvaluateAllViews(bool isSmartClientOffline = false);
    int EvaluateViews(vector<std::string> &irvNodeKeys);
    int GetViewEvaluators(vector<std::string> &irvNodeKeys, vector<DSSRWViewEvaluation*> &orvViewEvaluator);
    int EvaluateView(std::string &irNodeKey, int iViewIndex);
//    xiazhou, 04/23/2014. Add isOfflineUC to support offline UC.
     int CalculateTemplate(DSSRWNodeModel* ipNodeModel, DSSViewDataSet* ipViewDataset = NULL, bool isOfflineUC = false);

//    Offline UC. Collect GBUnits in UC on this node's path
//    @param irNodeKey : node key.
//    @param oGBUnitMap : a collection of UC GBUnits on the path. Use GBUnit's DSS_ID as key.
    int collectInformation(std::string &irNodeKey, map<DSS_ID, DSSGroupByUnit *> &oGBUnitMap);

//    Offline UC. Evaluate nodes that are affected by UC manipulation.
//    @param irvNodeKeys : vector of node keys.
    int ReevaluateViewsForUC(vector<std::string> &irvNodeKeys);

//    Offline UC. Evaluate a node that affected by UC manipulation.
//    @param irNodeKey :  node key.
    int ReevaluateViewForUC(std::string &irNodeKey);

//    Offline UC. Evaluate a section node that affected by UC manipulation.
//    Iterate its child nodes to find grid/FG node.
//    @param irNodeKey : section node key.
//    @param ipNode : section node.
    int ReevaluateSectionForUC(std::string &irNodeKey, DSSRWNode* ipNode);

    void addNodeKey(std::string iNodeKey) { mvNodeKeys.push_back(iNodeKey); }

	int getGlobalLookupVer(){return mGlobalLookupVer;}
    int getGlobalLookupDataVer(){return mGlobalLookupDataVer;}
    //769298
    void InsertDDICAttr(DSSTabularAttribute* ipAttr);

    void InsertDDICIndex(DSSTabularIndex* ipIndex);

    int AnalyzeTransactionThreshold(DSSThreshold* ipThreshold, bool* opResult);
    int AnalyzeTransactionThresholdOnGrid(DSSThreshold* ipThreshold, int iRow, bool* opResult);
    int AnalyzeTransactionThresholdOnGrid(DSSThreshold* ipThreshold, int iRow, int iCol, bool* opResult);
    int getCachedIterator(string iNodeKey, DSSRWIterator** opIterator);

    bool hasMultiPassCSI();
    bool hasSchemaRelation();
    //tliao, 06/13/2014, 898249, refresh NGBPathInfo after applying delta and view evaluation
    int GenNewNGBPaths();
    
    typedef std::vector < NodeAndControls* > NodeAndControlsVector;
    const NodeAndControlsVector *targetToSource(string iTargetKey);
    AutoSelectionMap &getAutoSelectionMap(){return mAutoSelectionMap;}
	
	int ReevaluateViewForFinancialReport(std::string &irNodeKey);
    // weilin 2016/08/02 get nodekey2iterator
    multimap<MBase::CString, DSSRWIterator *> getNodeKey2Iterator();
private:
    //hyan,2012/03/21
    //for performance optimization we cache the default format of Calculation Field
    DSSFormat *mpDefaultFormatOfCF;

private:
    // 9/25/2012 tehe
    int hInitNodeModel(DSSRWNodeModel *ipNodeModel, DSSBinaryDataReader *ipReadStream, bool ibCanUpdate);
    int hLoadNodeModels(DSSBinaryDataReader* ipReadStream, DSSRWNodeModel ** oppNodeModel,int iFlag1, int iFlag2);

    int hLoadRelations(DSSBinaryDataReader* ipReadStream);

    //junwang, 18/11/2011
    int hRefreshAfterLoad();
    int hCompareLookUpElement(int iRow, DSSTabularAttribute* ipLookup, DSSBaseElementProxy* pElement, bool* opMatch);

    //zhyang 01/11/2012
    int hSetNodeModel(string key, DSSRWNodeModel *ipNodeModel, bool ibCanUpdate);

    //junwang, 02/06/2012
    int hAddSelectorToSectionTargets();

    // 9/21/2012 tehe
    void toString();

    // 1/28/2012 tehe
    void hUpdateNodeKey2Iterator(MBase::CString iNodeKey, DSSRWNodeModel *ipNewNodeModel);

    //zhyang, 28/07/2013
    int hDeleteEmptyNodeModel(const string &iKey);

    //tliao, 08/13/2013, rename to LoadBaseDatasetsFromBinary
    //int hLoadBaseDataSets(DSSBinaryDataReader *ipReadStream);

    int hAddViewEvaluator(DSSRWNodeModel *ipNodeModel, vector<DSSRWViewEvaluation*> &orvViewEvaluator);
    int hUpdateTemplateMetrics(DSSTemplate* ipTemplate, DSSRWNode* ipNode, DSSTabularMetrics* ipBaseTabularMetrics, DSSTabularMetrics* ipTabularMetrics);

    //zhyang, UC target DDIC
    int hLoadSeprateDDICModel(DSSBinaryDataReader* ipReadStream);

    //tliao, offline
    int hGetNGBIndex(NGBPathInfo* ipNGBPath,
                     Int32 iNGBNode,
                     DSSViewDataSet* ipViewDataset,
                     MBase::Buffer* ipBuffer,
                     vector<IndexMask*>& ovrIndexMasks,
                     DSSMaskInfos *ipMaskInfos);
    int hGetIndexLevels(NGBPathInfo* ipNGBPath,
                        Int32 iStart, Int32 iEnd,
                        Int32* ipNGBtoUnitMap,
                        std::vector<vector<Int32> >& ovrIndexLevels,
                        std::vector<vector<Int32> >& ovrTotalUnits);
    int hFindNodeTargetViews(vector<DSSViewDataSet*>& ovViews,
                             vector<string>& ovNodeIndex,
                             vector<Int32>& ovViewIndex,
                             vector<bool>& ovPrimary,
                             vector<DSSMaskInfos*> &ovMaskInfos);
    int hUpdateMetricRange(DSSRWNodeModel *ipNodeModel, DSSViewDataSet *ipViewDataSet, DSSCube *ipCube);
    int hReGenerateOLViewForFG(vector<std::string> &irvNodeKeys);
private:
    //hyan,2012/03/29
    //cache DisplayText and ElementProxy for Selector
    //hyan,2012/03/28
    //cache displayText and ElementProxy for perfermance optimization
    //map<int, string> mvDisplayTextMap;
    //map<int,DSSBaseElementProxy*> mvElementProxyMap;



    map<string, map<int, string>* >mvDisplayTextMapForSelectors;
    map<string, map<int,DSSBaseElementProxy*>* >mvElementProxyMapForSelectors;

private:

    DSSDocumentInstance* mpDocumentInstance;

	DSSObjectContext * mpObjectContext;

    DSSGUnitContainer* mpGlobalLookupContainer;

    Int32 mfTrimEmptySections;

    int mfUnset;

    map<string, DSSRWNodeModel*> mvNodeModels;
    map<string, DSSRWNodeModel*> mvNodeModelsLatest;

    //for NGB
    vector<NGBPathInfo> mvNGBPaths;
    map<string,int> mMapFromNGBNodeKeyToLayer;
    vector<DSSXTabBitVector*> mvpMasks;


    vector<int> mvPrimaryBranch;
    bool mfDisplayNullOnTop;

    /*	map<std::string, DSSRWNodeModel*> mNodeKeyToModelMap;

     bool mfDisplayNullOnTop;
     Int32 mfTrimEmptySections;
     Int32 mfUnset;
     bool mfNullChecking;
     */

    bool mfNullChecking;
    vector<MBase::CString> mvTotalDispForFG;
    // jimzhang Apr/10/2012
    //      (1) Element of "mvNullDispForFG" is null displayment of metric, other that attribute element.
    //      (2) The size of "mvNullDispForFG" is 4 currently, and mapping of index of "mvNullDispForFG" and null type is as below:
    //      0 : DssDataNull, 1 : DssDataUnCalculated, 2 : DssDataMissing, 3 : others
    vector<MBase::CString> mvNullDispForFG;
    //ctang: relations
    vector<DSSRelation*> mvSchemaRelations;
    vector<DSSRelation*> mvDataSetRelations;

    map<MBase::CString, MBase::CString> mvAutoText;
    bool mfRemoveNullGroup;


    // 5/17/2012 tehe
    MBase::CString mDataSourcePath;
    MBase::CString mDocumentID;

    //zhyang
    vector<vector<string> > mvvNullDisplay;

    //hyan,11/1/2012
    string mJoinNullDisplay;

    bool mbTrimString;

    multimap<MBase::CString, DSSRWIterator *> mNodeKey2Iterator;

    DSSCMDataModel *mpDataModelCacheManager;

	int mGlobalLookupVer;
    int mGlobalLookupDataVer;
    vector<MBase::CString> mvRecalculateNodeKey;

    vector<DSSTabularAttribute*> mvDDICAttr;
    vector<DSSTabularIndex*> mvDDICIndex;

    //lwang, local schema
    Buffer * mpBuffer;
    MCESchema *mpSchema;

    //tliao, node key of node models that contains CSI and need to update
    vector<std::string> mvNodeKeys;
    std::set<std::string> msLayoutKeys;
    
    //xiazhou, record metric selectors that have been updated at least once.
    //Used in offline mode. This set should be cleared before evaluate all views.
    set<std::string> msMetricSelectorUpdated;

    bool mbIsSmartClientOffline;
    
    AutoSelectionMap mAutoSelectionMap;
};

#endif
