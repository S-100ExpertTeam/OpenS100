<?xml version="1.0" encoding="UTF-8"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
   <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
   <xsl:template match="SeabedArea[@primitive='Surface']" priority="1">
      <xsl:call-template name="natsur01">
         <xsl:with-param name="viewingGroup">34010</xsl:with-param>
         <xsl:with-param name="displayPlane">OVERRADAR</xsl:with-param>
         <xsl:with-param name="drawingPriority">9</xsl:with-param>
      </xsl:call-template>
   </xsl:template>
   <xsl:template match="SeabedArea[@primitive='Surface' and ((waterLevelEffect=3 or waterLevelEffect=4) and surfaceCharacteristics/natureOfSurface!='')]" priority="4">
      <xsl:call-template name="natsur01">
         <xsl:with-param name="viewingGroup">34010</xsl:with-param>
         <xsl:with-param name="displayPlane">OVERRADAR</xsl:with-param>
         <xsl:with-param name="drawingPriority">9</xsl:with-param>
      </xsl:call-template>
      <lineInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>34010</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>9</drawingPriority>
         <xsl:call-template name="simpleLineStyle">
            <xsl:with-param name="style">dash</xsl:with-param>
            <xsl:with-param name="width">0.32</xsl:with-param>
            <xsl:with-param name="colour">CHGRD</xsl:with-param>
         </xsl:call-template>
      </lineInstruction>
   </xsl:template>
   <xsl:template match="SeabedArea[@primitive='Surface' and (waterLevelEffect=4 and (surfaceCharacteristics/natureOfSurface=11 or surfaceCharacteristics/natureOfSurface=14 or surfaceCharacteristics/natureOfSurface=9))]" priority="5">
      <areaInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>34010</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>9</drawingPriority>
         <areaFillReference reference="RCKLDG01"/>
      </areaInstruction>
      <lineInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>34010</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>9</drawingPriority>
         <xsl:call-template name="simpleLineStyle">
            <xsl:with-param name="style">dash</xsl:with-param>
            <xsl:with-param name="width">0.32</xsl:with-param>
            <xsl:with-param name="colour">CHGRD</xsl:with-param>
         </xsl:call-template>
      </lineInstruction>
   </xsl:template>
   <xsl:template match="SeabedArea[@primitive='Curve']" priority="1">
      <lineInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>34010</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>12</drawingPriority>
         <xsl:call-template name="simpleLineStyle">
            <xsl:with-param name="style">solid</xsl:with-param>
            <xsl:with-param name="width">0.32</xsl:with-param>
            <xsl:with-param name="colour">CHGRD</xsl:with-param>
         </xsl:call-template>
      </lineInstruction>
      <xsl:call-template name="natsur01">
         <xsl:with-param name="viewingGroup">34010</xsl:with-param>
         <xsl:with-param name="displayPlane">OVERRADAR</xsl:with-param>
         <xsl:with-param name="drawingPriority">12</xsl:with-param>
      </xsl:call-template>
   </xsl:template>
   <xsl:template match="SeabedArea[@primitive='Point']" priority="1">
      <xsl:call-template name="natsur01">
         <xsl:with-param name="viewingGroup">34010</xsl:with-param>
         <xsl:with-param name="displayPlane">OVERRADAR</xsl:with-param>
         <xsl:with-param name="drawingPriority">12</xsl:with-param>
      </xsl:call-template>
   </xsl:template>
</xsl:transform>
