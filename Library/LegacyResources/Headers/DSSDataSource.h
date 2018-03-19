//
//  DSSDataSource.h
//  ReportServiceCore
//
//  Created by Xiaolong Wang on 8/16/10.
//  Copyright 2010 MicroStrategy, Inc. All rights reserved.
//

#ifdef __ANDROID__
	#include "DSSBaseString.h"
	#include <vector>
	#include "Base/Defines/BasicTypes.h"
#else
	#import "DSSAEHelper.h"
#endif
class DSSBinaryDataReader;
class DSSDataModelBase;

class DSSRWNodeModel;
class DSSViewDataSet;
class DSSDataModelControlImpl;
#define MEM_GOVERNOR_SIZE  32768	//32KB

//reserver x Bytes memory, if y > 100KB
#define MEM_GOVERNOR_RESERVE(x, y)	long lRequestTicket = 0; \
								if (y > MEM_GOVERNOR_SIZE) \
									lRequestTicket = DataSourceUtil::ReserveSize(x);\
								if (lRequestTicket < 0)\
									return E_OUT_OF_MEMORY;

//don't return, just set hr
#define MEM_GOVERNOR_RESERVE_LOCAL(x, y)	long lRequestTicket = 0; \
								if (y > MEM_GOVERNOR_SIZE) \
									lRequestTicket = DataSourceUtil::ReserveSize(x);\
								if (lRequestTicket < 0)\
									hr = E_OUT_OF_MEMORY;


#define MEM_GOVERNOR_COMMIT(x) 	if (lRequestTicket >= 0) \
									DataSourceUtil::CommitSize(x, lRequestTicket);

//release x Bytes memory, if y > 100KB
#define MEM_GOVERNOR_RELEASE(x, y) 	if (y > MEM_GOVERNOR_SIZE) \
										DataSourceUtil::ReleaseSize(x);


#define DEBUG_SWITCH    //this is a switch to turn off log
#ifdef DEBUG_SWITCH
    #define MODEL_DEBUG_LOG(CLASS_NAME, MESSAGE, ...)\
            DataSourceUtil::AddDebugLog(CLASS_NAME, __FUNCTION__, __LINE__, this, MESSAGE, ##__VA_ARGS__);
//            DataSourceUtil::AddDebugLog(CLASS_NAME, DEBUG_MSG, __PRETTY_FUNCTION__, __FILE__, __LINE__, this);

//======sample code============
//MODEL_DEBUG_LOG("DSSRWGridModel", "this is a log with 1 parameter");
//MODEL_DEBUG_LOG("DSSRWGridModel", "this is a log", "with 2 parameters");
//MODEL_DEBUG_LOG("DSSRWGridModel", "this is a log", "with 3 parameters of", "string");
//MODEL_DEBUG_LOG("DSSRWGridModel", "this is a log", "with 3 parameters with integer", 100);
//MODEL_DEBUG_LOG("DSSRWGridModel", "this is a log", "with 3 parameters with", L"wide char",8);
#endif


class DataSourceUtil
{
#ifdef __ANDROID__
private:
	static MBase::CString mstrRootPath;
public:
	static void setRootPath(const char* ipRootPath);
#endif
public:
	static int getLocalPath(MBase::CString &orPath);
	static int createFolder(MBase::CString &orPath);
	static int removeLocalFolder();
	static long ReserveSize(int iSize);
	static void CommitSize(int iSize, long iTicket);
	static void ReleaseSize(int iSize);
	static void ReserveSize(int iSize, std::string& iComponentID);
	static void ReleaseSize(int iSize, std::string& iComponentID);
    static int removeSubFolder(MBase::CString &irPath);
    
    static bool isMappedFileSupported();
    
	/*! 
	 @abstract	save the content of a memory block to a local file.
	 @discussion
     1. if ipFilePath already exist, check iFlag. If iFlag != 0, return failure. Otherwise, append ipData to the end of the file.
     2. Since mmap requires the offset to be aligned at page size, we need return both the original mapped filed memory address and the adjusted memory address.
	 */
    static int createMappedFileBuffer(int nBlockCount, const void ** ippDataBlocks, int* ipBlockSize, const char* ipFilePath, bool* iobUseNewFile, void **oppBuffer, int *opBuferSize, void **oppBufAligned, int* opOffset = NULL, int* opOffsetAligned = NULL);
    
    static int releaseMappedFileBuffer(void *ipBuffer, int iBufSize);
    
    static int reloadMappedFileBuffer(const char* ipFilePath, int length, int offset, int offsetAligned, void **oppBuffer, void **oppBufAligned);
    
    // tehe: iFlag == 0 means create time, 1 means modify time
    static int GetTimeIntervalSince1970(const char * ipFilePath, int iFlag, Int64 & oCreateTime);
    
    static int RemoveOldFiles(const char *ipDir,int iDay);
    
    // tehe 7/10/2013
    static int GetPageSize();
    
    //version1: 3 messages, all strings, the last two are optional
    static int AddDebugLog(const char * className, const char  * method, int line, void *ipThis, const char * message1, const char * message2 = NULL, const char * message3 = NULL);
    
    //version2: 3 messages, 2 strings, and the 3rd one is integer
    static int AddDebugLog(const char * className, const char  * method, int line, void *ipThis, const char * message1, const char * message2, int message3);
    
    //version3: 3 messages, 2 strings, and the 3rd one is wide char string with its size
    static int AddDebugLog(const char * className, const char  * method, int line, void *ipThis, const char * message1, const char * message2, const wchar_t * message3, unsigned int wchar_string_size);
    
};

class DSSDataSource
{
public:
	DSSDataSource();
	
	~DSSDataSource();
	
	/*! 
	 @abstract	Init data source with the binary data range [start, end) of ipSourceBinary
	 @discussion The data is not loaded until needed. So this function only remember where
				 to load(irPath), and how many to load (end - start).
	 @param ipSourceBinary	The Binary data reader, it has the binary data loaded from file 
	 param BinaryOffset The offset within binary file	
	 param start	The offset within ipSourceBinary
	 param end	The offset within ipSourceBinary
	 param nSlices	number of slices
	 param irvSliceOffset	The offset within ipSourceBinary for each data slice
	 param irPath	The binary file path
	 param ibMakeCopy if the binary file is not persist, we need make a local copy
	 */
	int Init(DSSBinaryDataReader *ipSourceBinary, int BinaryOffset, int start, int end,
			 int nSlices, int* ipvSliceOffset, int inSliceOffset, const MBase::CString &irPath, bool ibMakeCopy,bool ibFieldGroup = false);
    
	int Init(DSSBinaryDataReader *ipSourceBinary, int start, int end, MBase::CString &irPath, bool ibMakeCopy);
    
	inline DSSBinaryDataReader * getBinaryReader(){return mpBinaryReader;};
	
	int LoadData(DSSDataModelBase * ipModel, int iSlice, bool &orbLoadNewSlice);
	
	inline bool IsAllLoaded(){return mnLoadedSlices == mnSlices;};

	int LoadModel(DSSDataModelBase * ipModel);
	
	int size();
	
	void Reset();
	
	//return the start point in the binary file
	inline int getFileOffset(){return mFileOffset;};

    // 5/22/2012 tehe
    int LoadNewRWDModel(DSSRWNodeModel *ipNodeModel);
	
    // 5/21/2012 tehe
    int LoadSliceDataForNewRWD(DSSViewDataSet * ipViewDataSet, int iSlice/*, bool &orbLoadNewSlice*/);
    // tehe: we should be sure that we can clean the binary data before call this method
    void Clean();
    
    int InitForNewRWD(DSSBinaryDataReader *ipSourceBinary, int BinaryOffset, int start, int end,
                      int nSlices, std::vector<int> &irvSliceOffset, const MBase::CString &irPath, bool ibMakeCopy);

    //for field group optimization
    int LoadFieldData(DSSDataModelBase * ipModel,std::vector<int>& rvSlices);
	
	static	int ReadFileContent(std::string &irPath, int offset, int length, unsigned char** oppData);
    
    int createMMapMemory(const char * ipFilePath, bool& ibUseNewFile);
    int MapFileBuffer();
    int UnMapFileBuffer();
    
    //to load DDIC without its parent model
    int LoadInputControlModel(DSSDataModelBase* ipModel, std::string iKey, DSSDataModelControlImpl** oppControl);
    
    // 11/1/2012 tehe
    MBase::CString getFileDirectory();
    
    
    //zhyang
    MBase::CString getDataSourcePath(){return mLocalFilePath;};
private:
	
	int hGetFileDescriptor(MBase::CString &irPath, FILE **oppFileHandle, int &orFD);
	
	int hReadFileContent(MBase::CString &irPath, int offset, int length, char * & orpFileContent);
	
	DSSBinaryDataReader *mpBinaryReader;
	
	std::vector<int> mvOffset;
    int * mpOffset;
    int mnOffsets;
    
    std::string mMappedFilePath;
    void * mpMMPBuffer;
    int mMMPBufSize;
    int mOffsetAligned;
    int mOffset;
	
	int mnSlices;
	int mnLoadedSlices;
//	std::vector<bool> mvbIsLoaded;	//loaded or not
    bool *mpbIsLoaded;
	
	MBase::CString mLocalFilePath;
	int mTotalBytes;
	int mFileOffset;			//the start position of data source in the binary file
    
    
};


class DSSHeaderSource
{
public:
	DSSHeaderSource();
	
	~DSSHeaderSource();

    int Init(DSSBinaryDataReader *ipSourceBinary, int BinaryOffset, int start, int end,
             int iRTUnitOnRow, std::vector<int> iMapRTUnitOnRow2GBUnit, int iRTUnitOnCol, std::vector<int> iMapRTUnitOnCol2GBUnit,
			 const MBase::CString &irPath, bool ibMakeCopy);
    
	inline DSSBinaryDataReader * getBinaryReader(){return mpBinaryReader;};
   
    int LoadHeaderKeys(DSSDataModelBase* ipModel, std::vector<int>& rvWindowID);
	
	int size();
	

private:

	//read content from local file, for android client
	int hReadFileContent(MBase::CString &irPath, int offset, int length, char * & orpFileContent);
	
	DSSBinaryDataReader *mpBinaryReader;
	
    int mRowStartPos;
    int mColStartPos;
    
    int mRuntimeUnitOnRow;
    std::vector<int> mMapRuntimeUnitOnRow2GBUnit;
    
    int mRuntimeUnitOnCol;
    std::vector<int> mMapRuntimeUnitOnCol2GBUnit;
    
    int mTotalBytes;
	int mFileOffset;			//the start position of data source in the binary file
    
	MBase::CString mLocalFilePath;
	bool mbLocalFile;
};

