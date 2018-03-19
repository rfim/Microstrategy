//
//  DSSCManipulationSaveAs.h
//  ReportServiceCore
//
//  Created by Zhu, Wei on 11/23/12.
//
//

#ifndef ReportServiceCore_DSSCManipulationSaveAs_h
#define ReportServiceCore_DSSCManipulationSaveAs_h

#include "DSSCManipulation.h"

class DSSCManipulationSaveAs: public DSSCManipulation
{
private:
    std::string mFolderID;
    std::string mDocName;
    std::string mDocDescription;
    int mSaveAsFlag;//lishan add EnumDSSXMLDocSaveAsFlags
public:
	DSSCManipulationSaveAs(std::string iFolderID, std::string iDocName, std::string iDocDescription,int iSaveAsFlag);
	virtual ~DSSCManipulationSaveAs();
	int Execute();
	std::string getFolderID();
    std::string getDocName();
    std::string getDocDescription();
    int getSaveAsFlag();
};

#endif
