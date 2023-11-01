<?xml version="1.0" encoding="iso-8859-1"?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
  <xsl:template match="*[@primitive='Surface']">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>21010</viewingGroup>
      <displayPlane>UNDERRADAR</displayPlane>
      <drawingPriority>15</drawingPriority>
      <symbol reference="QUESMRK1">
        <areaPlacement placementMode="VisibleParts"/>
      </symbol>
    </pointInstruction>
    <lineInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>21010</viewingGroup>
      <displayPlane>UNDERRADAR</displayPlane>
      <drawingPriority>15</drawingPriority>
      <xsl:call-template name="simpleLineStyle">
        <xsl:with-param name="style">dash</xsl:with-param>
        <xsl:with-param name="width">0.32</xsl:with-param>
        <xsl:with-param name="colour">CHMGD</xsl:with-param>
      </xsl:call-template>
    </lineInstruction>
  </xsl:template>
  <xsl:template match="*[@primitive='Curve']">
    <lineInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>21010</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>15</drawingPriority>
      <lineStyleReference reference="QUESMRK1"/>
    </lineInstruction>
    <lineInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>21010</viewingGroup>
      <displayPlane>UNDERRADAR</displayPlane>
      <drawingPriority>15</drawingPriority>
      <xsl:call-template name="simpleLineStyle">
        <xsl:with-param name="style">dash</xsl:with-param>
        <xsl:with-param name="width">0.32</xsl:with-param>
        <xsl:with-param name="colour">CHMGD</xsl:with-param>
      </xsl:call-template>
    </lineInstruction>
  </xsl:template>
  <xsl:template match="*[@primitive='Point']">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>21010</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>15</drawingPriority>
      <symbol reference="QUESMRK1"/>
    </pointInstruction>
  </xsl:template>
</xsl:transform>
