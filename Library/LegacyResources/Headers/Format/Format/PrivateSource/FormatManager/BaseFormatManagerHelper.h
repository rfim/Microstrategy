#ifndef MsiFormat_BaseFormatManagerHelper_h
#define MsiFormat_BaseFormatManagerHelper_h

//#import "DatasetHelper.h"
#import "TemplateUnitInfo.h"
#import "FormatInfo.h"
#import "DSSTemplate.h"

namespace MsiFormat{
    
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
    
    class BaseFormatManager;
    
    class BaseFormatManagerHelper{
    public:
        BaseFormatManagerHelper(BaseFormatManager* ipBaseFormatManager , DSSTemplate* ipTemplate , MetricUnitInfo* ipInfo):mpBaseFormatManager(ipBaseFormatManager),mpTemplate(ipTemplate),mpInfo(ipInfo){};
        virtual ~BaseFormatManagerHelper();
        bool SetExtraCloumnFlag();
        CComVariant* GetFormat(Int32 iPropertyID);
        FormatInfo* GetFormat(void* ipGridCell, bool iIsPassToGridCell);
        void SetDrillable(void* ipGridCell, FormatInfo::Ptr iFormatPtr);
        bool HasLinkedDrill(DSSTemplateUnit* ipTemplateUnit);
        Int32 GetRowHeight();  //for fix mode only
        Int32 GetColumnWidth(Int32 iColumnIndex);
        void RegisterOneLayer(FORMAT_LAYER_INDEX iLayerIndex, GUID& irID, FORMAT_CATEGORY& irCellType);
        /********************************************************************************
         *																				*
         *		Following are helper functions for debugging, private to this class		*
         *																				*
         *********************************************************************************/
        
        void DumpTotalNumberFormat();    //this is for [testing].
        bool HasBanding()const;
    private:
        DSSTemplate* mpTemplate;
        BaseFormatManager *mpBaseFormatManager;
        MetricUnitInfo* mpInfo;
    };
}
#endif