<?xml version="1.0" encoding="UTF-8"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
   <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
   <xsl:template match="RecommendedTrafficLanePart[@primitive='Surface']" priority="1">
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>25020</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>12</drawingPriority>
         <symbol reference="RTLDEF51">
            <areaPlacement placementMode="VisibleParts"/>
         </symbol>
      </pointInstruction>
   </xsl:template>
   <xsl:template match="RecommendedTrafficLanePart[@primitive='Point']" priority="1">
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>25020</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>12</drawingPriority>
         <symbol reference="RTLDEF51"/>
      </pointInstruction>
   </xsl:template>
   <xsl:template match="RecommendedTrafficLanePart[@primitive='Surface' and orientation/orientationValue!='']" priority="2">
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>25020</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>12</drawingPriority>
         <symbol reference="RCTLPT52" rotation="{orientation/orientationValue}">
            <areaPlacement placementMode="VisibleParts"/>
         </symbol>
      </pointInstruction>
   </xsl:template>
   <xsl:template match="RecommendedTrafficLanePart[@primitive='Point' and orientation/orientationValue!='']" priority="2">
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>25020</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>12</drawingPriority>
         <symbol reference="RCTLPT52" rotation="{orientation/orientationValue}"/>
      </pointInstruction>
   </xsl:template>
</xsl:transform>
