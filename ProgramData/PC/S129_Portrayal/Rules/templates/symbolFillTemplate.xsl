<?xml version="1.0" encoding="utf-8" ?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output indent="yes" version="1.0"/>

  <xsl:template name="symbolFillTemplate">
    <xsl:param name="featureReference"/>
    <xsl:param name="viewingGroup"/>
    <xsl:param name="displayPlane"/>
    <xsl:param name="drawingPriority"/>
    <xsl:param name="symbolReference"/>
    <xsl:param name="rotation"/>
    <xsl:param name="rotationCRS"/>
    <xsl:param name="scaleFactor"/>
    <xsl:param name="symbolOffsetX"/>
    <xsl:param name="symbolOffsetY"/>
    <xsl:param name="x1"/>
    <xsl:param name="y1"/>
    <xsl:param name="x2"/>
    <xsl:param name="y2"/>
    <xsl:param name="clipSymbols"/>

    <xsl:choose>
      <xsl:when test="$featureReference!=''">
        <xsl:element name="areaInstruction">
          <xsl:element name="featureReference">
            <xsl:value-of select="$featureReference"/>
          </xsl:element>
          <xsl:element name="viewingGroup">
              <xsl:value-of select="$viewingGroup"/>
          </xsl:element>
          <xsl:element name="displayPlane">
            <xsl:value-of select="$displayPlane"/>
          </xsl:element>
          <xsl:element name="drawingPriority">
            <xsl:value-of select="$drawingPriority"/>
          </xsl:element>
          <xsl:element name="symbolFill">
            <xsl:element name="symbol">
              <xsl:attribute name="reference">
                <xsl:value-of select="$symbolReference"/>
              </xsl:attribute>
              <xsl:element name="rotation">
                <xsl:value-of select="$rotation"/>
              </xsl:element>
              <xsl:element name="rotationCRS">
                <xsl:value-of select="$rotationCRS"/>
              </xsl:element>
              <xsl:element name="scaleFactor">
                <xsl:value-of select="$scaleFactor"/>
              </xsl:element>
              <xsl:element name="offset">
                <xsl:element name="x">
                  <xsl:value-of select="$symbolOffsetX"/>  
                </xsl:element>
                <xsl:element name="y">
                  <xsl:value-of select="$symbolOffsetY"/>
                </xsl:element>
              </xsl:element>
            </xsl:element>
            <xsl:element name="v1">
              <xsl:element name="x">
                <xsl:value-of select="$x1"/>
              </xsl:element>
              <xsl:element name="y">
                <xsl:value-of select="$y1"/>
              </xsl:element>
            </xsl:element>
            <xsl:element name="v2">
              <xsl:element name="x">
                <xsl:value-of select="$x2"/>
              </xsl:element>
              <xsl:element name="y">
                <xsl:value-of select="$y2"/>
              </xsl:element>
            </xsl:element>
            <xsl:element name="clipSymbols">
                <xsl:value-of select="$clipSymbols"/>
              </xsl:element>
          </xsl:element>
        </xsl:element>
      </xsl:when>
    </xsl:choose>
  </xsl:template>
</xsl:transform>
