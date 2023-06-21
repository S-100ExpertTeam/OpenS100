<?xml version="1.0" encoding="UTF-8"?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
  <xsl:decimal-format name="dformat" decimal-separator="." grouping-separator=","/>

  <!--Include templates/rules for: attributes-->
  <xsl:include href="attributeRules.xsl"/>
  <!--Include templates/rules for: styles-->
  <xsl:include href="textStyle.xsl"/>
  <xsl:include href="simpleLineStyle.xsl"/>
  <!--Include templates/rules for: ContiguousZone-->
  <xsl:include href="ContiguousZone.xsl"/>
  <!--Include templates/rules for: ContinentalShelf-->
  <xsl:include href="ContinentalShelf.xsl"/>
  <!--Include templates/rules for: ExclusiveEconomicZone-->
  <xsl:include href="ExclusiveEconomicZone.xsl"/>
  <!--Include templates/rules for: InternalWater-->
  <xsl:include href="InternalWater.xsl"/>
  <!--Include templates/rules for: InternationalBoundary-->
  <xsl:include href="InternationalBoundary.xsl"/>
  <!--Include templates/rules for: JointManagementZone-->
  <xsl:include href="JointManagementZone.xsl"/>
  <!--Include templates/rules for: NormalBaseline-->
  <xsl:include href="NormalBaseline.xsl"/>
  <!--Include templates/rules for: StraightBaseline-->
  <xsl:include href="StraightBaseline.xsl"/>
  <!--Include templates/rules for: TerritorialSea-->
  <xsl:include href="TerritorialSea.xsl"/>
  <!--Include templates/rules for: Default-->
  <xsl:include href="_Default_COMMON.xsl"/>
  
  <xsl:template match="/">
    <displayList>
      <xsl:apply-templates select="Dataset/Features/*"/>
    </displayList>
  </xsl:template>
</xsl:transform>

