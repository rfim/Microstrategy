/*
 *  DSSLevelResolver.h
 *  ReportServiceCore
 *
 *  Created by Xiaolong Wang on 2/3/11.
 *  Copyright 2011 MicroStrategy, Inc. All rights reserved.
 *
 */

#ifndef LevelResolver_h
#define LevelResolver_h 1

#import "DSSAEHelper.h"
#import "DSSCube.h"

class DSSEngineLite;
class DSSObjectContext;
class CDSSEngineLite;
class MCESchema;

class DSSLevelResolver
{
public:
    DSSLevelResolver(DSSCube *ipCube, MCESchema *ipSchema = NULL);
	
	~DSSLevelResolver();
	
	int Init();
	
	int ResolveAggLevel(vector<int> &irvInputLevel, vector<int> &irvTargetLevel, vector<int> &orvAggLevel);
	
	int ResolveDownwardLevel(vector<int> &irvAggLevel, vector<int> &irvTargetLevel, vector<int> &orvFinalLevel);
	
	int ResolveTrueLevel(vector<int> &irvInputLevel, vector<int> &orvTrueLevel);
	
	int ResolveLookupUnits(vector<int> &irvInputLevel, vector<int> &irvLookupLevel);	
	
	int IsUpperLevel(vector<int> &irvInputLevel1, vector<int> &irvInputLevel2, bool &orResult);
    
    int ResolveInterimAggLevel(METRIC_ID	iMetricID,
                               Int32 iSliceID,                                        // the slice to be aggregated
                               vector<Int32>& irTargetLevels,                         // the target levels of the final aggregation
                               vector<Int32>& orAggLevels,                            // the temporary aggregation levels
                               vector<DSSSortDefinition*>* opSortDefinitions);		 // the sort definitions of the temporary aggregations
    
    inline MCESchema* getMCESchema(){return mpSchema;}
private:
    //DSSDataModelXTabImpl *mpDataModel;
    DSSCube *mpCube;
    Int32 mnUnit;
	GUID *mpUnitIDs;
    GUID *mpAllUnitIDs;
    EnumDSSTemplateUnitType *mpUnitTypes;
    //DSSEngineLite *mpEngineLite;
    
    MCESchema *mpSchema;
    CDSSEngineLite *mpEngineLite;
    MBase::Buffer *mpBuffer;
    Int32 *mvArity;
};

// jimzhang, Mar/4/2013, DSSEngineLite will be implemented by SQL-Engine team, and below is just hard code of it which
//      is for temporay use. In addition, below code defines a schema relation of Tutorial Project to be used by DSSEngineLite.

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////// BEGIN: SCHEMA RELATION OF Tutorial Project //////////

#define CUSTOMERS_ATTRCOUNT 14
#define GEOGRAPHY_ATTRCOUNT 11
#define PRODUCTS_ATTRCOUNT 7
#define TIME_ATTRCOUNT 5

#define ALL_ATTRCOUNT (CUSTOMERS_ATTRCOUNT + GEOGRAPHY_ATTRCOUNT + PRODUCTS_ATTRCOUNT + TIME_ATTRCOUNT)

typedef enum EnumDSSAttributeNumber
{
    EnumDSSAttributUnknown = -1,
    
    // Customers Hierarchy
    EnumDSSAttributeOrder = 0,
    EnumDSSAttributeRushOrder,
    EnumDSSAttributePaymentMethod,
    EnumDSSAttributeShipper,
    EnumDSSAttributeCustomer,
    EnumDSSAttributeShipDate,
    EnumDSSAttributeIncomeBracket,
    EnumDSSAttributeZipCode,
    EnumDSSAttributeCustomerBirthDate,
    EnumDSSAttributeCustomerCity,
    EnumDSSAttributeCustomerAge,
    EnumDSSAttributeCustomerState,
    EnumDSSAttributeCustomerRegion,
    EnumDSSAttributeCustomerCountry,
    
    // Geography Hierarchy
    EnumDSSAttributeEmployee = 0,
    EnumDSSAttributeSalary,
    EnumDSSAttributeCallCenter,
    EnumDSSAttributeHireDate,
    EnumDSSAttributeEmployBirthDate,
    EnumDSSAttributeRegion,
    EnumDSSAttributeDistributionCenter,
    EnumDSSAttributeManager,
    EnumDSSAttributeEmployeeExperience,
    EnumDSSAttributeEmployeeAge,
    EnumDSSAttributeCountry,
    
    // Products Hierarchy
    EnumDSSAttributeItem = 0,
    EnumDSSAttributeDiscontinuedCode,
    EnumDSSAttributeCatalog,
    EnumDSSAttributeBrand,
    EnumDSSAttributeSupplier,
    EnumDSSAttributeSubcategory,
    EnumDSSAttributeCategory,
    
    // Time Hierarchy
    EnumDSSAttributeDay = 0,
    EnumDSSAttributeMonth,
    EnumDSSAttributeMonthOfYear,
    EnumDSSAttributeQuarter,
    EnumDSSAttributeYear
} EnumDSSAttributeNumber;

typedef enum EnumDSSHierarchyID
{
    EnumDSSHierarchyUnknown = -1,
    
    EnumDSSHierarchyCustomers = 0,
    EnumDSSHierarchyGeography,
    EnumDSSHierarchyProducts,
    EnumDSSHierarchyTime
} EnumDSSHierarchyID;

typedef struct _AttrInfo
{
    EnumDSSAttributeNumber attrNumber;
    EnumDSSHierarchyID hierID;
} AttrInfo;

typedef struct _AttrMappingNode
{
    std::string name;
    AttrInfo attrInfo;
} AttrMappingNode;

extern AttrMappingNode AttrMappingTable[ALL_ATTRCOUNT];

// Customers Hierarchy
extern EnumDSSAttributeNumber *CustomerChildTable[CUSTOMERS_ATTRCOUNT];

// Geography Hierarchy
extern EnumDSSAttributeNumber* GeographyChildTable[GEOGRAPHY_ATTRCOUNT];

// Products Hierarchy
extern EnumDSSAttributeNumber *ProductsChildTable[PRODUCTS_ATTRCOUNT];

// Time Hierarchy
extern EnumDSSAttributeNumber *TimeChildTable[TIME_ATTRCOUNT];

////////// END: SCHEMA RELATION OF Tutorial Project //////////

////////// BEGIN: HARD CODED DSSEngineLite //////////
class DSSEngineLite
{
public:
    DSSEngineLite(DSSCube *ipCube);
    ~DSSEngineLite();
    
    //  ResolveAggLevel(pInputLevel, pTargetLevel, pAggLevel)
    //  {
    //      Reduce pTargetLevel to pLowTargetLevel // e.g. {Year, Month,Product} -> {Month,Product}
    //      for-each iLevel in pLowTargetLevel
    //          if (iLevel is in pInputLevel)
    //              pAggLevel.add(iLevel)
    //          else if (child of iLevel is in pInputLevel)
    //              pAggLevel.add(iLevel)
    //          else if (parent of iLevel is in pInputLevel)
    //              lParent = lowest level parent in pInputLevel
    //              pAggLevel.add(lParent)
    //  }
    int ResolveAggLevel(int iLength, GUID *iSet, vector<int> &irvInput, vector<int> &irvWanted, vector<int> &orvResult);
    
    //  ResolveDownwardLevel(pAggLevel, pTargetLevel, pFinalLevel)
    //  {
    //      for-each iLevel in pAggLevel
    //          if (iLevel is in pTargetLevel)
    //              pFinalLevel.add(iLevel)
    //          else if (child of iLevel is in pTargetLevel)
    //              pChilds [] = highest level childs in pTargetLevel
    //              pFinalLevel.add(pChilds)
    //          else
    //              assert error
    //  }
    int ResolveDownwardLevel(int iLength, GUID *iSet, vector<int> &irvMetricLevel, vector<int> &irvTargetAttrs, vector<int> &orvResult);
    
    int ResolveTrueLevel(int iLength, GUID *iSet, vector<int> &irvInput, vector<int> &orvResult);
    
    int IsUpperLevel(int iLength, GUID *iSet, vector<int> &irvLevel1, vector<int> &irvLevel2, bool &orResult);
    
    int ResolveAllLevel(int iLength, GUID *iSet, vector<int> &irvInLevel, vector<int> &orvRusult);
    
private:
    int hNameToAttrInfo(std::string &irName, AttrInfo &orAttrInfo);
    
    int hGetAttrRelation(AttrInfo &irAttrInfo1, AttrInfo &irAttrInfo2, int &orRelation);
    
    int hIsChild(EnumDSSAttributeNumber** ippAttrNumber, EnumDSSAttributeNumber iAttrNumber1, EnumDSSAttributeNumber iAttrNumber2, bool &ofrIsChild);
    
    int hReduceAttr(vector<GUID> &irvGUID, vector<AttrInfo> &irvAttrInfo, bool ifToHighLevel = false);
    
private:
    //DSSDataModelXTabImpl *mpDataModel;
    DSSCube *mpCube;
    DSSObjectContext* mpObjectContext;
};

////////// END: HARD CODED DSSEngineLite //////////

#endif
