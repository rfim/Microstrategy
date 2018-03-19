#import "DSSAEHelper.h"

class DSSSorter;
class DSSSortDefinition;
class DSSXTabHeader;
class DSSTabularData;
class DSSXTabView;

/**
 * DSSMetricSorter base class.
 *
 * @author Qiang Xue <qxue@microstrategy.com>
 */
class DSSMetricSorter : public DSSSorter
{
public:
	DSSMetricSorter(DSSSortDefinition* definition);
	virtual ~DSSMetricSorter();
	int sort(DSSXTabHeader* header, vector<int>& items);
	bool operator ()(int a, int b);
    virtual int buildBucket(DSSXTabHeader* header);
private:
	DSSTabularData* _model;
	int _unitID;
	int _metricID;
	EnumDSSSubtotalsPosition _subtotalPosition;
	int _level;
	bool _isNullOnTop;
	bool _isAscending;
	map<int,int> _elementIDs;
	int _subtotalID;
	vector<const char*> _values;
	vector<EnumDSSDataType> _types;
	vector<DSSDataFlag> _flags;
	vector<int> _levels;
    
    //zhyang, use for hierarchical sort
    vector<int> _mvUnits;
	
	double toDouble(const char* value, EnumDSSDataType type);
	bool isNumerical(EnumDSSDataType type);
    
    bool hGetNullReplacement(MBase::CString &iNullString);
    
    bool mbNewRWD;
    
    DSSXTabView* hGetXTabView();
    vector<int> _ranks;
    vector<int> _mvMaskUnits;
    MBase::CString _mNullReplacement;
    bool _mbForMicroChart;
    
    //zhyang, sort for micro chart
    int buildRowMapForMicroChart(DSSXTabHeader* header, vector<int> items, vector<int> &iovRowMap);

    int respectHierarchyRightMost(DSSXTabHeader* header, vector<int>& inputRank, vector<int>& metricOrder, vector<int>& outputRank);
};
