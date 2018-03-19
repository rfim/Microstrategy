#import "DSSAEHelper.h"
#import <map>

class DSSDataModelBase;
class DSSSortDefinition;
class DSSSorts;
class DSSSort;
class DSSAxis;
class DSSTabularConsolidation;
class DSSXTabHeader;
class DSSXTabView;
class DSSTabularMetrics;
class DSSTabularData;
class EventContext;
class DSSMetric;

/**
 * DSSSortConverter class.
 *
 * @author Qiang Xue <qxue@microstrategy.com>
 */
class DSSSortConverter
{
public:
	DSSSortConverter(DSSDataModelBase *ipDataModelBase, DSSTabularData* model);
    DSSSortConverter(DSSViewDataSet * ipViewDataSet, DSSXTabView *ipXTabView = NULL);
	~DSSSortConverter();
	vector<DSSSortDefinition*>* convert(DSSAxis* axis, vector<int>& unitIDs);
    int OLAPSortConvert(vector<DSSSort*> &iSorts,
                                                EventContext *ipEventContext,
                                                int inSlices,
                                                int *ipInSliceIDs,
                                                DSSMetric *ipDimMetricGov, vector<DSSSortDefinition*> **oppSortDefinition);
	
    int generateDefaultSortDefinition(int unitID, vector<DSSSortDefinition*>* definitions);
    vector<DSSSortDefinition*>* convertForMicroChart(DSSAxis* axis, vector<int>& unitIDs, GUID &iMetricGUID, bool ibAsc, bool isTreeMode, int iSubtotalID);
    //tliao, 954998, 2014/08/28, support dynamic sort by
    int OLAPSortConvert(DFCSimpleExpr               *ipSortLevel,
                        EventContext                *ipEventContext,
                        int                         inSlices,
                        int                         *ipInSliceIDs,
                        DSSMetric                   *ipDimMetricGov,
                        vector<DSSSortDefinition*>  **oppSortDefinition);
    
private:
	//convertUserDefinedSorts(axis:IDSSAxis, layout:XTabHeaderLayout, definitions:Array):Array
	//convertDefaultSorts(axis:IDSSAxis, layout:XTabHeaderLayout, mask:Array, definitions:Array):void
	EnumDSSSubtotalsPosition determineSubtotalPosition(DSSSort* sort);
	int convertAttributeFormSortDefinition(DSSSort* sort, vector<int>& unitIDs, vector<DSSSortDefinition*>* definitions, bool ibDEFormSort = false);
	int convertAttributeDefaultSortDefinition(DSSSort* sort, vector<int>& unitIDs, vector<DSSSortDefinition*>* definitions);
	void generateAttributeDefaultSorts();
	int convertMetricSortDefinition(DSSSort* sort, vector<int>& unitIDs, vector<DSSSortDefinition*>* definitions);
	int convertMetricHierarchicalSortDefinition(DSSSort* sort, vector<int>& unitIDs, DSSXTabHeader*header, vector<DSSSortDefinition*>* definitions, bool ibForMicroChart = false, GUID *iMetricGUID = NULL, bool ibAsc = true, bool isTreeMode = false, int iSubtotalID = -100);
	int convertCustomGroupSortDefinition(DSSSort* sort, vector<int>& unitIDs, vector<DSSSortDefinition*>* definitions);
	int convertConsolidationSortDefinition(DSSSort* sort, vector<int>& unitIDs, vector<DSSSortDefinition*>* definitions);
	void generateDerivedElementDefaultSortDefinition(DSSTabularConsolidation* unit, vector<DSSSortDefinition*>* definitions);
    int hGetAxisID(DSSSort* iSort);
    
    int hConvertAttributeFormSortDefinition(DSSSort* sort, vector<DSSSortDefinition*>* definitions);
	int hConvertAttributeDefaultSortDefinition(DSSSort* sort, vector<DSSSortDefinition*>* definitions);
	int ConvertOlapMetricSort(DSSSort* sort, vector<DSSSortDefinition*>* definitions, EventContext *ipEventContext, DSSMetric *ipDimMetricGov);
    //	int hConvertMetricHierarchicalSortDefinition(DSSSort* sort, vector<int>& unitIDs, DSSXTabHeader*header, vector<DSSSortDefinition*>* definitions);
	int hConvertCustomGroupSortDefinition(DSSSort* sort, vector<DSSSortDefinition*>* definitions);
	int hConvertConsolidationSortDefinition(DSSSort* sort, vector<DSSSortDefinition*>* definitions);
    int hConvertValueSortDefinition(DSSSort* sort, vector<DSSSortDefinition*>* definitions);
    
    int ConvertMetricSortSimpleExpr(DFCSimpleExprUnit           *ipSort,
                                    vector<DSSSortDefinition*>  *definitions,
                                    EventContext                *ipEventContext,
                                    DSSMetric                   *ipDimMetricGov);
    int ConvertAttributeSortSimpleExpr(DFCSimpleExprUnit           *ipSort,
                                       vector<DSSSortDefinition*>  *definitions);
    int hGetAttrFormFromUnit(DFCSimpleExprUnit *ipUnit, Int32 &oAttrIndex, Int32 &oAttrForm, Int32 &oLocale);
    int ConvertCustomGroupSortSimpleExpr(DFCSimpleExprUnit *ipSort,
                                         vector<DSSSortDefinition*>  *definitions);
    int ConvertConsolidationSortSimpleExpr(DFCSimpleExprUnit *ipSort,
                                           vector<DSSSortDefinition*>  *definitions);
	/*
     generateSortDefinitions(axis:IDSSAxis, layout:XTabHeaderLayout):Array
     */
    int convertRecursiveUnitFormSortDefinition(DSSSort* sort, vector<int>& unitIDs, vector<DSSSortDefinition*>* definitions, bool ibDEFormSort = false);
    int convertRecursiveUnitDefaultSortDefinition(DSSSort* sort, vector<int>& unitIDs, vector<DSSSortDefinition*>* definitions);
private:
    //    DSSTemplate *hGetTemplate();
    inline bool hGetNullOnTop();
    inline DSSTabularMetrics* hGetMetricUnit();
    inline int hGetMetricUnitID();
    inline DSSObjectContext* hGetObjectContext();
    int FindUnit(const GUID &irGUID, EnumDSSTemplateUnitType iType, bool includeLinkUnit = true, bool iCheckRecursive = false);
    int Count();
    int FindElement(DSSBaseElementProxy *ipElement, int &orUnit, int &orKey);
    
	DSSTabularData* _model;
    DSSXTabView *mpXTabView;
	std::map<GUID, DSSSorts*> _defaultAttributeSorts;
	EnumDSSSubtotalsPosition _subtotalPosition;
    bool mbNewRWD;
    
    bool mbIsNullOnTop;
    MBase::CString mNullReplacement;
    DSSTemplate *mpTemplate;
    //map<int, bool> unitsortmap;  //zhyang, 745798, 745499, remember the manipulation's asc/desc property for certain unit
};
