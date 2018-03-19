//
//  CellDataHelper.h
//  ReportServiceCore
//
//  Created by Liao Taimin on 9/20/14.
//  Copyright (c) 2014 MicroStrategy, Inc. All rights reserved.
//

#ifndef ReportServiceCore_CellDataHelper_h
#define ReportServiceCore_CellDataHelper_h

#include "PDCHeader/PDCvector"
//#include "DFCDateTime/DFCDateTime.h"

using namespace std;

/**
 * CellDataHelper class.
 *
 * CellDataHelper is a helper class providing methods to deal with cell formatting data.
 */
class CellDataHelper
{
public:
	/**
	 * Splits an array of VARIANT objects into values, types and format IDs (the three components in a cell format object).
	 * @param Int32 the length of the VARIANT array
	 * @param VARIANT* the VARIANT array
	 * @param std::vector<double>& the value array (output)
	 * @param std::vector<EnumCellFormatDataTypes>& the type array (output)
	 * @param std::vector<DSS_ID>& the format ID array
	 * @return HRESULT the return status
	 */
	static int SplitCellData(Int32 nParam, CComVariant * pParam,
                            std::vector<double>& orValues,
                            std::vector<EnumCellFormatDataTypes>& orTypes,
                            std::vector<DSS_ID>& orFormatIDs);
    
	/**
	 * Creates a VARIANT based on the given cell format data components.
	 * @param double the value of the cell format object
	 * @param EnumCellFormatDataTypes the type (tag) of the cell format object
	 * @param GUID& the DSS_ID of the cell format object
	 * @param VARIANT* the constructed VARIANT object (output)
	 * @return HRESULT the return status
	 */
	static int CreateVariantFromCellData(double pValue, EnumCellFormatDataTypes pType, const GUID& pFormatID, CComVariant* oResult);
    
	/**
	 * Calculates the type of the resulting cell format object.
	 * This method determines the type of the resulting cell format object based on the following rule:
	 * - A op A: A
	 * - A op B: MIXED
	 * - A op const: MIXED
	 * - const op A: MIXED
	 * @param Int32 the number of input cell format objects
	 * @param EnumCellFormatDataTypes* the types of the input cell format objects
	 * @return EnumCellFormatDataTypes the type of the resulting cell format object
	 */
	static EnumCellFormatDataTypes CalculateCellDataType(Int32 nParam, EnumCellFormatDataTypes * pParam);
    
	/**
	 * Calculates the format ID of the resulting cell format object.
	 * This method determines the format ID of the resulting cell format object based on the following rule:
	 * - A op A: A
	 * - A op B: MIXED
	 * - A op const: MIXED
	 * - const op A: MIXED
	 * @param Int32 the number of input cell format objects
	 * @param DSS_ID* the format IDs of the input cell format objects
	 * @return DSS_ID the format ID of the resulting cell format object
	 */
	static DSS_ID CalculateCellDataFormatID(Int32 nParam, DSS_ID * pParam);
    
	/**
	 * @return DSS_ID the default format ID
	 */
	static DSS_ID GetDefaultFormatID();
    
	/**
	 * @return DSS_ID the format ID for mixed cell format calculation result
	 */
	static DSS_ID GetMixFormatID();
    
};

#endif
