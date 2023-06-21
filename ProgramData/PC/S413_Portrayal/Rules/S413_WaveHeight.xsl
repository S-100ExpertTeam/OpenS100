<?xml version="1.0" encoding="utf-8"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
  
  <xsl:template match="WaveHeight[@primitive='Point']" priority="1">
    <xsl:call-template name="WaveHeightFillcolor">
      <xsl:with-param name="viewingGroup">13030</xsl:with-param>
      <xsl:with-param name="displayPlane">UNDERRADAR</xsl:with-param>
      <xsl:with-param name="drawingPriority">10</xsl:with-param>
    </xsl:call-template>
  </xsl:template>
  
  <xsl:template name="WaveHeightFillcolor">
    <xsl:param name="viewingGroup"/>
    <!--  The viewing group  -->
    <xsl:param name="displayPlane"/>
    <!--  The display plane  -->
    <xsl:param name="drawingPriority"/>
    <xsl:variable name="WaveHeight">
      <xsl:value-of select="waveHeight"/>
    </xsl:variable>
    <xsl:variable name="Color">
      <xsl:choose>
        <xsl:when test="$WaveHeight = 5.0">
          <xsl:value-of select="'PLRTE'"/>
        </xsl:when>
        <xsl:when test="$WaveHeight > 4.2">
          <xsl:value-of select="'APLRT'"/>
        </xsl:when>
        <xsl:when test="$WaveHeight > 3.4">
          <xsl:value-of select="'CHYLW'"/>
        </xsl:when>
        <xsl:when test="$WaveHeight > 2.6">
          <xsl:value-of select="'RADLO'"/>
        </xsl:when>
        <xsl:when test="$WaveHeight > 1.8">
          <xsl:value-of select="'RESBL'"/>
        </xsl:when>
        <xsl:when test="$WaveHeight > 1.0">
          <xsl:value-of select="'DEPVS'"/>
        </xsl:when>
        <xsl:when test="$WaveHeight > 0.0">
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

  <!--<xsl:template match="S413_WaveHeight[@primitive='Point']" priority="1">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>26010</viewingGroup>
      <displayPlane>UNDERRADAR</displayPlane>
      <drawingPriority>15</drawingPriority>
      <symbol reference="WAVEHT01">
      </symbol>
    </pointInstruction>
  </xsl:template>

  <xsl:template match="S413_WaveHeight[@primitive='Point' and waveHeight='1']" priority="2">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>26010</viewingGroup>
      <displayPlane>UNDERRADAR</displayPlane>
      <drawingPriority>15</drawingPriority>
      <symbol reference="WAVEHT01">
      </symbol>
    </pointInstruction>
  </xsl:template>

  <xsl:template match="S413_WaveHeight[@primitive='Point' and waveHeight='2']" priority="2">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>26010</viewingGroup>
      <displayPlane>UNDERRADAR</displayPlane>
      <drawingPriority>15</drawingPriority>
      <symbol reference="WAVEHT02">
      </symbol>
    </pointInstruction>
  </xsl:template>

  <xsl:template match="S413_WaveHeight[@primitive='Point' and waveHeight='3']" priority="2">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>26010</viewingGroup>
      <displayPlane>UNDERRADAR</displayPlane>
      <drawingPriority>15</drawingPriority>
      <symbol reference="WAVEHT03">
      </symbol>
    </pointInstruction>
  </xsl:template>-->

</xsl:transform>