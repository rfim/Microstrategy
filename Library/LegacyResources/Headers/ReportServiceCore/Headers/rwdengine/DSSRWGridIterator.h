/*
 *  DSSRWGridIterator.h
 *  DashboardViewer
 *
 *  Created by Xiaolong Wang on 1/25/10.
 *  Copyright 2010 MicroStrategy, Inc.. All rights reserved.
 *
 */
#import <map>
#import "DSSRWIterator.h"

#ifndef _DSSRWGRIDITERATOR
#define _DSSRWGRIDITERATOR

class DSSTemplateUnit;
class DSSDataModelXTabImpl;
class DSSTabularUnit;
class DSSXTabView;
class DSSRWTemplateNode;
class DSSTemplateMetrics;
class DSSCellHandle;
class DSSRWControl;
class DSSConsolidationElement;
class DSSBaseElementsProxy;
class DSSXTabHashTable;
class DSSDataModelTransaction;
class CComVariant;
class DSSColumnMappingInfo;
class AERawData;
class DSSCubeDataSlice;
class DSSRWGridModel;
class DSSRWSectionModelIterator;
class DSSRWDataModel;
class DSSTemplate;
class DSSTabularData;
class DSSTabularMetrics;

#ifndef __ANDROID__
class TemplateSelectionRow;
class TemplateSelectionItem;
class TemplateSelectionUnit;
class DSSCExpression;
class DSSExpNodeOperator;
#endif
struct HeaderElementInfo {
	int mnRows;
	int mnUnits;
	int mWidth;
	vector<int> mvUnitWidth;			//each unit's width
	vector<int> mvDepthToUnit;			//map from depth to unit
	HeaderElementInfo():mnRows(0), mnUnits(0), mWidth(0){};
};

struct ElementsRelationInfo {
    //int parentIndex;
    //int firstChild;
    //int nextSibling;
    int level;
    bool isExpand;
    bool hasChildren;
};

class DSSRWGridIterator : public DSSRWIterator
{
public:
	DSSRWGridIterator();
	~DSSRWGridIterator();
	
	int Init(DSSRWNode *ipNode, DSSRWSectionIterator * ipParent, vector<DSSBaseElementProxy*>& ivNGBElements);
    
    // DSSRWNode* getRWNode();
	
	int ReInit();
	/*!
	 @abstract   return the total number of rows
	 @discussion
	 */
	int CountRows();
	
	/*!
	 @method
	 @abstract   return the total number of columns
	 @discussion
	 */
	int CountColumns();
	
    /*!
	 @abstract   move cursor to the first row
	 @discussion if grid is empty, return false
	 */
	bool FirstRow();
	
	/*!
	 @method
	 @abstract   move cursor to the last row
	 @discussion if grid is empty, return false
	 */
	//bool LastRow();
	
	/*!
	 @method
	 @abstract   move cursor to the next row
	 @discussion if cursor is at the last row, return false
	 */
	bool NextRow();
	
	/*!
	 @method
	 @abstract   move the cursor to the previous row
	 @discussion if cursor is at the first row, return false
	 */
	bool PreviousRow();
	/*!
	 @abstract   move cursor to the first column
	 @discussion if grid is empty, return false
	 */
	bool FirstColumn();
	
	/*!
	 @method
	 @abstract   move cursor to the last row
	 @discussion if grid is empty, return false
	 */
	//bool LastColumn();
	
	/*!
	 @method
	 @abstract   move cursor to the next column
	 @discussion if cursor is at the last column, return false
	 */
	bool NextColumn();
	
	/*!
	 @method
	 @abstract   move cursor to the previous column
	 @discussion if cursor is at the first column, return false
	 */
	bool PreviousColumn();
	
	/*!
	 @method     
	 @abstract   return the number of units on row or column axis
	 @discussion iAxis is 0-based.
	 */
	int getNumUnits(int iAxis);
	
    /*!
     @method   return the number of units on row or column axis in soucer grid.
     In Graph Matrix, need to slice soucer grid into child grid for eace mini-graph.
     */
    
    int getNumUnitsForGM(int iAxis);
    
	/*!
	 @method     
	 @abstract   return the width of the axis
	 @discussion iAxis is 0-based.
	 */
	int getHeaderWidth(int iAxis);
	
	/*!
	 @method     
	 @abstract   return the width of very unit, how many columns they occupy
	 @discussion  only custom group unit may may have a width > 1
	 */
	int getUnitWidth(int iAxis, int iUnits);
	
	/*!
	 @method     
	 @abstract   return the start depth of a unit.
	 @discussion  
	 */
	int getUnitDepth(int iAxis, int iUnits);
	
    /*!
	 @abstract   return the current row position
	 @discussion the return value is 0-based
	 */
	int getRowOrdinal();
	
	/*!
	 @method
	 @abstract   return the current column position
	 @discussion the return value is 0-based
	 */
	int getColOrdinal();
	
	/*!
	 @method
	 @abstract   find the start and end ordinal of the element at depth iDepth
	 @discussion iDepth is 0-based. In most cases, iDepth is equal to iUnit, except
	 when there's hierarchical displayed custom group unit
	 */
	int getRowElementRange(int iDepth, int* opStart, int* opEnd);
	
	/*!
	 @method
	 @abstract   find the start and end ordinal of the element at depth iDepth
	 @discussion iDepth is 0-based. In most cases, iDepth is equal to iUnit, except
	 when there's hierarchical displayed custom group unit
	 */
	int getColElementRange(int iDepth, int* opStart, int* opEnd);
	
	/*!
	 @method
	 @abstract   return the number of rows/column the element occupies
	 @discussion iDepth is 0-based. In most cases, iDepth is equal to iUnit, except
	 when there's hierarchical displayed custom group unit
	 */
	int getElementSpan(int iAxis, int iDepth);
    
    vector<int> getElementSpanForPie(int iAxis, int iDepth);
    
	/*!
	 @method
	 @abstract   return every element's row/column span in the grid
	 @discussion 1. orvElementSpans[i] is the number of rows/columns the i-th element occupies
     2. it's independent from the row cursor, i.e. no need call nextrow().
	 */
    int getElementsSpanArray(int iAxis, int iDepth, vector<int> & orvElementSpans);
	
	/*!
	 @method
	 @abstract   return the offset of the oridinal in the element's range
	 @discussion iDepth, iOrdinal is 0-based. In most cases, iDepth is equal to iUnit, except
	 when there's hierarchical displayed custom group unit
	 */
	int getElementOffsetInSpan(int iAxis, int iDepth, int iOrdinal);
	
    /*!
	 @abstract   return the width of the element (for custom group hierarchical display)
	 @discussion iDepth, iOrdinal is 0-based. In most cases, iDepth is equal to iUnit, except
	 when there's hierarchical displayed custom group unit
	 */
	int getElementWidth(int iAxis, int iDepth);
	
	/*!
	 @method
	 @abstract   return if an element is subtotal or not
	 @discussion
	 */
	bool IsRowElementSubtotal(int iDepth);
	bool IsColElementSubtotal(int iDepth);
	
    /*!
	 @abstract   return the subtotal type at the current row/column
	 @discussion no need for iDepth parameter, since each row/column can only have one subtotal type
	 */
	EnumDSSMetricType getRowSubtotalType();
	EnumDSSMetricType getColSubtotalType();
	
	/*!
	 @method
	 @abstract   return the raw data value of an attribute form determined by (iDepth, iBaseForm)
	 @discussion If the unit is custom group or consolidation, iFrom will not be used
	 */
	int getRowElementValue(int iDepth, int iBaseForm, const void **oppData, int *opDataSize, EnumDSSDataType *opDataType, DSSDataFlag *opDataFlag);
	int getRowElementValue(int iDepth, int iBaseForm, std::string &orText, DSSDataFlag *opDataFlag);
	
	/*!
	 @method
	 @abstract   return the raw data value of an attribute form determined by (iDepth, iBaseForm)
	 @discussion If the unit is custom group or consolidation, iFrom will not be used
	 */
	int getColElementValue(int iDepth, int iBaseForm, const void **oppData, int *opDataSize, EnumDSSDataType *opDataType, DSSDataFlag *opDataFlag);
	int getColElementValue(int iDepth, int iBaseForm, std::string &orText, DSSDataFlag *opDataFlag);
	
	/*!
	 @method
	 @abstract   return the raw data value of metric
	 @discussion the metric cell is determined by the currect row and column
	 */
	int getCellValue(const void **oppData, int *opDataSize, EnumDSSDataType *opDataType, DSSDataFlag *opDataFlag,
					 int *opThreshold = NULL, int *opMetric = NULL);
	
	/*!
	 @method
	 @abstract   return the metric value as double
	 @discussion If the data type is non-numerical and can't be converted into double, it's treated as NULL
	 */
	int getCellValue(double *opVal, DSSDataFlag *opDataFlag, int *opThreshold = NULL, int *opMetric = NULL);
	
	/*!
	 @method
	 @abstract   get the cell handle
	 @discussion
	 */
	DSSCellHandle getCellHandle();
	DSSCellHandle getCellHandleAt(int iRowOrdinal, int iColOrdinal);
	
	/*!
	 @method
	 @abstract   get the cell handle
	 @discussion
	 */
	DSSCellHandle getElementHandle(int iAxis, int iDepth);
	DSSCellHandle getElementHandleAt(int iAxis, int iOrdinal, int iDepth, int iFormId = 0);
	
	/*!
	 @method
	 @abstract   get the header unit handle
	 @discussion the cell handle type can be DssCellHandleTemplateUnit, DssCellHandleTemplateUnitAndForm or DssCellHandleCustomGroupAndDepth:
	 */
	DSSCellHandle getUnitHandle(int iAxis, int iDepth, int iFormIndex = -1);
	
	/*!
	 @method
	 @abstract  get available commands from handle
	 @discussion
	 */
	int getCommandList(DSSCellHandle &irHandle, vector<EnumManipulationCommandTypes> &orvCommand);
	
	/*!
	 @method
	 @abstract  get element from handle, the caller owns the element
	 @discussion
	 */
	virtual DSSBaseElementProxy * createElementFromHandle(DSSCellHandle &irHandle);
	
    /*!
     @method
     @abstract  get element from handle and all its parent elements
     @discussion
     */
    virtual int createElementsFromRAHandle(DSSCellHandle &irHandle, vector<DSSBaseElementProxy *> &orvpElements);
	/*!
	 @method
	 @abstract  get elements from handle, the caller owns the elements
	 @discussion
	 */
	virtual int createElementsFromHandle(DSSCellHandle &irHandle, int iAxisIndex, vector<DSSBaseElementProxy *> &orvpElements);
	
	/*!
	 @method
	 @abstract  get elements from handle, the caller owns the elements
	 @discussion
	 */
	virtual int createSiblingElementsFromHandle(DSSCellHandle &irHandle, bool iParentElementsOnly, vector<DSSBaseElementProxy *> &orvpElements);
	
    /*!
	 @abstract  get DSSAxis from Handle
	 @discussion
	 */
	virtual EnumDSSAxisName getAxisFromHandle(DSSCellHandle &irHandle);
	
	/*!
	 @method
	 @abstract  return the depth of a header cell handle
	 @discussion return -1 if the cell handle is not DssCellHandleXTabHeaderElement or invalid;
	 */
	int getDepthFromHandle(DSSCellHandle &irHandle);
	
	/*!
	 @method
	 @abstract  return the attribute form index of a header cell handle
	 @discussion	1. return -1 if the cell handle is not DssCellHandleXTabHeaderElement or invalid;
	 2. if the cell is not an attribute, treat it as if only contains 1 form (return 0);
	 */
	int getFormIDFromHandle(DSSCellHandle &irHandle);
	
	/*!
	 @method
	 @abstract  return the row/column ordinal of a header cell handle
	 @discussion return -1 if the cell handle is not DssCellHandleXTabHeaderElement or invalid;
	 */
	int getOrdinalFromHandle(DSSCellHandle &irHandle);
	
	/*!
	 @method
	 @abstract  return the ordinal of a data (metric) cell handle
	 @discussion return -1 if the cell handle is not DssCellHandleXTabData or invalid;
	 */
	int getOrdinalFromHandle(DSSCellHandle &irHandle, int &orRowOrdinal, int &orColOrdinal);
	
    /*!
	 @abstract   return if the specified element is total or not
	 @discussion
	 */
	bool IsSubtotalAt(int iAxis, int iOrdinal, int iDepth);
	//Method for GM
    bool IsSubtotalAtForGM(int iAxis, int iOrdinal, int iDepth);
	/*!
	 @method
	 @abstract   return subtotal type at the specified row/column
	 @discussion
	 */
	EnumDSSMetricType getSubtotalTypeAt(int iAxis, int iOrdinal);
	
	/*!
	 @method
	 @abstract   return if a row is at total level
	 @discussion
	 */
	bool IsSubtotalAtOrdinal(int iAxis, int iOrdinal);
	
	/*!
	 @method
	 @abstract   return an element's value
	 @discussion If the unit is custom group or consolidation, iFrom will not be used
	 */
	int getElementValueAt(int iAxis, int iOrdinal, int iDepth, int iBaseForm, const void **oppData, int *opDataSize, EnumDSSDataType *opDataType, DSSDataFlag *opDataFlag);
    int getElementValueAtForGM(int iAxis, int iOrdinal, int iDepth, int iBaseForm, const void **oppData, int *opDataSize, EnumDSSDataType *opDataType, DSSDataFlag *opDataFlag);
	int getElementValueAt(int iAxis, int iOrdinal, int iDepth, int iBaseForm, std::string &orText, DSSDataFlag *opDataFlag);
    
    int getElementOriginalValueAt(int iAxis, int iOrdinal, int iDepth, int iBaseForm, const void **oppData, int *opDataSize, EnumDSSDataType *opDataType, DSSDataFlag *opDataFlag);
	
    int getElementThresholdAt(int iAxis, int iOrdinal, int iDepth, int &orThresholdID, int spanRowCount = -1, int* opThresholdRow = NULL);
    int getElementThresholdAt(EnumDSSAxisName iAxis, int iRowNum, const GUID& iUnitID, int &orThresholdID);
	/*!
	 @method
	 @abstract   return the number of rows/column the element occupies
	 @discussion iDepth is 0-based. In most cases, iDepth is equal to iUnit, except
	 when there's hierarchical displayed custom group unit
	 */
	int getElementSpanAt(int iAxis, int iOrdinal, int iDepth);
	
	/*!
	 @method
	 @abstract   return the raw data value of metric
	 @discussion the metric cell is determined by the currect row and column
	 */
	int getCellValueAt(int iRowOrdinal, int iColOrdinal, const void **oppData, int *opDataSize, EnumDSSDataType *opDataType, DSSDataFlag *opDataFlag,
					   int *opThreshold = NULL, int *opMetric = NULL);
	int getCellValueAt(int iRowOrdinal, int iColOrdinal, double *opVal, DSSDataFlag *opDataFlag, int *opThreshold = NULL, int *opMetric = NULL);
	
	/*!
	 @method
	 @abstract
	 @discussion
	 */
	DSSTemplateUnit * getUnit(int iAxis, int iDepth);
	
    //Method getUnitForGM Widget
    DSSTemplateUnit * getUnitForGM(int iAxis, int iDepth);
    
	/*!
	 @method
	 @abstract   return template unit GUID on DSSAxis
	 @discussion
	 */
	GUID getUnitID(int iAxis, int iDepth);
	
	inline bool IsMetricInRow(){return mbHasMetricOnRow;};
	
	/*!
	 @method
	 @abstract   further slice the grid using template units, unit value is [0, N), means the i-th element on row/column
	 @discussion It will create a slice window map based on group by units. The map is from groupby path to window row range and column range
	 Later we'll create new grid iterators based on a virtual template whose layout is determined by this API.
	 Right now, we have some restrictions:
	 1. The groupby units should the left N units on row axis and top M units on column axis
	 2. If caller pivot the group-by units, the target axis should not have any units except metrics
	 3. we can only pivot one unit.
	 */
	int setGroupByUnitsForSliceWindow(int nRowUnits, int *ipRowUnits, int nColUnits, int *ipColUnits);
	int setAndPivotGroupByUnitsForSliceWindow(int iAxis, int nUnits, int *ipUnits, int nPivotUnits, int *ipPivotUnits);
	
	/*!
	 @method
	 @abstract   create a grid iterator based on a virtual template.
	 @discussion 1. The virtual template only contains group-by units sepcified by setGroupByUnitsForSliceWindow.
	 2. New iterator still point the original template node
	 3. it's a get function, so caller doesn't own the iterator pointer (no need to release)
	 */
	DSSRWGridIterator * getIteratorForGroupBy();
	
	/*!
	 @method
	 @abstract   set slice window ID
	 @discussion The valid slice ID will be [0, N), N is Num_Row_Groupby_Paths * Num_of_Column_Groupby_Path
	 */
	int setSliceWindowID(int iSlice);
	
	/*!
	 @method
	 @abstract   create a grid iterator on the fly based on a virtual template
	 @discussion 1. The virtual template contains all the non-groupby units plus the metrics unit
	 2. New iterator still point the original template node
	 3. It can return a NULL iterator when there's "holes" after pivot group by unit.
	 4. It's a get function, so caller doesn't own the iterator pointer (no need to release)
	 5. Caller should not keep the iterator either as the instance change after setSliceWindowID
	 */
	//DSSRWGridIterator * getIteratorForSliceWindow();
    DSSRWGridIterator* getIteratorForSliceWindow(bool isNewIterator = false);
	
	/*!
	 @method     set color by units
	 @abstract
	 @discussion for example: {Region}, {Counry} and {Category} on row,
	 color by units = {Region} and {Category}
	 then irRowColorByUnits = [0, 2]
	 */
	int setColorByUnits(vector<int> &irRowColorByUnits, vector<int> &irColColorByUnits);
    
    
    /*
     set color by units for pie chart
     if colorby dropzone has template-metrc, use the metric Number in x,y,angel dropzone int the method.
     */
    int setColorByUnitsForPieChart(vector<int> &irRowColorByUnits, vector<int> &irColColorByUnits, vector<vector<int> > &irXYAColorByUnitsIndex);
	
	/*!
	 @method     return the number of colors specified by color-by units on row and column
	 @abstract   it's the combination of row and comlumn color-by elements
	 @discussion for example: {Region}, {Counry} and {Category} on row, {Year} on column
	 color by units = {Region}, {Category} and {Year}
	 There're 8 regions, 4 categories and 2 Years, it will return (8*4) * 2 = 64
	 */
	int getColorByCount();
    
    map<vector<int>, int> getColorsMap();
    
    vector<vector<int> > getLegendColorItems();
	
	/*!
	 @method     return the color-by index of a metric cell
	 @abstract
	 @discussion the function get the cell's corresponding color-by unit's element
	 and calculate its color index
	 */
	int getColorByIndex(int iRowOrdinal, int iColOrdinal);
    int getColorByIndexForPieChart(int iRowOrdinal, int iColOrdinal,vector<int> &irXYAColorByMetricIndex);
	
    bool getColorByRowColIndexForGM(int &iRowOrdinal, int &iColOrdinal);
    
    std::vector<std::pair<int, int> > convertGlobalRowColToLocalRowCol(const std::vector<std::pair<int, int> > &iGlobalRowCol);

	/*!
	 @method
	 @abstract   find the corresponding axis for the handle
	 @discussion
	 */
	//int getUnitIDAndOriginalAxis(int iAxis, int iDepth, int *opTemplateAxis);
	
    /*!
	 @abstract   if the header element at the specified position is a consolidation element, return it. Otherwise return NULL
	 @discussion caller need check the return pointer if it's a NULL
	 */
	DSSConsolidationElement* getConsolidationElement(int iAxis, int iDepth);
	DSSConsolidationElement* getConsolidationElementAt(int iAxis, int iOrdinal, int iDepth);
	
    /*!
	 @abstract   return a base element proxy for the custom group element at spcified position.
	 @discussion caller need check the return pointer if it's a NULL
	 */
	DSSBaseElementProxy* getCustomGroupElement(int iAxis, int iDepth);
	DSSBaseElementProxy* getCustomGroupElementAt(int iAxis, int iOrdinal, int iDepth);
	
	/*!
	 @method
	 @abstract   move the cursor to a specified position.
	 @discussion This API should not be called frequently as it need binary search to decide how to move the cursor
	 */
	bool MoveCursor(int iAxis, int iOrdinal);
	
	/*!
	 @method
	 @abstract   return the null display string for metric data
	 @discussion
	 */
	int getCellNullDisplay(DSSDataFlag iDataFlag, std::string &orVal);
	
	/*!
	 @method
	 @abstract   return the null display string for header element data
	 @discussion
	 */
	int getElementNullDisplay(DSSDataFlag iDataFlag, std::string &orVal);
	
	/*!
	 @method
	 @abstract   return the null display string for header element data
	 @discussion
	 */
	int getMaxMetricValue(GUID &irMetriID, const void **oppData, int *opDataSize, EnumDSSDataType *opDataType, DSSDataFlag *opDataFlag, bool ibSkipNull = false);
	
	/*!
	 @method
	 @abstract   return the null display string for header element data
	 @discussion
	 */
	int getMinMetricValue(GUID &irMetriID, const void **oppData, int *opDataSize, EnumDSSDataType *opDataType, DSSDataFlag *opDataFlag, bool ibSkipNull = false);
	
	/*!
	 @method
	 @abstract   return the null display string for header element data in rowwindow and column window of RWGridIterator
	 @discussion   imihai- if ibUseSubtotal=true and subtotals exist then get Max from subtotal rows, else get Max from non-subtotal rows
	 */
	int getMaxMetricValueOfSliceWindow(GUID &irMetriID, const void **oppData, int *opDataSize, EnumDSSDataType *opDataType, DSSDataFlag *opDataFlag, bool ibSkipNull=false, bool ibUseSubtotal=false);

    
    /*!
	 @method     
	 @abstract   return the null display string for header element data in rowwindow and column window of RWGridIterator
	 @discussion   imihai- if ibUseSubtotal=true and subtotals exist then get Min from subtotal rows, else get Min from non-subtotal rows
	 */
	int getMinMetricValueOfSliceWindow(GUID &irMetriID, const void **oppData, int *opDataSize, EnumDSSDataType *opDataType, DSSDataFlag *opDataFlag, bool ibSkipNull=false, bool ibUseSubtotal=false);
	
    /*!
	 @abstract   return if a unit is DE
	 @discussion
	 */
	bool IsDerivedElement(int iAxis, int iDepth);
	bool IsDerivedElementAt(int iAxis, int iOrdinal, int iDepth);
	bool HasDerivedElementAlongPath(DSSCellHandle &irHandle);
	
	DSSTemplateUnit* getTemplateUnit(DSSCellHandle &irHandle, bool iNeedTemplateMetric = false); // iNeedTemplateMetric==true returns DSSTemplateMetric, otherwise returns DSSTemplateMetrics
	
	int getXTabElementKey(DSSCellHandle &irHandle, DSSTabularUnit *ipUnit, int &orKey);
	bool HasRuntimeUnit();
	
	//return true if the template node is targeted by a metric selector
	bool HasGroupbyMetrics();
	
	//return the selected metric GUID.
	int getSelectedMetrics(vector<GUID> &orMetricsList);
	
	void setExcludeTotal(bool flag);
	
	/*!
	 @method
	 @abstract  get a list of ordinal numbers, each of which determines an element at the position [iAxis, iDpeth, ordinal] that is
     equal to e. e is one element from ipElements.
     if ipElements contains ALL element, set flag orbHasALL
	 equal to e. e is one element from ipElements.
	 if ipElements contains ALL element, set flag orbHasALL
	 @discussion	it's used to highlight the current selection for template selector
	 caller should check the return value if it's S_OK
	 */
	int SearchElement(int iAxis, int iDepth, DSSBaseElementsProxy *ipElements, vector<int> &orvOrdinal, bool &orbHasALL);
	
	// JLI Support graph drill,
	int getGraphDrillUnitAndCellHandle(int iRowIndex, int iColumnIndex, DSSTemplateUnit* & opDrillUnit, DSSCellHandle & oCellHandle);
	
	int LookupCellFormatID(int iFmtID, GUID &orFmtGUID);
	
	bool HasCellFormatData();
	
	/*!
	 @method
	 @abstract  create a cellhandle which is the first on to match the given element
	 @discussion	it set the callhandle with different types:
	 if the element is invalid or not found, set DssCellHandleReserved
	 if the element is the ALL element, set DssCellHandleTemplateUnit
	 otherwise, set DssCellHandleXTabHeaderElement. If there're multiple rows matching it, return the first one
	 */
	int createCellHandleFromElement(int iAxis, int iDepth, DSSBaseElementProxy *ipElement, DSSCellHandle &orCellHandle);
	
	DSSTabularUnit* getRowXTabUnit(int i);
	DSSTabularUnit* getColXTabUnit(int i);
	int getUnitValue(DSSTabularUnit *ipUnit, int iKey, int iBaseForm, const void **oppData, int *opDataSize, EnumDSSDataType *opDataType, DSSDataFlag *opDataFlag);
	int getRowEleKey(int iColumn);
	int getColEleKey(int iColumn);
	int getRowEleOrdinal(int iColumn);
	int getColEleOrdinal(int iColumn);
	int getEleKeyAt(int iAxis, int iColumn, int iOrdinal);
	
	virtual int UpdateData(DSSCellHandle &irHandle, CComVariant &irNewValue);
	
	virtual int UndoDataChange(DSSCellHandle &irHandle);
	
	DSSDataModelTransaction* getTransactionDataModel();
	
	virtual int MarkRows(int iRowOrdinal, EnumDSSRWMarkRowAction iActionType);
	
	virtual int getDataChangeType(DSSCellHandle &irHandle, bool ibTrxSubPreset = false);
	
    virtual int DiscardTransactionChange(DSSCellHandle &irHandle);
    
	EnumDSSTransactionDataChangeType getRowChangeType(int iOrdinal);
    
    int getValidationFailureRows(vector<int>& vRows);
	
	/*!
	 @abstract	return number of rows of a metric
	 @discussion subtotal value is not included
	 the count is calculated before slicing, so it's NOT affected by CGB and NGB selector 
	 */
	int getMetricCount(GUID &iMetricID);
	
	/*!
	 @abstract		delete some data that can be loaded back on demand, to reduce appplication's memory foot print
	 @discussion
	 */
	virtual void ReleaseUnusedMemory();
    
	int CreateInputControlIterator(std::string iKey, DSSRWInputControlIterator** oppControlIterator);
	
	
	int getTrxColumnIndexByMetricID(int iMetricID);
	
	int getElementSpanStart(int iAxis, int iDepth);
	
    //get ordinal from display row, used for  transaction on android
    int getOrdinalFromDisplay(int iAxis, int iRow, int& orOrdinal);
    
    //only generate transaction record, don't update xtab
    int GenerateTransactionRecord(DSSCellHandle &irHandle, CComVariant &irNewValue);
    //only update xtab, don't generate transaction record
    int UpdateXTabCell(DSSCellHandle &irHandle, CComVariant &irNewValue);
    
    int getMarkedRows(vector<int>& ovRows);
    //[transaction] required column validation, check whether the row containing this cell is updated.
    bool IsRowUpdated(DSSCellHandle &irHandle, bool ibTrxSubVal = false);
    
    EnumDSSDataType getTransactionInputDataType(DSSCellHandle &irHandle);
    
    //imihai
    bool HasMultipleSubtotalsAtSameLevel(int iAxis);
    //imihai
    vector<int> getSubtotalSliceIDsForCurrentGroupBy();
    
    //check if a header cell is a join null element of an attribute
    bool IsJoinNull(DSSCellHandle &irHandle);
    
    inline bool IsModelLoaded(){return (mpGridModel != NULL || mpXTabModel != NULL);};
    
    int getNumRowsByUnits(int iAxis, vector<GUID> &iUnits);
    
    void resetColorByUnits();
    
    /*!
     @abstract get a list of metric cells, which are specified by row ordinal and column ordinal(orvCellOrdinals[i]).
     irvCurrentElements is one of the template selector's current selection tuples. For example, {Year=2010 AND Category=Electronics}, {Year=2011 AND Category=Books} or {Year=2010 AND Category=Movies}
     */
    
    //zhyang, 820043, if the attribute contains control, return it
    DSSRWControl* GetControlForWidget(DSSCellHandle &irHandle, DSSTabularUnit *ipUnit);
    
    int getExtremes(EnumDSSRuntimeSubtotalType iExtremeType,
                    Int32 nRows,
                    Int32 *ipRows,
                    Int32 nColumns,
                    Int32 *ipColumns,
                    Int32 *onMetricIDs,
                    Int32 **oppMetricIDs,
                    EnumDSSDataType **oppDataTypes,
                    EnumDSSRuntimeSubtotalType **oppExtremeTypes,
                    Int32 *onGroupExtremeInfos,
                    void ** oppExtremeInfos);
    
    int getRefOrTrendInfo(EnumDSSRuntimeSubtotalType iLineType,
                          EnumDSSRuntimeSubtotalType iLevel,
                          Int32 nRows,
                          Int32 *ipRows,
                          Int32 nColumns,
                          Int32 *ipColumns,
                          Int32 nBreakbyUnits,
                          Int32 *ipBreakbyUnits,
                          Int32 *onMetricIDs,
                          Int32 **oppMetricIDs,                                 // length of *onMetricIDs
                          Int32 **oppModelIndices,                       // length of *onMetricIDs
                          Int32 **oppTRMetricIDs,                        // length of *onMetricIDs, only for trendline over metrics case
                          EnumDSSDataType **oppDataTypes,          // length of *onMetricIDs
                          Int32 **oppnVariables,
                          Int32 ***opppVariables,
                          EnumDSSObjectType ***opppVariableTypes,
                          Int32 **opIsIncludeBreakBy,                    // length of *onMetricIDs, only for trendline over metrics case
                          Int32 **oppnBreakbyElements,                   // length of (nRows * nColumns) depends on iLevel
                          Int32 **oppBreanbyKeys,                    // length of (nRows * nColumns) * (*opnBreakElements) * nBreakbyUnits
                          Int32 ***oppnSize,                             // length of (nRows * nColumns) * (*opnBreakElements),
                          unsigned char ****oppInfo,                 // length of (nRows * nColumns) * (*opnBreakElements),
                          DSSDataFlag ***oppDataFlags,           // length of (nRows * nColumns) * (*opnBreakElements),
                          void ****oppForecastInfo                      // type fo ForecastInfo ***,length of (nRows * nColumns) * (*opnBreakElements),
    );
    
    void setIgnoreShowAll(bool ibIgnoreShowAll){mbIgnoreShowAll = ibIgnoreShowAll;return;};
    int SearchElement(const vector<GUID> & irvUnitIDs, const vector<DSSBaseElementProxy *> & irvCurrentElements, vector<pair<int, int> >&orvCellOrdinals);
    
    //Conditional logic support
    virtual int AnalyzeTransactionThreshold(DSSThreshold* ipThreshold, bool* opResult);
    
    virtual int UpdateRequiredColumnStatus();

    int AnalyzeTransactionThresholdOnGrid(DSSThreshold* ipThreshold, int iRow, bool* opResult);
    int AnalyzeTransactionThresholdOnGrid(DSSThreshold* ipThreshold, int iRow, int iCol, bool* opResult);
    int getMetricSliceOffset(int iRowOrdinal, int iMetric, int* opOffset);
    int getReportLevelSlice(int iMetric, DSSCubeDataSlice** oppSlice);

    // weilin US51811 2016/07/20 If this grid is source for cgb, remember the cgb sectionmodeliterator
    void setReinitForView(bool ibTemp);

    // weilin DE40331 2016/08/02
    void refreshTargetCGB();

    // weilin DE40331 2016/08/02
    DSSRWSectionIterator* getParentIterator();

    // weilin DE42411, 42497 2016/08/30
    virtual int getTopmostPosInCandidate(GUID iUnitID, bool iAllowTotalSelected, vector<int> &ivCandidateElements);
private:
    
    int hSearchElement(const vector<GUID> & irvUnitIDs, const vector<DSSBaseElementProxy *> & irvCurrentElements, const vector<int> &irvUnitDepth,int iAxis, vector<int> & ovOrdinal);

	bool hIsSubtotal(int iRowIndex);
    
    int hGetValueOffset(int iRowIndex,int iColumnIndex);
    
    int hGetMaxValueOff(const vector<int>& ivOffset,int iMetricID,bool ibSkipNull);
    
    int hGetMinValueOff(const vector<int>& ivOffset,int iMetricID,bool ibSkipNull);
	
	void hIncreaseCursors(int ordinal, int inc, vector<vector<int> > &irvEleOrdinals, vector<int> &rvCursors);
	
	int hInitTemplateStructure(DSSRWNode * ipNode);
	
//	int hGetRowColumnOrdinal(DSSCellHandle &irHandle, int &orRow, int &orCol);
	
	int hGetRecordData(int row, int col, vector<AERawData> &irvOriginalData);
	
	int hBuildTransactionUnitMap(DSSRWNode * ipNode);

	int hFillIndex(int cellID, int iAxisIndex, vector<int> &orKeys);
	
	int hMatchEditableTemplateUnit(DSSTemplateUnit*ipUnit, int iXTabUnitID);
	
	int hFindAttributeForm(DSSColumnMappingInfo *ipColumnInfo, int &orXTabUnitID, int &orFormID);
	
	int hReInit();
    
    // imihai-- 530403
    /*!
     @method  builds the colors map used for legend
     @abstract
     @discussion contains tuples of unit indexes corresponding to legend items -mapped to->: color index (from 0 to number of colors)
     */
    int hBuildColorsMap(vector<int> &irRowColorByUnits, vector<int> &irColColorByUnits);
    
    int hBuildColorsMapForPieChart(vector<int> &irRowColorByUnits, vector<int> &irColColorByUnits, vector<vector<int> > &irXYAColorByIndex);
    
    // imihai-- 530403
    /*!
     @method returns the key in the colors map corresponding to a (row,col) combination
     @abstract
     @discussion the key is created taking into account only the units used for legend
     */
    int getColorKey(int iRowOrdinal, int iColOrdinal, vector<int> &irRowColorByUnits, vector<int> &irColColorByUnits, vector<int> &key);
	
    int getColorKeyForPie(int iRowOrdinal, int iColOrdinal, vector<int> &irRowColorByUnits, vector<int> &irColColorByUnits, vector<int> &irXYAColorByMetrics, vector<int> &key);
    
    
	int hBuildOfflineTransactionUnitMap(DSSRWNode * ipNode);
    int hMatchEditableTemplateUnitOfflineTrx(DSSTemplateUnit*ipUnit, int iXTabUnitID);
    
    //tliao, only called by GetElementOriginalValueAt, return non S_OK means fail to get transaction record
    int hGetElementOriginalValueAt(int iAxis, int iOrdinal, int iDepth, int iBaseForm, const void **oppData, int *opDataSize, EnumDSSDataType *opDataType, DSSDataFlag *opDataFlag);
    
	typedef struct WBColumnInfo{
		int mXTabUnitID;
		int mBaseForm;
		WBColumnInfo(int iUnit, int iForm): mXTabUnitID(iUnit), mBaseForm(iForm){};
		WBColumnInfo():mXTabUnitID(-1), mBaseForm(-1){};
	} WBColumnInfo;
	
	
	//	vector<WBColumnInfo> mvWBColumnMap;		//write back columns
	vector<int> mapTrxColumn2XTabUnit;		//replace mvWBColumnMap to map transaction column to map tabular unit ID
	vector<int> mapTrxColumn2BaseForm;		//replace mvWBColumnMap to map transaction column to tabular unit baseform/metric ID
	vector<int> mapTrxColumn2WriteBackForm;
	
	map<int,int> mvXTabUnitFormToTransactionMap; //map (unitID + formID * unitCount) to target columnIndex, actually a reverse map of mvWBColumnMap to improve temporal performance
	
	vector<int> mvXTabMetricToTransactionMap;
	
    // imihai-- 530403
    map<vector<int>, int> colorsMap; //colors map corresponding to legend
    vector<vector<int> > legendcolorItems;
    
    //offline transaction
    typedef struct offlineWBColumnInfo{
		int mXTabUnitID;
		int mBaseForm;
		offlineWBColumnInfo(int iUnit, int iForm): mXTabUnitID(iUnit), mBaseForm(iForm){};
		offlineWBColumnInfo():mXTabUnitID(-1), mBaseForm(-1){};
	} offlineWBColumnInfo;
    
    //	vector<WBColumnInfo> mvWBColumnMap;		//write back columns
	vector<int> mapOfflineTrxColumn2XTabUnit;		//replace mvWBColumnMap to map transaction column to map tabular unit ID
	vector<int> mapOfflineTrxColumn2BaseForm;		//replace mvWBColumnMap to map transaction column to tabular unit baseform/metric ID
	vector<int> mapOfflineTrxColumn2WriteBackForm;
    vector<int> mapOfflineTrxColumn2TrxColumn;
	vector<EnumDSSDataType> mvOfflineTrxDataType;
	map<int,int> mvXTabUnitFormToOfflineTransactionMap; //map (unitID + formID * unitCount) to target columnIndex, actually a reverse map of mvWBColumnMap to improve temporal performance
	
	vector<int> mvXTabMetricToOfflineTransactionMap;
	
    // weilin US51811 2016/07/20
    bool mbReinitForView;
protected:
	
	int hGetUnit(int iAxis, int iDepth, DSSTabularUnit **oppUnit);
	
	int hGetUnitValue(DSSTabularUnit *ipUnit, int iKey, int iBaseForm, const void **oppData, int *opDataSize, EnumDSSDataType *opDataType, DSSDataFlag *opDataFlag);
	
	//	int hGetAttributeFormValue(DSSTabularUnit *ipUnit, int iOffset, int iBaseForm, const void **oppData, int *opDataSize, EnumDSSDataType *opDataType, DSSDataFlag *opDataFlag);
	
	int hGetSubtotalName(int iSubtotalID, const void **oppData, int *opDataSize, EnumDSSDataType *opDataType, DSSDataFlag *opDataFlag);
	
	int hGetMetricName(int iMetricID, const void **oppData, int *opDataSize, EnumDSSDataType *opDataType, DSSDataFlag *opDataFlag);
	
	//	int hGetCustomGroupValue(DSSTabularUnit *ipUnit, int iOffset, const void **oppData, int *opDataSize, EnumDSSDataType *opDataType, DSSDataFlag *opDataFlag);
	
	//	int hGetConsolidationValue(DSSTabularUnit *ipUnit, int iOffset, int iBaseForm, const void **oppData, int *opDataSize, EnumDSSDataType *opDataType, DSSDataFlag *opDataFlag);
	
	int hCreateCursor();
	
	int hPopulate(DSSRWGridIterator *ipSourceIterator, bool ibPartial);
	
	int hUpdateSliceWindow(DSSRWGridIterator *ipSourceIterator);
	
	void hAdjustOrdinal(vector<vector<int> > &irvOrigOrdinal, vector<vector<int> > &orvNewOrdinal);
	
	DSSRWControl* hGetControl(DSSCellHandle &irHandle, DSSTabularUnit *ipUnit, bool &orbInvalidTotalSelection);
	DSSRWControl* hGetControl(DSSTemplateUnit *ipTemplateUnit, bool ibIgnoreShowAll = false); //zhyang, 850494
	
	int hCreateKeyTable(int nRows, vector<vector<int> > &irOrdinal, vector<vector<int> > &irKeys, vector<int> &orKeyTable);
	
	void hSelectDistinct(vector<int> &irSrc, vector<int> &orDes);
	
	int hGetUnitStartDepth(int iAxis, int iUnitIndex);
	
	void hCleanUp(int iFlag = 0);
	
	DSSDataModelXTabImpl *mpXTabModel;
	DSSXTabView *mpView;
	bool mbNodeHasGBUnits;
	
	vector<DSSTabularUnit*> mvRowXTabUnits;
	vector<DSSTabularUnit*> mvColXTabUnits;
	
	vector<DSSTemplateUnit *> mvpTemplateUnits;
	vector<DSSTemplateUnit *> mvRowTemplateUnits;
	vector<DSSTemplateUnit *> mvColTemplateUnits;
	DSSTemplateMetrics * mpTemplateMetrics;
	
	vector<int> mvUnitMapToAxis;
	
	HeaderElementInfo  mHeader[2];
	
	vector<int> mvRowWindow;
	vector<int> mvColumnWindow;
	
	int mRowOrdinal;
	int mColOrdinal;
	
	vector<vector<int> > mvRowEleOrdinals;		//local (view slice) ordinal
	vector<vector<int> > mvRowEleKeys;
	vector<int> mvRowUnitCursors;
	
	vector<vector<int> > mvColEleOrdinals;		//local (view slice) ordinal
	vector<vector<int> > mvColEleKeys;
	vector<int> mvColUnitCursors;
	
	vector<int> mvRowKeyTable;
	vector<int> mvColKeyTable;
	
    vector<vector<ElementsRelationInfo> > mvRowRelations;
    vector<vector<ElementsRelationInfo> > mvColRelations;
	bool mbHasMetricOnRow;
	
	bool mbExcludeTotal;
	
	//support for widget
	int mnRowGBUnits;
	int mnColGBUnits;
	bool mbPivotGB;
	DSSRWGridIterator *mpGroupbyGridIterator;
	DSSRWGridIterator *mpSliceWindowGridIterator;
	DSSRWGridIterator *mpParentGridIterator;
	vector<int> mvRowSliceWindow;		//smaller slice window groupby mnRowGBUnits
	vector<int> mvColSliceWindow;		//smaller slice window groupby mnColGBUnits
	int mRowSliceWindowSize;
	int mColSliceWindowSize;
	int mRowSliceWindowID;
	int mColSliceWindowID;
	vector<int> mvRowColorByUnitOffset;
	vector<int> mvColColorByUnitOffset;
	vector<int> mvRowColorByKeys;
	vector<int> mvColColorByKeys;
	DSSXTabHashTable *mpRowColorMap;
	DSSXTabHashTable *mpColColorMap;
	int mnColorByRows;
	int mnColorByCols;
	int mCBRowStartOrdinal;
	int mCBColStartOrdinal;
    
    // imihai-- 530403
    vector<int> mvRowColorByUnits;
    vector<int> mvColColorByUnits;
    
    bool mbIgnoreShowAll; //zhyang, 850494
   
public:
    int hGetRowColumnOrdinal(DSSCellHandle &irHandle, int &orRow, int &orCol);

     // tehe 11/18/2011 support for new rwd
	// int Init(DSSRWGridModel *ipGridModel, DSSRWSectionModelIterator * ipParent);
    int Init (DSSRWDataModel* pDataModel, DSSRWNodeModel *pNodeModel, DSSRWNode *pNode, DSSRWSectionModelIterator *ipParentIterator);
    int UpdateModel();
    
    inline bool IsNewRWD() { return mbIsNewRWD; }
    int ResetCurrentNodeModel(DSSRWNodeModel *ipNewNodeModel);
    bool isEvaluated();  //zhyang, for muti-source grid with DE from different base reprots
    string getDEInvalidErrorMessage();
    
    //zhyang
    int sortForMicroChart(GUID &iMetricGUID, bool ibAsc = true, bool isTreeMode = false, int iSubtotalID = -100);
    
    DSSTabularData* getTabularData(){return mpTabularData;}
    virtual DSSXTabView* getXTabView(){return mpView;}
    vector<int>* getRowWindow(){return &mvRowWindow;}
	
	inline bool IsInitialized() {return mbIsInitialized;}
	
    //msun, financial report
    ElementsRelationInfo* getElementRelationInfo(int iAxis, int iOrdinal, int iDepth);
    bool isRecursiveUnit(int iAxis, int iDepth);
    virtual DSSRWDataModel* getDataModel(){return mpDataModel;};
    virtual DSSTabularUnit* getLatestTabularUnit(int iAxis, int iDepth);
    int getCountDisplayChildren(int iAxis, int iOrdinal, int iDepth);
    vector<int> getOrdinalListForElement(int iAxis, int iOrdinal, int iDepth);
    int getFirstOrdinalForElement(int iAxis, int iOrdinal, int iDepth);
    int getParentElementText(int iAxis, int iOrdinal, int iDepth, int iBaseFormId, string& oText);
private:
    // tehe 11/18/2011 support for new rwd
    EnumDSSMetricType hGetSubtotalType(int key);
    int hGetElementNullDisplay(DSSDataFlag iFlag, std::string &orText);
    int hGetUnitsCount(int *oUnitsCount);
    int hGetMetricsUnit(DSSTabularMetrics **oppTabularMetrics);
    int hGetTabularUnit(int iUnitID, DSSTabularUnit **oppUnit);
    int hGetMetricUnitID();
    
    int hBuildDisplayInformation(); //zhyang, get the second part of reinit here. for micro chart
    int hBuildDisplayRelationInformation();//msun, financial report.
    
    // 11/7/2012 tehe
    int PopulateNGBElements();
    
    friend class DSSRWNodeModel;
    friend class DSSRWGridModel;
    DSSRWSectionIterator *mpParentIterator;
    DSSRWDataModel *mpDataModel;
    // DSSTemplate *mpTemplate;
    DSSRWGridModel *mpGridModel;
    // DSSRWNode *mpNode;
    // DSSXTabView2 *mpXTabView;
    DSSTabularData *mpTabularData;
    bool mbNeedUpdateModel;
    bool mbIsNewRWD;
    
    bool mbFirstRender;
    
	int mGlobalLookupVer;
    int mGlobalLookupDataVer;
    
    bool mbIsInitialized;
#ifndef __ANDROID__
private:
    std::vector<TemplateSelectionRow*>* mpSelectionRows;
public:
    void calculateCurrentSelection();
    static std::vector<TemplateSelectionRow*>* calculateCurrentSelectionForTemplateLevelSelector(DSSRWControl* ipControl, DSSTemplate* ipTemplate);
    static void fromExpression(DSSCExpression* ipExpression, DSSExpNodeOperator* ipRoot, DSSRWControl* ipControl, DSSTemplate* ipTemplate, std::vector<TemplateSelectionRow*>* ivCurrentSelections);
    static void fromExpression2(DSSCExpression* ipExpression, DSSExpNodeOperator* ipRoot, DSSRWControl* ipControl, DSSTemplate* ipTemplate, TemplateSelectionRow* lpSelections);
    static void fromExpressionForHtml5VI(DSSCExpression* ipExpression, DSSExpNodeOperator* ipRoot, DSSRWControl* ipControl, DSSTemplate* ipTemplate, std::vector<TemplateSelectionRow*>* ivCurrentSelections);
    static void fromExpression2ForHtml5VI(DSSCExpression* ipExpression, DSSExpNodeOperator* ipRoot, DSSRWControl* ipControl, DSSTemplate* ipTemplate, TemplateSelectionRow* lpSelections);
    static TemplateSelectionItem* fromExpression3(DSSCExpression* ipExpression, DSSExpNodeOperator* ipRoot, DSSRWControl* ipControl, DSSTemplate* ipTemplate);
    
    void cleanSelection();
    int getInitialSelectionStatus(); // 0: no selection, 1: on header, 2: on data
    bool getInitialSelectedCellOridnals(std::vector<std::pair<int, int> >& ovCellOrdinals, bool iDataOnly = false);
    bool isCellHandleInitialSelected(DSSCellHandle* ipCellHandle);
    bool getInitialSelectedElements(GUID& iAttributeID, DSSBaseElementsProxy* &oElements);
#endif
};

#endif
