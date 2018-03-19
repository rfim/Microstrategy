#import "DSSAEHelper.h"

//<!DOCTYPE mrgmandoctype[<!ENTITY mrgman_form_feed "&#x0A;">]>

class DSSSorter;
class DSSSortDefinition;
class DSSXTabHeader;
class DSSDataColumn;
class DSSTabularData;

/**
 * DSSAttributeFormSorter base class.
 *
 * @author Qiang Xue <qxue@microstrategy.com>
 */
class DSSAttributeFormSorter : public DSSSorter
{
public:
	DSSAttributeFormSorter(DSSSortDefinition* definition);
	virtual ~DSSAttributeFormSorter();
	int sort(DSSXTabHeader* header, vector<int>& items);
	virtual bool sortUnitElements(vector<int>& elements);
	bool operator ()(int a, int b);
    virtual int buildBucket(DSSXTabHeader* header);
    //vector<int> _buckets;
    
    //junwang, 2012/07/25, optimized version
    int sort_New(DSSXTabHeader* header, vector<int>& items);
private:
	int _unitID;
	int _formID;
	EnumDSSSubtotalsPosition _subtotalPosition;
	bool _isNullOnTop;
	bool _isAscending;
	
	DSSDataColumn* _column;
	DSSTabularData* _model;
    
    int _nSortedElem; //junwang, 2012/07/25, record number of sorted elements
    int _unitIndex;
    vector<int> mvUnitKeys;
    
    //zhyang, 777421
    bool OnlySortDisplay();
    int sortDisplay(DSSXTabHeader* header, vector<int>& items);
};
