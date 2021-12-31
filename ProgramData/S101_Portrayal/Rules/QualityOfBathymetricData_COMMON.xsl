<?xml version="1.0" encoding="UTF-8"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
   <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
   <xsl:template match="QualityOfBathymetricData[@primitive='Surface']" priority="1">
     <nullInstruction>
       <featureReference>
         <xsl:value-of select="@id"/>
       </featureReference>
       <viewingGroup>0</viewingGroup>
       <displayPlane>UNDERRADAR</displayPlane>
       <drawingPriority>0</drawingPriority>
     </nullInstruction>
     <!--<lineInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>31010</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>12</drawingPriority>
         <xsl:call-template name="simpleLineStyle">
            <xsl:with-param name="style">dash</xsl:with-param>
            <xsl:with-param name="width">2</xsl:with-param>
            <xsl:with-param name="colour">CHGRD</xsl:with-param>
         </xsl:call-template>
      </lineInstruction>-->
       </xsl:template>
</xsl:transform>
