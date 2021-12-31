<?xml version="1.0" encoding="UTF-8"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
   <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
   <xsl:template match="Causeway[@primitive='Surface']" priority="1">
      <areaInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>22010</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>15</drawingPriority>
         <colorFill>
            <color>CHBRN</color>
         </colorFill>
      </areaInstruction>
      <lineInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>22010</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>15</drawingPriority>
         <xsl:call-template name="simpleLineStyle">
            <xsl:with-param name="style">solid</xsl:with-param>
            <xsl:with-param name="width">0.32</xsl:with-param>
            <xsl:with-param name="colour">CSTLN</xsl:with-param>
         </xsl:call-template>
      </lineInstruction>
   </xsl:template>
   <xsl:template match="Causeway[@primitive='Surface' and waterLevelEffect=4]" priority="2">
      <areaInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>22010</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>15</drawingPriority>
         <colorFill>
            <color>DEPIT</color>
         </colorFill>
      </areaInstruction>
      <lineInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>22010</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>15</drawingPriority>
         <xsl:call-template name="simpleLineStyle">
            <xsl:with-param name="style">dash</xsl:with-param>
            <xsl:with-param name="width">0.64</xsl:with-param>
            <xsl:with-param name="colour">CSTLN</xsl:with-param>
         </xsl:call-template>
      </lineInstruction>
   </xsl:template>
   <xsl:template match="Causeway[@primitive='Curve']" priority="1">
      <lineInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>22010</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>15</drawingPriority>
         <xsl:call-template name="simpleLineStyle">
            <xsl:with-param name="style">solid</xsl:with-param>
            <xsl:with-param name="width">0.96</xsl:with-param>
            <xsl:with-param name="colour">LANDF</xsl:with-param>
         </xsl:call-template>
      </lineInstruction>
   </xsl:template>
</xsl:transform>
