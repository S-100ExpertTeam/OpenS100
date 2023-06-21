<?xml version="1.0" encoding="UTF-8"?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
  <xsl:decimal-format name="dformat" decimal-separator="." grouping-separator=","/>

  <!--Include templates/rules for: attributes-->
  <xsl:include href="attributeRules.xsl"/>

  <!--Include templates/rules for: styles-->
  <xsl:include href="textStyle.xsl"/>
  <xsl:include href="simpleLineStyle.xsl"/>

  <!--Include templates/rules for: csps-->

  <!--Include templates/rules for new S-412 features-->

  <!--Include templates/rules for: Default-->
  <xsl:include href="_Default_COMMON.xsl"/>

  <!--Include templates/rules for: MarineProtectedArea-->
  <xsl:include href="Building_COMMON.xsl"/>
  <xsl:include href="CoastguardStation.xsl"/>
  <xsl:include href="DataCoverage_COMMON.xsl"/>
  <xsl:include href="ForecastAreaAggregate.xsl"/>
  <xsl:include href="FuzzyAreaAggregate.xsl"/>
  <xsl:include href="GMDSSArea.xsl"/>
  <xsl:include href="IndeterminateZone.xsl"/>
  <xsl:include href="InmarsatOceanRegionArea.xsl"/>
  <xsl:include href="Landmark_COMMON.xsl"/>
  <xsl:include href="NavigationalMeterorologicalArea.xsl"/>
  <xsl:include href="NavtexStationArea.xsl"/>
  <xsl:include href="QualityOfNonBathymetricData.xsl"/>
  <xsl:include href="RadioServiceArea.xsl"/>
  <xsl:include href="RadioStation.xsl"/>
  <xsl:include href="TextPlacement.xsl"/>
  <xsl:include href="WeatherForecastWarningArea.xsl"/>
  
  <xsl:template match="/">
    <displayList>
      <xsl:apply-templates select="Dataset/Features/*"/>
    </displayList>
  </xsl:template>
</xsl:transform>

