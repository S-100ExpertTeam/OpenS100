<?xml version="1.0" encoding="ISO-8859-1"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
  <xsl:template match="NavwarnAreaAffected[@primitive='Point']" priority="1">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>31020</viewingGroup>
      <displayPlane>UNDERRADAR</displayPlane>
      <drawingPriority>15</drawingPriority>
      <symbol reference="NavigationalWarningFeaturePart">
      </symbol>
    </pointInstruction>
  </xsl:template>

  <xsl:template match="NavwarnAreaAffected[@primitive='Surface']" priority="1">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>31020</viewingGroup>
      <displayPlane>UNDERRADAR</displayPlane>
      <drawingPriority>15</drawingPriority>
      <symbol reference="NavigationalWarningFeaturePart">
          <areaPlacement placementMode="VisibleParts"/>
      </symbol>
    </pointInstruction>
  </xsl:template>

</xsl:transform>
