<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
  <xsl:template match="UnderKeelClearancePlanArea[@primitive='Surface']" priority="1">
    <xsl:choose>
      <xsl:when test="$PlainBoundaries != 'true'">
        <lineInstruction>
          <featureReference>
            <xsl:value-of select="@id"/>
          </featureReference>
          <viewingGroup>29010</viewingGroup>
          <displayPlane>UnderRadar</displayPlane>
          <drawingPriority>6</drawingPriority>
          <lineStyleReference reference="UKCARE01"/>
        </lineInstruction>
      </xsl:when>  
      <xsl:otherwise>
        <lineInstruction>
          <featureReference>
            <xsl:value-of select="@id"/>
          </featureReference>
          <viewingGroup>29010</viewingGroup>
          <displayPlane>UnderRadar</displayPlane>
          <drawingPriority>6</drawingPriority>
          <lineStyle>
            <capStyle/>
            <joinStyle/>
            <intervalLength>3.18</intervalLength>
            <offset/>
            <pen width="0.32">
              <color>CHMGD</color>
            </pen>
            <dash>
              <start>0.0</start>
              <length>1.76</length>
            </dash>
          </lineStyle>
        </lineInstruction>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>
</xsl:transform>
