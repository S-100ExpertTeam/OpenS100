<?xml version="1.0" encoding="UTF-8"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
   <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
   <xsl:template match="TrafficSeparationSchemeLanePart[@primitive='Surface']" priority="1">
      <areaInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>25010</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>18</drawingPriority>
         <areaFillReference reference="TSSJCT02"/>
      </areaInstruction>
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>25010</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>18</drawingPriority>
         <symbol reference="CTNARE51">
            <areaPlacement placementMode="VisibleParts"/>
         </symbol>
      </pointInstruction>
      <xsl:if test="information/text!= ''">
         <textInstruction>
            <featureReference>
               <xsl:value-of select="@id"/>
            </featureReference>
            <viewingGroup>24</viewingGroup>
            <displayPlane>UNDERRADAR</displayPlane>
            <drawingPriority>18</drawingPriority>
            <textPoint horizontalAlignment="Center">
               <element>
                  <text>
                     <xsl:apply-templates select="information/text" mode="text"/>
                  </text>
                  <xsl:call-template name="textStyle">
                     <xsl:with-param name="style">default</xsl:with-param>
                  </xsl:call-template>
               </element>
               <offset>
                  <x>0</x>
                  <y>7.02</y>
               </offset>
               <areaPlacement placementMode="VisibleParts"/>
            </textPoint>
         </textInstruction>
      </xsl:if>
      <xsl:call-template name="RESCSP03">
         <xsl:with-param name="viewingGroup">25010</xsl:with-param>
         <xsl:with-param name="displayPlane">UNDERRADAR</xsl:with-param>
         <xsl:with-param name="drawingPriority">18</xsl:with-param>
      </xsl:call-template>
   </xsl:template>
</xsl:transform>
