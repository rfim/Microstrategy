#ifndef MsiChart_NumberObjectHelper_h
#define MsiChart_NumberObjectHelper_h

#include "ChartContext.h"

namespace MsiChart{
	class NumberObject;

    class NumberObjectHelper{
    public:
		NumberObjectHelper(NumberObject * iNumberObject,ChartContext::Ptr iChartContext):mpNumberObject(iNumberObject),mChartContextPtr(iChartContext){};
		virtual ~NumberObjectHelper(){mpNumberObject = NULL;mChartContextPtr = NULL;};
        virtual std::string ConvertToFraction(double value, std::string& formatString);
        virtual bool GetPreAndSuffix(std::string& iorFormatStr, std::string& orPrefix, std::string& orSuffix, bool& orHasBrakcket, bool iDividPosAndNeg = true);
        virtual void SetTextFromGrid(Dataset::Ptr iDatasetPtr, MFormat::NumberFormat* ipNumberFormat, LCID iLocale);
        virtual void SetNumberFormat(Dataset::Ptr iDatasetPtr, const ChartAdvFormat& irNumberFormat,  LCID iLocale);
        virtual void RemoveMinusFromNegExpComponet(std::string& iorNumberFormat);
        virtual bool replaceQuestionMarkWithHash(std::string& iorNumberFormat);
        virtual bool GetNumberFromText(double& orValue) const;
        virtual void ReplaceCurencySymbol(std::string& iorFormat);
        virtual void ParseConditions(std::string& iorFormat);
        virtual void SetCorrectDateFormatString(ChartAdvFormat& orFormat);
        virtual void MapFormatType(const TripleId& irTripleId,  ChartAdvFormat& iorNumberFormat);
    protected:
        NumberObject *mpNumberObject;
        ChartContext::Ptr mChartContextPtr;
    };
}
#endif