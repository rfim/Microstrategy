//
//  CGCanvas.h
//  IPhoneChart
//
//  Created by Jin, Dong on 6/11/14.
//
//
#ifndef MsiChart_CGCanvas_h
#define MsiChart_CGCanvas_h

#import "ChartCanvas.h"

namespace MsiChart {
    
    class CGCanvas:public ChartCanvas{
    public:

        
        CGCanvas(ChartContext* ipChartContext);
        virtual ~CGCanvas();
		virtual bool SaveToString(std::string& orGraphDesc);
		/**
		 This function records the current status of the canvas, so that it can be restored later by calling RestoreState().
		 */
		virtual void SaveState();
		
		/**
		 This function restores the canvas to the status stored by the lastest call of SaveState() function.
		 */
		virtual void RestoreState();
		
		/**
		 Clips the canvas with the current path as its boundary.
		 */
		virtual void Clip();
		
		/**
		 Scales the canvas.
		 @param iX is the scaling factor for the horizontal axis.
		 @param iY is the scaling factor for the vertical axis.
		 */
		virtual void Scale(const double iX, const double iY);
		
		/**
		 Rotates the canvas.
		 @param irAnchor is the fixed point for the rotation.
		 @param iRadian is the angle (in terms of radian) of the rotation.
		 */
		virtual void Rotate(const Point2D& irAnchor, double iRadian);
        
		/**
		 Create a new path.
		 */
		virtual void NewPath();
		
		/**
		 Close the current path by LineTo() the start point of the path.
		 */
		virtual void ClosePath();
		
		/**
		 Move the cursor to the position specified by parameters.
		 @param iX specifies the X-axis value of the destination.
		 @param iY specifies the Y-axis value of the destination.
		 */
		virtual void MoveTo(double iX, double iY, bool iIsCurveAnimation = false, FPoint2D* iBasePoint = NULL);
        
		/**
		 Draw a line from the current postion of cursor to the position specified by parameters.
		 @param iX specifies the X-axis value of the destination.
		 @param iY specifies the Y-axis value of the destination.
		 */
		virtual void LineTo(double iX, double iY, bool iIsCurveAnimation = false, FPoint2D* iBasePoint = NULL);
        
		/**
		 Draw a cubic Bezier curve from the current postion of cursor to (iX, iY)
		 @param iCP1X specifies the X-axis value of the first control point.
		 @param iCP1Y specifies the Y-axis value of the first control point.
		 @param iCP2X specifies the X-axis value of the second control point.
		 @param iCP2Y specifies the Y-axis value of the second control point.
		 @param iX specifies the X-axis value of the end point.
		 @param iY specifies the Y-axis value of the end point.
		 */
		virtual void BezierCurveTo(double iCP1X, double iCP1Y, double iCP2X, double iCP2Y, double iX, double iY);
        
		/**
		 Draw a rectangle.
		 @param iX specifies the X-axis value of the top-left corner of the rectangle.
		 @param iY specifies the Y-axis value of the top-left corner of the rectangle.
		 @param iW specifies the width of the rectangle.
		 @param iH specifies the height of the rectangle.
		 */
		virtual void AddRect(float iX, float iY, float iW, float iH);
        
		/**
		 Draw an arc, specified by the center and radius of the corresponding circle, along with the starting and end angle of the arc.
		 @param iX specifies the X-axis value of center of the circle.
		 @param iY specifies the Y-axis value of center of the circle.
		 @param iRadius specifies the radius of the circle.
		 @param iStartAngle specifies the start angle.
		 @param iEndAngle specifies the end angle.
		 @param bAntiClockwise specifies whether the arc is drawn anti-clockwise from the start angle to the end angle.
		 */
		virtual void AddArc(float iX, float iY, float iRadius, float iStartAngle, float iEndAngle, bool bAntiClockwise);
        
        /**
		 This instruction is for the animation of Gauge style chart. The animation route of a gauge needle is a rotation around the base point.
		 So a simple interpolation of the starting and end frame does not suffice.
		 This instruction passed adequate parameters of a needle object so that the GUI can reconstruct the needle and its animation.
		 @param iNeedleStyle is an enumeration named EnumDSSGraphGaugeNeedleStyle
		 @param iBasePoint specifies the fixed point of the rotation
		 @param iAngle is the current angle (in radian) of the needle
		 @param iSize is the size of the needle
		 */
		virtual void AddNeedle(char iNeedleStyle, Point2D iBasePoint, short iAngle, float iSize);
		
		/**
		 Add a string.
		 @param irPoint specifies the top-left corner of the string.
		 @param ipString points to text being drawn.
		 */
		virtual void AddString(const Point2D& irPoint, const char* ipString);
        
		/**
		 Set the patterns for dashed stroke line.
		 @param ipLinePattern is an array of numbers, indicating the alternating lengths of lines and blanks.
		 @param iDashNumber is length of the array.
		 */
		virtual void SetLineDash(double* ipLinePattern, short iDashNumber);
		
		/**
		 Set the width of stroke line.
		 @param iLineWidth is the width in terms of pixels.
		 */
		virtual void SetLineWidth(double iLineWidth);
        
		/**
		 Set the font size.
		 @param iFontSize is the size in terms of pt.
		 */
		virtual void SetFontSize(short iFontSize);
        
		/**
		 Set the line join type.
		 @param iLineJoinType is the desired line join type.
		 */
		virtual void SetLineJoin(EnumLineJoinType iLineJoinType);
		/**
		 Set the fill color and transparency.
		 @param iRed is brightness of red color with its value in the range of [0,1].
		 @param iGreen is brightness of green color with its value in the range of [0,1].
		 @param iBlue is brightness of blue color with its value in the range of [0,1].
		 @param iAlpha is transparency with its value in the range of [0,1].
		 */
		virtual void SetRGBAFillColor(float iRed, float iGreen, float iBlue, float iAlpha);
        
		/**
		 Set the stroke color and transparency.
		 @param iRed is brightness of red color with its value in the range of [0,1].
		 @param iGreen is brightness of green color with its value in the range of [0,1].
		 @param iBlue is brightness of blue color with its value in the range of [0,1].
		 @param iAlpha is transparency with its value in the range of [0,1].
		 */
		virtual void SetRGBAStrokeColor(float iRed, float iGreen, float iBlue, float iAlpha);
        
		/**
		 Fill the current path with the fill format specified before.
		 */
		virtual void FillPath();
        
		/**
		 Stroke the current path with the fill format specified before.
		 */
		virtual void StrokePath();
        
		/**
		 Fill the current path with the simple fill color in @param ipFill.
		 @param ipFill is a pointer to the FormatFill object being used.
		 */
		virtual void FillSimple(const FormatFill* ipFill, float iDarkerFactor, RGBColor* iColor = NULL, bool iHasExtraPara = false);
        
		/**
		 Fill the current path with the fill pattern in @param ipFill.
		 @param ipFill is a pointer to the FormatFill object being used.
		 */
		virtual void FillPattern(const FormatFill* ipFill);
        
		/**
		 Fill the current path with the gradient in @param ipFill.
		 @param ipFill is a pointer to the FormatFill object being used.
		 @param irBoundingRect is a rectangle specifying the boundaries where starting and end colors of the gradient is applied.
		 */
		virtual void FillGradient(const FormatFill* ipFill, const Rect2D& irBoundingRect, float iDarkerFactor);
        
		/**
		 This method deals with smooth edge and chisel edge type of bevel.
		 @param ipFill is a pointer to the FormatFill object being used.
		 @param irPolygon is a clockwise polygon corresponding to current path.
		 */
		virtual void FillEdgeType(const FormatFill* ipFill, const std::vector<Point2D>& irPolygon);
		
		/**
		 This method deals with radient gradient for pie slice and ring.
		 @param ipFill is a pointer to the FormatFill object being used.
		 @param iRGUsage is usage of radient gradient.
         **/
		virtual void FillRadientGradient(const FormatFill* ipFill, EnumRadientGradientUsage iRGUsage, double iCPX, double iCPY, double iInnerRadius, double iOuterRadius, bool iHasExtraPara = false, int iExtraDepth = 0, int iExtraIntensity = 0);
		
		/**
		 This method deals with gauge border style - metallic.
		 @param ipFill is a pointer to the FormatFill object being used.
		 @param iCPX iCPY is center point.
		 @param iRadius is the border ridus.
		 @param iRadius is the border thickness.
		 */
		virtual void FillBorderMetallic(const FormatFill* ipFill, short iBorderUse, double iCPX, double iCPY, double iRadius, double iThickness);
        
		/**
		 This method deals with donut and sphere type of bevel.
		 @param ipFill is a pointer to the FormatFill object being used.
		 @param irBoundingRect is the bounding rectangle of current path.
		 @param iIsCircularShape specifies whether current object is a circular ShapeObject or not.
		 */
		virtual void FillDonutOrSphere(const FormatFill* ipFill, const Rect2D& irBoundingRect, bool iIsCircularShape);
		
		/**
		 Makes current FormatFill darker.
		 This method is used in pseudo 3D effect.
		 Each color component in current FormatFill will be multiplied by iFactor.
		 @param ipFill points to the FormatFill object being used.
		 @param iFactor is the darkening factor in the range of [0, 1], where 1.0 means no darkening at all and 0.0 means darken to pure black.
		 */
		virtual void MakeDarker(const FormatFill* ipFill, float iFactor);
		
		/**
		 Restores the fill color darkened by the MakeDarker() function, it is equivalent to MakeDarker(ipFill, 1.0)
		 @param ipFill points to the FormatFill object being used.
		 */
		virtual void RestoreColor(const FormatFill* ipFill);
		
		/**
		 Sets the stroke style according to the input FormatLine object.
		 @param ipLine points to the FormatLine object to be used.
		 */
		virtual void ApplyLineFormat(const FormatLine* ipLine);
		
		/**
		 Adds a string with specified format.
		 @param ipFont points to a FormatFont object that specifies the format of the text.
		 @param irStartPoint specifies where the top left corner of the text is being drawn.
		 @param irText stores the text to be drawn.
		 */
		virtual void AddFormatedText(const FormatFont* ipFont, const Point2D& irStartPoint, const std::string& irText);
		
		/**
		 Applies beveled frame effect to a rectangle.
		 @param irRect is the input rectangle.
		 @param iRotationAngle specifies the angle at which the resulting rectangle is rotated.
		 @param iBevelDepth specifies the depth at the beveled frame.
		 @param irLTColor specifies the color at the left top part of the beveled frame.
		 @param iLTAlpha specifies the transparency at the left top part of the beveled frame.
		 @param irRBColor specifies the color at the right bottom part of the beveled frame.
		 @param iRBAlpha specifies the transparency at the right bottom part of the beveled frame.
		 */
		virtual void ApplyBeveledFrameToRectangle(const Rect2D& irRect, double iRotationAngle, short iBevelDepth,
												  const RGBColor& irLTColor, short iLTAlpha,
												  const RGBColor& irRBColor, short iRBAlpha);
		
		//Draw gauge marker
		virtual void DrawGaugeMarker(FPoint2D* iBasePoint, Int32& iMarkerSize, Int32& iMarkerShape, float iAngle, short iNeedSize);
        
		// fill donut to rectangle
		virtual void ApplyDonutToRect(const Point<double>& irStartPoint, const Point<double>& irEndPoint, const FormatFill* ipFill);
        
		/**
		 Used to facilitate animation.
		 All instructions sent to the canvas after calling this function and before calling the EndFrame() function
		 are meant to describe the "first frame", i.e. the frame displayed in the beginning of animation.
		 */
		virtual void BeginFirstFrame();
		
		/**
		 Used to facilitate animation.
		 All instructions sent to the canvas after calling this function and before calling the EndFrame() function
		 are meant to describe the "last frame", i.e. the frame displayed at the end of animation.
		 */
		
		virtual void BeginLastFrame();
		
		/**
		 Used to facilitate animation.
		 This function is called to signal the end of a frame.
		 */
		virtual void EndFrame();
        
		/**
		 This function tries to close a drawing unit.
		 A drawing unit may consists of one (when no animation is required) or two (when animation is needed) frames.
		 This function makes sure that the current unit is valid, i.e. containing enough frames, and closes it.
		 */
		virtual void EndUnit();
        
        CGPoint GetCurrentPoint();
    };
}

#endif
