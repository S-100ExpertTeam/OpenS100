<?xml version="1.0" encoding="utf-8"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
  
    <xsl:template match="SkyStatus[@primitive='Point']" priority="1">
    <xsl:call-template name="select_SkyStatus">
      <xsl:with-param name="viewingGroup">13030</xsl:with-param>
      <xsl:with-param name="displayPlane">UNDERRADAR</xsl:with-param>
      <xsl:with-param name="drawingPriority">10</xsl:with-param>
    </xsl:call-template>
  </xsl:template>
  
  <xsl:template name="select_SkyStatus">
    <xsl:param name="viewingGroup"/>
    <!--  The viewing group  -->
    <xsl:param name="displayPlane"/>
    <!--  The display plane  -->
    <xsl:param name="drawingPriority"/>
    <xsl:variable name="SkyStatus">
       <xsl:value-of select="skyStatus"/>
    </xsl:variable>
    <xsl:choose>
      <xsl:when test="1 >= $SkyStatus">
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
          <!--<symbol reference="SCAROW01" rotation="$surfaceCurrentDirection"/>-->
          <xsl:element name="symbol">
            <xsl:attribute name="reference">SUNNYW01</xsl:attribute>
          </xsl:element>
        </pointInstruction>
      </xsl:when>
      <xsl:when test="2 >= $SkyStatus">
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
            <xsl:attribute name="reference">LOWCLD01</xsl:attribute>
          </xsl:element>
        </pointInstruction>
      </xsl:when>
      <xsl:when test="3 >= $SkyStatus">
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
            <xsl:attribute name="reference">HIGCLD01</xsl:attribute>
          </xsl:element>
        </pointInstruction>
      </xsl:when>
      <xsl:otherwise>
        <pointInstruction>
          <featureReference>
            <xsl:value-of select="@id"/>
          </featureReference>
          <viewingGroup>
          </viewingGroup>
          <xsl:value-of select="$viewingGroup"/>
          <displayPlane>
            <xsl:value-of select="$displayPlane"/>
          </displayPlane>
          <drawingPriority>
            <xsl:value-of select="$drawingPriority"/>
          </drawingPriority>
          <xsl:element name="symbol">
            <xsl:attribute name="reference">CLOUDY01</xsl:attribute>
          </xsl:element>
        </pointInstruction>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>
</xsl:transform>


