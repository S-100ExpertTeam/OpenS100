<?xml version="1.0" encoding="utf-8" ?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output indent="yes" version="1.0"/>

  <xsl:template name="differentDisplayByScaleTemplate">
    <xsl:param name="featureReference"></xsl:param>
    <xsl:param name="viewingGroup"></xsl:param>
    <xsl:param name="displayPlane"></xsl:param>
    <xsl:param name="drawingPriority"></xsl:param>

    <xsl:variable name="Orient">
      <xsl:value-of select="direction"/>
    </xsl:variable>
    <xsl:variable name="Speed">
      <xsl:value-of select="speed"/>
    </xsl:variable>

    <!-- 1번째 심볼 범위 Scale -->
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
      <scaleMinimum>0</scaleMinimum>
      <rotation>
        <xsl:value-of select="$Orient"/>
      </rotation>
      <xsl:element name="symbol">
        <xsl:attribute name="reference">OWNSHP01</xsl:attribute>
      </xsl:element>
    </pointInstruction>
    
  </xsl:template>
  
</xsl:transform>