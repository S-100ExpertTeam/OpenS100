<?xml version="1.0" encoding="UTF-8"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
   <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
   <xsl:template match="MagneticVariation[@primitive='Surface']" priority="1">
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>31080</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>12</drawingPriority>
         <symbol reference="MAGVAR51">
            <areaPlacement placementMode="VisibleParts"/>
         </symbol>
      </pointInstruction>
   </xsl:template>
   <xsl:template match="MagneticVariation[@primitive='Curve']" priority="1">
      <lineInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>31080</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>12</drawingPriority>
         <xsl:call-template name="simpleLineStyle">
            <xsl:with-param name="style">solid</xsl:with-param>
            <xsl:with-param name="width">0.64</xsl:with-param>
            <xsl:with-param name="colour">CHMGF</xsl:with-param>
         </xsl:call-template>
      </lineInstruction>
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>31080</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>12</drawingPriority>
         <symbol reference="MAGVAR51">
            <linePlacement placementMode="Relative">
               <offset>0.5</offset>
            </linePlacement>
         </symbol>
      </pointInstruction>
      <xsl:if test="valueOfMagneticVariation!= ''">
         <textInstruction>
            <featureReference>
               <xsl:value-of select="@id"/>
            </featureReference>
            <viewingGroup>27</viewingGroup>
            <displayPlane>OVERRADAR</displayPlane>
            <drawingPriority>12</drawingPriority>
            <textLine>
               <element>
                  <text>
                     <xsl:apply-templates select="valueOfMagneticVariation" mode="text">
                        <xsl:with-param name="prefix">varn </xsl:with-param>
                     </xsl:apply-templates>
                  </text>
                  <xsl:call-template name="textStyle">
                     <xsl:with-param name="style">default</xsl:with-param>
                  </xsl:call-template>
               </element>
               <startOffset>0.5</startOffset>
               <placementMode>Relative</placementMode>
            </textLine>
         </textInstruction>
      </xsl:if>
   </xsl:template>
   <xsl:template match="MagneticVariation[@primitive='Point']" priority="1">
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>31080</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>12</drawingPriority>
         <symbol reference="MAGVAR01"/>
      </pointInstruction>
      <xsl:if test="valueOfMagneticVariation!= ''">
         <textInstruction>
            <featureReference>
               <xsl:value-of select="@id"/>
            </featureReference>
            <viewingGroup>27</viewingGroup>
            <displayPlane>OVERRADAR</displayPlane>
            <drawingPriority>12</drawingPriority>
            <textPoint>
               <element>
                  <text>
                     <xsl:apply-templates select="valueOfMagneticVariation" mode="text"/>
                  </text>
                  <xsl:call-template name="textStyle">
                     <xsl:with-param name="style">default</xsl:with-param>
                  </xsl:call-template>
               </element>
               <offset>
                  <x>3.51</x>
                  <y>3.51</y>
               </offset>
            </textPoint>
         </textInstruction>
      </xsl:if>
   </xsl:template>
</xsl:transform>
