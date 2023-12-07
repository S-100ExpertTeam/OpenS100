#pragma once

#include "SGeometry.h"
#include "Record.h"
#include "Field.h"

#define DB2_V72_FIX_BYTE_ORDER(x) ((((x)&0x31) == (x)) ? ((x)&0x1) : (x))

typedef enum
{
	XDR = 0, /**< MSB/Sun/Motorola: Most Significant Byte First   */
	NDR = 1  /**< LSB/Intel/Vax: Least Significant Byte First      */
} ByteOrder;

typedef enum
{
	Field_DSID = 1,
	Field_PRID = 2,
	Field_MRID = 3,
	Field_CRID = 4,
	Field_CCID = 5,
	Field_SRID = 6,
	Field_FRID = 7,
	Field_PTAS = 11,
	Field_SEGH = 12,
	Field_CUCO = 13,
	Field_RIAS = 14,
	Field_ATTR = 21,
	Field_INAS = 22,
	Field_SPAS = 23,
	Field_FASC = 24,
	Field_MASK = 25
} S101FieldType;

using namespace GISLibrary;

class S101GeometryFactory
{
public:
	static SGeometry* CreateFromWkb(void*, size_t = static_cast<size_t>(-1));
	static SGeometry* CreateGeometry(SGeometryType);
	static void DestroyGeometry(SGeometry*);

	static Record* CreateRecord(RCNM);
	static void DestroyRecord(Record*);
};

class S101FieldFactory
{
public:
	static Field* CreateField(S101FieldType FieldType);
	static void DestoryField(Field*);
};

class S101GeometryUtil
{
public:
	static SGeometryType ReadWKBGeometryType(const unsigned char*);
};