<?xml version="1.0" encoding="UTF-8"?>
<xsl:transform version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:output indent="yes" encoding="UTF-8" method="xml"/>
  <xsl:template name="HumidityFillcolor">
    <xsl:variable name="Humidity">
      <xsl:value-of select="humidity"/>
    </xsl:variable>
    <xsl:variable name="Color">
      <xsl:choose>
        <xsl:when test="$Humidity = 100">
          <xsl:value-of select="'PLRTE'"/>
        </xsl:when>
        <xsl:when test="$Humidity > 95">
          <xsl:value-of select="'APLRT'"/>
        </xsl:when>
        <xsl:when test="$Humidity > 90">
          <xsl:value-of select="'CHYLW'"/>
        </xsl:when>
        <xsl:when test="$Humidity > 85">
          <xsl:value-of select="'RADLO'"/>
        </xsl:when>
        <xsl:when test="$Humidity > 80">
          <xsl:value-of select="'RESBL'"/>
        </xsl:when>
        <xsl:when test="$Humidity > 75">
          <xsl:value-of select="'DEPVS'"/>
        </xsl:when>
        <xsl:when test="$Humidity > 70">
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
      <viewingGroup>53050</viewingGroup>
      <displayPlane>UNDERRADAR</displayPlane>
      <drawingPriority>24</drawingPriority>
      <colorFill>
        <color>
          <xsl:value-of select="$Color"/>
        </color>
      </colorFill>
    </areaInstruction>
  </xsl:template>

</xsl:transform>