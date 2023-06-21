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

  <!--Include templates/rules for new S-124 features-->
  <xsl:include href="S413_AvgWaveDir.xsl"/>
  <xsl:include href="S413_AvgWavePrd.xsl"/>
  <xsl:include href="S413_HumidityFillcolor.xsl"/>
  <xsl:include href="S413_Humidity.xsl"/>
  <xsl:include href="S413_MaxWaveDir.xsl"/>
  <xsl:include href="S413_MaxWpkPrd.xsl"/>
  <xsl:include href="S413_NewSnowAmount3hr.xsl"/>
  <xsl:include href="S413_OceanCurrentDirSpd.xsl"/>
  <xsl:include href="S413_Pressure.xsl"/>
  <xsl:include href="S413_RainAmount3hr.xsl"/>
  <xsl:include href="S413_RainProbability.xsl"/>
  <xsl:include href="S413_Temperature3hrFillcolor.xsl"/>
  <xsl:include href="S413_Temperature3hr.xsl"/>
  <xsl:include href="S413_Visibility.xsl"/>
  <xsl:include href="S413_WaterSaltFillcolor.xsl"/>
  <xsl:include href="S413_WaterSalt.xsl"/>
  <xsl:include href="S413_WaterTemperatureFillcolor.xsl"/>
  <xsl:include href="S413_WaterTemperature.xsl"/>
  <xsl:include href="S413_WaveHeightInst.xsl"/>
  <xsl:include href="S413_WaveHeight.xsl"/>
  <xsl:include href="S413_WaveLength.xsl"/>
  <xsl:include href="S413_WaveWindDirSpd.xsl"/>
  <xsl:include href="S413_WindDirSpd.xsl"/>
  <xsl:include href="S413_SkyStatus.xsl"/>

  <!--Include templates/rules for: Default-->
  <xsl:include href="_Default_COMMON.xsl"/>

  <!--Include templates/rules for: Features -->

  <!--Include templates/rules for: Meta Features -->
  
  <xsl:template match="/">
    <displayList>
      <xsl:apply-templates select="Dataset/Features/*"/>
    </displayList>
  </xsl:template>
</xsl:transform>

