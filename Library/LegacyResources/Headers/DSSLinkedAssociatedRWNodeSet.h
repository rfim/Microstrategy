//
//  DSSLinkedAssociatedRWNodeSet.h
//  ReportServiceCore
//
//  Created by Li, Dong on 8/27/13.
//  Copyright (c) 2013 MicroStrategy, Inc. All rights reserved.
//

#ifndef LINKED_ASSOCIATED_RWNODE_SET_H
#define LINKED_ASSOCIATED_RWNODE_SET_H

#include <string>
#include <vector>

class DSSLinkedAssociatedRWNodeSetNode
{
public:
    DSSLinkedAssociatedRWNodeSetNode(std::string iKey);
    ~DSSLinkedAssociatedRWNodeSetNode();
    std::string getKey();
    int Count();
    DSSLinkedAssociatedRWNodeSetNode* Item(int iIndex);
    void Add(DSSLinkedAssociatedRWNodeSetNode* ipNode);
    DSSLinkedAssociatedRWNodeSetNode* find(std::string iKey);
    void linkedNodes(std::vector<std::string>& iNodeKeys); // add all the node keys into the output array
private:
    std::string mKey;
    std::vector<DSSLinkedAssociatedRWNodeSetNode*> mChildNodes;
};

class DSSLinkedAssociatedRWNodeSet
{
public:
    DSSLinkedAssociatedRWNodeSet();
    ~DSSLinkedAssociatedRWNodeSet();
    int Count();
    DSSLinkedAssociatedRWNodeSetNode* Item(int iIndex);
    void insert(std::string iKey1, std::string iKey2);
    void Clear();
    int find(std::string iKey);
    void linkedNodes(std::string iKey, std::vector<std::string>& iNodeKeys);
private:
    std::vector<DSSLinkedAssociatedRWNodeSetNode*> mNodes;
};
#endif
