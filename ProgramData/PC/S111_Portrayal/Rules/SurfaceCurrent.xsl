<?xml version="1.0" encoding="UTF-8"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
   <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
  <xsl:template match="SurfaceCurrent[@primitive='Point']" priority="1">
    <xsl:call-template name="select_arrow">
      <xsl:with-param name="viewingGroup">13030</xsl:with-param>
      <xsl:with-param name="displayPlane">UNDERRADAR</xsl:with-param>
      <xsl:with-param name="drawingPriority">10</xsl:with-param>
    </xsl:call-template>
  </xsl:template>
</xsl:transform>