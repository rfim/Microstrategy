//
//  GraphMatrixDataSetWrapper.h
//  IPhoneChart
//
//  Created by Deng, Jun on 8/7/13.
//
//

#ifndef MsiChart_GraphMatrixDataSetWrapper_h
#define MsiChart_GraphMatrixDataSetWrapper_h
#include "../Dataset.h"
#include "GraphMatrixDataSetWrapperHelper.h"

namespace MsiChart
{
#ifdef _VE_CE
	using namespace MVE;
#endif
	class GraphMatrixDataSetWrapper:public Dataset
	{
	public:
#ifndef _VE_CE
			GraphMatrixDataSetWrapper(void* iGMDataSet);
#else
			GraphMatrixDataSetWrapper(GMCEDataProvider::Ptr iGMDataSet);
#endif
            virtual ~GraphMatrixDataSetWrapper() throw();
            virtual void SetSeriesByColumn(bool iSeriesByColumn);
            virtual bool GetSeriesByColumn() const;
            virtual void SetReverseGroup(bool iReverseGroup);
            virtual bool GetReverseGroup();
            virtual void SetReverseSeries(bool iRevereSeries);
            virtual void SetForHistogram(bool iIsForHistogram);
            virtual float GetMemoryUsageEstimation();
            virtual bool GetSliceGroupRange(Int32& iBeginGroup, Int32& iEndGroup);
            virtual bool PopulateOneSlice(Int32 iBeinSeries, Int32 iEndSeries, Int32 iBeginGroup, Int32 iEndGroup);
            virtual bool GetOneSliceRange(Int32& iBeinSeries, Int32& iEndSeries, Int32& iBeginGroup, Int32& iEndGroup);
            virtual bool PopulateOneSlice(const std::vector<Int32>& irSeriesIndex, const std::vector<Int32>& irGroupIndex, SLICE_TYPE iType = SLICE_STRIPPED);
            virtual bool GetOneSliceRange(std::vector<Int32>& irSeriesIndex, std::vector<Int32>& irGroupIndex);
            virtual bool ShuffleIndex(bool iIsOnRow, const std::vector<Int32>& iShuffledIndexList);
            virtual SHUFFLE_STATUS GetShuffledStatus();
            virtual void RestoreFromShuffle();
            virtual bool PopulateOneSlice(Int32 iBegin, Int32 iEnd, const std::vector<Int32>& irIndex, SLICE_TYPE iType = SLICE_MIXED_GROUP);
            virtual bool GetSliceFlag();
            virtual void EnableSlices(bool iUseSlice = true);
            virtual bool IsSliced();
            virtual bool SetActiveSliceIndex(Int32 iIndex = 0);
            virtual Int32 GetActiveSliceIndex();
            virtual bool AdjustGroupRange(Int32 iOffset);
            virtual bool AdjustGroupRange(Int32 iBeginGroup, Int32 iEndGroup);
            virtual HRESULT SetDataGroup(Int32 iDataGroup, bool iIgnoreForNoSeriesOrGroup = true);
            virtual Int32 GetDataGroup();
            virtual HRESULT GetData(Int32 iSeriesID, Int32 iGroupID, const double*& iorValue, Int32 iColumnOffset = 0, bool iIsForSlice = true);
            virtual HRESULT GetTotalData(Int32 iIndex, bool iIsGroupTotal, Int32 iSpanStartIndex, Int32 iSpanEndIndex, double& iorValue, Int32 iColumnOffset,  bool iIsUseAbsValue = false, bool iIsForSlice = true);
            virtual HRESULT GetTotalData(Int32 iIndex, bool iIsGroupTotal, const std::vector<Int32>& iPickedIndexSet, double& iorValue, Int32 iColumnOffset,  bool iIsUseAbsValue = false, bool iIsForSlice = true);
            virtual HRESULT GetToolTipDataString(Int32 iSeriesID, Int32 iGroupID, MBase::String& orDataString, Int32 iOffset);
            virtual HRESULT GetPieSubtotal(std::map<std::pair<Int32, Int32>, double> iPieSubtotalMap, Int32 iGroupId, const std::vector<Int32>& iPickedIndexSet, double& iorValue, Int32 iColumnOffset,  bool iIsUseAbsValue = false, bool iIsForSlice = true);
            virtual void GetSubTotalGlobalRowAndColumnIndex(Int32 iGroupId, const std::vector<Int32>& iPickedIndexSet, Int32 iColumnOffset, Int32 &iorRowOrdinal, Int32 &iorColOrdinal);
            virtual HRESULT GetPrimaryData(Int32 iSeriesID,Int32 iGroupID, const double*& iorValue, Int32 iPrimaryIndex = 0, bool iIsForColor = false,  bool iIsForSlice = true, int iAngleIndex = -3, int iSliceIndex = -3);
            virtual HRESULT GetGMPieMaxMin(double& orMax, double& orMin, Int32 iColumnOffset, DATA_RETRIEVAL_TYPE iType = DRT_NORMAL);
            virtual HRESULT GetGMPieSubtotalMaxMin(std::map<std::pair<Int32, Int32>, double> iPieSubtotalMap, double& orMax, double& orMin, Int32 iColumnOffset, DATA_RETRIEVAL_TYPE iType = DRT_NORMAL);
            
            virtual HRESULT GetSeriesCount(Int32& orSeriesCount);
            virtual HRESULT GetGroupCount(Int32& orGroupCount);
            virtual HRESULT GetSeriesLabel(Int32 iSeriesID,MBase::String& orSeriesLabel, bool iSubstitute = true, Int32 iColumnOffset = 0, bool iIsForSlice = true, LABEL_LEVEL_OPTION iOption = LLO_NORMAL, bool iIsForEmptyAxisWithTemplateMetrc = false);
            virtual HRESULT GetGroupLabel(Int32 iGroupID, MBase::String& orGroupLabel, bool iSubstitute = true, Int32 iColumnOffset = 0, bool iIsForSlice = true, LABEL_LEVEL_OPTION iOption = LLO_NORMAL);
            virtual HRESULT GetPeakValuesAcrossSeries(double& orMax, double& orMin, const std::vector<Int32>& irSereisSet, Int32 iColumnOffset = 0);
            virtual HRESULT GetPeakValuesAcrossSeries(double& orMax, double& orMin, Int32 iBeginSeries, Int32 iEndSeries, Int32 iColumnOffset = 0);
            virtual void GetGroupDimTree(DimTree*& iopDimTree);
            virtual MBase::String GetTemplateUnitElementName(Int32 iDimLayer, Int32 iIndexInsideDim, bool iIsInRowTree = false, Int32 iFormIndex = 0, MBase::String iSeperator = L" ");
            virtual MBase::String GetTemplateUnitName(Int32 iDimLayer, bool iIsInRowTree = false, bool iUseAlias = false);
            virtual Int32 GetTreeWidth(Int32 iDimLayer, bool iIsInRowTree, bool iIsActualSize = false, bool iIsSqueezeHCG = true);         ///Get the number of dim node in one dim.
            virtual DimTree* GetRowTree();
            virtual DimTree* GetColTree();
            virtual HRESULT GetValueAxisNumberFormat(ChartAdvFormat& orFormat, bool iConsiderThresholds = true, Int32 iStartIndex = 0, Int32 iStep = 0);
            virtual HRESULT GetValueAxisNumberFormat(ChartAdvFormat& orFormat, const std::set<Int32>& irMetricSet);
            virtual HRESULT GetGridDataFormat(ChartAdvFormat& orFormat, Int32 iSeriesId, Int32 iGroupId, Int32 iColumnOffset, Int32 iAngleId = -3, Int32 iSlcieId = -3);
            virtual HRESULT GetGridDataFormat(ChartAdvFormat& orFormat, Int32 iHostMetricIndex, bool iIsAngleMetrixIndex = false, Int32 iGroupId = 0);
            
            virtual HRESULT	GetThresholdFillColor(Int32 iSeriesId, Int32 iGroupId, CComVariant* opValue);
            virtual THRESHOLD_FILL_STYLE GetCurrentTreshHoldFillType();
            virtual bool GetReplacedText(Int32 iSeriesId, Int32 iGroupId, Int32 iColumnOffset,MBase::String& orReplacedText, Int32& orColor);
            virtual HRESULT SetRowAndColumn(Int32 iRowNumber, Int32 iColumnNumber);
            virtual HRESULT SetData(Int32 iRow, Int32 iCol, double iValue);
            virtual HRESULT SetRowLabel(Int32 iRow, const wchar_t* ipRowLabel);
            virtual HRESULT SetColumnLabel(Int32 iColumn, const wchar_t* ipColumnLabel);
#ifndef _VE_CE
            virtual HRESULT InitFromTemplateAndGrid(DSSTemplate* ipTemplate, DSSRWGridIterator* ipGridIterator, bool iIsParseTotals);
            virtual HRESULT InitFromTemplateSlice(DSSTemplate* ipTemplate, DSSRWGridIterator* ipGridIterator, Int32 iSliceID, bool iIsCacheData = false, bool iIncludeSubtotal = true) ;
#endif
            virtual bool IsHeaderTreeEmpty(bool iIsForGroup);
            virtual void Delete();
            virtual bool PopulateOneTree(std::wifstream& iFileStream, bool iIsForRow = true);
            //[End of Public Methods for Misc use]
            virtual void ConvertLocalAEIndexToGlobal(int& ioRowIndex, int& ioColIndex);
            virtual bool PopulateTokenManager(bool iEnableToken);
            virtual bool GetTokenFlag();
            virtual TokenManager* GetTokenManager();
            virtual bool PopulateTotalValues(std::wifstream& iFileStream);
            virtual bool Refresh();
            virtual bool GetRowCount(Int32& orCount);
            virtual bool GetMetricsCount(Int32& orCount);
            virtual bool GetRowLabel(Int32 iIndex, MBase::String& orLabel);
            virtual bool GetMetricsLabel(Int32 iIndex, MBase::String& orLabel);
            virtual MetricUnitInfo* GetMetricInfo();
            virtual bool IsMetricBySeries();
            virtual bool IsSubtotalAt(Int32 iRowIndex, Int32 iColIndex, bool& orIsTotal);
            virtual bool IsEmpty();
            virtual Int32 GetMetricsCount(bool iIsAtTopLevel = false);
            virtual std::string GetLabelInDrillPath(bool iIsByRow, Int32 iIndex, const std::string& irCandiate, bool iIncludeAllForms = false);
        public:
            virtual bool IsHeaderEmpty(bool iIsForRow = true);
            virtual bool GetAvgValue(double& orValue, Int32 iIndex, Int32 iColumnOffset = 0, bool iIsForSeries = true, bool iIsForSlice = false);
            virtual void SetTreeInstance(DimTree* ipTree, bool iIsForRowTree);
            virtual Int32 GetThresholdIndex(Int32 iRowIndex, Int32 iColIndex);
            virtual Int32 GetMetricIndex(Int32 iRowIndex, Int32 iColIndex);
            virtual Int32 GetAESliceId();
            virtual Int32 GetAggregationResult(double& orValue, Int32 iIndex, bool iIsForSeries = true,  AGG_TYPE iAggType = AGG_TYPE_AVG); //return valide data cell.
            
            virtual Int32 GetUnitWidth(Int32 iIndex, bool iIsInRow);
            
            virtual DATASET_STATUS GetStatus(bool iIsGraph, bool iIsDummyGridChart = false);
            virtual bool NextSliceToDisplay(bool iIsPrevious, bool iIsStaring = false);
            virtual Int32 CurrentSliceNumberToDisplay();
            virtual bool IsDisplaySliceInvalid();
            virtual void CurrentSlicePosition(Int32& orRowIndex, Int32& orColIndex);
            virtual void SeriesGroupIndexToRowColumnIndex(Int32 iSeriesID, Int32 iGroupID, Int32& orRowIndex, Int32& orColumnIndex, Int32 iColumnOffset, Int32 iAngleId = -3, Int32 iSliceId = -3, bool iIsConsiderSubtotal = true);
            virtual void GetRowAndColumnIndexForMinMaxMetrixIndex(bool iIsForMax, std::vector<Int32> iMetricIndex, Int32& orRowIndex, Int32& orColumnIndex);
            virtual void SetFormatManager(FormatManager* ipFormatManager);
            virtual bool GetSeriesGroupIdsFromIndex(std::vector<std::pair<Int32, Int32> >& iorSGPairs, const std::vector<Int32>& irRowIndexSet, const std::vector<Int32>& irColIndexSet);
            
            virtual Int32 GetColorStyleIndex(Int32 iLocalIndex);
            virtual Int32 GetColorStyleIndex(Int32 iSeriesID, Int32 iGroupID, Int32 iColumnOffset = 0, bool iIsConsiderSubtotal = true);
            virtual std::string GetGMColorByKey(std::vector<std::pair<Int32, Int32> > iColorByUnitsIndex, Int32 iSeriesID, Int32 iGroupID, Int32 iColumnOffset = 0, bool iIsConsiderSubtotal = true);
            virtual int GetGMColorByIndex(Int32 iSeriesID, Int32 iGroupID, Int32 iAngleIndex = -3, Int32 iSliceIndex = -3);  //iSliceIndex is valid only for pie chart
            virtual void getLineMarkerColorSetting(std::vector<int>& iSetting);
            virtual MBase::String GetColorStyleLabel(Int32 iSeriesID, Int32 iGroupID, Int32 iColumnOffset, bool iIsConsiderSubtotal = true);
            virtual MBase::String GetColorStyleLabel(Int32 iLocalColorIndex);
            virtual Int32 GetColorStyleNumber();
            virtual void SetColorSlices(std::vector<Int32>& irRowUnits, std::vector<Int32> irColumnUnits);
#ifndef _VE_CE
            virtual void SetGlobalIterator(DSSRWGridIterator* ipIterator);
#endif
            virtual	void GetLabelCollection(Int32 iOrdinal, bool iIsForRow, std::vector<std::string>&orCollection);
            virtual	void GetDataTextLabel(Int32 iOrdinal, bool iIsForRow, MBase::String& orCollection, int iDepth);
            virtual	void GetBreakByLabel(Int32 iOrdinal, bool iIsForRow, MBase::String& orCollection);
            virtual void GetUnitNameCollection(bool iIsForRow, std::vector<std::string>&orCollection);
            virtual	void GetSliceLabel(Int32 iOrdinal, bool iIsForRow, MBase::String& orCollection);
            virtual	void GetTextDataLabel(Int32 iGroupId, Int32 iSeriesId, std::string& orCollection, Int32 iAngleId = -3, Int32 iSliceId = -3);
            virtual void GetTextDataLabel(Int32 iOrdinal, std::string& orCollection);
#ifndef _VE_CE
            virtual bool PopulateReformer(DSSRWGridIterator* ipGridIterator, DataSetReformInfo* ipReformInfo);
#endif
            virtual bool IsReformed();
            virtual Int32 GetMetricLabelCount();
            virtual Int32 GetBalancedCount(bool iIsForGroup);
            virtual const DataGroupInfo* GetDataGroupInfo();
            virtual Int32 GetSeriesPieceStartIndex(Int32 iIndex);
            virtual void SetDatagroupInfo(const DataGroupInfo& irInfo);
            virtual const std::vector<int> GetAxisInfo();
            virtual void SetAxisInfo(const std::vector<int> &irAxisInfo);
            virtual void GetSeriesSplitVector(std::vector<Int32>& orVec);
            virtual void SetSeriesSplitVector(const std::vector<Int32>& irVec);
            virtual void SetPieGroupDetph(Int32 irDetph);
            virtual void GetPieGroupDetph(Int32 &orDepth);
            virtual void DumpDataSet(bool iIsNaive, bool isGlobalIterator = false);
            virtual void SetGMDataSet(void *pDataSet = NULL);
            virtual void PrepareIterator();
            double hGetThresholdBackgroundColor(Int32 iMetrixId, Int32 iThresholdId, double iDefaultColorValue);
#ifndef _VE_CE
            bool hGetGradientPropertyForThreshold(DSSFormat* ipFormat, CComVariant* opValue);
#else
            bool hGetGradientPropertyForThreshold(IDSSFormat* ipFormat, CComVariant* opValue);
#endif
            virtual HRESULT GetPieAngleCount(Int32 iGroupId, Int32 iSeriesId, Int32& orAngleCount);
            virtual HRESULT GetPieSliceCount(Int32 iGroupId, Int32 iSeriesId, Int32 iAngleId, Int32& orSliceCount);
            virtual HRESULT GetPieSliceData(Int32 iGroupId, Int32 iSeriesId, Int32 iAngleId, Int32 iSliceId, const double*& iorValue, Int32 iColumnOffset = -1);
            virtual HRESULT GetData(Int32 iGroupId, Int32 iSeriesId, EnumGraphMatrixDataIndexType iDataIndexType, const double*& iorValue, Int32 iAngleId = -3);
            virtual HRESULT GetDataPositionPartSize(Int32& orDataPositionPartSize);
            virtual HRESULT GetDataSizeByPartSize(Int32& orDataSizeByPartSize);
            virtual HRESULT GetDataAdditionPartSize(Int32& orDataAdditionPartSize);
            virtual Int32 GetMetricIndex(Int32 iGroupId, Int32 iSeriesId, Int32 iAngleIndex, Int32 iSliceIndex);
            virtual void SetOnScreenGroupRange(Int32 iStartGroupId, Int32 iEndGroupId);
            virtual bool IsGroupOnScreen(Int32 iGroupId);
            virtual bool IsSeriesOnScreen(Int32 iSeriesId);
            virtual Int32 GetShapeBySeriesId(Int32 iSeriesId);
            // [GM Metric Level Data Label Support]
            virtual bool IsDataLabelEnaled(Int32 iSeriesId, Int32 iGroupId, Int32 iAngleId = -3, Int32 iSliceId = -3);
            virtual HRESULT GetCumulatedData(Int32 iGroupId, Int32 iSeriesId, EnumGraphMatrixDataIndexType iDataIndexType, const double*& iorValue);
            virtual std::set<int> GetSeriesInSecondaryAxis();
            // [GM Bullet Chart Suppor]
            virtual void GetBandDivision(Int32 iGroupId, Int32 iVAIndex, std::vector<double>& iMetrics, std::vector<double>& iRatios);
            
            // [GM Trendline Support]
            virtual std::vector<double> GetTrendLineValuesWithForecast(Int32 iSeriesId);
            virtual bool IsExpomentialBaseNearOne(Int32 iSeriesId);
            virtual std::vector<double> GetCoefficients(Int32 iSeriesId);
            
            // GM RefLine Support
            virtual std::vector<double> GetRefLineValues(Int32 iSeriesId);
            virtual void GetRefLineLable(Int32 iSeriesId, Int32 iRefLineIndex, std::string& iText, bool& orIsShowValue, double& orValue);
            virtual std::vector<double> GetRefLineValuesForBubble(Int32 iIndex, Int32 iAxisIndex);
            
            //get/set method
            double* GetDataCellValueReference();
            double* GetDataCellColorReference();
            double GetDataCellValue();
            void SetDataCellValue(double iDataCellValue);
            double GetDataCellColor();
            void SetDataCellColor(double iDataCellColor);
            void SetCurrentThresholdStyle(THRESHOLD_FILL_STYLE mCurrentThresholdStyle);
            DataGroupInfo* GetDataGroupInfoValue();
            void SetDataGroupInfoValue(DataGroupInfo iDataGroupInfo);
            virtual std::pair<int, int> getOnScreenTextRange(bool isForGroup);
            //[GM text as shape support]
            virtual bool GetTextValue(Int32 iSeriesId, Int32 iGroupId,double& orDataValue);
            //[GM getting properties support]
            virtual Int32 FindCell(bool iFromSeries,Int32 Index,Int32 Target);
            virtual bool GetGenericFillFormatColor(RGBColor& orColor, Int32 iSeriesId ,Int32 iGroupId,Int32 iAngleId = -3 , Int32 iSliceId = -3);
            virtual bool GetGenericFillFormatType(unsigned char& orFillType,Int32 iSeriesId ,Int32 iGroupId,Int32 iAngleId = -3 , Int32 iSliceId = -3);
            virtual bool GetGenericFillFormatAlpha(unsigned char& orAlpha,Int32 iSeriesId ,Int32 iGroupId,Int32 iAngleId = -3 , Int32 iSliceId = -3);
            virtual bool GetGenericFillFormatGradient(GraphFillGradient& irFillGradient,Int32 iSeriesId ,Int32 iGroupId, Int32 iSliceId = -3);
            
            virtual bool GetTrendLineThickness(Int32& orLineThickness,Int32 iSeriesId,Int32 iGroupId);
            virtual bool GetTrendLineStyle(unsigned char& orLineStyle,Int32 iSeriesId,Int32 iGroupId);
            virtual bool GetTrendLineColor(RGBColor& orLineColor,Int32 iSeriesId,Int32 iGroupId);
            
            virtual bool GetReferenceLineThickness(Int32& orLineThickness,Int32 iSeriesId,Int32 iGroupId);
            virtual bool GetReferenceLineStyle(unsigned char& orLineStyle,Int32 iSeriesId,Int32 iGroupId);
            virtual bool GetReferenceLineColor(RGBColor& orLineColor,Int32 mSeriesId,Int32 iGroupId);
            
            virtual bool GetAxisOriginLineThickness(Int32& orLineThickness,Int32 iObjectID);
            virtual bool GetAxisOriginLineStyle(unsigned char& orLineStyle,Int32 iObjectID);
            virtual bool GetAxisOriginLineColor(RGBColor& orLineColor,Int32 iObjectID);
            
            virtual bool GetGenericLineThickness(Int32& orLineThickness,Int32 iSeriesId ,Int32 iGroupId,Int32 iAngleId = -3 , Int32 iSliceId = -3);
            virtual bool GetGenericLineStyle(unsigned char& orLineStyle,Int32 iSeriesId ,Int32 iGroupId,Int32 iAngleId = -3 , Int32 iSliceId = -3);
            virtual bool GetGenericLineColor(RGBColor& orLineColor,Int32 iSeriesId ,Int32 iGroupId,Int32 iAngleId = -3 , Int32 iSliceId = -3);
            
            virtual bool GetAxisLineThicknessFormat(Int32& orLineThickness);
            virtual bool GetAxisLineStyleFormat(unsigned char& orLineStyle);
            virtual bool GetAxisLineColorFormat(RGBColor& orLineColor);
            
            virtual bool GetAxisMajorTickLineThicknessFormat(Int32& orLineThickness,Int32 iObjectID);
            virtual bool GetAxisMajorTickLineStyleFormat(unsigned char& orLineStyle,Int32 iObjectID);
            virtual bool GetAxisMajorTickLineColorFormat(RGBColor& orLineColor,Int32 iObjectID);
            
            virtual bool GetAxisMinorTickLineThicknessFormat(Int32& orLineThickness,Int32 iObjectID);
            virtual bool GetAxisMinorTickLineStyleFormat(unsigned char& orLineStyle,Int32 iObjectID);
            virtual bool GetAxisMinorTickLineColorFormat(RGBColor& orLineColor,Int32 iObjectID);
            
            virtual bool GetReferenceLineFontColor(RGBColor& orFontColor,Int32 iSeriesId,Int32 iGroupId);
            virtual bool GetReferenceLineFontSize(Int32& orFontSize,Int32 iSeriesId,Int32 iGroupId);
            virtual bool GetReferenceLineFontStyle(unsigned char& orFontStyle,Int32 iSeriesId,Int32 iGroupId);
            virtual bool GetReferenceLineFontName(MBase::String& orFontName,Int32 iSeriesId,Int32 iGroupId);
            
            virtual bool GetDataLableFontAlignment(unsigned char& orFontAlignment);
            virtual bool GetDataLableFontSize(Int32& orFontSize);
            virtual bool GetDataLableFontStyle(unsigned char& orFontStyle);
            virtual bool GetDataLableFontName(MBase::String& orFontName);
            virtual bool GetDataLableFontBox(unsigned char& orFontBox);
            virtual bool GetDataLableFontAlpha(unsigned char& orFontAlpha);
            virtual bool GetDataLabelFontColor(RGBColor& orFontColor);
            
            virtual bool GetAxisLabelFontSize(Int32& orFontSize ,Int32 iWhichAxis);
            virtual bool GetAxisLabelFontName(MBase::String& orFontName ,Int32 iWhichAxis);
            virtual bool GetAxisLabelFontStyle(unsigned char& orFontStyle ,Int32 iWhichAxis);
            virtual bool GetAxisLabelFontRotation(unsigned char& orFontRotation ,Int32 iWhichAxis);
            virtual bool GetAxisLabelFontColor(RGBColor& orFontColor ,Int32 iWhichAxis);
            
            virtual bool GetTextAsShapeFontSize(Int32& orFontSize,Int32 iSeriesId ,Int32 iGroupId);
            virtual bool GetTextAsShapeFontColor(RGBColor& orFontColor,Int32 iSeriesId ,Int32 iGroupId);
            virtual bool GetTextAsShapeFontName(MBase::String& orFontName,Int32 iSeriesId ,Int32 iGroupId);
            virtual bool GetTextAsShapeFontStyle(unsigned char& orFontStyle,Int32 iSeriesId ,Int32 iGroupId);
            virtual bool GetTextAsShapeAlignment(unsigned char& orFontAlignment,Int32 iSeriesId ,Int32 iGroupId);
                
            bool GetAxisOriginValue(double& orAxisOriginValue , Int32 iObjectId);
			GraphMatrixDataSetWrapperHelper* GetGraphMatrixDataSetWrapperHelper(void) { return mpGraphMatrixDataSetWrapperHelper; }

#ifdef _VE_CE
			GMCEDataProvider::Ptr GetGraphMatricDataSet();
#endif

        private:
            GraphMatrixDataSetWrapperHelper *mpGraphMatrixDataSetWrapperHelper;
#ifndef _VE_CE
            void* mpGMDataSet;
#else
            GMCEDataProvider::Ptr mpGMDataSet;
            std::vector<int> mAxisInfo;            

            FormatManager* mpFormatManager;
			MetricUnitInfo* mpAEMetricsInfo;
#endif
            static double sDataCellValue;
            static double sDataCellColor;
            THRESHOLD_FILL_STYLE mCurrentThresholdStyle;
            DataGroupInfo mDataGroupInfo;
            void hGetAxisBoolSettingByAxisType(int iAxisType, bool& isYAxis, bool& isPrimaryAxis);
            void hConvertIntColorToRGBColor(RGBColor& oRGBColor, unsigned int iIntColor);
#ifdef _VE_CE
            bool hGetValueAxisNumberFormat(ChartAdvFormat& orFormat, bool iConsiderThresholds);
#endif
        };

#ifdef _VE_CE
		inline GMCEDataProvider::Ptr GraphMatrixDataSetWrapper::GetGraphMatricDataSet()
        {
            return mpGMDataSet;
        }
#endif

        inline DataGroupInfo* GraphMatrixDataSetWrapper::GetDataGroupInfoValue()
        {
            return &mDataGroupInfo;
        }
        inline void GraphMatrixDataSetWrapper::SetDataGroupInfoValue(DataGroupInfo iDataGroupInfo)
        {
            mDataGroupInfo = iDataGroupInfo;
        }
        inline double GraphMatrixDataSetWrapper::GetDataCellValue()
        {
            return sDataCellValue;
        }
        inline void GraphMatrixDataSetWrapper::SetDataCellValue(double iDataCellValue)
        {
            sDataCellValue = iDataCellValue;
        }
        inline double* GraphMatrixDataSetWrapper::GetDataCellValueReference()
        {
            return &sDataCellValue;
        }
        inline double* GraphMatrixDataSetWrapper::GetDataCellColorReference()
        {
            return &sDataCellColor;
        }
        inline double GraphMatrixDataSetWrapper::GetDataCellColor()
        {
            return sDataCellColor;
        }
        inline void GraphMatrixDataSetWrapper::SetDataCellColor(double iDataCellColor)
        {
            sDataCellColor = iDataCellColor;
        }
        inline void GraphMatrixDataSetWrapper::SetCurrentThresholdStyle(THRESHOLD_FILL_STYLE iCurrentThresholdStyle)
        {
            mCurrentThresholdStyle = iCurrentThresholdStyle;
        }
    }
#endif
