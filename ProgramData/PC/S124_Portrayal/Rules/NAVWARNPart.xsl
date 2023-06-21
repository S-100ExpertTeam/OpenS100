<?xml version="1.0" encoding="ISO-8859-1"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
  <xsl:template match="NAVWARNPart[@primitive='Point']" priority="1">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>12210</viewingGroup>
      <displayPlane>UNDERRADAR</displayPlane>
      <drawingPriority>15</drawingPriority>
      <symbol reference="NavigatoinalWarningFeaturePart">
      </symbol>
    </pointInstruction>
  </xsl:template>
  <xsl:template match="NAVWARNPart[@primitive='Curve']" priority="1">
    <lineInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>12210</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>15</drawingPriority>
      <lineStyleReference reference="NavigatoinalWarningFeaturePart"/>
    </lineInstruction>
  </xsl:template>
  <xsl:template match="NAVWARNPart[@primitive='Surface']" priority="1">
    <pointInstruction>
    <featureReference>
      <xsl:value-of select="@id"/>
    </featureReference>
    <viewingGroup>12210</viewingGroup>
    <displayPlane>OVERRADAR</displayPlane>
    <drawingPriority>15</drawingPriority>
    <symbol reference="NavigatoinalWarningFeaturePart_point"/>
    </pointInstruction>
    <lineInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>12210</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>15</drawingPriority>
      <lineStyleReference reference="NavigatoinalWarningFeaturePart"/>
    </lineInstruction>
    <areaInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>12210</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>15</drawingPriority>
      <colorFill>
        <color transparency="0.80">00011</color>
      </colorFill>
    </areaInstruction>
  </xsl:template>

</xsl:transform>
