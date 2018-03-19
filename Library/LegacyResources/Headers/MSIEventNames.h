//
//  MSIEventNames.h
//  MicroStrategyMobile
//
//  Created by Zeng, Yingpei on 5/21/14.
//  Copyright (c) 2014 MicroStrategy Inc. All rights reserved.
//
//  Centralize the event names of Microstrategy Mobile to here.

/**
 * The default event name, same as nil event name. It indicates that model is changed.
 */
extern NSString *const MSIModelDidChangeEvent;

/**
 * Event for that the secondary model is changed.
 */
extern NSString *const MSISecondaryModelDidChangeEvent; //@"secondaryNodeKey"

/**
 * Event for that internal views need to be recontructed.
 */
extern NSString *const MSIReconstructEvent;         //ReDrawEvent;@"ReDraw"@"Reconstruct"

/**
 * Event for that internal views need to be recontructed, and need to do a selection.
 */
extern NSString *const MSIReconstructAndSelectEvent;//@"ReDrawAndSelect"

/**
 * Drill event.
 */
extern NSString *const MSIDrillEvent;               //@"Drill"

/**
 * Changing visualization type event.
 */
extern NSString *const MSIChangeVizTypeEvent;       //@"ChangeVizType"

/**
 * Setting properties event.
 */
extern NSString *const MSISetPropertiesEvent;       //@"SetProperties"

/**
 * MoveLayout event.
 */
extern NSString *const MSIMoveLayoutManipulationEvent; //@"ManipulationMoveLayout"

/**
 * Selection only event.
 */
extern NSString *const MSISelectionOnlyEvent;          //@"selectionOnly"

/**
 * Event for updating from MacroManipulation.
 */
extern NSString *const MSIUpdateSelectorAfterMacroManipulationEvent;//@"updateSelectorAfterMacroManipulation"

/**
 * Event for when we need to call reInit.
 */
extern NSString *const MSINeedReInitEvent;            //@"Init"




// Transaction related


extern NSString *const MSIAutoRefreshEvent;       //@"autorefresh"
extern NSString *const MSIRequireValidationEvent; //REQUIRE_VALIDATION
extern NSString *const MSIDiscardEvent;           //@"Discard"
extern NSString *const MSIRecordEvent;            //@"Record"
extern NSString *const MSIAfterSubmitEvent;       //@"AfterSubmit"
extern NSString *const MSIConditionalChangeEvent; //@"conditionalChange"


