<?xml version="1.0" encoding="UTF-8"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
   <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
   <xsl:template match="SubmarinePipelineArea[@primitive='Surface']" priority="1">
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>34030</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>9</drawingPriority>
         <symbol reference="INFARE51">
            <areaPlacement placementMode="VisibleParts"/>
         </symbol>
      </pointInstruction>
      <lineInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>34030</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>9</drawingPriority>
         <lineStyleReference reference="PIPARE51"/>
      </lineInstruction>
      <xsl:call-template name="RESCSP03">
         <xsl:with-param name="viewingGroup">34030</xsl:with-param>
         <xsl:with-param name="displayPlane">UNDERRADAR</xsl:with-param>
         <xsl:with-param name="drawingPriority">9</xsl:with-param>
      </xsl:call-template>
   </xsl:template>
   <xsl:template match="SubmarinePipelineArea[@primitive='Surface' and (categoryOfPipelinePipe=2 or categoryOfPipelinePipe=3)]" priority="3">
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>34030</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>9</drawingPriority>
         <symbol reference="INFARE51">
            <areaPlacement placementMode="VisibleParts"/>
         </symbol>
      </pointInstruction>
      <lineInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>34030</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>9</drawingPriority>
         <lineStyleReference reference="PIPARE61"/>
      </lineInstruction>
      <xsl:call-template name="RESCSP03">
         <xsl:with-param name="viewingGroup">34030</xsl:with-param>
         <xsl:with-param name="displayPlane">UNDERRADAR</xsl:with-param>
         <xsl:with-param name="drawingPriority">9</xsl:with-param>
      </xsl:call-template>
   </xsl:template>
   <xsl:template match="SubmarinePipelineArea[@primitive='Surface' and product=3]" priority="2">
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>34030</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>9</drawingPriority>
         <symbol reference="INFARE51">
            <areaPlacement placementMode="VisibleParts"/>
         </symbol>
      </pointInstruction>
      <lineInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>34030</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>9</drawingPriority>
         <lineStyleReference reference="PIPARE61"/>
      </lineInstruction>
      <xsl:call-template name="RESCSP03">
         <xsl:with-param name="viewingGroup">34030</xsl:with-param>
         <xsl:with-param name="displayPlane">UNDERRADAR</xsl:with-param>
         <xsl:with-param name="drawingPriority">9</xsl:with-param>
      </xsl:call-template>
   </xsl:template>
</xsl:transform>
