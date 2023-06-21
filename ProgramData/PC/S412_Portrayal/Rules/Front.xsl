<?xml version="1.0" encoding="ISO-8859-1"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="UTF-8" indent="yes"/>

  <xsl:template match="Front[@primitive='Curve']" priority="1">
    <lineInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>26010</viewingGroup>
      <displayPlane>UNDERRADAR</displayPlane>
      <drawingPriority>15</drawingPriority>
      <lineStyleReference reference="FRONTS01"/>
    </lineInstruction>
  </xsl:template>

  <xsl:template match="Front[@primitive='Curve' and categoryOfFront = '1']" priority="2">
    <lineInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>26010</viewingGroup>
      <displayPlane>UNDERRADAR</displayPlane>
      <drawingPriority>15</drawingPriority>
      <lineStyleReference reference="FRONTS01"/>
    </lineInstruction>
  </xsl:template>
  <xsl:template match="Front[@primitive='Curve' and categoryOfFront = '2']" priority="2">
    <lineInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>26010</viewingGroup>
      <displayPlane>UNDERRADAR</displayPlane>
      <drawingPriority>15</drawingPriority>
      <lineStyleReference reference="FRONTS04"/>
    </lineInstruction>
  </xsl:template>
  
  <xsl:template match="Front[@primitive='Curve' and categoryOfFront = '5']" priority="2">
    <lineInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>26010</viewingGroup>
      <displayPlane>UNDERRADAR</displayPlane>
      <drawingPriority>15</drawingPriority>
      <lineStyleReference reference="FRONTS02"/>
    </lineInstruction>
  </xsl:template>

  <xsl:template match="Front[@primitive='Curve' and categoryOfFront = '3']" priority="2">
    <lineInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>26010</viewingGroup>
      <displayPlane>UNDERRADAR</displayPlane>
      <drawingPriority>15</drawingPriority>
      <lineStyleReference reference="FRONTS03"/>
    </lineInstruction>
  </xsl:template>

  <xsl:template match="Front[@primitive='Curve' and categoryOfFront = '4']" priority="2">
    <lineInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>26010</viewingGroup>
      <displayPlane>UNDERRADAR</displayPlane>
      <drawingPriority>15</drawingPriority>
      <lineStyleReference reference="FRONTS05"/>
    </lineInstruction>
  </xsl:template>
  
</xsl:transform>


