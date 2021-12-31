<?xml version="1.0" encoding="UTF-8"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
   <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
   <xsl:template match="CurrentNonGravitational[@primitive='Curve']" priority="1">
      <lineInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>33060</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>18</drawingPriority>
         <lineStyleReference reference="CURENT01"/>
      </lineInstruction>
   </xsl:template>
   <xsl:template match="CurrentNonGravitational[@primitive='Point']" priority="1">
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>33060</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>15</drawingPriority>
         <symbol reference="CURDEF01"/>
      </pointInstruction>
   </xsl:template>
   <xsl:template match="CurrentNonGravitational[@primitive='Point' and orientation/orientationValue!='']" priority="2">
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>33060</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>15</drawingPriority>
         <symbol reference="CURENT01" rotation="{orientation/orientationValue}"/>
      </pointInstruction>
   </xsl:template>
   <xsl:template match="CurrentNonGravitational[@primitive='Point' and (orientation/orientationValue!='' and currentVelocity/velocityMaximum!='')]" priority="3">
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>33060</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>15</drawingPriority>
         <symbol reference="CURENT01" rotation="{orientation/orientationValue}"/>
      </pointInstruction>
      <xsl:if test="currentVelocity/velocityMaximum!= ''">
         <textInstruction>
            <featureReference>
               <xsl:value-of select="@id"/>
            </featureReference>
            <viewingGroup>31</viewingGroup>
            <displayPlane>OVERRADAR</displayPlane>
            <drawingPriority>15</drawingPriority>
            <textPoint>
               <element>
                  <text>
                     <xsl:apply-templates select="currentVelocity/velocityMaximum" mode="text"/>
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
