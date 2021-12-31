<?xml version="1.0" encoding="UTF-8"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
   <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
   <xsl:template match="TrafficSeparationSchemeLanePart[@primitive='Surface' and orientation/orientationValue!='']" priority="2">
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>25010</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>18</drawingPriority>
         <symbol reference="TSSLPT51" rotation="{orientation/orientationValue}">
            <areaPlacement placementMode="VisibleParts"/>
         </symbol>
      </pointInstruction>
      <xsl:call-template name="RESCSP03">
         <xsl:with-param name="viewingGroup">25010</xsl:with-param>
         <xsl:with-param name="displayPlane">UNDERRADAR</xsl:with-param>
         <xsl:with-param name="drawingPriority">18</xsl:with-param>
      </xsl:call-template>
   </xsl:template>
</xsl:transform>
