<?xml version="1.0" encoding="utf-8"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
  
    <xsl:template match="AvgWaveDir[@primitive='Point']" priority="1">
    <xsl:call-template name="select_AvgWaveDir">
      <xsl:with-param name="viewingGroup">13030</xsl:with-param>
      <xsl:with-param name="displayPlane">UNDERRADAR</xsl:with-param>
      <xsl:with-param name="drawingPriority">10</xsl:with-param>
    </xsl:call-template>
  </xsl:template>
  
  <xsl:template name="select_AvgWaveDir">
    <xsl:param name="viewingGroup"/>
    <!--  The viewing group  -->
    <xsl:param name="displayPlane"/>
    <!--  The display plane  -->
    <xsl:param name="drawingPriority"/>
    <xsl:variable name="Direction">
      <xsl:choose>
        <xsl:when test="avgWaveDir and number(avgWaveDir) = number(avgWaveDir) ">
          <xsl:value-of select="number(avgWaveDir)"/>
        </xsl:when>
        <xsl:otherwise>10</xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <pointInstruction>
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
      <xsl:element name="symbol">
        <xsl:attribute name="reference">SCAROW01</xsl:attribute>
        <xsl:attribute name="rotation">
          <xsl:value-of select="$Direction"/>
        </xsl:attribute>
      </xsl:element>
    </pointInstruction>
  </xsl:template>
</xsl:transform>


