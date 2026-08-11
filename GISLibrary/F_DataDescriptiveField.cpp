#include "stdafx.h"
#include "F_DataDescriptiveField.h"

#include "../LatLonUtility/LatLonUtility.h"

#include <algorithm>

std::string DDF[50] = {
		"2600;&%/GAttribute▲*NATC!ATIX!PAIX!ATIN!ATVL▲(3b12,b11,A)▼",
		"3600;&%/GInformation□Association▲RRNM!RRID!NIAC!NARC!IUIN\\\\*NATC!ATIX!PAIX!ATIN!ATVL▲(b11,b14,2b12,b11,3b12,b11,A)▼",
		"3600;&%/GData□Set□Identification▲RCNM!RCID!ENSP!ENED!PRSP!PRED!PROF!DSNM!DSTL!DSRD!DSLG!DSAB!DSED\\\\*DSTC▲(b11,b14,7A,A(8),3A,b11)▼",
		"1600;&□□□Data□Set□Structure□Information▲DCOX!DCOY!DCOZ!CMFX!CMFY!CMFZ!NOIR!NOPN!NOMN!NOCN!NOXN!NOSN!NOFR▲(3b48,10b14)▼",
		"2600;&□□□Attribute□Codes▲*ATCD!ANCD▲(A,b12)▼",
		"2600;&□□□Information□Type□Codes▲*ITCD!ITNC▲(A,b12)▼",
		"2600;&□□□Feature□Type□Codes▲*FTCD!FTNC▲(A,b12)▼",
		"2600;&□□□Information□Association□Codes▲*IACD!IANC▲(A,b12)▼",
		"2600;&□□□Feature□Association□Codes▲*FACD!FANC▲(A,b12)▼",
		"2600;&□□□Association□Codes▲*ARCD!ARNC▲(A,b12)▼",
		"1100;&□□□Coordinate□Reference□System□Record□Identifier▲RCNM!RCID!NCRC▲(b11,b14,b11)▼",
		"1600;&%/GCoordinate□Reference□System□Header▲CRIX!CRST!CSTY!CRNM!CRSI!CRSS!SCRI▲(3b11,2A,b11,A)▼",
		"2100;&□□□Coordinate□System□Axes▲*AXTY!AXUM▲(2b11)▼",
		"1600;&□□□Projection▲PROM!PRP1!PRP2!PRP3!PRP4!PRP5!FEAS!FNOR!▲(b11,7b48)▼",
		"1600;&%/GGeodetic□Datum▲DTNM!ELNM!ESMA!ESPT!ESPM!CMNM!CMGL!▲(2A,b48,b11,b48,A,b48)▼",
		"1600;&%/GVertical□Datum▲ DTNM!DTID!DTSR!SCRI▲(2A,b11,A)▼",
		"1100;&□□□Information□Type□Record□Identifier▲RCNM!RCID!NITC!RVER!RUIN▲(b11,b14,2b12,b11)▼",
		"1100;&□□□Coordinate□Control▲COUI!COIX!NCOR▲(b11,2b12)▼",
		"1100;&□□□2-D□Integer□Coordinate□Tuple▲YCOO!XCOO▲(2b24)▼",
		"1100;&□□□3-D□Integer□Coordinate□Tuple▲VCID!YCOO!XCOO!ZCOO▲(b11,3b24)▼",
		"2200;&□□□2-D□Floating□Point□Coordinate□Tuple▲YCOO!XCOO▲(2b48)▼",
		"3600;&□□□3-D□Floating□Point□Coordinate□Tuple▲VCID!YCOO!XCOO!ZCOO▲(b11,3b48)▼",
		"2100;&□□□2-D□Integer□Coordinate□List▲*YCOO!XCOO▲(2b24)▼",
		"3100;&□□□3-D□Integer□Coordinate□List▲VCID\\\\*YCOO!XCOO!ZCOO▲(b11,3b24)▼",
		"2200;&□□□2-D□Floating□Point□Coordinate□List▲*YCOO!XCOO▲(2b48)▼",
		"3600;&□□□3-D□Floating□Coordinate□List▲VCID\\\\*YCOO!XCOO!ZCOO▲(b11,3b24)▼",
		"1600;&□□□Knot▲KMUL!KVAL▲(b11,b48)▼",
		"3600;&□□□2-D□Derivative□List□Float▲YCOO!XCOO!DRVO!\\\\*YDRV!XDRV▲(2b48,b11,2b48)▼",
		"3600;&□□□2-D□Derivative□List□Integer▲YCOO!XCOO!DRVO!\\\\*YDRV!XDRV▲(2b24,b11,2b24)▼",
		"1100;&□□□Point□Record□Identifier▲RCNM!RCID!RVER!RUIN▲(b11,b14,b12,b11)▼",
		"1100;&□□□Multi□Point□Record□Identifier▲RCNM!RCID!RVER!RUIN▲(b11,b14,b12,b11)▼",
		"1100;&□□□Curve□Record□Identifier▲RCNM!RCID!RVER!RUIN▲(b11,b14,b12,b11)▼",
		"2100;&□□□Point□Association▲*RRNM!RRID!TOPI▲(b11,b14,b11)▼",
		"1100;&□□□Segment□Control▲SEUI!SEIX!NSEG▲(b11,2b12)▼",
		"1100;&□□□Segment□Header▲INTP▲(b11)▼",
		"1100;&□□□Circle□Parameter▲RADI!RADU▲(b48,b11)▼",
		"1100;&□□□Arc□Parameter▲RADI!RADU!SBRG!ANGL▲(b48,b11,2b48)▼",
		"1100;&□□□Spline□Parameter▲DEGR!KSPC!RTNL▲(3b11)▼",
		"1100;&□□□Polynomial□Spline□Parameter▲DEGR!KNUM!KSPC!RTNL!NDRV!NDVI▲(5b11)▼",
		"1100;&□□□Composite□Curve□Record□Identifier▲RCNM!RCID!RVER!RUIN▲(b11,b14,b12,b11)▼",
		"1100;&□□□Curve□Component□Control▲CCUI!CCIX!NCCO▲(b11,2b12)▼",
		"2100;&□□□Curve□Component▲*RRNM!RRID!ORNT▲(b11,b14,b11)▼",
		"1100;&□□□Surface□Record□Identifier▲RCNM!RCID!RVER!RUIN▲(b11,b14,b12,b11)▼",
		"2100;&□□□Ring□Association▲*RRNM!RRID!ORNT!USAG!RAUI▲(b11,b14,3b11)▼",
		"1100;&□□□Feature□Type□Record□Identifier▲RCNM!RCID!NFTC!RVER!RUIN▲(b11,b14,2b12,b11)▼",
		"1100;&□□□Feature□Object□Identifier▲AGEN!FIDN!FIDS▲(b12,b14,b12)▼",
		"2100;&□□□Spatial□Association▲*RRNM!RRID!ORNT!SMIN!SMAX!SAUI▲(b11,b14,b11,2b14,b11)▼",
		"3600;&%/GFeature□Association▲RRNM!RRID!NFAC!NARC!APUI\\\\*NATC!ATIX!PAIX!ATIN!ATVL▲(b11,b14,2b12,b11,3b12,b11,A)▼",
		"2100;&□□□Theme□Association▲*RRNM!RRID!TAUI▲(b11,b14,b11)▼",
		"2100;&□□□Masked□Spatial□Record▲*RRNM!RRID!MIND!MUIN▲(b11,b14,2b11)▼"
};

std::string DDFTagName[50] =
{
	"ATTR",
	"INAS",
	"DSID",
	"DSSI",
	"ATCS",
	"ITCS",
	"FTCS",
	"IACS",
	"FACS",
	"ARCS",
	"CSID",
	"CRSH",
	"CSAX",
	"PROJ",
	"GDAT",
	"VDAT",
	"IRID",
	"COCC",
	"C2IT",
	"C3IT",
	"C2FT",
	"C3FT",
	"C2IL",
	"C3IL",
	"C2FL",
	"C3FL",
	"KNOT",
	"DRVF",
	"DRVI",
	"PRID",
	"MRID",
	"CRID",
	"PTAS",
	"SECC",
	"SEGH",
	"CIPM",
	"ARPM",
	"SPLI",
	"PSPL",
	"CCID",
	"CCOC",
	"CUCO",
	"SRID",
	"RIAS",
	"FRID",
	"FOID",
	"SPAS",
	"FASC",
	"THAS",
	"MASK"
};

F_DataDescriptiveField::F_DataDescriptiveField()
{

}

F_DataDescriptiveField::F_DataDescriptiveField(const F_DataDescriptiveField& other)
{
	spaceOld = other.spaceOld;
	spaceNew = other.spaceNew;
	UTOld = other.UTOld;
	UTNew = other.UTNew;
	FTOld = other.FTOld;
	FTNew = other.FTNew;
	type = other.type;
}

F_DataDescriptiveField::F_DataDescriptiveField(DDFType type)
{
	this->type = (int)type;
}

F_DataDescriptiveField::~F_DataDescriptiveField()
{

}

std::string F_DataDescriptiveField::Replace(std::string text)
{
	text = LatLonUtility::replace_all(text, spaceOld, spaceNew);
	text = LatLonUtility::replace_all(text, UTOld, UTNew);
	text = LatLonUtility::replace_all(text, FTOld, FTNew);

	return text;
}

bool F_DataDescriptiveField::WriteField(CFile* file)
{
	std::string ddf = DDF[type];

	ddf = Replace(ddf);

	file->Write(ddf.data(), (UINT)ddf.size());

	return true;
}

int F_DataDescriptiveField::GetFieldLength()
{
	std::string ddf = DDF[type];

	ddf = Replace(ddf);

	return (int)ddf.length();
}

std::string F_DataDescriptiveField::GetTagName()
{
	return DDFTagName[type].substr(0, 4);
}

F_DataDescriptiveField F_DataDescriptiveField::operator=(const F_DataDescriptiveField& other)
{
	spaceOld = other.spaceOld;
	spaceNew = other.spaceNew;
	UTOld = other.UTOld;
	UTNew = other.UTNew;
	FTOld = other.FTOld;
	FTNew = other.FTNew;
	type = other.type;

	return *this;
}

bool F_DataDescriptiveField::operator==(const F_DataDescriptiveField& other) const
{
	return this->type == other.type;
}

//bool F_DataDescriptiveField::operator<(const F_DataDescriptiveField& other) const
//{
//	return this->type < other.type;
//}
//
//
//bool F_DataDescriptiveField::operator<(const F_DataDescriptiveField& other) const
//{
//	return this->type < other.type;
//}
