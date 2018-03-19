//==============================================================================================
//	FILENAME	:	Rect2D.h
//	AUTHOR		:	Liang Liu
//	CREATION	:	2008-09-25
//	Copyright (C) MicroStrategy Incorporated 2008
//	All Rights Reserved
//==============================================================================================

#ifndef MsiChart_Rect2D_h
#define MsiChart_Rect2D_h

#include "Point2D.h"

namespace MsiChart
{	
	typedef enum
	{
		REC_EXP_INVALID = 0,
		REC_EXP_HRIZONTAL = 1,
		REC_EXP_VERTICAL = 2,
		REC_EXP_BOTH = 3
	}REC_EXP_DIRECTION;

	/// Use four Int32 values to represent a 2D rectangle.
	class  Rect2D
	{
	public:
		/**
		Constructs from four Int32 values.
		@param iX X-coordinate of left edge.
		@param iY Y-coordinate of top edge.
		@param iWidth Width of current rectangle.
		@param iHeight Height of current rectangle.
		*/
		Rect2D(int iX, int iY, 
			Int32 iWidth, Int32 iHeight) : x(iX),y(iY),width(iWidth),height(iHeight){}
		
		/// Constructs without parameters. All data members will be initialized as 0.
		Rect2D(): x(0), y(0), width(0), height(0){}		

		/**
		Explode current Rect2D object.
		@param iXOffset x=x-iXOffset, width=width+2*iXOffset.
		@param iYOffset y=y-iXOffset, height=height+2*iYOffset.
		*/
		void Explode(Int32 iXOffset, Int32 iYOffset);
		
		/**
		Expand currect Rect2D on specified direction
		*/
		void Expand(Int32 iOffset, REC_EXP_DIRECTION iExpDirection);

		/**
		Implode current Rect2D object.
		@param iXOffset x=x+iXOffset, width=width-2*iXOffset.
		@param iYOffset y=y+iXOffset, height=height-2*iYOffset.
		*/
		void Implode(Int32 iXOffset, Int32 iYOffset);

		/**
		Get top left vertex of current rectangle.
		@return The top left vertex.
		*/
		Point2D GetTopLeft() const;
		/**
		Get bottom left vertex of current rectangle.
		@return The bottom left vertex.
		*/
		Point2D GetBottomLeft() const;
		/**
		Get top right vertex of current rectangle.
		@return The top right vertex.
		*/
		Point2D GetTopRight() const;
		/**
		Get bottom right vertex of current rectangle.
		@return The bottom right vertex.
		*/
		Point2D GetBottomRight() const;
		/**
		Get the center point of current rectangle.
		@return The center point of current rectangle.
		*/
		Point2D GetCenter() const;
		/**
		Get Left Middle vertex of current rectangle.
		@return The left middle vertex.
		*/
		Point2D GetLeftMiddle() const;
		/**
		Get Top middle vertex of current rectangle.
		@return The top middle vertex.
		*/
		Point2D GetTopMiddle() const;
		/**
		Get right Middle vertex of current rectangle.
		@return The right middle vertex.
		*/
		Point2D GetRightMiddle() const;
		/**
		Get bottom middle vertex of current rectangle.
		@return The bottom middle vertex.
		*/
		Point2D GetBottomMiddle() const;
		/**
		Judge whether two rectangle intersected.
		@param irRect Specifies another rectangle.
		@return true if intersected, false if not.
		*/
		bool HasIntersection(const Rect2D& irRect) const;
		/**
		Check whether a point is inside current rectangle or not.
		@param irPoint Specifies a point.
		@return Return true if irPoint point is inside current rectangle. Otherwise, return false.
		*/
		bool PointInRectangle(const Point2D& irPoint) const;

		void Reset(int iX, int iY, Int32 iWidth, Int32 iHeight);
	
        bool IsEqualTo(Rect2D& irRect) const;
	public:	
		int x;		///< X-coordinate of left edge.
		int y;		///< Y-coordinate of left edge.
		Int32 width;	///< Width of current rectangle.
		Int32 height;	///< Height of current rectangle.
	};

	inline void  Rect2D::Explode(Int32 iXOffset, Int32 iYOffset)
	{
		x -= iXOffset;
		y -= iYOffset;
		width += (iXOffset*2);
		height += (iYOffset*2);
	}

	inline void Rect2D::Expand(Int32 iOffset, REC_EXP_DIRECTION iExpDirection)
	{
//		if (iOffset <= 0)
//			return;
		
		if (iExpDirection & REC_EXP_HRIZONTAL)
		{
			x -= iOffset;
			width += 2 * iOffset;
		}
		
		if (iExpDirection & REC_EXP_VERTICAL)
		{
			y -= iOffset;
			height += 2 * iOffset;
		}
	}

	inline void Rect2D::Implode(Int32 iXOffset, Int32 iYOffset)
	{
		if (iXOffset * 2 > width)
		{
			iXOffset = width / 2;
		}
		if (iYOffset * 2 > height)
		{
			iYOffset = height / 2;
		}
		Rect2D::Explode(-iXOffset, -iYOffset);
	}

	inline Point2D Rect2D::GetTopLeft() const
	{
		return Point2D(x, y);
	}
	inline Point2D Rect2D::GetBottomLeft() const
	{
		return Point2D(x, y + height);
	}
	inline Point2D Rect2D::GetTopRight() const
	{
		return Point2D(x + width, y);
	}
	inline Point2D Rect2D::GetBottomRight() const
	{
		return Point2D(x + width, y + height);
	}
	inline Point2D Rect2D::GetCenter() const
	{
		return Point2D(x + width / 2, y + height / 2);
	}
	
	inline Point2D Rect2D::GetLeftMiddle() const
	{
		return Point2D(x, y + height / 2);
	}
		
	inline Point2D Rect2D::GetTopMiddle() const
	{
		return Point2D(x + width / 2, y);
	}
		
	inline Point2D Rect2D::GetRightMiddle() const
	{
		return Point2D(x + width, y + height / 2);
	}
	
	inline Point2D Rect2D::GetBottomMiddle() const
	{
		return Point2D(x + width / 2, y + height);
	}
	inline bool Rect2D::HasIntersection(const Rect2D& irRect) const
	{
		return x < (irRect.x + irRect.width) && irRect.x < (x + width) &&
			y < (irRect.y + irRect.height) && irRect.y < (y + height);
	}

	inline bool Rect2D::PointInRectangle(const Point2D& irPoint) const
	{
		return ((irPoint.x >= x) && (irPoint.x <= x + width)
			&& (irPoint.y >= y) && (irPoint.y <= y + height));
	}

	inline void Rect2D::Reset(int iX, int iY, Int32 iWidth, Int32 iHeight)
	{
		x = iX;
		y = iY;
		width = iWidth;
		height = iHeight;
	}
    
    inline bool Rect2D::IsEqualTo(Rect2D& irRect) const
    {
        if (x != irRect.x || y != irRect.y)
            return false;
        if (width != irRect.width || height != irRect.height)
            return false;
        return true;
    }

}
#endif
