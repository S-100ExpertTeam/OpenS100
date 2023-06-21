<?xml version="1.0" encoding="ISO-8859-1"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
  <xsl:template match="HighWindMessage[@primitive='Surface']" priority="1">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>26010</viewingGroup>
      <displayPlane>UNDERRADAR</displayPlane>
      <drawingPriority>15</drawingPriority>
      <symbol reference="QUESMRK1">
      </symbol>
    </pointInstruction>

    <lineInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>26010</viewingGroup>
      <displayPlane>UNDERRADAR</displayPlane>
      <drawingPriority>15</drawingPriority>
      <lineStyleReference reference="ESSARE01"/>
    </lineInstruction>

    <areaInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>53050</viewingGroup>
      <displayPlane>UNDERRADAR</displayPlane>
      <drawingPriority>14</drawingPriority>
      <colorFill>
        <color transparency="0.75">ADINF</color>
      </colorFill>
    </areaInstruction>
    
  </xsl:template>

</xsl:transform>


