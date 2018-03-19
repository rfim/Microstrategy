//
//  MobilePNR_Visualization.swift
//  MicroStrategyMobile
//
//  Created by Administrator on 10/15/15.
//  Copyright © 2015 MicroStrategy Inc. All rights reserved.
//

import XCTest


class MobilePNR_Visualization: XCTestCase {
    
    let app = XCUIApplication()
    
    override func setUp() {
        super.setUp()
        
        // Put setup code here. This method is called before the invocation of each test method in the class.
        
        // In UI tests it is usually best to stop immediately when a failure occurs.
        continueAfterFailure = false
        // UI tests must launch the application that they test. Doing this in setup will make sure it happens for each test method.
        app.launch()
        sleep(5)
        
        ActionUtils.clearCache(app, test: self)

        // In UI tests it’s important to set the initial state - such as interface orientation - required for your tests before they run. The setUp method is a good place to do this.
    }
    
    override func tearDown() {
        // Put teardown code here. This method is called after the invocation of each test method in the class.
        app.terminate()
        super.tearDown()
    }
    
    func test_001_Graph() {
        ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
        ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
        ActionUtils.scrollAndTap("_Performance Automation", application: app, test: self)
        
        for _ in 0..<2 {
            ActionUtils.scrollAndTap("Graph_Area, Line, Bar_1000*3", application: app, test: self)

            app.buttons["Close"].tap()
            ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
            sleep(5)
        }
    }
    
    
    func test_002_GraphMatrxRS(){
        ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
        ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
        ActionUtils.scrollAndTap("_Performance Automation", application: app, test: self)
        ActionUtils.scrollAndTap("Widget", application: app, test: self)
        
        for _ in 0..<2 {
            ActionUtils.scrollAndTap("GraphMatrix_Subcategory", application: app, test: self)
            
            sleep(25)
            app.buttons["Close"].tap()
            ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
            sleep(5)
        }
    }
    
    func test_003_Heatmap(){
        ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
        ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
        ActionUtils.scrollAndTap("_Performance Automation", application: app, test: self)
        ActionUtils.scrollAndTap("Widget", application: app, test: self)
        
        for _ in 0..<2 {
            ActionUtils.scrollAndTap("Heatmap_Label", application: app, test: self)
            app.buttons["Close"].tap()
            ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
            sleep(5)
        }
        
        for _ in 0..<2 {
            ActionUtils.scrollAndTap("Heatmap_noLabel", application: app, test: self)
            app.buttons["Close"].tap()
            ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
            sleep(5)
        }
        
    }
    
    func test_004_ImageLayout(){
        ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
        ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
        ActionUtils.scrollAndTap("_Performance Automation", application: app, test: self)
        ActionUtils.scrollAndTap("Widget", application: app, test: self)
        
        for _ in 0..<2 {
            ActionUtils.scrollAndTap("Customer_4K", application: app, test: self)
            sleep(5)
            app.buttons["Close"].tap()
            ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
            sleep(5)
        }
        
        for _ in 0..<2 {
            ActionUtils.scrollAndTap("CustomerRegion-Customer4K*7", application: app, test: self)
            sleep(5)
            app.buttons["Close"].tap()
            ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
            sleep(5)
        }
        
    }
    
    func test_005_ClusterMap(){
        ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
        ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
        ActionUtils.scrollAndTap("_Performance Automation", application: app, test: self)
        ActionUtils.scrollAndTap("Widget", application: app, test: self)
        
        for _ in 0..<2 {
            ActionUtils.scrollAndTap("Map_Bubble_10000", application: app, test: self)
            sleep(10)
            app.buttons["Close"].tap()
            ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
            sleep(5)
        }
        
        for _ in 0..<2 {
            ActionUtils.scrollAndTap("Map_Density_10000", application: app, test: self)
            sleep(10)
            app.buttons["Close"].tap()
            ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
            sleep(5)
        }
        
    }
    
    func test_006_ClusterOFFMap(){
        ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
        ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
        ActionUtils.scrollAndTap("_Performance Automation", application: app, test: self)
        ActionUtils.scrollAndTap("Widget", application: app, test: self)
        
        for _ in 0..<2 {
            ActionUtils.scrollAndTap("Map_Bubble5000_ClusterOff_MultiLayer", application: app, test: self)
            sleep(10)
            app.buttons["Close"].tap()
            ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
            sleep(5)
        }
        
        for _ in 0..<2 {
            ActionUtils.scrollAndTap("ESRIMap_zipcode_5000", application: app, test: self)
            sleep(15)
            app.buttons["Close"].tap()
            ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
            sleep(5)
        }
        
    }
    
    func test_007_MicroChart(){
        ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
        ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
        ActionUtils.scrollAndTap("_Performance Automation", application: app, test: self)
        ActionUtils.scrollAndTap("Widget", application: app, test: self)
    

        
        for _ in 0..<2 {
            ActionUtils.scrollAndTap("MicroChart_Treemode", application: app, test: self)
            sleep(5)
            //ActionUtils.printDescription(app)
            app.buttons["Close"].tap()
            ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
            sleep(5)
        }
    
    }
    
    func test_008_MovieCalendar(){
        ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
        ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
        ActionUtils.scrollAndTap("_Performance Automation", application: app, test: self)
        ActionUtils.scrollAndTap("Widget", application: app, test: self)
        
        for _ in 0..<2 {
            ActionUtils.scrollAndTap("Movie Calendar_500KB Subcategory", application: app, test: self)
            app.buttons["Close"].tap()
            ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
            sleep(5)
        }
        
        
    }
    
/*    func test_009_MultiMedia(){
        ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
        ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
        ActionUtils.scrollAndTap("_Performance Automation", application: app, test: self)
        ActionUtils.scrollAndTap("Widget", application: app, test: self)
        
        for _ in 0..<2 {
            ActionUtils.scrollAndTap("MultiMedia", application: app, test: self)
            app.buttons["Close"].tap()
        }
        
        
    }*/
    
    func test_010_PhotoViwer(){
        ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
        ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
        ActionUtils.scrollAndTap("_Performance Automation", application: app, test: self)
        ActionUtils.scrollAndTap("Widget", application: app, test: self)
        
        for _ in 0..<2 {
            ActionUtils.scrollAndTap("PhotoViewer_Filmstrip_250images", application: app, test: self)
            sleep(10)
            app.buttons["Close"].tap()
            ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
            sleep(5)
        }
        
        ActionUtils.clearCache(app, test: self)
        
        for _ in 0..<2 {
            ActionUtils.scrollAndTap("PhotoViewer_Matrix_250images", application: app, test: self)
            sleep(10)
            app.buttons["Close"].tap()
            ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
            sleep(5)
        }
        ActionUtils.clearCache(app, test: self)
        
        for _ in 0..<2 {
            ActionUtils.scrollAndTap("PhotoViewer_Slide_250images", application: app, test: self)
            sleep(10)
            app.buttons["Close"].tap()
            ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
            sleep(5)
        }
        
        
    }
    
    func test_011_TimeSeries(){
        ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
        ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
        ActionUtils.scrollAndTap("_Performance Automation", application: app, test: self)
        ActionUtils.scrollAndTap("Widget", application: app, test: self)
        
        for _ in 0..<2 {
            ActionUtils.scrollAndTap("TimeSeries_500KB", application: app, test: self)
            
            sleep(5)
            app.buttons["Close"].tap()
            ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
            sleep(5)
        }
    }
    
    func test_012_Timeline(){
        
        ActionUtils.scrollAndTap("ILFC Metrics (Test)", application: app, test: self)
        ActionUtils.scrollAndTap("Profitability", application: app, test: self)
        ActionUtils.scrollAndTap("Load Test", application: app, test: self)
        
        for _ in 0..<2 {
            ActionUtils.scrollAndTap("Profitability Timeline", application: app, test: self)
            ActionUtils.verifyAndTapElement("This is a MSMTimeLineWidgetViewer Widget.", application: app, type: .other, test: self, tap: false)
            sleep(5)
            app.buttons["Close"].tap()
            ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
            sleep(5)
        }
        
    }
    
    func test_013_TimeBasedCalendar(){
        
        ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
        ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
        ActionUtils.scrollAndTap("_Performance Automation", application: app, test: self)
        ActionUtils.scrollAndTap("Widget", application: app, test: self)
        
        for _ in 0..<2 {
            ActionUtils.scrollAndTap("TimeBasedCalendar_2K", application: app, test: self)

            app.buttons["Close"].tap()
            ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
            sleep(5)
        }
        
    }
    
    func test_014_ESRIMap(){
        
        ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
        ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
        ActionUtils.scrollAndTap("_Performance Automation", application: app, test: self)
        ActionUtils.scrollAndTap("Widget", application: app, test: self)
        
        for _ in 0..<2 {
            ActionUtils.scrollAndTap("ESRIMap_RWD_Bubble_ClusterOn_10000", application: app, test: self)
            sleep(5)
            app.buttons["Close"].tap()
            ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
            sleep(5)
        }
        
    }
    
/*    func test_015_TreeWidget(){
        
        ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
        ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
        ActionUtils.scrollAndTap("_Performance Automation", application: app, test: self)
        ActionUtils.scrollAndTap("Widget", application: app, test: self)
        
        for _ in 0..<2 {
            ActionUtils.scrollAndTap("TreeWidget_1K", application: app, test: self)
            app.buttons["Close"].tap()
            ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
            sleep(5)
        }
        
    }*/
    
    func test_016_Network(){
        
        ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
        ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
        ActionUtils.scrollAndTap("_Performance Automation", application: app, test: self)
        ActionUtils.scrollAndTap("Widget", application: app, test: self)
        
        for _ in 0..<2 {
            ActionUtils.scrollAndTap("Network_Circle", application: app, test: self)
            //ActionUtils.printDescription(app)
            ActionUtils.verifyAndTapElement("This is a NetVisWidget Widget.", application: app, type: .other, test: self, tap: false)
            
            sleep(5)
            app.buttons["Close"].tap()
            ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
            sleep(5)
        }
        
    }
    
    func test_017_GraphMatrix(){
        
        ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
        ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
        ActionUtils.scrollAndTap("_Performance Automation", application: app, test: self)
        ActionUtils.scrollAndTap("Widget", application: app, test: self)
        
        for _ in 0..<2 {
            ActionUtils.scrollAndTap("GraphMatrix_72*36", application: app, test: self)
            ActionUtils.verifyAndTapElement("navi bar multi layout", application: app, type: .button, test: self, tap: false)
            
            sleep(5)
            app.buttons["Close"].tap()
            ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
            sleep(5)
        }
        
    }
    
    func test_018_GraphMatrixH5(){
        
        ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
        ActionUtils.scrollAndTap("_Lingcen", application: app, test: self)
        ActionUtils.scrollAndTap("Mobile VI", application: app, test: self)
        ActionUtils.scrollAndTap("GraphMatrix", application: app, test: self)
        ActionUtils.scrollAndTap("HTML5 Graph Matrix", application: app, test: self)
        
        
        for _ in 0..<2 {
            ActionUtils.scrollAndTap("Bubble_49762", application: app, test: self)
            ActionUtils.verifyAndTapElement("VIFilter", application: app, type: .button, test: self, tap: false)
            
            sleep(25)
            app.buttons["Close"].tap()
            ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
            sleep(5)
        }
        
        for _ in 0..<2 {
            ActionUtils.scrollAndTap("Graph Matrix_ABL_bar_longchartNone_partial rendering", application: app, test: self)
            ActionUtils.verifyAndTapElement("VIFilter", application: app, type: .button, test: self, tap: false)
            
            sleep(10)
            app.buttons["Close"].tap()
            ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
            sleep(5)
        }
        
    }
    
    func test_019_GraphMatrixColorBy(){
        
        ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
        ActionUtils.scrollAndTap("_Lingcen", application: app, test: self)
        ActionUtils.scrollAndTap("Mobile VI", application: app, test: self)
        ActionUtils.scrollAndTap("GraphMatrix", application: app, test: self)
        ActionUtils.scrollAndTap("Partial Rendering", application: app, test: self)
        
        for _ in 0..<2 {
            ActionUtils.scrollAndTap("Graph Matrix_ABL_bar_longchartDay_partial rendering_color by", application: app, test: self)
            
            sleep(25)
            app.buttons["Close"].tap()
            ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
            sleep(5)
        }
        
        for _ in 0..<2 {
            ActionUtils.scrollAndTap("Graph Matrix_ABL_bar_longchartNone_partial rendering_color by", application: app, test: self)
            
            sleep(15)
            app.buttons["Close"].tap()
            ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
            sleep(5)
        }
        
    }
    
    func test_020_GraphMatrixPR(){
        ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
        ActionUtils.scrollAndTap("_Lingcen", application: app, test: self)
        ActionUtils.scrollAndTap("Mobile VI", application: app, test: self)
        ActionUtils.scrollAndTap("GraphMatrix", application: app, test: self)
        ActionUtils.scrollAndTap("Partial Rendering", application: app, test: self)
        
        for _ in 0..<2 {
            ActionUtils.scrollAndTap("Graph Matrix_ABL_area_longchartMonth_partial rendering", application: app, test: self)
            
            sleep(15)
            app.buttons["Close"].tap()
            ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
            sleep(5)
        }
        
        ActionUtils.scrollAndTap("Grid", application: app, test: self)
        
        for _ in 0..<2 {
            ActionUtils.scrollAndTap("Graph Matrix_Grid_longchartNone_partial rendering_horizontal", application: app, test: self)
            
            sleep(15)
            app.buttons["Close"].tap()
            ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
            sleep(5)
        }
        

    }
    
    func test_021_GraphMatrixGrid() {
        ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
        ActionUtils.scrollAndTap("_Lingcen", application: app, test: self)
        ActionUtils.scrollAndTap("Mobile VI", application: app, test: self)
        ActionUtils.scrollAndTap("GraphMatrix", application: app, test: self)
        ActionUtils.scrollAndTap("Partial Rendering", application: app, test: self)
        ActionUtils.scrollAndTap("Grid", application: app, test: self)
        
        for _ in 0..<2 {
            ActionUtils.scrollAndTap("2D_horizontal273_vertical336", application: app, test: self)
            
            sleep(15)
            app.buttons["Close"].tap()
            ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
            sleep(5)
        }
    }
    
    func test_022_GraphMatrixOther(){
        ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
        ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
        ActionUtils.scrollAndTap("_Performance Automation", application: app, test: self)
        ActionUtils.scrollAndTap("Widget", application: app, test: self)
        
        for _ in 0..<2 {
            ActionUtils.scrollAndTap("GraphMatrix_Product_Bar_Smaller", application: app, test: self)
            
            sleep(10)
            app.buttons["Close"].tap()
            ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
            sleep(5)
        }
        
        
        for _ in 0..<2 {
            ActionUtils.scrollAndTap("Graph Matrix_pie bubble/scatter_7M", application: app, test: self)
            
            sleep(15)
            app.buttons["Close"].tap()
            ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
            sleep(5)
        }
        
    }
    
    
    func test_023_MicroChartAsSelector() {
        // Use recording to get started writing UI tests.
        // Use XCTAssert and related functions to verify your tests produce the correct results.

        ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
        ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
        ActionUtils.scrollAndTap("_Performance Automation", application: app, test: self)
        ActionUtils.scrollAndTap("Widget", application: app, test: self)
        
        for i in 0..<2 {
            ActionUtils.scrollAndTap("MicroChart_Treemode_AsSelector_New", application: app, test: self)
            sleep(3)
            
            ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
            sleep(5)

            if (i == 1 && ENABLE_MANI){
                app.tables.staticTexts["[Sparkline]  Business"].tap()
                sleep(5)
                ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
                sleep(5)
            }
            
            app.buttons["Close"].tap()
        }
        
    }
    
    func test_024_PhotoViewerAsSelector() {
        // Use recording to get started writing UI tests.
        // Use XCTAssert and related functions to verify your tests produce the correct results.
        ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
        ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
        ActionUtils.scrollAndTap("_Performance Automation", application: app, test: self)
        ActionUtils.scrollAndTap("Widget", application: app, test: self)
        
        for i in 0..<2 {
            ActionUtils.scrollAndTap("PhotoViewer_Filmstrip_250images_AsSelector", application: app, test: self)
            sleep(3)
            
            ActionUtils.verifyAndTapElement("This is a ImageViewerWidget Widget.", application: app, type: .other, test: self, tap: false)
            ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
            sleep(5)
            if (i == 1 && ENABLE_MANI){
                
                app.buttons.matching(identifier: "").element(boundBy: 0).tap()
                ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
                sleep(5)
            }
            
            app.buttons["Close"].tap()
        }
        
    }
    
    func test_025_HeatmapAsSelector() {
        // Use recording to get started writing UI tests.
        // Use XCTAssert and related functions to verify your tests produce the correct results.
        ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
        ActionUtils.scrollAndTap("_Lingcen", application: app, test: self)
        ActionUtils.scrollAndTap("Mobile VI", application: app, test: self)
        
        for _ in 0..<2 {
            ActionUtils.scrollAndTap("Heat Map_different size_target selector", application: app, test: self)
            ActionUtils.verifyAndTapElement("VIFilter", application: app, type: .button, test: self, tap: false)

         /*   if (i == 1 && ENABLE_MANI){
                app.scrollViews.containingType(.StaticText, identifier: "Northeast").elementBoundByIndex(0).tap()
                sleep(3)
            }*/
            
            app.buttons["Close"].tap()
            ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
            sleep(5)
        }
        
    }
    
    func test_026_ChangeLayout() {
        // Use recording to get started writing UI tests.
        // Use XCTAssert and related functions to verify your tests produce the correct results.
        ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
        ActionUtils.scrollAndTap("_Lingcen", application: app, test: self)
        ActionUtils.scrollAndTap("Mobile VI", application: app, test: self)
        
        for _ in 0..<2 {
            ActionUtils.scrollAndTap("layout4", application: app, test: self)
            ActionUtils.verifyAndTapElement("VIFilter", application: app, type: .button, test: self, tap: false)
           // ActionUtils.printDescription(app)
         /*   if (i == 1 && ENABLE_MANI){
                
                app.buttons["navi bar layout arrow right"].tap()
                ActionUtils.verifyAndTapElement("VIFilter", application: app, type: .Button, test: self, tap: false)
                app.buttons["navi bar layout arrow right"].tap()
                ActionUtils.verifyAndTapElement("VIFilter", application: app, type: .Button, test: self, tap: false)
                app.buttons["Jan 2009"].tap()
                sleep(5)
            }*/
            
            app.buttons["Close"].tap()
            ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
            sleep(5)
        }
        
    }
    
    func test_027_ChangePanel() {
        // Use recording to get started writing UI tests.
        // Use XCTAssert and related functions to verify your tests produce the correct results.
        ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
        ActionUtils.scrollAndTap("_Lingcen", application: app, test: self)
        ActionUtils.scrollAndTap("Mobile VI", application: app, test: self)
        
        for _ in 0..<2 {
            ActionUtils.scrollAndTap("panel3", application: app, test: self)
            ActionUtils.verifyAndTapElement("VIFilter", application: app, type: .button, test: self, tap: false)
      /*      if (i == 1 && ENABLE_MANI){
                
                app.textFields["Heat Map"].swipeLeft()
                ActionUtils.verifyAndTapElement("VIFilter", application: app, type: .Button, test: self, tap: true)
                sleep(2)
                app.staticTexts["Quarter"].tap()
                app.tables.staticTexts["2007 Q2"].tap()
                sleep(7)
                app.buttons["VIFilter"].tap()
                sleep(3)
                app.scrollViews.otherElements[", Quarter, Row Headings"].pressForDuration(LONG_PRESS)
                ActionUtils.verifyAndTapElement("Drill Anywhere...", application: app, type: .MenuItem, test: self, tap: true)
                app.staticTexts["Country"].tap()
                app.buttons["Update"].tap()
                
            }*/
            
            app.buttons["Close"].tap()
            ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
            sleep(5)
        }
        
    }
    
    func test_028_MMSort(){
        ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
        ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
        ActionUtils.scrollAndTap("_Performance Automation", application: app, test: self)
        ActionUtils.scrollAndTap("Widget", application: app, test: self)
        

        for _ in 0..<2 {
            ActionUtils.scrollAndTap("MultiMedia_Vertical", application: app, test: self)
            ActionUtils.verifyAndTapElement("mediaSortButton", application: app, type: .button, test: self, tap: false)
         //   ActionUtils.printDescription(app)
         //   ActionUtils.verifyAndTapElement("VIFilter", application: app, type: .Button, test: self, tap: false)
            
            app.buttons["Close"].tap()
            ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
            sleep(5)
        }
    }
    
}
