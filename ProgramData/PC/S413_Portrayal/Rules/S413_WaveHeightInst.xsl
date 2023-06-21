<?xml version="1.0" encoding="utf-8"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
  <xsl:template match="WaveHeightInst[@primitive='Point']" priority="1">
    <textInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>21</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>27</drawingPriority>
      
      
      <textPoint horizontalAlignment="Center" verticalAlignment="Center">
        <element>
          <text>
            <xsl:apply-templates select="featureName" mode="text">
              <xsl:with-param name="prefix">by </xsl:with-param>
            </xsl:apply-templates>
          </text>
          <xsl:call-template name="textStyle">
            <xsl:with-param name="style">default</xsl:with-param>
            <xsl:with-param name="bodySize">10</xsl:with-param>
            <xsl:with-param name="foreground">CHBLK</xsl:with-param>
          </xsl:call-template>
        </element>
        <offset>
          <x>-3.51</x>
          <y>3.51</y>
        </offset>
      </textPoint>
    </textInstruction>

  </xsl:template>

</xsl:transform>


