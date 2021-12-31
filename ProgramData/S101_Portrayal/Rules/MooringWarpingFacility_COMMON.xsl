<?xml version="1.0" encoding="UTF-8"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
   <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
   <xsl:template match="MooringWarpingFacility[@primitive='Surface']" priority="1">
      <areaInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>12410</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>18</drawingPriority>
         <colorFill>
            <color>CHBRN</color>
         </colorFill>
      </areaInstruction>
      <lineInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>12410</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>18</drawingPriority>
         <xsl:call-template name="simpleLineStyle">
            <xsl:with-param name="style">solid</xsl:with-param>
            <xsl:with-param name="width">0.32</xsl:with-param>
            <xsl:with-param name="colour">CHBLK</xsl:with-param>
         </xsl:call-template>
      </lineInstruction>
   </xsl:template>
   <xsl:template match="MooringWarpingFacility[@primitive='Curve']" priority="1">
      <lineInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>12410</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>18</drawingPriority>
         <xsl:call-template name="simpleLineStyle">
            <xsl:with-param name="style">solid</xsl:with-param>
            <xsl:with-param name="width">0.64</xsl:with-param>
            <xsl:with-param name="colour">CSTLN</xsl:with-param>
         </xsl:call-template>
      </lineInstruction>
   </xsl:template>
   <xsl:template match="MooringWarpingFacility[@primitive='Curve' and categoryOfMooringWarpingFacility=6]" priority="2">
      <lineInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>14010</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>18</drawingPriority>
         <xsl:call-template name="simpleLineStyle">
            <xsl:with-param name="style">dash</xsl:with-param>
            <xsl:with-param name="width">0.32</xsl:with-param>
            <xsl:with-param name="colour">CHMGF</xsl:with-param>
         </xsl:call-template>
      </lineInstruction>
   </xsl:template>
   <xsl:template match="MooringWarpingFacility[@primitive='Point']" priority="1">
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>12410</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>18</drawingPriority>
         <symbol reference="MORFAC03"/>
      </pointInstruction>
   </xsl:template>
   <xsl:template match="MooringWarpingFacility[@primitive='Point' and categoryOfMooringWarpingFacility=2]" priority="2">
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>12410</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>18</drawingPriority>
         <symbol reference="MORFAC04"/>
      </pointInstruction>
   </xsl:template>
   <xsl:template match="MooringWarpingFacility[@primitive='Point' and categoryOfMooringWarpingFacility=3]" priority="2">
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>32440</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>18</drawingPriority>
         <symbol reference="PILPNT02"/>
      </pointInstruction>
   </xsl:template>
   <xsl:template match="MooringWarpingFacility[@primitive='Point' and categoryOfMooringWarpingFacility=5]" priority="2">
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>12410</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>18</drawingPriority>
         <symbol reference="PILPNT02"/>
      </pointInstruction>
   </xsl:template>
   <xsl:template match="MooringWarpingFacility[@primitive='Point' and categoryOfMooringWarpingFacility=7]" priority="2">
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>27010</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>24</drawingPriority>
         <symbol reference="BOYMOR11"/>
      </pointInstruction>
   </xsl:template>
</xsl:transform>
