<?xml version="1.0" encoding="UTF-8"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
   <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
   <xsl:template match="CableOverhead[@primitive='Curve']" priority="1">
      <lineInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>12210</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>24</drawingPriority>
         <xsl:call-template name="simpleLineStyle">
            <xsl:with-param name="style">dash</xsl:with-param>
            <xsl:with-param name="width">1.28</xsl:with-param>
            <xsl:with-param name="colour">CHGRD</xsl:with-param>
         </xsl:call-template>
      </lineInstruction>
   </xsl:template>
   <xsl:template match="CableOverhead[@primitive='Curve' and radarConspicuous='true']" priority="2">
      <lineInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>12210</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>24</drawingPriority>
         <xsl:call-template name="simpleLineStyle">
            <xsl:with-param name="style">dash</xsl:with-param>
            <xsl:with-param name="width">1.28</xsl:with-param>
            <xsl:with-param name="colour">CHGRD</xsl:with-param>
         </xsl:call-template>
      </lineInstruction>
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>12210</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>24</drawingPriority>
         <symbol reference="RACNSP01">
            <linePlacement placementMode="Relative">
               <offset>0.5</offset>
            </linePlacement>
         </symbol>
      </pointInstruction>
   </xsl:template>
   <xsl:template match="CableOverhead[@primitive='Curve' and (radarConspicuous='true' and verticalClearanceFixed/clearanceValueVertical!='')]" priority="3">
      <lineInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>12210</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>24</drawingPriority>
         <xsl:call-template name="simpleLineStyle">
            <xsl:with-param name="style">dash</xsl:with-param>
            <xsl:with-param name="width">1.28</xsl:with-param>
            <xsl:with-param name="colour">CHGRD</xsl:with-param>
         </xsl:call-template>
      </lineInstruction>
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>12210</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>24</drawingPriority>
         <symbol reference="RACNSP01">
            <linePlacement placementMode="Relative">
               <offset>0.5</offset>
            </linePlacement>
         </symbol>
      </pointInstruction>
      <xsl:if test="verticalClearanceFixed/clearanceValueVertical!= ''">
         <textInstruction>
            <featureReference>
               <xsl:value-of select="@id"/>
            </featureReference>
            <viewingGroup>11</viewingGroup>
            <displayPlane>OVERRADAR</displayPlane>
            <drawingPriority>24</drawingPriority>
            <textLine>
               <element>
                  <text>
                     <xsl:apply-templates select="verticalClearanceFixed/clearanceValueVertical" mode="text">
                        <xsl:with-param name="prefix">clr </xsl:with-param>
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
   <xsl:template match="CableOverhead[@primitive='Curve' and (radarConspicuous='true' and verticalClearanceSafe/verticalUncertainty!='')]" priority="3">
      <lineInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>12210</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>24</drawingPriority>
         <xsl:call-template name="simpleLineStyle">
            <xsl:with-param name="style">dash</xsl:with-param>
            <xsl:with-param name="width">1.28</xsl:with-param>
            <xsl:with-param name="colour">CHGRD</xsl:with-param>
         </xsl:call-template>
      </lineInstruction>
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>12210</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>24</drawingPriority>
         <symbol reference="RACNSP01">
            <linePlacement placementMode="Relative">
               <offset>0.5</offset>
            </linePlacement>
         </symbol>
      </pointInstruction>
      <xsl:if test="verticalClearanceSafe/verticalUncertainty!= ''">
         <textInstruction>
            <featureReference>
               <xsl:value-of select="@id"/>
            </featureReference>
            <viewingGroup>11</viewingGroup>
            <displayPlane>OVERRADAR</displayPlane>
            <drawingPriority>24</drawingPriority>
            <textLine>
               <element>
                  <text>
                     <xsl:apply-templates select="verticalClearanceSafe/verticalUncertainty" mode="text">
                        <xsl:with-param name="prefix">sf clr </xsl:with-param>
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
   <xsl:template match="CableOverhead[@primitive='Curve' and verticalClearanceFixed/clearanceValueVertical!='']" priority="2">
      <lineInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>12210</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>24</drawingPriority>
         <xsl:call-template name="simpleLineStyle">
            <xsl:with-param name="style">dash</xsl:with-param>
            <xsl:with-param name="width">1.28</xsl:with-param>
            <xsl:with-param name="colour">CHGRD</xsl:with-param>
         </xsl:call-template>
      </lineInstruction>
      <xsl:if test="verticalClearanceFixed/clearanceValueVertical!= ''">
         <textInstruction>
            <featureReference>
               <xsl:value-of select="@id"/>
            </featureReference>
            <viewingGroup>11</viewingGroup>
            <displayPlane>OVERRADAR</displayPlane>
            <drawingPriority>24</drawingPriority>
            <textLine>
               <element>
                  <text>
                     <xsl:apply-templates select="verticalClearanceFixed/clearanceValueVertical" mode="text">
                        <xsl:with-param name="prefix">clr </xsl:with-param>
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
   <xsl:template match="CableOverhead[@primitive='Curve' and verticalClearanceSafe/verticalUncertainty!='']" priority="2">
      <lineInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>12210</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>24</drawingPriority>
         <xsl:call-template name="simpleLineStyle">
            <xsl:with-param name="style">dash</xsl:with-param>
            <xsl:with-param name="width">1.28</xsl:with-param>
            <xsl:with-param name="colour">CHGRD</xsl:with-param>
         </xsl:call-template>
      </lineInstruction>
      <xsl:if test="verticalClearanceSafe/verticalUncertainty!= ''">
         <textInstruction>
            <featureReference>
               <xsl:value-of select="@id"/>
            </featureReference>
            <viewingGroup>11</viewingGroup>
            <displayPlane>OVERRADAR</displayPlane>
            <drawingPriority>24</drawingPriority>
            <textLine>
               <element>
                  <text>
                     <xsl:apply-templates select="verticalClearanceSafe/verticalUncertainty" mode="text">
                        <xsl:with-param name="prefix">sf clr </xsl:with-param>
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
</xsl:transform>
