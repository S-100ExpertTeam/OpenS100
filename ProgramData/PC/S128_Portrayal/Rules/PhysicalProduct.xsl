<?xml version="1.0" encoding="UTF-8"?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
<xsl:output method="xml" encoding="UTF-8" indent="yes"/>
   <xsl:template match="PhysicalProduct[@primitive='Surface']" priority="1">
    <lineInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>31000</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>15</drawingPriority>
      <lineStyle>
        <capStyle/>
        <joinStyle/>
        <intervalLength>8</intervalLength>
        <offset/>
        <pen width="0.32">
          <color>CHGRN</color>
        </pen>
        <dash>
          <start>2</start>
          <length>6</length>
        </dash>
      </lineStyle>
    </lineInstruction>
    <areaInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>31000</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>15</drawingPriority>
      <colorFill>
        <color transparency="0.30">CHGRN</color>
      </colorFill>
    </areaInstruction>
  </xsl:template>
</xsl:transform>