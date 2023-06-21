<?xml version="1.0" encoding="UTF-8"?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
  <xsl:template match="DifferentDisplayByScale[@primitive='Point']" priority="1">
    <!--<pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>34050</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="ICEARE04P"/>
    </pointInstruction>-->

    <xsl:call-template name="differentDisplayByScaleTemplate">
      <xsl:with-param name="minScale" select="minimumRangeSymbolScale"/>
      <xsl:with-param name="maxScale" select="maximumRangeSymbolScale"/>
      <xsl:with-param name="featureReference" select="@id"/>
      <xsl:with-param name="viewingGroup" select="33021"/>
      <xsl:with-param name="displayPlane" select="'UNDERRADAR'"/>
      <xsl:with-param name="drawingPriority" select="15"/>
    </xsl:call-template>
  </xsl:template>
</xsl:transform>
