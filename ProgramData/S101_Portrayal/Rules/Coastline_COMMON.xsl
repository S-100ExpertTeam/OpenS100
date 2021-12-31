<?xml version="1.0" encoding="UTF-8"?>

<!--Modified Sept 2016 to call QUALIN03 -->

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
   <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
   <xsl:template match="Coastline[@primitive='Curve']" priority="1">
      <lineInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>12410</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>21</drawingPriority>
         <xsl:call-template name="simpleLineStyle">
            <xsl:with-param name="style">solid</xsl:with-param>
            <xsl:with-param name="width">0.32</xsl:with-param>
            <xsl:with-param name="colour">CSTLN</xsl:with-param>
         </xsl:call-template>
      </lineInstruction>
	  <!-- call QUALIN03 override curve drawing for low accuracy curves with a lowacc21 linestyle -->
      <xsl:call-template name="QUALIN03">
         <xsl:with-param name="viewingGroup">12410</xsl:with-param>
         <xsl:with-param name="displayPlane">OVERRADAR</xsl:with-param>
         <xsl:with-param name="drawingPriority">22</xsl:with-param>
      </xsl:call-template>
   </xsl:template>
   <xsl:template match="Coastline[@primitive='Curve' and radarConspicuous = 'true']" priority="2">
      <lineInstruction suppression="false" >
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>12410</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>21</drawingPriority>
         <xsl:call-template name="simpleLineStyle">
            <xsl:with-param name="style">solid</xsl:with-param>
            <xsl:with-param name="width">0.96</xsl:with-param>
            <xsl:with-param name="colour">CHMGF</xsl:with-param>
         </xsl:call-template>
      </lineInstruction>
      <lineInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>12410</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>22</drawingPriority>
         <xsl:call-template name="simpleLineStyle">
            <xsl:with-param name="style">solid</xsl:with-param>
            <xsl:with-param name="width">0.32</xsl:with-param>
            <xsl:with-param name="colour">CSTLN</xsl:with-param>
         </xsl:call-template>
      </lineInstruction>
	  <!-- call QUALIN03 override curve drawing for low accuracy curves with a lowacc21 linestyle on top of the fat magenta line -->
      <xsl:call-template name="QUALIN03">
         <xsl:with-param name="viewingGroup">12410</xsl:with-param>
         <xsl:with-param name="displayPlane">OVERRADAR</xsl:with-param>
         <xsl:with-param name="drawingPriority">23</xsl:with-param>
      </xsl:call-template>
   </xsl:template>
   <xsl:template match="Coastline[@primitive='Curve' and (categoryOfCoastline=6 or categoryOfCoastline=7 or categoryOfCoastline=8 or categoryOfCoastline=10)]" priority="5">
      <lineInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>12410</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>21</drawingPriority>
         <xsl:call-template name="simpleLineStyle">
            <xsl:with-param name="style">dash</xsl:with-param>
            <xsl:with-param name="width">0.32</xsl:with-param>
            <xsl:with-param name="colour">CSTLN</xsl:with-param>
         </xsl:call-template>
      </lineInstruction>
   </xsl:template>
</xsl:transform>
