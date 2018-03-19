//
//  ManipulationHelper.h
//  ReportServiceCore
//
//  Created by wzhu on 3/26/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef ReportServiceCore_GUIDHelper_h
#define ReportServiceCore_GUIDHelper_h

#include "GUID.h"
#include "DSSBaseElementProxy.h"
#include "time.h"

namespace ManipulationHelper
{
    // TToHexString converts an <T> value (unsigned integral) to a
    // character hexadecimal string.
    // As it is only used internally it doesn't do any sanity checks.
    template<class T>
    inline void TToHexString(T iT, char* ipString)
    {
        const char lcHexDigits[16] =
        {
            '0', '1', '2', '3', '4', '5', '6', '7',
            '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
        };
    
        
        for(int i = (sizeof(T)*2-1); i >= 0; --i, iT >>= 4)
        {
            ipString[i] = lcHexDigits[iT & 0x0000000F];
        }
    }
    
    /**
     *  Generates a GUID (unique identifier) based on 
     *  pseudo-random number generator and the current time.
     *
     *  <p>The GUID has the form
     *  <code>"XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX"</code>
     *  where X is a hexadecimal digit (0-9, A-F).</p>
     *
     *  <p>This UID will not be truly globally unique; but it is the best
     *  </p>
     *
     *  @return The newly-generated GUID.
     */
    inline GUID createUID()
    {
        
        short ALPHA_CHAR_CODES[] = {48, 49, 50, 51, 52, 53, 54, 
            55, 56, 57, 65, 66, 67, 68, 69, 70};
        
        std::string uid;
        
        int i = 0;
        int j = 0;
        
        srand(time(NULL));
        
        for (i = 0; i < 8; i++)
        {
            uid.push_back(ALPHA_CHAR_CODES[rand()%16]);
        }
        
        for (i = 0; i < 3; i++)
        {
            for (j = 0; j < 4; j++)
            {
                uid.push_back(ALPHA_CHAR_CODES[rand()%16]);
            }
        }
        
        double lTime = time(NULL)*1000;
        
        char lIDString[33];
        const unsigned int* pc = reinterpret_cast<const unsigned int*>(&lTime);
        TToHexString<unsigned int>(*pc, lIDString);
        TToHexString<unsigned int>(*(pc + 1), lIDString + 8);
        TToHexString<unsigned int>(*(pc + 2), lIDString + 16);
        TToHexString<unsigned int>(*(pc + 3), lIDString + 24);
        lIDString[32] = '\0';
        
        //MDataType::DateTime lEndTime = MDataType::DateTime::NowUTC();
        // Note: time is the number of milliseconds since 1970,
        // which is currently more than one trillion.
        // We use the low 8 hex digits of this number in the UID.
        // Just in case the system clock has been reset to
        // Jan 1-4, 1970 (in which case this number could have only
        // 1-7 hex digits), we pad on the left with 7 zeros
        // before taking the low digits.
        std::string lTemp = "0000000";
        lTemp += lIDString;
        std::string timeString = lTemp.substr(lTemp.size() - 8);
        
        for (i = 0; i < 8; i++)
        {
            uid.push_back(timeString[i]);
        }
        
        for (i = 0; i < 4; i++)
        {
           uid.push_back(ALPHA_CHAR_CODES[rand()%16]);
        }
        
        GUID lGUID;
        DSSBaseElementProxy::ConvertFromStringToGUID(uid, lGUID);
        return lGUID;
    }
    
    
}


#endif
