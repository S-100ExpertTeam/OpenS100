<?xml version="1.0" encoding="UTF-8"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
   <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
   <xsl:template match="DistanceMark[@primitive='Point']" priority="1">
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>27030</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>21</drawingPriority>
         <symbol reference="DISMAR03"/>
      </pointInstruction>
      <xsl:if test="information/text!= ''">
         <textInstruction>
            <featureReference>
               <xsl:value-of select="@id"/>
            </featureReference>
            <viewingGroup>21</viewingGroup>
            <displayPlane>OVERRADAR</displayPlane>
            <drawingPriority>21</drawingPriority>
            <textPoint horizontalAlignment="End">
               <element>
                  <text>
                     <xsl:apply-templates select="information/text" mode="text"/>
                  </text>
                  <xsl:call-template name="textStyle">
                     <xsl:with-param name="style">default</xsl:with-param>
                  </xsl:call-template>
               </element>
               <offset>
                  <x>7.02</x>
                  <y>0</y>
               </offset>
            </textPoint>
         </textInstruction>
      </xsl:if>
   </xsl:template>
   <xsl:template match="DistanceMark[@primitive='Point' and categoryOfDistanceMark=1]" priority="2">
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>27030</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>21</drawingPriority>
         <symbol reference="DISMAR04"/>
      </pointInstruction>
      <xsl:if test="information/text!= ''">
         <textInstruction>
            <featureReference>
               <xsl:value-of select="@id"/>
            </featureReference>
            <viewingGroup>21</viewingGroup>
            <displayPlane>OVERRADAR</displayPlane>
            <drawingPriority>21</drawingPriority>
            <textPoint horizontalAlignment="End">
               <element>
                  <text>
                     <xsl:apply-templates select="information/text" mode="text"/>
                  </text>
                  <xsl:call-template name="textStyle">
                     <xsl:with-param name="style">default</xsl:with-param>
                  </xsl:call-template>
               </element>
               <offset>
                  <x>7.02</x>
                  <y>0</y>
               </offset>
            </textPoint>
         </textInstruction>
      </xsl:if>
   </xsl:template>
</xsl:transform>
