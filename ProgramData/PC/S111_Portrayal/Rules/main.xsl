<?xml version="1.0" encoding="UTF-8"?>
<!-- 
  Version 2.0.0 Build 20240801 Update of the S-111 Edition 1.0.0 PC for S-111 Edition 2.0.0
    Updated by Raphael Malyankar (Portolan Sciences LLC)
    Notes: (1) Expects all DCFs in S-111 to generate "Coverage" spatial primitives for portrayal purposes
      (cf. S-100 5.2.0 9-7.2), i.e., designed for use with an S-111 FC with the "pointSet" spatial primitive
      removed; (2) Based on S-100 Ed. 5.2.0; (3) INDEX_OF_TIMES is a residual from the Ed. 1.0.0 PC whose
      rationale is unknown as of July 2024; (4) The lookup table in the surface current template intentionally
      omits entries for current speed zero and fill value, implying no symbolization for zero or noData speeds.
-->
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
  <xsl:decimal-format name="dformat" decimal-separator="." grouping-separator=","/>

  <xsl:include href="SurfaceCurrent.xsl"/>
  
  <!-- Not clear what the parameter INDEX_OF_TIMES is for -->
  <xsl:param name="INDEX_OF_TIMES" select ="0"/>

  <xsl:template match="/">
    <p:displayList xmlns:p="http://www.iho.int/S100Presentation/5.2">
      <xsl:apply-templates select="Dataset/Features/*"/>
    </p:displayList>
  </xsl:template>
</xsl:transform>