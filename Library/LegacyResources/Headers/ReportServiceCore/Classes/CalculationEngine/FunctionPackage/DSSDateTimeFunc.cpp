//
//  DSSDateTimeFunc.cpp
//  ReportServiceCore
//
//  Created by Jia Zhang on 4/22/13.
//  Copyright (c) 2013 MicroStrategy, Inc. All rights reserved.
//

#include "DSSDateTimeFunc.h"

//#include "StdAfx.h"
#include <time.h>
#include "DSSDateTime.h"
#include "DSSTimeUtil.h"
#include "DFCDataConversion.h"
#include "DSSFunctionUtility.h"

DSSDateTimeFunc::DSSDateTimeFunc()
: mfTrimString(true)
{
}


DSSDateTimeFunc::~DSSDateTimeFunc()
{
}

int DSSDateTimeFunc::Calculate (Int32 nParam, CComVariant *pParam, CComVariant *pResult, DSSDataFlag *pFlag)
{
  	if(!pParam || !pResult || !pFlag)
		return E_POINTER;
    
	if ( (pParam->vt & (VT_ARRAY|VT_VARIANT)) != (VT_ARRAY|VT_VARIANT))
		return E_FAIL;
    
	int hr = S_OK;
	if (mFunctionIndex != DssFuncCurrentDate && mFunctionIndex != DssFuncCurrentTime &&
		mFunctionIndex != DssFuncCurrentDateTime)
	{
		if (nParam < 1)
			return E_FAIL;
	}
    
	switch (mFunctionIndex)
	{
        case DssFuncCurrentDate:
		{
			// only return the date, time will be 12:00 AM
			MDataType::DSSDateTime lCurDateTime = MDataType::DSSDateTime::Now();
			MDataType::DSSDateTime lCurDate(lCurDateTime.GetYear(), lCurDateTime.GetMonth(), lCurDateTime.GetDay(), 0, 0, 0, 0);
			hr = DFCDataConversion::ConvertToVariant((BYTE *)&lCurDate, DssDataTypeDate, 0, pResult);
			CHECK_HR;
		}
            break;            
            
        case DssFuncDayOfMonth:
		{
			CComVariant v1;
			getVariant(pParam, 0, v1);
            
			Int32 lDayOfMonth = 0;
            
            if (VT_DATE == v1.vt)
            {
                MDataType::DSSDateTime lDateTime;
                int lcbData = sizeof(MDataType::DSSDateTime);
                hr = DFCDataConversion::ConvertFromVariant(&v1, DssDataTypeDate, (unsigned char *)&lDateTime, &lcbData);
                CHECK_HR;
                
                lDayOfMonth = lDateTime.GetDay();
            }
            else
            {
				// TQMS 657294, support conversion from non-datetime to datetime
				// for other types that not Date/Time, converted into double first
				// its value is days distances from 1899/12/30 as default.
				double lVar = 0;
				Int32 lSizeDouble = sizeof(double);

				hr = DFCDataConversion::ConvertFromVariant(&v1, DssDataTypeDouble, (BYTE*)&lVar, &lSizeDouble);
				if (S_OK != hr)
				{
					*pFlag = DssDataInvalid;
					return S_OK;
				}
             
				// convert days into date and then get day of month
				MDataType::DSSDateTime lDate(lVar);
				lDayOfMonth = lDate.GetDay();
			} 
            
			hr = DFCDataConversion::ConvertToVariant((BYTE *)&lDayOfMonth, DssDataTypeInteger, 0, pResult);
			CHECK_HR;
		}
            break;
          
        case DssFuncDayOfWeek:
		{
			CComVariant v1;
			getVariant(pParam, 0, v1);
            
			Int32 lDayOfWeek = 0;
			/*CComQIPtr<IDSSTime, &IID_IDSSTime> lpTime;
			if (v1.vt == VT_DISPATCH)
				lpTime = CComQIPtr<IDSSTime, &IID_IDSSTime>(v1.pdispVal);
            
			if (lpTime)
			{
				hr = lpTime->get_DayOfWeek((EnumDSSDayOfWeek *)&lDayOfWeek);
				CHECK_HR;
			}*/
            
            if (VT_DATE == v1.vt)
            {
                MDataType::DSSDateTime lDateTime;
                int lcbData = sizeof(MDataType::DSSDateTime);
                hr = DFCDataConversion::ConvertFromVariant(&v1, DssDataTypeDate, (unsigned char *)&lDateTime, &lcbData);
                CHECK_HR;
                
                lDayOfWeek = lDateTime.GetDayOfWeek();
            }
			else
			{
				double lVar = 0;
				Int32 lSizeDouble = sizeof(double);
                
				hr = DFCDataConversion::ConvertFromVariant(&v1, DssDataTypeDouble, (BYTE*)&lVar, &lSizeDouble);
				if (S_OK != hr)
				{
					*pFlag = DssDataInvalid;
					return S_OK;
				}
                
				MDataType::DSSDateTime lDate(lVar);
				lDayOfWeek = lDate.GetDayOfWeek();
			}
            
			lDayOfWeek++;		// start from 1, values will be [1, 2, 3, 4, 5, 6, 7]
			hr = DFCDataConversion::ConvertToVariant((BYTE *)&lDayOfWeek, DssDataTypeInteger, 0, pResult);
			CHECK_HR;
		}
            break;
 
        case DssFuncDayOfYear:
		{
			CComVariant v1;
			getVariant(pParam, 0, v1);
            
			/*CComQIPtr<IDSSTime, &IID_IDSSTime> lpTime;
			if (v1.vt == VT_DISPATCH)
				lpTime = CComQIPtr<IDSSTime, &IID_IDSSTime>(v1.pdispVal);*/
            
			struct tm lTime;
			memset(&lTime, 0, sizeof(lTime));
            
			Int32 lDayOfYear = 0;
			/*if (lpTime)
			{
				if (mktime(lpTime, lTime) != -1)
					lDayOfYear = lTime.tm_yday + 1;
			}*/
            if (VT_DATE == v1.vt)
            {
                MDataType::DSSDateTime lDateTime;
                int lcbData = sizeof(MDataType::DSSDateTime);
                hr = DFCDataConversion::ConvertFromVariant(&v1, DssDataTypeDate, (unsigned char *)&lDateTime, &lcbData);
                CHECK_HR;
                
                if (mktime(&lDateTime, lTime) != -1)
					lDayOfYear = lTime.tm_yday + 1;
            }
			else
			{
				double lVar = 0;
				Int32 lSizeDouble = sizeof(double);
                
				hr = DFCDataConversion::ConvertFromVariant(&v1, DssDataTypeDouble, (BYTE*)&lVar, &lSizeDouble);
				if (S_OK != hr)
				{
					*pFlag = DssDataInvalid;
					return S_OK;
				}
                
				MDataType::DSSDateTime lDate(lVar);
				if (mktime(&lDate, lTime) != -1)
					lDayOfYear = lTime.tm_yday + 1;
			}
            
			hr = DFCDataConversion::ConvertToVariant((BYTE *)&lDayOfYear, DssDataTypeInteger, 0, pResult);
			CHECK_HR;
		}
            break;
            
            // get week of the year
        case DssFuncWeek:
		{
			CComVariant v1;
			getVariant(pParam, 0, v1);
            
			/*CComQIPtr<IDSSTime, &IID_IDSSTime> lpTime;
			if (v1.vt == VT_DISPATCH)
				lpTime = CComQIPtr<IDSSTime, &IID_IDSSTime>(v1.pdispVal);*/
            
			struct tm lTime;
			memset(&lTime, 0, sizeof(lTime));
            
			Int32 lDayOfYear = 0;
			Int32 lDayOfWeek = 0;
			/*if (lpTime)
			{
				if (mktime(lpTime, lTime) != -1)
					lDayOfYear = lTime.tm_yday + 1;
                
				hr = lpTime->get_DayOfWeek((EnumDSSDayOfWeek *)&lDayOfWeek);
				CHECK_HR;
			}*/
            if (VT_DATE == v1.vt)
            {
                MDataType::DSSDateTime lDateTime;
                int lcbData = sizeof(MDataType::DSSDateTime);
                hr = DFCDataConversion::ConvertFromVariant(&v1, DssDataTypeDate, (unsigned char *)&lDateTime, &lcbData);
                CHECK_HR;
                
                if (mktime(&lDateTime, lTime) != -1)
					lDayOfYear = lTime.tm_yday + 1;
                
				lDayOfWeek = lDateTime.GetDayOfWeek();
            }
			else
			{
				double lVar = 0;
				Int32 lSizeDouble = sizeof(double);
                
				hr = DFCDataConversion::ConvertFromVariant(&v1, DssDataTypeDouble, (BYTE*)&lVar, &lSizeDouble);
				if (S_OK != hr)
				{
					*pFlag = DssDataInvalid;
					return S_OK;
				}
                
				MDataType::DSSDateTime lDate(lVar);
				if (mktime(&lDate, lTime) != -1)
					lDayOfYear = lTime.tm_yday + 1;
                
				lDayOfWeek = lDate.GetDayOfWeek();
			}
            
			// use day of year and current day of week to calculate the weeks past in the year
			Int32 lWeek = (lDayOfYear - (lDayOfWeek + 1))/7 + static_cast<Int32>(ceil(((lDayOfYear - (lDayOfWeek + 1)) % 7) / 7.0)) + 1;
			hr = DFCDataConversion::ConvertToVariant((BYTE *)&lWeek, DssDataTypeInteger, 0, pResult);
			CHECK_HR;
		}
            break;
            
        case DssFuncMonth:
		{
			CComVariant v1;
			getVariant(pParam, 0, v1);
            
			/*CComQIPtr<IDSSTime, &IID_IDSSTime> lpTime;
			if (v1.vt == VT_DISPATCH)
				lpTime = CComQIPtr<IDSSTime, &IID_IDSSTime>(v1.pdispVal);*/
            
			Int32 lMonth = 0;
			/*if (lpTime)
			{
				hr = lpTime->get_Month((EnumDSSMonth *)&lMonth);
				CHECK_HR;
			}*/
            if (VT_DATE == v1.vt)
            {
                MDataType::DSSDateTime lDateTime;
                int lcbData = sizeof(MDataType::DSSDateTime);
                hr = DFCDataConversion::ConvertFromVariant(&v1, DssDataTypeDate, (unsigned char *)&lDateTime, &lcbData);
                CHECK_HR;
                
                lMonth = lDateTime.GetMonth();
            }
			else
			{
				double lVar = 0;
				Int32 lSizeDouble = sizeof(double);
                
				hr = DFCDataConversion::ConvertFromVariant(&v1, DssDataTypeDouble, (BYTE*)&lVar, &lSizeDouble);
				if (S_OK != hr)
				{
					*pFlag = DssDataInvalid;
					return S_OK;
				}
                
				MDataType::DSSDateTime lDate(lVar);
				lMonth = lDate.GetMonth();
			}
            
			hr = DFCDataConversion::ConvertToVariant((BYTE *)&lMonth, DssDataTypeInteger, 0, pResult);
			CHECK_HR;
		}
            break;
            
        case DssFuncQuarter:
		{
			CComVariant v1;
			getVariant(pParam, 0, v1);
            
			/*CComQIPtr<IDSSTime, &IID_IDSSTime> lpTime;
			if (v1.vt == VT_DISPATCH)
				lpTime = CComQIPtr<IDSSTime, &IID_IDSSTime>(v1.pdispVal);*/
            
			EnumDSSMonth lMonth;
			/*if (lpTime)
			{
				hr = lpTime->get_Month(&lMonth);
				CHECK_HR;
			}*/
            if (VT_DATE == v1.vt)
            {
                MDataType::DSSDateTime lDateTime;
                int lcbData = sizeof(MDataType::DSSDateTime);
                hr = DFCDataConversion::ConvertFromVariant(&v1, DssDataTypeDate, (unsigned char *)&lDateTime, &lcbData);
                CHECK_HR;
                
                lMonth = static_cast<EnumDSSMonth>(lDateTime.GetMonth() );
            }
			else
			{
				double lVar = 0;
				Int32 lSizeDouble = sizeof(double);
                
				hr = DFCDataConversion::ConvertFromVariant(&v1, DssDataTypeDouble, (BYTE*)&lVar, &lSizeDouble);
				if (S_OK != hr)
				{
					*pFlag = DssDataInvalid;
					return S_OK;
				}
                
				MDataType::DSSDateTime lDate(lVar);
				lMonth = static_cast<EnumDSSMonth>(lDate.GetMonth() );
			}
            
			Int32 lQuarter = 0;
			if (lMonth >= DssMonthJanuary)
			{
				if (lMonth <= DssMonthMarch)
					lQuarter = 1;
				else if (lMonth <= DssMonthJune)
					lQuarter = 2;
				else if (lMonth <= DssMonthSeptember)
					lQuarter = 3;
				else if (lMonth <= DssMonthDecember)
					lQuarter = 4;
			}
            
			hr = DFCDataConversion::ConvertToVariant((BYTE *)&lQuarter, DssDataTypeInteger, 0, pResult);
			CHECK_HR;
		}
            break;
            
        case DssFuncYear:
		{
			CComVariant v1;
			getVariant(pParam, 0, v1);
            
			/*CComQIPtr<IDSSTime, &IID_IDSSTime> lpTime;
			if (v1.vt == VT_DISPATCH)
				lpTime = CComQIPtr<IDSSTime, &IID_IDSSTime>(v1.pdispVal);*/
            
			Int32 lYear = 0;
			/*if (lpTime)
			{
				hr = lpTime->get_Year(&lYear);
				CHECK_HR;
			}*/
            if (VT_DATE == v1.vt)
            {
                MDataType::DSSDateTime lDateTime;
                int lcbData = sizeof(MDataType::DSSDateTime);
                hr = DFCDataConversion::ConvertFromVariant(&v1, DssDataTypeDate, (unsigned char *)&lDateTime, &lcbData);
                CHECK_HR;
                
				lYear = lDateTime.GetYear();
            }
			else
			{
				double lVar = 0;
				Int32 lSizeDouble = sizeof(double);
                
				hr = DFCDataConversion::ConvertFromVariant(&v1, DssDataTypeDouble, (BYTE*)&lVar, &lSizeDouble);
				if (S_OK != hr)
				{
					*pFlag = DssDataInvalid;
					return S_OK;
				}
                
				MDataType::DSSDateTime lDate(lVar);
				lYear = lDate.GetYear();
			}
            
			hr = DFCDataConversion::ConvertToVariant((BYTE *)&lYear, DssDataTypeInteger, 0, pResult);
			CHECK_HR;
		}
            break;
            
        case DssFuncCurrentDateTime:
		{
			MDataType::DSSDateTime lCurDateTime = MDataType::DSSDateTime::Now();
			hr = DFCDataConversion::ConvertToVariant((BYTE *)&lCurDateTime, DssDataTypeTimeStamp, 0, pResult);
			CHECK_HR;
		}
            break;
            
        case DssFuncCurrentTime:
		{
			// only return the time, date set to be 1899-12-30 as default
			MDataType::DSSDateTime lCurDateTime = MDataType::DSSDateTime::Now();
			MDataType::DSSDateTime lCurTime(1899, 12, 30, lCurDateTime.GetHour(), lCurDateTime.GetMinute(),
                                         lCurDateTime.GetSecond(), lCurDateTime.GetMillisecond() );
            
			hr = DFCDataConversion::ConvertToVariant((BYTE *)&lCurTime, DssDataTypeTime, 0, pResult);
			CHECK_HR;
		}
            break;
            
        case DssFuncHour:
		{
			CComVariant v1;
			getVariant(pParam, 0, v1);
            
			/*CComQIPtr<IDSSTime, &IID_IDSSTime> lpTime;
			if (v1.vt == VT_DISPATCH)
				lpTime = CComQIPtr<IDSSTime, &IID_IDSSTime>(v1.pdispVal);*/
            
			Int32 lHour = 0;
			/*if (lpTime)
			{
				hr = lpTime->get_Hour(&lHour);
				CHECK_HR;
			}*/
            if (VT_DATE == v1.vt)
            {
                MDataType::DSSDateTime lDateTime;
                int lcbData = sizeof(MDataType::DSSDateTime);
                hr = DFCDataConversion::ConvertFromVariant(&v1, DssDataTypeDate, (unsigned char *)&lDateTime, &lcbData);
                CHECK_HR;
                
                lHour = lDateTime.GetHour();
            }
			else
			{
				double lVar = 0;
				Int32 lSizeDouble = sizeof(double);
                
				hr = DFCDataConversion::ConvertFromVariant(&v1, DssDataTypeDouble, (BYTE*)&lVar, &lSizeDouble);
				if (S_OK != hr)
				{
					*pFlag = DssDataInvalid;
					return S_OK;
				}
                
				MDataType::DSSDateTime lDate(lVar);
				lHour = lDate.GetHour();
			}
            
			hr = DFCDataConversion::ConvertToVariant((BYTE *)&lHour, DssDataTypeInteger, 0, pResult);
			CHECK_HR;
		}
            break;
            
        case DssFuncMinute:
		{
			CComVariant v1;
			getVariant(pParam, 0, v1);
            
			/*CComQIPtr<IDSSTime, &IID_IDSSTime> lpTime;
			if (v1.vt == VT_DISPATCH)
				lpTime = CComQIPtr<IDSSTime, &IID_IDSSTime>(v1.pdispVal);*/
            
			Int32 lMinute = 0;
			/*if (lpTime)
			{
				hr = lpTime->get_Minutes(&lMinute);
				CHECK_HR;
			}*/
            if (VT_DATE == v1.vt)
            {
                MDataType::DSSDateTime lDateTime;
                int lcbData = sizeof(MDataType::DSSDateTime);
                hr = DFCDataConversion::ConvertFromVariant(&v1, DssDataTypeDate, (unsigned char *)&lDateTime, &lcbData);
                CHECK_HR;
                
                lMinute = lDateTime.GetMinute();
            }
			else
			{
				double lVar = 0;
				Int32 lSizeDouble = sizeof(double);
                
				hr = DFCDataConversion::ConvertFromVariant(&v1, DssDataTypeDouble, (BYTE*)&lVar, &lSizeDouble);
				if (S_OK != hr)
				{
					*pFlag = DssDataInvalid;
					return S_OK;
				}
                
				MDataType::DSSDateTime lDate(lVar);
				lMinute = lDate.GetMinute();
			}
            
			hr = DFCDataConversion::ConvertToVariant((BYTE *)&lMinute, DssDataTypeInteger, 0, pResult);
			CHECK_HR;
		}
            break;
            
        case DssFuncSecond:
		{
			CComVariant v1;
			getVariant(pParam, 0, v1);
            
			/*CComQIPtr<IDSSTime, &IID_IDSSTime> lpTime;
			if (v1.vt == VT_DISPATCH)
				lpTime = CComQIPtr<IDSSTime, &IID_IDSSTime>(v1.pdispVal);*/
            
			Int32 lSecs = 0;
			/*if (lpTime)
			{
				hr = lpTime->get_Seconds(&lSecs);
				CHECK_HR;
			}*/
            if (VT_DATE == v1.vt)
            {
                MDataType::DSSDateTime lDateTime;
                int lcbData = sizeof(MDataType::DSSDateTime);
                hr = DFCDataConversion::ConvertFromVariant(&v1, DssDataTypeDate, (unsigned char *)&lDateTime, &lcbData);
                CHECK_HR;
                
                lSecs = lDateTime.GetSecond();
            }
			else
			{
				double lVar = 0;
				Int32 lSizeDouble = sizeof(double);
                
				hr = DFCDataConversion::ConvertFromVariant(&v1, DssDataTypeDouble, (BYTE*)&lVar, &lSizeDouble);
				if (S_OK != hr)
				{
					*pFlag = DssDataInvalid;
					return S_OK;
				}
                
				MDataType::DSSDateTime lDate(lVar);
				lSecs = lDate.GetSecond();
			}
            
			hr = DFCDataConversion::ConvertToVariant((BYTE *)&lSecs, DssDataTypeInteger, 0, pResult);
			CHECK_HR;
		}
            break;
            
        case DssFuncMilliSecond:
		{
			CComVariant v1;
			getVariant(pParam, 0, v1);
            
			/*CComQIPtr<IDSSTime, &IID_IDSSTime> lpTime;
			if (v1.vt == VT_DISPATCH)
				lpTime = CComQIPtr<IDSSTime, &IID_IDSSTime>(v1.pdispVal);*/
            
			Int32 lMilliSecs= 0;
			/*if (lpTime)
			{
				hr = lpTime->get_MilliSeconds(&lMilliSecs);
				CHECK_HR;
			}*/
            if (VT_DATE == v1.vt)
            {
                MDataType::DSSDateTime lDateTime;
                int lcbData = sizeof(MDataType::DSSDateTime);
                hr = DFCDataConversion::ConvertFromVariant(&v1, DssDataTypeDate, (unsigned char *)&lDateTime, &lcbData);
                CHECK_HR;
                
                lMilliSecs = lDateTime.GetMillisecond();
            }
			else
			{
				double lVar = 0;
				Int32 lSizeDouble = sizeof(double);
                
				hr = DFCDataConversion::ConvertFromVariant(&v1, DssDataTypeDouble, (BYTE*)&lVar, &lSizeDouble);
				if (S_OK != hr)
				{
					*pFlag = DssDataInvalid;
					return S_OK;
				}
                
				MDataType::DSSDateTime lDate(lVar);
				lMilliSecs = lDate.GetMillisecond();
			}
            
			hr = DFCDataConversion::ConvertToVariant((BYTE *)&lMilliSecs, DssDataTypeInteger, 0, pResult);
			CHECK_HR;
		}
            break;
            
        case DssFuncDate:
		{
			CComVariant v1;
			getVariant(pParam, 0, v1);
            
			/*CComQIPtr<IDSSTime, &IID_IDSSTime> lpTime;
			if (v1.vt == VT_DISPATCH)
				lpTime = CComQIPtr<IDSSTime, &IID_IDSSTime>(v1.pdispVal);*/
            
			/*if (lpTime)
			{
				if (!lpTime->put_Hour(0) || !lpTime->put_Minutes(0) ||
					!lpTime->put_Seconds(0) || !lpTime->put_MilliSeconds(0) )
				{
					*pFlag = DssDataInvalid;
					return S_OK;
				}
                
				VariantCopy(pResult, &v1);
			}*/
            if (VT_DATE == v1.vt)
            {
                MDataType::DSSDateTime lDateTime;
                int lcbData = sizeof(MDataType::DSSDateTime);
                hr = DFCDataConversion::ConvertFromVariant(&v1, DssDataTypeDate, (unsigned char *)&lDateTime, &lcbData);
                if (S_OK != hr)
				{
					*pFlag = DssDataInvalid;
					return S_OK;
				}
                lDateTime.PutHour(0);lDateTime.PutMinute(0);lDateTime.PutSecond(0);lDateTime.PutMillisecond(0);
                
                hr = DFCDataConversion::ConvertToVariant((BYTE *)&lDateTime, DssDataTypeTimeStamp, 0, pResult);
				CHECK_HR;
            }
			else
			{
				double lVar = 0;
				Int32 lSizeDouble = sizeof(double);
                
				hr = DFCDataConversion::ConvertFromVariant(&v1, DssDataTypeDouble, (BYTE*)&lVar, &lSizeDouble);
				if (S_OK != hr)
				{
					*pFlag = DssDataInvalid;
					return S_OK;
				}
                
				lVar = (Int32)lVar;
				MDataType::DSSDateTime lDate(lVar);
				hr = DFCDataConversion::ConvertToVariant((BYTE *)&lDate, DssDataTypeTimeStamp, 0, pResult);
				CHECK_HR;
			}
            
		}
            break;
            
        case DssFuncDaysBetween:
		{
			CComVariant v1;
			getVariant(pParam, 0, v1);
            
			CComVariant v2;
			getVariant(pParam, 1, v2);
            
			/*CComQIPtr<IDSSTime, &IID_IDSSTime> lpStartTime;
			if (v1.vt == VT_DISPATCH)
				lpStartTime = CComQIPtr<IDSSTime, &IID_IDSSTime>(v1.pdispVal);*/
            
			DATE lStartDate = 0;
			/*if (lpStartTime)
			{
				hr = lpStartTime->get_DateTime(&lStartDate);
				CHECK_HR;
			}*/
            if (VT_DATE == v1.vt)
            {
                MDataType::DSSDateTime lDateTime;
                int lcbData = sizeof(MDataType::DSSDateTime);
                hr = DFCDataConversion::ConvertFromVariant(&v1, DssDataTypeDate, (unsigned char *)&lDateTime, &lcbData);
                CHECK_HR;
                
                lStartDate = lDateTime.ToDATE();
            }
			else
			{
				double lVar = 0;
				Int32 lSizeDouble = sizeof(double);
                
				hr = DFCDataConversion::ConvertFromVariant(&v1, DssDataTypeDouble, (BYTE*)&lVar, &lSizeDouble);
				if (S_OK != hr)
				{
					*pFlag = DssDataInvalid;
					return S_OK;
				}
                
				lStartDate = static_cast<DATE>(lVar);
			}
            
			/*CComQIPtr<IDSSTime, &IID_IDSSTime> lpEndTime;
			if (v2.vt == VT_DISPATCH)
				lpEndTime = CComQIPtr<IDSSTime, &IID_IDSSTime>(v2.pdispVal);*/
            
			DATE lEndDate = 0;
			/*if (lpEndTime)
			{
				hr = lpEndTime->get_DateTime(&lEndDate);
				CHECK_HR;
			}*/
            if (VT_DATE == v2.vt)
            {
                MDataType::DSSDateTime lDateTime;
                int lcbData = sizeof(MDataType::DSSDateTime);
                hr = DFCDataConversion::ConvertFromVariant(&v2, DssDataTypeDate, (unsigned char *)&lDateTime, &lcbData);
                CHECK_HR;
                
                lEndDate = lDateTime.ToDATE();
            }
			else
			{
				double lVar = 0;
				Int32 lSizeDouble = sizeof(double);
                
				hr = DFCDataConversion::ConvertFromVariant(&v2, DssDataTypeDouble, (BYTE*)&lVar, &lSizeDouble);
				if (S_OK != hr)
				{
					*pFlag = DssDataInvalid;
					return S_OK;
				}
                
				lEndDate = static_cast<DATE>(lVar);
			}
            
			// TQMS 648733, return the difference based on the number of day boundaries crossed
			// not the number of twenty-four hours periods elapsed
			Int32 lnDays = (Int32)(lEndDate) - (Int32)(lStartDate);
			hr = DFCDataConversion::ConvertToVariant((BYTE *)&lnDays, DssDataTypeInteger, 0, pResult);
			CHECK_HR;
		}
            break;
            
        case DssFuncMonthsBetween:
		{
			CComVariant v1;
			getVariant(pParam, 0, v1);
            
			CComVariant v2;
			getVariant(pParam, 1, v2);
            
			/*CComQIPtr<IDSSTime, &IID_IDSSTime> lpStartTime;
			if (v1.vt == VT_DISPATCH)
				lpStartTime = CComQIPtr<IDSSTime, &IID_IDSSTime>(v1.pdispVal);*/
            
			Int32 lStartYear = 0;
			Int32 lStartMonth = 0;
			Int32 lStartDayOfMonth = 0;
			/*if (lpStartTime)
			{
				hr = lpStartTime->get_Year(&lStartYear);
				CHECK_HR;
				
				hr = lpStartTime->get_Month((EnumDSSMonth *)&lStartMonth);
				CHECK_HR;
                
				hr = lpStartTime->get_Day(&lStartDayOfMonth);
				CHECK_HR;
			}*/
            if (VT_DATE == v1.vt)
            {
                MDataType::DSSDateTime lDateTime;
                int lcbData = sizeof(MDataType::DSSDateTime);
                hr = DFCDataConversion::ConvertFromVariant(&v1, DssDataTypeDate, (unsigned char *)&lDateTime, &lcbData);
                CHECK_HR;
                
                lStartYear = lDateTime.GetYear();
                lStartMonth = lDateTime.GetMonth();
                lStartDayOfMonth = lDateTime.GetDay();
            }
			else
			{
				double lVar = 0;
				Int32 lSizeDouble = sizeof(double);
                
				hr = DFCDataConversion::ConvertFromVariant(&v1, DssDataTypeDouble, (BYTE*)&lVar, &lSizeDouble);
				if (S_OK != hr)
				{
					*pFlag = DssDataInvalid;
					return S_OK;
				}
                
				MDataType::DSSDateTime lDate(lVar);
				lStartYear = lDate.GetYear();
				lStartMonth = lDate.GetMonth();
				lStartDayOfMonth = lDate.GetDay();
			}
            
			/*CComQIPtr<IDSSTime, &IID_IDSSTime> lpEndTime;
			if (v2.vt == VT_DISPATCH)
				lpEndTime = CComQIPtr<IDSSTime, &IID_IDSSTime>(v2.pdispVal);*/
            
			Int32 lEndYear = 0;
			Int32 lEndMonth = 0;
			Int32 lEndDayOfMonth = 0;
			/*if (lpEndTime)
			{
				hr = lpEndTime->get_Year(&lEndYear);
				CHECK_HR;
                
				hr = lpEndTime->get_Month((EnumDSSMonth *)&lEndMonth);
				CHECK_HR;
                
				hr = lpEndTime->get_Day(&lEndDayOfMonth);
				CHECK_HR;
			}*/
            if (VT_DATE == v2.vt)
            {
                MDataType::DSSDateTime lDateTime;
                int lcbData = sizeof(MDataType::DSSDateTime);
                hr = DFCDataConversion::ConvertFromVariant(&v2, DssDataTypeDate, (unsigned char *)&lDateTime, &lcbData);
                CHECK_HR;
                
                lEndYear = lDateTime.GetYear();
                lEndMonth = lDateTime.GetMonth();
                lEndDayOfMonth = lDateTime.GetDay();
            }
			else
			{
				double lVar = 0;
				Int32 lSizeDouble = sizeof(double);
                
				hr = DFCDataConversion::ConvertFromVariant(&v2, DssDataTypeDouble, (BYTE*)&lVar, &lSizeDouble);
				if (S_OK != hr)
				{
					*pFlag = DssDataInvalid;
					return S_OK;
				}
                
				MDataType::DSSDateTime lDate(lVar);
				lEndYear = lDate.GetYear();
				lEndMonth = lDate.GetMonth();
				lEndDayOfMonth = lDate.GetDay();
			}
            
			// TQMS 648727, calculate by the months elapsed, not the number of month boundaries crossed
			Int32 lnMonth = (lEndYear - lStartYear) * 12 + (lEndMonth - lStartMonth);
			if(lnMonth > 0)
				lnMonth = lEndDayOfMonth < lStartDayOfMonth ? lnMonth - 1 : lnMonth;
			else if (lnMonth < 0)
				lnMonth = lEndDayOfMonth > lStartDayOfMonth ? lnMonth + 1 : lnMonth;
            
			hr = DFCDataConversion::ConvertToVariant((BYTE *)&lnMonth, DssDataTypeInteger, 0, pResult);
			CHECK_HR;
		}
            break;
            
        case DssFuncAddDays:
		{
			CComVariant v1;
			getVariant(pParam, 0, v1);
            
			CComVariant v2;
			getVariant(pParam, 1, v2);
            
			/*CComQIPtr<IDSSTime, &IID_IDSSTime> lpTime1;
			if (v1.vt == VT_DISPATCH)
				lpTime1 = CComQIPtr<IDSSTime, &IID_IDSSTime>(v1.pdispVal);*/
            
			double lDate = 0;
			/*if (lpTime1)
			{
				//if parameter is TimeStamp, use offset from the StartDate
				hr = lpTime1->get_DateTime( (DATE *)&lDate);
				CHECK_HR;
			}*/
            if (VT_DATE == v1.vt)
            {
                MDataType::DSSDateTime lDateTime;
                int lcbData = sizeof(MDataType::DSSDateTime);
                hr = DFCDataConversion::ConvertFromVariant(&v1, DssDataTypeDate, (unsigned char *)&lDateTime, &lcbData);
                CHECK_HR;
                
                lDate = lDateTime.ToDATE();
            }
			else
			{
				//convert into double if necessary
				Int32 lSizeDouble = sizeof(double);
                
				hr = DFCDataConversion::ConvertFromVariant(&v1, DssDataTypeDouble, (BYTE*)&lDate, &lSizeDouble);
				if (S_OK != hr)
				{
					*pFlag = DssDataInvalid;
					return S_OK;
				}
			}
            
			/*CComQIPtr<IDSSTime, &IID_IDSSTime> lpTime2;
			if (v2.vt == VT_DISPATCH)
				lpTime2 = CComQIPtr<IDSSTime, &IID_IDSSTime>(v2.pdispVal);*/
            
			// return XTabNull if the second parameter is Date/Tiem/DateTime
			//if (lpTime2)
            if (VT_DATE == v2.vt)
			{
				*pFlag = DssDataInvalid;
				return S_OK;
			}
            
			// also add the fraction when add days
			double lDeltaDate = 0;
			Int32 lSizeDouble = sizeof(double);
			hr = DFCDataConversion::ConvertFromVariant(&v2, DssDataTypeDouble, (BYTE*)&lDeltaDate, &lSizeDouble);
			if (S_OK != hr)
			{
				*pFlag = DssDataInvalid;
				return S_OK;
			}
            
			lDate += lDeltaDate;
			MDataType::DSSDateTime lResDate(lDate);
			hr = DFCDataConversion::ConvertToVariant((BYTE *)&lResDate, DssDataTypeTimeStamp, 0, pResult);
			CHECK_HR;
		}
            break;
            
        case DssFuncAddMonths:
		{
			CComVariant v1;
			getVariant(pParam, 0, v1);
            
			CComVariant v2;
			getVariant(pParam, 1, v2);
            
			/*CComQIPtr<IDSSTime, &IID_IDSSTime> lpTime1;
			if (v1.vt == VT_DISPATCH)
				lpTime1 = CComQIPtr<IDSSTime, &IID_IDSSTime>(v1.pdispVal);*/
            
			Int32 lCurYear = 0;
			Int32 lCurMonth = 0;
			Int32 lCurDayOfMonth = 0;
            
			MDataType::DSSDateTime lDate;
			//get current year/month/dayofmonth to calculate the total month
			/*if (lpTime1)
			{
				hr = lpTime1->get_Year(&lCurYear);
				CHECK_HR;
                
				hr = lpTime1->get_Month((EnumDSSMonth *)&lCurMonth);
				CHECK_HR;
                
				hr = lpTime1->get_Day(&lCurDayOfMonth);
				CHECK_HR;
			}*/
            if (VT_DATE == v1.vt)
            {
                MDataType::DSSDateTime lDateTime;
                int lcbData = sizeof(MDataType::DSSDateTime);
                hr = DFCDataConversion::ConvertFromVariant(&v1, DssDataTypeDate, (unsigned char *)&lDateTime, &lcbData);
                CHECK_HR;
                
				lCurYear = lDateTime.GetYear();
				lCurMonth = lDateTime.GetMonth();
				lCurDayOfMonth = lDateTime.GetDay();
            }
			else
			{
				double lVar;
				Int32 lSizeDouble = sizeof(double);
                
				hr = DFCDataConversion::ConvertFromVariant(&v1, DssDataTypeDouble, (BYTE*)&lVar, &lSizeDouble);
				if (S_OK != hr)
				{
					*pFlag = DssDataInvalid;
					return S_OK;
				}
                
				lDate = MDataType::DSSDateTime(lVar);
				lCurYear = lDate.GetYear();
				lCurMonth = lDate.GetMonth();
				lCurDayOfMonth = lDate.GetDay();
			}
            
			Int32 lnMonth = lCurYear * 12 + lCurMonth;
            
			/*CComQIPtr<IDSSTime, &IID_IDSSTime> lpTime2;
			if (v2.vt == VT_DISPATCH)
				lpTime2 = CComQIPtr<IDSSTime, &IID_IDSSTime>(v2.pdispVal);*/
            
			// return XTabNull if the second parameter is Date/Tiem/DateTime
			//if (lpTime2)
            if (VT_DATE == v2.vt)
			{
				*pFlag = DssDataInvalid;
				return S_OK;
			}
            
			double lDeltaMonth = 0;
			Int32 lSizeDouble = sizeof(double);
			hr = DFCDataConversion::ConvertFromVariant(&v2, DssDataTypeDouble, (BYTE*)&lDeltaMonth, &lSizeDouble);
			if (S_OK != hr)
			{
				*pFlag = DssDataInvalid;
				return S_OK;
			}
            
			//calculate months from 00/00, only add the integer part for delta month
			Int64 lMonth64 = (Int64) (lnMonth + lDeltaMonth);
            
			//deliu, 2013-02-07, 716851, precision lose trancate from double to int
			Int64 lYear64 = lMonth64 / 12;
			if (lYear64 > 0x7FFFFFFF)
			{
				// overflow the max integer
				VariantInit(pResult);
				*pFlag = DssDataInvalid;
				return S_OK;
			}
            
			Int32 lYear = (Int32)lYear64;
			Int32 lMonth = (Int32)(lMonth64 % 12);
			if (lMonth == 0)
			{
				lYear--;
				lMonth = 12;
			}
            
			//get dayoffset from the start date
			//if (lpTime1)
            if (VT_DATE == v1.vt)
			{
                MDataType::DSSDateTime lDateTime;
                int lcbData = sizeof(MDataType::DSSDateTime);
                hr = DFCDataConversion::ConvertFromVariant(&v1, DssDataTypeDate, (unsigned char *)&lDateTime, &lcbData);
                CHECK_HR;
                
				if (!lDateTime.PutYear(lYear) || !lDateTime.PutMonth((EnumDSSMonth)lMonth) ||
					!lDateTime.PutDay(lCurDayOfMonth) )
				{
					VariantInit(pResult);
					*pFlag = DssDataInvalid;
					return S_OK;
				}
                
                hr = DFCDataConversion::ConvertToVariant((BYTE *)&lDateTime, DssDataTypeTimeStamp, 0, pResult);
				CHECK_HR;
			}
			else
			{
				if (!lDate.PutYear(lYear) || !lDate.PutMonth((EnumDSSMonth)lMonth) ||
					!lDate.PutDay(lCurDayOfMonth) )
				{
					VariantInit(pResult);
					*pFlag = DssDataInvalid;
					return S_OK;
				}
                
				hr = DFCDataConversion::ConvertToVariant((BYTE *)&lDate, DssDataTypeTimeStamp, 0, pResult);
				CHECK_HR;
			}
		}
            break;
            
        case DssFuncMonthStartDate:
		{
			CComVariant v1;
			getVariant(pParam, 0, v1);
            
			/*CComQIPtr<IDSSTime, &IID_IDSSTime> lpTime;
			if (v1.vt == VT_DISPATCH)
				lpTime = CComQIPtr<IDSSTime, &IID_IDSSTime>(v1.pdispVal);*/
            
			//if (lpTime)
            if (VT_DATE == v1.vt)
			{
                MDataType::DSSDateTime lDateTime;
                int lcbData = sizeof(MDataType::DSSDateTime);
                hr = DFCDataConversion::ConvertFromVariant(&v1, DssDataTypeDate, (unsigned char *)&lDateTime, &lcbData);
                CHECK_HR;
                
				/*if (!lpTime->put_Day(1) || !lpTime->put_Hour(0) ||
					!lpTime->put_Minutes(0) || !lpTime->put_Seconds(0) ||
					!lpTime->put_MilliSeconds(0) )*/
                if (!lDateTime.PutDay(1) || !lDateTime.PutHour(0) || !lDateTime.PutMinute(0) ||
					!lDateTime.PutSecond(0) || !lDateTime.PutMillisecond(0) )
				{
					VariantInit(pResult);
					*pFlag = DssDataInvalid;
					return S_OK;
				}
                
                hr = DFCDataConversion::ConvertToVariant((BYTE *)&lDateTime, DssDataTypeTimeStamp, 0, pResult);
				CHECK_HR;
			}
			else
			{
				double lVar = 0;
				Int32 lSizeDouble = sizeof(double);
                
				hr = DFCDataConversion::ConvertFromVariant(&v1, DssDataTypeDouble, (BYTE*)&lVar, &lSizeDouble);
				if (S_OK != hr)
				{
					*pFlag = DssDataInvalid;
					return S_OK;
				}
                
				MDataType::DSSDateTime lDate(lVar);
				if (!lDate.PutDay(1) || !lDate.PutHour(0) || !lDate.PutMinute(0) ||
					!lDate.PutSecond(0) || !lDate.PutMillisecond(0) )
				{
					VariantInit(pResult);
					*pFlag = DssDataInvalid;
					return S_OK;
				}
                
				hr = DFCDataConversion::ConvertToVariant((BYTE *)&lDate, DssDataTypeTimeStamp, 0, pResult);
				CHECK_HR;
			}
		}
            break;
            
        case DssFuncMonthEndDate:
		{
			CComVariant v1;
			getVariant(pParam, 0, v1);
            
			/*CComQIPtr<IDSSTime, &IID_IDSSTime> lpTime;
			if (v1.vt == VT_DISPATCH)
				lpTime = CComQIPtr<IDSSTime, &IID_IDSSTime>(v1.pdispVal);*/
			
			//if (lpTime)
            if (VT_DATE == v1.vt)
			{
                MDataType::DSSDateTime lDateTime;
                int lcbData = sizeof(MDataType::DSSDateTime);
                hr = DFCDataConversion::ConvertFromVariant(&v1, DssDataTypeDate, (unsigned char *)&lDateTime, &lcbData);
                CHECK_HR;
                
				/*if (!lpTime->put_Day(31) || !lpTime->put_Hour(0) ||
					!lpTime->put_Minutes(0) || !lpTime->put_Seconds(0) ||
					!lpTime->put_MilliSeconds(0) )*/
                if (!lDateTime.PutDay(31) || !lDateTime.PutHour(0) || !lDateTime.PutMinute(0) ||
					!lDateTime.PutSecond(0) || !lDateTime.PutMillisecond(0) )
				{
					VariantInit(pResult);
					*pFlag = DssDataInvalid;
					return S_OK;
				}
                
                hr = DFCDataConversion::ConvertToVariant((BYTE *)&lDateTime, DssDataTypeTimeStamp, 0, pResult);
				CHECK_HR;
			}
			else
			{
				double lVar = 0;
				Int32 lSizeDouble = sizeof(double);
                
				hr = DFCDataConversion::ConvertFromVariant(&v1, DssDataTypeDouble, (BYTE*)&lVar, &lSizeDouble);
				if (S_OK != hr)
				{
					*pFlag = DssDataInvalid;
					return S_OK;
				}
                
				MDataType::DSSDateTime lDate(lVar);
				// will check the maximum day of month in the date function
				if (!lDate.PutDay(31) || !lDate.PutHour(0) || !lDate.PutMinute(0) ||
					!lDate.PutSecond(0) || !lDate.PutMillisecond(0) )
				{
					VariantInit(pResult);
					*pFlag = DssDataInvalid;
					return S_OK;
				}
                
				hr = DFCDataConversion::ConvertToVariant((BYTE *)&lDate, DssDataTypeTimeStamp, 0, pResult);
				CHECK_HR;
			}
		}
            break;
            
        case DssFuncYearStartDate:
		{
			CComVariant v1;
			getVariant(pParam, 0, v1);
            
			/*CComQIPtr<IDSSTime, &IID_IDSSTime> lpTime;
			if (v1.vt == VT_DISPATCH)
				lpTime = CComQIPtr<IDSSTime, &IID_IDSSTime>(v1.pdispVal);*/
            
			//if (lpTime)
            if (VT_DATE == v1.vt)
			{
                MDataType::DSSDateTime lDateTime;
                int lcbData = sizeof(MDataType::DSSDateTime);
                hr = DFCDataConversion::ConvertFromVariant(&v1, DssDataTypeDate, (unsigned char *)&lDateTime, &lcbData);
                CHECK_HR;
                
				Int32  lYear = lDateTime.GetYear();
                
				MDataType::DSSDateTime lStartDate(lYear, 1, 1);
				hr = DFCDataConversion::ConvertToVariant((BYTE *)&lStartDate, DssDataTypeTimeStamp, 0, pResult);
				CHECK_HR;
			}
			else
			{
				double lVar = 0;
				Int32 lSizeDouble = sizeof(double);
                
				hr = DFCDataConversion::ConvertFromVariant(&v1, DssDataTypeDouble, (BYTE*)&lVar, &lSizeDouble);
				if (S_OK != hr)
				{
					*pFlag = DssDataInvalid;
					return S_OK;
				}
                
				MDataType::DSSDateTime lDate(lVar);
				Int32  lYear = lDate.GetYear();
                
				MDataType::DSSDateTime lStartDate(lYear, 1, 1);
				hr = DFCDataConversion::ConvertToVariant((BYTE *)&lStartDate, DssDataTypeTimeStamp, 0, pResult);
				CHECK_HR;
			}
		}
            break;
            
        case DssFuncYearEndDate:
		{
			CComVariant v1;
			getVariant(pParam, 0, v1);
            
			/*CComQIPtr<IDSSTime, &IID_IDSSTime> lpTime;
			if (v1.vt == VT_DISPATCH)
				lpTime = CComQIPtr<IDSSTime, &IID_IDSSTime>(v1.pdispVal);*/
            
			//if (lpTime)
            if (VT_DATE == v1.vt)
			{
				MDataType::DSSDateTime lDateTime;
                int lcbData = sizeof(MDataType::DSSDateTime);
                hr = DFCDataConversion::ConvertFromVariant(&v1, DssDataTypeDate, (unsigned char *)&lDateTime, &lcbData);
                CHECK_HR;
                
                Int32  lYear = lDateTime.GetYear();
                
				MDataType::DSSDateTime lEndDate(lYear, 12, 31);
				hr = DFCDataConversion::ConvertToVariant((BYTE *)&lEndDate, DssDataTypeTimeStamp, 0, pResult);
				CHECK_HR;
			}
			else
			{
				double lVar = 0;
				Int32 lSizeDouble = sizeof(double);
                
				hr = DFCDataConversion::ConvertFromVariant(&v1, DssDataTypeDouble, (BYTE*)&lVar, &lSizeDouble);
				if (S_OK != hr)
				{
					*pFlag = DssDataInvalid;
					return S_OK;
				}
                
				MDataType::DSSDateTime lDate(lVar);
				Int32  lYear = lDate.GetYear();
                
				MDataType::DSSDateTime lEndDate(lYear, 12, 31);
				hr = DFCDataConversion::ConvertToVariant((BYTE *)&lDate, DssDataTypeTimeStamp, 0, pResult);
				CHECK_HR;
			}
		}
            break;
            
        default:
            hr = E_INVALIDARG;
            break;
    }
    
    *pFlag = DssDataOk;
    
    return hr;
}

int DSSDateTimeFunc::Calculate(int nParam, double *ipParams, double *opResult, DSSDataFlag *opFlag)
{
    return E_NOTIMPL;
}

/*
Int32 DSSDateTimeFunc::mktime(IDSSTime *ipTime, struct tm& oTime) const
{
	int hr = S_OK;
    
	Int32 lYear = 0;
	hr = ipTime->get_Year(&lYear);
	CHECK_HR;
	oTime.tm_year = MDataType::DateTime::YearOfSamePattern(lYear) - 1900;
 
	Int32 lMonth = 0;
	hr = ipTime->get_Month((EnumDSSMonth *)&lMonth);
	CHECK_HR;
	oTime.tm_mon = lMonth - MDataType::JAN;
    
	Int32 lHour = 0;
	hr = ipTime->get_Hour(&lHour);
	CHECK_HR;
	oTime.tm_hour = lHour;
    
	Int32 lDay = 0;
	hr = ipTime->get_Day(&lDay);
	CHECK_HR;
	oTime.tm_mday = lDay;
    
	Int32 lMinute = 0;
	hr = ipTime->get_Minutes(&lMinute);
	CHECK_HR;
	oTime.tm_min = lMinute;
    
	Int32 lSecs = 0;
	hr = ipTime->get_Seconds(&lSecs);
	CHECK_HR;
	oTime.tm_sec = lSecs;
    
	oTime.tm_isdst = -1;		// let runtime library decide
	time_t lTime = ::mktime(&oTime);
    
	return (Int32)lTime;
}*/

Int32 DSSDateTimeFunc::mktime(MDataType::DSSDateTime *ipTime, struct tm& oTime) const
{
	//int hr = S_OK;
    
	Int32 lYear = ipTime->GetYear();
	oTime.tm_year = MDataType::DSSDateTime::YearOfSamePattern(lYear) - 1900;
    
	Int32 lMonth = ipTime->GetMonth();
	oTime.tm_mon = lMonth - MDataType::JAN;
    
	Int32 lHour = ipTime->GetHour();
	oTime.tm_hour = lHour;
    
	Int32 lDay = ipTime->GetDay();
	oTime.tm_mday = lDay;
    
	Int32 lMinute = ipTime->GetMinute();
	oTime.tm_min = lMinute;
    
	Int32 lSecs = ipTime->GetSecond();
	oTime.tm_sec = lSecs;
    
	oTime.tm_isdst = -1;		// let runtime library decide
	time_t lTime = ::mktime(&oTime);
    
	return (Int32)lTime;
}

Int32 DSSDateTimeFunc::YMD2DAYS(Int32 inYear, Int32 inMonth, Int32 inDay)
{
    if (inMonth <  MDataType::JAN || inMonth > MDataType::DEC)
	{
		_ASSERTE(false);
		return 0;
	}
    
	// days since 01/01/0000 to 01/01/inYear
	Int32 lnDays = inYear*365+inYear/4+(inYear%4?1:0)-(inYear-1)/100+(inYear-1)/400;
    
	// days since 01/01/1900 to inMonth/01/inYear
	lnDays += DaysBeforeMonth[inMonth-1];
    
	// adjust for leap year
	if (inMonth > MDataType::FEB && MDataType::DSSDateTime::IsLeapYear(inYear))
	{
		lnDays++;
	}
    
	lnDays += inDay-1;
	// days since 12/30/1899
	lnDays -= 693959;
    
	return lnDays;
}

void DSSDateTimeFunc::DAYS2YMD(Int32 inDays, Int32& ornYear, Int32& ornMonth, Int32& ornDay)
{
    inDays += 693959;	// days to Jan/1/0
    
    Int32 lnYear = inDays/365;
    Int32 lnRemain = inDays - lnYear*365;
    Int32 lnLeapDays = lnYear/4-lnYear/100+lnYear/400;
    lnRemain = lnRemain - lnLeapDays;
    if (lnRemain<0)
    {
        lnYear--;
        lnRemain += 365;
        while (lnRemain<=0)
        {
            lnYear--;
            lnRemain += 365;
        }
    }
    
	Int32 lnDays = inDays - (lnYear*365+lnYear/4+(lnYear%4?1:0)-(lnYear-1)/100+(lnYear-1)/400);
    
	// deliu, 2012-07-16, TQMS 628159
	if (lnDays > 365)
	{
		lnYear++;
		lnDays -= 365;
		lnDays = MDataType::DSSDateTime::IsLeapYear(lnYear - 1) ? (lnDays - 1) : lnDays;
	}
    
	if (lnDays==365 && !MDataType::DSSDateTime::IsLeapYear(lnYear))
	{
		ornYear = lnYear+1;
		ornMonth = MDataType::JAN;
		ornDay = 1;
		return;
	}
    
	if (lnDays==-1)
	{
		ornYear = lnYear-1;
		ornMonth = MDataType::DEC;
		ornDay = 31;
		return;
	}
    
	ornYear = lnYear;
    
	ornMonth = MDataType::DEC;
	while (lnDays < DaysBeforeMonth[ornMonth-1])
	{
		ornMonth--;
	}
    
	ornDay = lnDays - DaysBeforeMonth[ornMonth-1] + 1;
    
	if (ornMonth > MDataType::FEB && MDataType::DSSDateTime::IsLeapYear(ornYear))
	{
		ornDay--;
		if (ornDay==0)
		{
			ornMonth--;
			ornDay = DaysInMonth[ornMonth-1];
			if (ornMonth == MDataType::FEB)
			{
				ornDay++;
			}
		}
	}
}

int DSSDateTimeFunc::SetProperty (Int32 Index, CComVariant *pValue)
{
	return E_NOTIMPL;
}

int DSSDateTimeFunc::GetOutputType (Int32 /*size*/, EnumDSSDataType * inputType, EnumDSSDataType *outputType)
{
 	if(outputType == NULL || inputType == NULL) 
		return E_POINTER;
    
	switch (mFunctionIndex)
	{
        case DssFuncDayOfMonth:
        case DssFuncDayOfWeek:
        case DssFuncDayOfYear:
        case DssFuncWeek:
        case DssFuncMonth:
        case DssFuncQuarter:
        case DssFuncYear:
        case DssFuncHour:
        case DssFuncMinute:
        case DssFuncSecond:
        case DssFuncMilliSecond:
        case DssFuncDaysBetween:
        case DssFuncMonthsBetween:
            *outputType = DssDataTypeInteger;
            break;
            
            // TQMS 670982/670983, return date type for AddDays/AddMonths functions
        case DssFuncAddDays:
        case DssFuncAddMonths:
        case DssFuncMonthStartDate:
        case DssFuncMonthEndDate:
        case DssFuncYearStartDate:
        case DssFuncYearEndDate:
		{
			if (*inputType == DssDataTypeTime || *inputType == DssDataTypeDate
				|| *inputType == DssDataTypeTimeStamp)
			{
				*outputType = *inputType;
			}
			else
			{
				// for these that numeric values that can be converted into days after the start date
				*outputType = DssDataTypeTimeStamp;
			}
		}
            break;
            
            // TQMS 648719, only return date for CurrentDate function
        case DssFuncCurrentDate:
        case DssFuncDate:
            *outputType = DssDataTypeDate;
            break;
            
            // TQMS 632141, return time for CurrentTime function
        case DssFuncCurrentTime:
            *outputType = DssDataTypeTime;
            break;
            
        case DssFuncCurrentDateTime:
            *outputType = DssDataTypeTimeStamp;
            break;
            
        default:
            *outputType = DssDataTypeUnknown;
            break;
	}
    
	return S_OK;
}

int DSSDateTimeFunc::SetStringCompareOption(VARIANT_BOOL iOpt)
{
	mfTrimString = (iOpt == VARIANT_TRUE);
    
	return S_OK;
}