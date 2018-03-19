#ifndef MsiChart_GraphMatrixDataSetWrapperIOSHelper_h
#define MsiChart_GraphMatrixDataSetWrapperIOSHelper_h

//#import "../../../../../IPhone/IPadDashboardViewer/Classes/Viewer/WidgetViewerClasses/GraphMatrix/CE/GMCELocalDataSet.h"

#import "GraphMatrixDataSetWrapperHelper.h"

namespace MsiChart{
    class GraphMatrixDataSetWrapperIOSHelper : public GraphMatrixDataSetWrapperHelper
    {
    public:
        GraphMatrixDataSetWrapperIOSHelper(GraphMatrixDataSetWrapper *ipGraphMatrixDataSetWrapper , void *mpGMDataSet);
        virtual ~GraphMatrixDataSetWrapperIOSHelper();
        bool GetSeriesByColumn() const;
        
        bool GetOneSliceRange(std::vector<Int32>& irSeriesIndex, std::vector<Int32>& irGroupIndex);
 
        HRESULT InitFromTemplateAndGrid(DSSTemplate* ipTemplate, DSSRWGridIterator* ipGridIterator, bool iIsParseTotals);
        Int32 GetDataGroup();
        
        HRESULT GetData(Int32 iSeriesID, Int32 iGroupID, const double*& iorValue, Int32 iColumnOffset, bool iIsForSlice);
        
        HRESULT GetPrimaryData(Int32 iSeriesID,Int32 iGroupID, const double*& iorValue, Int32 iPrimaryIndex, bool iIsForColor,  bool iIsForSlice, int iAngleIndex, int iSliceIndex);
        
        HRESULT GetGMPieSubtotalMaxMin(std::map<std::pair<Int32, Int32>, double> iPieSubtotalMap, double& orMax, double& orMin, Int32 iColumnOffset, DATA_RETRIEVAL_TYPE iType);
        HRESULT GetSeriesLabel(Int32 iSeriesID,MBase::String& orSeriesLabel, bool iSubstitute, Int32 iColumnOffset, bool iIsForSlice, LABEL_LEVEL_OPTION iOption, bool iIsForEmptyAxisWithTemplateMetrc);
        HRESULT GetGroupLabel(Int32 iGroupID, MBase::String& orGroupLabel, bool iSubstitute, Int32 iColumnOffset, bool iIsForSlice, LABEL_LEVEL_OPTION iOption);
        
        void ConvertLocalAEIndexToGlobal(int& ioRowIndex, int &ioColIndex);
        HRESULT GetValueAxisNumberFormat(ChartAdvFormat& orFormat, bool iConsiderThresholds, Int32 iStartIndex, Int32 iStep);
        HRESULT GetValueAxisNumberFormat(ChartAdvFormat& orFormat, const std::set<Int32>& irMetricSet);
        HRESULT GetGridDataFormat(ChartAdvFormat& orFormat, Int32 iSeriesId, Int32 iGroupId, Int32 iColumnOffset, Int32 iAngleId, Int32 iSliceId);
        HRESULT GetGridDataFormat(ChartAdvFormat& orFormat, Int32 iHostMetricIndex, bool iIsAngleMetricIndex, Int32 iGroupId);
        HRESULT	GetThresholdFillColor(Int32 iSeriesId, Int32 iGroupId, CComVariant* opValue);
        double GetThresholdBackgroundColor(Int32 iMetrixId, Int32 iThresholdId, double iDefaultColorValue);
        HRESULT GetGroupCount(Int32& orGroupCount);
        bool GetMetricsCount(Int32& orCount);
        Int32 GetMetricsCount(bool iIsAtTopLevel);
        //lizhang, de38822, 20160727, specialcase in combo
        bool IsReformed();
        bool IsMetricBySeries();
        MetricUnitInfo* GetMetricInfo();
        Int32 GetColorStyleIndex(Int32 iLocalIndex);
        bool IsHeaderEmpty(bool iIsForRow);
        Int32 GetMetricIndex(Int32 iRowIndex, Int32 iColIndex);
        
        void SeriesGroupIndexToRowColumnIndex(Int32 iSeriesID, Int32 iGroupID, Int32& orRowIndex, Int32& orColumnIndex, Int32 iColumnOffset, Int32 iAngleId, Int32 iSliceId, bool iIsConsiderSubtotal);
        void GetRowAndColumnIndexForMinMaxMetrixIndex(bool iIsForMax, std::vector<Int32> iMetricIndex, Int32& orRowIndex, Int32& orColumnIndex);
        void SetFormatManager(FormatManager* ipFormatManager);
        
        int GetGMColorByIndex(Int32 iSeriesID, Int32 iGroupID, Int32 iAngleIndex, Int32 iSliceIndex);
        void getLineMarkerColorSetting(std::vector<int>& iSetting);
        void SetGlobalIterator(DSSRWGridIterator* ipIterator);
        void GetLabelCollection(Int32 iOrdinal, bool iIsForRow, std::vector<std::string>&orCollection);
        
        void GetTextDataLabel(Int32 iGroupId, Int32 iSeriesId, std::string& orCollection, Int32 iAngleId, Int32 iSliceId);
        
        void GetTextDataLabel(Int32 iOrdinal, std::string& orCollection);
        
        bool PopulateReformer(DSSRWGridIterator* ipGridIterator, DataSetReformInfo* ipReformInfo);

        Int32 GetBalancedCount(bool iIsForGroup);
        const DataGroupInfo* GetDataGroupInfo();
        void PrepareIterator();
 
        
        bool hGetGradientPropertyForThreshold(DSSFormat* ipFormat, CComVariant* &opValue);
        
        
        HRESULT GetPieAngleCount(Int32 iGroupId, Int32 iSeriesId, Int32& orAngleCount);
        HRESULT GetPieSliceCount(Int32 iGroupId, Int32 iSeriesId, Int32 iAngleId, Int32& orSliceCount);
        HRESULT GetPieSliceData(Int32 iGroupId, Int32 iSeriesId, Int32 iAngleId, Int32 iSliceId, const double*& iorValue, Int32 iColumnOffset);
        HRESULT GetData(Int32 iGroupId, Int32 iSeriesId, EnumGraphMatrixDataIndexType iDataIndexType, const double*& iorValue, Int32 iAngleId);
        HRESULT GetDataPositionPartSize(Int32& orDataPositionPartSize);

        Int32 GetMetricIndex(Int32 iGroupId, Int32 iSeriesId, Int32 iAngleIndex, Int32 iSliceIndex);
        bool IsGroupOnScreen(Int32 iGroupId);
        bool IsSeriesOnScreen(Int32 iSeriesId);
        Int32 GetShapeBySeriesId(Int32 iSeriesId);
        bool IsDataLabelEnaled(Int32 iSeriesId, Int32 iGroupId, Int32 iAngleId = -3, Int32 iSliceId = -3);
        std::set<int> GetSeriesInSecondaryAxis();
        HRESULT InitFromTemplateSlice(DSSTemplate* ipTemplate, DSSRWGridIterator* ipGridIterator, Int32 iSliceID, bool iIsCacheData, bool iIncludeSubtotal);
        HRESULT GetSeriesCount(Int32& orSeriesCount);
        HRESULT GetDataSizeByPartSize(Int32& orDataSizeByPartSize);
        HRESULT GetDataAdditionPartSize(Int32& orDataAdditionPartSize);
        Int32 GetColorStyleNumber();
        Int32 GetMetricLabelCount();
        std::pair<int, int> getOnScreenTextRange(bool isForGroup);
        
        // [GM Trendline Support]
        std::vector<double> GetTrendLineValuesWithForecast(Int32 iSeriesId);
        bool IsExpomentialBaseNearOne(Int32 iSeriesId);
        std::vector<double> GetCoefficients(Int32 iSeriesId);
        
        //Reference line info
        std::vector<double> GetRefLineValues(Int32 iSeriesId);
        std::vector<double> GetRefLineValuesForBubble(Int32 iIndex, Int32 iAxisIndex);
        void GetRefLineLable(Int32 iSeriesId, Int32 iRefLineIndex, std::string& iText, bool& orIsShowValue, double& orValue);
        bool GetReferenceLineThickness(Int32& orLineThickness,Int32 iSeriesId, Int32 iGroupId);
        bool GetReferenceLineStyle(unsigned char& orLineStyle,Int32 iSeriesId, Int32 iGroupId);
        bool GetReferenceLineColor(unsigned int& orLineColor,Int32 iSeriesId, Int32 iGroupId);
        bool GetReferenceLineFontColor(unsigned int& orFontColor,Int32 iSeriesId,Int32 iGroupId);
        bool GetReferenceLineFontSize(Int32& orFontSize,Int32 iSeriesId,Int32 iGroupId);
        bool GetReferenceLineFontStyle(unsigned char& orFontStyle,Int32 iSeriesId,Int32 iGroupId);
        bool GetReferenceLineFontName(std::string& orFontName,Int32 iSeriesId,Int32 iGroupId);
        bool GetTrendLineThickness(Int32& orLineThickness,Int32 iSeriesId, Int32 iGroupId);
        bool GetTrendLineStyle(unsigned char& orLineStyle,Int32 iSeriesId, Int32 iGroupId);
        bool GetTrendLineColor(unsigned int& orLineColor,Int32 iSeriesId, Int32 iGroupId);

        //Axis Label properties
        bool getAxisLabelFontSize(bool isYAxis,  bool isPrimary, int& oFontSize);
        bool getAxisLabelFontName(bool isYAxis,  bool isPrimary, std::string& oFontName);
        bool getAxisLabelFontStyle(bool isYAxis,  bool isPrimary, unsigned char& oFontStyle);
        bool getAxisLabelFontColor(bool isYAxis,  bool isPrimary, unsigned int& oFontColor);
        bool GetAxisMajorTickLineThicknessFormat(Int32& orLineThickness,Int32 iObjectID);
        bool GetAxisMajorTickLineStyleFormat(unsigned char& orLineStyle,Int32 iObjectID);
        bool GetAxisMajorTickLineColorFormat(unsigned int& orLineColor,Int32 iObjectID);
        
        bool getDataLabelFontSize(int& orFontSize);
        bool getDataLabelFontStype(unsigned char& orFontStyle);
        bool getDataLabelFontName(std::string& orFontName);
        bool getDataLabelFontColor(unsigned int& orFontColor);
        
        //Shape element format setting
        bool getGenericFillFormatColor(unsigned int& orColor, Int32 iSeriesId ,Int32 iGroupId,Int32 iAngleId = -3 , Int32 iSliceId = -3);
        bool getGenericFillFormatType(unsigned char& orFillType,Int32 iSeriesId ,Int32 iGroupId,Int32 iAngleId = -3 , Int32 iSliceId = -3);
        bool getGenericFillFormatAlpha(unsigned char& orAlpha,Int32 iSeriesId ,Int32 iGroupId,Int32 iAngleId = -3 , Int32 iSliceId = -3);
        bool getGenericFillFormatGradient(Int32 iSeriesId ,Int32 iGroupId, Int32 iSliceId, unsigned int& orColor1, unsigned int& orColor2, int& orOrentation);
        bool getGenericLineThickness(int& orLineThickness,Int32 iSeriesId ,Int32 iGroupId,Int32 iAngleId = -3 , Int32 iSliceId = -3);
        bool getGenericLineStyle(unsigned char& orLineStyle,Int32 iSeriesId ,Int32 iGroupId,Int32 iAngleId = -3 , Int32 iSliceId = -3);
        bool getGenericLineColor(unsigned int& orLineColor,Int32 iSeriesId ,Int32 iGroupId,Int32 iAngleId = -3 , Int32 iSliceId = -3);
        
        bool GetAxisOriginValue(double& orAxisOriginValue , Int32 iObjectId);
        bool GetAxisOriginLineThickness(Int32& orLineThickness,Int32 iObjectID);
        bool GetAxisOriginLineStyle(unsigned char& orLineStyle,Int32 iObjectID);
        bool GetAxisOriginLineColor(unsigned int& orLineColor,Int32 iObjectID);
        
        void* mpGMDataSet;
    };
}
#endif