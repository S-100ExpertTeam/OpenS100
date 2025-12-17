<?xml version="1.0" encoding="UTF-8"?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
	<xsl:output method="xml" encoding="UTF-8" indent="yes"/>
	<xsl:template match="featureName" mode="text">
		<xsl:param name="prefix"/>
		<xsl:param name="suffix"/>
		<!-- features can have more than one featureName, the name used for display should have "displayName = 'true'" but if there is no featureName with displayName = 'true' this will output the first featureName in the list. If there is more than one featureName with displayName = 'true' all of them will be output, DCEG says only one should be true -->
			<xsl:if test="nameUsage = 1 or (position() = 1 and count(../featureName[nameUsage = 1]) = 0  )">
				<xsl:if test="$prefix">
					<xsl:value-of select="$prefix"/>
				</xsl:if>
				<xsl:value-of select="name"/>
				<xsl:if test="$suffix">
					<xsl:value-of select="$suffix"/>
				</xsl:if>
			</xsl:if>
	</xsl:template>
  <xsl:template match="text" mode="text">
    <xsl:param name="prefix"/>
    <xsl:param name="suffix"/>
    <!-- features can have more than one featureName, the name used for display should have "displayName = 'true'" but if there is no featureName with displayName = 'true' this will output the first featureName in the list. If there is more than one featureName with displayName = 'true' all of them will be output, DCEG says only one should be true -->
    <xsl:if test="$prefix">
      <xsl:value-of select="$prefix"/>
    </xsl:if>
    <xsl:value-of select="."/>
    <xsl:if test="$suffix">
      <xsl:value-of select="$suffix"/>
    </xsl:if>
  </xsl:template>
	<xsl:template match="information/text" mode="text">
		<xsl:param name="prefix"/>
		<xsl:param name="suffix"/>
		<xsl:if test="$prefix">
			<xsl:value-of select="$prefix"/>
		</xsl:if>
		<xsl:value-of select="."/>
		<xsl:if test="$suffix">
			<xsl:value-of select="$suffix"/>
		</xsl:if>
	</xsl:template>
</xsl:transform>
