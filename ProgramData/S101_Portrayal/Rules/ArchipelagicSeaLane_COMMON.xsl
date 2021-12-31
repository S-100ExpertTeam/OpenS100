<?xml version="1.0" encoding="UTF-8"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
   <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
   <xsl:template match="ArchipelagicSeaLane[@primitive='Surface']" priority="1">
      <lineInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>26260</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>15</drawingPriority>
         <lineStyleReference reference="ARCSLN01"/>
      </lineInstruction>
   </xsl:template>
</xsl:transform>
