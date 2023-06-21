<?xml version="1.0" encoding="ISO-8859-1"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
  <xsl:template match="UesrDefineFeatureText[@primitive='Point']" priority="1">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>12210</viewingGroup>
      <displayPlane>UNDERRADAR</displayPlane>
      <drawingPriority>15</drawingPriority>
      <symbol reference="USRPNT01">
      </symbol>
    </pointInstruction>
    <xsl:if test="text!= ''">
      <textInstruction>
        <featureReference>
          <xsl:value-of select="@id"/>
        </featureReference>
        <viewingGroup>26</viewingGroup>
        <displayPlane>UNDERRADAR</displayPlane>
        <drawingPriority>12</drawingPriority>
        <textPoint horizontalAlignment="Right" verticalAlignment="Center">
          <element>
            <text>
              <xsl:apply-templates select="text" mode="text"/>
            </text>
            <xsl:call-template name="textStyle">
              <xsl:with-param name="style">default</xsl:with-param>
            </xsl:call-template>
          </element>
          <offset>
            <x>2</x>
            <y>0</y>
          </offset>
          <areaPlacement placementMode="VisibleParts"/>
        </textPoint>
      </textInstruction>
    </xsl:if>
  </xsl:template>
  <xsl:template match="UesrDefineFeaturePoint[@primitive='Point']" priority="1">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>12210</viewingGroup>
      <displayPlane>UNDERRADAR</displayPlane>
      <drawingPriority>15</drawingPriority>
      <symbol reference="USRPNT01">
      </symbol>
    </pointInstruction>
  </xsl:template>
  <xsl:template match="UesrDefineFeatureLine[@primitive='Curve']" priority="1">
    <lineInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>12210</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>27</drawingPriority>
      <lineStyleReference reference="UserDefineFeatureLine"/>
    </lineInstruction>
  </xsl:template>
  <xsl:template match="UesrDefineFeatureArea[@primitive='Surface']" priority="1">
    <lineInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>12210</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>27</drawingPriority>
      <lineStyleReference reference="UserDefineFeatureLine"/>
    </lineInstruction>
    <areaInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>12210</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>27</drawingPriority>
      <colorFill>
        <color transparency="0.80">00011</color>
      </colorFill>
    </areaInstruction>
  </xsl:template>

</xsl:transform>
