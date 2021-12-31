<?xml version="1.0" encoding="UTF-8"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
   <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
   <xsl:template match="UnsurveyedArea[@primitive='Surface']" priority="1">
      <areaInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>11050</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>3</drawingPriority>
         <colorFill>
            <color>NODTA</color>
         </colorFill>
      </areaInstruction>
      <areaInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>11050</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>3</drawingPriority>
         <areaFillReference reference="NODATA03"/>
      </areaInstruction>
      <lineInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>11050</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>3</drawingPriority>
         <xsl:call-template name="simpleLineStyle">
            <xsl:with-param name="style">solid</xsl:with-param>
            <xsl:with-param name="width">0.64</xsl:with-param>
            <xsl:with-param name="colour">CHGRD</xsl:with-param>
         </xsl:call-template>
      </lineInstruction>
   </xsl:template>
</xsl:transform>
