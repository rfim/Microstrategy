//
//  GraphMatrixDataSetWrapperHelper.h
//  IPhoneChart
//
//  Created by Jin, Dong on 24/9/14.
//
//

#ifndef MsiChart_GraphMatrixDataSetWrapperHelper_h
#define MsiChart_GraphMatrixDataSetWrapperHelper_h
#include "../Dataset.h"

namespace MsiChart{
    class GraphMatrixDataSetWrapper;
    
    class GraphMatrixDataSetWrapperHelper
    {
    public:
        GraphMatrixDataSetWrapperHelper(GraphMatrixDataSetWrapper *ipGraphMatrixDataSetWrapper);
        virtual ~GraphMatrixDataSetWrapperHelper();
        virtual bool GetSeriesByColumn() const;
        
        virtual bool GetOneSliceRange(std::vector<Int32>& irSeriesIndex, std::vector<Int32>& irGroupIndex);
        
        virtual Int32 GetDataGroup();
        
        virtual HRESULT GetData(Int32 iSeriesID, Int32 iGroupID, const double*& iorValue, Int32 iColumnOffset, bool iIsForSlice);
        
        virtual HRESULT GetPrimaryData(Int32 iSeriesID,Int32 iGroupID, const double*& iorValue, Int32 iPrimaryIndex, bool iIsForColor,  bool iIsForSlice, int iAngleIndex, int iSliceIndex);
        
        virtual HRESULT GetGMPieSubtotalMaxMin(std::map<std::pair<Int32, Int32>, double> iPieSubtotalMap, double& orMax, double& orMin, Int32 iColumnOffset, DATA_RETRIEVAL_TYPE iType);
        virtual HRESULT GetSeriesLabel(Int32 iSeriesID,MBase::String& orSeriesLabel, bool iSubstitute, Int32 iColumnOffset, bool iIsForSlice, LABEL_LEVEL_OPTION iOption, bool iIsForEmptyAxisWithTemplateMetrc);
        virtual HRESULT GetGroupLabel(Int32 iGroupID, MBase::String& orGroupLabel, bool iSubstitute, Int32 iColumnOffset, bool iIsForSlice, LABEL_LEVEL_OPTION iOption);
        
        virtual void ConvertLocalAEIndexToGlobal(int& ioRowIndex, int &ioColIndex);
        virtual HRESULT GetValueAxisNumberFormat(ChartAdvFormat& orFormat, bool iConsiderThresholds, Int32 iStartIndex, Int32 iStep);
        virtual HRESULT GetValueAxisNumberFormat(ChartAdvFormat& orFormat, const std::set<Int32>& irMetricSet);
        virtual HRESULT GetGridDataFormat(ChartAdvFormat& orFormat, Int32 iSeriesId, Int32 iGroupId, Int32 iColumnOffset, Int32 iAngleId, Int32 iSliceId);
        virtual HRESULT GetGridDataFormat(ChartAdvFormat& orFormat, Int32 iHostMetricIndex, bool iIsAngleMetricIndex, Int32 iGroupId);
        virtual HRESULT	GetThresholdFillColor(Int32 iSeriesId, Int32 iGroupId, CComVariant* opValue);
        virtual double GetThresholdBackgroundColor(Int32 iMetrixId, Int32 iThresholdId, double iDefaultColorValue);
        virtual HRESULT GetGroupCount(Int32& orGroupCount);
        virtual bool GetMetricsCount(Int32& orCount);
        virtual Int32 GetMetricsCount(bool iIsAtTopLevel);
        //lizhang, de38822, 20160727, specialcase in combo
        virtual bool IsReformed();
        virtual bool IsMetricBySeries();
        virtual MetricUnitInfo* GetMetricInfo();
        virtual Int32 GetColorStyleIndex(Int32 iLocalIndex);
        
        
        virtual bool IsHeaderEmpty(bool iIsForRow);
        
        virtual Int32 GetMetricIndex(Int32 iRowIndex, Int32 iColIndex);
        
        virtual void SeriesGroupIndexToRowColumnIndex(Int32 iSeriesID, Int32 iGroupID, Int32& orRowIndex, Int32& orColumnIndex, Int32 iColumnOffset, Int32 iAngleId, Int32 iSliceId, bool iIsConsiderSubtotal);
        virtual void GetRowAndColumnIndexForMinMaxMetrixIndex(bool iIsForMax, std::vector<Int32> iMetricIndex, Int32& orRowIndex, Int32& orColumnIndex);
        virtual void SetFormatManager(FormatManager* ipFormatManager);
        
        virtual int GetGMColorByIndex(Int32 iSeriesID, Int32 iGroupID, Int32 iAngleIndex, Int32 iSliceIndex);
        virtual void getLineMarkerColorSetting(std::vector<int>& iSetting);
        virtual void GetLabelCollection(Int32 iOrdinal, bool iIsForRow, std::vector<std::string>&orCollection);
        
        virtual void GetTextDataLabel(Int32 iGroupId, Int32 iSeriesId, std::string& orCollection, Int32 iAngleId, Int32 iSliceId);
        
        virtual void GetTextDataLabel(Int32 iOrdinal, std::string& orCollection);
        
        
        virtual Int32 GetBalancedCount(bool iIsForGroup);
        virtual const DataGroupInfo* GetDataGroupInfo();
        virtual void PrepareIterator();
        
        virtual HRESULT GetPieAngleCount(Int32 iGroupId, Int32 iSeriesId, Int32& orAngleCount);
        virtual HRESULT GetPieSliceCount(Int32 iGroupId, Int32 iSeriesId, Int32 iAngleId, Int32& orSliceCount);
        virtual HRESULT GetPieSliceData(Int32 iGroupId, Int32 iSeriesId, Int32 iAngleId, Int32 iSliceId, const double*& iorValue, Int32 iColumnOffset);
        virtual HRESULT GetData(Int32 iGroupId, Int32 iSeriesId, EnumGraphMatrixDataIndexType iDataIndexType, const double*& iorValue, Int32 iAngleId);
        virtual HRESULT GetDataPositionPartSize(Int32& orDataPositionPartSize);
        
        virtual Int32 GetMetricIndex(Int32 iGroupId, Int32 iSeriesId, Int32 iAngleIndex, Int32 iSliceIndex);
        virtual bool IsGroupOnScreen(Int32 iGroupId);
        virtual bool IsSeriesOnScreen(Int32 iSeriesId);
        virtual Int32 GetShapeBySeriesId(Int32 iSeriesId);
        // [GM Metric Level Data Label Support]
        virtual bool IsDataLabelEnaled(Int32 iSeriesId, Int32 iGroupId, Int32 iAngleId = -3, Int32 iSliceId = -3);
        virtual std::set<int> GetSeriesInSecondaryAxis();
        virtual HRESULT GetSeriesCount(Int32& orSeriesCount);
        virtual HRESULT GetDataSizeByPartSize(Int32& orDataSizeByPartSize);
        virtual HRESULT GetDataAdditionPartSize(Int32& orDataAdditionPartSize);
        virtual Int32 GetColorStyleNumber();
        virtual Int32 GetMetricLabelCount();
        virtual std::pair<int, int> getOnScreenTextRange(bool isForGroup);
        
        // [GM Trendline Support]
        virtual std::vector<double> GetTrendLineValuesWithForecast(Int32 iSeriesId);
        virtual bool IsExpomentialBaseNearOne(Int32 iSeriesId);
        virtual std::vector<double> GetCoefficients(Int32 iSeriesId);
        
        //Reference line info
        virtual std::vector<double> GetRefLineValues(Int32 iSeriesId);
        virtual void GetRefLineLable(Int32 iSeriesId, Int32 iRefLineIndex, std::string& iText, bool& orIsShowValue, double& orValue);
        virtual std::vector<double> GetRefLineValuesForBubble(Int32 iIndex, Int32 iAxisIndex);
        virtual bool GetReferenceLineThickness(Int32& orLineThickness,Int32 iSeriesId, Int32 iGroupId);
        virtual bool GetReferenceLineStyle(unsigned char& orLineStyle,Int32 iSeriesId, Int32 iGroupId);
        virtual bool GetReferenceLineColor(unsigned int& orLineColor,Int32 iSeriesId, Int32 iGroupId);
        virtual bool GetReferenceLineFontColor(unsigned int& orFontColor,Int32 iSeriesId,Int32 iGroupId);
        virtual bool GetReferenceLineFontSize(Int32& orFontSize,Int32 iSeriesId,Int32 iGroupId);
        virtual bool GetReferenceLineFontStyle(unsigned char& orFontStyle,Int32 iSeriesId,Int32 iGroupId);
        virtual bool GetReferenceLineFontName(std::string& orFontName,Int32 iSeriesId,Int32 iGroupId);
        virtual bool GetTrendLineThickness(Int32& orLineThickness,Int32 iSeriesId, Int32 iGroupId);
        virtual bool GetTrendLineStyle(unsigned char& orLineStyle,Int32 iSeriesId, Int32 iGroupId);
        virtual bool GetTrendLineColor(unsigned int& orLineColor,Int32 iSeriesId, Int32 iGroupId);
        
        //Axis Label properties
        virtual bool getAxisLabelFontSize(bool isYAxis,  bool isPrimary, Int32& oFontSize);
        virtual bool getAxisLabelFontName(bool isYAxis,  bool isPrimary, std::string& oFontName);
        virtual bool getAxisLabelFontStyle(bool isYAxis,  bool isPrimary, unsigned char& oFontStyle);
        virtual bool getAxisLabelFontColor(bool isYAxis,  bool isPrimary, unsigned int& oFontColor);
        virtual bool GetAxisMajorTickLineThicknessFormat(Int32& orLineThickness,Int32 iObjectID);
        virtual bool GetAxisMajorTickLineStyleFormat(unsigned char& orLineStyle,Int32 iObjectID);
        virtual bool GetAxisMajorTickLineColorFormat(unsigned int& orLineColor,Int32 iObjectID);
        
		//Axis line properties
		virtual bool getAxisLineThickness(bool isYAxis,  bool isPrimary, Int32& oLineThickness);
        virtual bool getAxisLineStyle(bool isYAxis,  bool isPrimary, unsigned char& oLineStyle);
        virtual bool getAxisLineColor(bool isYAxis,  bool isPrimary, unsigned int& oLineColor);

        virtual bool getDataLabelFontSize(int& orFontSize);
        virtual bool getDataLabelFontStype(unsigned char& orFontStyle);
        virtual bool getDataLabelFontName(std::string& orFontName);
        virtual bool getDataLabelFontColor(unsigned int& orFontColor);
        
        //Shape element format setting
        virtual bool getGenericFillFormatColor(unsigned int& orColor, Int32 iSeriesId ,Int32 iGroupId,Int32 iAngleId = -3 , Int32 iSliceId = -3);
        virtual bool getGenericFillFormatType(unsigned char& orFillType,Int32 iSeriesId ,Int32 iGroupId,Int32 iAngleId = -3 , Int32 iSliceId = -3);
        virtual bool getGenericFillFormatAlpha(unsigned char& orAlpha,Int32 iSeriesId ,Int32 iGroupId,Int32 iAngleId = -3 , Int32 iSliceId = -3);
        virtual bool getGenericFillFormatGradient(Int32 iSeriesId ,Int32 iGroupId, Int32 iSliceId, unsigned int& orColor1, unsigned int& orColor2, int& orOrentation);
        virtual bool getGenericLineThickness(int& orLineThickness,Int32 iSeriesId ,Int32 iGroupId,Int32 iAngleId = -3 , Int32 iSliceId = -3);
        virtual bool getGenericLineStyle(unsigned char& orLineStyle,Int32 iSeriesId ,Int32 iGroupId,Int32 iAngleId = -3 , Int32 iSliceId = -3);
        virtual bool getGenericLineColor(unsigned int& orLineColor,Int32 iSeriesId ,Int32 iGroupId,Int32 iAngleId = -3 , Int32 iSliceId = -3);
        virtual bool GetAxisOriginValue(double& orAxisOriginValue , Int32 iObjectId);
        virtual bool GetAxisOriginLineThickness(Int32& orLineThickness,Int32 iObjectID);
        virtual bool GetAxisOriginLineStyle(unsigned char& orLineStyle,Int32 iObjectID);
        virtual bool GetAxisOriginLineColor(unsigned int& orLineColor,Int32 iObjectID);
		/*** Add iOS specific function within the following ifdef only ***/
#ifndef _VE_CE
        virtual HRESULT InitFromTemplateAndGrid(DSSTemplate* ipTemplate, DSSRWGridIterator* ipGridIterator, bool iIsParseTotals);
        virtual void SetGlobalIterator(DSSRWGridIterator* ipIterator);
        virtual bool PopulateReformer(DSSRWGridIterator* ipGridIterator, DataSetReformInfo* ipReformInfo);
        virtual bool hGetGradientPropertyForThreshold(DSSFormat* ipFormat, CComVariant* &opValue);
        virtual HRESULT InitFromTemplateSlice(DSSTemplate* ipTemplate, DSSRWGridIterator* ipGridIterator, Int32 iSliceID, bool iIsCacheData, bool iIncludeSubtotal);
#else
        virtual bool hGetGradientPropertyForThreshold(IDSSFormat* ipFormat, CComVariant* &opValue);
#endif

        
    protected:
        //void *mpGMDataSet;
        GraphMatrixDataSetWrapper* mpGraphMatrixDataSetWrapper;
    };

}
#endif
