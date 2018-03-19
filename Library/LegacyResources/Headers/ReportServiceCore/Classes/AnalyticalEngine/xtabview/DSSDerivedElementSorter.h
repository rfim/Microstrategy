#import "DSSAEHelper.h"

class DSSSorter;
class DSSSortDefinition;
class DSSXTabHeader;
class DSSDataColumn;
class DSSTabularData;
class DSSTabularConsolidation;

/**
 * DSSDerivedElementSorter base class.
 *
 * @author Qiang Xue <qxue@microstrategy.com>
 */
class DSSDerivedElementSorter : public DSSSorter
{
public:
	DSSDerivedElementSorter(DSSSortDefinition* definition, EnumDSSSortType sortType);
	virtual ~DSSDerivedElementSorter();
	int sort(DSSXTabHeader* header, vector<int>& items);
	virtual bool sortUnitElements(vector<int>& elements);
	bool operator ()(int a, int b);

private:
	int _unitID;
	int _formID;
	EnumDSSSubtotalsPosition _subtotalPosition;
	bool _isNullOnTop;
	bool _isAscending;
	vector<int> _buckets;
	DSSDataColumn* _column;
	DSSTabularData* _model;
	DSSTabularConsolidation* _consolidation;
	bool _stringTyped;
    
    //xiazhou, 2013-07-26, TQMS 765743. Initialized in sort(), Used to test null element.
    int _globalRowCount;
    EnumDSSSortType _sortType;//DssSortDerivedElementForm or DssSortDerivedElementDefault.
};
