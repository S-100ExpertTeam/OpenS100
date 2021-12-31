<?xml version="1.0" encoding="UTF-8"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
   <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
   <xsl:template match="DepthContour[@primitive='Curve']" priority="1">
      <xsl:call-template name="DEPCNT03">
         <xsl:with-param name="viewingGroup">33020</xsl:with-param>
         <xsl:with-param name="displayPlane">OVERRADAR</xsl:with-param>
         <xsl:with-param name="drawingPriority">15</xsl:with-param>
      </xsl:call-template>
   </xsl:template>
</xsl:transform>
