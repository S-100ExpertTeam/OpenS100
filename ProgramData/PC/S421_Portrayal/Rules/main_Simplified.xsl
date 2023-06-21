<?xml version="1.0" encoding="UTF-8"?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
  <xsl:decimal-format name="dformat" decimal-separator="." grouping-separator=","/>

  <!--Include templates/rules for: attributes-->
  <xsl:include href="attributeRules.xsl"/>
  <xsl:include href="simpleLineStyle.xsl"/>

  <!--Include templates/rules for: RouteLeg -->
  <!--Control value of Parameters for Text and Font in the below file -->
  <xsl:include href="RouteWaypointLeg.xsl"/>

  <!--Include templates/rules for: Waypoint-->
  <xsl:include href="RouteWaypoint.xsl"/>

  <!--Include templates/rules for: RouteActionPoint -->
  <!--Control value of Parameters for Text,Font, AreaFillGroup of Surface in the below file -->
  <xsl:include href="RouteActionPoint.xsl"/>

  <!--Include templates/rules for: Default-->
  <xsl:include href="Default.xsl"/>

  <xsl:template match="/">
    <displayList xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xsi:noNamespaceSchemaLocation="S100Presentation.xsd">
      <xsl:apply-templates select="Dataset/Features/*"/>
    </displayList>
  </xsl:template>
</xsl:transform>

