//==============================================================================================
//	FILENAME	:	DatasetHelper.h
//	AUTHOR		:	Dong Shi
//	CREATION	:	1/25/10
//	Copyright (C) MicroStrategy, Inc. 2008
//==============================================================================================
#ifndef DATASETHELPER_H
#define DATASETHELPER_H

#include "../Dataset.h"
#include "FormatInfo.h"
#include <set>

#ifndef _VE_CE
#import "DatasetIOSHelper.h"
#define DSSTEMPLATE DSSTemplate
#define DSSTEMPLATEMETRIC DSSTemplateMetric
#define DSSRWGRIDITERATOR DSSRWGridIterator
#define DSSTEMPLATECUSTOMGROUP DSSTemplateCustomGroup
#else
#define DSSTEMPLATE ICDSSTemplate
#define DSSTEMPLATEMETRIC ICDSSTemplateMetric
#define DSSRWGRIDITERATOR ICDSSRWIterator
#define DSSTEMPLATECUSTOMGROUP ICDSSTemplateCustomGroup
#endif
/************************************************************************************************
 *		This Header File Includes All the Helper Classes Referrenced By DatasetImpl             *
 ************************************************************************************************/

typedef enum
{
	BSFT_RESOLVED = -1,
	BSFT_DATETIME = 1,
	BSFT_DATE = 8,
	BSFT_TIME = 9,
	BSFT_UNDEFINED = -2,
}BASE_FORMAT_TYPE;

typedef enum
{
	TM_INVALID = -1,
	TM_YEAR = 0,
	TM_MONTH = 1,
	TM_DAY = 2,
	TM_TIME = 3
}TM_LABEL_PART;

typedef enum
{
    LINK_TEXT_FULL,
    LINK_TEXT_LINK,
    LINK_TEXT_DISPLAY,
}LINK_TEXT_TYPE;

class DSSTemplateMetric;
class DSSCustomGroup;
class DSSTemplateCustomGroup;
class DSSTemplateDimension;
namespace MsiChart
{
	const double kNodeSizeFactor = 100.0;
    ////////////////   For Data Formating Optimization	////////////////

	//class MDataType::DSSDateTime;
	class DimTree;
	struct TMLocaleInfo;
	class FormattedDataString {
    public:
		class DateTimeInfo {
		public:
			DateTimeInfo(): mpData(NULL), mpNumberCategory(NULL), mpNumberFormat(NULL)
            {
			}
			~DateTimeInfo(){
			}

		public:
			const void* mpData;
			Int32 mSize;
			EnumDSSDataType mType;
			DSSDataFlag mFlag;
			Int32 mBaseFormType;
			CComVariant* mpNumberCategory;
			CComVariant* mpNumberFormat;
		};

		class DateTimeIndex {
		public:
			DateTimeIndex(): mpData(NULL), mpLocale(NULL), mIndex(-1){
			}
			DateTimeIndex(const void* ipData, TMLocaleInfo* ipLocale, Int32 iIndex): mpData(ipData), mpLocale(ipLocale), mIndex(iIndex){
			}
			~DateTimeIndex(){
			}

			bool operator == (const DateTimeIndex& x) const;

			bool operator != (const DateTimeIndex& x) const
			{
				return !(*this == x);
			}

		public:
			const void* mpData;
			TMLocaleInfo *mpLocale;
			Int32 mIndex;
		};

	public:

		FormattedDataString(const FormattedDataString &x) : mpDateTimeIndex (NULL), mpDateTimeInfo(NULL), mText(L"")
#ifdef _VE_CE
#else
        ,mpFormattedDataStringHelper(NULL)
#endif
        {
			if (x.mpDateTimeIndex != NULL) {
				mpDateTimeIndex = new DateTimeIndex(*(x.mpDateTimeIndex));
			}
			if (x.mpDateTimeInfo != NULL) {
				mpDateTimeInfo = new DateTimeInfo(*(x.mpDateTimeInfo));
			}
			mText = x.mText;
            mIsLink = x.mIsLink;
#ifdef _VE_CE
#else
            mpFormattedDataStringHelper = new FormattedDataStringHelper(this);
#endif
		}

		FormattedDataString& operator= (const FormattedDataString &x)
		{
			if (this != &x) {
				if (mpDateTimeIndex != NULL) {
					delete mpDateTimeIndex;
				}
				if (x.mpDateTimeIndex != NULL) {
					mpDateTimeIndex = new DateTimeIndex(*(x.mpDateTimeIndex));
				}
				else {
					mpDateTimeIndex = NULL;
				}

				if (mpDateTimeInfo != NULL) {
					delete mpDateTimeInfo;
				}
				if (x.mpDateTimeInfo != NULL) {
					mpDateTimeInfo = new DateTimeInfo(*(x.mpDateTimeInfo));
				}
				else {
					mpDateTimeInfo = NULL;
				}

				mText = x.mText;
                mIsLink = x.mIsLink;
			}
			return *this;
		}

		FormattedDataString() : mText(L""), mpDateTimeInfo(NULL), mpDateTimeIndex(NULL), mIsLink(false)
#ifdef _VE_CE
#else
        ,mpFormattedDataStringHelper(NULL)
#endif
        {
#ifdef _VE_CE
#else
            mpFormattedDataStringHelper = new FormattedDataStringHelper(this);
#endif
		}

		FormattedDataString(const void* ipData, EnumDSSDataType iType, DSSDataFlag iFlag, Int32 iSize, Int32 iBaseFormType, FormatInfo& iFormat, CComVariant* ipNumberCategory, CComVariant* ipNumberFormat) : mText(L""), mpDateTimeInfo(NULL), mpDateTimeIndex(NULL), mIsLink(false)
#ifdef _VE_CE
#else
        ,mpFormattedDataStringHelper(NULL)
#endif
		{
#ifdef _VE_CE
#else
            mpFormattedDataStringHelper = new FormattedDataStringHelper(this);
#endif
			SetData(ipData, iType, iFlag, iSize, iBaseFormType, iFormat, ipNumberCategory, ipNumberFormat);
		}

		FormattedDataString(const void* ipData, TMLocaleInfo* ipLocale, Int32 iIndex) : mText(L""), mpDateTimeInfo(NULL), mpDateTimeIndex(NULL), mIsLink(false)
#ifdef _VE_CE
#else
        ,mpFormattedDataStringHelper(NULL)
#endif
		{
#ifdef _VE_CE
#else
            mpFormattedDataStringHelper = new FormattedDataStringHelper(this);
#endif
			SetData(ipData, ipLocale, iIndex);
		}

		FormattedDataString(const MBase::String &iText) : mText(iText), mpDateTimeInfo(NULL), mpDateTimeIndex(NULL), mIsLink(false)
#ifdef _VE_CE
#else
        ,mpFormattedDataStringHelper(NULL)
#endif
		{
#ifdef _VE_CE
#else
            mpFormattedDataStringHelper = new FormattedDataStringHelper(this);
#endif
		}

		FormattedDataString(const std::string &iText) : mText(MBase::UTF8ToWideChar(iText.c_str()).c_str()), mpDateTimeInfo(NULL), mpDateTimeIndex(NULL), mIsLink(false)
#ifdef _VE_CE
#else
        ,mpFormattedDataStringHelper(NULL)
#endif
		{
#ifdef _VE_CE
#else
            mpFormattedDataStringHelper = new FormattedDataStringHelper(this);
#endif
		}

		~FormattedDataString()
		{
			if (mpDateTimeInfo != NULL)
				delete mpDateTimeInfo;
			if (mpDateTimeIndex != NULL)
				delete mpDateTimeIndex;
#ifdef _VE_CE
#else
            if(mpFormattedDataStringHelper != NULL)
            {
                delete mpFormattedDataStringHelper;
                mpFormattedDataStringHelper = NULL;
            }
#endif
		}

		void SetData(const void* ipData, EnumDSSDataType iType, DSSDataFlag iFlag, Int32 iSize, Int32 iBaseFormType, FormatInfo& iFormat, CComVariant* ipNumberCategory, CComVariant* ipNumberFormat);
		void SetData(const MBase::String& irText);
		void SetData(const std::string& irText);
		void SetData(const void* ipData, TMLocaleInfo* ipLocale, Int32 iIndex);

		MBase::String GetData();
		MBase::String GetLink();

		bool operator == (const FormattedDataString& irDataString) const;

		bool operator != (const FormattedDataString& irDataString) const
		{
			return !(*this == irDataString);
		}


        static void ResolveLinkText(const MBase::String& irBaseStr, MBase::String& iorText, LINK_TEXT_TYPE iWantType = LINK_TEXT_DISPLAY);

        DateTimeIndex* GetDateTimeIndex();
        void SetDateTimeIndex(DateTimeIndex* ipDateTimeIndex);
        DateTimeInfo* GetDateTimeInfo();
        void SetDateTimeInfo(DateTimeInfo* ipDateTimeInfo);
        void SetFormattedDataStringText(const MBase::String& iText);
        MBase::String GetFormattedDataStringText();
        bool IsLink();
        void SetIsLink(bool iIsLink);
        MBase::String mText;
	private:
		DateTimeInfo* mpDateTimeInfo;
		DateTimeIndex* mpDateTimeIndex;
        bool mIsLink;
#ifdef _VE_CE
#else
        FormattedDataStringHelper *mpFormattedDataStringHelper;
#endif
	};


    ////////////////   For Slicing Support	////////////////

	//            ---0. DataPiece---			//
	class DataPiece
	{
	public:
		//ctor
		DataPiece(SLICE_TYPE iType, Dataset* ipParent);
		virtual ~DataPiece();

		//Exposed for Client Callers
		virtual HRESULT GetData(Int32 iSeriesID, Int32 iGroupID, const double*& iorValue, Int32 iColumnOffset = 0);
		virtual HRESULT GetSeriesCount(Int32& orSeriesCount);
		virtual HRESULT GetGroupCount(Int32& orGroupCount);

		//Mapping
		virtual Int32 MapSreiesToParentalDataset(Int32 iSeriesIndex);
		virtual Int32 MapGroupToParentalDataset(Int32 iGroupIndex);
		virtual void MapIndexToParentalDataset(Int32 iSeriesIndex, Int32 iGroupIndex, Int32& orSeriesIndex, Int32& orGroupIndex);

		//Slice Type
		virtual SLICE_TYPE GetSliceType();
		virtual void SetSliceType(SLICE_TYPE iType = SLICE_STEPWISE);

		//Piece Update
		virtual bool Update(Int32 iBeinSeries, Int32 iEndSeries, Int32 iBeginGroup, Int32 iEndGroup);
		virtual bool Update(const std::vector<Int32>& irSeriesIndex, const std::vector<Int32>& irGroupIndex);

		//Get Info like Range
		virtual bool GetRange(Int32& iBeinSeries, Int32& iEndSeries, Int32& iBeginGroup, Int32& iEndGroup);
		virtual bool GetRange(std::vector<Int32>& orSeriesIndex, std::vector<Int32>& orGroupIndex);
		virtual bool GetGroupRange(Int32& orBeginGroup, Int32& orEndGroup);
		virtual bool IsValid();
	protected:
		SLICE_TYPE mType;	///< Sliced Type
		bool mIsValid;      ///< This flag denotes the validity of the data piece object, if it is ture, then this object is ready for later use. otherwise 1) It may contain no data(0 group 0 series), 2) Its mother dataset is invalid.
		Dataset* mpParent;  ///< Dataset instance pointer
	};

	//            ---1. DataSlice---			//
	class DataSlice : public DataPiece
	{
	public:
		DataSlice(Int32 iBeginGroup, Int32 iEndGroup, Int32 iBeginSeries, Int32 iEndSeries, Dataset* ipParent, SLICE_TYPE iType = SLICE_STEPWISE);


		virtual HRESULT GetSeriesCount(Int32& orSeriesCount);
		virtual HRESULT GetGroupCount(Int32& orGroupCount);

		//Map index info back to parental dataset.
		virtual Int32 MapSreiesToParentalDataset(Int32 iSeriesIndex);
		virtual Int32 MapGroupToParentalDataset(Int32 iGroupIndex);
		virtual void MapIndexToParentalDataset(Int32 iSeriesIndex, Int32 iGroupIndex, Int32& orSeriesIndex, Int32& orGroupIndex);

		//Update
		virtual bool Update(Int32 iBeinSeries, Int32 iEndSeries, Int32 iBeginGroup, Int32 iEndGroup);

		//Get info
		virtual bool GetRange(Int32& iBeinSeries, Int32& iEndSeries, Int32& iBeginGroup, Int32& iEndGroup);
		virtual bool GetGroupRange(Int32& orBeginGroup, Int32& orEndGroup);

	private:
		void hTrimBoundary(Int32 iSeriesCount, Int32 iGroupCount);
		bool hCheckValidity();

		Int32 mBeginGroup;
		Int32 mEndGroup;
		Int32 mBeginSeries;
		Int32 mEndSeries;
	};


	//            ---2. DataStrip---			//
	class DataStrip : public DataPiece
	{
	public:
		//Ctor
		DataStrip(const std::vector<Int32>& irSeriesIndex, const std::vector<Int32>& irGroupIndex, Dataset* ipParent);

		//Exposed For Clinet Callers
		virtual HRESULT GetSeriesCount(Int32& orSeriesCount);
		virtual HRESULT GetGroupCount(Int32& orGroupCount);

		//Map index info back to parental dataset.
		virtual Int32 MapSreiesToParentalDataset(Int32 iSeriesIndex);
		virtual Int32 MapGroupToParentalDataset(Int32 iGroupIndex);
		virtual void MapIndexToParentalDataset(Int32 iSeriesIndex, Int32 iGroupIndex, Int32& orSeriesIndex, Int32& orGroupIndex);

		//Update
		virtual bool Update(const std::vector<Int32>& irSeriesIndex, const std::vector<Int32>& irGroupIndex);

		//Get info
		virtual bool GetRange(std::vector<Int32>& orSeriesIndex, std::vector<Int32>& orGroupIndex);
		virtual bool GetGroupRange(Int32& orBeginGroup, Int32& orEndGroup);

	private:

		bool hPickValidIndex(Int32 iInput, Int32 iInputUpperLimit, Int32 iInputLowerLimit = 0);
		void hUpdate(const std::vector<Int32>& irSeriesIndex, const std::vector<Int32>& irGroupIndex);

		std::vector<Int32> mSeriesIndex;
		std::vector<Int32> mGroupIndex;
	};


	//For Tree Structure Support
	/**
	 *	Node information in a Dimensional tree
	 */
	class DimNode
	{
	public:
		/**
		 * Constructor of a Dimentional Node
		 * @param iDim The dimension index for this node
		 * @param iIndex The index within the specified dimension
		 */
		DimNode(Int32 iDim,Int32 iIndex):mDim(iDim),
		mIndex(iIndex),
		mpParent(NULL),
		mStartChild(0),
		mGroupPos(-1.0),
		mOrdinal(-1),
		mShowNode(true),
		mOffspringCount(0)
		{}

		/**
		 * Desctruct the tree structure, release the space
         */
		~DimNode();

		/**
		 * Judge whether the node is a leaf node or not
		 * @Return true: leaf node false: not leaf node
		 */
		bool IsLeafNode();

		/**
		 * Judge whether the node support bracket(has more than one children)
		 * @Return true: support bracket false: not support bracket
		 */
		bool IsSupportBracket();

		/*
		 *Return the right most node of the subtree rooted at the current node.
		 */
		DimNode* RightMostOffspring();

		/*
		 *Treverse to the right bottom most leaf node while collecting the path info.
		 */
		void RightMostOffspringForPathInfo(std::vector<Int32>& iorPathInfo);

		/**
		 * Get the Left or Top most child node of this node
		 * @Return Left or Top most node
		 */
		DimNode* LeftOrTopMost(){return mChildren.front();}
		/**
		 * Get the Right or Bottom most child node of this node
		 * @Return Right or Bottom most node
		 */
		DimNode* RightOrBottomMost(){return mChildren.back();}


        Int32 mDim;							///< The dimensionality of the node
		Int32 mIndex;						///< The index in this dimensionality
		Int32 mOrdinal;						///< The ordinal for this node - only used for leafnode
		float mGroupPos;					///< Each node's group index
		std::vector<DimNode*> mChildren;	///< Children nodes
		DimNode* mpParent;					///< The node's parent
		bool mShowNode;						///< Whether the node is shown or not
		Int32 mWidth;						///< The size of child    ---dshi
		Int32 mOffspringCount;				///< the leafnode coverd by the subtree rooted at this node --dshi
		Int32 mStartChild;                  ///< the index of its first child into the layer it stays at.
	};


	/////////////////////////////////

    //Unit Info Structure//

	/////////////////////////////////
	struct UnitInfo
	{
		UnitInfo()
		{
			mName = L"";
			mIndexInAxis = 0;
			mType = DssTemplateReserved;
			mIsValid = false;
		}

		UnitInfo(MBase::String& iName, GUID& iID, Int32 iIndex, Int32 iType) : mID(iID), mIndexInAxis(iIndex), mType(iType), mName(iName)
		{
			mIsValid = true;
		}
	public:
		MBase::GUID GetID() {return mID;}
		Int32 GetIndex() {return mIndexInAxis;}
		Int32 GetType() {return mType;}
		MBase::String GetName() {return mName;}
		Int32 mIndexInAxis;
		bool mIsValid;
	protected:
		MBase::GUID mID;
		Int32 mType;
		MBase::String mName;
	};

	typedef std::pair<MBase::GUID, MBase::String> ElementInfo;

	class DimTree;
	struct MetricUnitInfo: public UnitInfo
	{
		typedef MSynch::SmartPtr<MetricUnitInfo> Ptr;

		MetricUnitInfo(MBase::String& iName, MBase::GUID& iID, Int32 iIndex, Int32 iType) : UnitInfo(iName, iID, iIndex, iType)
		{
			mIsInRowAxis = false;
			mMetricNumber = 0;
		}

		MetricUnitInfo(DSSTEMPLATE* ipTemplate)
		{
			mIsInRowAxis = false;
			mMetricNumber = 0;
			mIsValid = hPopulateMetricInfo(ipTemplate);
		}
		void AddOneElement(const MBase::GUID& iID, const MBase::String& iName) {mElements.push_back(std::make_pair(iID, iName));}
		void GetOneElement(MBase::GUID& orID, MBase::String& orName, Int32 iIndex)
		{
			_ASSERTE( iIndex >= 0 && iIndex < mElements.size());
			orID = mElements[iIndex].first;
			orName = mElements[iIndex].second;
		}
		GUID GetMetricID(Int32 iIndex)
		{
			_ASSERTE(iIndex >= 0 && iIndex < mElements.size());
			return mElements[iIndex].first;
		}
		DSSTemplateMetric* GetTemplateMetric(MBase::GUID& irID);
		MBase::String GetMetricName(Int32 iIndex)
		{
			return mElements[iIndex].second;
		}

		void GetElementNames(std::vector<MBase::String>& orElements)
		{
			orElements.clear();
			size_t lSize = mElements.size();
			for (size_t i = 0; i < lSize; i++)
				orElements.push_back(mElements[i].second);
		}
		Int32 GetElementNumber(){return (Int32)mElements.size();}
		Int32 MetricIDToTemaplateMetricIndex(GUID& irID);
        Int32 GetMemoryUsageEstimation()
        {
            Int32 lSize = sizeof(*this);

            Int32 lCount = (Int32)mOccupyPairSet.size();
            Int32 kSize = sizeof(std::pair<Int32, Int32>);
            lSize += lCount * kSize;

            lCount = (Int32)mIDToTemplateMetric.size();
            kSize = sizeof(MBase::GUID) + sizeof(DSSTemplateMetric*);
            lSize += lCount * kSize;

            lCount = (Int32)mElements.size();
            kSize = sizeof(ElementInfo);
            lSize += lCount * kSize;

            return lSize;
        }

		bool mIsInRowAxis;
		bool mHasMetrics;
		Int32 mMetricNumber;
	protected:
		std::vector<ElementInfo> mElements;
		std::vector<std::pair<Int32, Int32> > mOccupyPairSet; //<width, index>
		std::map<MBase::GUID, DSSTEMPLATEMETRIC*> mIDToTemplateMetric;
	private:
		bool hPopulateMetricInfo(DSSTEMPLATE* ipTemplate);

	};

	//CustomGroupUnitInfo
	typedef enum
	{//to identify custom group levels ()
		CUSTGRP_UNDEFINED = -1,
		CUSTGRP_ELEMENT = 5,
		CUSTGRP_BANDING = 7,
		CUSTGRP_INDIVIDUAL_ITEM = 11,
	}CUSTOM_GROUP_LEVEL;
	//CustomGroupUnitInfo impl
	struct CustomGroupUnitInfo: public UnitInfo
	{
		typedef MSynch::SmartPtr<CustomGroupUnitInfo> Ptr;

		Int32 mCustomGroupNum[2];//0 for rows, 1 for columns

		CustomGroupUnitInfo(MBase::String& iName, MBase::GUID& iID, Int32 iIndex, Int32 iType) : UnitInfo(iName, iID, iIndex, iType)
		{
			mCustomGroupNum[0] = mCustomGroupNum[1] = 0;
		}
		CustomGroupUnitInfo(DSSRWGRIDITERATOR* ipIterator)
		{
			mCustomGroupNum[0] = mCustomGroupNum[1] = 0;
			mIsValid = hPopulateCustomGroupUnitInfo(ipIterator);
		}
		//iAxis = 0(Row) or 1(Column)
		DSSTEMPLATECUSTOMGROUP* getCustomGroupInAxis(int iAxis, int iDepth, CUSTOM_GROUP_LEVEL& iLevel);//not fully implemented
		std::vector<std::pair<Int32, Int32> > getCustomGroupPositions(bool iIsRow);
		bool hasCustomGroupInRow(){return (mCustomGroupNum[0]>0);}
		bool hasCustomGroupInColumn(){return (mCustomGroupNum[1]>0);}
	protected:
		std::vector<std::pair<Int32, Int32> > mOccupyPairSet[2];				//<beginIndex endIndex> -- [bgin, end)
		std::map<Int32, MBase::GUID> mCustomGroupIndexToID[2];					//<CGIndex, ID>
		std::map<MBase::GUID, DSSTEMPLATECUSTOMGROUP*> mIDToTemplateCustomGroup;
		//format info pool
		std::map<Int32, std::pair<GUID, Int32> > mCustomGroupIndexToElement[2];
	private:
		bool hPopulateCustomGroupUnitInfo(DSSRWGRIDITERATOR* mpIterator);
	};
	//CustomGroupUnitInfo~

	//Forward Declaration, each unit should contain its type/ID/Name/StartIndex/Width
	/**
	 *	Dimensional tree structure
	 */
	class DimTree
	{
	public:

		typedef std::vector<FormattedDataString>  StringForms;
		typedef std::vector<FormattedDataString>*  StringFormsPtr;
		typedef std::vector<std::vector<std::vector<FormattedDataString> >* > FormStringPool;


		/**
		 * Construct the dimensional tree, allocate the space for Label Container
		 */
		DimTree() : mpLocale(NULL)
		{
			// Create root node
			mRoot.Reset(new DimNode(-1,-1));
			// Assure the root node is not null
			_ASSERT(!mRoot.IsNull());
		}
		~DimTree();

		void Delete()
		{
			delete this;
		}

		void AddLevel()
		{
			mLabelCon.mDimString.push_back(FormStringPool::value_type());
		}

		float GetMemoryUsageEstimation()
		{
			return mLeafNodes.size() * kNodeSizeFactor;
		}

        void GetStringPathToRoot(std::vector<MBase::String>& orPath, DimNode* ipNode)
        {
            DimNode* lpNode = ipNode;
            while(lpNode != mRoot.Get() && lpNode)
            {
                MBase::String lNodeString = GetFormattedString(lpNode, true).c_str();
                orPath.push_back(lNodeString);
                lpNode = lpNode->mpParent;
            }
        }

		//viewer will get it directly from template
		MBase::String GetTemplateUnitName(Int32 iDimLayer, bool iUseAlias = false, bool iIsSqueezeHCG = true)
		{
			_ASSERTE(iDimLayer >= 0 && iDimLayer < mLabelCon.mDimString.size());
			if (!iIsSqueezeHCG)
			{
				return mAttributes[iDimLayer];
			}
			else
			{
				Int32 lDepthIndex = mUnitIndexToStartDepthIndex[iDimLayer];
				return mAttributes[lDepthIndex];
			}
		}

		MBase::String GetTemplateUnitElementName(Int32 iDimLayer, Int32 iIndexInsideDim, Int32 iFormIndex = 0, MBase::String iSeperator = L" ", bool iIsSqueezeHCG = true)
		{
			MBase::String lStr = L"";
			Int32 lSize = (Int32)mLabelCon.mDimString.size();

			if (!iIsSqueezeHCG || (iIsSqueezeHCG && mHCGMap.find(iDimLayer) == mHCGMap.end()))
			{

				if(iIsSqueezeHCG) //Squeeze, but not HCG, only one column to take.
					iDimLayer = mUnitIndexToStartDepthIndex[iDimLayer]; //convert to depth

				if (iDimLayer < 0 || iDimLayer >= lSize)
					return lStr;

				std::vector<DimNode*> lpNodesVec = GetDimNodes(iDimLayer, false);
				if (iIndexInsideDim < 0 || iIndexInsideDim >= lpNodesVec.size())
					return lStr;

				DimNode* lpNode = lpNodesVec[iIndexInsideDim];
				return mLabelCon.GetString(lpNode, iFormIndex, iSeperator);
			}
			else //squeezed and a HCG
			{
				_ASSERTE(mHCGMap.find(iDimLayer) != mHCGMap.end()); //a HCG

				Int32 lWidth = mHCGMap[iDimLayer].second;
				Int32 lStartDepth = mHCGMap[iDimLayer].first;
				Int32 lEndDepth = lStartDepth + lWidth - 1;

				if (lEndDepth < 0 || lEndDepth >= lSize || lStartDepth < 0 || lStartDepth >= lSize)
					return lStr;

				std::vector<DimNode*> lpNodesVec = GetDimNodes(lEndDepth, false);
				if (iIndexInsideDim < 0 || iIndexInsideDim >= lpNodesVec.size())
					return lStr;

				DimNode* lpNode = lpNodesVec[iIndexInsideDim];
				std::vector <MBase::String> lvParts;
				while (lWidth > 0)
				{
					MBase::String lPart = mLabelCon.GetString(lpNode, iFormIndex, iSeperator);
					lvParts.insert(lvParts.begin(), lPart);

					lpNode = lpNode->mpParent;
					lWidth--;
				}

				Int32 lvSize = (Int32)lvParts.size();
				_ASSERTE(lSize >= 2);
				for (Int32 i = 0; i < lvSize - 1; i++)
					lStr += lvParts[i] + iSeperator;
				lStr += lvParts.back();
				return lStr;
			}
		}

		Int32 GetTreeDepth(bool iIsSqueezeHCG = true)
		{
			if (!iIsSqueezeHCG)
				return (Int32)mLabelCon.mDimString.size();
			else
			{
				return (Int32)mUnitIndexToStartDepthIndex.size();
			}
		}

		Int32 GetTreeWidth(Int32 iDimLayer, bool iIsActualSize = false, bool iIsSqueezeHCG = true)
		{
			if (!iIsSqueezeHCG || (iIsSqueezeHCG && mHCGMap.find(iDimLayer) == mHCGMap.end()))
			{
				if (iIsSqueezeHCG)
					iDimLayer = mUnitIndexToStartDepthIndex[iDimLayer];
			}
			else //squeeze HCG
			{
				_ASSERTE(mHCGMap.find(iDimLayer) != mHCGMap.end());
				Int32 lStartDepth = mHCGMap[iDimLayer].first;
				Int32 lWidth = mHCGMap[iDimLayer].second;
				_ASSERTE(lWidth >= 2);
			    Int32 lEndDepth = lStartDepth + lWidth - 1;
				iDimLayer = lEndDepth;
			}

			_ASSERTE(iDimLayer >= 0 && iDimLayer < GetTreeDepth(false));
			if(iIsActualSize)
				return (Int32)GetDimNodes(iDimLayer, false).size();
			else
				return (Int32)mLabelCon.mDimString[iDimLayer]->size();
		}

		Int32 GetLeafNodesByRoot(Int32 iDimLayer, Int32 iIndexInsideDim, bool iIsSqueezeHCG = true)
		{
			if (!iIsSqueezeHCG || (iIsSqueezeHCG && mHCGMap.find(iDimLayer) == mHCGMap.end()))
			{
				if (iIsSqueezeHCG)
					iDimLayer = mUnitIndexToStartDepthIndex[iDimLayer];
			}
			else
			{
				_ASSERTE(mHCGMap.find(iDimLayer) != mHCGMap.end());
				Int32 lStartDepth = mHCGMap[iDimLayer].first;
				Int32 lWidth = mHCGMap[iDimLayer].second;
				iDimLayer = lStartDepth + lWidth - 1;
			}
			_ASSERTE(GetDim(false) > iDimLayer);
			_ASSERTE(GetDimNodes(iDimLayer, false).size() > iIndexInsideDim);
			return GetDimNodes(iDimLayer, false)[iIndexInsideDim]->mOffspringCount;
		}

		/**
		 *	Inner class, Dimensional string container
		 */
		class DimStringContainer
		{
		public:
			/**
			 * Constuct the Dimensional String Container
			 * @param iMaxLevel: The dimensional string container's level
			 */
			DimStringContainer();
			/**
			 *	Destructor, release the container's space
		     */
			~DimStringContainer()
			{
				std::vector<std::vector<std::vector<FormattedDataString> >* >::iterator pos = mDimString.begin();
				for(;pos!=mDimString.end();++pos)
				{
					if((*pos)!=NULL)
					{
						delete (*pos); (*pos) = NULL;
					}
				}
			}

			/**
			 *	Find the string's index, if not exist return -1
			 *	@param iDimIndex The dimensional index
			 *	@param ipString The string value
			 *	@return Find the string's index, if not exist return -1
			 */
			Int32 Find(Int32 iDimIndex, StringFormsPtr ipString);

			/**
			 *	Add the string to the container, return its index
			 *	@param iDimIndex The dimensional index
			 *	@param ipString The string value
			 *	@return add the string to the container, return its index
			 */
			Int32 Add(Int32 iDimIndex, StringFormsPtr ipString);

			/**
			 *	Return the string with DimNode information
			 *	@param ipNode Dimensional node information
			 *  @param iType denote which part should be retrieved if it is a link
			 *	@return the string with DimNode information
			 */
			MBase::String GetString(DimNode* ipNode, Int32 iFormIndex = -1, MBase::String lSeperator = L" ", LINK_TEXT_TYPE iType = LINK_TEXT_DISPLAY);  //[enhanced for forms]

			/**
			 *	Return the string with DimNode information
			 *	@param iDim Dimensional index along the row/column axis
			 *	@param iIndex the index value inside one dimension
			 *	@param  Dimensional node information
			 *  @param iType denote which part should be retrieved if it is a link
			 *	@return the string with DimNode information
			 */
			MBase::String GetString(Int32 iDim, Int32 iIndex, Int32 iFormIndex = -1, MBase::String lSeperator = L" ", LINK_TEXT_TYPE iType = LINK_TEXT_DISPLAY); ///< Added by dshi

			/**
			 *	Return the dimensionality within this container
			 *	@return the dimensionality
			 */
			unsigned Int32 GetDim()
			{
				unsigned Int32 lDim = 0;
				std::vector<std::vector<std::vector<FormattedDataString> >* >::iterator pos = mDimString.begin();
				while(pos!=mDimString.end() && (*pos)!=NULL)
				{
					lDim++;		++pos;
				}
				return lDim;
			}
		public:

			/// Data used for storing label string information
			//We make the mDimString a 3 dimentional matrix to support forms of string.
			//D1 : DimIndex, D2: IndexInsideOneDim, D3: FormIndex.
			FormStringPool mDimString;
		};


		/// Time Series Decider: used for time series
		bool IsLastAttrOfDateStyle();



		/// Traverse the tree, get each node's GroupIndex
		void LevelTraverse();
		/// Return number of dimension
		unsigned Int32 GetDim(bool iIsSqueezeHCG = true);
		/// return all the nodes
		std::vector<DimNode*> GetDimNodes(unsigned Int32 iDimIndex, bool iIsSqueezeHCG = true);
		/// Get Leaf Count(Group Count)
		unsigned Int32 GetLeafCount() const;
		/// Get plain label with specified GroupIndex
		MBase::String GetPlainLabels(unsigned Int32 iGroupIndex, bool iSuppressLastLevel = false, LABEL_LEVEL_OPTION iOption = LLO_NORMAL);
		MBase::String GetFormattedString(DimNode* ipNode, bool iIsAbbreviated = false); ///< Get Formatted Label String Piece
		bool IsLastAttrToDisplay(DimNode* ipNode); //Hard-coded to be deleted or revised in future.

		/**Build the label whose last piece is at leaf node indexed by iIndex, according to the iOption.
         @param iIndex, the index of the leaf node which is the last piece of the result string.
         @param iOption, indicate which part of the labels will be used.
         */
		MBase::String BuildLabelWithOption(Int32 iIndex, LABEL_LEVEL_OPTION iOption);

	private:
		/// Recursive call to get the nodes in that dimension
		void hRecGetNodes(DimNode* ipNode,Int32 iDimIndex, std::vector<DimNode*>& iorCon);
		///	Recursive traverse the tree structure
		void hRecLevelTraverse(DimNode* ipNode);
	    MBase::String hAbbreviateText(Int32 iDim, const MBase::String& irLabelPiece);
	public:
		DimStringContainer mLabelCon;				///< Dimensional String
		MBase::StrongPtr<DimNode> mRoot;			///< Root node for the tree
		std::vector<DimNode*> mLeafNodes;			///< Leaf nodes, used for getting plain labels
		std::vector<MBase::String> mAttributes;		///< Attr Names, added by dshi.  //for phrase 1
		std::vector<Int32> mWidths;					///< Width for each unit.
		std::vector<GUID> mIDs;						///< Template Unit IDs For this Axis. //for phrase 2.
		MetricUnitInfo::Ptr mMetricPtr;				///< MetricInfo, could be null.
		std::map<Int32, Int32> mDepthToUnitIndex;	///< Depth index to unit index
		std::map<Int32, Int32> mUnitIndexToStartDepthIndex;			///< unit index --> its start index.
		std::map<Int32, std::pair<Int32, Int32> > mHCGMap;			///< Hierachical CG unit  [UnitIndex  ==> <startDepth, width>]
		std::vector<Int32> mFormStyles;				///< the formtype for displayed form, used in time series.
		TMLocaleInfo* mpLocale;						///< For postponed date/time formatting     --xxiao
	};













	//For Drill Support
	/**
	 *	Token Manager's implementation, to decide where the token is along
	 *  Pre-assumtion: the metric should be the last unit along the column axis.
	 *  Warning: this class is only useful when the input tree is valid, and it should be a row tree.
	 */
	class TokenManager
	{
	public:
		//Ctor & dtor
		TokenManager();
		~TokenManager();
		void Populate(DimTree* ipRowTree, DimTree*  ipColTree);

		//Movement
		void ResetToken();
		bool MoveDownToken(Int32 iIndex);
		bool MoveUpToken();

		//Window Setting, shold be followed by refresh of dataset if called
		bool SetRowColIndex(Int32 iRowIndex, Int32 iColIndex);

		//All the Getor methods
		MBase::String GetAttributeName();
		bool GetRowHeader(MBase::String& orLabel);							 //Get the row header for CURRENT level
		bool GetRowHeadersForChildren(std::vector<MBase::String>& orLabels); //Get the row header for its children. Note, only row header, not
		bool GetAttributeName(MBase::String& orAttrName);					 //Get Attr Name for ONE LAYER below
		Int32 GetLevel();
		Int32 GetIndexInsideDim();
		Int32 GetRowIndex();
		Int32 GetColIndex();

		//Misc
		bool IsAtBottomLevel();									//At the last level above leafs
		bool IsDataWindowReady();								//Whether data window is set
		bool ToColumnRange(Int32& orBegin, Int32& orEnd);		//Get the range of columns

		bool mIsValid;
	private:
		Int32 mDim;     ///<For Position in the Tree : dim index
		Int32 mIndex;   ///<For Position in the Tree: index along the dim
		Int32 mRowDimLimit; ///< Layer Count
		Int32 mRowPathIndex;///< For Data Slicing :
		Int32 mColPathIndex;///< For Data Slicing :
		DimTree* mpRowTree;	///< Should be the row tree.
		DimTree* mpColTree;	///< Should be the col tree.
		bool mIsWindowValid; //user set the window by calling set rowcolindex
		bool mIsPivot;
	};

	//TMLocaleInfo
	/*********************************************************
     *= This is a helper struct to get locale info to forma  =*
     *= the x-axis labels in Time Series Chart;				=*
     *********************************************************/
	struct TMLocaleInfo
	{
		//for time series
		void* mpLocale;
		std::string mDateFormatRule;
		std::string mTimeFormatRule;
		void* mpDateFormatter;
		bool mIsIncludeTime;
		std::vector<std::string> mDateSeperators;
		int mDateOrderType;

		//methods:
		TMLocaleInfo(Int32 iPartsNumber);
		~TMLocaleInfo();
		void Populate();
		void ConvertToDateTimeParts(const void* ipDateData, std::vector<std::string>& iorParts);
		void FormatYMD(const std::string& iFullString, std::vector<std::string>& iorParts);
        MBase::String GetDateByDateOrderType(MBase::String &irYear,MBase::String &irMonth , MBase::String &irDay ,MBase::String irHour = L"00",MBase::String irMinute = L"00",MBase::String irSecond = L"00");
#ifdef _VE_CE
#else
    private:
        TMLocaleInfoHelper *mpTMLocaleInfoHelper;
#endif
	};
#ifndef _VE_CE
	//DimTreePopulator
	/*********************************************************
     *= This is a static class to  create dim tree instance, =*
     *= Usage: DimTreePopulator::ToTree(XXXXX);				=*
     *********************************************************/
	class DimTreePopulator
	{
	public:
		//Method 1: get a normal tree
		static	DimTree* ToTree(bool iIsForRow, DSSTemplate* ipTemplate, DSSRWGridIterator* ipGridIterator, Int32 iBeginLayer = 0, Int32 iEndLayer = -1, bool iIsShowSubtotal = true);

		//Method 2: get a time series tree
		static  DimTree* ToTimeSeriesTree(bool iIsForRow, DSSTemplate* ipTemplate, DSSRWGridIterator* ipGridIterator, Int32 iBeginLayer = 0, Int32 iEndLayer = -1, bool iIsShowSubtotal = true);

	protected:
		//helper methods : to build normal tree
		static DimTree* hPopulateTree(DSSRWGridIterator* ipGridIterator, DSSTemplate* ipTemplate, bool iIsRow, Int32 iBeginLayer, Int32 iEndLayer, bool iIsShowSubtotal = true); //[iBeginLayer, iEndLayer)
		static bool hTraverseTreeForCompletion(DimNode* ipNode, DimTree* ipTree);

		//helper methods : to build time series tree.
		//1. if of time sieres, return how many layers to parse.
		//2. if not, return -1.
		static Int32 hIsUnitForTimeSeries(DSSTemplateUnit* ipUnit);
		static DimTree* hPopulateTimeSeriesTree(bool iIsForRow, DSSTemplateUnit* iDateUnit, DSSTemplate* ipTemplate, DSSRWGridIterator* ipGridIterator, Int32 iTreeDepth);
		static Int32 hBuildSubtreeOnDate(DSSRWGridIterator* ipGridIterator, DimTree*& ipTree, Int32 iPreOrdinal, Int32 iOridanlCount, Int32 iFormId,
                                         bool iIsForRow,  bool iIsIncludingTime);
		static bool hHasRepeatedElementAt(DSSRWGridIterator* ipGridIterator, Int32 iAxisId, Int32 iDepth);
		/**
		 Build the time series tree structure with the Dim node.
		 @param ipCurrentNode, previously constructed node at current dim, it will be updated if it is replaced by the node to add.
		 @param	ipParentNode, parental node to this node.
		 @param iDimIndex, dim index
		 @param ipContentToAdd, previously used content of ipCurrentNode
		 @param ipCurrentContent, content of candidate node.
		 @param ipTree, the tree instance
		 @param iDimPathChagned, true means the previous dim has different dim node path, in this case the candidate node will be added anyway.
		 on the other case it is the candidate node's content that will decide wether it will be added to the tree.
		 @return true if the candidate is added, and false otherwise.
		 */
		static bool hBuildOneDimNodeOnDate(DimNode** ipCurrentNode, DimNode* ipParentNode, int iDimIndex, FormattedDataString* ipContentToAdd, FormattedDataString* ipCurrentContent, DimTree* ipTree, bool iDimPathChagned);
	};


	/***************************
     ****DimTree Populator***
     **************************/
	struct DimTreeCreator
    {
        Int32 mBeginLayer;
        Int32 mEndLayer;

		Int32 mBeginDepth;			//[Note : we should build tree by depth, not by unit.]
		Int32 mEndDepth;
		Int32 mCurrentDepth;
		Int32 mAxisId;

        bool mHasRunTimeUnit;
        bool mIsForRow;
        bool mShowSubtotalValue;  //important.

        DSSTemplate* mpTemplate;
        DSSRWGridIterator* mpGridIterator;

        DimTreeCreator();
		~DimTreeCreator();
        DimTree* CreateTree();

		std::map<Int32, Int32> mDepthToUnitIndex; //depth index to unit index.
		std::map<Int32, Int32> mUnitIndexToStartDepthIndex; //UnitIndex --> firstDepthIndex for this unit, only CG can take multiple depths.
		FormatInfo mFormatInfo;
		CComVariant mNumberCategory;
		CComVariant mNumberFormat;
		CComVariant* mpNumberCategory;
		CComVariant* mpNumberFormat;
        //following public method are from protected for code re-organization
		bool GetElementContent(std::vector<FormattedDataString>&orForms, DSSTemplateUnit* ipUnit, Int32 iDepth);
        FormattedDataString GetElementStringPiece(DSSTemplateUnit* ipUnit, const void* ipData, EnumDSSDataType iType, DSSDataFlag iFlag, Int32 iSize, Int32 iBaseFormType);
        bool GetAttrElementContent(std::vector<FormattedDataString>&orForms, DSSTemplateAttribute* ipAttr, Int32 iDepth, bool iIsSubtotal); //#7        bool hMoveIteratorByElement(Int32 iDepth, bool iIsWithBoundaryCheck, bool iIsForward);    //To replace the NextXXXNByElement
        bool GetElementContentFromDimension(std::vector<FormattedDataString>&orForms, DSSTemplateDimension* ipUnit, Int32 iDepth);
    protected:
        bool hPopulateOneDim(DimTree* ipTree, DSSTemplateUnit* ipUnit, Int32 iDepth,
                             std::vector<DimNode*>& irParentalSet,
							 std::vector<std::pair<Int32, Int32> >& irParentalOccypySet, bool iIsBuiltByOrdinal = false);

		bool hMoveIteratorByElement(Int32 iDepth, bool iIsWithBoundaryCheck, bool iIsForward);    //To replace the NextXXXNByElement
        bool hMoveIteratorByElement(Int32 iDepth, bool iIsForward);                //To replace the NextXXXNByElement
		/***To Get the elements distrubution at layer(i+1) where iDepth = i.
         @orOccupySet is a set of element distribution, it is a vector of paris <Number_Of_Elements, Starting_Ordianl>, note we may exclude subtotals
         @iDepth, the parental layer index.
         */
		void hGetChildCountSet(std::vector<std::pair<Int32, Int32> >& orOccupySet, Int32 iDepth, bool iIsBuiltByOrdinal = false);            //To replace getChildCount
        bool hMoveIteratorToPosition(Int32 iPosition);
        Int32 hCountElementsInRange(Int32 iDepth, Int32 lStart, Int32 lEnd);
		void hGetFormatString(DSSTemplateUnit* ipUnit);
		bool hIsHierarchicalCustomGroup(DSSTemplateUnit* ipUnit);

    private:
        DSSAxis* hGetAxis();
		void hPopulateDepthMap(DSSAxis* ipAixs);
		void hPopulateSubtotalMap(DSSAxis* ipAxis);

		std::vector<Int32> hFilterOutSubtotal(Int32 iDimIndex, Int32 iStart, Int32 iEnd);


		//void hGetLocaleRelatedInfo();  //for time series
		void hPopulateLocaleRelatedInfo();
        DimTreeCreatorHelper *mpDimTreeCreatorHelper;
    };
#endif
	/********************/
	/*  DateTime Parser */
	/********************/
	struct DateTimeParser
	{
	public:
		DateTimeParser(MBase::String& irString);
		DateTimeParser(std::string& irString);
		DateTimeParser(Int32 iYear, Int32 iMonth, Int32 iDay);
		DateTimeParser(Int32 iYear, Int32 iMonth, Int32 iDay, Int32 iHour, Int32 iMinute, Int32 iSecond, bool iIsIncludingTime);
		DateTimeParser(const std::string& iDay, const std::string& iMonth, const std::string& iYear, const std::string& iTime);

		MBase::String ToYear(bool iIsShorten);
		MBase::String ToDay(bool iIsShorten);
		MBase::String ToMonth(bool iIsDigital, bool iIsShorten = false);
		MBase::String ToMonth(bool iIsShorten) const;
		MBase::String ToFormattedDate(Int32 iFormatType, const MBase::String iSerperator = L"-", bool iIsShorten = true, Int32 iTimeLenght = 5);
		MBase::String ToTime(bool iIsShorten, Int32 iLength);
		bool IsValid();
		static std::map<MBase::String, MBase::String> msMonthlyMap;
	private:
		bool hCheckValidity(MBase::String& irString);
		bool hBuildDate(Int32 iYear, Int32 iMonth, Int32 iDay);
		bool hBuildTime(Int32 iHour, Int32 iMinute, Int32 iSecond);
		MBase::String hDigitsToWString(Int32 iNumber);
		bool mIsValid;
		MBase::String mYear;
		MBase::String mMonth;
		MBase::String mDay;
		MBase::String mTime;
		static bool mIsInit;
		void hInit();
		void hTrimSpaces(MBase::String& iorString);
	};
#ifndef _VE_CE
	struct DataTableCell
	{
		//Int32 mRow;
		//Int32 mCol;
		//Int32 mMetricIndex;
		//Int32 mThresholdIndex;

		Int32 mAERow;
		Int32 mAECol;
		double mValue;
		bool mIsNull;
		bool mIsDataInit;					//if data is inited.

		bool IsNull(DSSRWGridIterator* ipIterator);
		double* GetValue(DSSRWGridIterator*);					//For easier use.
		Int32 GetColor();					//Used when color is by metrics.
		Int32 GetSize(Int32 iOffset);		//Not sure if this will be used.

		DataTableCell(void* ipController);	//< To Be Delete
		DataTableCell();

		bool Populate(DSSRWGridIterator* ipIterator, Int32 iRowIndex, Int32 iColIndex);
		void Populate(Int32 iRowIndex, Int32 iColIndex);
		void PrintInfo(bool iHasOriginalIndex); //< for testing & debug.

	};

	class DataTableReformer								//We expose this structre here for GM widget to compoute stacked value easily.
	{
	public:

		typedef DataTableReformer* Ptr;
		DataTableReformer(DSSRWGridIterator* ipIterator, DataSetReformInfo* ipReformInfo, bool iHasConsolidatedMetrics = false);
		~DataTableReformer();
		DataSetReformInfo* GetReformInfo();
		void Delete();
		bool mIsValid;			//whether valid for further operation or not.
		bool mIsReformed;		//whether reform operation has been carried out or not.
		bool mHasMetrics;		//whether there are metric cells for the input iterator (it could be filter out by grouping by metrics).
		bool mHasConsolidatedMetrics;	//Whether the break-by unit is put on row axis.

		Int32 mRowCount;
		Int32 mColCount;
		Int32 mReformedDataGroup;
		std::set<Int32> mAdditionalMetrics;

		static DataTableCell globalCell;

		bool Reform();

        bool ReformForHorizontalCategoryPie();

		DataTableCell* GetOneCell(Int32 iRowIndex, Int32 iColIndex, Int32 iColumnOffset = 0, bool iConsiderSeriesGroup = false);

		bool GetMaxMinValue(Int32 iGroupID, double& orMax, double& orMin, Int32 iStartSeriesIndex = 0, Int32 iSpan = 1, DATA_RETRIEVAL_TYPE iType = DRT_STACKED);

		Int32 GetGroupCount();
        Int32 GetSeriesCount();
		Int32 GetRealMetricNumber();
		void Dump(bool iHasOriginalIndex = false);
        Int32 mBalancedGroupCount;
	protected:
		bool hPopulate(DSSRWGridIterator* ipIterator, DataSetReformInfo* ipReformInfo, bool iIsDataNeeded);
		//void hExpand(Int32 iBlockNumber, Int32 iBlockSize, std::vector<DataTableCell*>& orAllCells);
		//void hReform(std::vector<DataTableCell*>& irAllCells, int iBlockNumber, int iBlockSize, int iReformedDataGroup);

		DataTableCell** hExpand(Int32 iBlockNumber, Int32 iBlockSize, bool iIsColShuffled = false);
		void hReform(DataTableCell** ipAllCells, int iBlockNumber, int iBlockSize, int iReformedDataGroup);
        void hReformForHorzontalCateoryPieBreakCol(DataTableCell** ipAllCells, int iBlockNumber, int iBlockSize, int iReformedGroupNum);
        void hReformForCategoryPieCombineGroup(DataTableCell** ipAllCells, int iBlockNumber, int iBlockSize, int iCombineGroupNum);
        void hReformForCategoryPieCombineBlock(DataTableCell** ipAllCells, int iBlockNumber, int iBlockSize, int iCombineSliceNum);
        Int32 hGetSuffledColumnIndex(Int32 iInputIndex);
        void hBuildShuffledColumns(Int32 iSuiteCount, Int32 iDataMetricsNumber, Int32 iAddtionalMetricsNumber);

	private:
		DSSRWGridIterator* mpIterator;						//<AE Iterator
		DataSetReformInfo* mpReformInfo;					//<Reformed Information
		//std::vector<DataTableCell*> mAllCells;				//<Array of all cells, stroed in "Row first" order. It contains both real and virtual cells.
		DataTableCell** mAllCells;				//<Array of all cells, stroed in "Row first" order. It contains both real and virtual cells.
		std::vector<DataTableCell*> mDeleteCellsPool;		//<A helper vector to hold all the real cells.
		Int32 mRealMetricNumber;							//<Templatemetric number in the template.or when groupping by metrics, the selected number of metrics.
		bool mPerfHasDuplicatedData;						//<For performance, additional metrics could be reused.
		bool mIsDataNeeded;									//<For some case, like to compute the uniform axis width/heigh, we actually do not need the data part from dataset.
		Int32 mAllCellsCount;								//<Cell number in total
        std::map<Int32, Int32> mShuffledColumns;            //<Shuffle the mAllCells by column for horizontal chart with breakby
	};


	inline DataSetReformInfo* DataTableReformer::GetReformInfo() {return mpReformInfo;}
	inline Int32 DataTableReformer::GetRealMetricNumber() {return mRealMetricNumber;}
	inline void DataTableCell::Populate(Int32 iRowIndex, Int32 iColIndex) {mAERow = iRowIndex; mAECol = iColIndex;}
#endif
}
#endif
