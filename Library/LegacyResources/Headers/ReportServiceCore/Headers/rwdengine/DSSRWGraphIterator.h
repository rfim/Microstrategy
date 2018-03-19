/*
 *  DSSRWGraphIterator.h
 *  ReportServiceCore
 *
 *  Created by Xiaolong Wang on 3/3/10.
 *  Copyright 2010 MicroStrategy, Inc.. All rights reserved.
 *
 */

#import "DSSRWGridIterator.h"

class DSSRWGraphIterator : public DSSRWGridIterator
{
public:
	DSSRWGraphIterator();
	
	~DSSRWGraphIterator();
	
	int Init(DSSRWNode *ipNode, DSSRWSectionIterator * ipParent, vector<DSSBaseElementProxy*>& ivNGBElements);
	
	/*!
	 @method     
	 @abstract   move the cursor to the start position of the next element
	 @discussion if the current element is the last one, return false
	 */
	bool NextNRowsByElement(int iDepth);
	
	/*!
	 @method     
	 @abstract   move the cursor to the start position of the previous element
	 @discussion if the current element is the first one, return false
	 */
	bool PrevousNRowsByElement(int iDepth);
	
	/*!
	 @method     
	 @abstract   move the cursor to the start position of the next element at the specified depth
	 @discussion if the current element is the last one, return false
	 */
	bool NextNColumnsByElement(int iDepth);
	
	/*!
	 @method     
	 @abstract   move the cursor to the start position of the previous element
	 @discussion if the current element is the first one, return false
	 */
	bool PrevousNColumnsByElement(int iDepth);
	
	//=================================================================================
	//the following two methods getAccumulatedIndex and getChildCount are used by
	//CE to build header element tree
	/*!
	 @method     
	 @abstract   return the element offset from the first element
	 @discussion and element may occupy multiple rows
	 */
	int getAccumulatedIndex(int iAxis, int iDepth);
	/*!
	 @method     
	 @abstract   return the element count of the next depth
	 @discussion iDepth start from -1 to TotalDepth-1
	 */
	int getChildCount(int iAxis, int iDepth);
	//=================================================================================
	
	// JLI Support graph drill, 
	int getGraphDrillUnitAndCellHandle(int iRowIndex, int iColumnIndex, DSSTemplateUnit* & opDrillUnit, DSSCellHandle & oCellHandle);

private:
	bool hMoveCursors(int iDepth, int inc, vector<vector<int> > &irvEleOrdinals, vector<int> &rvCursors);
};
