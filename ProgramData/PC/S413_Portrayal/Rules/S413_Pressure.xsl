<?xml version="1.0" encoding="utf-8"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
  <xsl:template match="Pressure[@primitive='Point']" priority="1">
    <xsl:call-template name="PressureFillcolor">
      <xsl:with-param name="viewingGroup">13030</xsl:with-param>
      <xsl:with-param name="displayPlane">UNDERRADAR</xsl:with-param>
      <xsl:with-param name="drawingPriority">24</xsl:with-param>
    </xsl:call-template>
  </xsl:template>
  
  <xsl:template name="PressureFillcolor">
    <xsl:param name="viewingGroup"/>
    <!--  The viewing group  -->
    <xsl:param name="displayPlane"/>
    <!--  The display plane  -->
    <xsl:param name="drawingPriority"/>
    <xsl:variable name="Pressure">
      <xsl:value-of select="pressure"/>
    </xsl:variable>
    <xsl:variable name="Color">
      <xsl:choose>
        <xsl:when test="$Pressure = 1030.0">
          <xsl:value-of select="'PLRTE'"/>
        </xsl:when>
        <xsl:when test="$Pressure > 1016.7">
          <xsl:value-of select="'APLRT'"/>
        </xsl:when>
        <xsl:when test="$Pressure > 1003.4">
          <xsl:value-of select="'CHYLW'"/>
        </xsl:when>
        <xsl:when test="$Pressure > 990.1">
          <xsl:value-of select="'RADLO'"/>
        </xsl:when>
        <xsl:when test="$Pressure > 976.8">
          <xsl:value-of select="'RESBL'"/>
        </xsl:when>
        <xsl:when test="$Pressure > 963.5">
          <xsl:value-of select="'DEPVS'"/>
        </xsl:when>
        <xsl:when test="$Pressure > 950.0">
          <xsl:value-of select="'CHMGD'"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="'CHWHT'"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <areaInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>
        <xsl:value-of select="$viewingGroup"/>
      </viewingGroup>
      <displayPlane>
        <xsl:value-of select="$displayPlane"/>
      </displayPlane>
      <drawingPriority>
        <xsl:value-of select="$drawingPriority"/>
      </drawingPriority>
      <colorFill>
        <color>
          <xsl:value-of select="$Color"/>
        </color>
      </colorFill>
    </areaInstruction>
  </xsl:template>
</xsl:transform>


