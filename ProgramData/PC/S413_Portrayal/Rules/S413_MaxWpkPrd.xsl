<?xml version="1.0" encoding="utf-8"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="UTF-8" indent="yes"/>

  <xsl:template match="MaxWpkPrd[@primitive='Point']" priority="1">
    <xsl:call-template name="select_MaxWpkPrd">
      <xsl:with-param name="viewingGroup">13030</xsl:with-param>
      <xsl:with-param name="displayPlane">UNDERRADAR</xsl:with-param>
      <xsl:with-param name="drawingPriority">10</xsl:with-param>
    </xsl:call-template>
  </xsl:template>
  
  <xsl:template name="select_MaxWpkPrd">
    <xsl:param name="viewingGroup"/>
    <!--  The viewing group  -->
    <xsl:param name="displayPlane"/>
    <!--  The display plane  -->
    <xsl:param name="drawingPriority"/>
    <xsl:variable name="MaxWpkPrd">
      <xsl:value-of select="maxWpkPrd"/>
    </xsl:variable>
    <xsl:variable name="Color">
      <xsl:choose>
        <xsl:when test="$MaxWpkPrd = 15.0">
          <xsl:value-of select="'PLRTE'"/>
        </xsl:when>
        <xsl:when test="$MaxWpkPrd > 12.6">
          <xsl:value-of select="'APLRT'"/>
        </xsl:when>
        <xsl:when test="$MaxWpkPrd > 10.2">
          <xsl:value-of select="'CHYLW'"/>
        </xsl:when>
        <xsl:when test="$MaxWpkPrd > 7.8">
          <xsl:value-of select="'RADLO'"/>
        </xsl:when>
        <xsl:when test="$MaxWpkPrd > 5.4">
          <xsl:value-of select="'RESBL'"/>
        </xsl:when>
        <xsl:when test="$MaxWpkPrd > 3.0">
          <xsl:value-of select="'DEPVS'"/>
        </xsl:when>
        <xsl:when test="$MaxWpkPrd > 1.0">
          <xsl:value-of select="'CHMGD'"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="'CHWHT'"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <areaInstruction>
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
      <colorFill>
        <color>
          <xsl:value-of select="$Color"/>
        </color>
      </colorFill>
    </areaInstruction>
  </xsl:template>
  
  <!--<xsl:template match="S413_MaxWpkPrd[@primitive='Point']" priority="1">
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


