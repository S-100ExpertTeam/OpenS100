<?xml version="1.0" encoding="utf-8" ?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output indent="yes" version="1.0"/>

  <xsl:template name="pointSimpleSymbolTemplate">
    <xsl:param name="featureReference"></xsl:param>
    <xsl:param name="viewingGroup"></xsl:param>
    <xsl:param name="displayPlane"></xsl:param>
    <xsl:param name="drawingPriority"></xsl:param>

    <xsl:param name="symbolReference"></xsl:param>
    <xsl:param name="rotation"></xsl:param>
    <xsl:param name="rotationCRS"></xsl:param>
    <xsl:param name="scaleFactor"></xsl:param>
    <xsl:param name="offsetX"></xsl:param>
    <xsl:param name="offsetY"></xsl:param>

    <xsl:choose>
      <xsl:when test="$featureReference!=''">
        <xsl:element name="pointInstruction">
          <xsl:element name="featureReference">
            <xsl:attribute name="reference">
              <xsl:value-of select="$featureReference"/>
            </xsl:attribute>
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
          <xsl:element name="symbol">
            <xsl:element name="symbolReference">
              <xsl:value-of select="$symbolReference"/>
            </xsl:element>
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
                <xsl:value-of select="$offsetX"/>  
              </xsl:element>
              <xsl:element name="y">
                <xsl:value-of select="$offsetY"/>
              </xsl:element>
            </xsl:element>
          </xsl:element>

          </xsl:element> 
      </xsl:when>
    </xsl:choose>
    
  </xsl:template>
  
</xsl:transform>