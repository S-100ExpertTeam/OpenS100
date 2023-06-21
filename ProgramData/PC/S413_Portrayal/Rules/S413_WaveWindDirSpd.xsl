<?xml version="1.0" encoding="utf-8"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
  <xsl:template match="WaveWind[@primitive='Point']" priority="1">
    <xsl:variable name="Speed" select="speed"/>
    <xsl:variable name="Direction" select="direction"/>
    <xsl:variable name="scale">
      <xsl:choose>
        <xsl:when test="$Speed >= 1">
          <xsl:value-of select="speed * 0.5"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="0.5"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:choose>
      <xsl:when test="0.49 >= $Speed">
        <pointInstruction>
          <featureReference>
            <xsl:value-of select="@id"/>
          </featureReference>
          <viewingGroup>26010</viewingGroup>
          <displayPlane>UNDERRADAR</displayPlane>
          <drawingPriority>15</drawingPriority>
          <xsl:element name="symbol">
            <xsl:attribute name="reference">SCAROW01</xsl:attribute>
            <xsl:attribute name="rotation">
              <xsl:value-of select="$Direction"/>
            </xsl:attribute>
            <xsl:attribute name="scaleFactor">
              <xsl:value-of select="$scale"></xsl:value-of>
            </xsl:attribute>
          </xsl:element>
        </pointInstruction>
      </xsl:when>
      <xsl:when test="0.99 >= $Speed">
        <pointInstruction>
          <featureReference>
            <xsl:value-of select="@id"/>
          </featureReference>
          <viewingGroup>26010</viewingGroup>
          <displayPlane>UNDERRADAR</displayPlane>
          <drawingPriority>15</drawingPriority>
          <xsl:element name="symbol">
            <xsl:attribute name="reference">SCAROW02</xsl:attribute>
            <xsl:attribute name="rotation">
              <xsl:value-of select="$Direction"/>
            </xsl:attribute>
            <xsl:attribute name="scaleFactor">
              <xsl:value-of select="$scale"></xsl:value-of>
            </xsl:attribute>
          </xsl:element>
        </pointInstruction>
      </xsl:when>
      <xsl:otherwise>
        <pointInstruction>
          <featureReference>
            <xsl:value-of select="@id"/>
          </featureReference>
          <viewingGroup>26010</viewingGroup>
          <displayPlane>UNDERRADAR</displayPlane>
          <drawingPriority>15</drawingPriority>
          <xsl:element name="symbol">
            <xsl:attribute name="reference">SCAROW09</xsl:attribute>
            <xsl:attribute name="rotation">
              <xsl:value-of select="$Direction"/>
            </xsl:attribute>
            <xsl:attribute name="scaleFactor">
              <xsl:value-of select="$scale"></xsl:value-of>
            </xsl:attribute>
          </xsl:element>
        </pointInstruction>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>
</xsl:transform>