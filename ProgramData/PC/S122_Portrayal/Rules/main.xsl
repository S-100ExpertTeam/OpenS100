<?xml version="1.0" encoding="UTF-8"?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
	<xsl:output method="xml" encoding="UTF-8" indent="yes"/>
	<xsl:decimal-format name="dformat" decimal-separator="." grouping-separator=","/>

  <!--Include templates/rules for: styles-->
  <xsl:include href="simpleLineStyle.xsl"/>

  <!--Include templates/rules for: GeoFeature-->
	<xsl:include href="MarineProtectedArea.xsl"/>
  <xsl:include href="RestrictedAreaNavigational.xsl"/>
  <xsl:include href="RestrictedAreaRegulatory.xsl"/>
  <xsl:include href="VesselTrafficServiceArea.xsl"/>
  
  <!--Include templates/rules for: MetaFeature-->
  <xsl:include href="DataCoverage.xsl"/>
  <xsl:include href="QualityOfNonBathymetricData.xsl"/>
  
  <!--Include templates/rules for: Default-->
  <xsl:include href="Default.xsl"/>

	<xsl:template match="/">
		<displayList>
			<xsl:apply-templates select="Dataset/Features/*"/>
		</displayList>
	</xsl:template>
</xsl:transform>
