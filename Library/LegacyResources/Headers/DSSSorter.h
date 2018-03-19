#import "DSSAEHelper.h"

class DSSXTabHeader;

/**
 * DSSSorter base class.
 *
 * @author Qiang Xue <qxue@microstrategy.com>
 */

#ifndef AEDSSSORTER
#define AEDSSSORTER

class DSSSorter
{
public:
	virtual int sort(DSSXTabHeader* header, vector<int>& items)=0;
	virtual bool sortUnitElements(vector<int>& elements) { return false; }
    virtual ~DSSSorter() {}
    virtual int buildBucket(DSSXTabHeader* header){return S_OK;};
//protected:
    int count;
    DSSXTabHeader* _header;
    vector<int> _buckets;
};

#endif