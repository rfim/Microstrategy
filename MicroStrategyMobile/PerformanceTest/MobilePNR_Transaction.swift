//
//  MobilePNR_Transaction.swift
//  MicroStrategyMobile
//
//  Created by Administrator on 11/26/15.
//  Copyright © 2015 MicroStrategy Inc. All rights reserved.
//

import XCTest

class MobilePNR_Transaction: XCTestCase {
    let app = XCUIApplication()
    override func setUp() {
        super.setUp()
        
        // Put setup code here. This method is called before the invocation of each test method in the class.
        
        // In UI tests it is usually best to stop immediately when a failure occurs.
        continueAfterFailure = false
        // UI tests must launch the application that they test. Doing this in setup will make sure it happens for each test method.
        XCUIApplication().launch()

        // In UI tests it’s important to set the initial state - such as interface orientation - required for your tests before they run. The setUp method is a good place to do this.
    }
    
    override func tearDown() {
        // Put teardown code here. This method is called after the invocation of each test method in the class.
        app.terminate()
        super.tearDown()
    }
    
 /*   func test_001_Transaction1(){
        ActionUtils.scrollAndTap("Transactions(SingleGridSource=False)", application: app, test: self)
        ActionUtils.scrollAndTap("Test Suite", application: app, test: self)
        ActionUtils.scrollAndTap("Offline", application: app, test: self)
        ActionUtils.scrollAndTap("Grid-MarkRowsForSelection-Checked_2000row_Refresh", application: app, test: self)
        
        ActionUtils.verifyAndTapElement("Submit", application: app, type: .button, test: self, tap: false)
        
        ActionUtils.printDescription(app)
        
        for i in 0..<5{
            let location = CGPoint(x: 196 + 34 * CGFloat(i), y: 861)
            ActionUtils.tapAtPosition(position: location, element: app)
            sleep(1)
        }
        
        app.buttons["Submit"].tap()
        sleep(5)
        app.buttons["Close"].tap()
        
      //  app.terminate()
        
    }
 
    
    func test_002_Transaction2(){
        ActionUtils.scrollAndTap("Transactions(SingleGridSource=False)", application: app, test: self)
        ActionUtils.scrollAndTap("Test Suite", application: app, test: self)
        ActionUtils.scrollAndTap("Perf_Used_2000row_Refresh", application: app, test: self)
        
        ActionUtils.verifyAndTapElement("Submit", application: app, type: .button, test: self, tap: false)
        
        //ActionUtils.printDescription(app)
        
        
        for i in 1..<6 {
            
            var tapString = "jinAttr1: \(i), jinFloat, 10"
            ActionUtils.verifyAndTapElement(tapString, application: app, type: .other, test: self, tap: true)
            sleep(1)
        }
        
        app.buttons["Submit"].tap()
        sleep(5)
        
        app.buttons["Close"].tap()
        
        ActionUtils.resetTransaction(app, test: self)
        
       // app.terminate()
    }*/
    
    func test_003_Signature(){
        ActionUtils.scrollAndTap("Transactions(SingleGridSource=False)", application: app, test: self)
        ActionUtils.scrollAndTap("Test Suite", application: app, test: self)
        ActionUtils.scrollAndTap("Data Input Control", application: app, test: self)
        ActionUtils.scrollAndTap("Signature Capture", application: app, test: self)
        ActionUtils.scrollAndTap("1:1_Default_Attribute_2000Rows", application: app, test: self)
        
        ActionUtils.verifyAndTapElement("Submit", application: app, type: .button, test: self, tap: false)
        ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
        //ActionUtils.printDescription(app)

        
        for i in 1..<6 {
            
            var tapString = "jinAttr1 ID: \(i), Addison"
            let element = app.otherElements[tapString].children(matching: .other).matching(identifier: "").element
            element.tap()
            sleep(1)

            ActionUtils.verifyAndTapElement("Clear", application: app, type: .button, test: self, tap: false)
            //ActionUtils.printDescription(app)
            
            let startCoordinate = app.coordinate(withNormalizedOffset: CGVector(dx: 0.0, dy: 0.0)).withOffset(CGVector(dx: app.frame.size.width/2, dy: app.frame.size.height/2))
            let endCoordinate = startCoordinate.withOffset(CGVector(dx: 150.0, dy: 0))
            
            startCoordinate.press(forDuration: 1, thenDragTo: endCoordinate)
            
            app.buttons["Done"].tap()
            sleep(1)
            
        }
        _ = self.dataFromRemoteEndpoint("clearLog", args: "")
        app.buttons["Submit"].tap()
        sleep(5)
        
        app.buttons["Close"].tap()
        
        ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
        
        ActionUtils.resetTransaction(app, test: self)
        
      //  app.terminate()
    }
 
    func test_004_TXNTable(){
        ActionUtils.scrollAndTap("Transactions(SingleGridSource=False)", application: app, test: self)
        ActionUtils.scrollAndTap("Transaction Perf&Stab", application: app, test: self)
        ActionUtils.scrollAndTap("Table", application: app, test: self)
        
        ActionUtils.scrollAndTap("Basic_00", application: app, test: self)
        ActionUtils.verifyAndTapElement("Submit", application: app, type: .button, test: self, tap: false)
        ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
        
        app.tables.staticTexts["Date"].tap()
        app.datePickers.pickerWheels.element(boundBy: 0).adjust(toPickerWheelValue: "August")
        sleep(1)
        //ActionUtils.printDescription(app)
        ActionUtils.verifyAndTapElement("dismiss popup", application: app, type: .other, test: self, tap: true)
        //app.tap()
        
        app.tables.staticTexts["Time"].tap()
        app.datePickers.pickerWheels.element(boundBy: 2).adjust(toPickerWheelValue: "PM")
        
        sleep(1)
        
        ActionUtils.verifyAndTapElement("dismiss popup", application: app, type: .other, test: self, tap: true)
        
        _ = self.dataFromRemoteEndpoint("clearLog", args: "")

        app.buttons["Submit"].tap()
        
        app.buttons["Close"].tap()
        ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
        ActionUtils.resetTransaction(app, test: self)
        
      //  app.terminate()
    }
 
    func test_005_DDIC(){
        ActionUtils.scrollAndTap("Transactions(SingleGridSource=False)", application: app, test: self)
        ActionUtils.scrollAndTap("Test Suite", application: app, test: self)
        ActionUtils.scrollAndTap("Data Input Control", application: app, test: self)
        ActionUtils.scrollAndTap("DDIC", application: app, test: self)
        
        //ActionUtils.printDescription(app)
        ActionUtils.scrollAndTap("Attribute-Default_KPI_SmallList", application: app, test: self)

        ActionUtils.verifyAndTapElement("Submit", application: app, type: .button, test: self, tap: false)
        ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
        //ActionUtils.printDescription(app)
        
        
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
        _ = self.dataFromRemoteEndpoint("clearLog", args: "")
        app.buttons["Submit"].tap()
        sleep(7)
        
        app.buttons["Close"].tap()
        ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
        ActionUtils.resetTransaction(app, test: self)
        
      //  app.terminate()
    }

/*    func test_006_TransactionInFG(){
        ActionUtils.scrollAndTap("Transactions(SingleGridSource=False)", application: app, test: self)
        ActionUtils.scrollAndTap("Test Suite", application: app, test: self)
        ActionUtils.scrollAndTap("Perf_Used_Field_Refresh", application: app, test: self)
        
        ActionUtils.verifyAndTapElement("Submit", application: app, type: .button, test: self, tap: false)
        
        //ActionUtils.printDescription(app)
        app.switches.element.tap()
        sleep(1)
        
        app.buttons["stepper plus"].tap()
        sleep(1)
        
        
        app.staticTexts["30"].tap()
        sleep(1)
        
        app.buttons["Discard Changes"].tap()
        sleep(2)
        app.switches.element.tap()
        sleep(1)
        
        app.buttons["stepper plus"].tap()
        sleep(1)
        
        
        app.staticTexts["30"].tap()
        sleep(1)
        
        app.buttons["Submit"].tap()
        sleep(7)
        
        
        
        app.buttons["Close"].tap()
        
     //   app.terminate()
    }*/
    
    func test_007_Survey(){
        ActionUtils.scrollAndTap("Transactions(SingleGridSource=False)", application: app, test: self)
        ActionUtils.scrollAndTap("Transaction Perf&Stab", application: app, test: self)
        ActionUtils.scrollAndTap("940NewFeature", application: app, test: self)
        ActionUtils.scrollAndTap("Survey", application: app, test: self)
        ActionUtils.scrollAndTap("1.2_Simple-AllFields_KPI", application: app, test: self)
        
        sleep(5)
        
        if (app.buttons["Apply"].exists){
            app.buttons["Apply"].tap()
        }
        
        ActionUtils.verifyAndTapElement("Submit", application: app, type: .button, test: self, tap: false)
        ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
        //ActionUtils.printDescription(app)
        
        app.textFields.element(boundBy: 2).tap()
        app.typeText("abc@microstrategy.com")
        app.buttons["Hide keyboard"].tap()
        sleep(1)
        
        app.scrollViews.staticTexts["Less than 1 year"].tap()
        sleep(1)
        _ = self.dataFromRemoteEndpoint("clearLog", args: "")
        sleep(5)
        app.buttons["Submit"].tap()
        
        sleep(7)
        
        app.buttons["Close"].tap()
        ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
      //  app.terminate()
    }
    
    func test_008_1ClickSubmit(){
        ActionUtils.scrollAndTap("Transactions(SingleGridSource=False)", application: app, test: self)
        ActionUtils.scrollAndTap("Transaction Perf&Stab", application: app, test: self)
        ActionUtils.scrollAndTap("940NewFeature", application: app, test: self)
        ActionUtils.scrollAndTap("One-Click Submit", application: app, test: self)
        ActionUtils.scrollAndTap("All_Field_Insert_UITesting", application: app, test: self)
        ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
        ActionUtils.verifyAndTapElement("Submit", application: app, type: .button, test: self, tap: true)
        ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
        
        sleep(5)
        
        app.buttons["Close"].tap()
     //   app.terminate()
        
      //  app.terminate()
    }
    
 /*   func test_009_PhotoUploader() {
        // Use recording to get started writing UI tests.
        // Use XCTAssert and related functions to verify your tests produce the correct results.
        
        ActionUtils.scrollAndTap("Photo Widget", application: app, test: self)
        ActionUtils.scrollAndTap("Photo Uploader", application: app, test: self)
        ActionUtils.scrollAndTap("Test Suite", application: app, test: self)
        ActionUtils.scrollAndTap("Basic Function", application: app, test: self)
        ActionUtils.scrollAndTap("1. Insert-Default", application: app, test: self)
        
        sleep(5)
        
        if (app.alerts.element.exists){
            app.buttons["OK"].tap()
        }
        
        //ActionUtils.printDescription(app)
        app.otherElements["This is a ImageUploaderWidgetView Widget."].tap()
        
        //ActionUtils.printDescription(app)
        
        app.buttons["Choose Existing Photo"].tap()
        sleep(3)
        if (app.alerts.element.exists){
            app.buttons["OK"].tap()
        }
        app.buttons["Album"].tap()
        app.staticTexts["TestUsed"].tap()
        
       // ActionUtils.printDescription(app)
        
        app.cells.elementBoundByIndex(0).tap()
        app.buttons["Done"].tap()
       // ActionUtils.printDescription(app)
        app.navigationBars["Gallery"].childrenMatchingType(.Button).elementBoundByIndex(0).tap()
       // ActionUtils.verifyAndTapElement("Use", application: app, type: .Button, test: self, tap: true)
        sleep(2)
        
        app.buttons["Submit"].tap()
        sleep(2)
        if(app.alerts.element.exists){
            app.buttons["Continue"].tap()
        }
        
        sleep(5)
        
        app.buttons["Discard Changes"].tap()
        
        app.otherElements["This is a ImageUploaderWidgetView Widget."].tap()
        
        //ActionUtils.printDescription(app)
        
        app.buttons["Choose Existing Photo"].tap()
        sleep(3)
        if (app.alerts.element.exists){
            app.buttons["OK"].tap()
        }
        app.buttons["Album"].tap()
        app.staticTexts["TestUsed"].tap()
        for var i = 0; i < 10; ++i{
            
            app.cells.elementBoundByIndex(UInt(i)).tap()
            
        }
        app.buttons["Done"].tap()
        // ActionUtils.verifyAndTapElement("Use", application: app, type: .Button, test: self, tap: true)
        app.navigationBars["Gallery"].childrenMatchingType(.Button).elementBoundByIndex(0).tap()
        sleep(2)
        app.buttons["Submit"].tap()
        
        if(app.alerts.element.exists){
            app.buttons["Continue"].tap()
        }
        
        sleep(5)

        
        app.buttons["Close"].tap()
    }*/
    
    func test_011_OfflineTXN1(){
        
        ActionUtils.scrollAndTap("Transactions(SingleGridSource=False)", application: app, test: self)
        ActionUtils.scrollAndTap("Test Suite", application: app, test: self)
        ActionUtils.scrollAndTap("Perf_Used_Field_Refresh", application: app, test: self)

        
        ActionUtils.verifyAndTapElement("Submit", application: app, type: .button, test: self, tap: false)
        ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
       // ActionUtils.printDescription(app)
        
        app.switches.element.tap()
        sleep(1)
        app.buttons["stepper minus"].tap()
        sleep(1)
        
        app.buttons["Submit"].tap()
        sleep(5)
        
        app.buttons["Close"].tap()
        ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
        ActionUtils.changeOfflineMode(app, test: self)
        ActionUtils.scrollAndTap("Perf_Used_Field_Refresh", application: app, test: self)
        ActionUtils.verifyAndTapElement("Submit", application: app, type: .button, test: self, tap: false)
        ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
        app.switches.element.tap()
        sleep(1)
        app.buttons["stepper minus"].tap()
        sleep(1)
        app.buttons["Submit"].tap()
        
        sleep(5)
        app.buttons["Close"].tap()
        
        ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
        //ActionUtils.transactionQueue(app, test: self)
        ActionUtils.changeOfflineMode(app, test: self)
        
        sleep(5)
        
        ActionUtils.transactionQueue(app, test: self)
        ActionUtils.verifyDisappearWithTimeout("Perf_Used_Field_Refresh", application: app, type: .staticText, test: self, timeout: 60)
        ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
      //  ActionUtils.printDescription(app)
        
        

    }
    
    func test_012_OfflineTXN2(){
        ActionUtils.scrollAndTap("Transactions(SingleGridSource=False)", application: app, test: self)
        ActionUtils.scrollAndTap("Test Suite", application: app, test: self)
        ActionUtils.changeOfflineMode(app, test: self)
        ActionUtils.scrollAndTap("Perf_Used_Field_Refresh", application: app, test: self)
        
        
        ActionUtils.verifyAndTapElement("Submit", application: app, type: .button, test: self, tap: false)
        ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
        
        for i in 0..<10 {
            app.switches.element.tap()
            sleep(1)
            
            if (i % 2 == 0){
                app.buttons["stepper plus"].tap()
            }else{
                app.buttons["stepper minus"].tap()
            }
            sleep(1)
            app.buttons["Submit"].tap()
            sleep(5)
            ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
            
        }
        
        app.buttons["Close"].tap()
        
        ActionUtils.changeOfflineMode(app, test: self)
        
        sleep(10)
        
        ActionUtils.transactionQueue(app, test: self)
        ActionUtils.verifyDisappearWithTimeout("Perf_Used_Field_Refresh", application: app, type: .staticText, test: self, timeout: 60)
        
        ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
        sleep(5)
        
    }
    
    func test_013_SmartClient(){
        ActionUtils.scrollAndTap("Transactions(SingleGridSource=False)", application: app, test: self)
        ActionUtils.scrollAndTap("Transaction Perf&Stab", application: app, test: self)
        ActionUtils.scrollAndTap("Polaris", application: app, test: self)
        ActionUtils.scrollAndTap("SmartClient", application: app, test: self)
        ActionUtils.scrollAndTap("20DocTo1Dataset", application: app, test: self)
        
        ActionUtils.scrollAndTap("Field_Default_Update_1", application: app, test: self)
        ActionUtils.verifyAndTapElement("Submit", application: app, type: .button, test: self, tap: false)
        app.buttons["Close"].tap()
        ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
        ActionUtils.scrollAndTap("Field_Default_Update_0", application: app, test: self)
        ActionUtils.verifyAndTapElement("Submit", application: app, type: .button, test: self, tap: false)
        //ActionUtils.printDescription(app)
        app.buttons["Close"].tap()
        ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
        ActionUtils.changeOfflineMode(app, test: self)
        
        ActionUtils.scrollAndTap("Field_Default_Update_0", application: app, test: self)
        ActionUtils.verifyAndTapElement("Submit", application: app, type: .button, test: self, tap: false)
        //ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
        for _ in 0..<6 {
            app.switches.element.tap()
            sleep(1)
            //ActionUtils.printDescription(app)
            app.buttons["Submit"].tap()
            sleep(5)
        }
        
        app.buttons["Close"].tap()
        ActionUtils.scrollAndTap("Field_Default_Update_0", application: app, test: self)
        ActionUtils.verifyAndTapElement("Submit", application: app, type: .button, test: self, tap: false)
        _ = self.dataFromRemoteEndpoint("clearLog", args: "")
        for _ in 0..<5 {

            app.switches.element.tap()
            sleep(1)
            app.buttons["Submit"].tap()
            sleep(5)
            
            app.buttons["Close"].tap()
            
            ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
            
            ActionUtils.scrollAndTap("Field_Default_Update_0", application: app, test: self)
            ActionUtils.verifyAndTapElement("Submit", application: app, type: .button, test: self, tap: false)
            //_ = self.dataFromRemoteEndpoint("clearLog", args: "")
            ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
        }
        
        app.buttons["Close"].tap()
        ActionUtils.scrollAndTap("Field_Default_Update_1", application: app, test: self)
        ActionUtils.verifyAndTapElement("Submit", application: app, type: .button, test: self, tap: false)
        app.buttons["Close"].tap()
        ActionUtils.sendPerformanceLog(IPAddress: IPAddress, test: self, app: app)
        ActionUtils.changeOfflineMode(app, test: self)
        
        sleep(10)
        
        ActionUtils.transactionQueue(app, test: self)
        ActionUtils.verifyDisappearWithTimeout("Perf_Used_Field_Refresh", application: app, type: .staticText, test: self, timeout: 60)
        _ = self.dataFromRemoteEndpoint("clearLog", args: "")
    }
    
}
