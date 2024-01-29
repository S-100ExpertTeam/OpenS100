#include "stdafx.h"
#include "F_DataDescriptiveField.h"

#include "../LatLonUtility/LatLonUtility.h"

#include <algorithm>

std::string DDF[50] = {
		"2600;&%/GAttribute¡ã*NATC!ATIX!PAIX!ATIN!ATVL¡ã(3b12,b11,A)¡å",
		"3600;&%/GInformation¡àAssociation¡ãRRNM!RRID!NIAC!NARC!IUIN\\\\*NATC!ATIX!PAIX!ATIN!ATVL¡ã(b11,b14,2b12,b11,3b12,b11,A)¡å",
		"3600;&%/GData¡àSet¡àIdentification¡ãRCNM!RCID!ENSP!ENED!PRSP!PRED!PROF!DSNM!DSTL!DSRD!DSLG!DSAB!DSED\\\\*DSTC¡ã(b11,b14,7A,A(8),3A,b11)¡å",
		"1600;&¡à¡à¡àData¡àSet¡àStructure¡àInformation¡ãDCOX!DCOY!DCOZ!CMFX!CMFY!CMFZ!NOIR!NOPN!NOMN!NOCN!NOXN!NOSN!NOFR¡ã(3b48,10b14)¡å",
		"2600;&¡à¡à¡àAttribute¡àCodes¡ã*ATCD!ANCD¡ã(A,b12)¡å",
		"2600;&¡à¡à¡àInformation¡àType¡àCodes¡ã*ITCD!ITNC¡ã(A,b12)¡å",
		"2600;&¡à¡à¡àFeature¡àType¡àCodes¡ã*FTCD!FTNC¡ã(A,b12)¡å",
		"2600;&¡à¡à¡àInformation¡àAssociation¡àCodes¡ã*IACD!IANC¡ã(A,b12)¡å",
		"2600;&¡à¡à¡àFeature¡àAssociation¡àCodes¡ã*FACD!FANC¡ã(A,b12)¡å",
		"2600;&¡à¡à¡àAssociation¡àCodes¡ã*ARCD!ARNC¡ã(A,b12)¡å",
		"1100;&¡à¡à¡àCoordinate¡àReference¡àSystem¡àRecord¡àIdentifier¡ãRCNM!RCID!NCRC¡ã(b11,b14,b11)¡å",
		"1600;&%/GCoordinate¡àReference¡àSystem¡àHeader¡ãCRIX!CRST!CSTY!CRNM!CRSI!CRSS!SCRI¡ã(3b11,2A,b11,A)¡å",
		"2100;&¡à¡à¡àCoordinate¡àSystem¡àAxes¡ã*AXTY!AXUM¡ã(2b11)¡å",
		"1600;&¡à¡à¡àProjection¡ãPROM!PRP1!PRP2!PRP3!PRP4!PRP5!FEAS!FNOR!¡ã(b11,7b48)¡å",
		"1600;&%/GGeodetic¡àDatum¡ãDTNM!ELNM!ESMA!ESPT!ESPM!CMNM!CMGL!¡ã(2A,b48,b11,b48,A,b48)¡å",
		"1600;&%/GVertical¡àDatum¡ã DTNM!DTID!DTSR!SCRI¡ã(2A,b11,A)¡å",
		"1100;&¡à¡à¡àInformation¡àType¡àRecord¡àIdentifier¡ãRCNM!RCID!NITC!RVER!RUIN¡ã(b11,b14,2b12,b11)¡å",
		"1100;&¡à¡à¡àCoordinate¡àControl¡ãCOUI!COIX!NCOR¡ã(b11,2b12)¡å",
		"1100;&¡à¡à¡à2-D¡àInteger¡àCoordinate¡àTuple¡ãYCOO!XCOO¡ã(2b24)¡å",
		"1100;&¡à¡à¡à3-D¡àInteger¡àCoordinate¡àTuple¡ãVCID!YCOO!XCOO!ZCOO¡ã(b11,3b24)¡å",
		"2200;&¡à¡à¡à2-D¡àFloating¡àPoint¡àCoordinate¡àTuple¡ãYCOO!XCOO¡ã(2b48)¡å",
		"3600;&¡à¡à¡à3-D¡àFloating¡àPoint¡àCoordinate¡àTuple¡ãVCID!YCOO!XCOO!ZCOO¡ã(b11,3b48)¡å",
		"2100;&¡à¡à¡à2-D¡àInteger¡àCoordinate¡àList¡ã*YCOO!XCOO¡ã(2b24)¡å",
		"3100;&¡à¡à¡à3-D¡àInteger¡àCoordinate¡àList¡ãVCID\\\\*YCOO!XCOO!ZCOO¡ã(b11,3b24)¡å",
		"2200;&¡à¡à¡à2-D¡àFloating¡àPoint¡àCoordinate¡àList¡ã*YCOO!XCOO¡ã(2b48)¡å",
		"3600;&¡à¡à¡à3-D¡àFloating¡àCoordinate¡àList¡ãVCID\\\\*YCOO!XCOO!ZCOO¡ã(b11,3b24)¡å",
		"1600;&¡à¡à¡àKnot¡ãKMUL!KVAL¡ã(b11,b48)¡å",
		"3600;&¡à¡à¡à2-D¡àDerivative¡àList¡àFloat¡ãYCOO!XCOO!DRVO!\\\\*YDRV!XDRV¡ã(2b48,b11,2b48)¡å",
		"3600;&¡à¡à¡à2-D¡àDerivative¡àList¡àInteger¡ãYCOO!XCOO!DRVO!\\\\*YDRV!XDRV¡ã(2b24,b11,2b24)¡å",
		"1100;&¡à¡à¡àPoint¡àRecord¡àIdentifier¡ãRCNM!RCID!RVER!RUIN¡ã(b11,b14,b12,b11)¡å",
		"1100;&¡à¡à¡àMulti¡àPoint¡àRecord¡àIdentifier¡ãRCNM!RCID!RVER!RUIN¡ã(b11,b14,b12,b11)¡å",
		"1100;&¡à¡à¡àCurve¡àRecord¡àIdentifier¡ãRCNM!RCID!RVER!RUIN¡ã(b11,b14,b12,b11)¡å",
		"2100;&¡à¡à¡àPoint¡àAssociation¡ã*RRNM!RRID!TOPI¡ã(b11,b14,b11)¡å",
		"1100;&¡à¡à¡àSegment¡àControl¡ãSEUI!SEIX!NSEG¡ã(b11,2b12)¡å",
		"1100;&¡à¡à¡àSegment¡àHeader¡ãINTP¡ã(b11)¡å",
		"1100;&¡à¡à¡àCircle¡àParameter¡ãRADI!RADU¡ã(b48,b11)¡å",
		"1100;&¡à¡à¡àArc¡àParameter¡ãRADI!RADU!SBRG!ANGL¡ã(b48,b11,2b48)¡å",
		"1100;&¡à¡à¡àSpline¡àParameter¡ãDEGR!KSPC!RTNL¡ã(3b11)¡å",
		"1100;&¡à¡à¡àPolynomial¡àSpline¡àParameter¡ãDEGR!KNUM!KSPC!RTNL!NDRV!NDVI¡ã(5b11)¡å",
		"1100;&¡à¡à¡àComposite¡àCurve¡àRecord¡àIdentifier¡ãRCNM!RCID!RVER!RUIN¡ã(b11,b14,b12,b11)¡å",
		"1100;&¡à¡à¡àCurve¡àComponent¡àControl¡ãCCUI!CCIX!NCCO¡ã(b11,2b12)¡å",
		"2100;&¡à¡à¡àCurve¡àComponent¡ã*RRNM!RRID!ORNT¡ã(b11,b14,b11)¡å",
		"1100;&¡à¡à¡àSurface¡àRecord¡àIdentifier¡ãRCNM!RCID!RVER!RUIN¡ã(b11,b14,b12,b11)¡å",
		"2100;&¡à¡à¡àRing¡àAssociation¡ã*RRNM!RRID!ORNT!USAG!RAUI¡ã(b11,b14,3b11)¡å",
		"1100;&¡à¡à¡àFeature¡àType¡àRecord¡àIdentifier¡ãRCNM!RCID!NFTC!RVER!RUIN¡ã(b11,b14,2b12,b11)¡å",
		"1100;&¡à¡à¡àFeature¡àObject¡àIdentifier¡ãAGEN!FIDN!FIDS¡ã(b12,b14,b12)¡å",
		"2100;&¡à¡à¡àSpatial¡àAssociation¡ã*RRNM!RRID!ORNT!SMIN!SMAX!SAUI¡ã(b11,b14,b11,2b14,b11)¡å",
		"3600;&%/GFeature¡àAssociation¡ãRRNM!RRID!NFAC!NARC!APUI\\\\*NATC!ATIX!PAIX!ATIN!ATVL¡ã(b11,b14,2b12,b11,3b12,b11,A)¡å",
		"2100;&¡à¡à¡àTheme¡àAssociation¡ã*RRNM!RRID!TAUI¡ã(b11,b14,b11)¡å",
		"2100;&¡à¡à¡àMasked¡àSpatial¡àRecord¡ã*RRNM!RRID!MIND!MUIN¡ã(b11,b14,2b11)¡å"
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

F_DataDescriptiveField* F_DataDescriptiveField::Clone() const
{
	F_DataDescriptiveField* ddf = new F_DataDescriptiveField();

	ddf->spaceOld = spaceOld;
	ddf->spaceNew = spaceNew;
	ddf->UTOld = UTOld;
	ddf->UTNew = UTNew;
	ddf->FTOld = FTOld;
	ddf->FTNew = FTNew;
	ddf->type = type;

	return ddf;
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
