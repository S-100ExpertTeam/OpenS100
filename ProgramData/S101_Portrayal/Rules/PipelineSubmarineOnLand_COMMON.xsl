<?xml version="1.0" encoding="UTF-8"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
   <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
   <xsl:template match="PipelineSubmarineOnLand[@primitive='Curve']" priority="1">
      <lineInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>34070</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>18</drawingPriority>
         <lineStyleReference reference="PIPSOL05"/>
      </lineInstruction>
   </xsl:template>
   <xsl:template match="PipelineSubmarineOnLand[@primitive='Curve' and (categoryOfPipelinePipe=2 or categoryOfPipelinePipe=3 or categoryOfPipelinePipe=4 or categoryOfPipelinePipe=5)]" priority="5">
      <lineInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>34070</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>18</drawingPriority>
         <lineStyleReference reference="PIPSOL06"/>
      </lineInstruction>
   </xsl:template>
   <xsl:template match="PipelineSubmarineOnLand[@primitive='Curve' and product=3]" priority="2">
      <lineInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>34070</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>18</drawingPriority>
         <lineStyleReference reference="PIPSOL06"/>
      </lineInstruction>
   </xsl:template>
   <xsl:template match="PipelineSubmarineOnLand[@primitive='Point']" priority="1">
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
