<?xml version="1.0" encoding="UTF-8"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
   <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
   <xsl:template match="CableSubmarine[@primitive='Curve']" priority="1">
      <lineInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>34070</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>9</drawingPriority>
         <lineStyleReference reference="CBLSUB06"/>
      </lineInstruction>
   </xsl:template>
   <xsl:template match="CableSubmarine[@primitive='Curve' and categoryOfCable=6]" priority="2">
      <lineInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>24010</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>18</drawingPriority>
         <xsl:call-template name="simpleLineStyle">
            <xsl:with-param name="style">dash</xsl:with-param>
            <xsl:with-param name="width">0.32</xsl:with-param>
            <xsl:with-param name="colour">CHMGD</xsl:with-param>
         </xsl:call-template>
      </lineInstruction>
   </xsl:template>
</xsl:transform>
