<?xml version="1.0" encoding="utf-8" ?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  
  <xsl:import href="./templates/areaHatchFillTemplate.xsl"/>
  <xsl:import href="./templates/areaSimpleColorFillTemplate.xsl"/>
  
  <xsl:template match="UnderKeelClearanceAlmostNonNavigableArea">
  
    <xsl:call-template name="AreaSimpleColorFillTemplate">
      <xsl:with-param name="featureReference" select="@id"/>
      <xsl:with-param name="viewingGroup" select="29020"/>
      <xsl:with-param name="displayPlane" select="'UnderRadar'"/>
      <xsl:with-param name="drawingPriority" select="6"/>
      <xsl:with-param name="colorToken" select="'GOLDN'"/>
      <xsl:with-param name="transparency" select="0.5"/>
    </xsl:call-template>
  
  </xsl:template>  

</xsl:transform>
