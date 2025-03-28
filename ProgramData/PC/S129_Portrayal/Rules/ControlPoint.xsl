<?xml version="1.0" encoding="utf-8" ?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:import href="./templates/PointSimpleSymbolTemplate.xsl"/>
  
  <xsl:template match="UnderKeelClearanceControlPoint">
    <xsl:call-template name="pointSimpleSymbolTemplate">
      <xsl:with-param name="featureReference" select="@id"/>
      <xsl:with-param name="viewingGroup" select="29050"/>
      <xsl:with-param name="displayPlane" select="'UnderRadar'"/>
      <xsl:with-param name="drawingPriority" select="21"/>
      
      <xsl:with-param name="symbolReference" select="'UKCCONPT'"/>
      <xsl:with-param name="rotation" select="0"/>
      <xsl:with-param name="rotationCRS" select="'portrayalCRS'"/>
      <xsl:with-param name="scaleFactor" select="1"/>
      <xsl:with-param name="offsetX" select="0"/>
      <xsl:with-param name="offsetY" select="0"/>
    </xsl:call-template>
  </xsl:template>
  
</xsl:transform>
