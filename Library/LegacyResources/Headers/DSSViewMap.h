/*
 *  DSSViewMap.h
 *  DashboardViewer
 *
 *  Created by Xiaolong Wang on 1/5/10.
 *  Copyright 2010 MicroStrategy, Inc.. All rights reserved.
 *
 */
#import <set>
#import "DSSAEHelper.h"

class DSSXTabHashTable;

class DSSViewMap
{
public:
	DSSViewMap();
	~DSSViewMap();

	int Init(vector<GUID> &irGBUnitID, int nRows, int *ipKeys, std::string iFilePath = "", char* pGBPage = NULL, int GBPageSize= 0, int GBPagePosInFile = 0);
	
	int CountRows();
	
	int getView(int *ipKeys, int nUnits, int &orViewID);

	int getViewPartialMatch(int *ipKeys, int nUnits, vector<int> &orvViews, int &orCount);
	
	inline int getUnitCount(){return mnGBUnits;};
	
	int findGBUnit(GUID &irUnitID);

	int getUnitKey(int iRow, int iUnit, int &orKey);
	
	int size();
	
	int getSubtotalKeys(GUID &irUnitID, set<int> &irExcludedTotalKeys, vector<int> &orTotalKeys);
	
	int getUnitID(int iUnit, GUID& orUnitID);

    int getGBUnitCount();
    
    int createMMapMemory(const char * ipFilePath, bool& ibUseNewFile);
    //On-demand file mapping    
    int MapGBKeys(bool ibReloadHashTable = true);
    int UnMapGBKeys(bool ibReleaseHashTable = true);

private:

	vector<GUID> mvGroupbyUnitID;
	int* mpGroupbyKeys;
    int mnGroupbyKeys;
    char* mpGBPage;
    int mGBPageSize;
    int mGBPagePosInFile;
    int mGBKeyOffset;
	DSSXTabHashTable *mpGBLookupTable;
	int mnGBPaths;
	int mnGBUnits;
    std::string mFilePath;
    bool mbGBLookupMapped;
  //  int mFileHandle;
};
