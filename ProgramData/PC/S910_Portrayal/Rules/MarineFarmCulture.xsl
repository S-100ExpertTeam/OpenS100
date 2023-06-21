<?xml version="1.0" encoding="UTF-8"?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
  <xsl:template match="MarineFarmCulture[@primitive='Surface']" priority="1">
    <lineInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>12310</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>15</drawingPriority>
      <lineStyle>
        <capStyle/>
        <joinStyle/>
        <offset/>
        <pen width="0.32">
          <color>UINFB</color>
        </pen>
      </lineStyle>
    </lineInstruction>
    <areaInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>12310</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>15</drawingPriority>
      <colorFill>
        <color transparency="0.85">UINFB</color>
      </colorFill>
    </areaInstruction>
  </xsl:template>
  <xsl:template match="MarineFarmCulture[@primitive='Curve']" priority="1">
   <lineInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>12310</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>15</drawingPriority>
      <lineStyle>
        <capStyle/>
        <joinStyle/>
        <offset/>
        <pen width="0.32">
          <color>UINFB</color>
        </pen>
      </lineStyle>
    </lineInstruction>
  </xsl:template>
</xsl:transform>
