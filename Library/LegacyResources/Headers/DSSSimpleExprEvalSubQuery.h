//
//  DSSSimpleExprEvalSubQuery.h
//  ReportServiceCore
//
//  Created by Taimin Liao on 2/4/13.
//  Copyright (c) 2013 MicroStrategy, Inc. All rights reserved.
//

#ifndef ReportServiceCore_DSSSimpleExprEvalSubQuery_h
#define ReportServiceCore_DSSSimpleExprEvalSubQuery_h

class DFCSimpleExpr;
class AEFilterExprNode;
struct DSSSliceID;
class DSSTabularIndex;
class DSSGUnitContainer;
class DSSFunctionServer;
class DSSCube;
class DSSTabularData;
class DSSObjectContext;

class DSSSimpleExprEvalSubQuery
{
public:
	DSSSimpleExprEvalSubQuery(DSSGUnitContainer* ipContainer, 
                               DSSFunctionServer* ipFunction, 
                               DSSObjectContext* ipSource, 
                               DSSCube *ipSourceCube, 
                               DSSCube* ipTargetCube);//, IMCEDataContainer* ipMCEDataContainer);
	~DSSSimpleExprEvalSubQuery();
    
	int EvalExpr(DFCSimpleExpr *ipExpr, DSSSliceID * opResultID, AEFilterExprNode **oppNode);
    
private:
    
	int hEvalExpr(DFCSimpleExpr *ipExpr, DSSTabularIndex** oppIndices, DSSSliceID* opResultID, DSSTabularData** oppTabularData);
    
	int hCalculateExpr(DFCSimpleExpr *ipExpr, DSSSliceID * opResultID, AEFilterExprNode **oppNode);
    
	DSSGUnitContainer* mpGUnitContainer;
	DSSFunctionServer *mpFuncServer;
	DSSObjectContext *mpSource;
	DSSCube *mpTargetCube;
	DSSCube *mpSourceCube;
};


#endif
