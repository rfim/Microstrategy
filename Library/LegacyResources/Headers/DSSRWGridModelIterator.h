/*
 *  DSSRWGridModelIterator.h
 *  ReportServiceCore
 *
 *  Created by Xiaolong Wang on 9/26/11.
 *  Copyright 2011 MicroStrategy, Inc. All rights reserved.
 *
 */

#ifndef RWGridModelIterator_H
#define RWGridModelIterator_H

//#import "DSSRWNodeModelIterator.h"
//
//#include "DSSRWGridIterator.h"
//

//class DSSRWDataModel;
//class DSSRWGridModel;
//class DSSRWNodeModel;
//class DSSRWNode;
//class DSSXTabView2;
//class DSSTemplate;
//class DSSRWSectionModelIterator;
//
//
//class DSSTemplateUnit;
//// class DSSDataModelXTabImpl;
//class DSSTabularUnit;
//// class DSSXTabView;
//class DSSRWTemplateNode;
//class DSSTemplateMetrics;
//class DSSCellHandle;
//class DSSRWControl;
//class DSSConsolidationElement;
//class DSSBaseElementsProxy;
//class DSSXTabHashTable;
//class DSSDataModelTransaction;
//class CComVariant;
//class DSSColumnMappingInfo;
//class AERawData;
//
//class HeaderElementInfo;

//class DSSRWGridModelIterator : public DSSRWNodeModelIterator
//{
//public:
//	
//	DSSRWGridModelIterator();
//	
//	virtual ~DSSRWGridModelIterator();
//
//// server side compatible  
//public:  
//    // IDSSGridGraphIterator interface methods
//	HRESULT get_WindowSize(EnumDSSAxisName Axis, Int32 * pSize );
//    
//	HRESULT GetWindow(EnumDSSAxisName Axis, Int32 Size, Int32 * pWindow, Int32 * pWritten);
    
	//virtual HRESULT get_ReportInstance(IDSSReportInstance ** ppReportInstance);
	// int GetTemplate(DSSTemplate ** oppTemplate);
    
//	//virtual HRESULT get_SlicedGridWindow(IDSSSlicedGridWindow ** ppSlicedGridWindow);
//	HRESULT get_DisplayMode(enum EnumDSSDisplayMode * pDisplayMode);
//	
//    //ctang 2011-8-23
//	HRESULT get_IsSliced(VARIANT_BOOL* opSliced);
//	//for IDSSGridGraphIterator2
//	// virtual HRESULT get_SlicedGridWindowIF(int,IDSSSlicedGridWindow **);
//    
//	// ICDSSGridGraphIterator interface methods
//	HRESULT get_NumSlices(Int32 * pSize );
//    
//	HRESULT GetSlices(Int32 Size, Int32 * pSlices, Int32 * pWritten);
//    
//	// overridding parent impelementations
//	HRESULT get_Type(EnumDSSIteratorType *pType);
	// virtual HRESULT InterfaceSupportsErrorInfo(REFIID riid);
    
//	int Init(DSSRWDataModel *ipDataModel, DSSRWNode *ipNode, DSSRWNodeModel *ipNodeModel, DSSRWSectionModelIterator *ipParentIterator,DSSTemplate* ipTemplate, DSSXTabView2* ipXTabView);
   // int Init(DSSRWGridModel *ipGridModel, DSSRWSectionModelIterator * ipParent);

// tehe-client side compatible from gridview and gridgraphview
//public: 
//    int Init(DSSRWNode *ipNode, DSSRWSectionModelIterator * ipParent, vector<DSSBaseElementProxy*>& ivNGBElements);
//	
//	int ReInit();
//    
//	/*!
//	 @method     
//	 @abstract   return the total number of rows
//	 @discussion 
//	 */
//	int CountRows();
//	
//	/*!
//	 @method     
//	 @abstract   return the total number of columns
//	 @discussion 
//	 */
//	int CountColumns();
//    
//	/*!
//	 @method     
//	 @abstract   move cursor to the first row
//	 @discussion if grid is empty, return false
//	 */
//	bool FirstRow();
//	
//	/*!
//	 @method     
//	 @abstract   move cursor to the last row
//	 @discussion if grid is empty, return false
//	 */
//	//bool LastRow();
//	
//	/*!
//	 @method     
//	 @abstract   move cursor to the next row
//	 @discussion if cursor is at the last row, return false
//	 */
//	bool NextRow();
//	
//	/*!
//	 @method     
//	 @abstract   move the cursor to the previous row
//	 @discussion if cursor is at the first row, return false
//	 */
//	bool PreviousRow();
//    
//	/*!
//	 @method     
//	 @abstract   move cursor to the first column
//	 @discussion if grid is empty, return false
//	 */
//	bool FirstColumn();
//	
//	/*!
//	 @method     
//	 @abstract   move cursor to the last row
//	 @discussion if grid is empty, return false
//	 */
//	//bool LastColumn();
//	
//	/*!
//	 @method     
//	 @abstract   move cursor to the next column
//	 @discussion if cursor is at the last column, return false
//	 */
//	bool NextColumn();
//	
//	/*!
//	 @method     
//	 @abstract   move cursor to the previous column
//	 @discussion if cursor is at the first column, return false
//	 */
//	bool PreviousColumn();
//	
//	/*!
//     @method     
//     @abstract   return the number of units on row or column axis
//     @discussion iAxis is 0-based.
//     */
//	int getNumUnits(int iAxis);
//	
//	/*!
//     @method     
//     @abstract   return the width of the axis
//     @discussion iAxis is 0-based.
//     */
//	int getHeaderWidth(int iAxis);
//	
//	/*!
//     @method     
//     @abstract   return the width of very unit, how many columns they occupy
//     @discussion  only custom group unit may may have a width > 1
//     */
//	int getUnitWidth(int iAxis, int iUnits);
//	
//	/*!
//     @method     
//     @abstract   return the start depth of a unit.
//     @discussion  
//     */
//	int getUnitDepth(int iAxis, int iUnits);
//    
//	/*!
//	 @method     
//	 @abstract   return the current row position
//	 @discussion the return value is 0-based
//	 */
//	int getRowOrdinal();
//	
//	/*!
//	 @method     
//	 @abstract   return the current column position
//	 @discussion the return value is 0-based
//	 */
//	int getColOrdinal();
//	
//	/*!
//	 @method     
//	 @abstract   find the start and end ordinal of the element at depth iDepth
//	 @discussion iDepth is 0-based. In most cases, iDepth is equal to iUnit, except
//     when there's hierarchical displayed custom group unit
//	 */
//	int getRowElementRange(int iDepth, int* opStart, int* opEnd);		
//	
//	/*!
//	 @method     
//	 @abstract   find the start and end ordinal of the element at depth iDepth
//	 @discussion iDepth is 0-based. In most cases, iDepth is equal to iUnit, except
//     when there's hierarchical displayed custom group unit
//	 */
//	int getColElementRange(int iDepth, int* opStart, int* opEnd);		
//	
//	/*!
//	 @method     
//	 @abstract   return the number of rows/column the element occupies
//	 @discussion iDepth is 0-based. In most cases, iDepth is equal to iUnit, except
//	 when there's hierarchical displayed custom group unit
//	 */
//	int getElementSpan(int iAxis, int iDepth);
//	
//	/*!
//	 @method     
//	 @abstract   return the offset of the oridinal in the element's range
//	 @discussion iDepth, iOrdinal is 0-based. In most cases, iDepth is equal to iUnit, except
//	 when there's hierarchical displayed custom group unit
//	 */
//	int getElementOffsetInSpan(int iAxis, int iDepth, int iOrdinal);
//    
//	/*!
//	 @method     
//	 @abstract   return the width of the element (for custom group hierarchical display)
//	 @discussion iDepth, iOrdinal is 0-based. In most cases, iDepth is equal to iUnit, except
//	 when there's hierarchical displayed custom group unit
//	 */
//	int getElementWidth(int iAxis, int iDepth);
//	
//	/*!
//	 @method     
//	 @abstract   return if an element is subtotal or not
//	 @discussion
//	 */
//	bool IsRowElementSubtotal(int iDepth);
//	bool IsColElementSubtotal(int iDepth);
//    
//	/*!
//	 @method     
//	 @abstract   return the subtotal type at the current row/column
//	 @discussion no need for iDepth parameter, since each row/column can only have one subtotal type
//	 */
//	EnumDSSMetricType getRowSubtotalType();
//	EnumDSSMetricType getColSubtotalType();
//	
//	/*!
//	 @method     
//	 @abstract   return the raw data value of an attribute form determined by (iDepth, iBaseForm)
//	 @discussion If the unit is custom group or consolidation, iFrom will not be used
//	 */
//	int getRowElementValue(int iDepth, int iBaseForm, const void **oppData, int *opDataSize, EnumDSSDataType *opDataType, DSSDataFlag *opDataFlag);
//	int getRowElementValue(int iDepth, int iBaseForm, std::string &orText, DSSDataFlag *opDataFlag);
//	
//	/*!
//	 @method     
//	 @abstract   return the raw data value of an attribute form determined by (iDepth, iBaseForm)
//	 @discussion If the unit is custom group or consolidation, iFrom will not be used
//	 */
//	int getColElementValue(int iDepth, int iBaseForm, const void **oppData, int *opDataSize, EnumDSSDataType *opDataType, DSSDataFlag *opDataFlag);
//	int getColElementValue(int iDepth, int iBaseForm, std::string &orText, DSSDataFlag *opDataFlag);
//	
//	/*!
//	 @method     
//	 @abstract   return the raw data value of metric
//	 @discussion the metric cell is determined by the currect row and column
//	 */
//	int getCellValue(const void **oppData, int *opDataSize, EnumDSSDataType *opDataType, DSSDataFlag *opDataFlag,
//					 int *opThreshold = NULL, int *opMetric = NULL);
//	
//	/*!
//	 @method     
//	 @abstract   return the metric value as double
//	 @discussion If the data type is non-numerical and can't be converted into double, it's treated as NULL
//	 */
//	int getCellValue(double *opVal, DSSDataFlag *opDataFlag, int *opThreshold = NULL, int *opMetric = NULL);
//	
//	/*!
//	 @method     
//	 @abstract   get the cell handle
//	 @discussion 
//	 */
//	DSSCellHandle getCellHandle();
//	DSSCellHandle getCellHandleAt(int iRowOrdinal, int iColOrdinal);
//	
//	/*!
//	 @method     
//	 @abstract   get the cell handle
//	 @discussion 
//	 */
//	DSSCellHandle getElementHandle(int iAxis, int iDepth);
//	DSSCellHandle getElementHandleAt(int iAxis, int iOrdinal, int iDepth, int iFormId = 0);
//	
//	/*!
//	 @method     
//	 @abstract   get the header unit handle
//	 @discussion the cell handle type can be DssCellHandleTemplateUnit, DssCellHandleTemplateUnitAndForm or DssCellHandleCustomGroupAndDepth:
//	 */
//	DSSCellHandle getUnitHandle(int iAxis, int iDepth, int iFormIndex = -1);
//	
//	/*!
//	 @method     
//	 @abstract  get available commands from handle
//	 @discussion
//	 */
//	int getCommandList(DSSCellHandle &irHandle, vector<EnumManipulationCommandTypes> &orvCommand);
//	
//	/*!
//	 @method     
//	 @abstract  get element from handle, the caller owns the element
//	 @discussion
//	 */
//	virtual DSSBaseElementProxy * createElementFromHandle(DSSCellHandle &irHandle);
//	
//	/*!
//	 @method     
//	 @abstract  get elements from handle, the caller owns the elements
//	 @discussion
//	 */
//	virtual int createElementsFromHandle(DSSCellHandle &irHandle, int iAxisIndex, vector<DSSBaseElementProxy *> &orvpElements);
//	
//	/*!
//	 @method     
//	 @abstract  get elements from handle, the caller owns the elements
//	 @discussion
//	 */
//	virtual int createSiblingElementsFromHandle(DSSCellHandle &irHandle, bool iParentElementsOnly, vector<DSSBaseElementProxy *> &orvpElements);
//    
//	/*!
//	 @method     
//	 @abstract  get DSSAxis from Handle
//	 @discussion
//	 */
//	virtual EnumDSSAxisName getAxisFromHandle(DSSCellHandle &irHandle);
//    
//	/*!
//	 @method     
//	 @abstract   return if the specified element is total or not
//	 @discussion 
//	 */
//	bool IsSubtotalAt(int iAxis, int iOrdinal, int iDepth);
//	
//	/*!
//	 @method     
//	 @abstract   return subtotal type at the specified row/column
//	 @discussion 
//	 */
//	EnumDSSMetricType getSubtotalTypeAt(int iAxis, int iOrdinal);
//	
//	/*!
//	 @method     
//	 @abstract   return if a row is at total level
//	 @discussion 
//	 */
//	bool IsSubtotalAtOrdinal(int iAxis, int iOrdinal);
//	
//	/*!
//	 @method     
//	 @abstract   return an element's value
//	 @discussion If the unit is custom group or consolidation, iFrom will not be used
//	 */
//	int getElementValueAt(int iAxis, int iOrdinal, int iDepth, int iBaseForm, const void **oppData, int *opDataSize, EnumDSSDataType *opDataType, DSSDataFlag *opDataFlag);
//	int getElementValueAt(int iAxis, int iOrdinal, int iDepth, int iBaseForm, std::string &orText, DSSDataFlag *opDataFlag);
//	
//	/*!
//	 @method     
//	 @abstract   return the number of rows/column the element occupies
//	 @discussion iDepth is 0-based. In most cases, iDepth is equal to iUnit, except
//	 when there's hierarchical displayed custom group unit
//	 */
//	int getElementSpanAt(int iAxis, int iOrdinal, int iDepth);
//	
//	/*!
//	 @method     
//	 @abstract   return the raw data value of metric
//	 @discussion the metric cell is determined by the currect row and column
//	 */
//	int getCellValueAt(int iRowOrdinal, int iColOrdinal, const void **oppData, int *opDataSize, EnumDSSDataType *opDataType, DSSDataFlag *opDataFlag,
//                       int *opThreshold = NULL, int *opMetric = NULL);
//	int getCellValueAt(int iRowOrdinal, int iColOrdinal, double *opVal, DSSDataFlag *opDataFlag, int *opThreshold = NULL, int *opMetric = NULL);
//    
//	/*!
//	 @method     
//	 @abstract   
//	 @discussion 
//	 */
//	DSSTemplateUnit * getUnit(int iAxis, int iDepth);
//    
//	/*!
//	 @method     
//	 @abstract   return template unit index on DSSAxis
//	 @discussion 
//	 */
//	int getUnitID(int iAxis, int iDepth);
//	
//	inline bool IsMetricInRow(){return mbHasMetricOnRow;};
//	
//	/*!
//	 @method     
//	 @abstract   further slice the grid using template units, unit value is [0, N), means the i-th element on row/column 
//	 @discussion It will create a slice window map based on group by units. The map is from groupby path to window row range and column range
//     Later we'll create new grid iterators based on a virtual template whose layout is determined by this API.
//     Right now, we have some restrictions:
//     1. The groupby units should the left N units on row axis and top M units on column axis
//     2. If caller pivot the group-by units, the target axis should not have any units except metrics
//     3. we can only pivot one unit.
//	 */
//	int setGroupByUnitsForSliceWindow(int nRowUnits, int *ipRowUnits, int nColUnits, int *ipColUnits);
//	int setAndPivotGroupByUnitsForSliceWindow(int iAxis, int nUnits, int *ipUnits, int nPivotUnits, int *ipPivotUnits);
//	
//	/*!
//	 @method     
//	 @abstract   create a grid iterator based on a virtual template.
//	 @discussion 1. The virtual template only contains group-by units sepcified by setGroupByUnitsForSliceWindow.
//     2. New iterator still point the original template node
//     3. it's a get function, so caller doesn't own the iterator pointer (no need to release)
//	 */
//	DSSRWGridModelIterator * getIteratorForGroupBy();
//	
//	/*!
//	 @method     
//	 @abstract   set slice window ID
//	 @discussion The valid slice ID will be [0, N), N is Num_Row_Groupby_Paths * Num_of_Column_Groupby_Path
//	 */
//	int setSliceWindowID(int iSlice);
//	
//	/*!
//	 @method     
//	 @abstract   create a grid iterator on the fly based on a virtual template
//	 @discussion 1. The virtual template contains all the non-groupby units plus the metrics unit
//     2. New iterator still point the original template node
//     3. It can return a NULL iterator when there's "holes" after pivot group by unit.
//     4. It's a get function, so caller doesn't own the iterator pointer (no need to release)
//     5. Caller should not keep the iterator either as the instance change after setSliceWindowID
//	 */
//	DSSRWGridModelIterator * getIteratorForSliceWindow();
//    
//	/*!
//	 @method     set color by units
//	 @abstract   
//	 @discussion for example: {Region}, {Counry} and {Category} on row,
//     color by units = {Region} and {Category}
//     then irRowColorByUnits = [0, 2]
//	 */
//	int setColorByUnits(vector<int> &irRowColorByUnits, vector<int> &irColColorByUnits);
//	
//	/*!
//	 @method     return the number of colors specified by color-by units on row and column
//	 @abstract   it's the combination of row and comlumn color-by elements
//	 @discussion for example: {Region}, {Counry} and {Category} on row, {Year} on column
//     color by units = {Region}, {Category} and {Year}
//     There're 8 regions, 4 categories and 2 Years, it will return (8*4) * 2 = 64
//	 */
//	int getColorByCount();
//	
//	/*!
//	 @method     return the color-by index of a metric cell
//	 @abstract   
//	 @discussion the function get the cell's corresponding color-by unit's element
//     and calculate its color index
//	 */
//	int getColorByIndex(int iRowOrdinal, int iColOrdinal);
//	
//	/*!
//	 @method     
//	 @abstract   find the corresponding axis for the handle
//	 @discussion 
//	 */
//	//int getUnitIDAndOriginalAxis(int iAxis, int iDepth, int *opTemplateAxis);
//    
//	/*!
//	 @method     
//	 @abstract   if the header element at the specified position is a consolidation element, return it. Otherwise return NULL
//	 @discussion caller need check the return pointer if it's a NULL
//	 */
//	DSSConsolidationElement* getConsolidationElement(int iAxis, int iDepth);
//	DSSConsolidationElement* getConsolidationElementAt(int iAxis, int iOrdinal, int iDepth);
//    
//	/*!
//	 @method     
//	 @abstract   return a base element proxy for the custom group element at spcified position.
//	 @discussion caller need check the return pointer if it's a NULL
//	 */
//	DSSBaseElementProxy* getCustomGroupElement(int iAxis, int iDepth);
//	DSSBaseElementProxy* getCustomGroupElementAt(int iAxis, int iOrdinal, int iDepth);
//	
//	/*!
//	 @method     
//	 @abstract   move the cursor to a specified position.
//	 @discussion This API should not be called frequently as it need binary search to decide how to move the cursor
//	 */
//	bool MoveCursor(int iAxis, int iOrdinal);
//	
//	/*!
//	 @method     
//	 @abstract   return the null display string for metric data
//	 @discussion 
//	 */
//	int getCellNullDisplay(DSSDataFlag iDataFlag, std::string &orVal);
//	
//	/*!
//	 @method     
//	 @abstract   return the null display string for header element data
//	 @discussion 
//	 */
//	int getElementNullDisplay(DSSDataFlag iDataFlag, std::string &orVal);
//	
//	/*!
//	 @method     
//	 @abstract   return the null display string for header element data
//	 @discussion 
//	 */
//	int getMaxMetricValue(GUID &irMetriID, const void **oppData, int *opDataSize, EnumDSSDataType *opDataType, DSSDataFlag *opDataFlag);
//	
//	/*!
//	 @method     
//	 @abstract   return the null display string for header element data
//	 @discussion 
//	 */
//	int getMinMetricValue(GUID &irMetriID, const void **oppData, int *opDataSize, EnumDSSDataType *opDataType, DSSDataFlag *opDataFlag);
//	
//	/*!
//	 @method     
//	 @abstract   return if a unit is DE
//	 @discussion 
//	 */
//	bool IsDerivedElement(int iAxis, int iDepth);
//	bool IsDerivedElementAt(int iAxis, int iOrdinal, int iDepth);
//	bool HasDerivedElementAlongPath(DSSCellHandle &irHandle);
//    
//	DSSTemplateUnit* getTemplateUnit(DSSCellHandle &irHandle, bool iNeedTemplateMetric = false); // iNeedTemplateMetric==true returns DSSTemplateMetric, otherwise returns DSSTemplateMetrics
//	
//	int getXTabElementKey(DSSCellHandle &irHandle, DSSTabularUnit *ipUnit, int &orKey);
//	bool HasRuntimeUnit();
//    
//	//return true if the template node is targeted by a metric selector
//	bool HasGroupbyMetrics();
//	
//	//return the selected metric GUID.  
//	int getSelectedMetrics(vector<GUID> &orMetricsList);
//	
//	void setExcludeTotal(bool flag);
//	
//	/*!
//	 @method     
//	 @abstract  get a list of ordinal numbers, each of which determines an element at the position [iAxis, iDpeth, ordinal] that is 
//     equal to e. e is one element from ipElements.
//     if ipElements contains ALL element, set flag orbHasALL
//	 @discussion	it's used to highlight the current selection for template selector
//     caller should check the return value if it's S_OK
//	 */
//	int SearchElement(int iAxis, int iDepth, DSSBaseElementsProxy *ipElements, vector<int> &orvOrdinal, bool &orbHasALL);
//	
//	// JLI Support graph drill, 
//	int getGraphDrillUnitAndCellHandle(int iRowIndex, int iColumnIndex, DSSTemplateUnit* & opDrillUnit, DSSCellHandle & oCellHandle);
//	
//	int LookupCellFormatID(int iFmtID, GUID &orFmtGUID);
//	
//	bool HasCellFormatData();
//	
//	/*!
//	 @method     
//	 @abstract  create a cellhandle which is the first on to match the given element
//	 @discussion	it set the callhandle with different types:
//	 if the element is invalid or not found, set DssCellHandleReserved
//	 if the element is the ALL element, set DssCellHandleTemplateUnit
//	 otherwise, set DssCellHandleXTabHeaderElement. If there're multiple rows matching it, return the first one
//	 */
//	int createCellHandleFromElement(int iAxis, int iDepth, DSSBaseElementProxy *ipElement, DSSCellHandle &orCellHandle);
//	
//	virtual int UpdateData(DSSCellHandle &irHandle, CComVariant &irNewValue);
//	
//	virtual int UndoDataChange(DSSCellHandle &irHandle);
//	
//	DSSDataModelTransaction* getTransactionDataModel();
//    
//	virtual int MarkRows(int iRowOrdinal, EnumDSSRWMarkRowAction iActionType);
//    
//	virtual EnumDSSTransactionDataChangeType getDataChangeType(DSSCellHandle &irHandle);
//	
//	EnumDSSTransactionDataChangeType getRowChangeType(int iOrdinal);
//	
//	/*!
//	 @abstract	return number of rows of a metric
//	 @discussion subtotal value is not included
//     the count is calculated before slicing, so it's NOT affected by CGB and NGB selector 
//	 */
//	int getMetricCount(GUID &iMetricID);
//	
//	/*!
//	 @abstract		delete some data that can be loaded back on demand, to reduce appplication's memory foot print
//	 @discussion
//	 */
//	virtual void ReleaseUnusedMemory();
//
//// tehe-client side compatible from gridview and gridgraphview
//private:
//    
//	void hIncreaseCursors(int ordinal, int inc, vector<vector<int> > &irvEleOrdinals, vector<int> &rvCursors);
//	
//	int hInitTemplateStructure(DSSRWNode * ipNode);
//    
//	int hGetRowColumnOrdinal(DSSCellHandle &irHandle, int &orRow, int &orCol);
//	
//	int hGetRecordData(int row, int col, vector<AERawData> &irvOriginalData);
//	
//	int hBuildTransactionUnitMap(DSSRWNode * ipNode);
//	
//	int hFillIndex(int cellID, int iAxisIndex, vector<int> &orKeys);
//    
//	int hMatchEditableTemplateUnit(DSSTemplateUnit*ipUnit, int iXTabUnitID);
//    
//	int hFindAttributeForm(DSSColumnMappingInfo *ipColumnInfo, int &orXTabUnitID, int &orFormID);
//	
//	int hReInit();
//	
//	typedef struct WBColumnInfo{
//		int mXTabUnitID;
//		int mBaseForm;
//		WBColumnInfo(int iUnit, int iForm): mXTabUnitID(iUnit), mBaseForm(iForm){};
//		WBColumnInfo():mXTabUnitID(-1), mBaseForm(-1){};
//	} WBColumnInfo;
//	
//	
//    //	vector<WBColumnInfo> mvWBColumnMap;		//write back columns
//	vector<int> mapTrxColumn2XTabUnit;		//replace mvWBColumnMap to map transaction column to map tabular unit ID
//	vector<int> mapTrxColumn2BaseForm;		//replace mvWBColumnMap to map transaction column to tabular unit baseform/metric ID
//	
//	
//	map<int,int> mvXTabUnitFormToTransactionMap; //map (unitID + formID * unitCount) to target columnIndex, actually a reverse map of mvWBColumnMap to improve temporal performance
//    
//	vector<int> mvXTabMetricToTransactionMap;
//
//// tehe-client side compatible from gridview and gridgraphview
//protected:
//	int GetActualGBElements(vector<vector<int> >& ovvGroupByElements);
//      
//	int hGetUnit(int iAxis, int iDepth, DSSTabularUnit **oppUnit);
//	
//	int hGetUnitValue(DSSTabularUnit *ipUnit, int iKey, int iBaseForm, const void **oppData, int *opDataSize, EnumDSSDataType *opDataType, DSSDataFlag *opDataFlag);
//	
//    //	int hGetAttributeFormValue(DSSTabularUnit *ipUnit, int iOffset, int iBaseForm, const void **oppData, int *opDataSize, EnumDSSDataType *opDataType, DSSDataFlag *opDataFlag);
//	
//	int hGetSubtotalName(int iSubtotalID, const void **oppData, int *opDataSize, EnumDSSDataType *opDataType, DSSDataFlag *opDataFlag);
//	
//	int hGetMetricName(int iMetricID, const void **oppData, int *opDataSize, EnumDSSDataType *opDataType, DSSDataFlag *opDataFlag);
//	
//    //	int hGetCustomGroupValue(DSSTabularUnit *ipUnit, int iOffset, const void **oppData, int *opDataSize, EnumDSSDataType *opDataType, DSSDataFlag *opDataFlag);
//	
//    //	int hGetConsolidationValue(DSSTabularUnit *ipUnit, int iOffset, int iBaseForm, const void **oppData, int *opDataSize, EnumDSSDataType *opDataType, DSSDataFlag *opDataFlag);
//	
//	int hCreateCursor();
//	
//	int hPopulate(DSSRWGridModelIterator *ipSourceIterator, bool ibPartial);
//	
//	int hUpdateSliceWindow(DSSRWGridModelIterator *ipSourceIterator);
//	
//	void hAdjustOrdinal(vector<vector<int> > &irvOrigOrdinal, vector<vector<int> > &orvNewOrdinal);
//	
//	DSSRWControl* hGetControl(DSSCellHandle &irHandle, DSSTabularUnit *ipUnit, bool &orbInvalidTotalSelection);
//	DSSRWControl* hGetControl(DSSTemplateUnit *ipTemplateUnit);
//	
//	int hCreateKeyTable(int nRows, vector<vector<int> > &irOrdinal, vector<vector<int> > &irKeys, vector<int> &orKeyTable);
//    
//	void hSelectDistinct(vector<int> &irSrc, vector<int> &orDes);
//	
//	int hGetUnitStartDepth(int iAxis, int iUnitIndex);
//	
//	void hCleanUp(int iFlag = 0);
//    
//	// DSSDataModelXTabImpl *mpXTabModel;
//	// DSSXTabView *mpView;
//    DSSRWGridModel *mpGridModel;
//	bool mbNodeHasGBUnits;
//	
//	vector<DSSTabularUnit*> mvRowXTabUnits;
//	vector<DSSTabularUnit*> mvColXTabUnits;
//	
//	vector<DSSTemplateUnit *> mvpTemplateUnits;
//	vector<DSSTemplateUnit *> mvRowTemplateUnits;
//	vector<DSSTemplateUnit *> mvColTemplateUnits;
//	DSSTemplateMetrics * mpTemplateMetrics;
//	
//	vector<int> mvUnitMapToAxis;
//	
//	HeaderElementInfo  mHeader[2];
//	
//	vector<int> mvRowWindow;
//	vector<int> mvColumnWindow;
//	
//	int mRowOrdinal;
//	int mColOrdinal;
//	
//	vector<vector<int> > mvRowEleOrdinals;		//local (view slice) ordinal
//	vector<vector<int> > mvRowEleKeys;
//	vector<int> mvRowUnitCursors;
//	
//	vector<vector<int> > mvColEleOrdinals;		//local (view slice) ordinal
//	vector<vector<int> > mvColEleKeys;
//	vector<int> mvColUnitCursors;
//    
//	vector<int> mvRowKeyTable;
//	vector<int> mvColKeyTable;
//	
//	bool mbHasMetricOnRow;
//	
//	bool mbExcludeTotal;
//	
//	//support for widget
//	int mnRowGBUnits;
//	int mnColGBUnits;
//	bool mbPivotGB;
//	DSSRWGridModelIterator *mpGroupbyGridIterator;
//	DSSRWGridModelIterator *mpSliceWindowGridIterator;
//	DSSRWGridModelIterator *mpParentGridIterator;
//	vector<int> mvRowSliceWindow;		//smaller slice window groupby mnRowGBUnits
//	vector<int> mvColSliceWindow;		//smaller slice window groupby mnColGBUnits
//	int mRowSliceWindowSize;
//	int mColSliceWindowSize;
//	int mRowSliceWindowID;
//	int mColSliceWindowID;
//	vector<int> mvRowColorByUnitOffset;
//	vector<int> mvColColorByUnitOffset;
//	vector<int> mvRowColorByKeys;
//	vector<int> mvColColorByKeys;
//	DSSXTabHashTable *mpRowColorMap;
//	DSSXTabHashTable *mpColColorMap;
//	int mnColorByRows;
//	int mnColorByCols;
//	int mCBRowStartOrdinal;
//	int mCBColStartOrdinal;
//
//private:
//	DSSXTabView2* mpXTabView;
//    
//	DSSTemplate* mpTemplate;
//	vector<int> mvRowRange;
//	vector<int> mvColumnRange;
//    
//    vector<vector<int> > mvvGroupByElements;
// };

// #endif