<?xml version="1.0" encoding="UTF-8"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
   <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
   <xsl:template match="Berth[@primitive='Surface']" priority="1">
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>32440</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>9</drawingPriority>
         <symbol reference="BRTHNO01">
            <areaPlacement placementMode="VisibleParts"/>
         </symbol>
      </pointInstruction>
      <xsl:if test="featureName!= ''">
         <textInstruction>
            <featureReference>
               <xsl:value-of select="@id"/>
            </featureReference>
            <viewingGroup>29</viewingGroup>
            <displayPlane>UNDERRADAR</displayPlane>
            <drawingPriority>9</drawingPriority>
            <textPoint>
               <element>
                  <text>
                     <xsl:apply-templates select="featureName" mode="text">
                        <xsl:with-param name="prefix">Nr </xsl:with-param>
                     </xsl:apply-templates>
                  </text>
                  <xsl:call-template name="textStyle">
                     <xsl:with-param name="style">default</xsl:with-param>
                  </xsl:call-template>
               </element>
               <offset>
                  <x>3.51</x>
                  <y>0</y>
               </offset>
               <areaPlacement placementMode="VisibleParts"/>
            </textPoint>
         </textInstruction>
      </xsl:if>
   </xsl:template>
   <xsl:template match="Berth[@primitive='Curve']" priority="1">
      <lineInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>32440</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>15</drawingPriority>
         <xsl:call-template name="simpleLineStyle">
            <xsl:with-param name="style">solid</xsl:with-param>
            <xsl:with-param name="width">0.96</xsl:with-param>
            <xsl:with-param name="colour">CHGRD</xsl:with-param>
         </xsl:call-template>
      </lineInstruction>
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>32440</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>15</drawingPriority>
         <symbol reference="BRTHNO01">
            <linePlacement placementMode="Relative">
               <offset>0.5</offset>
            </linePlacement>
         </symbol>
      </pointInstruction>
      <xsl:if test="featureName!= ''">
         <textInstruction>
            <featureReference>
               <xsl:value-of select="@id"/>
            </featureReference>
            <viewingGroup>29</viewingGroup>
            <displayPlane>OVERRADAR</displayPlane>
            <drawingPriority>15</drawingPriority>
            <textLine horizontalAlignment="Center" verticalAlignment="Center">
               <element>
                  <text>
                     <xsl:apply-templates select="featureName" mode="text">
                        <xsl:with-param name="prefix">Nr </xsl:with-param>
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
   <xsl:template match="Berth[@primitive='Point']" priority="1">
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>32440</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>15</drawingPriority>
         <symbol reference="BRTHNO01"/>
      </pointInstruction>
      <xsl:if test="featureName!= ''">
         <textInstruction>
            <featureReference>
               <xsl:value-of select="@id"/>
            </featureReference>
            <viewingGroup>29</viewingGroup>
            <displayPlane>OVERRADAR</displayPlane>
            <drawingPriority>15</drawingPriority>
            <textPoint>
               <element>
                  <text>
                     <xsl:apply-templates select="featureName" mode="text">
                        <xsl:with-param name="prefix">Nr </xsl:with-param>
                     </xsl:apply-templates>
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
