//==============================================================================================
//	FILENAME	:	CuboidObject.h
//	AUTHOR		:	Liang Liu
//	CREATION	:	2009-05-18
//	Copyright (C) MicroStrategy Incorporated 2009
//	All Rights Reserved
//==============================================================================================
#ifndef MsiChart_CuboidObject_h
#define MsiChart_CuboidObject_h

#include "PDCHeader/PDCvector"
#include "Shape3DObject.h"
#include "Transformer.h"
#include "Axis.h"

namespace MsiChart
{	
	/**
	Indicates the subtypes of cuboid object which is supported by CuboidObject class. Generally, each of these 3D objects can be determined by two 
	opposite vertices of the bounding cuboid of that object.
	*/
	typedef enum  
	{
		CS_BOX,		///< Default cuboid object is a box.
		CS_PYRAMID,	///< Assume that the bottom face will always be perpendicular to Y axis.
		CS_SPHERE,	///< If the 8 corners of a cube is cut at the middle points of the edges, we get a sphere object.
		CS_OCTAGON, ///< A prism which has an octagon as its bottom face.
		CS_CUT_CORNER,	///< A prism which has a five-edge polygon as its bottom face. This five-edge polygon comes from cutting one corner of a square.
		CS_PLUS_SYMBOL_THIN, 
		CS_PLUS_SYMBOL_THICK,
		CS_X_SHAPE,
		CS_TETRAHEDRON,
		CS_STAR
	} CuboidSubtype;	

	/* Extra parameter for CS_BOX.
	*/
	typedef enum
	{
		BST_ALL,
		BST_WITHOUT_X_SIDE,
		BST_WITHOUT_Y_SIDE,
		BST_WITHOUT_Z_SIDE
	} BoxSurfaceType;

	/// This class implements cuboid object. 
	class CuboidObject: public Shape3DObject
	{
	public:
		typedef MSynch::SmartPtr<CuboidObject> Ptr;

		/** Constructor
		@param irTripleId,ipManager,ipParentObject,iHasFill Required by base class.		
		@param irStartPoint,irEndPoint Specifies a cuboid using these two opposite vertices.
		For pyramid, irEndVertex should be on the bottom face.
		*/
		CuboidObject(const TripleId& irTripleId, GraphObjectManager* ipManager, const DPoint3D& irStartVertex, const DPoint3D& irEndVertex,
			GraphCollectionObject* ipParentObject = NULL, bool iHasFill = true, Int32 iCuboidSubtype = CS_BOX, Int32 iBoxSurfaceType = BST_ALL); 
		virtual ~CuboidObject();

		/**
		Generate grid lines for X axis.
		@param irGridLinePositions Specifies the positions of the grid lines.
		@param ipGridCollection Specifies the collection object of the grid lines.
		@param iorGraphObjectList The generated grid lines will be appended to this list.
		*/
		void GenerateGridLinesForXAxis(const std::vector<double>& irGridLinePositions, GraphCollectionObject* ipGridCollection,
			std::vector<GraphObject::Ptr>& iorGraphObjectList) const;

		/**
		Generate grid lines for Y axis.
		@param irGridLinePositions Specifies the positions of the grid lines.
		@param ipGridCollection Specifies the collection object of the grid lines.
		@param iorGraphObjectList The generated grid lines will be appended to this list.
		*/
		void GenerateGridLinesForYAxis(const std::vector<double>& irGridLinePositions, GraphCollectionObject* ipGridCollection,
			std::vector<GraphObject::Ptr>& iorGraphObjectList) const;

		/**
		Generate grid lines for Z axis.
		@param irGridLinePositions Specifies the positions of the grid lines.
		@param ipGridCollection Specifies the collection object of the grid lines.
		@param iorGraphObjectList The generated grid lines will be appended to this list.
		*/
		void GenerateGridLinesForZAxis(const std::vector<double>& irGridLinePositions, GraphCollectionObject* ipGridCollection,
			std::vector<GraphObject::Ptr>& iorGraphObjectList) const;

		/**
		Generate grid lines for X axis.
		@param irGridLinePositions Specifies the positions of the grid lines.
		@param ipGridCollection Specifies the collection object of the grid lines.
		@param iorGraphObjectList The generated grid lines will be appended to this list.
		*/
		void GenerateInterlacedGridsForXAxis(const std::vector<double>& irGridLinePositions, GraphCollectionObject* ipGridCollection,
			std::vector<GraphObject::Ptr>& iorGraphObjectList) const;
		/**
		Generate grid lines for Y axis.
		@param irGridLinePositions Specifies the positions of the grid lines.
		@param ipGridCollection Specifies the collection object of the grid lines.
		@param iorGraphObjectList The generated grid lines will be appended to this list.
		*/
		void GenerateInterlacedGridsForYAxis(const std::vector<double>& irGridLinePositions, GraphCollectionObject* ipGridCollection,
			std::vector<GraphObject::Ptr>& iorGraphObjectList) const;
		/**
		Generate grid lines for Z axis.
		@param irGridLinePositions Specifies the positions of the grid lines.
		@param ipGridCollection Specifies the collection object of the grid lines.
		@param iorGraphObjectList The generated grid lines will be appended to this list.
		*/
		void GenerateInterlacedGridsForZAxis(const std::vector<double>& irGridLinePositions, GraphCollectionObject* ipGridCollection,
			std::vector<GraphObject::Ptr>& iorGraphObjectList) const;


	private:
		void hAddFacesForCuboid(Int32 iBoxSurfaceType);	
		void hAddFacesForPyramid();
		void hAddHalfFacesForSphere();
		void hAddFacesForOctagon();
		void hAddFacesForCutCorner();
		void hAddFacesForPlus();
		void hAddFacesForTetrahedron();
		void hAddFacesForStar();
		void hPreprocessVertices();
		DPoint3D hCoordTransform(const DPoint3D& irPoint) const;
		DPoint3D hReverseCoordTransform(const DPoint3D& irPoint) const;
		void hProcessQuadrangle(const DPoint3D& irA, const DPoint3D& irB, const DPoint3D& irC, const DPoint3D& irD, const DVector3D& irNormal);
		void hProcessTriangle(const DPoint3D& irA, const DPoint3D& irB, const DPoint3D& irC, const DVector3D& irNormal);
		void hCreateOneGridLine(const DPoint3D& irStartPoint, const DPoint3D& irEndPoint, GraphCollectionObject* ipGridCollection,
			std::vector<GraphObject::Ptr>& iorGraphObjectList) const;
		void hCreateOneGridBox(const DPoint3D& irStartPoint, const DPoint3D& irEndPoint, GraphCollectionObject* ipGridCollection,
			std::vector<GraphObject::Ptr>& iorGraphObjectList, Int32 iBoxSurfaceType) const;

		DPoint3D mStartVertex;
		DPoint3D mEndVertex;
		Int32 mCuboidSubtype;
		Transformer* mpTransformer;		
	};		
}
#endif
