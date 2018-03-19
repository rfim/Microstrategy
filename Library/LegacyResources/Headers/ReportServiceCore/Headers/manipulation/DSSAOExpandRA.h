//
//  DSSAOExpandRA.h
//  ReportServiceCore
//
//  Created by Sun, Mingmin on 5/9/16.
//  Copyright © 2016 MicroStrategy, Inc. All rights reserved.
//

#ifndef DSSAOExpandRA_h
#define DSSAOExpandRA_h
#include "DSSActionObject.h"

class DSSAOExpandRA : public DSSActionObject
{
private:
    std::string mNodeKey;
    int mRowOrdinal;
    int mDepth;
    bool mbExpand;
    bool mbApplyAll;
    std::vector<int> mNGBPath;
public:
    DSSAOExpandRA();
    DSSAOExpandRA(EnumManipulationCommandTypes iManipulationType);
    ~DSSAOExpandRA();
    std::vector<std::string> getArguments(int iWebServerType = 0);
    void setArguments(std::vector<std::string> args, int iWebServerType);
    void setNodeKey(std::string value);
    std::string getNodeKey();
    void setRowOrdinal(int iOrdinal);
    int getRowOrdinal();
    void setDepth(int iDepth);
    int getDepth();
    void setExpand(bool iExpand);
    bool getExpand();
    void setApplyAll(bool iApplyAll);
    bool getApplyAll();
    void setNGBPath(std::vector<int> iNGBPath);
    std::vector<int> getNGBPath();
};


#endif /* DSSAOExpandRA_h */
