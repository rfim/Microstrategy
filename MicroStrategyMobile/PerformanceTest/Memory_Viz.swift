//
//  Memory_Viz.swift
//  MicroStrategyMobile
//
//  Created by Administrator on 11/5/15.
//  Copyright © 2015 MicroStrategy Inc. All rights reserved.
//

import XCTest

class Memory_Viz: XCTestCase {
    
    let app = XCUIApplication()
    
    var count = LEAKMODE ? 10 : 1
        
    override func setUp() {
        super.setUp()
        
        // Put setup code here. This method is called before the invocation of each test method in the class.
        
        // In UI tests it is usually best to stop immediately when a failure occurs.
        continueAfterFailure = false
        // UI tests must launch the application that they test. Doing this in setup will make sure it happens for each test method.
        app.launch()

        // In UI tests it’s important to set the initial state - such as interface orientation - required for your tests before they run. The setUp method is a good place to do this.
    }
    
    override func tearDown() {
        // Put teardown code here. This method is called after the invocation of each test method in the class.
        app.terminate()
        super.tearDown()
    }
    
    func test_001_Graph() {
        // Use recording to get started writing UI tests.
        // Use XCTAssert and related functions to verify your tests produce the correct results.
        
        //area_shake_depth
        ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
        ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
        ActionUtils.scrollAndTap("_Memory Governor", application: app, test: self)
        ActionUtils.scrollAndTap("Grid&Graph", application: app, test: self)
        
        for _ in 0 ..< count{
        
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
            ActionUtils.scrollAndTap("area_shake_depth", application: app, test: self)
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
            sleep(5)
            
            app.buttons["Close"].tap()
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
            sleep(5)
            _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
            
        }
        sleep(5)
        ActionUtils.terminateAndRelaunch(app)
        
        
        //bar_area_line_bipolar_curve_depth
        if (app.tables.staticTexts["MicroStrategy Tutorial"].exists){
            ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
            ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
            ActionUtils.scrollAndTap("_Memory Governor", application: app, test: self)
            ActionUtils.scrollAndTap("Grid&Graph", application: app, test: self)
        }
        print(count)
        
        for _ in 0 ..< count{
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
            ActionUtils.scrollAndTap("bar_area_line_bipolar_curve_depth", application: app, test: self)
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
            sleep(5)
            
            app.buttons["Close"].tap()
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
            sleep(5)
            _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
        }

        sleep(5)
        ActionUtils.terminateAndRelaunch(app)
        
        
        //bar_area_line_dual_step_depth
        if (app.tables.staticTexts["MicroStrategy Tutorial"].exists){
            ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
            ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
            ActionUtils.scrollAndTap("_Memory Governor", application: app, test: self)
            ActionUtils.scrollAndTap("Grid&Graph", application: app, test: self)
        }
        
        for _ in 0 ..< count{
            sleep(5)
            
            ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
        
            ActionUtils.scrollAndTap("bar_area_line_dual_step_depth", application: app, test: self)
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
            sleep(5)
            
            app.buttons["Close"].tap()
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
            sleep(5)
            _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
        }
        sleep(5)
        ActionUtils.terminateAndRelaunch(app)
        
        
        //dual_axis_step_line
        if (app.tables.staticTexts["MicroStrategy Tutorial"].exists){
            ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
            ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
            ActionUtils.scrollAndTap("_Memory Governor", application: app, test: self)
            ActionUtils.scrollAndTap("Grid&Graph", application: app, test: self)
        }
        
        for _ in 0 ..< count{
       
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
            ActionUtils.scrollAndTap("dual_axis_step_line", application: app, test: self)
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
            sleep(5)
            
            app.buttons["Close"].tap()
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
            sleep(5)
            _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
        }
        sleep(5)
        ActionUtils.terminateAndRelaunch(app)
        
        
        //series_line
        if (app.tables.staticTexts["MicroStrategy Tutorial"].exists){
            ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
            ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
            ActionUtils.scrollAndTap("_Memory Governor", application: app, test: self)
            ActionUtils.scrollAndTap("Grid&Graph", application: app, test: self)
        }
        
        for _ in 0 ..< count{
            
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
            ActionUtils.scrollAndTap("series_line", application: app, test: self)
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
            sleep(5)
            
            app.buttons["Close"].tap()
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
            sleep(5)
            _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
        }
        sleep(5)
        
        ActionUtils.terminateAndRelaunch(app)
        
        
        //Graph_Area, Line, Bar_ 1000*3
        if (app.tables.staticTexts["MicroStrategy Tutorial"].exists){
            ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
            ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
            ActionUtils.scrollAndTap("_Memory Governor", application: app, test: self)
            ActionUtils.scrollAndTap("Grid&Graph", application: app, test: self)
        }
        
        for _ in 0 ..< count{
            
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
            ActionUtils.scrollAndTap("Graph_Area, Line, Bar_ 1000*3", application: app, test: self)
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
            sleep(5)
            
            app.buttons["Close"].tap()
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
            sleep(5)
            _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
        }

        sleep(5)
        
    }
    
    func test_002_ImageRelated(){
        
        //Filmstrip_250images
        
        for i in 0 ..< 2{
            if(app.tables.staticTexts["MicroStrategy Tutorial"].exists){
                ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
                ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
                ActionUtils.scrollAndTap("_Memory Governor", application: app, test: self)
                ActionUtils.scrollAndTap("Image&Text", application: app, test: self)
                ActionUtils.scrollAndTap("Photo Viewer", application: app, test: self)
            }
            if ((i == 0) && !LEAKMODE){
        
                ActionUtils.clearCache(app, test: self)
            }
            
            for _ in 0 ..< count{
        
                sleep(5)
                ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
                ActionUtils.scrollAndTap("Filmstrip_250images", application: app, test: self)
                sleep(5)
                ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
                sleep(5)
                
                app.buttons["Close"].tap()
                sleep(5)
                ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
                sleep(5)
                _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
            }
            sleep(5)
            ActionUtils.terminateAndRelaunch(app)
            
            if (LEAKMODE){
                break
            }
        
        }
        
        
        //Matrix_250images
        for i in 0 ..< 2{
            if(app.tables.staticTexts["MicroStrategy Tutorial"].exists){
                ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
                ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
                ActionUtils.scrollAndTap("_Memory Governor", application: app, test: self)
                ActionUtils.scrollAndTap("Image&Text", application: app, test: self)
                ActionUtils.scrollAndTap("Photo Viewer", application: app, test: self)
            }
            if ((i == 0) && !LEAKMODE){
                
                ActionUtils.clearCache(app, test: self)
            }
            
            for _ in 0 ..< count{
                
                sleep(5)
                ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
                ActionUtils.scrollAndTap("Matrix_250images", application: app, test: self)
                sleep(5)
                ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
                sleep(5)
                
                app.buttons["Close"].tap()
                sleep(5)
                ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
                sleep(5)
                _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
            }
            sleep(5)
            ActionUtils.terminateAndRelaunch(app)
            
            if (LEAKMODE){
                break
            }
            
        }
        //Slide_250images
        
        for i in 0 ..< 2{
            if(app.tables.staticTexts["MicroStrategy Tutorial"].exists){
                ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
                ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
                ActionUtils.scrollAndTap("_Memory Governor", application: app, test: self)
                ActionUtils.scrollAndTap("Image&Text", application: app, test: self)
                ActionUtils.scrollAndTap("Photo Viewer", application: app, test: self)
            }
            if ((i == 0) && !LEAKMODE){
                
                ActionUtils.clearCache(app, test: self)
            }
            
            for _ in 0 ..< count{
                
                sleep(5)
                ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
                ActionUtils.scrollAndTap("Slide_250images", application: app, test: self)
                sleep(5)
                ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
                sleep(5)
                
                app.buttons["Close"].tap()
                sleep(5)
                ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
                sleep(5)
                _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
            }
            sleep(5)
            ActionUtils.terminateAndRelaunch(app)
            
            if (LEAKMODE){
                break
            }
            
        }
        //Customer_2K
        
        for i in 0 ..< 2{
            if(app.tables.staticTexts["MicroStrategy Tutorial"].exists){
                ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
                ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
                ActionUtils.scrollAndTap("_Memory Governor", application: app, test: self)
                ActionUtils.scrollAndTap("Image&Text", application: app, test: self)
                ActionUtils.scrollAndTap("Image Layout", application: app, test: self)
            }
            if ((i == 0) && !LEAKMODE){
                
                ActionUtils.clearCache(app, test: self)
            }
            
            for _ in 0 ..< count{
                
                sleep(5)
                ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
                ActionUtils.scrollAndTap("Customer_2K", application: app, test: self)
                sleep(5)
                ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
                sleep(5)
                
                app.buttons["Close"].tap()
                sleep(5)
                ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
                sleep(5)
                _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
            }
            sleep(5)
            ActionUtils.terminateAndRelaunch(app)
            
            if (LEAKMODE){
                break
            }
            
        }
        
        
        //CustomerRegion-Customer2K*7
        
        for i in 0 ..< 2{
            if(app.tables.staticTexts["MicroStrategy Tutorial"].exists){
                ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
                ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
                ActionUtils.scrollAndTap("_Memory Governor", application: app, test: self)
                ActionUtils.scrollAndTap("Image&Text", application: app, test: self)
                ActionUtils.scrollAndTap("Image Layout", application: app, test: self)
                ActionUtils.scrollAndTap("MultiImageLayout", application: app, test: self)
            }
            if ((i == 0) && !LEAKMODE){
                
                ActionUtils.clearCache(app, test: self)
            }
            
            for _ in 0 ..< count{
                
                sleep(5)
                ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
                ActionUtils.scrollAndTap("CustomerRegion-Customer2K*7", application: app, test: self)
                sleep(5)
                ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
                sleep(5)
                
                app.buttons["Close"].tap()
                sleep(5)
                ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
                sleep(5)
                _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
            }
            sleep(5)
            ActionUtils.terminateAndRelaunch(app)
            
            if (LEAKMODE){
                break
            }
            
        }
        
        
    }
    
    func test_003_RWDVizHeatmap(){
        
        //Heatmap_Label
        ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
        ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
        ActionUtils.scrollAndTap("_Memory Automation", application: app, test: self)
        ActionUtils.scrollAndTap("Widget", application: app, test: self)
        
        for _ in 0 ..< count{
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
            ActionUtils.scrollAndTap("Heatmap_Label", application: app, test: self)
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
            sleep(5)
            
            app.buttons["Close"].tap()
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
            sleep(5)
            _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
        }
        sleep(5)
        ActionUtils.terminateAndRelaunch(app)
        
        
        //Heatmap_noLabel
        if(app.tables.staticTexts["MicroStrategy Tutorial"].exists){
            ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
            ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
            ActionUtils.scrollAndTap("_Memory Automation", application: app, test: self)
            ActionUtils.scrollAndTap("Widget", application: app, test: self)
        }
        
        if (!LEAKMODE){
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
            ActionUtils.scrollAndTap("Heatmap_noLabel", application: app, test: self)
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
            sleep(5)
            
            app.buttons["Close"].tap()
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
            sleep(5)
            _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
        }
        sleep(5)
        ActionUtils.terminateAndRelaunch(app)
    }
    
    func test_004_Map(){
    
        //Map_Bubble_5000
        if(app.tables.staticTexts["MicroStrategy Tutorial"].exists){
            ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
            ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
            ActionUtils.scrollAndTap("_Memory Automation", application: app, test: self)
            ActionUtils.scrollAndTap("Widget", application: app, test: self)
        }
        
        if (!LEAKMODE){
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
            ActionUtils.scrollAndTap("Map_Bubble_5000", application: app, test: self)
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
            sleep(5)
            
            app.buttons["Close"].tap()
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
            sleep(5)
            _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
        }
        sleep(5)
        ActionUtils.terminateAndRelaunch(app)
        
        
        //Map_Bubble_10000
        if(app.tables.staticTexts["MicroStrategy Tutorial"].exists){
            ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
            ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
            ActionUtils.scrollAndTap("_Memory Automation", application: app, test: self)
            ActionUtils.scrollAndTap("Widget", application: app, test: self)
        }
        
        for _ in 0 ..< count{
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
            ActionUtils.scrollAndTap("Map_Bubble_10000", application: app, test: self)
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
            sleep(5)
            
            app.buttons["Close"].tap()
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
            sleep(5)
            _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
        }
        sleep(5)
        ActionUtils.terminateAndRelaunch(app)
        
        
        //Map_Density_5000
        if(app.tables.staticTexts["MicroStrategy Tutorial"].exists){
            ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
            ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
            ActionUtils.scrollAndTap("_Memory Automation", application: app, test: self)
            ActionUtils.scrollAndTap("Widget", application: app, test: self)
        }
        
        if (!LEAKMODE){
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
            ActionUtils.scrollAndTap("Map_Density_5000", application: app, test: self)
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
            sleep(5)
            
            app.buttons["Close"].tap()
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
            sleep(5)
            _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
        }
        sleep(5)
        ActionUtils.terminateAndRelaunch(app)
        
        
        //Map_Density_10000
        if(app.tables.staticTexts["MicroStrategy Tutorial"].exists){
            ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
            ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
            ActionUtils.scrollAndTap("_Memory Automation", application: app, test: self)
            ActionUtils.scrollAndTap("Widget", application: app, test: self)
        }
        
        for _ in 0 ..< count{
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
            ActionUtils.scrollAndTap("Map_Density_10000", application: app, test: self)
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
            sleep(5)
            
            app.buttons["Close"].tap()
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
            sleep(5)
            _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
        }
        sleep(5)
        ActionUtils.terminateAndRelaunch(app)
        
        
        //Area map
        if(app.tables.staticTexts["MicroStrategy Tutorial"].exists){
            ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
            ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
            ActionUtils.scrollAndTap("_Performance Automation", application: app, test: self)
            ActionUtils.scrollAndTap("Widget", application: app, test: self)
        }
        
        for _ in 0 ..< count{
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
            ActionUtils.scrollAndTap("ESRIMap_zipcode_5000", application: app, test: self)
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
            sleep(5)
            
            app.buttons["Close"].tap()
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
            sleep(5)
            _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
        }
        sleep(5)
        ActionUtils.terminateAndRelaunch(app)
        
        //ESRI Map
        if(app.tables.staticTexts["MicroStrategy Tutorial"].exists){
            ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
            ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
            ActionUtils.scrollAndTap("_Performance Automation", application: app, test: self)
            ActionUtils.scrollAndTap("Widget", application: app, test: self)
        }
        
        for _ in 0 ..< count{
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
            ActionUtils.scrollAndTap("ESRIMap_RWD_Bubble_ClusterOn_10000", application: app, test: self)
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
            sleep(5)
            
            app.buttons["Close"].tap()
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
            sleep(5)
            _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
        }
        sleep(5)
        ActionUtils.terminateAndRelaunch(app)
        
        
    }
    func test_005_MicroChart(){
    
        //MicroChart_NoTreemode
        if(app.tables.staticTexts["MicroStrategy Tutorial"].exists){
            ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
            ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
            ActionUtils.scrollAndTap("_Memory Automation", application: app, test: self)
            ActionUtils.scrollAndTap("Widget", application: app, test: self)
        }
        
/*        if (!LEAKMODE){
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
            ActionUtils.scrollAndTap("MicroChart_NoTreemode", application: app, test: self)
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
            sleep(5)
            
            app.buttons["Close"].tap()
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
            sleep(5)
            _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
        }
        sleep(5)
        ActionUtils.terminateAndRelaunch(app)
        */
        
        //MicroChart_Treemode
        if(app.tables.staticTexts["MicroStrategy Tutorial"].exists){
            ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
            ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
            ActionUtils.scrollAndTap("_Memory Automation", application: app, test: self)
            ActionUtils.scrollAndTap("Widget", application: app, test: self)
        }
        
        for _ in 0 ..< count{
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
            ActionUtils.scrollAndTap("MicroChart_Treemode", application: app, test: self)
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
            sleep(5)
            
            app.buttons["Close"].tap()
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
            sleep(5)
            _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
        }
        sleep(5)
        ActionUtils.terminateAndRelaunch(app)
    }
    
    func test_006_RWDVizCalendar(){
    
        
    //Movie Calendar_3.7MB Brand
        if(app.tables.staticTexts["MicroStrategy Tutorial"].exists){
            ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
            ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
            ActionUtils.scrollAndTap("_Memory Automation", application: app, test: self)
            ActionUtils.scrollAndTap("Widget", application: app, test: self)
        }
        
        for _ in 0 ..< count{
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
            ActionUtils.scrollAndTap("Movie Calendar_3.7MB Brand", application: app, test: self)
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
            sleep(5)
            
            app.buttons["Close"].tap()
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
            sleep(5)
            _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
        }
        sleep(5)
        ActionUtils.terminateAndRelaunch(app)
        
        
        //Movie Calendar_500KB Subcategory
        if(app.tables.staticTexts["MicroStrategy Tutorial"].exists){
            ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
            ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
            ActionUtils.scrollAndTap("_Memory Automation", application: app, test: self)
            ActionUtils.scrollAndTap("Widget", application: app, test: self)
        }
        
        if (!LEAKMODE){
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
            ActionUtils.scrollAndTap("Movie Calendar_500KB Subcategory", application: app, test: self)
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
            sleep(5)
            
            app.buttons["Close"].tap()
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
            sleep(5)
            _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
        }
        sleep(5)
        ActionUtils.terminateAndRelaunch(app)
    }
    
    func test_007_GM(){
    
        //GraphMatrix_Item
        if(app.tables.staticTexts["MicroStrategy Tutorial"].exists){
            ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
            ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
            ActionUtils.scrollAndTap("_Memory Automation", application: app, test: self)
            ActionUtils.scrollAndTap("Widget", application: app, test: self)
        }
        
        for _ in 0 ..< count{
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
            ActionUtils.scrollAndTap("GraphMatrix_Item", application: app, test: self)
            sleep(20)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
            sleep(5)
            
            app.buttons["Close"].tap()
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
            sleep(5)
            _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
        }
        sleep(5)
        ActionUtils.terminateAndRelaunch(app)
        
    }
    func test_008_TSTL(){
        //TimeSeries_500KB
        if(app.tables.staticTexts["MicroStrategy Tutorial"].exists){
            ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
            ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
            ActionUtils.scrollAndTap("_Performance Automation", application: app, test: self)
            ActionUtils.scrollAndTap("Widget", application: app, test: self)
        }
        
        for _ in 0 ..< count{
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
            ActionUtils.scrollAndTap("TimeSeries_500KB", application: app, test: self)
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
            sleep(5)
            
            app.buttons["Close"].tap()
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
            sleep(5)
            _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
        }
        sleep(5)
        ActionUtils.terminateAndRelaunch(app)
        

        
        //Profitability Timeline_Noinfo
        if(app.tables.staticTexts["ILFC Metrics (Test)"].exists){
            ActionUtils.scrollAndTap("ILFC Metrics (Test)", application: app, test: self)
            ActionUtils.scrollAndTap("Profitability", application: app, test: self)
            ActionUtils.scrollAndTap("Load Test", application: app, test: self)

        }else{
            app.buttons["Home"].tap()
            ActionUtils.scrollAndTap("ILFC Metrics (Test)", application: app, test: self)
            ActionUtils.scrollAndTap("Profitability", application: app, test: self)
            ActionUtils.scrollAndTap("Load Test", application: app, test: self)
        }
        
        for _ in 0 ..< count{
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
            ActionUtils.scrollAndTap("Profitability Timeline_Noinfo", application: app, test: self)
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
            sleep(5)
            
            app.buttons["Close"].tap()
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
            sleep(5)
            _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
        }
        sleep(5)
        
        
    }
    
    func test_009_VIGM(){
        
        //GraphMatrix_24*36
        ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
        ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
        ActionUtils.scrollAndTap("_Memory Automation", application: app, test: self)
        ActionUtils.scrollAndTap("Widget", application: app, test: self)
        
        if(!LEAKMODE){
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
            ActionUtils.scrollAndTap("GraphMatrix_24*36", application: app, test: self)
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
            sleep(5)
            
            app.buttons["Close"].tap()
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
            sleep(5)
            _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
        }
        sleep(5)
        ActionUtils.terminateAndRelaunch(app)
        
        
        //GraphMatrix_24*36
        ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
        ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
        ActionUtils.scrollAndTap("_Memory Automation", application: app, test: self)
        ActionUtils.scrollAndTap("Widget", application: app, test: self)
        
        if(!LEAKMODE){
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
            ActionUtils.scrollAndTap("GraphMatrix_72*36", application: app, test: self)
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
            sleep(5)
            _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
            
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
            app.buttons["navi bar layout arrow right"].tap()
            
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
            sleep(5)
            _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
            
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
            app.buttons["navi bar layout arrow right"].tap()
            
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
            sleep(5)
            
            app.buttons["Close"].tap()
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
            sleep(5)
            _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
        }
        sleep(5)
        ActionUtils.terminateAndRelaunch(app)
        
        
        //Graph Matrix_grid_L_single screen
        if(app.tables.staticTexts["MicroStrategy Tutorial"].exists){
            ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
            ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
            ActionUtils.scrollAndTap("_Memory Automation", application: app, test: self)
            ActionUtils.scrollAndTap("Widget", application: app, test: self)
        }
        
        for _ in 0 ..< count{
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
            ActionUtils.scrollAndTap("Graph Matrix_grid_L_single screen", application: app, test: self)
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
            sleep(5)
            
            app.buttons["Close"].tap()
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
            sleep(5)
            _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
        }
        sleep(5)
        ActionUtils.terminateAndRelaunch(app)
        
        
        //GraphMatrix_Product_Bar
        if(app.tables.staticTexts["MicroStrategy Tutorial"].exists){
            ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
            ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
            ActionUtils.scrollAndTap("_Memory Automation", application: app, test: self)
            ActionUtils.scrollAndTap("Widget", application: app, test: self)
        }
        
        for _ in 0 ..< count{
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
            ActionUtils.scrollAndTap("GraphMatrix_Product_Bar_Smaller", application: app, test: self)
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
            sleep(5)
            
            app.buttons["Close"].tap()
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
            sleep(5)
            _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
        }
        sleep(5)
        ActionUtils.terminateAndRelaunch(app)
        
        
        //Graph Matrix_pie bubble/scatter_7M
        if(app.tables.staticTexts["MicroStrategy Tutorial"].exists){
            ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
            ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
            ActionUtils.scrollAndTap("_Memory Automation", application: app, test: self)
            ActionUtils.scrollAndTap("Widget", application: app, test: self)
        }
        
        for _ in 0 ..< count{
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
            ActionUtils.scrollAndTap("Graph Matrix_pie bubble/scatter_7M", application: app, test: self)
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
            sleep(5)
            
            app.buttons["Close"].tap()
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
            sleep(5)
            _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
        }
        sleep(5)
        ActionUtils.terminateAndRelaunch(app)
        
        
        //layout4
        if(app.tables.staticTexts["MicroStrategy Tutorial"].exists){
            ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
            ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
            ActionUtils.scrollAndTap("_Memory Automation", application: app, test: self)
            ActionUtils.scrollAndTap("Widget", application: app, test: self)
        }
        
        for _ in 0 ..< count{
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
            ActionUtils.scrollAndTap("layout4", application: app, test: self)
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
            sleep(5)
            
            app.buttons["Close"].tap()
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
            sleep(5)
            _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
        }
        sleep(5)
        ActionUtils.terminateAndRelaunch(app)
        
        
        //panel3
        if(app.tables.staticTexts["MicroStrategy Tutorial"].exists){
            ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
            ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
            ActionUtils.scrollAndTap("_Memory Automation", application: app, test: self)
            ActionUtils.scrollAndTap("Widget", application: app, test: self)
        }
        
        for _ in 0 ..< count{
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
            ActionUtils.scrollAndTap("panel3", application: app, test: self)
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
            sleep(5)
            
            app.buttons["Close"].tap()
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
            sleep(5)
            _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
        }
        sleep(5)
        ActionUtils.terminateAndRelaunch(app)
    }
    func test_010_Network(){
        //Network_Circle
        if(app.tables.staticTexts["MicroStrategy Tutorial"].exists){
            ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
            ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
            ActionUtils.scrollAndTap("_Memory Automation", application: app, test: self)
            ActionUtils.scrollAndTap("Widget", application: app, test: self)
        }
        
        for _ in 0 ..< count{
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
            ActionUtils.scrollAndTap("Network_Circle", application: app, test: self)
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
            sleep(5)
            
            app.buttons["Close"].tap()
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
            sleep(5)
            _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
        }
        sleep(5)
        ActionUtils.terminateAndRelaunch(app)
        
        //Network_Line
        if(app.tables.staticTexts["MicroStrategy Tutorial"].exists){
            ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
            ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
            ActionUtils.scrollAndTap("_Memory Automation", application: app, test: self)
            ActionUtils.scrollAndTap("Widget", application: app, test: self)
        }
        
        for _ in 0 ..< count{
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
            ActionUtils.scrollAndTap("Network_Line", application: app, test: self)
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
            sleep(5)
            
            app.buttons["Close"].tap()
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
            sleep(5)
            _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
        }
        sleep(5)
        ActionUtils.terminateAndRelaunch(app)
    }
    
    func test_011_Heatmap(){
    
    
        //Heat Map_different size_target selector
        if(app.tables.staticTexts["MicroStrategy Tutorial"].exists){
            ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
            ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
            ActionUtils.scrollAndTap("_Memory Automation", application: app, test: self)
            ActionUtils.scrollAndTap("Widget", application: app, test: self)
        }
        
        
        for _ in 0 ..< count{
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
            ActionUtils.scrollAndTap("Heat Map_different size_target selector", application: app, test: self)
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
            sleep(5)
            _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
            
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
            app.scrollViews.containing(.staticText, identifier: "Northeast").element(boundBy: 0).tap()
            //app.scrollViews.containingType(.StaticText, identifier: "Northeast").elementBoundByIndex(0).tap()
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
            
            app.buttons["Close"].tap()
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
            sleep(5)
            _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
        }
        
        sleep(5)
        
    }

    
}
