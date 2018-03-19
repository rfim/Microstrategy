#import "DSSAEHelper.h"

class DSSSorter;
class DSSSortDefinition;
class DSSXTabHeader;
class DSSDataModelXTabImpl;

/**
 * DSSMetricDefaultSorter base class.
 *
 * @author Qiang Xue <qxue@microstrategy.com>
 */
class DSSMetricDefaultSorter : public DSSSorter
{
public:
	DSSMetricDefaultSorter(DSSSortDefinition* definition);
	virtual ~DSSMetricDefaultSorter();
	int sort(DSSXTabHeader* header, vector<int>& items);
	bool operator ()(int a, int b);

private:
	int _unitID;
	EnumDSSSubtotalsPosition _subtotalPosition;
	DSSDataModelXTabImpl* _model;
	DSSXTabHeader* _header;
};
