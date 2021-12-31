<?xml version="1.0" encoding="UTF-8"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
   <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
   <xsl:template match="RadarStation[@primitive='Point']" priority="1">
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>38010</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>15</drawingPriority>
         <symbol reference="POSGEN01"/>
      </pointInstruction>
   </xsl:template>
   <xsl:template match="RadarStation[@primitive='Point' and categoryOfRadarStation=2]" priority="2">
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>38010</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>15</drawingPriority>
         <symbol reference="RDOSTA02"/>
      </pointInstruction>
      <xsl:if test="communicationInformation/communicationChannel!= ''">
         <textInstruction>
            <featureReference>
               <xsl:value-of select="@id"/>
            </featureReference>
            <viewingGroup>11</viewingGroup>
            <displayPlane>OVERRADAR</displayPlane>
            <drawingPriority>15</drawingPriority>
            <textPoint>
               <element>
                  <text>
                     <xsl:apply-templates select="communicationInformation/communicationChannel" mode="text">
                        <xsl:with-param name="prefix">ch </xsl:with-param>
                     </xsl:apply-templates>
                  </text>
                  <xsl:call-template name="textStyle">
                     <xsl:with-param name="style">default</xsl:with-param>
                  </xsl:call-template>
               </element>
               <offset>
                  <x>0</x>
                  <y>0</y>
               </offset>
            </textPoint>
         </textInstruction>
      </xsl:if>
   </xsl:template>
</xsl:transform>
