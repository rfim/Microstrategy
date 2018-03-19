//
//  MobilePNR_Customer.swift
//  MicroStrategyMobile
//
//  Created by Administrator on 10/15/15.
//  Copyright © 2015 MicroStrategy Inc. All rights reserved.
//

import XCTest

class MobilePNR_Customer: XCTestCase {
    let app = XCUIApplication()
    let device = XCUIDevice()
    override func setUp() {
        super.setUp()
        
        // Put setup code here. This method is called before the invocation of each test method in the class.
        
        // In UI tests it is usually best to stop immediately when a failure occurs.
        continueAfterFailure = false
        // UI tests must launch the application that they test. Doing this in setup will make sure it happens for each test method.
        app.launch()
        sleep(7)

        // In UI tests it’s important to set the initial state - such as interface orientation - required for your tests before they run. The setUp method is a good place to do this.
    }
    
    override func tearDown() {
        // Put teardown code here. This method is called after the invocation of each test method in the class.
        app.terminate()
        
        super.tearDown()
    }
    
    func test_001_RossSlicing() {
        // Use recording to get started writing UI tests.
        // Use XCTAssert and related functions to verify your tests produce the correct results.
        ActionUtils.clearCache(app, test: self)
        ActionUtils.scrollAndTap("MicroStrategy Tutorial (HQ)", application: app, test: self)
        ActionUtils.scrollAndTap("PM KPI Sheet", application: app, test: self)
        ActionUtils.scrollAndTap("Flash", application: app, test: self)
        
        for i in 0..<3 {
            if (i == 1){
                ActionUtils.clearCache(app, test: self)
            }
            
            ActionUtils.scrollAndTap("ROSS - Cached", application: app, test: self)

            ActionUtils.verifyAndTapElement("Region:", application: app, type: .staticText, test: self, tap: false)
            if(i == 2 && ENABLE_MANI){
                app.scrollViews.otherElements["selector, currently selected item is:Dothan"].tap()
                app.tables.staticTexts["Greensboro"].tap()
            }
            
            app.buttons["Close"].tap()
        }
        
        sleep(3)
        

    }
    
    
    func test_002_RossUC(){
        
        ActionUtils.clearCache(app, test: self)
        ActionUtils.scrollAndTap("MicroStrategy Tutorial (HQ)", application: app, test: self)
        ActionUtils.scrollAndTap("PM KPI Sheet", application: app, test: self)
        ActionUtils.scrollAndTap("Flash", application: app, test: self)
    
        for i in 0..<3 {
            
            if (i == 1){
                ActionUtils.clearCache(app, test: self)
            }
            
            ActionUtils.scrollAndTap("ROSS - with UC", application: app, test: self)
            ActionUtils.verifyAndTapElement("Region:", application: app, type: .staticText, test: self, tap: false)
            if(i == 2 && ENABLE_MANI){
                app.scrollViews.otherElements["selector, currently selected item is:Dothan"].tap()
                app.tables.staticTexts["Greensboro"].tap()
            }
            
            app.buttons["Close"].tap()
        }
        sleep(3)

    }
    
    func test_003_FAFlash(){
        ActionUtils.clearCache(app, test: self)
        ActionUtils.scrollAndTap("MicroStrategy Tutorial (HQ)", application: app, test: self)
        ActionUtils.scrollAndTap("PM KPI Sheet", application: app, test: self)
        ActionUtils.scrollAndTap("Flash", application: app, test: self)
        
    
        for i in 0..<3 {
            
            if (i == 1){
                ActionUtils.clearCache(app, test: self)
            }
            ActionUtils.scrollAndTap("FA - Flash - Cached", application: app, test: self)

            ActionUtils.verifyAndTapElement("Page by:", application: app, type: .staticText, test: self, tap: false)
            if(i == 2 && ENABLE_MANI){
                app.buttons["Visa"].tap()
                app.tables.staticTexts["Amex"].tap()
                sleep(3)
                app.scrollViews.otherElements["selector, currently selected item is:Panel1"].tap()
                app.tables.staticTexts["Panel3"].tap()
                sleep(5)
                // app.scrollViews.elementBoundByIndex(0).otherElements[", Unit Cost, Column Headings"].pressForDuration(LONG_PRESS)
                
                app.scrollViews.containing(.other, identifier:", Unit Cost, Column Headings").element(boundBy: 0).otherElements[", Unit Cost, Column Headings"].press(forDuration: LONG_PRESS)
                ActionUtils.verifyAndTapElement("Sort", application: app, type: .button, test: self, tap: true)
                sleep(2)
                app.buttons["Exit full screen mode."].tap()
                
            }
            
            ActionUtils.quitFullScreenAndClose(app, test: self)
        }
        
      //  app.terminate()

        
    }
    
    
    func test_004_CitiBankSlicing() {
        // Use recording to get started writing UI tests.
        // Use XCTAssert and related functions to verify your tests produce the correct results.
        ActionUtils.clearCache(app, test: self)
        app.staticTexts["MicroStrategy Tutorial (HQ)"].tap()
        ActionUtils.scrollAndTap("PM KPI Sheet", application: app, test: self)
        ActionUtils.scrollAndTap("Flash", application: app, test: self)
        
        for i in 0..<3 {
            
            if (i == 1){
                ActionUtils.clearCache(app, test: self)
            }
            ActionUtils.scrollAndTap("CitiBank with Slicing-Cached", application: app, test: self)

            ActionUtils.verifyAndTapElement("Filters:", application: app, type: .staticText, test: self, tap: false)
            if(i == 2 && ENABLE_MANI){
                app.scrollViews.otherElements["selector, currently selected item is:District of Columbia"].tap()
                ActionUtils.scrollAndTap("West Virginia", application: app, test: self)
                
                
            }
            
            ActionUtils.quitFullScreenAndClose(app, test: self)
        }
        
      //  app.terminate()
    }
    
    func test_005_CitiBankUC(){
        
        ActionUtils.clearCache(app, test: self)
        app.staticTexts["MicroStrategy Tutorial (HQ)"].tap()
        ActionUtils.scrollAndTap("PM KPI Sheet", application: app, test: self)
        ActionUtils.scrollAndTap("Flash", application: app, test: self)
    
        for i in 0..<3 {
            
            if (i == 1){
                ActionUtils.clearCache(app, test: self)
            }
            ActionUtils.scrollAndTap("CitiBank with UC-Cached", application: app, test: self)
            ActionUtils.verifyAndTapElement("Filters:", application: app, type: .staticText, test: self, tap: false)
            if(i == 2 && ENABLE_MANI){
                app.scrollViews.otherElements["selector, currently selected item is:District of Columbia"].tap()
                ActionUtils.scrollAndTap("West Virginia", application: app, test: self)
                
                
            }
            
            ActionUtils.quitFullScreenAndClose(app, test: self)
        }
        
      //  app.terminate()
    }
    
    func test_006_CitiBankCube(){
        
        ActionUtils.clearCache(app, test: self)
        app.staticTexts["MicroStrategy Tutorial (HQ)"].tap()
        ActionUtils.scrollAndTap("PM KPI Sheet", application: app, test: self)
        ActionUtils.scrollAndTap("Flash", application: app, test: self)
        
    
        for i in 0..<3 {
            
            if (i == 1){
                ActionUtils.clearCache(app, test: self)
            }
            ActionUtils.scrollAndTap("CitiBank with Cube as DS and UC Selectors - Flash-Cached", application: app, test: self)
            ActionUtils.verifyAndTapElement("Filters:", application: app, type: .staticText, test: self, tap: false)
            if(i == 2 && ENABLE_MANI){
                app.scrollViews.otherElements["selector, currently selected item is:District of Columbia"].tap()
                ActionUtils.printDescription(app)
                ActionUtils.scrollAndTap("Texas", application: app, test: self)
                
                
            }
            
            ActionUtils.quitFullScreenAndClose(app, test: self)
        }
       // app.terminate()
        
        
    }
    
    func test_007_DeustcheBank(){
        
        ActionUtils.clearCache(app, test: self)
        app.staticTexts["MicroStrategy Tutorial (HQ)"].tap()
        ActionUtils.scrollAndTap("PM KPI Sheet", application: app, test: self)
        ActionUtils.scrollAndTap("Flash", application: app, test: self)
        
        for i in 0..<3 {
            
            if (i == 1){
                ActionUtils.clearCache(app, test: self)
            }
            
            ActionUtils.scrollAndTap("Deustche Bank - Flash Cached", application: app, test: self)
            if(i == 0 || i == 1){
                ActionUtils.verifyAndTapElement("Apply", application: app, type: .button, test: self, tap: true)
            }
            ActionUtils.verifyAndTapElement("selector, currently selected item is:(All)", application: app, type: .other, test: self, tap: false)
            
            //ActionUtils.printDescription(app)
            
            if(i == 2 && ENABLE_MANI){
                app.swipeLeft()
                sleep(2)
                //ActionUtils.verifyAppear("Title.ROA", application: app, type: .Other, test: self)
                //print(app.debugDescription)
                app.scrollViews.containing(.other, identifier: ", Region, Row Headings").element(boundBy: 2).otherElements[", Region, Row Headings"].press(forDuration: LONG_PRESS)
                sleep(2)
                app.buttons["Drill"].tap()
                app.staticTexts["Customer State"].tap()
                
                ActionUtils.verifyAndTapElement(", Customer State, Row Headings", application: app, type: .other, test: self, tap: false)

            }
            
            ActionUtils.quitFullScreenAndClose(app, test: self)
        }
        
      //  app.terminate()
    }
    
    func test_008_Sonic(){
        ActionUtils.clearCache(app, test: self)
        app.staticTexts["SONIC"].tap()
        ActionUtils.scrollAndTap("iPad", application: app, test: self)
        ActionUtils.scrollAndTap("Documents", application: app, test: self)
        ActionUtils.scrollAndTap("MFR", application: app, test: self)
        ActionUtils.scrollAndTap("2mb limit", application: app, test: self)
        
        for i in 0..<3 {
            if (i == 1){
                ActionUtils.clearCache(app, test: self)
            }
            
            ActionUtils.scrollAndTap("MFR Dealership test", application: app, test: self)
            
            if(i == 2 && ENABLE_MANI){
                app.scrollViews.otherElements["selector, currently selected item is:Acura of Serramonte"].tap()
                ActionUtils.scrollAndTap("BMW of Chattanooga", application: app, test: self)
                
                app.scrollViews.otherElements["selector, currently selected item is:NEW VEHICLE"].tap()
                ActionUtils.scrollAndTap("USED VEHICLE", application: app, test: self)
                
                app.otherElements["Comparison, not selected, Button Bar."].tap()
                sleep(3)
                
                app.scrollViews.otherElements["selector, currently selected item is:NEW VEHICLE"].tap()
                ActionUtils.scrollAndTap("F&I", application: app, test: self)
                
                app.scrollViews.otherElements["selector, currently selected item is:Total Net F&I"].tap()
                ActionUtils.scrollAndTap("F&I NET PUR", application: app, test: self)
                
                app.otherElements["Report, not selected, Button Bar."].tap()
                
                
                
            }
            app.buttons["Close"].tap()
            
        }
        
     //   app.terminate()
    }
    
 
    func test_009_DisneyFHBP(){
        
        ActionUtils.clearCache(app, test: self)
        app.staticTexts["DCP Dashboard - Disney"].tap()
        ActionUtils.scrollAndTap("_Dashboard iPad", application: app, test: self)
        
        for i in 0..<3 {
            if (i == 1){
                ActionUtils.clearCache(app, test: self)
            }
            ActionUtils.scrollAndTap("FHBP Dashboard_Cache enable", application: app, test: self)
            ActionUtils.verifyAndTapElement("REGION", application: app, type: .staticText, test: self, tap: false)
            if(i == 2 && ENABLE_MANI){
                
                app.otherElements["Wholesale Sales, not selected, Link Bar."].tap()
                
                app.scrollViews.containing(.other, identifier: "selector, currently selected item is:(All)").element(boundBy: 0).otherElements["selector, currently selected item is:(All)"].tap()
                app.tables.staticTexts["Emerging Markets Worldwide"].tap()
                sleep(5)
                
                app.tap()
                app.otherElements["Prior Year , not selected, a radio button option"].tap()
            }
            
            ActionUtils.quitFullScreenAndClose(app, test: self)
        }
      
      //  app.terminate()
    }
    
    func test_010_DisneyStationary(){
        
        ActionUtils.clearCache(app, test: self)
        app.staticTexts["DCP Dashboard - Disney"].tap()
        ActionUtils.scrollAndTap("_Dashboard iPad", application: app, test: self)
    
        for i in 0..<3 {
            if (i == 1){
                ActionUtils.clearCache(app, test: self)
            }
            ActionUtils.scrollAndTap("Stationery Dashboard_cache enable", application: app, test: self)
            ActionUtils.verifyAndTapElement("REGION", application: app, type: .staticText, test: self, tap: false)
            
            if(i == 2 && ENABLE_MANI){
                
                app.otherElements["Wholesale Sales, not selected, Button Bar."].tap()
                
                app.scrollViews.containing(.other, identifier: "selector, currently selected item is:(All)").element(boundBy: 0).otherElements["selector, currently selected item is:(All)"].tap()
                app.tables.staticTexts["Asia"].tap()
                sleep(5)
                
                app.tap()
                app.otherElements["Prior Year, not selected, a radio button option"].tap()
            }
            
            ActionUtils.quitFullScreenAndClose(app, test: self)
        }
        
        //app.terminate()
    }
    
    func test_011_DisneyToys(){
        
        ActionUtils.clearCache(app, test: self)
        app.staticTexts["DCP Dashboard - Disney"].tap()
        ActionUtils.scrollAndTap("_Dashboard iPad", application: app, test: self)
    
        for i in 0..<3 {
            if (i == 1){
                ActionUtils.clearCache(app, test: self)
            }
            ActionUtils.scrollAndTap("Toys Dashboard_cache enable", application: app, test: self)
            ActionUtils.verifyAndTapElement("REGION", application: app, type: .staticText, test: self, tap: false)
            
            if(i == 2 && ENABLE_MANI){
                
                app.otherElements["Wholesale Sales, not selected, Button Bar."].tap()
                
                app.scrollViews.containing(.other, identifier: "selector, currently selected item is:(All)").element(boundBy: 0).otherElements["selector, currently selected item is:(All)"].tap()
                app.tables.staticTexts["Asia"].tap()
                sleep(5)
                
                app.tap()
                app.otherElements["Prior Year, not selected, a radio button option"].tap()
            }
            
            ActionUtils.quitFullScreenAndClose(app, test: self)
        }
        
       // app.terminate()
        
    }
    
    func test_012_AirplaneMap(){
        ActionUtils.clearCache(app, test: self)
        app.tables.staticTexts["Airline On Time Performance"].tap()
        ActionUtils.scrollAndTap("Airline Airport App", application: app, test: self)
        for _ in 0..<2{
            ActionUtils.scrollAndTap("1. Flight Summary by Airport", application: app, test: self)
            
            ActionUtils.printDescription(app)

            ActionUtils.verifyAndTapElement("< 45%", application: app, type: .staticText, test: self, tap: false)
            
        /*    if (i == 1 && ENABLE_MANI){
                app.scrollViews.otherElements["This is a marker.Origin Airport:Salt Lake International,Scheduled Flights:,Average Delay (for delayed flights):,Avg Weather Delay:,"].tap()
            
                app.staticTexts["Tap a bubble to see monthly trends"].tap()  //dismiss info window
                sleep(2)
                
                app.buttons.matchingIdentifier("").elementBoundByIndex(0).tap()
                sleep(2)
                app.tables.staticTexts["Average Delay (for delayed flights)"].tap()
                sleep(2)
            }*/
            
            app.buttons["Close"].tap()
            
            
        }
        
       // app.terminate()
    }
    
    func test_013_AirplaneDelays(){
        
        ActionUtils.clearCache(app, test: self)
        app.tables.staticTexts["Airline On Time Performance"].tap()
        ActionUtils.scrollAndTap("Airline Airport App", application: app, test: self)
    
        for i in 0..<2 {
            ActionUtils.scrollAndTap("3. Delays by Flight Route", application: app, test: self)
            ActionUtils.verifyAndTapElement("Page by:", application: app, type: .staticText, test: self, tap: false)
            
            if(i == 1 && ENABLE_MANI){
                app.buttons["ABE:Lehigh Valley International"].tap()
                ActionUtils.scrollAndTap("ATL:Hartsfield-Jackson", application: app, test: self)
                sleep(2)
                
                app.scrollViews.otherElements[", To, Row Headings"].press(forDuration: LONG_PRESS)
                ActionUtils.verifyAndTapElement("Sort", application: app, type: .button, test: self, tap: true)
                sleep(2)
                
                app.staticTexts["Page by:"].tap() //Dismiss Sorting 
                app.scrollViews.otherElements[", ATL-HOU. This is a link"].tap()
               // app.scrollViews.otherElements[", bluearrow. This is a link"].tap()
                
                ActionUtils.verifyAndTapElement("Performance by Hour & Day", application: app, type: .staticText, test: self, tap: false)
            }
            app.buttons["Close"].tap()
            
        }
        
       // app.terminate()
    }
    
    func test_014_AirplaneRoute(){
    
        ActionUtils.clearCache(app, test: self)
        app.tables.staticTexts["Airline On Time Performance"].tap()
        ActionUtils.scrollAndTap("Airline Airport App", application: app, test: self)
        
        for i in 0..<2 {
            ActionUtils.scrollAndTap("5. Flight Route Analysis", application: app, test: self)
            
            if (i == 0){
                ActionUtils.verifyAndTapElement("Apply", application: app, type: .button, test: self, tap: true)
            }
            ActionUtils.verifyAndTapElement("Performance by Hour & Day", application: app, type: .staticText, test: self, tap: false)
            
            if (i == 1 && ENABLE_MANI){
                app.buttons["Filter"].tap()
                ActionUtils.verifyAndTapElement("Search", application: app, type: .button, test: self, tap: true)
                ActionUtils.verifyAndTapElement("Search for more elements", application: app, type: .searchField, test: self, tap: true)
                app.searchFields["Search for more elements"].typeText("ABE")
                ActionUtils.verifyAndTapElement("ABE-ATL", application: app, type: .staticText, test: self, tap: true)
                
                app.buttons["Apply"].tap()

                
                ActionUtils.verifyAndTapElement("Lehigh Valley International to Hartsfield-Jackson", application: app, type: .staticText, test: self, tap: false)
                
                app.scrollViews.otherElements["Performance by Carrier, not selected, Link Bar."].tap()
                ActionUtils.verifyAndTapElement("Delta Air Lines Inc., selected, Link Bar.", application: app, type: .other, test: self, tap: true)
                ActionUtils.verifyAndTapElement("All Flights. All Carriers., not selected, Link Bar.", application: app, type: .other, test: self, tap: true)
                ActionUtils.verifyAndTapElement("selector, currently selected item is:Sunday:Sun", application: app, type: .other, test: self, tap: true)
                app.tables.staticTexts["Friday:Fri"].tap()
                
            }
            
            app.buttons["Close"].tap()

        }
        
        //app.terminate()
    }
 
    
    func test_015_BankPerformanceWithoutCache(){
        ActionUtils.clearCache(app, test: self)
        app.tables.staticTexts["Banking Performance"].tap()
        ActionUtils.scrollAndTap("Bank Performance App", application: app, test: self)

            
        //Execute Home Page
        ActionUtils.scrollAndTap("Bank Performance App", application: app, test: self)
        //Link to Top 100 Bank
        ActionUtils.verifyAndTapElement("Top 100 Link. This is a Link. ", application: app, type: .image, test: self, tap: true)
        
        //Change selector
        ActionUtils.verifyAndTapElement("Rank by:", application: app, type: .staticText, test: self, tap: false)
        app.scrollViews.otherElements["selector, currently selected item is:Total Assets"].tap()
        app.tables.staticTexts["Total Deposits"].tap()
        sleep(2)
        //Link to Bank Details
        app.scrollViews.otherElements[", JPMORGAN CHASE BANK, NATIONAL ASSOCIATION. This is a link"].tap()
        ActionUtils.verifyAndTapElement("Filter", application: app, type: .button, test: self, tap: false)
     
        //Change selector
        app.otherElements["Balance Sheet, not selected, Button Bar."].tap()
       
        //Trigger info window
        ActionUtils.verifyAndTapElement("LOANS_LINK. This is a Link. ", application: app, type: .image, test: self, tap: true)
        sleep(2)
        app.scrollViews.images["LOANS_LINK. This is a Link. "].tap()
        
        //Reprompt
        app.buttons["Filter"].tap()
        ActionUtils.verifyAndTapElement("Search", application: app, type: .button, test: self, tap: true)
        ActionUtils.verifyAndTapElement("Search for more elements", application: app, type: .searchField, test: self, tap: true)
        app.searchFields["Search for more elements"].typeText("1st")
        ActionUtils.verifyAndTapElement("1ST ADVANTAGE BANK:SAINT PETERS, MO", application: app, type: .staticText, test: self, tap: true)
        
        app.buttons["Apply"].tap()
        ActionUtils.verifyAndTapElement("Reporting Quarter: 2011 Q1, 1ST ADVANTAGE BANK", application: app, type: .other, test: self, tap: false)
        
        //Link to Leaders&Laggards
       // ActionUtils.printDescription(app)
        
        ActionUtils.verifyAndTapElement("Image968. This is a Link. ", application: app, type: .image, test: self, tap: true)
       // app.images["Image966. This is a Link. "].tap()
        ActionUtils.verifyAndTapElement("L&L. This is a Link. ", application: app, type: .image, test: self, tap: true)
        
        //ActionUtils.verifyAndTapElement("Leaders & Laggards Link. This is a Link. ", application: app, type: .Image, test: self)
        ActionUtils.verifyAndTapElement("Compare 2011 Q1 to", application: app, type: .staticText, test: self, tap: false)
        
        //Trigger info window
        
        let position = CGPoint(x: 454, y: 665)
        ActionUtils.tapAtPosition(position: position, element: app)
        //Link Drill
        ActionUtils.verifyAndTapElement("Financial Institution: MORGAN STANLEY PRIVATE BANK, NATIONAL ASSOCIATION, bluearrow. This is a link", application: app, type: .other, test: self, tap: true)
        
      //  ActionUtils.verifyAndTapElement(", MORGAN STANLEY PRIVATE BANK, NATIONAL ASSOCIATION. This is a link", application: app, type: .Other, test: self, tap: true)
        
        ActionUtils.verifyAndTapElement("Filter", application: app, type: .button, test: self, tap: false)
        app.navigationBars["Bank Details"].children(matching: .button).matching(identifier: "Back").element(boundBy: 0).tap()
        app.navigationBars.children(matching: .button).matching(identifier: "Back").element(boundBy: 0).tap()
        app.navigationBars.children(matching: .button).matching(identifier: "Back").element(boundBy: 0).tap()
        app.navigationBars.children(matching: .button).matching(identifier: "Back").element(boundBy: 0).tap()
        app.buttons["Close"].tap()
            
            
        
       // app.terminate()
    }
 
    func test_016_BankPerformanceCache(){

        ActionUtils.scrollAndTap("Banking Performance", application: app, test: self)
        ActionUtils.scrollAndTap("Bank Performance App", application: app, test: self)
        
        
        //Execute Home Page
        ActionUtils.scrollAndTap("Bank Performance App", application: app, test: self)
        //Link to Top 100 Bank
        ActionUtils.verifyAndTapElement("Top 100 Link. This is a Link. ", application: app, type: .image, test: self, tap: true)
        
        //Change selector
        ActionUtils.verifyAndTapElement("Rank by:", application: app, type: .staticText, test: self, tap: false)
        app.scrollViews.otherElements["selector, currently selected item is:Total Assets"].tap()
        app.tables.staticTexts["Total Deposits"].tap()
        sleep(2)
        //Link to Bank Details
        app.scrollViews.otherElements[", JPMORGAN CHASE BANK, NATIONAL ASSOCIATION. This is a link"].tap()
        ActionUtils.verifyAndTapElement("Filter", application: app, type: .button, test: self, tap: false)
        
        //Change selector
        app.otherElements["Balance Sheet, not selected, Button Bar."].tap()
        
        //Trigger info window
        ActionUtils.verifyAndTapElement("LOANS_LINK. This is a Link. ", application: app, type: .image, test: self, tap: true)
        sleep(2)
        app.scrollViews.images["LOANS_LINK. This is a Link. "].tap()
        
        //Reprompt
        app.buttons["Filter"].tap()
        ActionUtils.verifyAndTapElement("Search", application: app, type: .button, test: self, tap: true)
        ActionUtils.verifyAndTapElement("Search for more elements", application: app, type: .searchField, test: self, tap: true)
        app.searchFields["Search for more elements"].typeText("1st")
        ActionUtils.verifyAndTapElement("1ST ADVANTAGE BANK:SAINT PETERS, MO", application: app, type: .staticText, test: self, tap: true)
        
        app.buttons["Apply"].tap()
        ActionUtils.verifyAndTapElement("Reporting Quarter: 2011 Q1, 1ST ADVANTAGE BANK", application: app, type: .other, test: self, tap: false)
        
        //Link to Leaders&Laggards
        
        ActionUtils.verifyAndTapElement("Image968. This is a Link. ", application: app, type: .image, test: self, tap: true)
        // app.images["Image966. This is a Link. "].tap()
        ActionUtils.verifyAndTapElement("L&L. This is a Link. ", application: app, type: .image, test: self, tap: true)
        
        //ActionUtils.verifyAndTapElement("Leaders & Laggards Link. This is a Link. ", application: app, type: .image, test: self, tap: true)
        ActionUtils.verifyAndTapElement("Compare 2011 Q1 to", application: app, type: .staticText, test: self, tap: false)
        
      //  let element = app.descendants(matching: .any).matching(identifier: "This is a Bubble chart..").element
        
        //Trigger info window
        //ActionUtils.printDescription(app)

        
        let position = CGPoint(x: 454, y: 665)
        ActionUtils.tapAtPosition(position: position, element: app)
        //Link Drill
        //ActionUtils.printDescription(app)
        //ActionUtils.verifyAndTapElement("This is a Horizontal bar chart..", application: app, type: .other, test: self, tap: false)
        
        ActionUtils.verifyAndTapElement("Financial Institution: MORGAN STANLEY PRIVATE BANK, NATIONAL ASSOCIATION, bluearrow. This is a link", application: app, type: .other, test: self, tap: true)
        ActionUtils.verifyAndTapElement("Filter", application: app, type: .button, test: self, tap: false)
        app.navigationBars["Bank Details"].children(matching: .button).matching(identifier: "Back").element(boundBy: 0).tap()
        app.navigationBars.children(matching: .button).matching(identifier: "Back").element(boundBy: 0).tap()
        app.navigationBars.children(matching: .button).matching(identifier: "Back").element(boundBy: 0).tap()
        app.navigationBars.children(matching: .button).matching(identifier: "Back").element(boundBy: 0).tap()
        app.buttons["Close"].tap()
        
        
        
      //  app.terminate()
    }
    
    func test_017_LinkFromGraphToPrompt(){
        //ActionUtils.clearCache(app, test: self)
        ActionUtils.scrollAndTap("Banking Performance", application: app, test: self)
        ActionUtils.scrollAndTap("Subdocuments", application: app, test: self)
        ActionUtils.scrollAndTap("Origional Version", application: app, test: self)
        
        for i in 0..<2 {
            ActionUtils.scrollAndTap("Top 100 Banks_Graph", application: app, test: self)
            ActionUtils.verifyAndTapElement("Rank by:", application: app, type: .staticText, test: self, tap: false)
           // ActionUtils.printDescription(app)
            if (i == 1 && ENABLE_MANI){
                let location = CGPoint(x: 395, y: 685)
                ActionUtils.tapAtPosition(position: location, element: app)
                ActionUtils.verifyAndTapElement("Filter", application: app, type: .button, test: self, tap: false)
            }
            
            app.buttons["Close"].tap()
        }
    }
    
    func test_018_CFO(){
        
        ActionUtils.clearCache(app, test: self)
        app.tables.staticTexts["CFO"].tap()
        ActionUtils.scrollAndTap("Mobile", application: app, test: self)
        ActionUtils.scrollAndTap("Ipad", application: app, test: self)
        ActionUtils.scrollAndTap("CFO Dashboard", application: app, test: self)
        
        for _ in 0..<2 {
            
            //Execute CFO Dashboard
            ActionUtils.scrollAndTap("CFO Dashboard", application: app, test: self)
            
            //Link to Segment Perf
            ActionUtils.verifyAndTapElement("Image415. This is a Link. ", application: app, type: .image, test: self, tap: true)
            
            //Link to Trends
            ActionUtils.verifyAndTapElement("Financial Summary 2011", application: app, type: .staticText, test: self, tap: false)
            app.images["Image116. This is a Link. "].tap()
            
            //Change Selector
            ActionUtils.verifyAndTapElement("selector, currently selected item is:Bar Chart by Time", application: app, type: .other, test: self, tap: true)
            app.tables.staticTexts["Bar Chart by Line of Business"].tap()
            sleep(2)
            
            //Link to Investment Analysis
            app.images["Image113. This is a Link. "].tap()
            
            //Link to Trading Desk
            ActionUtils.verifyAndTapElement("Image150. This is a Link. ", application: app, type: .image, test: self, tap: true)
            
            //Back
            ActionUtils.verifyAndTapElement("Trading Desk", application: app, type: .staticText, test: self, tap: false)
            app.buttons.matching(identifier: "Back").element(boundBy: 0).tap()
            sleep(2)
            
            //Link to EPS Simulation
            app.images["Image118. This is a Link. "].tap()
            ActionUtils.verifyAndTapElement("EPS Scenarios", application: app, type: .staticText, test: self, tap: false)
            
            app.buttons["Close"].tap()
            
            
            
        }
        
     //   app.terminate()
        
    }
    

    
}
