<?xml version="1.0" encoding="utf-8"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="UTF-8" indent="yes"/>

  <xsl:template match="MaxWaveDir[@primitive='Point']" priority="1">
    <xsl:call-template name="select_MaxWaveDir">
      <xsl:with-param name="viewingGroup">13030</xsl:with-param>
      <xsl:with-param name="displayPlane">UNDERRADAR</xsl:with-param>
      <xsl:with-param name="drawingPriority">10</xsl:with-param>
    </xsl:call-template>
  </xsl:template>

  <xsl:template name="select_MaxWaveDir">
    <xsl:param name="viewingGroup"/>
    <!--  The viewing group  -->
    <xsl:param name="displayPlane"/>
    <!--  The display plane  -->
    <xsl:param name="drawingPriority"/>
    <xsl:variable name="Direction">
      <xsl:choose>
        <xsl:when test="maxWaveDir and number(maxWaveDir) = number(maxWaveDir) ">
          <xsl:value-of select="number(maxWaveDir)"/>
        </xsl:when>
        <xsl:otherwise>0</xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
        <pointInstruction>
          <featureReference>
            <xsl:value-of select="@id"/>
          </featureReference>
          <viewingGroup>
            <xsl:value-of select="$viewingGroup"/>
          </viewingGroup>
          <displayPlane>
            <xsl:value-of select="$displayPlane"/>
          </displayPlane>
          <drawingPriority>
            <xsl:value-of select="$drawingPriority"/>
          </drawingPriority>
          <xsl:element name="symbol">
            <xsl:attribute name="reference">SCAROW01</xsl:attribute>
            <xsl:attribute name="rotation">
              <xsl:value-of select="$Direction"/>
            </xsl:attribute>
          </xsl:element>
        </pointInstruction>
  </xsl:template>

  <!--<xsl:template match="S413_MaxWaveDir[@primitive='Point']" priority="1">
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

  </xsl:template>-->

</xsl:transform>


