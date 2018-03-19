/*
 *  DSSRWControlIterator.h
 *  DashboardViewer
 *
 *  Created by Xiaolong Wang on 2/22/10.
 *  Copyright 2010 MicroStrategy, Inc.. All rights reserved.
 *
 */

#import "DSSRWIterator.h"
#import <set>
class DSSDataModelControlImpl;
class DSSRWControl;
class DSSBaseElementProxy;
class ControlCell;
class DSSObjectContext;
class CComVariant;

class DSSRWControlIterator : public DSSRWIterator
{
public:
	DSSRWControlIterator();
	virtual ~DSSRWControlIterator();
	
	int Init(DSSRWNode *ipNode, DSSRWSectionIterator * ipParent, vector<DSSBaseElementProxy*>& ivNGBElements);
	
	virtual int ReInit();
	
	int InitElements(std::string& iXMLString);
	
	/*!
	 @method     
	 @abstract   return the total number of rows
	 @discussion 
	 */
	int CountRows();
	
	/*!
	 @method     
	 @abstract   move cursor to the first row
	 @discussion if the control is empty, return false
	 */
	bool FirstRow();
	
	/*!
	 @method     
	 @abstract   move cursor to the next row
	 @discussion if cursor is at the last row, return false
	 */
	bool NextRow();
	
	/*!
	 @method     
	 @abstract  return ControlCell at the current position
	 @discussion
	 */
	virtual ControlCell getControlCell();
	
	/*!
	 @method     
	 @abstract  return ControlCell for a specified position
	 @discussion	index range from [0, N), N is the value returned by CountRows()
	 */
    

	virtual ControlCell getControlCellAt(int index);
    
//hyan,2012/07/12,since now we don't need to support remove null element feature
//comment these APIs
//    
//    virtual ControlCell getNonNullControlCellAt(int index);
//    
//    virtual int CountNonNullRows();
	
	/*!
	 @method     
	 @abstract  return Control
	 @discussion
	 */
	DSSRWControl *getControl();
	
	/*!
	 @method     
	 @abstract  return control element at the current row
	 @discussion
	 */
	//DSSBaseElementProxy* getElement();
	virtual DSSBaseElementProxy* getElementAt(int index);
	
	/*!
	 @method     
	 @abstract  return control element's display text at the current row
	 @discussion
	 */
    //hyan,16/11/2011, new RWD
	virtual int getElementText(std::string &orText);
	
	inline DSSDataModelControlImpl * getModel(){return mpControlModel;};
	
	/*!
	 @method     
	 @abstract  get available commands from handle
	 @discussion
	 */
	int getCommandList(DSSCellHandle &irHandle, vector<EnumManipulationCommandTypes> &orvCommand);
	
	/*!
	 @method     
	 @abstract  is the current control element selected
	 @discussion
	 */
	bool IsSelectedAt(int index);
	
	virtual int FindElement(DSSBaseElementProxy* ipElement);
	
	virtual int getMetricValueMin(const CComVariant ** oppVal);
    
	virtual int getMetricValueMax(const CComVariant ** oppVal);
	
	virtual int getMetricValueCount(const CComVariant ** oppVal);
	

    
    virtual int getElementDisplayText(DSSBaseElementProxy* ipElement, std::string &orText);
    
    virtual int getElementMatchText(DSSBaseElementProxy* ipElement, std::string &orText);
    
    virtual int FindElementDisplayIndex(DSSBaseElementProxy *ipElement);
    
    void ReInitSelectedElements();
protected:

	int hGetControllList(bool ibIncludeSelf, bool byElementBrowsing = false);
	
	bool hShowAll(DSSRWControl *ipControl);
	
	int hGenerateElements(DSSRWControl *ipControl, bool byElementBrowsing = false);
	
	int hGenerateMetricElements();
	
	void hClean();
    
    /**
     * Sets the element at the given index, the original one is deleted.  If the given index is out of bounary, no
     * operation is performed.
     *
     * @param iIndex the position of the element to set.
     * @param ipElement the element to set.
     */
    virtual void setElementAt(int iIndex, DSSBaseElementProxy* ipElement);
    void hGenerateSelectionInformation();
protected:
	// We don't cache DSSRWControl, we cache control key
	std::string mControlKey;
    
	DSSDataModelControlImpl *mpControlModel;
	int mnElements;
	vector<int> mvElements;
	int mCursor;
    
	vector<DSSBaseElementProxy*> mpElements;	//[TODO]: cache in data model
	DSSBaseElementProxy *mpAllElement;
	bool mbShowAll;
    std::set<int> mvSelectedCollection;
};

class ControlCell
{
public:
	ControlCell(int iEle, DSSRWControlIterator * ipIterator);
    
	/*!
	 @method     
	 @abstract  return the cell handle
	 @discussion
	 */
	DSSCellHandle getHandle();
	
	/*!
	 @method     
	 @abstract  return control element
	 @discussion
	 */
	DSSBaseElementProxy* getElement();
    
    
    EnumDSSElementType getElementType();
	
	/*!
	 @method     
	 @abstract  return the display text of control element
	 @discussion
	 */
	int getText(std::string &orText);
	
	/*!
	 @method     
	 @abstract  is the current control element selected
	 @discussion
	 */
	bool IsSelected();
	
	/*!
	 @method     
	 @abstract  does the current control have a target?
	 @discussion
	 */
	bool hasSelectionTarget();
	
private:
	
	int mElementID;
	DSSRWControlIterator *mpIterator;
};

