/*
 *  DSSModelPopulation.h
 *  DashboardViewer
 *
 *  Created by Xiaolong Wang on 1/2/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#import "DSSBinaryDataReader.h"
#import "DSSAEHelper.h"

enum EnumDataBlockType
{
	EnumDataBlockReserved = 0,
	EnumDataBlockAttributeV1 = 101,
	EnumDataBlockAttributeFormsV1 = 201,
	EnumDataBlockAttributeElementsV1 = 301,
	EnumDataBlockConsolidationV1 = 401,
    EnumDataBlockAttributeInputOrderV1 = 3701, //junwang 09/02/2011, Red Rocket: issue 500695
	EnumDataBlockCustomGroupV1 = 501, 
	EnumDataBlockMetricV1 = 601,
	EnumDataBlockXTabModelV1 = 701,  
	EnumDataBlockXTabModelV2 = 702,  
	EnumDataBlockXTabModelUnitsV1 = 801,  
	EnumDataBlockXTabModelSubtotalsV1 = 901,  
	EnumDataBlockXTabModelNullsV1 = 1001,  
	EnumDataBlockXTabModelLayoutV1 = 1101,  
	EnumDataBlockXTabModelViewsV1 = 1201, 
	EnumDataBlockXTabModelViewsV2 = 1202,
	EnumDataBlockFieldGroupModelV1 = 1301,
	EnumDataBlockFieldGroupModelUnitsV1 = 1401,
	EnumDataBlockFieldGroupModelFieldsV1 = 1501,
    EnumDataBlockFieldGroupModelFieldsV2 = 1502,
	EnumDataBlockControlNodeModelV1 = 1601,
	EnumDataBlockControlNodeModelUnitsV1 = 1701,
	EnumDataBlockControlNodeModelUnitsV2 = 1702,
	EnumDataBlockControlNodeModelElementsV1 = 1801,
    EnumDataBlockControlNodeModelElementsV2 = 1802,
	EnumDataBlockControlNodeModelRowsV1 = 1901,				
	EnumDataBlockGroupbyModelV1 = 2001,
	EnumDataBlockGroupbyModelUnitsV1 = 2101,
	EnumDataBlockGroupbyModelTextsV1 = 2201,
	EnumDataBlockGroupbyModelElementsV1 = 2301,
	EnumDataBlockXTabModelCellFormatsV1 = 2401,
	EnumDataBlockGlobalModelV1 = 2501,
	EnumDataBlockGlobalModelV2 = 2502,
	EnumDataBlockControlNodeModelFlagV1 = 2601,
	EnumDataBlockMetricSubtotalV1 = 2701,
	EnumDataBlockFieldGroupModelStaticFieldsV1 = 2801,
	EnumDataBlockXTabModelSliceInfoV1 = 2901,
	EnumDataBlockControlNodeModelRangeV1 = 3001,
	EnumDataBlockXTabModelCalculationPlanV1 = 3101,
	EnumDataBlockXTabModelEventsV1 = 3201,
	EnumDataBlockXTabModelEventMetricsV1 = 3301,
	EnumDataBlockXTabModelEventLevelsV1 = 3401,
	EnumDataBlockXTabModelPropertiesV1 = 3501,
	EnumDataBlockFieldGroupModelRawDataV1 = 3601,
    EnumDataBlockFieldGroupModelFieldsOffsetV1 = 3701,
    EnumDataBlockInputControlModelV1 = 3901,
    EnumDataBlockXTabModelLevelInfoV1 = 4001,
	EnumDataBlockDocumentV1 = 10001,
	EnumDataBlockGlobalLookUpTable = 130,
	EnumDataBlockRefColumn = 140,
    //these two should not be here, moved to DSSRWDataModel.h
	//EnumNewBlockDE = 2501,             //xzeng, load DE
	//EnumNewBlockDERelated = 2601,      //load DE Related
};

enum EnumBinaryObjectType
{
	EnumObjectReserved = 0,
	
	EnumObjectByte = 51,
	EnumObjectUnsignedByte = 52,
	EnumObjectInteger = 53,
	EnumObjectUnsignedInteger = 54,
	EnumObjectShort = 55,
	EnumObjectUnsignedShort = 56,
	EnumObjectFloat = 57,
	EnumObjectDouble = 58,
	EnumObjectArray = 59,
	EnumObjectString = 60,		
	
	EnumObjectObject = 100,
	EnumObjectXTabModel = 101,
	EnumObjectGroupbyModel = 102,
	EnumObjectControlNodeModel = 103,
	EnumObjectSectionModel = 104,
	EnumObjectFieldGroupModel = 105,
	EnumObjectSubtotal = 106,
	EnumObjectXTabView = 107,
	EnumObjectUnit = 108,
	EnumObjectAttributeUnit = 109,
	EnumObjectDummyUnit = 110,
	EnumObjectMetricUnit = 111,	
	EnumObjectConsolidationUnit = 112,
	EnumObjectConsolidation = 113,
	EnumObjectConsolidationElement = 114,
	EnumObjectCustomGroupUnit = 115,
	EnumObjectCustomGroup = 116,
	EnumObjectCustomGroupElement = 117,
	EnumObjectControlElement = 118,
	EnumObjectFieldGroupView = 119,
	EnumObjectField = 120,
	EnumObjectGroupbyElement = 121,
	EnumObjectAttributeForm = 122,
	EnumObjectAttribute = 123,
	EnumObjectMetric = 124,
	EnumObjectGlobalModel = 125,
};

inline int getType(int blockType)
{
	return blockType/100;
}

inline int getVersion(int blockType)
{
	return blockType%100;
}

inline int getBlockType(int type, int version)
{
	return type*100+version;
}

class DSSDataModelBase;
class DSSDataModelXTabImpl;
class DSSDataModelControlImpl;
class DSSDataModelFieldGroupImpl;
class DSSDataModelGroupbyImpl;
class DSSTabularUnit;
class DSSCubeDataSlice;
class DSSCube;
class DSSXTabView;
class DSSTabularIndex;
struct GroupbyElement;
class DSSDocumentInstance;

struct LayoutInfo {
	int axis;
	vector<int> vUnits;
	vector<int> mvUnitSortOrder;			//use this list to sort keys
	int mnRuntimeUnits;
	vector<int> mvMapRuntimeUnit2GBUnit;
	int metricsPos;

	vector<int> mvKeys;
	vector<short> mvLevelID;				//we only need level to sort, it can be viewed as part of keys
	vector<unsigned char> mvMetricID;		//metric ID for each row, empty if metricPos < 0
	vector<unsigned char> mvSubTotalID;		//subtotal type for each row

	vector<int> mvOriginalRows;
	vector<int> mvMap2DistinctRows;			//map to the distinct rows ID
	vector<int> mvMap2DisplayedRows;		//TODO: remove this member

	vector<int> mvDistinctRows;				//no duplicate rows in terms of attribute
	vector<int> mvOffset;					//offset in the index for each distinct row

	int mTotalLevels;						//number of levels;
	vector<vector<int> > mvLevelUnitsID;	//units list for each level, it's sorted by unit ID
	vector<int>	mvStartRow;					//start distinct row ID for each level

	vector<int> mvLevelRowRange;
	int mnDistinctRows;
	
    int mSelectedMetricCount;
    vector<bool> mvSelectedMetricFlag;
    
    vector<int> mvSubtotalFlag;
    
	void setRowCount(int nRows, bool ibInitKeys = true, bool ibInitSubtotalID = true)
	{
		int lnUnits = vUnits.size();
		
		if (lnUnits == 0)
			lnUnits = 1;
			
        if(ibInitKeys)
        {
            mvKeys.resize(nRows * lnUnits);
        }
		mvLevelID.resize(nRows);
        if(ibInitSubtotalID)
        {
            mvSubTotalID.resize(nRows);
        }
		mvMetricID.resize(nRows);
	};
};

class DSSModelPopulation
{
public:
	DSSModelPopulation(DSSBinaryDataReader * ipReadStream);
	~DSSModelPopulation();
	
	int Init(DSSDocumentInstance *ipDI, const char* ipBinaryFilePath, std::string iDataSourcePath);
	
	int LoadModelFrombinary(DSSDataModelBase **oppModel);
	
	int LoadGlobalLookup();
	
	static int LoadMetricData(DSSDataModelXTabImpl *ipXTabModel, DSSBinaryDataReader *ipReadStream, int iSlice, int iDataOffset, int iFlagOffset);
	static int LoadXTabModel(DSSBinaryDataReader *ipReadStream, DSSDataModelBase **oppModel, bool bIsInitialLoading = false);
	static int LoadControlModel(DSSBinaryDataReader *ipReadStream, DSSDataModelBase **oppModel, bool bIsInitialLoading = false);
	static int LoadFieldGroupModel(DSSBinaryDataReader *ipReadStream, DSSDataModelBase **oppModel, bool bIsInitialLoading = false);
	static int LoadGroupbyModel(DSSBinaryDataReader *ipReadStream, DSSDataModelBase **oppModel);
    static int LoadFieldData(DSSDataModelFieldGroupImpl *ipFieldGroupModel, DSSBinaryDataReader *ipReadStream, int iRow);
    static int LoadInputControlFromXTabModel(DSSBinaryDataReader* ipBinaryReader, std::string iKey, DSSDataModelControlImpl** oppControlModel);
    static int LoadInputControlFromFieldGroupModel(DSSBinaryDataReader* ipBinaryReader, std::string iKey, DSSDataModelControlImpl** oppControlModel);
    
    //zhyang, a helper funtion for new rwd
    static int LoadAttributeForDDIC(DSSBinaryDataReader *mpReadStream, int index, DSSTabularUnit **oppXTabUnit);
private:
	MBase::CString mDataSourcePath;
	
	int hLoadSlicedView(DSSDataModelXTabImpl *ipModel, LayoutInfo * ipLayout,
						vector<int> &irMetricDataType, vector<vector<int> > &irTotalDatatype);
						
	int hLoadCells(int iMetricsID, int iSubtotalID,
				   vector<int> &irMetricDataType, vector<vector<int> > &irTotalDatatype,
				   EnumDSSDataType &orCellDataType, vector<unsigned char> &orBuffer, int &orCellFmtType, int &orCellFmtID);
				   
	int hBuildHeaderIndex(LayoutInfo *ipLayout, DSSDataModelXTabImpl *ipXTabModel);
	
	int hBuildHeaderIndex2(LayoutInfo *ipLayout);
	
	int hSetMetricCell(DSSXTabView *ipView, int iMetricID, int iTotalID, int nUnits, int *ipKey,
					   int iCellOffsetint, int iRowLevel, int iColLevel,
					   vector<int> &irvMetricDataType, vector<vector<int> > &irvTotalDatatype,
					   vector<char> &irvCellFlags, vector<int> &irvThresholdID, vector<int> &irvThresholdMetricID);

	DSSBinaryDataReader * mmpReadStream;
	DSSBinaryDataReader * mpReadStream;
	MBase::CString mCurrentNodeKey;
	
	bool mmbDelayLoading;
    
    std::string mDocumentID;
};
