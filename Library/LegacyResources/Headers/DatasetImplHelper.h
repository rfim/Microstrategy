#ifndef Msi_DatasetImplHelper
#define Msi_DatasetImplHelper
namespace MsiChart{
    class DatasetImpl;
    class DatasetImplHelper{
    public:
        DatasetImplHelper(DatasetImpl* ipDatasetImpl);
        virtual ~DatasetImplHelper();
        std::string GetAttrElementPieceFromOneFromForGM(DSSTemplateUnit* ipUnit, Int32 iAxisId, Int32 iOrdinal, Int32 iDepth, Int32 iFormId, Int32 iFormType, LINK_TEXT_TYPE iLinkType);
        HRESULT GetToolTipDataString(Int32 iSeriesID, Int32 iGroupID, MBase::String& orDataString, Int32 iOffset);
        bool PopulateDataCells(DSSRWGridIterator* ipGridIterator, Int32 iRowCount, Int32 iColCount);
        HRESULT	GetThresholdFillColor(Int32 iSeriesId, Int32 iGroupId, CComVariant* opValue);
        bool GetGradientPropertyForThreshold(DSSFormat* ipFormat, CComVariant* opValue);
        std::string hGetAttrElementPieceFromOneFrom(DSSTemplateUnit* ipUnit, Int32 iAxisId, Int32 iOrdinal, Int32 iDepth, Int32 iFormId, Int32 iFormType, LINK_TEXT_TYPE iLinkType);
 
    private:
        DatasetImpl* mpDatasetImpl;
    };
}

#endif
