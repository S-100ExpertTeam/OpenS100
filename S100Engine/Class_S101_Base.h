#pragma once
#include <map>
#include <vector>
#include <d2d1.h>

typedef std::vector <unsigned int>	VEC_B1;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Dataset Descriptive Records
struct ST_DSID
{
	unsigned int				nRCNM;		// Record name
	unsigned int				nRCID;		// Record identification number
	CString					strENSP;		// Encoding specification
	CString					strENED;		// Encoding specification edition
	CString					strPRSP;		// Product identifier
	CString					strPRED;		// Product edition
	CString					strPROF;		// Application profile
	CString					strDSNM;		// Dataset file identifier
	CString					strDSTL;		// Dataset title
	CString					strDSRD;		// Dataset reference date
	CString					strDSLG;		// Dataset language
	CString					strDSAB;		// Dataset abstract
	CString					strDSED;		// Dataset edition
	VEC_B1					vecDSTC;		// Dataset topic category

	ST_DSID()
	{
		nRCNM = 0;			// Record name
		nRCID = 0;			// Record identification number
		strENSP.Empty();		// Encoding specification
		strENED.Empty();		// Encoding specification edition
		strPRSP.Empty();		// Product identifier
		strPRED.Empty();		// Product edition
		strPROF.Empty();		// Application profile
		strDSNM.Empty();	// Dataset file identifier
		strDSTL.Empty();		// Dataset title
		strDSRD.Empty();		// Dataset reference date
		strDSLG.Empty();		// Dataset language
		strDSAB.Empty();		// Dataset abstract
		strDSED.Empty();		// Dataset edition
		vecDSTC.clear();
	}
};

struct ST_DSSI
{
	double					fDCOX;			// Dataset Coordinate Origin X
	double					fDCOY;			// Dataset Coordinate Origin Y
	double					fDCOZ;			// Dataset Coordinate Origin Z
	unsigned int				nCMFX;			// Coordinate multiplication factor for x-coordinate
	unsigned int				nCMFY;			// Coordinate multiplication factor for x-coordinate
	unsigned int				nCMFZ;			// Coordinate multiplication factor for x-coordinate
	unsigned int				nNOIR;			// Number of information Type records
	unsigned int				nNOPN;			// Number of Point records
	unsigned int				nNOMN;			// Number of Multi Point records
	unsigned int				nNOCN;			// Number of Curve records
	unsigned int				nNOXN;			// Number of Composite Curve records
	unsigned int				nNOSN;			// Number of Surface records
	unsigned int				nNOFR;			// Number of Feature Type records

	ST_DSSI()
	{
		fDCOX = 0.0;			// Dataset Coordinate Origin X
		fDCOY = 0.0;			// Dataset Coordinate Origin Y
		fDCOZ = 0.0;			// Dataset Coordinate Origin Z
		nCMFX = 0;			// Coordinate multiplication factor for x-coordinate
		nCMFY = 0;			// Coordinate multiplication factor for x-coordinate
		nCMFZ = 0;			// Coordinate multiplication factor for x-coordinate
		nNOIR = 0;			// Number of information Type records
		nNOPN = 0;			// Number of Point records
		nNOMN = 0;			// Number of Multi Point records
		nNOCN = 0;			// Number of Curve records
		nNOXN = 0;			// Number of Composite Curve records
		nNOSN = 0;			// Number of Surface records
		nNOFR = 0;			// Number of Feature Type records
	}
};

struct ST_SUBFIELD
{
	CString					strCode;			
	unsigned int				nNumeric;		

	ST_SUBFIELD()
	{
		strCode.Empty();
		nNumeric = 0;
	}
};
typedef std::vector <ST_SUBFIELD>		VEC_SUBFIELD;

struct ST_ATTR_NATC
{
	unsigned int					nNATC;			// Numeric attibute code
	unsigned int					nATIX;				// Attribute index
	unsigned int					nPAIX;				// Parent index
	unsigned int					nATIN;			// Attribute Instruction
	CString							strATVL;			// Attribute value

	ST_ATTR_NATC()
	{
		nNATC = 0;
		nATIX = 0;
		nPAIX = 0;
		nATIN = 0;
		strATVL.Empty();
	}
};
typedef std::vector <ST_ATTR_NATC>		VEC_ATTR_NATC;

struct ST_ATTR
{
	VEC_ATTR_NATC			vecNATC;

	ST_ATTR()
	{
		vecNATC.clear();
	}
};
typedef std::vector <ST_ATTR>		VEC_ATTR;

struct ST_DSGI
{
	ST_DSID			stDSID;
	ST_DSSI			stDSSI;
	VEC_SUBFIELD	vecATCS;
	VEC_SUBFIELD	vecITCS;
	VEC_SUBFIELD	vecFTCS;
	VEC_SUBFIELD	vecIACS;
	VEC_SUBFIELD	vecFACS;
	VEC_SUBFIELD	vecARCS;
	VEC_ATTR		vecATTR;

	ST_DSGI()
	{
		vecATCS.clear();
		vecITCS.clear();
		vecFTCS.clear();
		vecIACS.clear();
		vecFACS.clear();
		vecARCS.clear();
		vecATTR.clear();
	}
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Dataset Coordinate Reference System Record
struct ST_CSID
{
	unsigned int					nRCNM;			// Record name
	unsigned int					nRCID;			// Record identification number
	unsigned int					nNCRC;			// Number of CRS Components

	ST_CSID()
	{
		nRCNM = 0;
		nRCID = 0;
		nNCRC = 0;
	}
};

struct  ST_CRSH
{
	unsigned int					nCRIX;				// CRS Index
	unsigned int					nCRST;			// CRS Type
	unsigned int					nCSTY;			// Coordinate System Type
	CString						strCRNM;			// CRS Name
	CString						strCRSI;			// CRS Identifier
	unsigned int					nCRSS;			// CRS Source
	CString						strSCRI;			// CRS Source Information

	ST_CRSH()
	{
		nCRIX = 0;
		nCRST = 0;
		nCSTY = 0;
		strCRNM.Empty();
		strCRSI.Empty();
		nCRSS = 0;
		strSCRI.Empty();
	}
};
typedef std::vector <ST_CRSH>		VEC_CRSH;

struct ST_CSAX
{
	unsigned int					nAXTY;			// Axis Type
	unsigned int					nAXUM;			// Axis Unit of Measure

	ST_CSAX()
	{
		nAXTY = 0;
		nAXUM = 0;
	}
};
typedef std::vector <ST_CSAX>		VEC_CSAX;

struct ST_PROJ
{
	unsigned int					nPROM;			// Projection Method
	double						fPRP1;				// Projection Parameter1
	double						fPRP2;				// Projection Parameter2
	double						fPRP3;				// Projection Parameter3
	double						fPRP4;				// Projection Parameter4
	double						fPRP5;				// Projection Parameter5
	double						fFEAS;				// False Easting
	double						fFNOR;			// False Northing

	ST_PROJ()
	{
		nPROM = 0;
		fPRP1 = 0.0;
		fPRP2 = 0.0;
		fPRP3 = 0.0;
		fPRP4 = 0.0;
		fPRP5 = 0.0;
		fFEAS = 0.0;
		fFNOR = 0.0;
	}
};

struct ST_GDAT
{
	CString						strDTNM;
	CString						strELNM;
	double						fESMA;
	unsigned int					nESPT;
	double						fESPM;
	CString						strCMNM;
	double						fCMGL;

	ST_GDAT()
	{
		strDTNM.Empty();
		strELNM.Empty();
		fESMA = 0.0;
		nESPT = 0;
		fESPM = 0.0;
		strCMNM.Empty();
		fCMGL = 0.0;
	}
};

struct ST_VDAT
{
	CString						strDTNM;			// Datum Name
	CString						strDTID;			// Datum Identifier
	unsigned int					nDTSR;			// Datum Source
	CString						strSCRI;			// Datum Source Information

	ST_VDAT()
	{
		strDTNM.Empty();
		strDTID.Empty();
		nDTSR = 0;
		strSCRI.Empty();
	}
};

struct ST_DCRS
{
	ST_CSID			stCSID;
	VEC_CRSH		vecCRSH;
	ST_CSAX			stCSAX;
	ST_PROJ			stPROJ;
	ST_GDAT			stGDAT;
	ST_VDAT			stVDAT;

	ST_DCRS()
	{
		vecCRSH.clear();
	}
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Information Type Record
struct ST_IRID
{
	unsigned int					nRCNM;			// Record Name
	unsigned int					nRCID;			// Record identification number
	unsigned int					nNITC;			// Numeric Information Type Code
	unsigned int					nRVER;			// Record version
	unsigned int					nRUIN;			// Record update instruction

	ST_IRID()
	{
		nRCNM = 0;
		nRCID = 0;
		nNITC = 0;
		nRVER = 0;
		nRUIN = 0;
	}
};

struct ST_INAS_NATC
{
	unsigned int					nNATC;			// Numeric Attribute Code
	unsigned int					nATIX;				// Attribute index
	unsigned int					nPAIX;				// Parent index
	unsigned int					nATIN;			// Attribute Instruction
	CString						strATVL;			// Attribute Value

	ST_INAS_NATC()
	{
		nNATC = 0;
		nATIX = 0;
		nPAIX = 0;
		nATIN = 0;
		strATVL.Empty();
	}
};
typedef std::vector <ST_INAS_NATC>		VEC_INAS_NATC;

struct ST_INAS
{
	unsigned int					nRRNM;			// Referenced Record Name
	unsigned int					nRRID;			// Referenced Record identifier
	unsigned int					nNIAC;			// Numeric Information Association Code
	unsigned int					nNARC;			// Numeric Association Roll Code
	unsigned int					nIUIN;				// Numeric Association Update Instruction
	VEC_INAS_NATC				vecNATC;

	ST_INAS()
	{
		nRRNM = 0;
		nRRID = 0;
		nNIAC = 0;
		nNARC = 0;
		nIUIN = 0;
		vecNATC.clear();
	}
};
typedef std::vector <ST_INAS>		VEC_INAS;

struct ST_INFO_RECORD
{
	ST_IRID			stIRID;
	VEC_ATTR		vecATTR;
	VEC_INAS			vecINAS;
	
	ST_INFO_RECORD()
	{
		vecATTR.clear();
		vecINAS.clear();
	}
};
typedef std::vector <ST_INFO_RECORD>		VEC_INFO_RECORD;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Point Record
struct ST_ID
{
	unsigned int					nRCNM;			// Record Name
	unsigned int					nRCID;			// Record identification number // ID!!!
	unsigned int					nRVER;			// Record version
	unsigned int					nRUIN;			// Record update instruction

	ST_ID()
	{
		nRCNM = 0;
		nRCID = 0;
		nRVER = 0;
		nRUIN = 0;
	}
};

struct ST_C2IT
{
	int							nYCOO;
	int							nXCOO;

	ST_C2IT()
	{
		nYCOO = 0;
		nXCOO = 0;
	}
};
typedef std::vector <ST_C2IT>		VEC_C2IT;

struct ST_C3IT
{
	int							nVCID;
	int							nYCOO;
	int							nXCOO;
	int							nZCOO;

	ST_C3IT()
	{
		nVCID = 0;
		nYCOO = 0;
		nXCOO = 0;
		nZCOO = 0;
	}
};
typedef std::vector <ST_C3IT>		VEC_C3IT;

struct ST_C2FT
{
	double					fYCOO;
	double					fXCOO;

	ST_C2FT()
	{
		fYCOO = 0.0;
		fXCOO = 0.0;
	}
};
typedef std::vector <ST_C2FT>		VEC_C2FT;

struct ST_C3FT
{
	int							nVCID;
	double					fYCOO;
	double					fXCOO;
	double					fZCOO;

	ST_C3FT()
	{
		nVCID = 0;
		fYCOO = 0.0;
		fXCOO = 0.0;
		fZCOO = 0.0;
	}
};
typedef std::vector <ST_C3IT>		VEC_C3IT;

struct ST_POINT_RECORD
{
	ST_ID						stPRID;
	ST_C2IT					stC2IT;
	ST_C3IT					stC3IT;
	ST_C2FT					stC2FT;
	ST_C3FT					stC3FT;
	VEC_INAS					vecINAS;

	ST_POINT_RECORD()
	{
		vecINAS.clear();
	}
};
typedef std::vector <ST_POINT_RECORD>		VEC_POINT_RECORD;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Multi Point Record
struct ST_C2IL
{
	int							nYCOO; // <x> / 10000000
	int							nXCOO; // <y> / 10000000

	ST_C2IL()
	{
		nYCOO = 0;
		nXCOO = 0;
	}
};
typedef std::vector <ST_C2IL>		VEC_C2IL;

struct ST_C3IL_XYZ
{
	int							nYCOO;
	int							nXCOO;
	int							nZCOO;

	ST_C3IL_XYZ()
	{
		nYCOO = 0;
		nXCOO = 0;
		nZCOO = 0;
	}
};
typedef std::vector <ST_C3IL_XYZ>		VEC_C3IL_XYZ;

struct ST_C3IL
{
	int							nVCID;
	VEC_C3IL_XYZ			vecXYZ;

	ST_C3IL()
	{
		nVCID = 0;
		vecXYZ.clear();
	}
};
typedef std::vector <ST_C3IL>		VEC_C3IL;

struct ST_C2FL
{
	double					fYCOO;
	double					fXCOO;

	ST_C2FL()
	{
		fYCOO = 0.0;
		fXCOO = 0.0;
	}
};
typedef std::vector <ST_C2FL>		VEC_C2FL;

struct ST_C3FL_XYZ
{
	double					fYCOO;
	double					fXCOO;
	double					fZCOO;

	ST_C3FL_XYZ()
	{
		fYCOO = 0.0;
		fXCOO = 0.0;
		fZCOO = 0.0;
	}
};
typedef std::vector <ST_C3FL_XYZ>		VEC_C3FL_XYZ;

struct ST_C3FL
{
	int							nVCID;
	VEC_C3FL_XYZ			vecXYZ;

	ST_C3FL()
	{
		nVCID = 0;
		vecXYZ.clear();
	}
};
typedef std::vector <ST_C3FL>		VEC_C3FL;

struct ST_COCC
{
	unsigned int				nCOUI;
	unsigned int				nCOIX;
	unsigned int				nNCOR;

	ST_COCC()
	{
		nCOUI = 0;
		nCOIX = 0;
		nNCOR = 0;
	}
};

struct ST_MULTI_POINT_RECORD
{
	ST_ID						stMRID;
	ST_COCC					stCOCC;
	VEC_C2IL					vecC2IL;
	VEC_C3IL					vecC3IL;
	VEC_C2FL					vecC2FL;
	VEC_C3FL					vecC3FL;
	VEC_INAS					vecINAS;

	ST_MULTI_POINT_RECORD()
	{
		vecC2IL.clear();
		vecC3IL.clear();
		vecC2FL.clear();
		vecC3FL.clear();
		vecINAS.clear();
	}
};
typedef std::vector <ST_MULTI_POINT_RECORD>		VEC_MULTI_POINT_RECORD;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Curve Record
struct ST_PTAS   // <Boundary>
{
	unsigned int				nRRNM;				// Reference Record Name
	unsigned int				nRRID;				// Reference Record identifier  // <Boundary ref="RRID">
	unsigned int				nTOPI;					// Topology indicator ( 1 = Beginning Point, 2 = End Point, 3 = Beginning & End Point ) // <Boundary boundaryType="TOPI"> 

	ST_PTAS()
	{
		nRRNM = 0;
		nRRID = 0;
		nTOPI = 0;
	}
};
typedef std::vector <ST_PTAS>		VEC_PTAS;

struct ST_SECC
{
	unsigned int				nSEUI;					// Segment update instruction ( 1 = Insert, 2 = Delete, 3 = Modify )		
	unsigned int				nSEIX;					// Segment Index
	unsigned int				nNSEG;				// Number of segments

	ST_SECC()
	{
		nSEUI = 0;
		nSEIX = 0;
		nNSEG = 0;
	}
};

struct ST_SEGH
{
	unsigned int				nINTP;					// Interpolation 
														// 1 = Linear, 2 = Arc3Points, 3 = Geodesic, 4 = Loxodromic, 5 = Elliptical, 6 = Conic, 7 = Circular Arc Center Point with Radius

	ST_SEGH()
	{
		nINTP = 0;
	}
};
typedef std::vector<ST_SEGH>			VEC_SEGH;

// <Curve>
struct ST_CURVE_RECORD
{
	ST_ID						stCRID;  // id
	VEC_PTAS					vecPTAS; // <Boundary> list
	ST_SECC					stSECC;
	VEC_SEGH				vecSEGH;     // <Segment interpolation="Loxodromic">
	ST_COCC					stCOCC;
	VEC_C2IL					vecC2IL; // <ControlPoint>
	VEC_C3IL					vecC3IL;
	VEC_C2FL					vecC2FL;
	VEC_C3FL					vecC3FL;
	VEC_INAS					vecINAS;

	ST_CURVE_RECORD()
	{
		vecPTAS.clear();
		vecSEGH.clear();
		vecC2IL.clear();
		vecC3IL.clear();
		vecC2FL.clear();
		vecC3FL.clear();
		vecINAS.clear();
	}
};
typedef std::vector<ST_CURVE_RECORD>			VEC_CURVE_RECORD;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Composite Curve Record
struct ST_CCOC
{
	unsigned int				nCCUI;					// Curve Compoinent update instruction ( 1 = Insert, 2 = Delete, 3 = Modify )
	unsigned int				nCCIX;						// Curve Component index
	unsigned int				nNCCO;					// Number of Curve Compnents

	ST_CCOC()
	{
		nCCUI = 0;
		nCCIX = 0;
		nNCCO = 0;
	}
};

struct ST_CUCO_RRNM
{
	unsigned int				nRRNM;					// Referenced Record name
	unsigned int				nRRID;					// Referenced Record identifier
	unsigned int				nORNT;					// Orientation ( 1 = Forward, 2 = Reverse)

	ST_CUCO_RRNM()
	{
		nRRNM = 0;
		nRRID = 0;
		nORNT = 1;
	}
};
typedef std::vector<ST_CUCO_RRNM>			VEC_CUCO_RRNM;

struct ST_CUCO
{
	VEC_CUCO_RRNM		vecRRNM;

	ST_CUCO()
	{
		vecRRNM.clear();
	}
};
typedef std::vector<ST_CUCO>					VEC_CUCO;

struct ST_COMP_CURVE_RECORD
{
	ST_ID						stCCID;
	ST_CCOC					stCCOC;
	VEC_CUCO				vecCUCO;
	VEC_INAS					vecINAS;

	ST_COMP_CURVE_RECORD()
	{
		vecCUCO.clear();
		vecINAS.clear();
	}
};
typedef std::vector<ST_COMP_CURVE_RECORD>		VEC_COMP_CURVE_RECORD;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Surface Record
struct ST_RIAS_RRNM
{
	unsigned int				nRRNM;					// Referenced Record name
	unsigned int				nRRID;					// Referenced Record identifier
	unsigned int				nORNT;					// Orientation (1 = Forward, 2 = Reverse)
	unsigned int				nUSAG;					// Usage Indicator (1 = Exterior, 2 = Interior)
	unsigned int				nRAUI;					// Ring Association update instrluction (1 = Insert, 2 = Delete)

	ST_RIAS_RRNM()
	{
		nRRNM = 0;
		nRRID = 0;
		nORNT = 1;
		nUSAG = 1;
		nRAUI = 1;
	}
};
typedef std::vector<ST_RIAS_RRNM>		VEC_RIAS_RRNM;

struct ST_RIAS
{
	VEC_RIAS_RRNM			vecRRNM;

	ST_RIAS()
	{
		vecRRNM.clear();
	}
};
typedef std::vector<ST_RIAS>		VEC_RIAS;

struct ST_SURFACE_RECORD
{
	ST_ID					stSRID;
	VEC_RIAS				vecRIAS;
	VEC_INAS				vecINAS;

	ST_SURFACE_RECORD()
	{
		vecRIAS.clear();
		vecINAS.clear();
	}
};
typedef std::vector<ST_SURFACE_RECORD>		VEC_SURFACE_RECORD;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Feature Type record
struct ST_FRID
{
	unsigned int				nRCNM;			// Record name
	unsigned int				nRCID;			// Record identification number
	unsigned int				nNFTC;			// Numeric Feature Type Code
	unsigned int				nRVER;			// Record version
	unsigned int				nRUIN;			// Record update instruction (1 = Insert, 2 = Delete, 3 = Modify)

	ST_FRID()
	{
		nRCNM = 0;
		nRCID = 0;
		nNFTC = 0;
		nRVER = 0;
		nRUIN = 0;
	}
};

struct ST_FOID
{
	unsigned int				nAGEN;			// Producing agency
	unsigned int				nFIDN;			// Feature identification number
	unsigned int				nFIDS;				// Feature identification subdivision

	ST_FOID()
	{
		nAGEN = 0;
		nFIDN = 0;
		nFIDS = 0;
	}
};

struct ST_SPAS_RRNM
{
	unsigned int				nRRNM;
	unsigned int				nRRID;
	unsigned int				nORNT;
	unsigned int				nSMIN;			// Scale Minimum
	unsigned int				nSMAX;			// Scale Maximum
	unsigned int				nSAUI;			// Spatial Association Update instruction (1 = Insert, 2 = Delete)

	ST_SPAS_RRNM()
	{
		nRRNM = 0;
		nRRID = 0;
		nORNT = 0;
		nSMIN = 0;
		nSMAX = 0;
		nSAUI = 0;
	}
};
typedef std::vector<ST_SPAS_RRNM>		VEC_SPAS_RRNM;

struct ST_SPAS
{
	VEC_SPAS_RRNM		vecRRNM;
	ST_SPAS()
	{
		vecRRNM.clear();
	}
};
typedef std::vector<ST_SPAS>		VEC_SPAS;

struct ST_FASC_NATC
{
	unsigned int				nNATC;
	unsigned int				nATIX;
	unsigned int				nPAIX;
	unsigned int				nATIN;
	CString					strATVL;

	ST_FASC_NATC()
	{
		nNATC = 0;
		nATIX = 0;
		nPAIX = 0;
		nATIN = 0;
		strATVL.Empty();
	}
};
typedef std::vector<ST_FASC_NATC>		VEC_FASC_NATC;

struct ST_FASC
{
	unsigned int				nRRNM;
	unsigned int				nRRID;
	unsigned int				nNFAC;			// Numeric Feature Association Code
	unsigned int				nNARC;			// Numeric Association Role Code
	unsigned int				nFAUI;				// Feature Association Update instruction (1 = Insert, 2 = Delete, 3 = Modify)
	VEC_FASC_NATC			vecNATC;

	ST_FASC()
	{
		nRRNM = 0;
		nRRID = 0;
		nNFAC = 0;
		nNARC = 0;
		nFAUI = 0;
		vecNATC.clear();
	}
};
typedef std::vector<ST_FASC>		VEC_FASC;

struct ST_THAS_RRNM
{
	unsigned int				nRRNM;
	unsigned int				nRRID;
	unsigned int				nTAUI;			// Theme Association Updater instruction(1 = Insert, 2 = Delete)

	ST_THAS_RRNM()
	{
		nRRNM = 0;
		nRRID = 0;
		nTAUI = 0;
	}
};
typedef std::vector<ST_THAS_RRNM>		VEC_THAS_RRNM;

struct ST_THAS
{
	VEC_THAS_RRNM		vecRRNM;

	ST_THAS()
	{
		vecRRNM.clear();
	}
};
typedef std::vector<ST_THAS>		VEC_THAS;

struct ST_MASK_RRNM
{
	unsigned int				nRRNM;
	unsigned int				nRRID;
	unsigned int				nMIND;					// Mask Indicator(1 = Truncated by the dataset limit, 2 = Suppress portrayal);
	unsigned int				nMUIN;					// Mask Update instruction (1 = Insert, 2 = Delete)

	ST_MASK_RRNM()
	{
		nRRNM = 0;
		nRRID = 0;
		nMIND = 0;
		nMUIN = 0;
	}
};
typedef std::vector<ST_MASK_RRNM>		VEC_MASK_RRNM;

struct ST_MASK
{
	VEC_MASK_RRNM		vecRRNM;

	ST_MASK()
	{
		vecRRNM.clear();
	}
};
typedef std::vector<ST_MASK>		VEC_MASK;

struct ST_FEATURE_RECORD
{
	ST_FRID				stFRID;
	ST_FOID				stFOID;
	VEC_ATTR			vecATTR;
	VEC_INAS				vecINAS;
	VEC_SPAS				vecSPAS;
	VEC_FASC				vecFASC;
	VEC_THAS			vecTHAS;
	VEC_MASK			vecMASK;

	ST_FEATURE_RECORD()
	{
		vecATTR.clear();
		vecINAS.clear();
		vecSPAS.clear();
		vecFASC.clear();
		vecTHAS.clear();
		vecMASK.clear();
	}
};
typedef std::vector<ST_FEATURE_RECORD>		VEC_FEATURE_RECORD;

class CClass_S101_Base
{
public:
	CClass_S101_Base();
	virtual ~CClass_S101_Base();

public:
	ST_DSGI						m_stDSGI;              // Dataset General Information Record
	ST_DCRS						m_stDCRS;              // Dataset Coordinate Reference System Record
	VEC_INFO_RECORD				m_vecInfoRecord;       // Information Record
	VEC_POINT_RECORD			m_vecPointRecord;      // Point Record
	VEC_MULTI_POINT_RECORD	    m_vecMultiPointRecord; // Multi Point Record
	VEC_CURVE_RECORD			m_vecCurveRecord;      // Curve Record
	VEC_COMP_CURVE_RECORD	    m_vecCompCurveRecord;  // Composite Curve Record
	VEC_SURFACE_RECORD			m_vecSurfaceRecord;    // Surface Record
	VEC_FEATURE_RECORD			m_vecFeatureRecord;    // Feature Type Record

	std::map<int, ST_FEATURE_RECORD*> m_mapFeatureRecord;
	std::map<int, ST_INFO_RECORD*> m_mapInformationRecord;
	std::map<int, ST_POINT_RECORD*> m_mapPointRecord;
	std::map<int, ST_MULTI_POINT_RECORD*> m_mapMultiPointRecord;
	std::map<int, ST_CURVE_RECORD*> m_mapCurveRecord;
	std::map<int, ST_COMP_CURVE_RECORD*> m_mapCompositeCurveRecord;
	std::map<int, ST_SURFACE_RECORD*> m_mapSurfaceRecord;

	std::map<int, CString> m_mapFeatureName;
	std::map<int, CString> m_mapInformationName;
	std::map<int, CString> m_mapAttributeName;

	// Point or Text Instruction
	// -> Point, Curve, Comp Curve, Surface 

	// Line Instruction
	// -> Curve, Comp Curve, Surface

	// Area Instruction
	// -> Surface


	// function
public:
	// function that reads S-101 data.
	bool				LoadBaseCell(CString strFilePath);
	// function that parses the read ENC file.
	bool				ParsingBaseCell(const BYTE *pData);
	// ISO 8211 Verification Function
	bool				Parsing_8211(const BYTE *pData, int &nRecordLength);
	// Dataset General Information Record
	bool				Parsing_DSGI(const BYTE *pData, int &nRecordLength);
	// Dataset Identification field
	bool				Parsing_DSID(const BYTE *pData, int nFieldLength);
	// Dataset Structure information field
	bool				Parsing_DSSI(const BYTE *pData, int nFieldLength);
	// Attribute Codes field
	bool				Parsing_ATCS(const BYTE *pData, int nFieldLength);
	// Information Type Codes field
	bool				Parsing_ITCS(const BYTE *pData, int nFieldLength);
	// Feature Type Codes field
	bool				Parsing_FTCS(const BYTE *pData, int nFieldLength);
	// Information Association Codes field
	bool				Parsing_IACS(const BYTE *pData, int nFieldLength);
	// Feature Association Codes field
	bool				Parsing_FACS(const BYTE *pData, int nFieldLength);
	// Association Role Codes field
	bool				Parsing_ARCS(const BYTE *pData, int nFieldLength);
	// Attribute field(Metadata)
	bool				Parsing_ATTR(const BYTE *pData, VEC_ATTR_NATC &vecNATC, int nFieldLength);

	// Dataset Coordinate Reference System Record
	bool				Parsing_DCRS(const BYTE *pData, int &nRecordLength);
	// Coordinate Reference System Record Identifier field
	bool				Parsing_CSID(const BYTE *pData, int nFieldLength);
	// Coordinate Reference System Header field
	bool				Parsing_CRSH(const BYTE *pData, int nFieldLength);
	// Coordinate System Axes field
	bool				Parsing_CSAX(const BYTE *pData, int nFieldLength);
	// Projection field 
	bool				Parsing_PROJ(const BYTE *pData, int nFieldLength);
	// Geodetic Datum field
	bool				Parsing_GDAT(const BYTE *pData, int nFieldLength);
	// Vertical Datum field
	bool				Parsing_VDAT(const BYTE *pData, int nFieldLength);

	// Information Record
	bool				Parsing_InfoRecord(const BYTE *pData, ST_INFO_RECORD &stInfoRecord, int &nRecordLength);
	// Information Type Record Identifier field
	bool				Parsing_IRID(const BYTE *pData, ST_INFO_RECORD &stInfoRecord, int nFieldLength);
	// Information Association field;
	bool				Parsing_INAS(const BYTE *pData, VEC_INAS &vecINAS, int nFieldLength);

	// Point Record
	bool				Parsing_PointRecord(const BYTE *pData, ST_POINT_RECORD &stPointRecord, int &nRecordLength);
	// Point Record Identifier field
	bool				Parsing_ID(const BYTE *pData, ST_ID &stID, int nFieldLength);
	// 2-D Integer Coordinate Tuple field
	bool				Parsing_C2IT(const BYTE *pData, ST_C2IT &stC2IT, int nFieldLength);
	// 3-D Integer Coordinate Tuple field
	bool				Parsing_C3IT(const BYTE *pData, ST_C3IT &stC3IT, int nFieldLength);
	// 2-D Floating Coordinate Tuple field
	bool				Parsing_C2FT(const BYTE *pData, ST_C2FT &stC2FT, int nFieldLength);
	// 3-D Floating Coordinate Tuple field
	bool				Parsing_C3FT(const BYTE *pData, ST_C3FT &stC3FT, int nFieldLength);


	// Multi Point Record
	bool				Parsing_MultiPointRecord(const BYTE *pData, ST_MULTI_POINT_RECORD &stMultiPointRecord, int &nRecordLength);
	// Coordinate Control Field
	bool				Parsing_COCC(const BYTE *pData, ST_COCC &stCOCC, int nFieldLength);
	// 2-D Integer Coordinate List field
	bool				Parsing_C2IL(const BYTE *pData, VEC_C2IL &vecC2IL, int nFieldLength);
	// 3-D Integer Coordinate List field
	bool				Parsing_C3IL(const BYTE *pData, VEC_C3IL &vecC3IL, int nFieldLength);
	// 2-D Floating Coordinate List field
	bool				Parsing_C2FL(const BYTE *pData, VEC_C2FL &vecC2FL, int nFieldLength);
	// 3-D Floating Coordinate List field
	bool				Parsing_C3FL(const BYTE *pData, VEC_C3FL &vecC3FL, int nFieldLength);

	// Curve Record
	bool				Parsing_CurveRecord(const BYTE *pData, ST_CURVE_RECORD &stCurveRecord, int &nRecordLength);
	// Point Association field
	bool				Parsing_PTAS(const BYTE *pData, VEC_PTAS &vecPTAS, int nFieldLength);
	// Segment Control field
	bool				Parsing_SECC(const BYTE *pData, ST_SECC &stSECC, int nFieldLength);
	// Segment Header field
	bool				Parsing_SEGH(const BYTE *pData, VEC_SEGH &vecSEGH, int nFieldLength);

	// Composite Curve Record
	bool				Parsing_CompositeCurveRecord(const BYTE *pData, ST_COMP_CURVE_RECORD &stCompCurveRecord, int &nRecordLength);
	// Curve Component Control field
	bool				Parsing_CCOC(const BYTE *pData, ST_CCOC &stCCOC, int nFieldLength);
	// Curve Component field
	bool				Parsing_CUCO(const BYTE *pData, VEC_CUCO_RRNM &vecRRNM, int nFieldLength);

	// Surface Record
	bool				Parsing_SurfaceRecord(const BYTE *pData, ST_SURFACE_RECORD &stSurfaceRecord, int &nRecordLength);
	// Ring Association field
	bool				Parsing_RIAS(const BYTE *pData, VEC_RIAS_RRNM &vecRRNM, int nFieldLength);

	// Feature Type Record
	bool				Parsing_FeatureRecord(const BYTE *pData, ST_FEATURE_RECORD &stFeatureRecord, int &nRecordLength);
	// Feature Type Record Identifier field
	bool				Parsing_FRID(const BYTE *pData, ST_FRID &stFRID, int nFieldLength);
	// Feature Object Identifier field
	bool				Parsing_FOID(const BYTE *pData, ST_FOID &stFOID, int nFieldLength);
	// Spatial Association field
	bool				Parsing_SPAS(const BYTE *pData, VEC_SPAS_RRNM &vecRRNM, int nFieldLength);
	// Feature Association field
	bool				Parsing_FASC(const BYTE *pData, ST_FASC &stFASC, int nFieldLength);
	// Theme Association field
	bool				Parsing_THAS(const BYTE *pData, VEC_THAS_RRNM &vecRRNM, int nFieldLength);
	// Masked Spatial type field
	bool				Parsing_MASK(const BYTE *pData, VEC_MASK_RRNM &vecRRNM, int nFieldLength);

	// function that changes Char Array to Int.
	int					CharDataToInt(const BYTE *pData, int nCount);
	// function to find Tag.
	CString			FindTag(const BYTE *pData);
	// function obtained by converting Char Data into UTF-8.
	CString			GetUTF_8(char *pData, int &nCount);

	// Get Record by ID
	ST_POINT_RECORD*       GetPointRecord(int id);
	ST_MULTI_POINT_RECORD* GetMultiPointRecord(int id);
	ST_CURVE_RECORD*       GetCurveRecord(int id);
	ST_COMP_CURVE_RECORD*  GetCompositeCurveRecord(int id);
	ST_SURFACE_RECORD*     GetSurfaceRecord(int id);
	ST_FEATURE_RECORD*     GetFeatureRecord(int id);

	bool CheckBase();
	void Output(CString& str);

	// Error       - 0
	// Point       - 110
	// Multi Point - 115
	// PolyLine    - 120 or 125
	// Polygon     - 130
	int GetGeometryTypeFromFeatureRecord(int id);

	// Get name by code

	// Check SPAS == 1
	bool CheckSPAS();

	// fuction : Return FeatureType CamelCase
	// factor : 
	//        int featureTypeCode - Feature Type Code
	// return : 
	//        success - CamelCase
	//        fail - _T("")
	CString GetFeatureTypeCamelCase(int featureTypeCode);

	// fuction : Return CamelCase of InformationType
	// factor : 
	//        int informationTypeCode - Information Type Code
	// return : 
	//        success - CamelCase
	//        fail - _T("")
	CString GetInformationTypeCamelCase(int informationTypeCode);

	// fuction : Attribute's CamelCase is returned.
	// factor : 
	//        int attributeCode - Attribute Code
	// return :
	//        success - CamelCase
	//		  fail - _T("")
	CString GetAttributeCamelCase(int attributeCode);

	void GetPoints(ST_FEATURE_RECORD *pFeatureRecord, std::vector<D2D1_POINT_2F>& points);
	D2D1_POINT_2F GetPoint(ST_POINT_RECORD* pST_POINT_RECORD);
	void GetPoints(ST_CURVE_RECORD* pST_CURVE_RECORD, std::vector<D2D1_POINT_2F>& points);
	void GetPoints(ST_COMP_CURVE_RECORD* pST_COMP_CURVE_RECORD, std::vector<D2D1_POINT_2F>& points);
	void GetPoints(ST_SURFACE_RECORD* pST_SURFACE_RECORD, std::vector<D2D1_POINT_2F>& points);

	void GetBoundingBox(float& xmin, float& ymin, float& xmax, float& ymax, int& scaleMin, int& scaleMax);
	void GetBoundingBox(int featureID, float& xmin, float& ymin, float& xmax, float& ymax);

	void GenerateMapForRecord();

	int GetCodeByAttributeName(CString attributeName);
	int GetCodeByFeatureName(CString featureName);
	int GetCodeByInformationName(CString informationName);

	bool CheckFTCS(); // Feature Name & Nemeric Code

	void GetFeatureByFeatureName(CString featureName, std::vector<ST_FEATURE_RECORD*>& vecFeatrue);
	void GetFeatureByFeatureNumericCode(int numericCode, std::vector<ST_FEATURE_RECORD*>& vecFeatrue);

	int fileSize = 0;
};