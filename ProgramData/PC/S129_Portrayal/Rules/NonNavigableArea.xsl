<?xml version="1.0" encoding="utf-8" ?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">

  <xsl:import href="./templates/areaHatchFillTemplate.xsl"/>
  <xsl:import href="./templates/areaSimpleColorFillTemplate.xsl"/>
  <xsl:import href="./templates/symbolFillTemplate.xsl"/>

  <xsl:template match="UnderKeelClearanceNonNavigableArea">

    <xsl:call-template name="AreaSimpleColorFillTemplate">
      <xsl:with-param name="featureReference" select="@id"/>
      <xsl:with-param name="viewingGroup" select="29030"/>
      <xsl:with-param name="displayPlane" select="'UnderRadar'"/>
      <xsl:with-param name="drawingPriority" select="6"/>
      <xsl:with-param name="colorToken" select="'RED'"/>
      <xsl:with-param name="transparency" select="0.5"/>
    </xsl:call-template>

    <xsl:call-template name="symbolFillTemplate">
      <xsl:with-param name="featureReference" select="@id"/>
      <xsl:with-param name="viewingGroupA" select="29030"/>
      <xsl:with-param name="viewingGroupB" select="29040"/>
      <xsl:with-param name="displayPlane" select="'UnderRadar'"/>
      <xsl:with-param name="drawingPriority" select="6"/>
      <xsl:with-param name="symbolReference" select="'DIAMOND1P'"/>
      <xsl:with-param name="rotation" select="0"/>
      <xsl:with-param name="rotationCRS" select="GeographicCRS"/>
      <xsl:with-param name="scaleFactor" select="1"/>
      <xsl:with-param name="symbolOffsetX" select="0"/>
      <xsl:with-param name="symbolOffsetY" select="0"/>
      <xsl:with-param name="x1" select="22.5"/>
      <xsl:with-param name="y1" select="0"/>
      <xsl:with-param name="x2" select="0"/>
      <xsl:with-param name="y2" select="43.13"/>
      <xsl:with-param name="clipSymbols" select="false"/>
    </xsl:call-template>

    <xsl:call-template name="symbolFillTemplate">
      <xsl:with-param name="featureReference" select="@id"/>
      <xsl:with-param name="viewingGroupA" select="29030"/>
      <xsl:with-param name="viewingGroupB" select="29040"/>
      <xsl:with-param name="displayPlane" select="'UnderRadar'"/>
      <xsl:with-param name="drawingPriority" select="6"/>
      <xsl:with-param name="symbolReference" select="'DIAMOND1P'"/>
      <xsl:with-param name="rotation" select="0"/>
      <xsl:with-param name="rotationCRS" select="GeographicCRS"/>
      <xsl:with-param name="scaleFactor" select="1"/>
      <xsl:with-param name="symbolOffsetX" select="1"/>
      <xsl:with-param name="symbolOffsetY" select="0"/>
      <xsl:with-param name="x1" select="22.5"/>
      <xsl:with-param name="y1" select="0"/>
      <xsl:with-param name="x2" select="0"/>
      <xsl:with-param name="y2" select="43.13"/>
      <xsl:with-param name="clipSymbols" select="false"/>
    </xsl:call-template>
  </xsl:template>
</xsl:transform>
