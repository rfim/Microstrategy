//
//  MIDisplayStyle.h
//  MicroStrategyMobile
//
//  Created by Jun Cheng on 5/12/11.
//  Copyright 2011 MicroStrategy Inc. All rights reserved.
//

typedef enum {
    MIAttributeElement,
	MICalendar,
	MIMap,
	MIHierarchy,
	MISwitcher,
	MIStepper,
	MITextBox,
	MISlider,
	MISliderMilt,
	MIBarcode,
	MIGeolocation,
	MIUnsupported,
	MIMectricSlider,
	MIMectricMenu,
	MISearch,
	MINumericPicker,
	MIAddNew,
    MIObjectPrompt,
    MICalendarPick,
    MIRecursiveAttributeElement // Added by wenzhou, 05/05/2016. To support recursive attribute of financial report. Related to SELECTOR_STYLE_RECURSIVE_ATTRIBUTE_FILTER.
} MIDisplayStyle;