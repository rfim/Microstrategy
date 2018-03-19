//==============================================================================================
//	FILENAME	:	RGBColor.h
//	AUTHOR		:	Liang Liu, Bing Huang
//	CREATION	:	2008-09-25
//	Copyright (C) MicroStrategy Incorporated 2008
//	All Rights Reserved
//==============================================================================================

#ifndef MsiChart_RGBColor_h
#define MsiChart_RGBColor_h


namespace MsiChart
{	
	struct HSLColor;

	/** Use three unsigned char to represent a color in RGB space. 
	This class also deals with the conversion between RGB space and HSL space.
	See http://en.wikipedia.org/wiki/HSL_color_space for reference.
	*/
	class RGBColor
	{
	public:
		/** 
		Constructs from three unsigned char values in range [0, 255].
		@param iRed Red component.
		@param iGreen Green component.
		@param iBlue Blue component.
		*/
		RGBColor(unsigned char iRed, unsigned char iGreen, unsigned char iBlue);
		
		/** 
		Constructs from an integer.
		@param iColor Represents a color using a 32-bit integer. The lowest 8 bits represent red component. 
		The next 8 bits represent green component. The next 8 bits represent blue component. 
		*/
		RGBColor(Int32 iColor = 0);
		/**
		Constructs from HSLColor.
		@param irHSL Represents a color in HSL space.
		*/
		RGBColor(const HSLColor& irHSL);

		/**
		Checks whether two RGBColor object have the same value.
		@irColor Represents another RGBColor object.
		@return Return true if any component value is not equal to the corresponding component value in irColor. 
		Otherwise, return false.
		*/
		bool operator != (const RGBColor& irColor) const;

		/**
		Using a 32-bit integer to represent current RGBColor.
		@return Represents a color using a 32-bit integer. The lowest 8 bits represent red component. 
		The next 8 bits represent green component. The next 8 bits represent blue component.
		*/
		Int32 ToInteger() const;

		/**
		Convert current color from RGB space to HSL space.
		See http://en.wikipedia.org/wiki/HSL_color_space for reference.
		@param orHSL This output parameter represents a color in HSL space.
		*/
		void ToHSL(HSLColor& orHSL) const;

		/**
		Make current color darker.
		This method is used in pseudo 3D effect.
		@param iFactor Should be a value in [0, 1]. 
		Each component in current color will be multiplied by iFactor.
		*/
		void MakeDarker(float iFactor);

		/**
		Set new components for current color.
		@param iRed Red component.
		@param iGreen Green component.
		@param iBlue Blue component.
		*/
		void Reset(unsigned char iRed, unsigned char iGreen, unsigned char iBlue);
        
        static RGBColor* GetGMCustomerPalette();
        static int GetCustomerPaletteNumber();
        static void SetGMCustomerPalette(int* iPalette, int iNumber);
        

	public:
		unsigned char r;	///< Red component.
		unsigned char g;	///< Green component.
		unsigned char b;	///< Blue component.

	private:
		/**
		This is a helper method of ToHSL().
		@param ioF If ioF<0, ioF=ioF+1. If ioF>1, ioF=ioF-1.
		*/
		void hRoundFloat(float& ioF) const;	
		/**
		This is a helper method of ToHSL().
		@param iP Intermediate variable.
		@param iQ Intermediate variable.
		@param iT Intermediate variable.
		@return One RGBColor component in range [0, 1].
		*/
		float hGetColorRGB(float iP, float iQ, float iT) const;
        
        static RGBColor* mGMCustomerColorPalette;
        static int mGMCustomerColorPaletteNumber;
	};
    

	extern const RGBColor gColorPalette[];
	extern const RGBColor gColorPaletteForTimeSeries[];
	extern const RGBColor gColorPaletteForGraphMatrix[];
    extern const RGBColor gColorArrayLightTheme[];
    extern const RGBColor gColorArrayDarkTheme[]; 

	inline bool RGBColor::operator != (const RGBColor& irColor) const
	{
		return ((r != irColor.r) || (g != irColor.g) || (b != irColor.b));
	}

#ifdef _VE_CE
	inline Int32 RGBColor::ToInteger() const
	{
		return b + (g << 8) + (r << 16);
	}
#else	
	inline Int32 RGBColor::ToInteger() const
	{
		return r + (g << 8) + (b << 16);
	}
#endif

	inline void RGBColor::MakeDarker(float iFactor)
	{
		r = static_cast<unsigned char>(r * iFactor);
		g = static_cast<unsigned char>(g * iFactor);
		b = static_cast<unsigned char>(b * iFactor);
	}

	inline void RGBColor::Reset(unsigned char iRed, unsigned char iGreen, unsigned char iBlue)
	{
		r = iRed;
		g = iGreen;
		b = iBlue;
	}

	/// Use three unsigned char to represent a color in HSL space. 
	struct HSLColor
	{	
		float mH;	///< Hue component.
		float mS;	///< Saturation component.
		float mL;	///< Lightness component.
	};

}
#endif
