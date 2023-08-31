<?xml version="1.0" encoding="UTF-8"?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
	<xsl:output method="xml" encoding="UTF-8" indent="yes"/>
	<xsl:decimal-format name="dformat" decimal-separator="." grouping-separator=","/>
	<!--Include templates/rules for: styles-->
	<!--Include templates/rules for: Default-->
	<xsl:include href="default.xsl"/>
	<!--Include templates/rules for: IceCompacting-->
	<xsl:include href="IceCompacting.xsl"/>
	<!--Include templates/rules for: IceLead-->
	<xsl:include href="IceLead.xsl"/>
	<!--Include templates/rules for: Floeberg-->
	<xsl:include href="Floeberg.xsl"/>
	<!--Include templates/rules for: IceShear-->
	<xsl:include href="IceShear.xsl"/>
	<!--Include templates/rules for: IceDivergence-->
	<xsl:include href="IceDivergence.xsl"/>
	<!--Include templates/rules for: IceRidgeHummock-->
	<xsl:include href="IceRidgeHummock.xsl"/>
	<!--Include templates/rules for: IceFracture-->
	<xsl:include href="IceFracture.xsl"/>
	<!--Include templates/rules for: IceFracture-->
	<xsl:include href="seaice.xsl"/>
  <!--Include templates/rules for: attributeRules-->
  <xsl:include href="attributeRules.xsl"/>
  <!--Include templates/rules for: simpleLineStyle-->
  <xsl:include href="simpleLineStyle.xsl"/>
  <!--Include templates/rules for: textStyle-->
  <xsl:include href="textStyle.xsl"/>
	<xsl:template match="/">
		<displayList>
			<xsl:apply-templates select="Dataset/Features/*"/>
		</displayList>
	</xsl:template>
</xsl:transform>
