/*
 *  DSSExpNode.h
 *  ReportServiceCore
 *
 *  Created by dong shi on 12/1/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef EXPNODE_H
#define EXPNODE_H


#import <string>
#import <vector>
#import "DSSSecondClassObject.h"
#import "DSSBinaryDataReader.h"
#import "DSSAxis.h"
class DSSCExpression;
class DSSDimty;
class DSSExpNode: public DSSSecondClassObject
{
public:	
	DSSExpNode(DSSObjectContext* ipObjectContext, EnumDSSNodeType iNodeType, const std::string& irKey, DSSCExpression* ipExpression);
	virtual ~DSSExpNode();
	
	int getChildIndex() ;			//< get index in parent's children list	
	int Count() ;					//< children number
	EnumDSSNodeType getType() ;				//< node type
	std::string getKey() ;			//< get key.
	EnumDSSExpressionType getExpressionType() ;		// get DSSCExpression type
	
	DSSExpNode* getChildNode() ;		//< get the 1st child
    void setChildIndex(int iIndex);
	DSSExpNode* Item(int iIndex) ;	//< get indexed child
	DSSExpNode* getFirstSibling() ;	//< get 1st brother
	DSSExpNode* getLastSibling() ;	//< get last brother
	DSSExpNode* getParentNode() ;		//< get parent node
	DSSExpNode* getNext() ;			//< get next brother.
	DSSExpNode* getPrevious() ;		//< get previous brother
	DSSExpNode* getRoot(); 			//< get root node	
	DSSExpNode* findInSubTree(const std::string& iKey) ;  //< Find a node in the subexpression tree of the node, including itself.
	DSSExpNode* Find(const std::string& irKey) ;				//< Find a node in the whole tree.
	DSSExpNode* Add(int iNodeType, const std::string& irKey);	//< Add one node with type and key.
	void SetKey(const std::string& irKey);						//< Set key.
	void removeByIndex(int iIndex);							//< Remove a child and its subtree by index.
	void removeByKeyInChildren(const std::string& irKey);	//< Remove a child and its subtree by key.
	void removeByKeyInSubExpression(const std::string& irKey);//<Rmv a node from the subexpression tree under the node with its key
	void Clear();											//< clear all children
	void Move(DSSExpNode* ipTargetNode, int iTargetIndex);		//< move us to one specified node with a index.
    void Insert(DSSExpNode* ipNode); //< insert another expnode as the new child.
    
	virtual int LoadDefnFromBinary(DSSBinaryDataReader* ipReader);	//<to be done.
    int LoadDimtyFromBinary(DSSBinaryDataReader* ipReader);
    int LoadDENodeProperty(DSSBinaryDataReader* ipReader);
    void setCanExpandIsSpliced(bool iCanExpand, bool iIsSpliced);
	void setExpressionType(EnumDSSExpressionType iType);
    void setExpression(DSSCExpression* iExpression);

	virtual void Duplicate(DSSExpNode* ipSourceNode);
	
	static std::string IntToString(int i);	
	static std::string VariantToString(const CComVariant& irVariant);
	static int StringToInt(std::string& iString);
	static double StringToDouble(std::string& iString);
	static void StringToVariant(std::string& istrVariantValue, int iVariantType, CComVariant& irVariant);
	virtual void* createXML(void* ipParentNode, std::vector<DSSObjectInfo*>* ipObjects);
    
    EnumDSSNodeDimty getDimtyType();
    DSSDimty* getDimty();
	
protected:
	void hUnRegisterFromParent();
	
	std::vector<DSSExpNode*> mChildNodes;
	std::string mKey;
	int mChildIndex;				//< index into parent's children list
	DSSExpNode* mpParentNode;
	EnumDSSNodeType mType;
	DSSCExpression* mpExpression;
	EnumDSSExpressionType mExpressionType;
    
    // added for dimty
    EnumDSSNodeDimty mDimtyType;
    DSSDimty* mpDimty;
    int mID;
    
    // DE definition
    bool mIsForDE;
    bool mIsSpliced;
    bool mCanExpand;
};

//inline methods.
inline int DSSExpNode::getChildIndex() 
{
	return mChildIndex;
}
inline void DSSExpNode::setChildIndex(int iIndex)
{
    mChildIndex = iIndex;
}
inline EnumDSSNodeType DSSExpNode::getType() 
{
	return mType;
}
inline std::string DSSExpNode::getKey() 
{
	return mKey;
}
inline EnumDSSExpressionType DSSExpNode::getExpressionType() 
{
	return mExpressionType;
}
inline DSSExpNode* DSSExpNode::getParentNode() 
{
	return mpParentNode;
}
inline void DSSExpNode::setExpressionType(EnumDSSExpressionType iType)
{
	mExpressionType = iType;
}
inline void DSSExpNode::setExpression(DSSCExpression* iExpression)
{
    // only set expression, never release expression.
	mpExpression = iExpression;
}
inline void DSSExpNode::Insert(DSSExpNode *ipNode)
{
    mChildNodes.push_back(ipNode);
    ipNode->mChildIndex = (int)mChildNodes.size();
    ipNode->mpParentNode = this;
}
inline EnumDSSNodeDimty DSSExpNode::getDimtyType()
{
    return mDimtyType;
}
inline DSSDimty* DSSExpNode::getDimty()
{
    return mpDimty;
}

#endif
