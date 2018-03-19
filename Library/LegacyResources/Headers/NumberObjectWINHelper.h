#ifndef MsiChart_NumberObjectWINHelper_h
#define MsiChart_NumberObjectWINHelper_h

#include "NumberObjectHelper.h"

namespace MsiChart{
    class NumberObjectWINHelper: public NumberObjectHelper{
    public:
		NumberObjectWINHelper(NumberObject * iNumberObject,ChartContext::Ptr iChartContext):NumberObjectHelper(iNumberObject, iChartContext){};
		~NumberObjectWINHelper(){};
        std::string ConvertToFraction(double value, std::string& formatString);
        bool GetPreAndSuffix(std::string& iorFormatStr, std::string& orPrefix, std::string& orSuffix, bool& orHasBrakcket, bool iDividPosAndNeg = true);
        void SetTextFromGrid(Dataset::Ptr iDatasetPtr, MFormat::NumberFormat* ipNumberFormat, LCID iLocale);
        void SetNumberFormat(Dataset::Ptr iDatasetPtr, const ChartAdvFormat& irNumberFormat,  LCID iLocale);
        void RemoveMinusFromNegExpComponet(std::string& iorNumberFormat);
        bool replaceQuestionMarkWithHash(std::string& iorNumberFormat);
        bool GetNumberFromText(double& orValue) const;
        void ReplaceCurencySymbol(std::string& iorFormat);
        void ParseConditions(std::string& iorFormat);
        void SetCorrectDateFormatString(ChartAdvFormat& orFormat);
        void MapFormatType(const TripleId& irTripleId,  ChartAdvFormat& iorNumberFormat);
    private:
        void hSwitchFormat(char* iopNumberFormatString ,int& iorFormatType);
    };
}
#endif