#import "DSSAEHelper.h"

class DSSSorter;
class DSSSortDefinition;
class DSSXTabHeader;
class DSSDataColumn;
class DSSTabularData;

/**
 * DSSCustomGroupSorter base class.
 *
 * @author Qiang Xue <qxue@microstrategy.com>
 */
class DSSCustomGroupSorter : public DSSSorter
{
public:
	DSSCustomGroupSorter(DSSSortDefinition* definition);
	virtual ~DSSCustomGroupSorter();
	int sort(DSSXTabHeader* header, vector<int>& items);
	bool operator ()(int a, int b);

private:
	int _unitID;
	EnumDSSSubtotalsPosition _subtotalPosition;
	bool _isNullOnTop;
	bool _isAscending;
	DSSTabularData* _model;
};
