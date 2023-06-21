<?xml version="1.0" encoding="UTF-8"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
   <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
   <xsl:template match="NauticalProducts[@primitive='Surface']" priority="1"> 
      <lineInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>12010</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>24</drawingPriority>
         <xsl:call-template name="simpleLineStyle">
            <xsl:with-param name="style">dot</xsl:with-param>
            <xsl:with-param name="width">0.32</xsl:with-param>
            <xsl:with-param name="colour">RESBL</xsl:with-param>
         </xsl:call-template>
      </lineInstruction>
   </xsl:template>
   
   <xsl:template match="NauticalProducts[@primitive='Point']" priority="1">
      <textInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>26</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>9</drawingPriority>
         <textPoint horizontalAlignment="Left" verticalAlignment="Bottom">
            <element>
               <text>
                  <xsl:apply-templates select="featureName" mode="text"/>
               </text>
               <xsl:call-template name="textStyle">
                  <xsl:with-param name="style">default</xsl:with-param>
               </xsl:call-template>
            </element>
            <offset>
               <x>-2</x>
               <y>1</y>
            </offset>
            <areaPlacement placementMode="VisibleParts"/>
         </textPoint>
      </textInstruction>
   </xsl:template>
   
   <xsl:template match="NauticalProducts[@primitive='Point' and textPlaceCorner=1]" priority="2">
      <textInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>26</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>9</drawingPriority>
         <textPoint horizontalAlignment="Left" verticalAlignment="Bottom">
            <element>
               <text>
                  <xsl:apply-templates select="featureName" mode="text"/>
               </text>
               <xsl:call-template name="textStyle">
                  <xsl:with-param name="style">default</xsl:with-param>
               </xsl:call-template>
            </element>
            <offset>
               <x>-2</x>
               <y>1</y>
            </offset>
            <areaPlacement placementMode="VisibleParts"/>
         </textPoint>
      </textInstruction>
   </xsl:template>
   
   <xsl:template match="NauticalProducts[@primitive='Point' and textPlaceCorner=2]" priority="2">
      <textInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>26</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>9</drawingPriority>
         <textPoint horizontalAlignment="Right" verticalAlignment="Top">
            <element>
               <text>
                  <xsl:apply-templates select="featureName" mode="text"/>
               </text>
               <xsl:call-template name="textStyle">
                  <xsl:with-param name="style">default</xsl:with-param>
               </xsl:call-template>
            </element>
            <offset>
               <x>2</x>
               <y>-1</y>
            </offset>
            <areaPlacement placementMode="VisibleParts"/>
         </textPoint>
      </textInstruction>
   </xsl:template>
</xsl:transform>
