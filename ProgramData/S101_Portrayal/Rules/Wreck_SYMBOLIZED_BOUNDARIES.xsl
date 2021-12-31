<?xml version="1.0" encoding="UTF-8"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
   <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
   <xsl:template match="Wreck[@primitive='Surface' and categoryOfWreck=3]" priority="2">
      <lineInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>34050</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>12</drawingPriority>
         <lineStyleReference reference="NAVARE51"/>
      </lineInstruction>
   </xsl:template>
   <xsl:template match="Wreck[@primitive='Surface' and (categoryOfWreck=3 and valueOfSounding!='')]" priority="3">
      <lineInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>34051</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>12</drawingPriority>
         <lineStyleReference reference="NAVARE51"/>
      </lineInstruction>
   </xsl:template>
</xsl:transform>
