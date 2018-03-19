//## begin module%3978763300A9.cm preserve=no
//	$Revision$
//## end module%3978763300A9.cm

//## begin module%3978763300A9.cp preserve=no
//	CopyRight  (c)  1997  MicroStrategy Inc.
//## end module%3978763300A9.cp

//## Module: DFCSchemaInterface%3978763300A9; Pseudo Package specification
//## Source file: Z:\Engine\SourceCode\Project\DFCSchemaInterface.h

#ifndef DFCSchemaInterface_h
#define DFCSchemaInterface_h 1

//## begin module%3978763300A9.additionalIncludes preserve=no
//## end module%3978763300A9.additionalIncludes

//## begin module%3978763300A9.includes preserve=yes
#include "Includes/DFCSchema_Includes.h"

//## end module%3978763300A9.includes

//## begin module%3978763300A9.additionalDeclarations preserve=yes
#include "DataStructure/DFCBitVector.h"
#include "DataStructure/DFCCompoundBitVector.h"
class DFCAttr;
class DFCCol;
class DFCDatabase;
class DFCDBRole;
class DFCDim;
class DFCFact;
class DFCFactGroup;
class DFCFunc;
class DFCRole;
class DFCTable;
class DFCTableSource;
class DFCProjectSchema;
class SchemaLoadIndex;

#ifdef _SMART_CLIENT
#include <fstream>
#undef BVPerfLog
#else
// AttrLkup
#include "Project/AttrLkup.h"
// ColLkup
#include "Project/ColLkup.h"
// DatabaseLkup
#include "Project/DatabaseLkup.h"
// DBRoleLkup
#include "Project/DBRoleLkup.h"
// DimLkup
#include "Project/DimLkup.h"
// FactLkup
#include "Project/FactLkup.h"
// FactGroupLkup
#include "Project/FactGroupLkup.h"
// FuncLkup
#include "Project/FuncLkup.h"
// TableLkup
#include "Project/TableLkup.h"
// TableSourceLkup
#include "Project/TableSourceLkup.h"
// RoleLkup
#include "Project/RoleLkup.h"
// FormLkup
#include "Project/AttrFormLkup.h"
//## end module%3978763300A9.additionalDeclarations
#include "Log/Log/Initializer.h"
#include "Log/Log/Router.h" 
#include "Log/Defines/Macro.h"
#endif

//## begin DFCSchemaInterface%3978763300A9.preface preserve=yes
class DLL_SCHEMA_EXIM DFCSchemaInterface;
typedef vector<DFCBitVectorBase *, MBase::Allocator<DFCBitVectorBase *> >  VecBitVec;
typedef enum {
	DFCNoRelation = 0,				//two attributes in different hierarchies
	DFCAncestor = 1,				//attr1 is the 1:M ancestor of attr2
	DFCDescendant = 2,				//attr1 is the M:1 descendant of attr2
	DFCM2M = 3,						//two attributes of M:M relation
	DFCSibling = 4,					//two attributes share a same descendant
} EnumDFCAttrRelation;

//## end DFCSchemaInterface%3978763300A9.preface

//## Class: DFCSchemaInterface%3978763300A9
//## Category: Project%3364E12F03E4
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class DFCSchemaInterface 
{
  //## begin DFCSchemaInterface%3978763300A9.initialDeclarations preserve=yes
public:
//#ifndef _SMART_CLIENT
	class SingleCubeWisdomConfig
	{
		public:	
		SingleCubeWisdomConfig(DSS_ID iCubeDSSID, DSS_ID iMemberAttributeID, DSS_ID iAffinityTemplateID, Int32 iTopXMultiplier, Int32 iConstantMultiplier/*,DSS_ID iDropMetricForHavingID*/)
					:mCubeDSSID(iCubeDSSID)
					,mMemberAttributeID(iMemberAttributeID)
					,mAffinityTemplateID(iAffinityTemplateID)
					,mTopXMultiplier(iTopXMultiplier)
					,mConstantMultiplier(iConstantMultiplier)
					//,mDropMetricForHavingID(iDropMetricForHavingID)
					{}

		DSS_ID mCubeDSSID;
		DSS_ID mMemberAttributeID;
		DSS_ID mAffinityTemplateID;
		Int32 mTopXMultiplier;
		Int32 mConstantMultiplier;
		//DSS_ID mDropMetricForHavingID;
	};
	class WisdomConfig
	{
	public:
		WisdomConfig(MBase::Buffer* iBuffer):mBuffer(iBuffer)
		{
			MBase::Allocator<SingleCubeWisdomConfig*> lAlloc(iBuffer);
			mSinleCubeWisdomConfigs = new (iBuffer) vector<SingleCubeWisdomConfig*, MBase::Allocator<SingleCubeWisdomConfig*> >(lAlloc);
		}
		SingleCubeWisdomConfig* GetConfigForCube(DSS_ID lCubeID)
		{
			vector<SingleCubeWisdomConfig*, MBase::Allocator<SingleCubeWisdomConfig*> >::iterator lIter = mSinleCubeWisdomConfigs->begin();
			for(; lIter != mSinleCubeWisdomConfigs->end(); lIter++)
			{
				if((*lIter)->mCubeDSSID == GUID_NULL)
				{
					//GUID_NULL matches everything
					return (*lIter);
				}
				if((*lIter)->mCubeDSSID == lCubeID)
				{
					return (*lIter);
				}
			}
			return NULL;//record for this cube not found
		};
	#ifdef _SMART_CLIENT
		void Init(){};
	#else
		void Init()
		{
			try
			{
			
				ifstream lInFile;
				char lInBuff[100];
				lInFile.open("WisdomEntConf.ini");
				if (lInFile.is_open())
				{
					//find the section for this cube
					//bool lFindCube = false;
					bool lOldFormat = false;
					while (!lInFile.eof())
					{
						lInFile >> lInBuff;
						while (!((lInBuff[0]=='C')&&(lInBuff[1]=='u')&&(lInBuff[2]=='b')&&(lInBuff[3]=='e')) && !lInFile.eof())
						{
							memset(lInBuff, 0, sizeof(lInBuff));
							lInFile >> lInBuff;
						}

						if (lInFile.eof())
						{
							lOldFormat = true;
							break;
						}
						lInFile >> lInBuff;
						MBase::String guid = MBase::UTF8ToWideChar(lInBuff).c_str();
						DSS_ID lCubeID = GUID_NULL;
						MBase::String2Guid(lCubeID, guid.c_str());
						//if (lCubeID == lThisCubeID)
						//	lFindCube = true;
						lInFile >> lInBuff;

						//support for adding comments in the next row of Cube_ID or IncrRpt row in WisdomEntConf.ini
						if((lInBuff[0]=='C' || lInBuff[0] == 'c') && lInBuff[1] == 'o' && lInBuff[2]=='m' && lInBuff[3]=='m' && lInBuff[4]=='e' && lInBuff[5]=='n' && lInBuff[6]=='t' && lInBuff[7]==':')
						{
							do
							{
								memset(lInBuff, 0, sizeof(lInBuff));
								lInFile >> lInBuff;
							}while(lInBuff[strlen(lInBuff, 100)-1] != '$');

							lInFile >> lInBuff;
						}
						lInFile >> lInBuff; //ykiryakov Read Filter Attribute here
						//lInFile.close();
						DFCString lAttrIDString(L"");
						lAttrIDString += MBase::UTF8ToWideChar(lInBuff).c_str();

						DSS_ID lFilterAttrID = GUID_NULL;
						MBase::String2Guid(lFilterAttrID, lAttrIDString.c_str());
						lInFile >> lInBuff; //ykiryakov Read AffinityTemplate ID here
						lInFile >> lInBuff; //ykiryakov Read AffinityTemplate ID here

						DFCString lAffinityTemplateIDString(L"");
						lAffinityTemplateIDString += MBase::UTF8ToWideChar(lInBuff).c_str();

						DSS_ID lAffinityTemplateID = GUID_NULL;
						MBase::String2Guid(lAffinityTemplateID, lAffinityTemplateIDString.c_str());

						DSS_ID lDropMetricForHavingID = GUID_NULL;

						/*lInFile >> lInBuff; //ykiryakov Read DropMetricForHaving ID here
						lInFile >> lInBuff; //ykiryakov Read DropMetricForHaving ID here

						DFCString lDropMetricForHavingIDString(L"");
						lDropMetricForHavingIDString += MBase::UTF8ToWideChar(lInBuff).c_str();

						MBase::String2Guid(lDropMetricForHavingID, lDropMetricForHavingIDString.c_str());
						*/
						
						Int32 iTopXMultiplier = -1;
						Int32 iConstantMultiplier = -1;

						lInFile >> lInBuff;
						lInFile >> iTopXMultiplier;
						lInFile >> lInBuff;
						lInFile >> iConstantMultiplier;

						SingleCubeWisdomConfig* lCubeConfig = new (mBuffer) SingleCubeWisdomConfig(lCubeID,lFilterAttrID,lAffinityTemplateID,iTopXMultiplier,iConstantMultiplier  /*, lDropMetricForHavingID*/);
						mSinleCubeWisdomConfigs->push_back(lCubeConfig);

					}

					if (!lOldFormat)
					{
						/*DFCString lStr(_T("The Cube "));
						lStr += (MBase::Guid2String(lThisCubeID)).c_str();
						lStr += L" is not in WisdomEntConf.ini";
						throw DFCEngineException(ENGINELOGIC_EXCEPTION,	lStr.c_str());*/
						lInFile.close();
						return;
					}

					if (lOldFormat)
					{
						lInFile.clear();
						lInFile.seekg(0, ios::beg); //return to the start of the file

					}
					lInFile >> lInBuff; //ykiryakov Read Filter Attribute here
					lInFile >> lInBuff; //ykiryakov Read Filter Attribute here
					
					DFCString lAttrIDString(L"");
					lAttrIDString += MBase::UTF8ToWideChar(lInBuff).c_str();

					DSS_ID lFilterAttrID = GUID_NULL;
					MBase::String2Guid(lFilterAttrID, lAttrIDString.c_str());

					lInFile >> lInBuff; //ykiryakov Read AffinityTemplate ID here
					lInFile >> lInBuff; //ykiryakov Read AffinityTemplate ID here

					DFCString lAffinityTemplateIDString(L"");
					lAffinityTemplateIDString += MBase::UTF8ToWideChar(lInBuff).c_str();

					DSS_ID lAffinityTemplateID = GUID_NULL;
					MBase::String2Guid(lAffinityTemplateID, lAffinityTemplateIDString.c_str());
					
					DSS_ID lCubeID = GUID_NULL;

					/*DSS_ID lDropMetricForHavingID = GUID_NULL;

					lInFile >> lInBuff; //ykiryakov Read DropMetricForHaving ID here
					lInFile >> lInBuff; //ykiryakov Read DropMetricForHaving ID here

					DFCString lDropMetricForHavingIDString(L"");
					lDropMetricForHavingIDString += MBase::UTF8ToWideChar(lInBuff).c_str();

					MBase::String2Guid(lDropMetricForHavingID, lDropMetricForHavingIDString.c_str());*/
					Int32 iTopXMultiplier = -1;
					Int32 iConstantMultiplier = -1;

					lInFile >> lInBuff;
					lInFile >> iTopXMultiplier;
					lInFile >> lInBuff;
					lInFile >> iConstantMultiplier;

					SingleCubeWisdomConfig* lCubeConfig = new (mBuffer) SingleCubeWisdomConfig(lCubeID,lFilterAttrID,lAffinityTemplateID, iTopXMultiplier, iConstantMultiplier/*, lDropMetricForHavingID*/);
					mSinleCubeWisdomConfigs->push_back(lCubeConfig);


					lInFile.close();
					return;
				} //end (lInFile.is_open())
			}
			catch(...)
			{
				//ignore any IO exceptions
			}
		} //end Init()
		#endif
		MBase::Buffer* mBuffer;
		vector<SingleCubeWisdomConfig*, MBase::Allocator<SingleCubeWisdomConfig*> >* mSinleCubeWisdomConfigs;
	};
//#endif
	// liliu 09/26/10, added for Schema Serialization
	friend class DFCSchemaSeriHelper;

  public:
	  DFCSchemaInterface(MBase::Buffer * ipBuffer);
	  typedef enum{
		  dssProjectLevelSchema = 0,	// project level schema
		  dssReportLevelSchema			// report level schema
	  } SCHEMA_TYPE;

  //## end DFCSchemaInterface%3978763300A9.initialDeclarations

  public:
#ifdef BVPerfLog
static const MLog::RouterPtr mRouterPtr_Engine_BVPerf;
#endif

    //## Destructor (generated)
      ~DFCSchemaInterface();


    //## Other Operations (specified)
      //## Operation: FindAttr%39787655010C
      virtual DFCAttr* FindAttr (Int32 iNbr) = 0;

      //## Operation: FindAttr%397876550120
      virtual DFCAttr* FindAttr (DFCString& iName) = 0;

	  virtual DFCAttr* FindAttrIncludingProject(DSS_ID* iID) { return NULL; }

	  virtual bool ReplaceAttrIfExisted(DFCAttr* ipAttr) = 0;

      //## Operation: FindCol%397876550134
      virtual DFCCol* FindCol (DFCString& iName) = 0;

	  //add
	  virtual DFCCol* FindCol(Int32 iNbr) = 0;

      //## Operation: FindDBRole%397876550148
      virtual DFCDBRole* FindDBRole (Int32 iNbr) = 0;

      //## Operation: FindDBRole%397876550152
      virtual DFCDBRole* FindDBRole (DFCString& iName) = 0;

      //## Operation: FindDatabase%397876550167
      virtual DFCDatabase* FindDatabase (Int32 iNbr) = 0;

      //## Operation: FindDatabase%39787655017B
      virtual DFCDatabase* FindDatabase (DFCString& iName) = 0;

      //## Operation: FindDim%39787655018F
      virtual DFCDim* FindDim (DFCString& iName) = 0;
	  //add
	  virtual DFCDim* FindDim (Int32 iNbr) = 0;

      //## Operation: FindFact%3978765501A3
      virtual DFCFact* FindFact (DFCString& iName) = 0;

      //## Operation: FindFact%3978765501B7
      virtual DFCFact* FindFact (Int32 iNbr) = 0;

	  virtual DFCFact* FindFact (DSS_ID& iID) { return NULL; }

      //## Operation: FindFactGroup%3978765501CB
      virtual DFCFactGroup* FindFactGroup (DFCString& iName) = 0;

      //## Operation: FindFactGroup%3978765501DF
      virtual DFCFactGroup* FindFactGroup (Int32 iNbr) = 0;

      //## Operation: FindFunc%3978765501F3
      virtual DFCFunc* FindFunc (DFCString& iName) = 0;

      //## Operation: FindFunc%397876550207
      //	Find the function in schema by function id number.
      virtual DFCFunc* FindFunc (Int32 iId) = 0;

      //## Operation: FindRole%39787655021B
      virtual DFCRole* FindRole (DFCString& iName) = 0;

      //## Operation: FindRole%39787655022F
      virtual DFCRole* FindRole (Int32 iNbr) = 0;

      //## Operation: FindTable%39787655024D
      virtual DFCTable* FindTable (DFCString& iName) = 0;

	  virtual DFCTable* FindTableByLogicalDSS_ID (DSS_ID& iID) { return NULL; }

      //## Operation: FindTable%397876550261
      virtual DFCTable* FindTable (Int32 iNbr) = 0;

      //## Operation: FindTableSource%397876550275
      virtual DFCTableSource* FindTableSource (DFCString& iName) = 0;

      //## Operation: FindTableSource%397876550289
      virtual DFCTableSource* FindTableSource (Int32 iNbr) = 0;

      //## Operation: GetFuncAND%3978765502CF
      virtual DFCFunc* GetFuncAND () = 0;

      //## Operation: GetFuncBETWEEN%3978765502ED
      virtual DFCFunc* GetFuncBETWEEN () = 0;

      //## Operation: GetFuncCOUNT%397876550301
      virtual DFCFunc* GetFuncCOUNT () = 0;

      //## Operation: GetFuncEQUAL%397876550315
      virtual DFCFunc* GetFuncEQUAL () = 0;

      //## Operation: GetFuncGREATEQUAL%397876550333
      virtual DFCFunc* GetFuncGREATEQUAL () = 0;

      //## Operation: GetFuncGREATER%397876550347
      virtual DFCFunc* GetFuncGREATER () = 0;

      //## Operation: GetFuncGREATEREQUAL%39787655036F
      virtual DFCFunc* GetFuncGREATEREQUAL () = 0;

      //## Operation: GetFuncIN%397876550383
      virtual DFCFunc* GetFuncIN () = 0;

      //## Operation: GetFuncINTERSECT%3978765503A1
      virtual DFCFunc* GetFuncINTERSECT () = 0;

      //## Operation: GetFuncINTERSECTIN%3978765503B5
      virtual DFCFunc* GetFuncINTERSECTIN () = 0;

      //## Operation: GetFuncISNOTNULL%3978765503D3
      virtual DFCFunc* GetFuncISNOTNULL () = 0;

      //## Operation: GetFuncISNULL%397876560009
      virtual DFCFunc* GetFuncISNULL (bool iTest=false) = 0;

      //## Operation: GetFuncLESS%397876560027
      virtual DFCFunc* GetFuncLESS () = 0;

      //## Operation: GetFuncLESSEQUAL%39787656003C
      virtual DFCFunc* GetFuncLESSEQUAL () = 0;

      //## Operation: GetFuncLIKE%39787656005A
      virtual DFCFunc* GetFuncLIKE () = 0;

      //## Operation: GetFuncMAX%397876560078
      virtual DFCFunc* GetFuncMAX () = 0;

      //## Operation: GetFuncMIN%397876560096
      virtual DFCFunc* GetFuncMIN () = 0;

      //## Operation: GetFuncNOT%3978765600AA
      virtual DFCFunc* GetFuncNOT () = 0;

      //## Operation: GetFuncNOTBETWEEN%3978765600C8
      virtual DFCFunc* GetFuncNOTBETWEEN () = 0;

      //## Operation: GetFuncNOTEQUAL%3978765600E6
      virtual DFCFunc* GetFuncNOTEQUAL () = 0;

      //## Operation: GetFuncNOTIN%397876560104
      virtual DFCFunc* GetFuncNOTIN () = 0;

      //## Operation: GetFuncNOTLIKE%397876560122
      virtual DFCFunc* GetFuncNOTLIKE () = 0;

      //## Operation: GetFuncNULLTOZERO%397876560140
      virtual DFCFunc* GetFuncNULLTOZERO () = 0;

      //## Operation: GetFuncOR%39787656015E
      virtual DFCFunc* GetFuncOR () = 0;

      //## Operation: GetFuncRANK%39787656017C
      virtual DFCFunc* GetFuncRANK () = 0;

      //## Operation: GetFuncSUM%3978765601A4
      virtual DFCFunc* GetFuncSUM () = 0;

      //## Operation: GetFuncZEROTONULL%3978765601C2
      virtual DFCFunc* GetFuncZEROTONULL () = 0;

	  virtual DFCFunc* GetFuncAmpersand () { return NULL; }

      //## Operation: PrintAttr%3978765603C1
      virtual void PrintAttr (ostream& iOstream) = 0;

      //## Operation: PrintCol%3978765603DF
      virtual void PrintCol (ostream& iOstream) = 0;

      //## Operation: PrintDBRole%39787657001F
      virtual void PrintDBRole (ostream& iOstream) = 0;

      //## Operation: PrintDatabase%397876570047
      virtual void PrintDatabase (ostream& iOstream) = 0;

      //## Operation: PrintDim%397876570065
      virtual void PrintDim (ostream& iOstream) = 0;

      //## Operation: PrintFact%39787657008D
      virtual void PrintFact (ostream& iOstream) = 0;

      //## Operation: PrintFactGroup%3978765700B5
      virtual void PrintFactGroup (ostream& iOstream) = 0;

      //## Operation: PrintFunc%3978765700DD
      virtual void PrintFunc (ostream& iOstream) = 0;

      //## Operation: PrintRole%397876570105
      virtual void PrintRole (ostream& iOstream) = 0;

      //## Operation: PrintTable%39787657012D
      virtual void PrintTable (ostream& iOstream) = 0;

      //## Operation: PrintTableSource%397876570155
      virtual void PrintTableSource (ostream& iOstream) = 0;

	  virtual void PrintAll (ostream& iOstream) = 0;
    // Additional Public Declarations
      //## begin DFCSchemaInterface%3978763300A9.public preserve=yes

	  //creates an empty BitVector of the appropriate size and type for this schema
	  virtual DFCBitVectorBase * NewEmptyBV(MBase::Buffer* iBuffer) = 0;
	  virtual DFCBitVectorBase * NewEmptyBV(Int32 iLength, MBase::Buffer* iBuffer) = 0;
	  virtual DFCBitVectorBase * NewBV(const DFCBitVectorBase * iInput, MBase::Buffer* iBuffer) = 0;

  	  virtual DFCBitVectorBase * GetDescendentsBVOfAttr (const DFCAttr *ipAttr) = 0;
  	  virtual DFCBitVectorBase * GetAncestorsBVOfAttr (const DFCAttr *ipAttr) = 0;
	  virtual DFCBitVectorBase * GetPartnersBVOfAttr (const DFCAttr *ipAttr) = 0;
	  virtual DFCBitVectorBase * GetParDescendentsBVOfAttr (const DFCAttr *ipAttr) = 0;
  	  virtual DFCBitVectorBase * GetParAncestorsBVOfAttr (const DFCAttr *ipAttr) = 0;
	  virtual DFCBitVectorBase * GetParPartnersBVOfAttr (const DFCAttr *ipAttr) = 0;
	  virtual DFCBitVectorBase * GetMMDescendentsBVOfAttr (const DFCAttr *ipAttr) = 0;
  	  virtual DFCBitVectorBase * GetMMAncestorsBVOfAttr (const DFCAttr *ipAttr) = 0;
	  virtual DFCBitVectorBase * GetMMPartnersBVOfAttr (const DFCAttr *ipAttr) = 0;

	  //hxiao 2012-10-25: begin (help function)
	  virtual DFCIntVec * GetRefChildrenVecOfAttr (const DFCAttr *ipAttr) = 0;
	  virtual DFCIntVec * GetRefParentsVecOfAttr (const DFCAttr *ipAttr) = 0;
	  virtual DFCIntVec * GetRefMMChildrenVecOfAttr (const DFCAttr *ipAttr) = 0;
	  virtual DFCIntVec * GetRefMMParentsVecOfAttr (const DFCAttr *ipAttr) = 0;

	  //print function for debugging
	  virtual void  PrintAllBitVectors(ostream& iOstream);
	  //hxiao 2012-10-25: end (help function)

	  virtual const VecBitVec * GetDescendentsBVs() const { return mpbvDescendentsBVs; };
	  virtual const VecBitVec * GetAncestorsBVs() const { return mpbvAncestorsBVs; };
	  virtual const VecBitVec * GetPartnersBVs() const { return mpbvPartnersBVs; };
  	  virtual const VecBitVec * GetParDescendentsBVs() const { return mpbvParDescendentsBVs; }; 
	  virtual const VecBitVec * GetParAncestorsBVs() const { return mpbvParAncestorsBVs; };
	  virtual const VecBitVec * GetParPartnersBVs() const { return mpbvParPartnersBVs; };
	  virtual const VecBitVec * GetMMDescendentsBVs() const {return mpbvMMDescendentsBVs; };
	  virtual const VecBitVec * GetMMAncestorsBVs() const { return mpbvMMAncestorsBVs; };
	  virtual const VecBitVec * GetMMPartnersBVs() const { return mpbvMMPartnersBVs; };

	  virtual const DFCVecIntVec * GetRefChildrenVec() const {return  mpvRefChildrenVec;};
	  virtual const DFCVecIntVec * GetRefParentsVec() const {return  mpvRefParentsVec;};
	  virtual const DFCVecIntVec * GetRefMMChildrenVec() const {return  mpvRefMMChildrenVec;};
	  virtual const DFCVecIntVec * GetRefMMParentsVec() const {return  mpvRefMMParentsVec;};
	  virtual const DFCVecIntVec * GetRefParChildrenVec() const {return  mpvRefParChildrenVec;};
	  virtual const DFCVecIntVec * GetRefParParentsVec() const {return  mpvRefParParentsVec;};


	  void PrintBVs();
      virtual DFCFunc* GetFuncPLUS () = 0;
      virtual DFCFunc* GetFuncMINUS () = 0;
      virtual DFCFunc* GetFuncTIMES () = 0;
      virtual DFCFunc* GetFuncDIVIDE () = 0;
	  virtual DFCFunc* GetFuncTUPLE () = 0;

	  virtual DFCDBRole* FindDBRole(DSS_ID* iID) = 0;
	  virtual bool HasSecAggFunc () = 0;
	  virtual void SetHasSecAggFunc(bool iHasSecAggFunc) = 0;

      virtual Int32 GetNumAttr () = 0;
      virtual Int32 GetNumCol () = 0;
      virtual Int32 GetNumDatabase () = 0;
      virtual Int32 GetNumDBRole () = 0;
      virtual Int32 GetNumDim () = 0;
      virtual Int32 GetNumFact () = 0;
      virtual Int32 GetNumFunc () = 0;
      virtual Int32 GetNumFactGroup () = 0;
      virtual Int32 GetNumRole () = 0;
      virtual Int32 GetNumTable () = 0;
      virtual Int32 GetNumTableSource () = 0;

	  virtual EnumDSSDatabaseType GetDatabaseType() = 0;

      virtual Int32 InsertAttr (DFCAttr& iAttr, DFCString& iName);
      virtual Int32 InsertCol (DFCCol& iCol, DFCString& iName);
      virtual Int32 InsertDatabase (DFCDatabase& iDatabase, DFCString& iName);
      virtual Int32 InsertDBRole (DFCDBRole& iDBRole, DFCString& iName);
      virtual Int32 InsertDim (DFCDim& iDim, DFCString& iName);
      virtual Int32 InsertFact (DFCFact& iFact, DFCString& iName);
      virtual Int32 InsertFactGroup (DFCFactGroup& iFactGroup, DFCString& iName);
      virtual Int32 InsertFunc (DFCFunc& iFunc, DFCString& iName);
      virtual Int32 InsertRole (DFCRole& iRole, DFCString& iName);
      virtual Int32 InsertTable (DFCTable& iTable, DFCString& iName);
      virtual Int32 InsertTableSource (DFCTableSource& iTableSource, DFCString& iName);

	  virtual void SetDatabaseType(EnumDSSDatabaseType iDbType) {return;};
	  virtual bool IsProjectSchema() {return false;}

	  virtual DFCFunc* GetFuncIF (bool iTest=false) = 0;
	  virtual DFCFunc* GetFuncUNION () = 0;
	  virtual DFCFunc* GetFuncEXCEPT () = 0;

	  virtual void SetAttrBitVectors() = 0;
	  virtual void SetLocalAttrBitVectors() = 0;

	  virtual DFCFunc* GetFuncLAST () = 0;
	  virtual DFCFunc* GetFuncFIRST () = 0;

	  virtual DFCProjectSchema* GetProjectSchema() = 0;

  	  virtual void AddLocale(Int32 iLocale) = 0;
	  virtual bool CheckLocaleInSchema(Int32 iLocale) = 0;
	  virtual bool UseSQLDI() = 0;
	  virtual void SetUseSQLDI(bool input) = 0;
	  virtual bool GetProjectDIEnabled()= 0;
	  virtual void SetProjectDIEnabled(bool input) = 0;

	  // TQMS 380638 - 2009-10-20
	  DFCSchemaInterface::SCHEMA_TYPE GetSchemaType() const;

	  // TQMS 401510 - 2010-06-15
	  MBase::Buffer* GetBuffer() const;
	  // liliu 09/26/10, added for Schema Serialization
	  virtual const set<Int32, less<Int32>, MBase::Allocator<Int32> >* GetLocales() = 0;
	  virtual Int32 InsertAttrForm (DFCForm& iAttrForm, DFCString& iName);
	  // liliu
	  virtual DFCSchemaInterface::SingleCubeWisdomConfig* GetWisdomConfigForCube(DSS_ID iCubeID) { return NULL;}

	  //hxiao 2012-05-17, add for project schema incrementally populating the DBMS
	  virtual void UpdateDatabase () = 0;

	  SchemaLoadIndex* GetSchemaLoadIndex(){ return mpSmIndex;}
	  void SetSchemaLoadIndex(SchemaLoadIndex* ipSmIndex){ mpSmIndex = ipSmIndex; }
	  //hxiao 2013-10-17: populate the local relationship edge
	  static void  AddEdge(Int32 iChildIndex, Int32 iParentIndex, VecBitVec * ioDescendentsBVs, VecBitVec * ioAncestorsBVs, MBase::Buffer* iBuffer);
	  //hxiao 2013-10-17: populate the local relationship immediate edge
	  static void  DeleteEdge(Int32 iChildIndex, Int32 iParentIndex, DFCVecIntVec * iRefChildrenVec, DFCVecIntVec * iRefParentsVec, VecBitVec * ioDescendentsBVs, VecBitVec * ioAncestorsBVs, MBase::Buffer* iBuffer);
	  //hxiao 2014-04-15 TQMS 866983: add parameter io1MRefChildrenVec
	  //@io1MRefChildrenVec: is helper parameter to check whether the MM relationship also have one 1M relationship
	  //before we remove the MM direct relationship, if it has 1M relationship , we will not remove the MM relationship. Since if we remove, it will lose the 1M relationship
	  //MCE side always gets the 1M from MM ref vector, it assumes the MM ref vector includes all 1M relationship
	  static void  AddRefEdge(Int32 iChildIndex, Int32 iParentIndex,DFCVecIntVec * ioRefChildrenVec, DFCVecIntVec * ioRefParentsVec, VecBitVec * iDescendentsBVs, VecBitVec * iAncestorsBVs,  MBase::Buffer* iBuffer, DFCVecIntVec * io1MRefChildrenVec = NULL);
	  static void  DeleteRefEdge(Int32 iChildIndex, Int32 iParentIndex, DFCVecIntVec * ioRefChildrenVec, DFCVecIntVec * ioRefParentsVec,  MBase::Buffer* iBuffer);
	  //hxiao 2013-10-17: remove the redundant ref edge in the MCESchema which may get from the project schema
	  //hxiao 2014-04-15 TQMS 866983: add parameter io1MRefChildrenVec
	  //@io1MRefChildrenVec: is helper parameter to check whether the MM relationship also have one 1M relationship
	  //before we remove the MM direct relationship, if it has 1M relationship , we will not remove the MM relationship. Since if we remove, it will lose the 1M relationship
	  //MCE side always gets the 1M from MM ref vector, it assumes the MM ref vector includes all 1M relationship
	  static void  hRemoveRedundantRefEdge(VecBitVec * iDescendentsBVs, VecBitVec * iAncestorsBVs,DFCVecIntVec * ioRefChildrenVec, DFCVecIntVec * ioRefParentsVec, MBase::Buffer* iBuffer, DFCVecIntVec * io1MRefChildrenVec = NULL);
	  //hxiao 2013-10-17:re-compute the descendants info of B and B's ancestors(union of its direct child)
	  static void  hReComputeBVByRef(Int32 iIndex, DFCVecIntVec *iRefChildrenVec, DFCVecIntVec * iRefParentsVec, VecBitVec * ioDescendentsBVs, MBase::Buffer* iBuffer);
	  //hxiao 2013-10-17: check the indirect path for the circle case when removing the redundant ref edge
	  static bool  hHasIndirectPath(Int32 iChildIndex, Int32 iParentIndex, DFCVecIntVec * iRefChildrenVec, MBase::Buffer* iBuffer);
	  //hxiao 2012-10-20: check whether iParent is the direct parent of Child;
	  static void  IsDirectRel(Int32 iChildIndex, Int32 iParentIndex, DFCVecIntVec * iRefChildrenVec,bool &oIsDirectRel);
	  

	  //hxiao 2014-09-15 extract the share part between DFCSchema and MCESchema for the relationship merge
	  static void hChangeDirect1MToSingleMM(Int32 iChildIndex, Int32 iParentIndex, MBase::Buffer* iBuffer, 
										   DFCVecIntVec * ioRefChildrenVec, DFCVecIntVec * ioRefParentsVec, DFCVecIntVec * ioRefMMChildrenVec, DFCVecIntVec * ioRefMMParentsVec,
										   VecBitVec * ioDescendentsBVs, VecBitVec * ioAncestorsBVs, VecBitVec * ioMMDescendentsBVs, VecBitVec * ioMMAncestorsBVs
										   );
	  static void hChangeDirect1MToCircleMM(Int32 iChildIndex, Int32 iParentIndex, MBase::Buffer* iBuffer, 
											DFCVecIntVec * ioRefChildrenVec, DFCVecIntVec * ioRefParentsVec, DFCVecIntVec * ioRefMMChildrenVec, DFCVecIntVec * ioRefMMParentsVec,
											VecBitVec * ioDescendentsBVs, VecBitVec * ioAncestorsBVs, VecBitVec * ioMMDescendentsBVs, VecBitVec * ioMMAncestorsBVs
											);
	  static void hChangeDirectMMTo1M(Int32 iChildIndex, Int32 iParentIndex, MBase::Buffer* iBuffer, 
											DFCVecIntVec * ioRefChildrenVec, DFCVecIntVec * ioRefParentsVec, DFCVecIntVec * ioRefMMChildrenVec, DFCVecIntVec * ioRefMMParentsVec,
											VecBitVec * ioDescendentsBVs, VecBitVec * ioAncestorsBVs, VecBitVec * ioMMDescendentsBVs, VecBitVec * ioMMAncestorsBVs
											);
	  static void hSetDirect1MRel(Int32 iChildIndex, Int32 iParentIndex, MBase::Buffer* iBuffer, 
											DFCVecIntVec * ioRefChildrenVec, DFCVecIntVec * ioRefParentsVec, DFCVecIntVec * ioRefMMChildrenVec, DFCVecIntVec * ioRefMMParentsVec,
											VecBitVec * ioDescendentsBVs, VecBitVec * ioAncestorsBVs, VecBitVec * ioMMDescendentsBVs, VecBitVec * ioMMAncestorsBVs
											);
	  static void hSetDirectMMRel(Int32 iChildIndex, Int32 iParentIndex, MBase::Buffer* iBuffer, 
											DFCVecIntVec * ioRefChildrenVec, DFCVecIntVec * ioRefParentsVec, DFCVecIntVec * ioRefMMChildrenVec, DFCVecIntVec * ioRefMMParentsVec,
											VecBitVec * ioDescendentsBVs, VecBitVec * ioAncestorsBVs, VecBitVec * ioMMDescendentsBVs, VecBitVec * ioMMAncestorsBVs
											);
	  /*
	  @iPreviousRelationshipType: it indicates the previous relationship type in schema between iChild and iPrarent. We need the previouls type to decide how to merge the added relationship.
	  @ioLocalorProjRelVec: it indicates the previous relationship is project one or local one
	  */
	  static HRESULT HandleMMLocalRel(Int32 iChildIndex, Int32 iParentIndex, MBase::Buffer* iBuffer, Int32 iPreviousRelationType, VecBitVec *ioLocalOrProjRelVec,
											DFCVecIntVec * ioRefChildrenVec, DFCVecIntVec * ioRefParentsVec, DFCVecIntVec * ioRefMMChildrenVec, DFCVecIntVec * ioRefMMParentsVec,
											VecBitVec * ioDescendentsBVs, VecBitVec * ioAncestorsBVs, VecBitVec * ioMMDescendentsBVs, VecBitVec * ioMMAncestorsBVs
											);
	  static HRESULT Handle1MOr11LocalRel(Int32 iChildIndex, Int32 iParentIndex, MBase::Buffer* iBuffer, Int32 iPreviousRelationType, VecBitVec *ioLocalOrProjRelVec,
											DFCVecIntVec * ioRefChildrenVec, DFCVecIntVec * ioRefParentsVec, DFCVecIntVec * ioRefMMChildrenVec, DFCVecIntVec * ioRefMMParentsVec,
											VecBitVec * ioDescendentsBVs, VecBitVec * ioAncestorsBVs, VecBitVec * ioMMDescendentsBVs, VecBitVec * ioMMAncestorsBVs
											);



	  EnumDFCAttrRelation DetermineRelationship(Int32 iSrcIndex, Int32 iDestIndex,bool isIgnoreSibling = false);

	  // look through mTables and check whether there eixts one is DFCHadoopFileTable
	  // overrides in DFCSchema
	  virtual bool ContainHadoopFileTable(bool iReportLevelOnly=false);

	  virtual bool ContainSchema(DFCSchemaInterface* ipSchema);
      //## Operation: FindFact%331F81FC0340
      //## end DFCSchemaInterface%3978763300A9.public
  protected:
#ifndef _SMART_CLIENT
    // Additional Protected Declarations
      //## begin DFCSchemaInterface%3978763300A9.protected preserve=yes
  	  void ProcessJointChildrenGroup(Int32 iAttrID, DFCVecIntVec * iJointChildrenGroup, VecBitVec * ioDescendentsBVs, VecBitVec * ioAncestorsBVs, VecBitVec * ioPartnersBVs);
	  void AddEdge(Int32 iChildID, Int32 iParentID, VecBitVec * ioDescendentsBVs, VecBitVec * ioAncestorsBVs, VecBitVec * ioPartnerBVs);
#endif
	  MBase::Buffer * mpBuffer;

	  VecBitVec * mpbvDescendentsBVs;
	  VecBitVec * mpbvAncestorsBVs;
	  VecBitVec * mpbvPartnersBVs;
	  VecBitVec * mpbvParDescendentsBVs;
	  VecBitVec * mpbvParAncestorsBVs;
	  VecBitVec * mpbvParPartnersBVs;
	  VecBitVec * mpbvMMDescendentsBVs;
	  VecBitVec * mpbvMMAncestorsBVs;
	  VecBitVec * mpbvMMPartnersBVs;

	  // to record immediate relationships
	  DFCVecIntVec * mpvRefChildrenVec;
	  DFCVecIntVec * mpvRefParentsVec;
	  DFCVecIntVec * mpvRefParChildrenVec;
	  DFCVecIntVec * mpvRefParParentsVec;
	  DFCVecIntVec * mpvRefMMChildrenVec;
	  DFCVecIntVec * mpvRefMMParentsVec;

	  DFCVecVecIntVec * mpvJointChildrenGroupVec; 
#ifndef _SMART_CLIENT
      AttrLkup mAttrs;
      ColLkup mCols;
      DatabaseLkup mDatabases;
      DBRoleLkup mDBRoles;
      DimLkup mDims;
      FactLkup mFacts;
      FactGroupLkup mFactGroups;
      FuncLkup mFuncs;
      RoleLkup mRoles;
      TableLkup mTables;
      TableSourceLkup mTableSources;
	  // liliu 09/26/10, added for Schema Serialization
	  AttrFormLkup mForms;

	  SCHEMA_TYPE mSchemaType;	  
#endif

	  SchemaLoadIndex* mpSmIndex;
      //## end DFCSchemaInterface%3978763300A9.protected
  private:
    // Additional Private Declarations
      //## begin DFCSchemaInterface%3978763300A9.private preserve=yes
      //## end DFCSchemaInterface%3978763300A9.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin DFCSchemaInterface%3978763300A9.implementation preserve=yes
      //## end DFCSchemaInterface%3978763300A9.implementation

};

//## begin DFCSchemaInterface%3978763300A9.postscript preserve=yes
// get vec of bit vectors
/*
inline const VecBitVec * DFCSchemaInterface::GetDescendentsBVs() const
{
	return mpbvDescendentsBVs;
}

inline const VecBitVec * DFCSchemaInterface::GetAncestorsBVs() const
{
	return mpbvAncestorsBVs;
}

inline const VecBitVec * DFCSchemaInterface::GetPartnersBVs() const
{
	return mpbvPartnersBVs;
}

inline const VecBitVec * DFCSchemaInterface::GetParDescendentsBVs() const
{
	return mpbvParDescendentsBVs;
}

inline const VecBitVec * DFCSchemaInterface::GetParAncestorsBVs() const
{
	return mpbvParAncestorsBVs;
}

inline const VecBitVec * DFCSchemaInterface::GetParPartnersBVs() const
{
	return mpbvParPartnersBVs;
}

inline const VecBitVec * DFCSchemaInterface::GetMMDescendentsBVs() const
{
	return mpbvMMDescendentsBVs;
}

inline const VecBitVec * DFCSchemaInterface::GetMMAncestorsBVs() const
{
	return mpbvMMAncestorsBVs;
}

inline const VecBitVec * DFCSchemaInterface::GetMMPartnersBVs() const
{
	return mpbvMMPartnersBVs;
}
*/

//## end DFCSchemaInterface%3978763300A9.postscript

// Class DFCSchemaInterface 

//## begin module%3978763300A9.epilog preserve=yes
//## end module%3978763300A9.epilog


#endif
