#ifndef MsiChart_FormatManagerWINHelper_h
#define MsiChart_FormatManagerWINHelper_h

#include "StdAfx.h"
#include "DatasetHelper.h"

namespace MsiChart{
    
    typedef enum
	{
		FC_INVALID,
		FC_ATTR_TITLE,
		FC_ATTR_GRID,
		FC_SUBTOTAL_TITLE,
		FC_SUBTOTAL_GRID,
		FC_METRICS_TITLE,
		FC_METRIC_TITLE,
		FC_METRIC_GRID,
		FC_THRESHOLD,
		FC_HEADERS, //used in banding layer.
		FC_CORNER_ROW, //used in case where one header is empty while the other has metrics defined on it.	Row axis will control its format
		FC_CORNER_COL, //used in case where one header is empty while the other has metrics defined on it.  Col axis will control its format
		FC_CONSOLIDATION_ELEMENT_TITLE,
		FC_CONSOLIDATION_ELEMENT_GRID,
		FC_TEMPLATE_GRID, //Data grid when there is no metric in template.
	}FORMAT_CATEGORY;
    
    typedef enum
	{
		FLI_RESERVED,   //undefined
		FLI_TEMPLATE,	//< lowest layer
		FLI_BANDING,
		FLI_SUBTOTAL,
		FLI_CUSTOMGROUP,
		FLI_THRESHOLD,
		FLI_CONSOLIDATION,
		FLI_DERIVEDELEMENT,
		FLI_COMPOSITE_CG_CONSO,
		FLI_CELL
	}FORMAT_LAYER_INDEX;
    const FORMAT_LAYER_INDEX gFormattingLayerIndex[] = {FLI_THRESHOLD,FLI_DERIVEDELEMENT,FLI_COMPOSITE_CG_CONSO,
        FLI_SUBTOTAL, FLI_BANDING, FLI_TEMPLATE, FLI_CELL};
    
    class FormatManager;
    
    class FormatManagerHelper{
    public:
        FormatManagerHelper(FormatManager* ipFormatManager , ICDSSTemplate* ipTemplate , 
				 ICDSSRWIterator* ipIterator, MetricUnitInfo* ipInfo):
						mpFormatManager(ipFormatManager),mpTemplate(ipTemplate),
						mpIterator(ipIterator), mpInfo(ipInfo){};
        virtual ~FormatManagerHelper();
        CComVariant* GetFormat(Int32 iPropertyID);
    private:
        ICDSSTemplate* mpTemplate;
		ICDSSRWIterator* mpIterator;
        FormatManager *mpFormatManager;
        MetricUnitInfo* mpInfo;
    };
}
#endif