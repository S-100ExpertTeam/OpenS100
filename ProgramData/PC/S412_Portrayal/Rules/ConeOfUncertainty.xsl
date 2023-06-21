<?xml version="1.0" encoding="utf-8"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
  <xsl:template match="ConeOfUncertainty[@primitive='Surface']" priority="1">
     <areaInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>31000</viewingGroup>
      <displayPlane>UNDERRADAR</displayPlane>
      <drawingPriority>14</drawingPriority>
      <colorFill>
        <color transparency="75">CHRED</color>
      </colorFill>
    </areaInstruction>
    
  </xsl:template>

</xsl:transform>


