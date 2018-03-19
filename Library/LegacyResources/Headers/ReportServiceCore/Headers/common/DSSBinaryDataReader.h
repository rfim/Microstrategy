/*
 *  DSSBinaryDataReader.h
 *  DashboardViewer
 *
 *  Created by Xiaolong Wang on 12/28/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef BINARY_DATA_READER_H
#define BINARY_DATA_READER_H

#import <string>
#import <stack>
#import <vector>
#include "GUID.h"
#include "DSSCOMMaster/DSSCOM_TLB_INC.H"
#include "DSSrwdconstants.h"

#import "PDCHeader/PDCwinnt.h"
#import "MsiCOM/MsiCOM/Msi_atldef.h"
#import "MsiCOM/MsiCOM/Msi_objbase.h"
#import "MsiCOM/MsiCOM/Msi_oleauto.h"
#import "MsiCOM/MsiCOM/Msi_ccomvariant.h"

#define BINARY_WRITER_BLOCK_SIZE 4096

namespace MDataType {
	class DSSBigDecimal;
	class DSSDateTime;
    // 2/24/2012 tehe support dynamic time for financial package
    class DSSDynamicTime;
    class DSSTimeSpan;
}

class DSSBinaryDataReader
{
public:
	//constructor and initialization functions
	DSSBinaryDataReader();
	virtual ~DSSBinaryDataReader();
	int Init(unsigned char * ipByteArray, size_t size, bool ibTakeOwnership = true);
	
	virtual int Init(std::string iFilePath, int iOffset, size_t iSize);
	
	//shouldn't be used anymore
	int Assign(unsigned char * ipByteArray, size_t size);
	
	void Clean();
	
	virtual const unsigned char *getBinaryData(int iOffset, size_t iSize);
	inline const unsigned char *getBinaryData(){return mpData;};
	
	inline int getBytesCount(){return mTotalBytes;};
    
	//read primitive data types
	int ReadBool(bool *ipVal);
	int ReadByte(BYTE *ipVal);
	int ReadUnsignedByte(unsigned char *ipVal);
	int ReadShort(short *ipVal);
	int ReadUnsignedShort(unsigned short *ipVal);
	int ReadInt(int *ipVal);
	int ReadUnsignedInt(unsigned int *ipVal);
	int ReadInt64(long long *ipVal);
	int ReadLong(long *ipVal);
	int ReadFloat(float *ipVal);
	int ReadDouble(double *ipVal);
    
	//read string, bigdecimal and DSSDateTime object
	int ReadStringLen(unsigned int *ipVal);
	int ReadUTF8String(char *pBuffer, unsigned int iBufSize);
	int ReadString(std::string &rVal);
	int ReadWString(std::wstring &rVal);
	int ReadBigDecimal(MDataType::DSSBigDecimal *ipVal);
	int ReadDate(MDataType::DSSDateTime *ipVal);
	int ReadGUID(GUID &irVal);
	
	//read arrays
	int ReadByteArray(char *ipVal, int iEleCount);
	int ReadByteArray(unsigned char *ipVal, int iEleCount);
	int ReadByteArray(std::vector<char> &rVal);
	int ReadIntArray(int *ipVal, int iEleCount);
	int ReadIntArray(std::vector<int> &rVal);
	int ReadFloatArray(float *ipVal, int iEleCount);
	int ReadDoubleArray(double *ipVal, int iEleCount);
	int ReadShortArray(short *ipVal, int iEleCount);
	// read format ref ID
	int ReadRefID(int *ipVal);
	void setRefIDOffset(int iOffset);
	int getRefIDOffset();
	
	int ReadHeader();
	int BeginReadBlock(int *BlockType);
	int EndReadBlock();
	bool HasDataAvailable();
	bool MoreDataInBlock();
	
	//check if the given block header exist at the current position.
	bool ProbeBlockHeader(short iBlockType);
	
	int setEndian(int iVal);
	int getEndian(int *ipVal);
	inline bool IsLittleEndian(){return mIsLittleEndian;};
	inline void setIsLittleEndian(int i){mIsLittleEndian = i;};
	int Read(unsigned char *pBuffer, int iRequestedBytes, int iTypeSize);
	//peek stream content without touch it
	int Peek(unsigned char *pBuffer, int iRequestedBytes, int iTypeSize);
	//skip some bytes of stream
	virtual int Skip(int iSkippedBytes);
	
	int GetPosition();
	virtual int SetPosition(int iByteStartToRead);
	
	int ReadTypedString(unsigned short iType, std::string &rVal);
	int ReadVariant(CComVariant& rVal);
	int ReadVariantBool(VARIANT_BOOL *ipVal);
	int ReadBSTRVariant(CComVariant& rVal);
    
	//create a new binary data reader with partial data content
	virtual DSSBinaryDataReader* MakeCopy(std::string iFilePath, int iFileOffset, int start, int end);
	DSSBinaryDataReader* MakeCopy(int start, int end);
    int getType(){return mType;}
    
    virtual std::string getFilePath(){return "";};
    virtual int getStart(){return 0;};
    
    // 11/23/2012 tehe: this method should be called directly after BeginReadBlock method
    int getCurrentBlockSize();
protected:
	int hConvertBigEndianToLittleEndian(int iTypeSize, unsigned char *pVal, int iRequestedBytes);
    
	virtual int Read(unsigned char *pBuffer, int iRequestedBytes);
	//peek stream content without touch it
	virtual int Peek(unsigned char *pBuffer, int iRequestedBytes);
	
	//data members
	int mIsLittleEndian;
	int mBytesLeft;
	int mBytesRead;
	int mTotalBytes;
	unsigned char * mpData;	//pointer to original binary data, unit8_t = unsigned char
	unsigned char * mpStream;	//pointer to current position to be read
	std::stack<int> mBlockSizeStack;
	int mRefIDOffset;
	bool mbTakeOwnership;
    int mType; //0-binaryReader, 1-fileReader
};

template <typename TBinDataReader>
class DSSReadBlockStream: public ICDSSReadBlockStream {
public:
    DSSReadBlockStream(TBinDataReader& iBinDataReader):
    mBinDataReader(iBinDataReader), mpWStrData(NULL) {}
    virtual ~DSSReadBlockStream()
    {
        if(mpWStrData)
        {
            delete mpWStrData;
            mpWStrData = NULL;
        }
    }
    
    virtual HRESULT STDMETHODCALLTYPE ReadBoolean(VARIANT_BOOL *ipVal) {
        return mBinDataReader.ReadVariantBool(ipVal);
    }
    virtual HRESULT STDMETHODCALLTYPE ReadShort(short * ipVal) {
        return mBinDataReader.ReadShort(ipVal);
    }
    virtual HRESULT STDMETHODCALLTYPE ReadShortArray(unsigned Int32 iEleCount,
                                                     short * ipVal)  {
        return mBinDataReader.ReadShortArray(ipVal, iEleCount);
    }
    virtual HRESULT STDMETHODCALLTYPE ReadInt(Int32 * ipVal) {
        return mBinDataReader.ReadInt(ipVal);
    };
    virtual HRESULT STDMETHODCALLTYPE ReadInt64(Int64 * ipVal) {
        return mBinDataReader.ReadInt64(ipVal);
    };
    virtual HRESULT STDMETHODCALLTYPE ReadIntArray(unsigned Int32 iEleCount,
                                                   Int32 * ipVal) {
        return mBinDataReader.ReadIntArray(ipVal, iEleCount);
    };
    virtual HRESULT STDMETHODCALLTYPE ReadFloat(float * ipVal) {
        return mBinDataReader.ReadFloat(ipVal);
    };
    virtual HRESULT STDMETHODCALLTYPE ReadFloatArray(unsigned Int32 iEleCount,
                                                     float * ipVal) {
        return mBinDataReader.ReadFloatArray(ipVal, iEleCount);
    };
    virtual HRESULT STDMETHODCALLTYPE ReadDouble(double *ipVal) {
        return mBinDataReader.ReadDouble(ipVal);
    }
    virtual HRESULT STDMETHODCALLTYPE ReadDoubleArray(unsigned Int32 iEleCount,
                                                      double * ipVal) {
        return mBinDataReader.ReadDoubleArray(ipVal, iEleCount);
    }
    virtual HRESULT STDMETHODCALLTYPE ReadByte(char * ipVal) {
        return mBinDataReader.ReadByteArray(ipVal, 1);
    };
    virtual HRESULT STDMETHODCALLTYPE ReadByteArray(unsigned Int32 iEleCount,
                                                    char * ipVal) {
        return mBinDataReader.ReadByteArray(ipVal, iEleCount);
    };
    
    virtual HRESULT STDMETHODCALLTYPE ReadChar(wchar_t * ipVal) {
        _ASSERT(false);
        return S_OK;
    };
    virtual HRESULT STDMETHODCALLTYPE ReadStringLength(unsigned Int32 * ipVal) {
    
#ifndef NO_WCHAR_SUPPORT
        if(mpWStrData)
        {
            delete mpWStrData;
            mpWStrData = NULL;
        }
        Int64 lLen;
        mBinDataReader.ReadInt64(&lLen);
        Int64 lMask;
        mBinDataReader.ReadInt64(&lMask);
        
        _ASSERT(lLen == ~lMask);
        
        unsigned char* lCharData = new unsigned char [lLen+1];
        memset(lCharData, 0, lLen + 1);
        
        mpWStrData = new wchar_t [lLen + 1];
        memset(mpWStrData, 0, sizeof(wchar_t) * (lLen + 1));
        HRESULT rVal = mBinDataReader.ReadByteArray(lCharData, lLen);
        
        MBase::ReturnWString lWStr = MBase::UTF8ToWideChar((const char*)lCharData);
        int lWlen = lWStr.GetLength() > lLen ? lLen : lWStr.GetLength();
        
        (*ipVal) = static_cast<unsigned Int32> (lWlen);
        
        //std::mbstowcs(&mpWStrData[0], (const char*)lCharData, lLen);
        memmove(mpWStrData, lWStr.c_str(),lWlen * sizeof(wchar_t));

        
        delete[] lCharData;
        
        return rVal;
#else
        return E_NOTIMPL;
#endif
    }
    virtual HRESULT STDMETHODCALLTYPE ReadString(unsigned Int32 iEleCount,
                                                 wchar_t * ipVal) {
        if(mpWStrData)
        {
            memmove(ipVal, mpWStrData, iEleCount*sizeof(wchar_t));
            delete mpWStrData;
            mpWStrData = NULL;
            return S_OK;
        }
        else
        {
            Int64 lLen;
            mBinDataReader.ReadInt64(&lLen);
            Int64 lMask;
            mBinDataReader.ReadInt64(&lMask);
            
            _ASSERT(lLen == ~lMask);
            
            unsigned char* lCharData = new unsigned char [lLen+1];
            HRESULT rVal = mBinDataReader.ReadByteArray(lCharData, iEleCount);
            if(ipVal)
            {
                for(Int32 i=0; i<iEleCount; ++i)
                {
                    unsigned char bytes[sizeof(wchar_t)] = {0};
                    bytes[sizeof(wchar_t)-1] = lCharData[i];
                    memmove(ipVal+i, bytes, sizeof(wchar_t));
                }
                delete[] lCharData;
                return rVal;
            }
            delete[] lCharData;
            return rVal | S_FALSE;
        }
    }
    virtual HRESULT STDMETHODCALLTYPE ReadBSTR(BSTR * ipVal) {
        std::wstring lstr;
        return mBinDataReader.ReadWString(lstr);
    }
    virtual HRESULT STDMETHODCALLTYPE ReadGUID(DSS_ID * ipVal) {
        if(ipVal)
            return mBinDataReader.ReadGUID(*ipVal);
        return S_FALSE;
    }
    virtual HRESULT STDMETHODCALLTYPE ReadGUIDArray(unsigned Int32 iEleCount,
                                                    DSS_ID * ipVal) {
        int rVal = S_OK;
        for(size_t i=0; i<iEleCount && rVal == S_OK; ++i)
            rVal |= mBinDataReader.ReadGUID(*(ipVal+i));
        return rVal;
    }
    virtual HRESULT STDMETHODCALLTYPE ReadSafeInt(Int32 * ipVal) {
        return S_FALSE;
    }
    virtual HRESULT STDMETHODCALLTYPE ReadVariant(VARIANT * ipVal) {
        CComVariant lVariant;
        int rVal = mBinDataReader.ReadVariant(lVariant);
        if(rVal == S_OK)
            rVal |= lVariant.Detach(ipVal);
        return rVal;
    }
    virtual HRESULT STDMETHODCALLTYPE ReadBlockHeader(short * ipVal) {
        int lType, rVal;
        // wyuan; 954998; Check MoreDataInBlock() as Server does.
        bool lVBool = mBinDataReader.MoreDataInBlock();
        if(lVBool)
        {
            rVal = mBinDataReader.BeginReadBlock(&lType);
            if(rVal == S_OK && ipVal)
                *ipVal = lType;
        }
        else
        {
            rVal = S_FALSE;
        }
        return rVal;
        
    };
    virtual HRESULT STDMETHODCALLTYPE FinishReadBlock() {
        return mBinDataReader.EndReadBlock();
    };
    virtual HRESULT STDMETHODCALLTYPE IsThereData(VARIANT_BOOL * ipVal) {
        if(ipVal)
            *ipVal = mBinDataReader.HasDataAvailable();
        return S_OK;
    }
    virtual HRESULT STDMETHODCALLTYPE Rewind() {
        return S_OK;
    }
    virtual HRESULT STDMETHODCALLTYPE get_Size(unsigned Int32 * ipVal) {
        if(ipVal)
            *ipVal = mBinDataReader.getBytesCount();
        return S_OK;
    }
    
    virtual HRESULT STDMETHODCALLTYPE MoreDataInBlock(VARIANT_BOOL * ipVal) {
        if(ipVal)
            *ipVal = mBinDataReader.MoreDataInBlock();
        return S_OK;
    }
    virtual HRESULT STDMETHODCALLTYPE get_StreamVersion(Int32 * ipVal) {
        return S_OK;
    }
    virtual HRESULT STDMETHODCALLTYPE put_StreamVersion(Int32) {
        return S_OK;
    };
private:
    TBinDataReader& mBinDataReader;
    wchar_t* mpWStrData;
    
};

class DSSBinaryDataWriter
{
public:
	DSSBinaryDataWriter();
	virtual ~DSSBinaryDataWriter();
	int Init(int iBlockSize = BINARY_WRITER_BLOCK_SIZE);
    
	int Write(const unsigned char *pBuffer, int iSize);
	int WriteInt(int iInt);
    int WriteShort(short iShort);
    int WriteInt64(long long iInt64);
    int WriteByte(char iByte);
    int WriteFloat(float iFloat);
    int WriteDouble(double iDouble);
    int WriteGUID(DSS_ID iGUID);
    int WriteBool(bool iBool);
    int WriteUnsignedInt(unsigned int iInt);
    int WriteUnsignedShort(unsigned short iShort);
	int WriteByteArrayAtOffset(int iOffset, const unsigned char* ipBuffer, int iSize);
	int WriteIntAtOffset(int iOffset, int iInt);
    int WriteInt64AtOffset(int iOffset, long long iInt64);
	int getBinary(unsigned char** oppBinary, int* opSize);
	int GetPosition();
    
    // tehe: some time we want to return the address where we write
    int WriteInt(int iInt, const unsigned char **oppAddress);
    int WriteByteArrayAtOffset(int iOffset, const unsigned char* ipBuffer, int iSize, const unsigned char **oppAddress);
    int WriteBlockHeader(short iType);
    int FinishWriteBlock();
    bool IsEmpty();
    int GetSize();
private:
	int mBlockSize;
	int mTotalBytes;
	unsigned char * mpData;
	int mSize;
    int mOffset;
	bool mIsLittleEndian;
    bool mbIsInitialized;
    std::stack<long long> mBlockPtrStack;
	std::stack<int> mBlockPtr32Stack;
	std::stack<long long> mBlockSizeStack;
	std::stack<unsigned short> mBlockTypeStack;
};



template <typename TBinDataWriter>
class DSSWriteBlockStream: public ICDSSWriteBlockStream {
public:
    DSSWriteBlockStream(TBinDataWriter& iBinDataWriter):
    mBinDataWriter(iBinDataWriter) {}
    
    virtual HRESULT STDMETHODCALLTYPE WriteBoolean(VARIANT_BOOL iVariantBool){
        bool iBool = iVariantBool == VARIANT_TRUE? true:false;
        return mBinDataWriter.WriteBool(iBool);
    }
    virtual HRESULT STDMETHODCALLTYPE WriteShort(short iShort){
        return mBinDataWriter.WriteShort(iShort);
    }
    virtual HRESULT STDMETHODCALLTYPE WriteShortArray(short * iShortArray,
                                                      unsigned Int32 iSize){
        //TODO: implement
        return S_OK;
    }
    virtual HRESULT STDMETHODCALLTYPE WriteInt(Int32 iInt){
        return mBinDataWriter.WriteInt(iInt);
    }
    virtual HRESULT STDMETHODCALLTYPE WriteInt64(Int64 iInt){
        return mBinDataWriter.WriteInt64(iInt);
    }
    virtual HRESULT STDMETHODCALLTYPE WriteIntArray(Int32 * iIntArray,
                                                    unsigned Int32 iSize){
        //TODO: implement
        return S_OK;
    }
    virtual HRESULT STDMETHODCALLTYPE WriteFloat(float iFloat){
        return mBinDataWriter.WriteFloat(iFloat);
    }
    virtual HRESULT STDMETHODCALLTYPE WriteFloatArray(float * iFloatArray,
                                                      unsigned Int32 iSize){
        //TODO: implement
        return S_OK;
    }
    
    virtual HRESULT STDMETHODCALLTYPE WriteDouble(double iDouble){
        return mBinDataWriter.WriteDouble(iDouble);
    }
    virtual HRESULT STDMETHODCALLTYPE WriteDoubleArray(double * iDoubleArray,
                                                       unsigned Int32 iSize){
        //TODO: implement
        return S_OK;
    }
    virtual HRESULT STDMETHODCALLTYPE WriteByte(char iChar){
        return mBinDataWriter.WriteByte(iChar);
    }
    
    virtual HRESULT STDMETHODCALLTYPE WriteByteArray(char * iCharArray,
                                                     unsigned Int32 iSize){
        return mBinDataWriter.Write((const unsigned char*)iCharArray, iSize);
    }
    
    virtual HRESULT STDMETHODCALLTYPE WriteChar(wchar_t iChar){
        //TODO: implement
        return S_OK;
    }
    

    virtual HRESULT STDMETHODCALLTYPE WriteString(wchar_t * iWString){
 
#ifndef NO_WCHAR_SUPPORT
        //Not Sure yet
        std::string lString = MBase::WideCharToUTF8(iWString).c_str();
        /*
        long long lSize = wcslen(iWString);
        mBinDataWriter.WriteInt64(lSize);
        long long lMask = ~lSize;
        mBinDataWriter.WriteInt64(lMask);
        char* lStr = new char[lSize];
        wcstombs(lStr, iWString, lSize);*/
        //DE6172
        long long lSize = lString.size();
        mBinDataWriter.WriteInt64(lSize);
        long long lMask = ~lSize;
        mBinDataWriter.WriteInt64(lMask);
        const char* lStr = lString.c_str();
        HRESULT hr = mBinDataWriter.Write((const unsigned char*)lStr, lSize);
        //delete lStr;
        return hr;
#else
        return E_NOTIMPL;
#endif
    }
    
    
    virtual HRESULT STDMETHODCALLTYPE WriteBSTR(BSTR iStr){
        //TODO: implement
        return S_OK;
    }
    virtual HRESULT STDMETHODCALLTYPE WriteGUID(DSS_ID * iGUID){
        DSS_ID lGUID = iGUID ? *iGUID : GUID_NULL;
        return mBinDataWriter.WriteGUID(lGUID);
    }
    
    virtual HRESULT STDMETHODCALLTYPE WriteGUIDArray(DSS_ID * iID,
                                                     unsigned Int32 iSize){
        //TODO: implement
        return S_OK;
    }
    
    virtual HRESULT STDMETHODCALLTYPE SafeWriteInt(Int32 iInt){
        //TODO: implement
        return S_OK;;
    }
    virtual HRESULT STDMETHODCALLTYPE WriteVariant(VARIANT * iVar){
        //TODO: implement
        return S_OK;
    }
    
    virtual HRESULT STDMETHODCALLTYPE WriteBlockHeader(short iType){
        return mBinDataWriter.WriteBlockHeader(iType);
    }
    virtual HRESULT STDMETHODCALLTYPE FinishWriteBlock(){
        return mBinDataWriter.FinishWriteBlock();
    }
    virtual HRESULT STDMETHODCALLTYPE FinishWriteBlockEx(short iShort){
        //TODO: implement
        return S_OK;
    }
    virtual HRESULT STDMETHODCALLTYPE IsEmpty(VARIANT_BOOL * ipVal){
        if(ipVal)
        {
            *ipVal = mBinDataWriter.IsEmpty()? VARIANT_TRUE : VARIANT_FALSE;
        }
        return S_OK;
    }
    virtual HRESULT STDMETHODCALLTYPE Rewind(){
        //TODO: implement
        return S_OK;
    }
    virtual HRESULT STDMETHODCALLTYPE get_Size(unsigned Int32 * ipVal){
        if(ipVal)
            *ipVal = mBinDataWriter.GetSize();
        return S_OK;
    }
    
private:
    TBinDataWriter& mBinDataWriter;
    
};


#endif
