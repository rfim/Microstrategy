/*
 *  DSSFileDataReader.h
 *  ReportServiceCore
 *
 *  Created by fyang on 6/8/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef FILE_DATA_READER_H
#define FILE_DATA_READER_H

#import <string>
#import <stack>
#import <vector>
#include "GUID.h"
#include "DSSrwdconstants.h"
#include "DSSBinaryDataReader.h"

#import "PDCHeader/PDCwinnt.h"
#import "MsiCOM/MsiCOM/Msi_atldef.h"
#import "MsiCOM/MsiCOM/Msi_objbase.h"
#import "MsiCOM/MsiCOM/Msi_oleauto.h"
#import "MsiCOM/MsiCOM/Msi_ccomvariant.h"

#define DATA_BLOCK_SIZE 131072   //64KB

class DSSFileDataReader : public DSSBinaryDataReader
{
public:
	//constructor and initialization functions
	DSSFileDataReader();
	virtual ~DSSFileDataReader();
	
	int Init(std::string iFilePath, int iOffset, size_t iSize);
	
	//skip some bytes of stream
	int Skip(int iSkippedBytes);
	
	int SetPosition(int iByteStartToRead);
	//create a new binary data reader with partial data content
	DSSBinaryDataReader* MakeCopy(std::string iFilePath, int iFileOffset, int start, int end);
	
	const unsigned char *getBinaryData(int iOffset, size_t iSize);
    std::string getFilePath(){return mFilePath;}
    int getStart(){return mStart;}
protected:
	
	int Read(unsigned char *pBuffer, int iRequestedBytes);
	//peek stream content without touch it
	int Peek(unsigned char *pBuffer, int iRequestedBytes);
	
	std::string mFilePath;
	int mStart;				//the start offset of this DataReader in the file
	int mLoadedBytesLeft;	//unread bytes left in current loaded block
	int mCurrentBlock;		//index of current loaded block, 0-based
	int mCurrentBlockSize;  //current block size, in most cases equals to DATA_BLOCK_SIZE, but the size of the last block could be smaller
	unsigned char* mpBuffer;//for getBinaryData
};

#endif