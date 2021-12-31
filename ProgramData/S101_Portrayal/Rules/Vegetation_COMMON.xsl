<?xml version="1.0" encoding="UTF-8"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
   <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
   <xsl:template match="Vegetation[@primitive='Surface']" priority="1">
      <areaInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>32030</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>9</drawingPriority>
         <areaFillReference reference="VEGATN03"/>
      </areaInstruction>
      <lineInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>32030</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>9</drawingPriority>
         <xsl:call-template name="simpleLineStyle">
            <xsl:with-param name="style">dash</xsl:with-param>
            <xsl:with-param name="width">0.32</xsl:with-param>
            <xsl:with-param name="colour">LANDF</xsl:with-param>
         </xsl:call-template>
      </lineInstruction>
   </xsl:template>
   <xsl:template match="Vegetation[@primitive='Surface' and (categoryOfVegetation=7 or categoryOfVegetation=21)]" priority="3">
      <areaInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>32030</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>9</drawingPriority>
         <areaFillReference reference="VEGATN04"/>
      </areaInstruction>
      <lineInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>32030</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>9</drawingPriority>
         <xsl:call-template name="simpleLineStyle">
            <xsl:with-param name="style">dash</xsl:with-param>
            <xsl:with-param name="width">0.32</xsl:with-param>
            <xsl:with-param name="colour">LANDF</xsl:with-param>
         </xsl:call-template>
      </lineInstruction>
   </xsl:template>
   <xsl:template match="Vegetation[@primitive='Curve']" priority="1">
      <lineInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>32030</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>9</drawingPriority>
         <xsl:call-template name="simpleLineStyle">
            <xsl:with-param name="style">dash</xsl:with-param>
            <xsl:with-param name="width">0.32</xsl:with-param>
            <xsl:with-param name="colour">LANDF</xsl:with-param>
         </xsl:call-template>
      </lineInstruction>
   </xsl:template>
   <xsl:template match="Vegetation[@primitive='Point']" priority="1">
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>32030</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>9</drawingPriority>
         <symbol reference="TREPNT04"/>
      </pointInstruction>
   </xsl:template>
   <xsl:template match="Vegetation[@primitive='Point' and (categoryOfVegetation=21 or categoryOfVegetation=7)]" priority="3">
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>32030</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>9</drawingPriority>
         <symbol reference="TREPNT05"/>
      </pointInstruction>
   </xsl:template>
</xsl:transform>
