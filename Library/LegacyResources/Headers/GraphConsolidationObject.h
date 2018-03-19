//
//  GraphConsolidationObject.h
//  IPhoneChart
//
//  Created by Shi, Dong on 12/17/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef IPhoneChart_GraphConsolidationObject_h
#define IPhoneChart_GraphConsolidationObject_h

#include "PDCHeader/PDCvector"
#include "../Dataset.h"
#include "GraphCollectionObject.h"

namespace MsiChart
{	
	/**
     Maintains a list of GraphObjects and for the caller they are seemed to be "merged" as one object. 
     User should not use this one as shared collection of line/fill format.
     */
	class GraphConsolidationObject : public GraphCollectionObject
	{
	public:
		typedef MSynch::SmartPtr<GraphConsolidationObject> Ptr;
        
        GraphConsolidationObject(const TripleId& irTripleId, GraphObjectManager* ipManager, EnumCollectionType iCollectionType, Dataset::Ptr iDatasetPtr);
		virtual ~GraphConsolidationObject();

        GraphObject::Ptr GetPrimaryObject(const Point2D& irPoint);
        GraphObject::Ptr GetPrimaryObject() const;
        GraphObject::Ptr GetExclusiveObject() const;  //Get the only object from the list.
        
		virtual void Draw() const;
		virtual void GenerateImageMap(ImageMap& iorImageMap);
        virtual void DrawOutLine();

        virtual void GetBoundingRect(Rect2D& orBoundingRect) const;		

        /** This function will add a GraphObject into current GraphCollectionObject. 
         @param iGraphObjectPtr Pointer to the GraphObject which is ready to be added into current GraphCollectionObject.
         Current GraphCollectionObject will be the parent object of iGraphObjectPtr. 		
         @param iShareFormat Bool value indicate whether child object will share the format information from parent.		
         */
		void AddGraphObject(GraphObject::Ptr iGraphObjectPtr, bool iShareFormat = false);
        
		/**	Clear mGraphObjectCollection.		
         */
		void ClearCollection();

        virtual void GetClockwisePolygon(std::vector<Point2D>& orPolygon) const; 
        
        void SetDrawObjects(bool iIsDraw);
        bool IsDrawObjects() const;
        
    protected:    
        
        Rect2D hGetBoundingRect() const;
        Polygon2D hGetConsolidatePolygon() const;
        bool mNeedDrawObjects;
        Int32 mPrimaryObjectIndex;
        
        mutable bool mIsValidRect;
        mutable bool mIsValidPolygon;
        mutable Rect2D mRect;
        mutable Polygon2D mConsolidatePolygon;
        Dataset::Ptr mDatasetPtr;                       //to make this class different from collection object.
    };
    
    inline void GraphConsolidationObject::SetDrawObjects(bool iIsDraw)
    {
        mNeedDrawObjects = iIsDraw;
    }
    
    inline bool GraphConsolidationObject::IsDrawObjects() const
    {
        return mNeedDrawObjects;
    }
}

#endif
