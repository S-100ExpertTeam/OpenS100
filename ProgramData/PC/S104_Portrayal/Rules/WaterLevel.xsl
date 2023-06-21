<?xml version="1.0" encoding="UTF-8"?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
  <xsl:template match="WaterLevel[@primitive='Point']" priority="1">
    <xsl:if test="waterLevelHeight!= ''">
      <textInstruction>
        <featureReference>
          <xsl:value-of select="@id"/>
        </featureReference>
        <viewingGroup>28</viewingGroup>
        <displayPlane>OVERRADAR</displayPlane>
        <drawingPriority>12</drawingPriority>
        <textPoint verticalAlignment="Center">
          <element>
            <text>
              <xsl:apply-templates select="waterLevelHeight" mode="text"/>
            </text>
            <xsl:call-template name="textStyle">
              <xsl:with-param name="style">default</xsl:with-param>
            </xsl:call-template>
          </element>
          <offset>
            <x>3.51</x>
            <y>0</y>
          </offset>
        </textPoint>
      </textInstruction>
    </xsl:if>
  </xsl:template>
</xsl:transform>
