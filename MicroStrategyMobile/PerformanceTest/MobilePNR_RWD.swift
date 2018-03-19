//
//  MobilePNR_UITesting.swift
//  MobilePNR_UITesting
//
//  Created by Administrator on 10/12/15.
//  Copyright © 2015 MicroStrategy Inc. All rights reserved.
//

import XCTest

var ENABLE_MANI: Bool = true
var LONG_PRESS: TimeInterval = 2
var IPAddress: String = "10.197.34.137"

class MobilePNR_RWD: XCTestCase {
    
    let app = XCUIApplication();
    let device = XCUIDevice();
    
        
    override func setUp() {
        super.setUp()
        
        // Put setup code here. This method is called before the invocation of each test method in the class.
        
        // In UI tests it is usually best to stop immediately when a failure occurs.
        continueAfterFailure = false

        // UI tests must launch the application that they test. Doing this in setup will make sure it happens for each test method.
        XCUIApplication().launch()
        
        sleep(7)

        // In UI tests it’s important to set the initial state - such as interface orientation - required for your tests before they run. The setUp method is a good place to do this.
    }
    
    override func tearDown() {
        // Put teardown code here. This method is called after the invocation of each test method in the class.
        
        app.terminate()
        super.tearDown()
    }
    
    func test_001_FAFlash(){
        ActionUtils.clearCache(app, test: self)
        ActionUtils.scrollAndTap("MicroStrategy Tutorial (HQ)", application: app, test: self)
        ActionUtils.scrollAndTap("PM KPI Sheet", application: app, test: self)
        ActionUtils.scrollAndTap("Flash", application: app, test: self)
        
        
        for i in 0..<3 {
            
            if (i == 1){
                ActionUtils.clearCache(app, test: self)
            }
            ActionUtils.scrollAndTap("FA - Flash - Cached_notFullScreen", application: app, test: self)
            
            ActionUtils.verifyAndTapElement("Page by:", application: app, type: .staticText, test: self, tap: false)
            ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
            
            if(i == 2 && ENABLE_MANI){
                app.buttons["Visa"].tap()
                app.tables.staticTexts["Amex"].tap()
                sleep(3)
                ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
                sleep(5)
                app.scrollViews.otherElements["selector, currently selected item is:Panel1"].tap()
                app.tables.staticTexts["Panel3"].tap()
                sleep(5)
                // app.scrollViews.elementBoundByIndex(0).otherElements[", Unit Cost, Column Headings"].pressForDuration(LONG_PRESS)
                ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
                sleep(5)
                app.scrollViews.containing(.other, identifier:", Unit Cost, Column Headings").element(boundBy: 0).otherElements[", Unit Cost, Column Headings"].press(forDuration: LONG_PRESS)
                ActionUtils.verifyAndTapElement("Sort", application: app, type: .button, test: self, tap: true)
                sleep(2)
                ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
                sleep(5)
                app.buttons["Close"].tap()
                
            }
            app.buttons["Close"].tap()
            // ActionUtils.quitFullScreenAndClose(app, test: self)
        }
        
        //  app.terminate()
        
        
    }
    
    func test_002_SelectorUC() {
        // Use recording to get started writing UI tests.
        // Use XCTAssert and related functions to verify your tests produce the correct results.
        ActionUtils.clearCache(app, test: self)
        app.tables.staticTexts["MicroStrategy Tutorial"].tap()
        ActionUtils.scrollAndTap("_Sil", application: app, test: self)
        ActionUtils.scrollAndTap("Performance Automation", application: app, test: self)
        
        //001: 3UC_5grids
        for i in 0..<2 {

            app.tables.staticTexts["3UC_5grids"].tap()
            ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)

        
            if(i == 1 && ENABLE_MANI){
                app.scrollViews.otherElements["selector, currently selected item is:January"].tap()
                app.tables.staticTexts["February"].tap()
                ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
                app.scrollViews.otherElements["selector, currently selected item is:February"].tap()
                app.tables.staticTexts["January"].tap()
                ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
            }
            app.buttons["Close"].tap()
        }
        
      //  app.terminate()
    }
    
/*    func test_002_SelectoreBay(){
    
        ActionUtils.clearCache(app, test: self)
        app.tables.staticTexts["MicroStrategy Tutorial"].tap()
        ActionUtils.scrollAndTap("_Sil", application: app, test: self)
        ActionUtils.scrollAndTap("Performance Automation", application: app, test: self)
        
        
        //002: eBay_Small
        for i in 0..<2  {
            ActionUtils.scrollAndTap("eBay_small", application: app, test: self)
            ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
            
            if(i == 1 && ENABLE_MANI){
                app.scrollViews.otherElements["selector, currently selected item is:Alabama"].tap()
                app.tables.staticTexts["Arizona"].tap()
                ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
                app.scrollViews.otherElements["selector, currently selected item is:Arizona"].tap()
                app.tables.staticTexts["Alabama"].tap()
                ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
            }
            app.buttons["Close"].tap()
        }
        
      //  app.terminate()
    }
    
    func test_003_SelectorManipulationCache(){
        
        ActionUtils.clearCache(app, test: self)
        app.tables.staticTexts["MicroStrategy Tutorial"].tap()
        ActionUtils.scrollAndTap("_Sil", application: app, test: self)
        ActionUtils.scrollAndTap("Performance Automation", application: app, test: self)
        
    
        //003: eBay_Samll_Manipulation_Cache
        for i in 0..<2  {
            ActionUtils.scrollAndTap("eBay_small_Manipulation Cache", application: app, test: self)
            ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
            
            if(i == 1 && ENABLE_MANI){
                app.scrollViews.otherElements["selector, currently selected item is:Alabama"].tap()
                app.tables.staticTexts["Arizona"].tap()
                ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
                app.scrollViews.otherElements["selector, currently selected item is:Arizona"].tap()
                app.tables.staticTexts["Arkansas"].tap()
                ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
                app.scrollViews.otherElements["selector, currently selected item is:Arkansas"].tap()
                app.tables.staticTexts["Alabama"].tap()
                ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
            }
            app.buttons["Close"].tap()
        }
        
      //  app.terminate()
    }*/
    
    func test_004_CGBonFG(){
        
        ActionUtils.clearCache(app, test: self)
        app.tables.staticTexts["MicroStrategy Tutorial"].tap()
        ActionUtils.scrollAndTap("_Sil", application: app, test: self)
        ActionUtils.scrollAndTap("Performance Automation", application: app, test: self)
        
    
    
        for i in 0..<2  {
            ActionUtils.scrollAndTap("2.1_CGBonFG_37MB_Customer", application: app, test: self)
            ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
            
            if(i == 1 && ENABLE_MANI){
                app.scrollViews.otherElements["selector, currently selected item is:Jan 2007"].tap()
                app.tables.staticTexts["Feb 2007"].tap()
                ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
                app.scrollViews.otherElements["selector, currently selected item is:Feb 2007"].tap()
                app.tables.staticTexts["Jan 2007"].tap()
                ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
            }
            app.buttons["Close"].tap()
        }
        
      //  app.terminate()
    }
    
    func test_005_CGBonGrid(){
        ActionUtils.clearCache(app, test: self)
/*        app.tables.staticTexts["MicroStrategy Tutorial"].tap()
        ActionUtils.scrollAndTap("_Sil", application: app, test: self)
        ActionUtils.scrollAndTap("Performance Automation", application: app, test: self)
        
    
    
        for i in 0..<2  {
            ActionUtils.scrollAndTap("1.1_1Grid_94MB", application: app, test: self)
            ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
            if(i == 1 && ENABLE_MANI){
                app.scrollViews.otherElements["selector, currently selected item is:January"].tap()
                app.tables.staticTexts["February"].tap()
                ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
                app.scrollViews.otherElements["selector, currently selected item is:February"].tap()
                app.tables.staticTexts["January"].tap()
                ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
            }
            app.buttons["Close"].tap()
        }*/
        
        app.tables.staticTexts["Drill path"].tap()
        ActionUtils.scrollAndTap("_Sarah", application: app, test: self)
        ActionUtils.scrollAndTap("Slice Size test", application: app, test: self)
        
        
        
        for i in 0..<2  {
            ActionUtils.scrollAndTap("2_Grid_138", application: app, test: self)
            ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
            if(i == 1 && ENABLE_MANI){
                app.scrollViews.otherElements["selector, currently selected item is:January"].tap()
                app.tables.staticTexts["February"].tap()
                ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
                app.scrollViews.otherElements["selector, currently selected item is:February"].tap()
                app.tables.staticTexts["January"].tap()
                
                ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
            }
            app.buttons["Close"].tap()
        }
        
      //  app.terminate()
    }
    
    func test_006_Collapable(){
        
        ActionUtils.clearCache(app, test: self)
        app.tables.staticTexts["MicroStrategy Tutorial"].tap()
        ActionUtils.scrollAndTap("_Sil", application: app, test: self)
        ActionUtils.scrollAndTap("Performance Automation", application: app, test: self)
        
    
    
        for i in 0..<2  {
            ActionUtils.scrollAndTap("_Collapable Panel", application: app, test: self)
            ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
            if(i == 1 && ENABLE_MANI){
                
                app.buttons["Filter"].tap()
                app.staticTexts["Customer"].tap()
                app.staticTexts["Abarca:Hugh"].tap()
                ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
                app.buttons["All"].tap()
                //  ActionUtils.verifyAppear("Close", application: app, type: .Button, test: self)
                // ActionUtils.printDescription(app)
                app.staticTexts["(All)"].tap()
                sleep(10)
                ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
                
            }
            app.buttons["Close"].tap()
        }
        
      //  app.terminate()
    }
    
/*    func test_007_FilterPanel(){
        
        ActionUtils.clearCache(app, test: self)
        app.tables.staticTexts["MicroStrategy Tutorial"].tap()
        ActionUtils.scrollAndTap("_Sil", application: app, test: self)
        ActionUtils.scrollAndTap("Performance Automation", application: app, test: self)
        
    
    
        for i in 0..<2  {
            ActionUtils.scrollAndTap("1000_Filter Panel", application: app, test: self)
            ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
            if(i == 1 && ENABLE_MANI){
                app.staticTexts["Abarca:Hugh, selected, List Box"].tap()
                ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
                
            }
            app.buttons["Close"].tap()
        }
        
      //  app.terminate()
    }
    */
    func test_008_01_CarryOver(){
        
        ActionUtils.clearCache(app, test: self)
        app.tables.staticTexts["MicroStrategy Tutorial"].tap()
        ActionUtils.scrollAndTap("_Sil", application: app, test: self)
        ActionUtils.scrollAndTap("Performance Automation", application: app, test: self)
        
    
    
        for i in 0..<2  {
            ActionUtils.scrollAndTap("2.1_CGBonFG_37MB_Carry Over", application: app, test: self)
            ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
            if(i == 1 && ENABLE_MANI){
                app.staticTexts["Link To UC_5Grid (Carry over selection) This is a Link"].tap()
                ActionUtils.verifyAndTapElement("The grids in this document have more than 100000 cells and are too big to be rendered. Please reduce the size of one of the grids.", application: app, type: .staticText, test: self, tap: false)
                ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
                app.buttons.matching(identifier: "Back").element(boundBy: 0).tap()
                ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
                
            }
            app.buttons["Close"].tap()
        }
        
        sleep(3)
     //   app.terminate()

    }
    
    func test_008_02_AttributeSelector(){
        ActionUtils.clearCache(app, test: self)
        ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
        ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
        ActionUtils.scrollAndTap("_Performance Automation", application: app, test: self)
        
        
        for i in 0..<2  {
            ActionUtils.scrollAndTap("Attribute Selector", application: app, test: self)
            ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
            if(i == 1 && ENABLE_MANI){
                app.scrollViews.otherElements["selector, currently selected item is:Customer City"].tap()
                app.tables.staticTexts["Year"].tap()
                ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)

            }
            app.buttons["Close"].tap()
        }

        
        
        
    }
    
    func test_009_ReportGrid(){
        
        ActionUtils.clearCache(app, test: self)
        ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
        ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
        ActionUtils.scrollAndTap("_Performance Automation", application: app, test: self)
        for _ in 0..<2 {
            ActionUtils.scrollAndTap("90,000 Cells_Formatting", application: app, test: self)
            ActionUtils.verifyAndTapElement("Switch to Graph View", application: app, type: .button, test: self, tap: false)
            app.buttons["Close"].tap()
            ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
        }
        
        
     //   app.terminate()
    }
    
    func test_010_HTMLGrid(){
    
        ActionUtils.clearCache(app, test: self)
        ActionUtils.scrollAndTap("HTML Grid Support", application: app, test: self)
        ActionUtils.scrollAndTap("HTML Grid Cell Test", application: app, test: self)
        ActionUtils.scrollAndTap("HTML Report", application: app, test: self)
        
        for _ in 0..<2 {
            ActionUtils.scrollAndTap("6 Attribute - 84000 cells", application: app, test: self)
            ActionUtils.verifyAndTapElement("Switch to Graph View", application: app, type: .button, test: self, tap: false)
            app.buttons["Close"].tap()
            ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
        }
    
/*        app.buttons["Home"].tap()
        
        app.tables.staticTexts["MicroStrategy Tutorial"].tap()
        ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
        ActionUtils.scrollAndTap("_Performance Automation", application: app, test: self)
       for _ in 0..<2 {
            ActionUtils.scrollAndTap("HTML Tags_1000", application: app, test: self)
            app.buttons["Close"].tap()
            ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
        }
        */
        
      //  app.terminate()
    }
    
    func test_011_DocGrid(){
        
        ActionUtils.clearCache(app, test: self)
    
        app.tables.staticTexts["MicroStrategy Tutorial"].tap()
        ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
        ActionUtils.scrollAndTap("_Performance Automation", application: app, test: self)
        
        for _ in 0..<2 {
            ActionUtils.scrollAndTap("6 x 15,000 Grid", application: app, test: self)
            app.buttons["Close"].tap()
            ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
        }
        
      //  app.terminate()
    }
    
    func test_012_RapidReporting(){
        
        ActionUtils.clearCache(app, test: self)
        
        app.tables.staticTexts["MicroStrategy Tutorial"].tap()
        ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
        ActionUtils.scrollAndTap("_Performance Automation", application: app, test: self)
    
        for _ in 0..<2 {
            ActionUtils.scrollAndTap("09.Year PB_14400display_43200", application: app, test: self)
            ActionUtils.verifyAndTapElement("Switch to Graph View", application: app, type: .button, test: self, tap: false)
            app.buttons["Close"].tap()
            ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
        }
        
      //  app.terminate()
    }
    
    func test_013_Drilling(){
    
        ActionUtils.clearCache(app, test: self)
        
        app.tables.staticTexts["MicroStrategy Tutorial"].tap()
        ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
        ActionUtils.scrollAndTap("_Performance Automation", application: app, test: self)
        
        ActionUtils.scrollAndTap("Sync. Drilling", application: app, test: self)
        for i in 0..<2 {
            ActionUtils.scrollAndTap("T3", application: app, test: self)
            ActionUtils.verifyAndTapElement("Customer", application: app, type: .staticText, test: self, tap: false)
            ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
            // print(XCUIApplication().debugDescription)
            if(i == 1 && ENABLE_MANI){
                // app.staticTexts["Customer Age: 79, Aaby"].pressForDuration(LONG_PRESS)
                
                app.scrollViews.otherElements["Customer Age: 80, Aaby"].press(forDuration: LONG_PRESS);
                app.buttons["Drill"].tap()
                app.tables.staticTexts["Day"].tap()
                ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
                
            }
            app.buttons["Close"].tap()
        }
        
/*        for _ in 0..<2 {
            ActionUtils.scrollAndTap("T3_After drilling", application: app, test: self)
            ActionUtils.verifyAndTapElement("Customer", application: app, type: .staticText, test: self, tap: false)
            app.buttons["Close"].tap()
            ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
        }
        */
       // app.terminate()
    }
    
    func test_014_DynamicText(){
        
        ActionUtils.clearCache(app, test: self)
        
        app.tables.staticTexts["MicroStrategy Tutorial"].tap()
        ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
        ActionUtils.scrollAndTap("_Performance Automation", application: app, test: self)

        
        for _ in 0..<2 {
            ActionUtils.scrollAndTap("20000_Details Section", application: app, test: self)

            app.buttons["Close"].tap()
            sleep(3)
            ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
        }
        
        sleep(3)
        
        for _ in 0..<2 {
            ActionUtils.scrollAndTap("20000_Details Section_Copy/Paste", application: app, test: self)
            app.buttons["Close"].tap()
            sleep(3)
            ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
        }
        
      //  app.terminate()
    }
    
    func test_015_StaticText(){
        
    
        ActionUtils.clearCache(app, test: self)
        
        app.tables.staticTexts["MicroStrategy Tutorial"].tap()
        ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
        ActionUtils.scrollAndTap("_Performance Automation", application: app, test: self)
    
        for _ in 0..<2 {
            ActionUtils.scrollAndTap("800 Text", application: app, test: self)
            app.buttons["Close"].tap()
            sleep(3)
            ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
        }
      //  app.terminate()
    }

    
  

    func test_016_PDFReader(){
        
        ActionUtils.clearCache(app, test: self)
        ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
        ActionUtils.scrollAndTap("Skye", application: app, test: self)
        ActionUtils.scrollAndTap("PDF Reader", application: app, test: self)
        
        for _ in 0..<2 {
            ActionUtils.scrollAndTap("50MPDF", application: app, test: self)
            ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
            ActionUtils.verifyAndTapElement("50M PDF This is a Link", application: app, type: .staticText, test: self, tap: true)
            
          //  ActionUtils.printDescription(app)
           // ActionUtils.verifyAndTapElement("navi bar back@2x", application: app, type: .Button, test: self, tap: true)
            ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
            ActionUtils.verifyAndTapElement("1/4732", application: app, type: .staticText, test: self, tap: true)
            app.buttons.matching(identifier: "Back").element(boundBy: 0).tap()

            app.buttons["Close"].tap()
        }
        
        sleep(3)
        
     //   app.terminate()
    }
    
    func test_017_DrillOnGraph(){
        ActionUtils.clearCache(app, test: self)
        
        app.tables.staticTexts["MicroStrategy Tutorial"].tap()
        ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
        ActionUtils.scrollAndTap("_Performance Automation", application: app, test: self)
        
        for i in 0..<2 {
            ActionUtils.scrollAndTap("Drill on Graph", application: app, test: self)
            ActionUtils.verifyAndTapElement("Switch to Grid View", application: app, type: .button, test: self, tap: false)
            ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
           // ActionUtils.printDescription(app)
            if (i == 1 && ENABLE_MANI){
              //  app.childrenMatchingType(.Other).matchingIdentifier("This is a Vertical Bar chart..Series: Revenue\rCategory: 2007 Kids / Family\rValue: $163,923").elementBoundByIndex(10).pressForDuration(LONG_PRESS)
                let element = app.coordinate(withNormalizedOffset: CGVector(dx: 0, dy: 0)).withOffset(CGVector(dx: 310, dy: 539))
                element.press(forDuration: LONG_PRESS)
                //ActionUtils.printDescription(app)
                ActionUtils.verifyAndTapElement("Drill", application: app, type: .staticText, test: self, tap: false)
                app.staticTexts["Drill"].tap()
                
                ActionUtils.verifyAndTapElement("Customers (Drilling)", application: app, type: .staticText, test: self, tap: true)
                ActionUtils.verifyAndTapElement("Customer", application: app, type: .staticText, test: self, tap: true)
                sleep(7)
                ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
            }
            
            app.buttons["Close"].tap()
            
        }
        
    }
    
    func test_017_ImageGrid(){
        ActionUtils.clearCache(app, test: self)
        
        app.tables.staticTexts["MicroStrategy Tutorial"].tap()
        ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
        ActionUtils.scrollAndTap("_Performance Automation", application: app, test: self)
        
        for _ in 0..<2 {
            ActionUtils.scrollAndTap("Gucci_250images_ColumnBMP", application: app, test: self)
            sleep(10)
            //ActionUtils.printDescription(app)
            app.buttons["Close"].tap()
            ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
        }
        
        sleep(5)
       
        for _ in 0..<2 {
            ActionUtils.scrollAndTap("Gucci_250images_ColumnPNG", application: app, test: self)
            sleep(10)
            app.buttons["Close"].tap()
            ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
        }
        
        sleep(5)
        
        for _ in 0..<2 {
            ActionUtils.scrollAndTap("Gucci_250images_Column", application: app, test: self)
            sleep(10)
            app.buttons["Close"].tap()
            ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
        }
        
        app.terminate()
        
    }
    
    func test_018_AttributeThreshold(){
        ActionUtils.clearCache(app, test: self)
        
        app.tables.staticTexts["MicroStrategy Tutorial"].tap()
        ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
        ActionUtils.scrollAndTap("_Performance Automation", application: app, test: self)
        
        for _ in 0..<2 {
            ActionUtils.scrollAndTap("AttributeThreshold", application: app, test: self)
            
            app.buttons["Close"].tap()
            ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
        }
        
    }
    
    func test_019_VIGrid(){
        ActionUtils.clearCache(app, test: self)
        
        app.tables.staticTexts["MicroStrategy Tutorial"].tap()
        ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
        ActionUtils.scrollAndTap("Grid", application: app, test: self)
        ActionUtils.scrollAndTap("VI Grid", application: app, test: self)
        
        for _ in 0..<2 {
            ActionUtils.scrollAndTap("12 Cells_4999Selected", application: app, test: self)
            
            app.buttons["Close"].tap()
            ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
        }
        
    }
    
    
    func test_last_deleteServerCache(){
        let urlAddress = "http://10.197.34.137:9090/"
        
        ActionUtils.clearServerCache(urlAddress, dataString: "delete")
        sleep(5)
        
        
        
    }
    
}
