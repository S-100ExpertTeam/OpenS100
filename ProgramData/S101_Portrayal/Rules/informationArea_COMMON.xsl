<?xml version="1.0" encoding="UTF-8"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
   <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
  <xsl:template match="InformationArea[@primitive='Point']" priority="1">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>12010</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="RTPBCN02"/>
    </pointInstruction>
    <xsl:if test="featureName!= ''">
      <textInstruction>
        <featureReference>
          <xsl:value-of select="@id"/>
        </featureReference>
        <viewingGroup>26</viewingGroup>
        <displayPlane>UNDERRADAR</displayPlane>
        <drawingPriority>9</drawingPriority>
        <textPoint horizontalAlignment="Right" verticalAlignment="Center">
          <element>
            <text>
              <xsl:apply-templates select="featureName" mode="text"/>
            </text>
            <xsl:call-template name="textStyle">
              <xsl:with-param name="style">default</xsl:with-param>
            </xsl:call-template>
          </element>
          <offset>
            <x>5</x>
            <y>0</y>
          </offset>
          <areaPlacement placementMode="VisibleParts"/>
        </textPoint>
      </textInstruction>
    </xsl:if>
  </xsl:template>
   <xsl:template match="InformationArea[@primitive='Surface']" priority="1">
      <nullInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>0</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>0</drawingPriority>
      </nullInstruction>
   </xsl:template>
</xsl:transform>
