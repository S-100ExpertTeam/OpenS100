<?xml version="1.0" encoding="UTF-8"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
   <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
   <xsl:template match="BuiltUpArea[@primitive='Surface']" priority="1">
      <areaInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>22240</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>9</drawingPriority>
         <colorFill>
            <color>CHBRN</color>
         </colorFill>
      </areaInstruction>
      <xsl:if test="featureName!= ''">
         <textInstruction>
            <featureReference>
               <xsl:value-of select="@id"/>
            </featureReference>
            <viewingGroup>26</viewingGroup>
            <displayPlane>UNDERRADAR</displayPlane>
            <drawingPriority>9</drawingPriority>
            <textPoint horizontalAlignment="Center" verticalAlignment="Center">
               <element>
                  <text>
                     <xsl:apply-templates select="featureName" mode="text"/>
                  </text>
                  <xsl:call-template name="textStyle">
                     <xsl:with-param name="style">default</xsl:with-param>
                  </xsl:call-template>
               </element>
               <offset>
                  <x>0</x>
                  <y>0</y>
               </offset>
               <areaPlacement placementMode="VisibleParts"/>
            </textPoint>
         </textInstruction>
      </xsl:if>
      <lineInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>22240</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>9</drawingPriority>
         <xsl:call-template name="simpleLineStyle">
            <xsl:with-param name="style">solid</xsl:with-param>
            <xsl:with-param name="width">0.32</xsl:with-param>
            <xsl:with-param name="colour">LANDF</xsl:with-param>
         </xsl:call-template>
      </lineInstruction>
   </xsl:template>
   <xsl:template match="BuiltUpArea[@primitive='Point']" priority="1">
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>22240</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>9</drawingPriority>
         <symbol reference="BUAARE02"/>
      </pointInstruction>
      <xsl:if test="featureName!= ''">
         <textInstruction>
            <featureReference>
               <xsl:value-of select="@id"/>
            </featureReference>
            <viewingGroup>26</viewingGroup>
            <displayPlane>OVERRADAR</displayPlane>
            <drawingPriority>9</drawingPriority>
            <textPoint verticalAlignment="Center">
               <element>
                  <text>
                     <xsl:apply-templates select="featureName" mode="text"/>
                  </text>
                  <xsl:call-template name="textStyle">
                     <xsl:with-param name="style">default</xsl:with-param>
                  </xsl:call-template>
               </element>
               <offset>
                  <x>3.51</x>
                  <y>0</y>
               </offset>
            </textPoint>
         </textInstruction>
      </xsl:if>
   </xsl:template>
</xsl:transform>
