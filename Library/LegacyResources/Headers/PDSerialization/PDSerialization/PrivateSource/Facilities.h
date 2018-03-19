//==============================================================================================
//	FILENAME	:	Facilities.h
//	AUTHOR		:	Yi Luo
//	CREATION	:	07/12/2002
//	Copyright (C) MicroStrategy, Inc. 2002
//==============================================================================================

#ifndef Serialization_Facilities_H
#define Serialization_Facilities_H


#define const_swap2(x) \
         ((unsigned short)( \
                 (((unsigned short)(x) & (unsigned short)0x00ffU) << 8) | \
                 (((unsigned short)(x) & (unsigned short)0xff00U) >> 8) ))

#define const_swap4(x) \
         ((unsigned int)( \
                 (((unsigned int)(x) & (unsigned int)0x000000ffUL) << 24) | \
                 (((unsigned int)(x) & (unsigned int)0x0000ff00UL) <<  8) | \
                 (((unsigned int)(x) & (unsigned int)0x00ff0000UL) >>  8) | \
                 (((unsigned int)(x) & (unsigned int)0xff000000UL) >> 24) ))

#define const_swap8(x) \
         ((unsigned Int64)( \
                 (unsigned Int64)(((unsigned Int64)(x) & INT64(0x00000000000000ff) ) << 56) | \
                 (unsigned Int64)(((unsigned Int64)(x) & INT64(0x000000000000ff00) ) << 40) | \
                 (unsigned Int64)(((unsigned Int64)(x) & INT64(0x0000000000ff0000) ) << 24) | \
                 (unsigned Int64)(((unsigned Int64)(x) & INT64(0x00000000ff000000) ) <<  8) | \
                 (unsigned Int64)(((unsigned Int64)(x) & INT64(0x000000ff00000000) ) >>  8) | \
                 (unsigned Int64)(((unsigned Int64)(x) & INT64(0x0000ff0000000000) ) >> 24) | \
                 (unsigned Int64)(((unsigned Int64)(x) & INT64(0x00ff000000000000) ) >> 40) | \
                 (unsigned Int64)(((unsigned Int64)(x) & INT64(0xff00000000000000) ) >> 56) ))


namespace MSerialization
{

/// Enumeration of different stream types
enum StreamType
{
	ST_MEMORY, ///!< Stream is serialized in memory
	ST_FILE,   ///!< Stream is serialized into a file
	ST_SHM	   ///!< Stream is serialized to shared memory
};

bool MoreThan32Bit(Int64 iData); 

const unsigned char GN_Data_Stream_Flag = 0xBE;

};

#endif // Serialization_Facilities_H
