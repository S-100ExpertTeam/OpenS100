<?xml version="1.0" encoding="ISO-8859-1"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
  <xsl:template match="TropicalCyclone[@primitive='Point']" priority="1">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>26010</viewingGroup>
      <displayPlane>UNDERRADAR</displayPlane>
      <drawingPriority>15</drawingPriority>
      <symbol reference="TROCYC14">
      </symbol>
    </pointInstruction>

  </xsl:template>

  <xsl:template match="TropicalCyclone[@primitive='Point' and windSpeedRange='1']" priority="2">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>26010</viewingGroup>
      <displayPlane>UNDERRADAR</displayPlane>
      <drawingPriority>15</drawingPriority>
      <symbol reference="TROCYC16">
      </symbol>
    </pointInstruction>
  </xsl:template>
  
  <xsl:template match="TropicalCyclone[@primitive='Point' and windSpeedRange='2']" priority="2">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>26010</viewingGroup>
      <displayPlane>UNDERRADAR</displayPlane>
      <drawingPriority>15</drawingPriority>
      <symbol reference="TROCYC17">
      </symbol>
    </pointInstruction>
  </xsl:template>

  <xsl:template match="TropicalCyclone[@primitive='Point' and windSpeedRange='3']" priority="2">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>26010</viewingGroup>
      <displayPlane>UNDERRADAR</displayPlane>
      <drawingPriority>15</drawingPriority>
      <symbol reference="TROCYC15">
      </symbol>
    </pointInstruction>

  </xsl:template>

</xsl:transform>


