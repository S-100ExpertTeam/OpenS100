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
  <xsl:include href="Obstruction_custom.xsl"/>
  <xsl:include href="UnderWaterAwashRock_custom.xsl"/>
  <xsl:include href="LightDescription.xsl"/>
  <xsl:include href="Lights.xsl"/>
  <xsl:include href="topmarks.xsl"/>
  <xsl:include href="seabed01.xsl"/>
  <xsl:include href="natsur01.xsl"/>
  <xsl:include href="rescsp03.xsl"/>
  <xsl:include href="slcons04.xsl"/>
  <xsl:include href="depare04.xsl"/>
  <xsl:include href="Depcnt03.xsl"/>
  <xsl:include href="qualin03.xsl"/>
  <xsl:include href="quapnt02.xsl"/>
  <xsl:include href="safcon02.xsl"/>
  <xsl:include href="soundg02.xsl"/>
  <xsl:include href="sndfrm04.xsl"/>
  <xsl:include href="Wreck_custom.xsl"/>

  <xsl:include href="LightSectored_custom.xsl"/>
  <xsl:include href="Landmark_COMMON.xsl"/>
  <xsl:include href="NavigationalSystemOfMarks_COMMON.xsl"/>
  <xsl:include href="DataCoverage_COMMON.xsl"/>
  <xsl:include href="QualityOfNonBathymetricData_COMMON.xsl"/>
  <xsl:include href="SoundingDatum_COMMON.xsl"/>
  <xsl:include href="Topmark_COMMON.xsl"/>
  <xsl:include href="LightHouse_COMMON.xsl"/>
  <xsl:include href="BeaconCardinal_PAPER_CHART.xsl"/>
  <xsl:include href="BeaconIsolatedDanger_PAPER_CHART.xsl"/>
  <xsl:include href="BeaconLateral_PAPER_CHART.xsl"/>
  <xsl:include href="BeaconSafeWater_PAPER_CHART.xsl"/>
  <xsl:include href="BeaconSpecialPurposeGeneral_PAPER_CHART.xsl"/>
  <xsl:include href="BuoyCardinal_PAPER_CHART.xsl"/>
  <xsl:include href="BuoyInstallation_PAPER_CHART.xsl"/>
  <xsl:include href="BuoyIsolatedDanger_PAPER_CHART.xsl"/>
  <xsl:include href="BuoyLateral_PAPER_CHART.xsl"/>
  <xsl:include href="BuoySafeWater_PAPER_CHART.xsl"/>
  <xsl:include href="BuoySpecialPurposeGeneral_PAPER_CHART.xsl"/>
  <xsl:include href="DeepWaterRouteCentreline_COMMON.xsl"/>
  <xsl:include href="DeepWaterRoutePart_PLAIN_BOUNDARIES.xsl"/>
  <xsl:include href="DeepWaterRoutePart_SYMBOLIZED_BOUNDARIES.xsl"/>
  <xsl:include href="FloatingDock_COMMON.xsl"/>
  <xsl:include href="FogSignal_COMMON.xsl"/>
  <xsl:include href="Landmark_PAPER_CHART.xsl"/>
  <xsl:include href="LightFloat_PAPER_CHART.xsl"/>
  <xsl:include href="LightSectored_custom.xsl"/>
  <xsl:include href="LightVessel_PAPER_CHART.xsl"/>
  <xsl:include href="OffshorePlatform_COMMON.xsl"/>
  <xsl:include href="OffshoreProductionArea_PLAIN_BOUNDARIES.xsl"/>
  <xsl:include href="OffshoreProductionArea_SYMBOLIZED_BOUNDARIES.xsl"/>
  <xsl:include href="Pile_COMMON.xsl"/>
  <xsl:include href="RadarLine_COMMON.xsl"/>
  <xsl:include href="RadarRange_COMMON.xsl"/>
  <xsl:include href="RadarReflector_COMMON.xsl"/>
  <xsl:include href="RadarStation_COMMON.xsl"/>
  <xsl:include href="RadarTransponderBeacon_COMMON.xsl"/>
  <xsl:include href="RetroReflector_PAPER_CHART.xsl"/>
  <xsl:include href="SeaAreaNamedWaterArea_COMMON.xsl"/>
  <xsl:include href="SignalStationTraffic_COMMON.xsl"/>
  <xsl:include href="SignalStationWarning_COMMON.xsl"/>
  <xsl:include href="SiloTank_COMMON.xsl"/>
  <xsl:include href="UnderWaterAwashRock_custom.xsl"/>
  <xsl:include href="Waterfall_COMMON.xsl"/>
  <xsl:include href="WaterTurbulence_COMMON.xsl"/>
  <xsl:include href="Topmark_COMMON.xsl"/>


  <xsl:param name="TWO_SHADES">true</xsl:param>
  <xsl:param name="SAFETY_DEPTH" select="30.0"/>
  <xsl:param name="SAFETY_CONTOUR" select="30.0"/>
  <xsl:param name="SHALLOW_CONTOUR" select="2.0"/>
  <xsl:param name="DEEP_CONTOUR" select="30.0"/>
  <xsl:param name="SHALLOW_PATTERN">false</xsl:param>
  <xsl:param name="SHOW_ISOLATED_DANGERS_IN_SHALLOW_WATERS">false</xsl:param>
  <xsl:param name="FULL_SECTORS">false</xsl:param>

  <xsl:template match="/">
    <displayList>
      <xsl:apply-templates select="Dataset/Features/*"/>
    </displayList>
  </xsl:template>
</xsl:transform>

