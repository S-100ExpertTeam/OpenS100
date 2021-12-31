<?xml version="1.0" encoding="UTF-8"?>

<!-- Modified Sept 2016 to call the DEPARE04 template to look for safety contours around safe depth regions-->	

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
   <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
   <xsl:template match="DepthArea[@primitive='Surface']" priority="1">
      <xsl:call-template name="seabed01">
         <xsl:with-param name="viewingGroup">13030</xsl:with-param>
         <xsl:with-param name="displayPlane">UNDERRADAR</xsl:with-param>
         <xsl:with-param name="drawingPriority">3</xsl:with-param>
      </xsl:call-template>
      <xsl:call-template name="DEPARE04"/>
   </xsl:template>
   <xsl:template match="DepthArea[@primitive='Surface' and (depthRangeMinimumValue='?' and depthRangeMaximumValue='?')]" priority="3">
      <areaInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>13030</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>1</drawingPriority>
         <colorFill>
            <color>NODTA</color>
         </colorFill>
      </areaInstruction>
      <areaInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>13030</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>1</drawingPriority>
         <areaFillReference reference="PRTSUR01"/>
      </areaInstruction>
      <lineInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>13030</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>3</drawingPriority>
         <xsl:call-template name="simpleLineStyle">
            <xsl:with-param name="style">solid</xsl:with-param>
            <xsl:with-param name="width">0.64</xsl:with-param>
            <xsl:with-param name="colour">CHGRD</xsl:with-param>
         </xsl:call-template>
      </lineInstruction>
   </xsl:template>
   <xsl:template match="DepthArea[@primitive='Curve']" priority="1">
      <nullInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>0</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>0</drawingPriority>
      </nullInstruction>
   </xsl:template>
</xsl:transform>
