<?xml version="1.0" encoding="UTF-8"?>
<xsl:transform version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:output indent="yes" encoding="UTF-8" method="xml"/>
  <xsl:template name="Temperature3hrFillcolor">
    <xsl:param name="viewingGroup"/>
    <!--  The viewing group  -->
    <xsl:param name="displayPlane"/>
    <!--  The display plane  -->
    <xsl:param name="drawingPriority"/>
    <xsl:variable name="Temperature3hr">
      <xsl:value-of select="temperature"/>
    </xsl:variable>
    <xsl:variable name="Color">
      <xsl:choose>
        <xsl:when test="$Temperature3hr > 40">
          <xsl:value-of select="'PLRTE'"/>
        </xsl:when>
        <xsl:when test="$Temperature3hr > 32">
          <xsl:value-of select="'APLRT'"/>
        </xsl:when>
        <xsl:when test="$Temperature3hr > 24">
          <xsl:value-of select="'CHYLW'"/>
        </xsl:when>
        <xsl:when test="$Temperature3hr > 16">
          <xsl:value-of select="'RADLO'"/>
        </xsl:when>
        <xsl:when test="$Temperature3hr > 8">
          <xsl:value-of select="'RESBL'"/>
        </xsl:when>
        <xsl:when test="$Temperature3hr > 0">
          <xsl:value-of select="'DEPVS'"/>
        </xsl:when>
        <xsl:when test="$Temperature3hr > -10">
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

</xsl:transform>