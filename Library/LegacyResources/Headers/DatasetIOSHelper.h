#ifndef Msi_DatasetIOSHelper
#define Msi_DatasetIOSHelper

#import "FormatInfo.h"
#import "DSSRWGridIterator.h"
#import "DSSTemplate.h"


namespace MsiChart{
    class FormattedDataString;
    class TMLocaleInfo;
    class DimTreeCreator;
    
    class FormattedDataStringHelper{
    public:
        FormattedDataStringHelper(FormattedDataString* ipFormattedDataString):mpFormattedDataString(ipFormattedDataString){};
        void SetData(const void* ipData, EnumDSSDataType iType, DSSDataFlag iFlag, Int32 iSize, Int32 iBaseFormType, FormatInfo& iFormat, CComVariant* ipNumberCategory, CComVariant* ipNumberFormat);
        MBase::String GetData();
    private:
        FormattedDataString *mpFormattedDataString;
    };
    
    
    class TMLocaleInfoHelper{
    public:
        TMLocaleInfoHelper(TMLocaleInfo* ipTMLocaleInfo):mpTMLocaleInfo(ipTMLocaleInfo){};
        virtual ~TMLocaleInfoHelper();
        void Populate();
        void ConvertToDateTimeParts(const void* ipDateData, std::vector<std::string>& iorParts);
    private:
        TMLocaleInfo* mpTMLocaleInfo;
    };
    
    class DimTreeCreatorHelper{
    public:
        DimTreeCreatorHelper(DimTreeCreator *ipDimTreeCreator):mpDimTreeCreator(ipDimTreeCreator){};
        virtual ~DimTreeCreatorHelper(){mpDimTreeCreator = NULL;};
        bool GetAttrElementContent(std::vector<FormattedDataString>&orForms, DSSTemplateAttribute* ipAttr,  Int32 iDepth, bool iIsSubtotal);
        bool GetElementContent(std::vector<FormattedDataString>&orForms, DSSTemplateUnit* ipUnit, Int32 iDepth);
    private:
        DimTreeCreator *mpDimTreeCreator;
    };
}
#endif