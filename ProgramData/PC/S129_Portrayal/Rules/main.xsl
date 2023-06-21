<?xml version="1.0" encoding="utf-8" ?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
  <xsl:decimal-format name="dformat" decimal-separator="." grouping-separator=","/>
  <xsl:include href="PlannedRoute.xsl"/>
  <xsl:include href="AlmostNavigableArea.xsl"/>
  <xsl:include href="NonNavigableArea.xsl"/>
  <xsl:include href="ControlPoint.xsl"/>
  <xsl:include href="InformationBox.xsl"/>
  <xsl:include href="UnderKeelClearancePlan.xsl"/>

  <xsl:template match="/">
    <displayList>
      <xsl:apply-templates select="Dataset/Features/*"></xsl:apply-templates>
    </displayList>
  </xsl:template>

</xsl:transform>