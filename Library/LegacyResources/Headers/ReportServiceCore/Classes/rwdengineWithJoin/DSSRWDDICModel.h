//
//  DSSRWDDICModel.h
//  ReportServiceCore
//
//  Created by xiazhou on 6/30/14.
//  Copyright (c) 2014 MicroStrategy, Inc. All rights reserved.
//
//  Move DDIC related members and methods from DSSRWGridModel/DSSRWFieldGroupModel to this class.

#ifndef DSSRWDDICModel_h
#define DSSRWDDICModel_h

#include "DSSAEHelper.h"

class DFCCSI;
class DSSTabularData;
class DSSTabularIndex;
class DSSTabularAttribute;
class DSSRWDataModel;
class DSSTabularIndex;
class DSSTabularUnit;

class DSSRWDDICModel{
    
public:
    DSSRWDDICModel(DSSRWDataModel *ipDataModel);
    ~DSSRWDDICModel();
    inline vector<DFCCSI*> getCSIsForDDIC() {return mvCSIForDDIC;}
    inline vector<GUID> getDatasetIDForDDIC() {return mvDatasetIDForDDIC;}
    int updateDDICElements(int iIndex, DSSTabularData* ipTabularData);
    int setSeperatedDDIC(string iControlKey, DSSTabularIndex *ipIndex, std::vector<DSSTabularAttribute *> ivAttr);
    int updateDDIC(string iKey);
    
    int getAttrForDDIC(string iKey, GUID &iGUID, DSSTabularUnit **oppAttr);
    int getDDICIndex(string iKey, DSSTabularIndex **oppIndex);
    int getAttrIDForDDIC(string iKey, const GUID &iGUID, int &oAttrID);
    //iLocalUnitID is the position in the local DDIC index
    int getAttrForDDIC(string iKey, int iLocalUnitID, DSSTabularUnit **oppAttr);
    int globalToDDICLocal(string iKey, GUID &iGUID, int iGlobalKey, int &oLocalKey);
    int getDDICPositionInContainer(string iKey);//get offset in the vectors, such as mvDDICIndex, mvvDDICAttr
    
    int cleanDDICCache(int iPos);
    int cleanAllDDICCache();
    
protected:
    std::map<string, int> mControlKeyToIndexMap;//[DDIC control key ==> index position]
    std::vector<DSSTabularIndex *> mvDDICIndex;// main index of DDIC view template
    std::vector<vector<DSSTabularAttribute *> > mvvDDICAttr;// local lookup of DDIC
    std::vector<DFCCSI*> mvCSIForDDIC;// CSI of DDIC, used to regenerate DDIC index and lookup
    std::vector<GUID> mvDatasetIDForDDIC;// source dataset ID of DDIC.
    
    std::vector<DSSTabularIndex *> mvLatestDDICIndex;//store partial update result
    std::vector<vector<DSSTabularAttribute *> > mvvLatestDDICAttr;//store partial update result
    
    DSSRWDataModel *mDataModel;
};


#endif
