<?xml version="1.0" encoding="UTF-8"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
   <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
   <xsl:template match="RestrictedAreaRegulatory[@primitive='Surface' and categoryOfRestrictedArea=27 ]" priority="2">
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>26010</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>15</drawingPriority>
         <symbol reference="ESSARE01">
            <areaPlacement placementMode="VisibleParts"/>
         </symbol>
      </pointInstruction>
      <lineInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>26010</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>15</drawingPriority>
         <lineStyleReference reference="ESSARE01"/>
      </lineInstruction>
   </xsl:template>
   <xsl:template match="RestrictedAreaRegulatory[@primitive='Surface' and  categoryOfRestrictedArea=28]" priority="2">
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>26010</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>15</drawingPriority>
         <symbol reference="PSSARE01">
            <areaPlacement placementMode="VisibleParts"/>
         </symbol>
      </pointInstruction>
      <lineInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>26010</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>15</drawingPriority>
         <lineStyleReference reference="ESSARE01"/>
      </lineInstruction>
   </xsl:template>
</xsl:transform>
