<?xml version="1.0" encoding="utf-8" ?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">

  <xsl:import href="./templates/SimpleLineTemplate.xsl"/>

  <xsl:template match="PlannedRoute">
    <xsl:call-template name="SimpleLineTemplate">
      <xsl:with-param name="featureReference" select="@id"/>
      <xsl:with-param name="viewingGroup" select="35000"/>
      <xsl:with-param name="displayPlane" select="'UNDARRADAR'"/>
      <xsl:with-param name="drawingPriority" select="5"/>
      <xsl:with-param name="offset" select="1"/>
      <xsl:with-param name="capStyle" select="'Butt'"/>
      <xsl:with-param name="joinStyle" select="'Miter'"/>     
      <xsl:with-param name="penWidth" select="1"/>
      <xsl:with-param name="colorToken" select="'ORG'"/>
      <xsl:with-param name="trasparency" select="1"/>      
    </xsl:call-template>
  </xsl:template>
  
</xsl:transform>