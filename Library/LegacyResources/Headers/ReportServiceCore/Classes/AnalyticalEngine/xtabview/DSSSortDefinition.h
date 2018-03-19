#import "DSSAEHelper.h"
#import <map>
#import "DSSSliceID.h"

class DSSDataModelBase;
class DSSSorter;
class DSSTabularData;

/**
 * DSSSortDefinition class.
 *
 * @author Qiang Xue <qxue@microstrategy.com>
 */
class DSSSortDefinition
{
public:
	DSSTabularData* model;
	EnumDSSSortType sortType;
	int unitID;
	int formID;
	int metricID;
	std::map<int,int> elementIDs;
	EnumDSSSubtotalsPosition subtotalPosition;
	bool isNullOnTop;
	bool isAscending;
	int subtotalID;
	int level;
    vector<int> units;//zhyang, HS using none
    //zhyang, 569270, units that in the level.
    //if we conver units and sobtotalID to level, we will lose some information. eg hierarchical sort, using none.
    vector<int> mvUnits;
    vector<int> mvMaskUnits;
    int bucketID;
    int unitIDInContianer;
    MBase::CString mNullReplacement;
    
    //Slice ID, used for OLAP Calculation Sort
    DSSSliceID mSliceID;
    
    EnumDSSOLAPNullPosition mNullPosition;
    int mLocale;
    
public:
	DSSSortDefinition(DSSTabularData* model);
	~DSSSortDefinition();
	DSSSorter* getSorter();
	EnumDSSSubtotalsPosition getSubtotalPosition();
    int get_AttributeAndForm(Int32 *pAttr, Int32 *pForm);
    bool mForMicroChart; //zhyang, sort for micro chart
    
private:
	DSSSorter* _sorter;
};

class DSSSortDefinitions
{
public:
    vector<DSSSortDefinition*> *mpSortDefs;  //DSSSortDefinitions doesn't own mpSortDef
};

