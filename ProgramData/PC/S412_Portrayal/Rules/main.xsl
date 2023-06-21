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
  <xsl:include href="ConeOfUncertainty.xsl"/>
  <xsl:include href="ConvergentBoundary.xsl"/>
  <xsl:include href="CycloneTrack.xsl"/>
  <xsl:include href="FreezingSprayMessage.xsl"/>
  <xsl:include href="Front.xsl"/>
  <xsl:include href="High.xsl"/>
  <xsl:include href="HighWindMessage.xsl"/>
  <xsl:include href="LargeSeasMessage.xsl"/>
  <xsl:include href="Low.xsl"/>
  <xsl:include href="PrecipitationMessage.xsl"/>
  <xsl:include href="ReducedVisibilityMessage.xsl"/>
  <xsl:include href="Ridge.xsl"/>
  <xsl:include href="Squall.xsl"/>
  <xsl:include href="TemperatureMessage.xsl"/>
  <xsl:include href="Thunderstorm.xsl"/>
  <xsl:include href="ThunderstormMessage.xsl"/>
  <xsl:include href="TropicalCyclone.xsl"/>
  <xsl:include href="TropicalCycloneMessage.xsl"/>

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

