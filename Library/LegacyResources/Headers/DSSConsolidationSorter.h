#import "DSSAEHelper.h"

class DSSSorter;
class DSSSortDefinition;
class DSSXTabHeader;
class DSSDataColumn;
class DSSTabularData;
class DSSTabularConsolidation;

/**
 * DSSConsolidationSorter base class.
 *
 * @author Qiang Xue <qxue@microstrategy.com>
 */
class DSSConsolidationSorter : public DSSSorter
{
public:
	DSSConsolidationSorter(DSSSortDefinition* definition);
	virtual ~DSSConsolidationSorter();
	int sort(DSSXTabHeader* header, vector<int>& items);
	bool operator ()(int a, int b);

private:
    //zhyang, if the ordinal is equal, we should sort according privious result;
    //int hBuildElementIDtoItemMap(DSSXTabHeader* header, vector<int>& items);
	int _unitID;
	EnumDSSSubtotalsPosition _subtotalPosition;
	bool _isNullOnTop;
	bool _isAscending;
	DSSTabularData* _model;
	vector<int> _buckets;
	DSSTabularConsolidation* _consolidation;
    bool mbNewRWD;
    //vector<int> _firstPosInItems;
    
    //xiazhou, 2013-07-26, TQMS 765743. Initialized in sort(), Used to test null element.
    int _globalRowCount;
};
