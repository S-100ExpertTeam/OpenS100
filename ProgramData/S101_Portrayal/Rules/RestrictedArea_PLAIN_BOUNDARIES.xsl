<?xml version="1.0" encoding="UTF-8"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
   <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
   <xsl:template match="RestrictedAreaRegulatory[@primitive='Surface']" priority="1">
      <xsl:call-template name="RESARE04">
         <xsl:with-param name="viewingGroup">26010</xsl:with-param>
         <xsl:with-param name="displayPlane">OVERRADAR</xsl:with-param>
         <xsl:with-param name="drawingPriority">15</xsl:with-param>
         <xsl:with-param name="symbolizedBoundaries">false</xsl:with-param>
      </xsl:call-template>
   </xsl:template>
</xsl:transform>
