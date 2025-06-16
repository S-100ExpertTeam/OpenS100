<?xml version="1.0" encoding="utf-8" ?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">

  <xsl:import href="./templates/areaHatchFillTemplate.xsl"/>
  <xsl:import href="./templates/areaSimpleColorFillTemplate.xsl"/>

  <xsl:template match="UnderKeelClearanceNonNavigableArea">
    
    <xsl:call-template name="AreaSimpleColorFillTemplate">
      <xsl:with-param name="featureReference" select="@id"/>
      <xsl:with-param name="viewingGroup" select="33022"/>
      <xsl:with-param name="displayPlane" select="'UNDERRADAR'"/>
      <xsl:with-param name="drawingPriority" select="9"/>
      <xsl:with-param name="colorToken" select="'RED'"/>
      <xsl:with-param name="transparency" select="0.9"/>
    </xsl:call-template>

    <xsl:call-template name="AreaHatchFillTemplate">
      <xsl:with-param name="featureReference" select="@id"/>
      <xsl:with-param name="viewingGroup" select="33022"/>
      <xsl:with-param name="displayPlane" select="'UNDERRADAR'"/>
      <xsl:with-param name="drawingPriority" select="12"/>
      <xsl:with-param name="x" select="1"/>
      <xsl:with-param name="y" select="-1"/>
      <xsl:with-param name="distance" select="1"/>
      <xsl:with-param name="offset" select="0"/>
      <xsl:with-param name="capStyle" select="'Butt'"/>
      <xsl:with-param name="joinStyle" select="'Miter'"/>
      <xsl:with-param name="penWidth" select="1"/>
      <xsl:with-param name="colorToken" select="'RED'"/>
      <xsl:with-param name="transparency" select="1"/>     
    </xsl:call-template>      
	
  </xsl:template>
</xsl:transform>
