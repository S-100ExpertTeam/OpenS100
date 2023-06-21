<?xml version="1.0" encoding="utf-8"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
  <xsl:template match="SnowAmount[@primitive='Point']" priority="1">
    <xsl:call-template name="SnowAmountFillcolor">
      <xsl:with-param name="viewingGroup">13030</xsl:with-param>
      <xsl:with-param name="displayPlane">UNDERRADAR</xsl:with-param>
      <xsl:with-param name="drawingPriority">24</xsl:with-param>
    </xsl:call-template>
  </xsl:template>

  <xsl:template name="SnowAmountFillcolor">
    <xsl:param name="viewingGroup"/>
    <!--  The viewing group  -->
    <xsl:param name="displayPlane"/>
    <!--  The display plane  -->
    <xsl:param name="drawingPriority"/>
    <xsl:variable name="SnowAmount">
      <xsl:value-of select="snowAmount"/>
    </xsl:variable>
    <xsl:variable name="Color">
      <xsl:choose>
        <xsl:when test="$SnowAmount > 10.0">
          <xsl:value-of select="'PLRTE'"/>
        </xsl:when>
        <xsl:when test="$SnowAmount > 8.0">
          <xsl:value-of select="'APLRT'"/>
        </xsl:when>
        <xsl:when test="$SnowAmount > 6.4">
          <xsl:value-of select="'CHYLW'"/>
        </xsl:when>
        <xsl:when test="$SnowAmount > 4.8">
          <xsl:value-of select="'RADLO'"/>
        </xsl:when>
        <xsl:when test="$SnowAmount > 3.2">
          <xsl:value-of select="'RESBL'"/>
        </xsl:when>
        <xsl:when test="$SnowAmount > 1.6">
          <xsl:value-of select="'DEPVS'"/>
        </xsl:when>
        <xsl:when test="$SnowAmount > 0.0">
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


