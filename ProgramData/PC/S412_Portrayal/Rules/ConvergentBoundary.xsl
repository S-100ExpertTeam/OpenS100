<?xml version="1.0" encoding="ISO-8859-1"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
  
  <xsl:template match="ConvergentBoundary[@primitive='Curve']" priority="1">
    <lineInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>26010</viewingGroup>
      <displayPlane>UNDERRADAR</displayPlane>
      <drawingPriority>15</drawingPriority>
      <lineStyleReference reference="CONVBO01"/>
    </lineInstruction>
  </xsl:template>
  
  <xsl:template match="ConvergentBoundary[@primitive='Curve' and categoryOfConvergentBoundary = '1']" priority="2">
    <lineInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>26010</viewingGroup>
      <displayPlane>UNDERRADAR</displayPlane>
      <drawingPriority>15</drawingPriority>
      <lineStyleReference reference="CONVBO02"/>
    </lineInstruction>
  </xsl:template>

  <xsl:template match="ConvergentBoundary[@primitive='Curve' and categoryOfConvergentBoundary = '2']" priority="2">
    <lineInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>26010</viewingGroup>
      <displayPlane>UNDERRADAR</displayPlane>
      <drawingPriority>15</drawingPriority>
      <lineStyleReference reference="CONVBO03"/>
    </lineInstruction>
  </xsl:template>

  <xsl:template match="ConvergentBoundary[@primitive='Curve' and categoryOfConvergentBoundary = '3']" priority="2">
    <lineInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>26010</viewingGroup>
      <displayPlane>UNDERRADAR</displayPlane>
      <drawingPriority>15</drawingPriority>
      <lineStyleReference reference="CONVBO04"/>
    </lineInstruction>
  </xsl:template>

  <xsl:template match="ConvergentBoundary[@primitive='Curve' and categoryOfConvergentBoundary = '4']" priority="2">
    <lineInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>26010</viewingGroup>
      <displayPlane>UNDERRADAR</displayPlane>
      <drawingPriority>15</drawingPriority>
      <lineStyleReference reference="CONVBO05"/>
    </lineInstruction>
  </xsl:template>

  <xsl:template match="ConvergentBoundary[@primitive='Curve' and categoryOfConvergentBoundary = '5']" priority="2">
    <lineInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>26010</viewingGroup>
      <displayPlane>UNDERRADAR</displayPlane>
      <drawingPriority>15</drawingPriority>
      <lineStyleReference reference="CONVBO06"/>
    </lineInstruction>
  </xsl:template>

  <xsl:template match="ConvergentBoundary[@primitive='Curve' and categoryOfConvergentBoundary = '6']" priority="2">
    <lineInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>26010</viewingGroup>
      <displayPlane>UNDERRADAR</displayPlane>
      <drawingPriority>15</drawingPriority>
      <lineStyleReference reference="CONVBO07"/>
    </lineInstruction>
  </xsl:template>

  <xsl:template match="ConvergentBoundary[@primitive='Curve' and categoryOfConvergentBoundary = '7']" priority="2">
    <lineInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>26010</viewingGroup>
      <displayPlane>UNDERRADAR</displayPlane>
      <drawingPriority>15</drawingPriority>
      <lineStyleReference reference="CONVBO08"/>
    </lineInstruction>
  </xsl:template>
  
</xsl:transform>


