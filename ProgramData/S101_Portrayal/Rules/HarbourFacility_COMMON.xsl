<?xml version="1.0" encoding="UTF-8"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
   <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
   <xsl:template match="HarbourFacility[@primitive='Surface']" priority="1">
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>32410</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>12</drawingPriority>
         <symbol reference="CHINFO07">
            <areaPlacement placementMode="VisibleParts"/>
         </symbol>
      </pointInstruction>
   </xsl:template>
   <xsl:template match="HarbourFacility[@primitive='Point']" priority="1">
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>32410</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>12</drawingPriority>
         <symbol reference="CHINFO07"/>
      </pointInstruction>
   </xsl:template>
   <xsl:template match="HarbourFacility[@primitive='Surface' and categoryOfHarbourFacility=1]" priority="2">
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>32410</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>12</drawingPriority>
         <symbol reference="ROLROL01">
            <areaPlacement placementMode="VisibleParts"/>
         </symbol>
      </pointInstruction>
   </xsl:template>
   <xsl:template match="HarbourFacility[@primitive='Surface' and categoryOfHarbourFacility=4]" priority="2">
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>32410</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>12</drawingPriority>
         <symbol reference="HRBFAC09">
            <areaPlacement placementMode="VisibleParts"/>
         </symbol>
      </pointInstruction>
   </xsl:template>
   <xsl:template match="HarbourFacility[@primitive='Point' and categoryOfHarbourFacility=4]" priority="2">
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>32410</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>12</drawingPriority>
         <symbol reference="HRBFAC09"/>
      </pointInstruction>
   </xsl:template>
   <xsl:template match="HarbourFacility[@primitive='Surface' and categoryOfHarbourFacility=5]" priority="2">
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>32410</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>12</drawingPriority>
         <symbol reference="SMCFAC02">
            <areaPlacement placementMode="VisibleParts"/>
         </symbol>
      </pointInstruction>
   </xsl:template>
   <xsl:template match="HarbourFacility[@primitive='Point' and categoryOfHarbourFacility=5]" priority="2">
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>32410</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>12</drawingPriority>
         <symbol reference="SMCFAC02"/>
      </pointInstruction>
   </xsl:template>
   <xsl:template match="HarbourFacility[@primitive='Point' and categoryOfHarbourFacility=1]" priority="2">
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>32410</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>12</drawingPriority>
         <symbol reference="ROLROL01"/>
      </pointInstruction>
   </xsl:template>
</xsl:transform>
