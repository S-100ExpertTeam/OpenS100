<?xml version="1.0" encoding="UTF-8"?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="UTF-8" indent="yes"/>

  <xsl:template match="TextPlacement[@primitive='Point']" priority="1">
    <xsl:if test="text!= ''">
      <textInstruction>
        <featureReference>
          <xsl:value-of select="@id"/>
        </featureReference>
        <viewingGroup>26</viewingGroup>
        <displayPlane>UNDERRADAR</displayPlane>
        <drawingPriority>12</drawingPriority>
        <textPoint horizontalAlignment="Right" verticalAlignment="Center">
          <element>
            <text>
              <xsl:apply-templates select="text" mode="text"/>
            </text>
            <xsl:call-template name="textStyle">
              <xsl:with-param name="style">default</xsl:with-param>
            </xsl:call-template>
            <font>
              <slant>
                <xsl:value-of select="flipBearing"/>
              </slant>
            </font>
          </element>
          <offset>
            <x>2</x>
            <y>0</y>
          </offset>
          <areaPlacement placementMode="VisibleParts"/>
        </textPoint>
      </textInstruction>
    </xsl:if>
  </xsl:template>
</xsl:transform>