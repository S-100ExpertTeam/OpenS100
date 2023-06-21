<?xml version="1.0" encoding="UTF-8"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
  <xsl:template match="RouteWaypoint[@primitive='Point']" priority="1">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="WAYPNT01"/>
    </pointInstruction>
  </xsl:template>
</xsl:transform>