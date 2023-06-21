<?xml version="1.0" encoding="UTF-8"?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
	<xsl:output method="xml" encoding="UTF-8" indent="yes"/>
	<xsl:decimal-format name="dformat" decimal-separator="." grouping-separator=","/>
	<!--Include templates/rules for: attributes-->
	<xsl:include href="attributeRules.xsl"/>
	<!--Include templates/rules for: styles-->
	<xsl:include href="textStyle.xsl"/>
	<xsl:include href="simpleLineStyle.xsl"/>
	<!--Include templates/rules for: Default-->
	<xsl:include href="default.xsl"/>
	<!--Include templates/rules for: CableOverhead-->
	<xsl:include href="DemarcationPoint.xsl"/>
	<xsl:include href="SeaAreaNamedWaterArea.xsl"/>
	<xsl:template match="/">
		<displayList>
			<xsl:apply-templates select="Dataset/Features/*"/>
		</displayList>
	</xsl:template>
</xsl:transform>
