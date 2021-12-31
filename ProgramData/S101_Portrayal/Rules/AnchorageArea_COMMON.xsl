<?xml version="1.0" encoding="UTF-8"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
   <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
   <xsl:template match="AnchorageArea[@primitive='Point']" priority="1">
      <xsl:if test="featureName!= ''">
         <textInstruction>
            <featureReference>
               <xsl:value-of select="@id"/>
            </featureReference>
            <viewingGroup>26</viewingGroup>
            <displayPlane>OVERRADAR</displayPlane>
            <drawingPriority>18</drawingPriority>
            <textPoint>
               <element>
                  <text>
                     <xsl:apply-templates select="featureName" mode="text"/>
                  </text>
                  <xsl:call-template name="textStyle">
                     <xsl:with-param name="style">default</xsl:with-param>
                  </xsl:call-template>
               </element>
               <offset>
                  <x>-3.51</x>
                  <y>-7.02</y>
               </offset>
            </textPoint>
         </textInstruction>
      </xsl:if>
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>26220</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>18</drawingPriority>
         <symbol reference="ACHARE02"/>
      </pointInstruction>
   </xsl:template>
</xsl:transform>
