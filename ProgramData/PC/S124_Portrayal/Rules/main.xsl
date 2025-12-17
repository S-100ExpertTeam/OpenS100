<?xml version="1.0" encoding="UTF-8"?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
  <xsl:decimal-format name="dformat" decimal-separator="." grouping-separator=","/>
  
  <xsl:include href="simpleLineStyle.xsl"/>
  
  <!--Include templates/rules for: attributes-->
  <xsl:include href="attributeRules.xsl"/>
  <xsl:include href="_Default_COMMON.xsl"/>

  <!--Include templates/rules for: styles-->
  <xsl:include href="textStyle.xsl"/>

  <!--Include templates/rules for: csps-->

  <!--Include templates/rules for new S-124 features-->
	<xsl:include href="NavwarnPart.xsl"/>
	<xsl:include href="NavwarnAreaAffected.xsl"/>
  <xsl:include href="TextPlacement.xsl"/>

  <!--Include templates/rules for: Meta Features -->
  
  <xsl:template match="/">
    <displayList>
      <xsl:apply-templates select="Dataset/Features/*"/>
    </displayList>
  </xsl:template>
</xsl:transform>
