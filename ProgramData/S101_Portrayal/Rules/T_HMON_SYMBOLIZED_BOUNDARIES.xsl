<?xml version="1.0" encoding="UTF-8"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
   <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
   <xsl:template match="T_HMON[@primitive='Surface']" priority="1">
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>33050</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>6</drawingPriority>
         <symbol reference="TIDEHT01">
            <areaPlacement placementMode="VisibleParts"/>
         </symbol>
      </pointInstruction>
      <lineInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>33050</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>6</drawingPriority>
         <lineStyleReference reference="TIDINF51"/>
      </lineInstruction>
   </xsl:template>
</xsl:transform>
