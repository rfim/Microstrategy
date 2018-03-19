//## begin module%331B834903AD.cm preserve=no
//	$Revision$
//## end module%331B834903AD.cm

//## begin module%331B834903AD.cp preserve=no
//	CopyRight  (c)  1997  MicroStrategy Inc.
//## end module%331B834903AD.cp

//## Module: DFCAttrGroup%331B834903AD; Pseudo Package specification
//## Source file: Z:\Engine\SourceCode\Attribute\DFCAttrGroup.h

#ifndef DFCAttrGroup_h
#define DFCAttrGroup_h 1

//## begin module%331B834903AD.additionalIncludes preserve=no
//## end module%331B834903AD.additionalIncludes

//## begin module%331B834903AD.includes preserve=yes















// DFCAttr
#ifndef __ANDROID__
#include "Attribute/DFCAttr.h"
#else
#include "../../../../Kernel/IPhone/DFCEngineDaemon/Attribute/DFCAttr.h"
#endif

#include "Includes/DFCSchema_Includes.h"
#include "Includes/DFCLog.h"
#include "SEH/DFCSchemaException.h"
#include "SEH/DFCSchemaLogicException.h"

//## end module%331B834903AD.includes

// DFCSchemaInterface
// map
#include "DataStructure/map.h"
// DFCBitVectorBase
#include "DataStructure/DFCBitVectorBase.h"
// vector
#include "DataStructure/vector.h"

//## begin module%331B834903AD.additionalDeclarations preserve=yes
//class MBase::Buffer;
class DFCSchemaInterface;

class DLL_SCHEMA_EXIM DFCAttrGroup;

//class DFCAttrGroup;
//class DFCAttrGroup::Iterator;
//class DLL_SCHEMA_EXIM DFCAttrGroup::Iterator;
//## end module%331B834903AD.additionalDeclarations


//## begin DFCAttrGroup%331B834903AD.preface preserve=yes
//## end DFCAttrGroup%331B834903AD.preface

//## Class: DFCAttrGroup%331B834903AD
//	It is a set of attributes. Later, it should provide an
//	iterator, which is exactly the map::iterator as typedef
//	and GetAttrs will be off.
//	>= and <= are used to compare if one group is full
//	refinement of the other.
//	> and < are used to test if one group is at least
//	partially refined to the other.
//## Category: Attribute%3364DFBD008F
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%331B83970124;DFCAttr { -> }
//## Uses: <unnamed>%331C450202DC;vector { -> }
//## Uses: <unnamed>%331E2E65007D;map { -> }
//## Uses: <unnamed>%332F015A03A9;DFCBitVectorBase { -> }
//## Uses: <unnamed>%3986F07400A6;DFCSchemaInterface { -> }

class DFCAttrGroup
{
    //## begin DFCAttrGroup%331B834903AD.initialDeclarations preserve=yes
public:
    struct DFCAttrIdComp : public binary_function<DFCAttr *, DFCAttr *, bool>
    {
        bool operator()(const DFCAttr * const &iA,
                        const DFCAttr * const &iB) const
        {
            return iA->GetId() < iB->GetId();
        }
    };
    typedef	set<DFCAttr *, DFCAttrIdComp, MBase::Allocator<DFCAttr *> > SetDataType;
    typedef enum { LessThan = -1, EqualTo = 0, GreaterThan = 1 } AttrGroupCompRes;
    
    class DLL_SCHEMA_EXIM Iterator;
    //## end DFCAttrGroup%331B834903AD.initialDeclarations
    
public:
    //## begin DFCAttrGroup::Iterator%3384AA8E029D.preface preserve=yes
    //## end DFCAttrGroup::Iterator%3384AA8E029D.preface
    
    //## Class: Iterator%3384AA8E029D
    //## Category: Attribute%3364DFBD008F
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n
    
    class Iterator
    {
        //## begin DFCAttrGroup::Iterator%3384AA8E029D.initialDeclarations preserve=yes
    public:
        friend class DFCAttrGroup;
        //## end DFCAttrGroup::Iterator%3384AA8E029D.initialDeclarations
        
    public:
        //## Destructor (generated)
        ~Iterator();
        
        
        //## Other Operations (specified)
        //## Operation: operator*%3384AAD600D4
        DFCAttr* operator * ();
        
        //## Operation: operator++%3384AADD01CE
        Iterator& operator ++ (Int32 i);
        
        //## Operation: operator!=%3384AAE302DB
        bool operator != (const Iterator& iRight);
        
        //## Operation: operator==%3384AAE90294
        bool operator == (const Iterator& iRight);
        
        // Additional Public Declarations
        //## begin DFCAttrGroup::Iterator%3384AA8E029D.public preserve=yes
        //## end DFCAttrGroup::Iterator%3384AA8E029D.public
        
    protected:
        // Additional Protected Declarations
        //## begin DFCAttrGroup::Iterator%3384AA8E029D.protected preserve=yes
        //## end DFCAttrGroup::Iterator%3384AA8E029D.protected
        
    private:
        // Data Members for Class Attributes
        
        //## Attribute: mItr%3384AB4A03B6
        //## begin DFCAttrGroup::Iterator::mItr%3384AB4A03B6.attr preserve=no  private: SetDataType::iterator {UA}
        SetDataType::iterator mItr;
        //## end DFCAttrGroup::Iterator::mItr%3384AB4A03B6.attr
        
        // Additional Private Declarations
        //## begin DFCAttrGroup::Iterator%3384AA8E029D.private preserve=yes
        //## end DFCAttrGroup::Iterator%3384AA8E029D.private
        
    private: //## implementation
        // Additional Implementation Declarations
        //## begin DFCAttrGroup::Iterator%3384AA8E029D.implementation preserve=yes
        //## end DFCAttrGroup::Iterator%3384AA8E029D.implementation
        
    };
    
    //## begin DFCAttrGroup::Iterator%3384AA8E029D.postscript preserve=yes
    //## end DFCAttrGroup::Iterator%3384AA8E029D.postscript
    
    //## Constructors (generated)
    DFCAttrGroup(MBase::Buffer *iBuffer, const DFCAttrGroup &right);
    
    //## Constructors (specified)
    //## Operation: DFCAttrGroup%34E89F3000E0
    //	take input buffer to construct self.
    DFCAttrGroup (MBase::Buffer *iBuffer);
    
    //## Destructor (generated)
    virtual ~DFCAttrGroup();
    
    //## Assignment Operation (generated)
    const DFCAttrGroup & operator=(const DFCAttrGroup &right);
    
    //## Equality Operations (generated)
    Int32 operator==(const DFCAttrGroup &right) const;
    
    Int32 operator!=(const DFCAttrGroup &right) const;
    //Int32 GreaterThanOrEqualTo(const DFCAttrGroup &right) const;
    
    //## Other Operations (specified)
    //## Operation: InsertAttr%331DD0EB00D7
    //	Insert the attribute into this container, if the
    //	attribute is already there, map won't insert it anyway.
    //	so, there is no worry about the uniqueness of the each
    //	attribute in this container.
    virtual void InsertAttr (DFCAttr* iAttr);
    
    //## Operation: Print%331EDC120378
    virtual void Print (ostream& iOstream, DFCOutCtrlType iCtrl = OUT_DEFAULT) const;
    
    //## Operation: GetBitVector%3325A44A025A
    //	Returns an equivalent bit vector.  The size of the bit
    //	vector is the number of attributes.  The bits
    //	corresponding to the attributes present in this
    //	attribute group are set.
    void GetBitVector (DFCBitVectorBase &oBV, DFCSchemaInterface *ipSchema, MBase::Buffer* iBuffer) const;
    
    // wyuan; 757472; All of mAttrList, its ancestors and its descendents are included.
    void GetHierarchyBitVector(DFCBitVectorBase &oBV, DFCSchemaInterface *ipSchema, MBase::Buffer *ipBuffer) const;
    
    //## Operation: HasAttr%3342F05B02FC
    //	To test if the specified attribute in the AttrGroup or
    //	not.
    bool HasAttr (DFCAttr* iAttr) const;
    
    //## Operation: IteratorBegin%3342F11802DF
    //	Return the begin iterator of the internal set.
    DFCAttrGroup::Iterator IteratorBegin () const;
    
    //## Operation: IteratorEnd%3342F12F03B5
    //	Return the end iterator of the internal set.
    DFCAttrGroup::Iterator IteratorEnd () const;
    
    //## Operation: InsertAttrs%334A58C60338
    //	Insert attributes from input AttrGroup.
    void InsertAttrs (DFCAttrGroup* iAttrs);
    
    //## Operation: GenAttrGroup%334D465F006B
    //	This method takes in a bit vector and "adds" all the
    //	attributes contained in the bit vector to this attribute
    //	group.
    void GenAttrGroup (DFCBitVectorBase& iBV, DFCSchemaInterface &iSchema);
    
    //## Operation: Reduce%334D46C600F5
    //	This method implements the canonical reduction
    //	algorithm.  It reduces the given set of attributes i.e.
    //	the contents of the set after a call to this method is
    //	always a subset of the original set.
    void Reduce (DFCSchemaInterface  *ipSchema, MBase::Buffer* iBuffer);
    
    //## Operation: Reduce2HCD%33722CD80171
    //	Reduce the attribute group to HCDs.
    void Reduce2HCD (DFCSchemaInterface &iSchema, MBase::Buffer* iBuffer, bool iTransformationProcessing = false);
    
    //## Operation: Reduce2Highest%3767CE030113
    //	this method reduce the attributes in this group to the
    //	set of attributes (within this group) that are highest
    //	in its hierarchy.
    void Reduce2Highest (DFCSchemaInterface *ipSchema, MBase::Buffer* iBuffer);
    
    //## Operation: operator -=%33BAEDC401F3
    //	Do substraction of an attribute from itself.
    DFCAttrGroup & operator -= (DFCAttr *iAttr);
    
    //## Operation: operator -=%33BAEEBA00B7
    //	Do substraction another group of attributes from itself.
    DFCAttrGroup & operator -= (const DFCAttrGroup &iAttrs);
    
    //## Operation: operator +=%33BAEED40186
    //	Do union of an attribute to itself.
    DFCAttrGroup & operator += (DFCAttr *iAttr);
    
    //## Operation: operator +=%33BAEF1D036C
    //	Do union of another group of attributes to itself.
    DFCAttrGroup & operator += (const DFCAttrGroup &iAttrs);
    
    //## Operation: Empty%33BBB68203BB
    //	Test if the attribute group is empty or not.
    bool Empty () const;
    
    //## Operation: Compare%33C2689D0099
    //	Return -1, 0 or 1 standing for Lessthan, EqualTo or
    //	GreaterThan.
    Int32 Compare (const DFCAttrGroup &iAttrs) const;
    
    //## Operation: AddRef%33C3A7900001
    //	Increase reference count by 1.
    void AddRef ();
    
    //## Operation: Release%33C3A7910234
    //	Decrease reference count by 1 and if it reach zero, call
    //	destrcutor to destroy itself.
    void Release ();
    
    //## Operation: Above%33FE0E3B02B4
    //	to check if all the elements of this group are above the
    //	corresponding member of input group, or not related to
    //	any member of the input group.
    bool Above (const DFCAttrGroup &iAttrs, DFCSchemaInterface *ipSchema, MBase::Buffer* iBuffer) const;
    
    //## Operation: HasAttrBelow%34058BE1013A
    //	Test if this has any attribute below any member of the
    //	given attribute group.
    bool HasAttrBelow (const DFCAttrGroup &iAttrs, DFCSchemaInterface *ipSchema, MBase::Buffer* iBuffer) const;
    
    //## Operation: OpLessThan%34E35CF701F0
    bool OpLessThan (const DFCAttrGroup& iRight, DFCSchemaInterface *ipSchema, MBase::Buffer* iBuffer);
    
    //## Operation: OpGreaterThan%34E35D4E01F6
    bool OpGreaterThan (const DFCAttrGroup& iRight, DFCSchemaInterface *ipSchema, MBase::Buffer* iBuffer);
    
    
    bool LessThanOrEqualTo(const DFCAttrGroup &right, DFCSchemaInterface  *ipSchema, MBase::Buffer* iBuffer) const;
    bool GreaterThanOrEqualTo(const DFCAttrGroup &right, DFCSchemaInterface  *ipSchema, MBase::Buffer* iBuffer) const;
    //## Operation: OR%34E3606703CC
    bool OR (const DFCAttrGroup& iAttrs, DFCSchemaInterface *ipSchema, MBase::Buffer* iBuffer);
    
    //## Operation: AnyRelated%354A3923037B
    //	This method detect if any of the element in this group
    //	is related, in any way, to any element in input group.
    bool AnyRelated (DFCAttrGroup &iAttrs, DFCSchemaInterface *ipSchema, MBase::Buffer *iBuffer);
    
    //## Operation: RefineRelated%354A397B0074
    //	This method return true if each of the element in this
    //	group is related, in refinement relationship either
    //	direction, to any element in input group.
    bool RefineRelated (DFCAttrGroup &iAttrs, DFCSchemaInterface *ipSchema, MBase::Buffer *iBuffer);
    
    //## Operation: AnyRefineRelated%3A282D4B00FA
    //	This method return true if any one of the element in
    //	this group is related, in refinement relationship either
    //	direction, to any element in input group.
    bool AnyRefineRelated (DFCAttrGroup &iAttrs, DFCSchemaInterface *ipSchema, MBase::Buffer *iBuffer);
    
    //## Operation: Reduce%354F317802FA
    //	This method does the reduction according to the VLDB
    //	setting.
    void Reduce (VLDBInfo* iVLDBInfo, DFCSchemaInterface *ipSchema, MBase::Buffer *iBuffer);
    
    //## Operation: InsertAttrs%357337B600A8
    //	Inserts all attributes from the given vector of
    //	attributes
    void InsertAttrs (vector<DFCAttr*, MBase::Allocator<DFCAttr*> >& iAttrVec);
    
    //## Operation: NeedRaise%358AC3960274
    //	This detects if the attribute group can be refined to
    //	the input.
    bool NeedRaise (DFCAttrGroup &iFilteringKey, DFCAttrGroup &iIgnoringAttrs, DFCSchemaInterface *iSchema, MBase::Buffer *iBuffer);
    
    //## Operation: FindRaiseToLevel%358AC83F0085
    //	This finds the targetting level to which this group of
    //	attributes needs to raise.
    void FindRaiseToLevel (DFCAttrGroup &iFilteringKey, DFCAttrGroup &iIgnoringAttrs, VLDBInfo* iVLDBInfo, DFCSchemaInterface *iSchema, DFCAttrGroup &oLevel, MBase::Buffer *iBuffer);
    
    //## Operation: NeedApply%358ADC5F020A
    //	This is to test if the group of attributes needs
    //	applying. if yes, return true.
    bool NeedApply (DFCAttrGroup &iFilteringKey, DFCAttrGroup &iIgnoringAttrs, DFCSchemaInterface *iSchema, MBase::Buffer *iBuffer);
    
    //## Operation: GroupAttrs%35C733530055
    //	This method, partitions the attribute group into
    //	disjoint sets.  All members of a set are related to each
    //	other.
    void GroupAttrs (MBase::Buffer* iBuffer, DFCSchemaInterface *iSchema, vector<DFCAttrGroup*, MBase::Allocator<DFCAttrGroup*> >& oAGVec);
    
    // wyuan; All server-only functions should be guarded by _SMART_CLIENT
#ifndef _SMART_CLIENT
    //TQMS 646552: 2014-02-15: liawang: Divide the attributes into different dimension group
    void DivideAttrs2DiffDims(MBase::Buffer* iBuffer,DFCSchemaInterface  *ipSchema, vector<DFCAttrGroup*, MBase::Allocator<DFCAttrGroup*> >& oDims);
    DFCBitVectorBase* GetAttrsInSameDim(DFCAttr* lAttrs,DFCSchemaInterface  *ipSchema,MBase::Buffer* iBuffer);
    bool isSameDimsion(DFCAttrGroup* lAttrs, DFCSchemaInterface  *ipSchema,MBase::Buffer* iBuffer);
    //TQMS 646552: 2014-02-15: liawang: Remove duplicate attributes(same definition) in this group
    void RemDuplicatedAttrs(DFCAttrGroup& ioAttrs,map<DFCAttr*,DFCAttr*>& ioDuplicatedAttrsMap,MBase::Buffer* iBuffer);
#endif
    //## Operation: Intersect%35C86F1A00B9
    bool Intersect (DFCAttrGroup& iAG, DFCAttrGroup& oAG);
    
    bool GetRelatedSubset(const DFCAttrGroup& iTargetAttrs, DFCAttrGroup& oAG,MBase::Buffer *iBuffer, DFCSchemaInterface *iSchema); //TQMS 921193 dewu, return the subset of attributes in this which is related or identical to the iTargetAttrs
    //## Operation: Size%35CA1B300329
    //	Returns the size of the internal container.
    Int32 Size ();
    
    //## Operation: Clear%364A4DC2032D
    //	Remove all the attributes, if there is any, from the
    //	collection to make it clean.
    void Clear ();
    
    //## Operation: SupportedBy%364A51190246
    //	Check this attr group what is supported (refined) by the
    //	any attr from input attr group and return those are
    //	refined into output attr group.
    void SupportedBy (DFCAttrGroup &iAG, DFCAttrGroup &oAG, MBase::Buffer *iBuffer, DFCSchemaInterface *iSchema);
    
    //## Operation: OJ2LkupRelated%38D020F40186
    bool OJ2LkupRelated (DFCAttr* iAttr, DFCSchemaInterface *ipSchema, MBase::Buffer *iBuffer);
    
    //## Operation: ContainChildOf%396B317D03D1
    bool ContainChildOf (const DFCAttr *ipAttr, DFCSchemaInterface *ipSchema, MBase::Buffer *wpBuffer) const;
    
    //The checking is on Ancestors strictly, return false even this AG contains ipAttr.
    bool ContainAncestorOf(const DFCAttr *ipAttr, DFCSchemaInterface *ipSchema, MBase::Buffer *wpBuffer) const;
    
    //dewu TQMS 901886
    bool ContainMMDescendentsOf(const DFCAttr *opAttr, DFCSchemaInterface* ipschema, MBase::Buffer *iBuffer) const;
    
    //dewu TQMS 901886
    bool ContainMMAncestorOf(const DFCAttr *opAttr, DFCSchemaInterface* ipschema, MBase::Buffer *iBuffer)const;
    
    //## Operation: GetUnionAllChildrenVect%397711780220
    DFCBitVectorBase * GetUnionAllChildrenVect (DFCSchemaInterface *ipSchema, MBase::Buffer *ipBuffer) const;
    
    bool ContainsAllAttributesOf(DFCAttrGroup* iRight) const;
    bool ContainsAllAttributesOf(DFCAttrGroup* iRight, DFCAttr** oMissingAttr) const;
    
    
public:
    // Additional Public Declarations
    //## begin DFCAttrGroup%331B834903AD.public preserve=yes
    void AdjustByFactTableKey(DFCAttrGroup & iKeys, DFCSchemaInterface * ipSchema, MBase::Buffer * iBuffer);
    //## end DFCAttrGroup%331B834903AD.public
    
    //GO begin
    virtual bool GOEqual(DFCAttrGroup* iAttrGroup);
    virtual void GOFUHashAppend2Str(DFCString& oHashStr, MBase::Buffer *iBuffer);
    //GO end
    // liliu 09/26/10, added for Schema Serialization
    void Serialize(ICDSSWriteBlockStream* ipStream, EnumSchemaSerializationLevel iLevel);
    // end liliu
    void GetAsString (DFCString* ioStr) const;
    
    
    // ydong Mar/26/2013, added for smart client
    void SerializeForSmartClient(ICDSSWriteBlockStream* ipWriteBlockStream, map<Int32, Int32, less<Int32>, MBase::Allocator<pair<const Int32, Int32> > >* ipAttrFilter);
    static DFCAttrGroup* UnserializeForSmartClient(ICDSSReadBlockStream* ipReadBlockStream, MBase::Buffer* ipBuffer);
    // end ydong
protected:
    
    //## Other Operations (specified)
    //## Operation: MarkParent%33722BAF001F
    //	Mark the parents whose child is also in the group.
    //	used for reduction.
    void MarkParent (DFCSchemaInterface *ipSchema, vector<Int32> &oMarks, MBase::Buffer* iBuffer);
    
    //## Operation: MarkChildren%33723D4C0023
    //	Make marks on those attribute whose parent(s) is in this
    //	AttributeGroup.
    void MarkChildren (const DFCBitVectorBase &iCD, vector<Int32> &oMarks, MBase::Buffer* iBuffer, DFCSchemaInterface *ipSchema);
    
    //## Operation: ElimMarkedAttr%33723D8D02B1
    //	Elimenate all the marked attributes from this Attribute
    //	Group.
    void ElimMarkedAttr (const vector<Int32>& iMarks);
    
    // Data Members for Class Attributes
    
    //## Attribute: mAttrList%331B835C0007
    //	The set of attributes in the group.
    //## begin DFCAttrGroup::mAttrList%331B835C0007.attr preserve=no  protected: SetDataType {UA}
    SetDataType mAttrList;
    //## end DFCAttrGroup::mAttrList%331B835C0007.attr
    
    // Additional Protected Declarations
    //## begin DFCAttrGroup%331B834903AD.protected preserve=yes
    //## end DFCAttrGroup%331B834903AD.protected
    
private:
    // Data Members for Class Attributes
    
    //## Attribute: mRefCount%33C3A7F300C2
    //## begin DFCAttrGroup::mRefCount%33C3A7F300C2.attr preserve=no  private: int {UA} 1
    Int32 mRefCount;
    //## end DFCAttrGroup::mRefCount%33C3A7F300C2.attr
    
    // Additional Private Declarations
    //## begin DFCAttrGroup%331B834903AD.private preserve=yes
    //## end DFCAttrGroup%331B834903AD.private
    
private: //## implementation
    // Additional Implementation Declarations
    //## begin DFCAttrGroup%331B834903AD.implementation preserve=yes
    //## end DFCAttrGroup%331B834903AD.implementation
    
};

//## begin DFCAttrGroup%331B834903AD.postscript preserve=yes
//## end DFCAttrGroup%331B834903AD.postscript

// Class DFCAttrGroup::Iterator

// Class DFCAttrGroup

//## begin module%331B834903AD.epilog preserve=yes
//## end module%331B834903AD.epilog


#endif