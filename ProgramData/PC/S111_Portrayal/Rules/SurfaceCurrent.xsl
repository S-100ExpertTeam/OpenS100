<?xml version="1.0" encoding="UTF-8"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
   <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
  
  <xsl:include href="select_arrow.xsl"/>
  
  <xsl:template match="SurfaceCurrent[@primitive='Coverage']" priority="1">
    <xsl:call-template name="select_arrow">
      <xsl:with-param name="viewingGroup">33060</xsl:with-param>
      <xsl:with-param name="displayPlane">UnderRadar</xsl:with-param>
      <xsl:with-param name="drawingPriority">10</xsl:with-param>
    </xsl:call-template>
  </xsl:template>
</xsl:transform>