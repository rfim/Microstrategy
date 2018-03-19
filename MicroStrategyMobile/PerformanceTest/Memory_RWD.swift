//
//  Memory_RWD.swift
//  MicroStrategyMobile
//
//  Created by Administrator on 11/5/15.
//  Copyright © 2015 MicroStrategy Inc. All rights reserved.
//

import XCTest

var LEAKMODE: Bool = false


class Memory_RWD: XCTestCase {
    
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
    
    func test_000_ChangeLogLevel(){
        ActionUtils.changeLogLevel("Messages", to: "All", app: app, test: self)
    }
    
    func test_001_GridAndPanel() {
      
        
        //15,000 Cells_Formatting
        ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
        ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
        ActionUtils.scrollAndTap("_Memory Governor", application: app, test: self)
        
        ActionUtils.scrollAndTap("Grid&Graph", application: app, test: self)
        
        if (!LEAKMODE){
            ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
            ActionUtils.scrollAndTap("15,000 Cells_Formatting", application: app, test: self)
           // ActionUtils.printDescription(app)
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
            app.buttons["Close"].tap()
            ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
            
            sleep(5)
            
            _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
            
            sleep(5)
            
            ActionUtils.terminateAndRelaunch(app)
        }
        
        
        
        //90,000 Cells_Formatting
        if (app.tables.staticTexts["MicroStrategy Tutorial"].exists){
            ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
            ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
            ActionUtils.scrollAndTap("_Memory Governor", application: app, test: self)
            
            ActionUtils.scrollAndTap("Grid&Graph", application: app, test: self)
        }
        
        for _ in 0 ..< count{
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
            ActionUtils.scrollAndTap("90,000 Cells_Formatting", application: self.app, test: self)
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
            app.buttons["Close"].tap()
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
            sleep(5)
            _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
            
        }
        sleep(5)
        ActionUtils.terminateAndRelaunch(app)
        
        
        //2 x 15,000 Grid
        if (app.tables.staticTexts["MicroStrategy Tutorial"].exists){
            ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
            ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
            ActionUtils.scrollAndTap("_Memory Governor", application: app, test: self)
            
            ActionUtils.scrollAndTap("Grid&Graph", application: app, test: self)
        }
        
        if (!LEAKMODE){
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
            ActionUtils.scrollAndTap("2 x 15,000 Grid", application: app, test: self)
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
            app.buttons["Close"].tap()
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
            sleep(5)
            _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
            sleep(5)
            ActionUtils.terminateAndRelaunch(app)
        }
        
        
        //6 x 15,000 Grid
        if (app.tables.staticTexts["MicroStrategy Tutorial"].exists){
            ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
            ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
            ActionUtils.scrollAndTap("_Memory Governor", application: app, test: self)
            
            ActionUtils.scrollAndTap("Grid&Graph", application: app, test: self)
        }
        
        for _ in 0 ..< count{
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
            ActionUtils.scrollAndTap("6 x 15,000 Grid", application: self.app, test: self)
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
            app.buttons["Close"].tap()
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
            sleep(5)
            _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
        
        }
        sleep(5)
        
        ActionUtils.terminateAndRelaunch(app)
        
        //90,000 Cells_2panel
        ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
        ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
        ActionUtils.scrollAndTap("_Memory Governor", application: app, test: self)
        
        ActionUtils.scrollAndTap("Grid&Graph", application: app, test: self)
        for _ in 0 ..< count{
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
            ActionUtils.scrollAndTap("90,000 Cells_2panel", application: app, test: self)
            sleep(5)
            
            ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
            
            _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
            //ActionUtils.printDescription(app)
            app.otherElements["Panel47, not selected, a radio button option"].tap()
            ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
            app.buttons["Close"].tap()
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
            sleep(5)
            _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
            
        }
        sleep(5)
        
        ActionUtils.terminateAndRelaunch(app)
        
        
 
        
        
        //90,000 Cells_2layout
        if(app.tables.staticTexts["MicroStrategy Tutorial"].exists){
            ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
            ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
            ActionUtils.scrollAndTap("_Memory Governor", application: app, test: self)
            
            ActionUtils.scrollAndTap("Grid&Graph", application: app, test: self)
            
            for _ in 0 ..< count{
                sleep(5)
                ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
                ActionUtils.scrollAndTap("90,000 Cells_2layout", application: self.app, test: self)
                sleep(5)
                ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
                
                _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
                
                app.staticTexts["Layout 1"].tap()
                
                ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
                app.staticTexts["6 x 15,000 Grid"].tap()
                sleep(5)
                ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
                
                app.buttons["Close"].tap()
                sleep(5)
                ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
                sleep(5)
                _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)

            }
            sleep(5)
            
            ActionUtils.terminateAndRelaunch(app)
            
            
        }
        
        //09.Year PB_14400display_43200
        if (app.tables.staticTexts["MicroStrategy Tutorial"].exists){
            ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
            ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
            ActionUtils.scrollAndTap("_Performance Automation", application: app, test: self)
            
        }
        
        for _ in 0 ..< count{
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
            ActionUtils.scrollAndTap("09.Year PB_14400display_43200", application: self.app, test: self)
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
            
            _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
            
            app.buttons["2007"].tap()
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
            app.staticTexts["2008"].tap()
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
            
            app.buttons["Close"].tap()
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
            sleep(5)
            _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
            //ActionUtils.verifyAppearWithTimeout("fb hamburger", application: app, type: .button, test: self, timeout: 600)
        }
        
        
        //10 Blank Panels_1 Panel Stack
        if (!LEAKMODE && app.tables.staticTexts["MicroStrategy Tutorial"].exists){
            ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
            ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
            ActionUtils.scrollAndTap("_Memory Governor", application: app, test: self)
            
            ActionUtils.scrollAndTap("Panel", application: app, test: self)
            
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
            ActionUtils.scrollAndTap("10 Blank Panels_1 Panel Stack", application: app, test: self)
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
            app.buttons["Close"].tap()
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
            sleep(5)
            _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
            
            sleep(5)
            
            ActionUtils.terminateAndRelaunch(app)
            
            
            //7 Blank Panel Stack
            if (app.tables.staticTexts["MicroStrategy Tutorial"].exists){
                ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
                ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
                ActionUtils.scrollAndTap("_Memory Governor", application: app, test: self)
                
                ActionUtils.scrollAndTap("Panel", application: app, test: self)
            }
            
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
            ActionUtils.scrollAndTap("7 Blank Panel Stack", application: app, test: self)
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
            app.buttons["Close"].tap()
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
            sleep(5)
            _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
            
            sleep(5)
            
        }
        

        
    }
    
    
    
    func test_002_CGBonGrid(){
        ActionUtils.clearCache(app, test: self)
        
        ActionUtils.scrollAndTap("Drill path", application: app, test: self)
        ActionUtils.scrollAndTap("_Sarah", application: app, test: self)
        ActionUtils.scrollAndTap("CGB", application: app, test: self)
        
        if (!LEAKMODE){
            
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
            ActionUtils.scrollAndTap("1.2_CGB on 1Grid_20MB", application: app, test: self)
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
            
            _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
            
            app.otherElements["selector, currently selected item is:2008"].tap()
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
            
            app.staticTexts["2007"].tap()
            sleep(5)
            
            ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
            
            app.buttons["Close"].tap()
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
            sleep(5)
            _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
            
            sleep(5)
            
            ActionUtils.terminateAndRelaunch(app)
            
            
        }
        
        for _ in 0 ..< count{
            
            ActionUtils.scrollAndTap("Drill path", application: app, test: self)
            ActionUtils.scrollAndTap("_Sarah", application: app, test: self)
            ActionUtils.scrollAndTap("CGB", application: app, test: self)
            
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
            ActionUtils.scrollAndTap("1.3_CGB on 1Grid_45MB", application: app, test: self)
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
            
            _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
            
            app.otherElements["selector, currently selected item is:Jan 2007:Jan"].tap()
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
            
            app.staticTexts["Feb 2007:Feb"].tap()
            sleep(5)
            
            ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
            
            app.buttons["Close"].tap()
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
            sleep(5)
            _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
            
            sleep(5)
            
            ActionUtils.terminateAndRelaunch(app)
        }
        
        if(!LEAKMODE){
            
            ActionUtils.scrollAndTap("Drill path", application: app, test: self)
            ActionUtils.scrollAndTap("_Sarah", application: app, test: self)
            ActionUtils.scrollAndTap("CGB", application: app, test: self)
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
            ActionUtils.scrollAndTap("2.1_CGB on FG_19MB", application: app, test: self)
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
            
            _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
            
           // ActionUtils.printDescription(app)
            
            app.otherElements["selector, currently selected item is:Jan 2007:Jan"].tap()
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
            
            app.staticTexts["Feb 2007:Feb"].tap()
            sleep(5)
            
            ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
            
            app.buttons["Close"].tap()
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
            sleep(5)
            _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
            
            sleep(5)
            
            ActionUtils.terminateAndRelaunch(app)
        }
        
        for _ in 0 ..< count {
            ActionUtils.scrollAndTap("Drill path", application: app, test: self)
            ActionUtils.scrollAndTap("_Sarah", application: app, test: self)
            ActionUtils.scrollAndTap("CGB", application: app, test: self)
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
            ActionUtils.scrollAndTap("2.1_CGB on FG_37MB", application: app, test: self)
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
            
            _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
            
            app.otherElements["selector, currently selected item is:Jan 2007:Jan"].tap()
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
            
            app.staticTexts["Feb 2007:Feb"].tap()
            sleep(5)
            
            ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
            
            app.buttons["Close"].tap()
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
            sleep(5)
            _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
            
            sleep(5)
            
            ActionUtils.terminateAndRelaunch(app)
        }
        
        if (!LEAKMODE){
            ActionUtils.scrollAndTap("Drill path", application: app, test: self)
            ActionUtils.scrollAndTap("_Sarah", application: app, test: self)
            ActionUtils.scrollAndTap("Slice Size test", application: app, test: self)
            ActionUtils.scrollAndTap("New_03022012", application: app, test: self)
            
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
            ActionUtils.scrollAndTap("1.1_1grid_94MB", application: app, test: self)
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
            
            _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
            
            app.otherElements["selector, currently selected item is:January"].tap()
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
            
            app.staticTexts["February"].tap()
            sleep(5)
            
            ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
            
            app.buttons["Close"].tap()
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
            sleep(5)
            _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
            
            sleep(5)
            
            ActionUtils.terminateAndRelaunch(app)
        }
        
    }
    
    
    func test_003_Text() {
        
        //400 Text
        ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
        ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
        ActionUtils.scrollAndTap("_Memory Governor", application: app, test: self)
        
        ActionUtils.scrollAndTap("Image&Text", application: app, test: self)
        ActionUtils.scrollAndTap("Text Viewer", application: app, test: self)
        
        if (!LEAKMODE){
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
            ActionUtils.scrollAndTap("400 Text", application: app, test: self)
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
            app.buttons["Close"].tap()
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
            sleep(5)
            _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
            sleep(5)
            ActionUtils.terminateAndRelaunch(app)
        }
        
        //800 Text
        if (app.tables.staticTexts["MicroStrategy Tutorial"].exists){
            ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
            ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
            ActionUtils.scrollAndTap("_Memory Governor", application: app, test: self)
            
            ActionUtils.scrollAndTap("Image&Text", application: app, test: self)
            ActionUtils.scrollAndTap("Text Viewer", application: app, test: self)
        }
        
        for _ in 0 ..< count{
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
            ActionUtils.scrollAndTap("800 Text", application: self.app, test: self)
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
            app.buttons["Close"].tap()
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
            sleep(5)
            _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
        }
        sleep(5)
        ActionUtils.terminateAndRelaunch(app)
        
        
        //5000_Details Section
        if (app.tables.staticTexts["MicroStrategy Tutorial"].exists){
            ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
            ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
            ActionUtils.scrollAndTap("_Memory Governor", application: app, test: self)
            
            ActionUtils.scrollAndTap("Image&Text", application: app, test: self)
            ActionUtils.scrollAndTap("Text Viewer", application: app, test: self)
        }
        
        if (!LEAKMODE){
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
            ActionUtils.scrollAndTap("5000_Details Section", application: app, test: self)
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
            app.buttons["Close"].tap()
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
            sleep(5)
            _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
            sleep(5)
            ActionUtils.terminateAndRelaunch(app)
        }
        
        
        //80000_Details Section
 /*       if (app.tables.staticTexts["MicroStrategy Tutorial"].exists){
            ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
            ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
            ActionUtils.scrollAndTap("_Memory Governor", application: app, test: self)
            
            ActionUtils.scrollAndTap("Image&Text", application: app, test: self)
            ActionUtils.scrollAndTap("Text Viewer", application: app, test: self)
        }
        
        for _ in 0 ..< count{
            sleep(5)
            
            ActionUtils.scrollAndTap("80000_Details Section", application: self.app, test: self)
            sleep(45)
           // ActionUtils.verifyAppearWithTimeout("fb hamburger", application: app, type: .button, test: self, timeout: 600)
        }
        sleep(5)
        ActionUtils.terminateAndRelaunch(app)
 
        */
        //5000_Details Section_Copy/Paste
        if (app.tables.staticTexts["MicroStrategy Tutorial"].exists){
            ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
            ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
            ActionUtils.scrollAndTap("_Memory Governor", application: app, test: self)
            
            ActionUtils.scrollAndTap("Image&Text", application: app, test: self)
            ActionUtils.scrollAndTap("Text Viewer", application: app, test: self)
        }
        
        if (!LEAKMODE){
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
            ActionUtils.scrollAndTap("5000_Details Section_Copy/Paste", application: app, test: self)
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
            app.buttons["Close"].tap()
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
            sleep(5)
            _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
            sleep(5)
            ActionUtils.terminateAndRelaunch(app)
        }
        
        
        //80000_Details Section_Copy/Past
  /*      if (app.tables.staticTexts["MicroStrategy Tutorial"].exists){
            ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
            ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
            ActionUtils.scrollAndTap("_Memory Governor", application: app, test: self)
            
            ActionUtils.scrollAndTap("Image&Text", application: app, test: self)
            ActionUtils.scrollAndTap("Text Viewer", application: app, test: self)
        }
        
        for var i = 0; i < count; ++i{
            sleep(5)
            
            ActionUtils.scrollAndTap("80000_Details Section_Copy/Past", application: self.app, test: self)
            ActionUtils.verifyAppearWithTimeout("fb hamburger", application: app, type: .button, test: self, timeout: 600)
        }
        sleep(5)
        ActionUtils.terminateAndRelaunch(app)*/
    }
    

    
    func test_004_ImageGrid01() {

        //1.55MB png
        for i in 0 ..< 2{
            if(app.tables.staticTexts["MicroStrategy Tutorial"].exists){
                ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
                ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
                ActionUtils.scrollAndTap("_Memory Governor", application: app, test: self)
                ActionUtils.scrollAndTap("Image&Text", application: app, test: self)
                ActionUtils.scrollAndTap("Image Grid", application: app, test: self)
            }
            if ((i == 0) && !LEAKMODE){
                
                ActionUtils.clearCache(app, test: self)
            }
            
            for _ in 0 ..< count{
                
                sleep(5)
                ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
                ActionUtils.scrollAndTap("1.55MB png", application: app, test: self)
                sleep(5)
                ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
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

        //10MB BMP
        for i in 0 ..< 2{
            if(app.tables.staticTexts["MicroStrategy Tutorial"].exists){
                ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
                ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
                ActionUtils.scrollAndTap("_Memory Governor", application: app, test: self)
                ActionUtils.scrollAndTap("Image&Text", application: app, test: self)
                ActionUtils.scrollAndTap("Image Grid", application: app, test: self)
            }
            if ((i == 0) && !LEAKMODE){
                
                ActionUtils.clearCache(app, test: self)
            }
            
            for _ in 0 ..< count{
                
                sleep(5)
                ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
                ActionUtils.scrollAndTap("10MB BMP", application: app, test: self)
                sleep(5)
                ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
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
        
        //Gucci_250images
        
        if (!LEAKMODE){
            for i in 0 ..< 2{
                ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
                ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
                ActionUtils.scrollAndTap("_Memory Governor", application: app, test: self)
                
                ActionUtils.scrollAndTap("Image&Text", application: app, test: self)
                ActionUtils.scrollAndTap("Image Grid", application: app, test: self)
                
                if(i == 0){
                    ActionUtils.clearCache(app, test: self)
                }
                
                sleep(5)
                ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
                ActionUtils.scrollAndTap("Gucci_250images", application: app, test: self)
                sleep(5)
                ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
                app.buttons["Close"].tap()
                sleep(5)
                ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
                sleep(5)
                _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
                sleep(5)
                ActionUtils.terminateAndRelaunch(app)
            }
        }
        
        //Gucci_250images_Column
        for i in 0 ..< 2{
            if(app.tables.staticTexts["MicroStrategy Tutorial"].exists){
                ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
                ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
                ActionUtils.scrollAndTap("_Memory Governor", application: app, test: self)
                ActionUtils.scrollAndTap("Image&Text", application: app, test: self)
                ActionUtils.scrollAndTap("Image Grid", application: app, test: self)
            }
            if ((i == 0) && !LEAKMODE){
                
                ActionUtils.clearCache(app, test: self)
            }
            
            for _ in 0 ..< count{
                
                sleep(5)
                ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
                ActionUtils.scrollAndTap("Gucci_250images_Column", application: app, test: self)
                sleep(5)
                ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
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
    
    func test_004_ImageGrid02(){
    
        //Gucci_250images_Pageby50each
        
        if (!LEAKMODE){
            for i in 0 ..< 2{
                ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
                ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
                ActionUtils.scrollAndTap("_Memory Governor", application: app, test: self)
                
                ActionUtils.scrollAndTap("Image&Text", application: app, test: self)
                ActionUtils.scrollAndTap("Image Grid", application: app, test: self)
                
                if(i == 0){
                    ActionUtils.clearCache(app, test: self)
                }
                
                sleep(5)
                ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
                ActionUtils.scrollAndTap("Gucci_250images_Pageby50each", application: app, test: self)
                sleep(5)
                ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
                app.buttons["Close"].tap()
                sleep(5)
                ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
                sleep(5)
                _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
                sleep(5)
                ActionUtils.terminateAndRelaunch(app)
            }
            
            //4.5MB BMP Full
            for i in 0 ..< 2{
                if(app.tables.staticTexts["MicroStrategy Tutorial"].exists){
                    ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
                    ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
                    ActionUtils.scrollAndTap("_Memory Governor", application: app, test: self)
                    
                    ActionUtils.scrollAndTap("Image&Text", application: app, test: self)
                    ActionUtils.scrollAndTap("Image Viewer", application: app, test: self)
                }
                
                if(i == 0){
                    ActionUtils.clearCache(app, test: self)
                }
                
                sleep(5)
                ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
                ActionUtils.scrollAndTap("4.5MB BMP Full", application: app, test: self)
                sleep(5)
                ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
                app.buttons["Close"].tap()
                sleep(5)
                ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
                sleep(5)
                _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
                sleep(5)
                ActionUtils.terminateAndRelaunch(app)
            }
            
            //4.5MB BMP 1/4
            for i in 0 ..< 2{
                if(app.tables.staticTexts["MicroStrategy Tutorial"].exists){
                    ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
                    ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
                    ActionUtils.scrollAndTap("_Memory Governor", application: app, test: self)
                    
                    ActionUtils.scrollAndTap("Image&Text", application: app, test: self)
                    ActionUtils.scrollAndTap("Image Viewer", application: app, test: self)
                }
                
                if(i == 0){
                    ActionUtils.clearCache(app, test: self)
                }
                
                sleep(5)
                ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
                ActionUtils.scrollAndTap("4.5MB BMP 1/4", application: app, test: self)
                sleep(5)
                ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
                app.buttons["Close"].tap()
                sleep(5)
                ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
                sleep(5)
                _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
                
                sleep(5)
                
                ActionUtils.terminateAndRelaunch(app)
            }
            
            //40_BMP_800KB
            for i in 0 ..< 2{
                if(app.tables.staticTexts["MicroStrategy Tutorial"].exists){
                    ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
                    ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
                    ActionUtils.scrollAndTap("_Memory Governor", application: app, test: self)
                    
                    ActionUtils.scrollAndTap("Image&Text", application: app, test: self)
                    ActionUtils.scrollAndTap("Image Viewer", application: app, test: self)
                }
                
                if(i == 0){
                    ActionUtils.clearCache(app, test: self)
                }
                
                sleep(5)
                ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
                ActionUtils.scrollAndTap("40_BMP_800KB", application: app, test: self)
                sleep(5)
                ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
                app.buttons["Close"].tap()
                sleep(5)
                ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
                sleep(5)
                _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
                sleep(5)
                ActionUtils.terminateAndRelaunch(app)
            }
            
            
            //10_4.5MB BMP Full
            for i in 0 ..< 2{
                if(app.tables.staticTexts["MicroStrategy Tutorial"].exists){
                    ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
                    ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
                    ActionUtils.scrollAndTap("_Memory Governor", application: app, test: self)
                    
                    ActionUtils.scrollAndTap("Image&Text", application: app, test: self)
                    ActionUtils.scrollAndTap("Image Viewer", application: app, test: self)
                }
                
                if(i == 0){
                    ActionUtils.clearCache(app, test: self)
                }
                
                sleep(5)
                ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
                ActionUtils.scrollAndTap("10_4.5MB BMP Full", application: app, test: self)
                sleep(5)
                ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
                app.buttons["Close"].tap()
                sleep(5)
                ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
                sleep(5)
                _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
                sleep(5)
                ActionUtils.terminateAndRelaunch(app)
            }
            
            //embeded image
            for i in 0 ..< 2{
                if(app.tables.staticTexts["MicroStrategy Tutorial"].exists){
                    ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
                    ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
                    ActionUtils.scrollAndTap("_Memory Governor", application: app, test: self)
                    
                    ActionUtils.scrollAndTap("Image&Text", application: app, test: self)
                    ActionUtils.scrollAndTap("Image Viewer", application: app, test: self)
                }
                
                if(i == 0){
                    ActionUtils.clearCache(app, test: self)
                }
                
                sleep(5)
                ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
                ActionUtils.scrollAndTap("6_Large_Embeded Image", application: app, test: self)
                sleep(5)
                ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
                app.buttons["Close"].tap()
                sleep(5)
                ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
                sleep(5)
                _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
                sleep(5)
                ActionUtils.terminateAndRelaunch(app)
            }

            
        }
        
        
    }
    
    
/*    func test_005_SelectorType() {
        
        //5000_ButtonBar

        ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
        ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
        ActionUtils.scrollAndTap("_Memory Automation", application: app, test: self)
        
        ActionUtils.scrollAndTap("Selector", application: app, test: self)

        for _ in 0 ..< count{
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
            ActionUtils.scrollAndTap("5000_ButtonBar", application: self.app, test: self)
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
            app.buttons["Close"].tap()
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
            sleep(5)
            _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
        }
        sleep(5)
        ActionUtils.terminateAndRelaunch(app)
        
        //5000_CheckBox

        if (app.tables.staticTexts["MicroStrategy Tutorial"].exists){
            ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
            ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
            ActionUtils.scrollAndTap("_Memory Automation", application: app, test: self)
            
            ActionUtils.scrollAndTap("Selector", application: app, test: self)
        }
        
        for _ in 0 ..< count{
            sleep(5)
            
            ActionUtils.scrollAndTap("5000_CheckBox", application: self.app, test: self)
            sleep(180)
        }
        sleep(5)
        ActionUtils.terminateAndRelaunch(app)
        
        
        //5000_RadioButton
        if (app.tables.staticTexts["MicroStrategy Tutorial"].exists){
            ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
            ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
            ActionUtils.scrollAndTap("_Memory Automation", application: app, test: self)
            
            ActionUtils.scrollAndTap("Selector", application: app, test: self)
        }
        
        for _ in 0 ..< count{
            sleep(5)
            
            ActionUtils.scrollAndTap("5000_RadioButton", application: self.app, test: self)
            sleep(180)
        }
        sleep(5)
        ActionUtils.terminateAndRelaunch(app)
        
        
        //5000_DropDownList
        if (app.tables.staticTexts["MicroStrategy Tutorial"].exists){
            ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
            ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
            ActionUtils.scrollAndTap("_Memory Automation", application: app, test: self)
            
            ActionUtils.scrollAndTap("Selector", application: app, test: self)
        }
        
        for _ in 0 ..< count{
            sleep(5)
            
            ActionUtils.scrollAndTap("5000_DropDownList", application: self.app, test: self)
            sleep(5)
            ActionUtils.verifyAppearWithTimeout("fb hamburger", application: app, type: .button, test: self, timeout: 600)
        }
        sleep(5)
        ActionUtils.terminateAndRelaunch(app)
        
        //5000_LinkBar
        if (app.tables.staticTexts["MicroStrategy Tutorial"].exists){
            ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
            ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
            ActionUtils.scrollAndTap("_Memory Automation", application: app, test: self)
            
            ActionUtils.scrollAndTap("Selector", application: app, test: self)
        }
        
        for _ in 0 ..< count{
            sleep(5)
            
            ActionUtils.scrollAndTap("5000_LinkBar", application: self.app, test: self)
            sleep(180)
        }
        sleep(5)
        ActionUtils.terminateAndRelaunch(app)
        
        
        //5000_ListBox
        if (app.tables.staticTexts["MicroStrategy Tutorial"].exists){
            ActionUtils.scrollAndTap("MicroStrategy Tutorial", application: app, test: self)
            ActionUtils.scrollAndTap("_Wenting", application: app, test: self)
            ActionUtils.scrollAndTap("_Memory Automation", application: app, test: self)
            
            ActionUtils.scrollAndTap("Selector", application: app, test: self)
        }
        
        for _ in 0 ..< count{
            sleep(5)
            
            ActionUtils.scrollAndTap("5000_ListBox", application: self.app, test: self)
            sleep(180)
        }
        sleep(5)
        ActionUtils.terminateAndRelaunch(app)
        
    }*/
    
    
    func test_006_Transaction(){
        
        //Grid-MarkRowsForSelection-Checked_2000row_Refresh
/*        ActionUtils.scrollAndTap("Transactions(SingleGridSource=False)", application: app, test: self)
        ActionUtils.scrollAndTap("Test Suite", application: app, test: self)
        ActionUtils.scrollAndTap("Offline", application: app, test: self)
        
        
        for _ in 0 ..< count{
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
            ActionUtils.scrollAndTap("Grid-MarkRowsForSelection-Checked_2000row_Refresh", application: self.app, test: self)
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
            app.buttons["Close"].tap()
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
            sleep(5)
            _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
        }
        sleep(5)
        ActionUtils.terminateAndRelaunch(app)
        
        
        //Perf_Used_2000row_Refresh
        if(app.tables.staticTexts["Transactions(SingleGridSource=False)"].exists){
            ActionUtils.scrollAndTap("Transactions(SingleGridSource=False)", application: app, test: self)
            ActionUtils.scrollAndTap("Test Suite", application: app, test: self)

        }
        
        for _ in 0 ..< count{
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
            ActionUtils.scrollAndTap("Perf_Used_2000row_Refresh", application: self.app, test: self)
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
            app.buttons["Close"].tap()
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
            sleep(5)
            _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
        }
        sleep(5)
        ActionUtils.terminateAndRelaunch(app)*/
        
        
        //Perf_Used_Field_Refresh
        if(app.tables.staticTexts["Transactions(SingleGridSource=False)"].exists){
            ActionUtils.scrollAndTap("Transactions(SingleGridSource=False)", application: app, test: self)
            ActionUtils.scrollAndTap("Test Suite", application: app, test: self)
            
        }
        
        for _ in 0 ..< count{
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
            ActionUtils.scrollAndTap("Perf_Used_Field_Refresh", application: self.app, test: self)
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
            _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
            sleep(5)
            
            ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
            app.switches.element.tap()
            sleep(1)
            
            app.buttons["stepper plus"].tap()
            sleep(1)
            
            
            app.staticTexts["30"].tap()
            sleep(1)
            
            app.buttons["Submit"].tap()
            sleep(7)
            
            ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)

            app.buttons["Close"].tap()
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
            sleep(5)
            _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
        }
        sleep(5)
        ActionUtils.terminateAndRelaunch(app)
        
        //Attribute-Default_KPI
        if(app.tables.staticTexts["Transactions(SingleGridSource=False)"].exists){
            ActionUtils.scrollAndTap("Transactions(SingleGridSource=False)", application: app, test: self)
            ActionUtils.scrollAndTap("Test Suite", application: app, test: self)
            ActionUtils.scrollAndTap("Data Input Control", application: app, test: self)
            ActionUtils.scrollAndTap("DDIC", application: app, test: self)
        }
        
        for _ in 0 ..< count{
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
            ActionUtils.scrollAndTap("Attribute-Default_KPI_SmallList", application: self.app, test: self)
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
            _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
            sleep(5)
            
            ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
            
            for i in 1..<6 {
                //var tapString = "jinAttr1: " + i + ", Addison"
                var tapString = "jinAttr1: \(i), Addison"
                //let position = CGPoint(x: 42, y: 160 + 15 * CGFloat(i))
                //ActionUtils.tapAtPosition(position: position, element: app)
                //ActionUtils.scrollAndTap(tapString, application: app, test: self)
                ActionUtils.verifyAndTapElement(tapString, application: app, type: .other, test: self, tap: true)
                
                sleep(2)
                
                app.staticTexts["Akron"].tap()
                sleep(2)
                
                
            }
            
            app.buttons["Submit"].tap()
            sleep(7)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
            app.buttons["Close"].tap()
            sleep(5)
            ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
            sleep(5)
            _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
        }
        sleep(5)
        ActionUtils.terminateAndRelaunch(app)
    }
    
    func test_007_SmartClient(){
    
        //Field_Default_Update_1
        if(app.tables.staticTexts["Transactions(SingleGridSource=False)"].exists){
            ActionUtils.scrollAndTap("Transactions(SingleGridSource=False)", application: app, test: self)
            ActionUtils.scrollAndTap("Transaction Perf&Stab", application: app, test: self)
            ActionUtils.scrollAndTap("Polaris", application: app, test: self)
            ActionUtils.scrollAndTap("SmartClient", application: app, test: self)
            ActionUtils.scrollAndTap("20DocTo1Dataset", application: app, test: self)
        }
        sleep(5)
        ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
        ActionUtils.scrollAndTap("Field_Default_Update_1", application: app, test: self)
        sleep(5)
        ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
        sleep(5)
        
        app.buttons["Close"].tap()
        sleep(5)
        ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
        sleep(5)
        _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
        
        sleep(5)
        
        ActionUtils.terminateAndRelaunch(app)
        
        ActionUtils.scrollAndTap("Transactions(SingleGridSource=False)", application: app, test: self)
        ActionUtils.scrollAndTap("Transaction Perf&Stab", application: app, test: self)
        ActionUtils.scrollAndTap("Polaris", application: app, test: self)
        ActionUtils.scrollAndTap("SmartClient", application: app, test: self)
        ActionUtils.scrollAndTap("20DocTo1Dataset", application: app, test: self)
        
        sleep(5)
        
        ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
        ActionUtils.scrollAndTap("Field_Default_Update_0", application: app, test: self)
        sleep(5)
        ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
        sleep(5)
        
        app.buttons["Close"].tap()
        sleep(5)
        ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
        sleep(5)
        _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
        
        ActionUtils.changeOfflineMode(app, test: self)
        
        for _ in 0 ..< count{
            
            if (!LEAKMODE){

                ActionUtils.scrollAndTap("Field_Default_Update_1", application: app, test: self)
                ActionUtils.verifyAndTapElement("Submit", application: app, type: .button, test: self, tap: false)

                
                for _ in 0..<10 {
                    app.switches.element.tap()
                    sleep(1)
                    app.buttons["Submit"].tap()
                    sleep(5)
                }
                
                app.buttons["Close"].tap()
                
                sleep(5)
                
                ActionUtils.terminateAndRelaunch(app)
                
                ActionUtils.scrollAndTap("Transactions(SingleGridSource=False)", application: app, test: self)
                ActionUtils.scrollAndTap("Transaction Perf&Stab", application: app, test: self)
                ActionUtils.scrollAndTap("Polaris", application: app, test: self)
                ActionUtils.scrollAndTap("SmartClient", application: app, test: self)
                ActionUtils.scrollAndTap("20DocTo1Dataset", application: app, test: self)
                
                sleep(5)
                ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
                ActionUtils.scrollAndTap("Field_Default_Update_1", application: app, test: self)
                sleep(5)
                ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
                sleep(5)
                
                app.buttons["Close"].tap()
                sleep(5)
                ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
                sleep(5)
                _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
                
                sleep(5)
                
                ActionUtils.terminateAndRelaunch(app)
                
                ActionUtils.scrollAndTap("Transactions(SingleGridSource=False)", application: app, test: self)
                ActionUtils.scrollAndTap("Transaction Perf&Stab", application: app, test: self)
                ActionUtils.scrollAndTap("Polaris", application: app, test: self)
                ActionUtils.scrollAndTap("SmartClient", application: app, test: self)
                ActionUtils.scrollAndTap("20DocTo1Dataset", application: app, test: self)
                
                sleep(5)
                
                ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
                ActionUtils.scrollAndTap("Field_Default_Update_0", application: app, test: self)
                sleep(5)
                ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
                sleep(5)
                
                app.buttons["Close"].tap()
                sleep(5)
                ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
                sleep(5)
                _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)
                
            }else{
                sleep(5)
                ActionUtils.getMemory(IPAddress: IPAddress, message: "before", test: self, app: app)
                ActionUtils.scrollAndTap("Field_Default_Update_1", application: app, test: self)
                sleep(5)
                ActionUtils.getMemory(IPAddress: IPAddress, message: "after", test: self, app: app)
                sleep(5)
                
                app.switches.element.tap()
                sleep(1)
                app.buttons["Submit"].tap()
                sleep(5)
                
                app.buttons["Close"].tap()
                sleep(5)
                ActionUtils.getMemory(IPAddress: IPAddress, message: "close", test: self, app: app)
                sleep(5)
                _ = self.dataFromRemoteEndpoint("sendMemory", args: IPAddress)

            }

            
        }
        ActionUtils.changeOfflineMode(app, test: self)
        sleep(5)
        ActionUtils.terminateAndRelaunch(app)


    }
    
    
}
