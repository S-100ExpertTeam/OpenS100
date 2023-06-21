<?xml version="1.0" encoding="UTF-8"?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
  <xsl:template match="MarineProtectedArea[@primitive='Curve']" priority="1">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>12210</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>15</drawingPriority>
      <symbol reference="MarineProtectedarea_MPA"/>
    </pointInstruction>
    <lineInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>12210</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>15</drawingPriority>
      <lineStyle>
        <capStyle/>
        <joinStyle/>
        <intervalLength>3.18</intervalLength>
        <offset/>
        <pen width="0.64">
          <color>TESTA</color>
        </pen>
        <dash>
          <start>0.0</start>
          <length>1.76</length>
        </dash>
      </lineStyle>
    </lineInstruction>
  </xsl:template>
  <xsl:template match="MarineProtectedArea[@primitive='Surface']" priority="1">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>12210</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>15</drawingPriority>
      <symbol reference="MarineProtectedarea_MPA"/>
    </pointInstruction>
    <lineInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>12210</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>15</drawingPriority>
      <lineStyle>
        <capStyle/>
        <joinStyle/>
        <intervalLength>3.18</intervalLength>
        <offset/>
        <pen width="0.64">
          <color>TESTA</color>
        </pen>
        <dash>
          <start>0.0</start>
          <length>1.76</length>
        </dash>
      </lineStyle>
    </lineInstruction>
    <areaInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>12210</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>15</drawingPriority>
      <colorFill>
        <color transparency="0.80">TESTB</color>
      </colorFill>
    </areaInstruction>
  </xsl:template>
</xsl:transform>
