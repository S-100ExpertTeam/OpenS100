<?xml version="1.0" encoding="UTF-8"?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
<xsl:output method="xml" encoding="UTF-8" indent="yes"/>
  <xsl:template match="IndeterminateZone[@primitive='Surface']" priority="1">
    <lineInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>21010</viewingGroup>
      <displayPlane>underRadar</displayPlane>
      <drawingPriority>15</drawingPriority>
      <xsl:call-template name="simpleLineStyle">
        <xsl:with-param name="style">dash</xsl:with-param>
        <xsl:with-param name="width">0.32</xsl:with-param>
        <xsl:with-param name="colour">UINFM</xsl:with-param>
      </xsl:call-template>
    </lineInstruction>
    <areaInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>12010</viewingGroup>
      <displayPlane>UNDERRADAR</displayPlane>
      <drawingPriority>9</drawingPriority>
      <colorFill>
        <color>
          <token>CURSR</token>
          <transparency>0.75</transparency>
        </color>
      </colorFill>
    </areaInstruction>
  </xsl:template>
</xsl:transform>