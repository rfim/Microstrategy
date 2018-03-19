/*
 *  DSSCExpression.h
 *  ReportServiceCore
 *
 *  Created by dong shi on 12/1/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef CEXPRESSION_H
#define CEXPRESSION_H

#import <string>
#import <map>
#import "DSSSecondClassObject.h"
#import "DSSBinaryDataReader.h"
#import "DSSAxis.h"
#import "DSSIPhoneEnums.h"
#import "DSSExpNodeOperator.h"
#import "DSSExpNodeConstant.h"

//use namespace filter_internal;
class DSSExpNode;
class DSSCExpression : public DSSSecondClassObject
{
public:
	DSSCExpression(DSSObjectContext* ipObjectContext);
    ~DSSCExpression();
	

    // weilin 2016/10/21
    DSSExpNodeOperator* getOperatorRoot();
    DSSExpNodeConstant* getConstantChild(int iIndex);
    int getOperatorRootType();
	
	DSSExpNode* addRoot(int iType, const std::string& iKey, bool iForDE = false);		//< Add the root node of the expression
	DSSExpNode* Root();											//< Get the root node of the expression
	DSSExpNode* Find(const std::string& irKey);					//< Find a node in the expression with its key
	
	void setRoot(DSSExpNode* ipNode);								//< Add the root node of the expression
    DSSExpNode* getRoot();
	void removeFromMap(DSSExpNode* ipNode);						//< Remove a whole subExpression tree from map, inclulding the indicated root node
	void Remove(const std::string& irKey);						//< Remove a node in the expression with its key
	void Clear();												//< Clear expression
	void insertToMap(const std::string& irKey, DSSExpNode* ipNode);//< Insert node and key to map
	
	std::string createNewKey(DSSExpNode* ipNode);					//< Create a new node key. If the node is provided, add this new node and key to map
	void findObjects(std::vector<DSSObjectInfo*>& oObjects);						//< for performance consideration, use reference instead of return an array
	
	int getExpresssionType();
	void setExpresssionType(int iType);
	
	int LoadDefnFromBinary(DSSBinaryDataReader* ipReader);
    int LoadDefnFromBinary2(DSSBinaryDataReader* ipReader);         // without block type
    int LoadDefnFromBinary(DSSBinaryDataReader* ipReader, bool iReadBlockType);
	void Duplicate(DSSCExpression* ipSourceExpression);				//< This method only duplicate the root node. root node will duplicate the whole expression if it's operation node
	void Clone(DSSCExpression* ipSourceExpression);					//< This method will make a deep copy of ipSourceExpression.

	void* getExpressionXML(void* ipParentNode);
    std::string getExpressionXML();
    DSSCExpression* Reverse();
    void InsertRoot(DSSExpNode* ipNewRoot); //< This method will insert the current root to ipNewRoot.
    DSSBaseElementsProxy* PopulateElementsFromExcludeExpression();
    
    
    std::string getRWNodeKey();
    void setRWNodeKey(std::string iRWNodeKey);
    
    // merge expression with special operator, it will not affect the input Expression;
    // only support and/or operator at present
    int Merge(DSSCExpression* iExpression, int iOperatorType);
    
private:
	
	void hClone(DSSExpNode* ipSoureceNode, DSSExpNode* ipDuplicatedSoureceNode);	//< Helper function called by Clone. 
	
	DSSExpNode* mRoot;
	int mSeed;
	int mExpressionType;
	std::map<std::string, DSSExpNode*> mNodeKeyMap;
    
    std::string mRWNodeKey;
};

inline int DSSCExpression::getExpresssionType()
{
	return mExpressionType;
}

inline void DSSCExpression::setExpresssionType(int iType)
{
	mExpressionType = iType;
}


inline void DSSCExpression::insertToMap(const std::string& irKey, DSSExpNode* ipNode)
{
	mNodeKeyMap[irKey] = ipNode;
}

inline void DSSCExpression::setRoot(DSSExpNode* ipNode)
{
	mRoot = ipNode;
}

inline DSSExpNode* DSSCExpression::getRoot()
{
	return mRoot;
}

inline DSSExpNode* DSSCExpression::Root()
{
	return mRoot;
}

#endif
