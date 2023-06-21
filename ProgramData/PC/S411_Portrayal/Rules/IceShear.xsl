<?xml version="1.0" encoding="UTF-8"?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
<xsl:output method="xml" encoding="UTF-8" indent="yes"/>
<xsl:decimal-format name="dformat" decimal-separator="." grouping-separator=","/>
	<xsl:template match="IceShear[@primitive='Point']" priority="1">
		<pointInstruction>
			<featureReference>
				<xsl:value-of select="@id"/>
			</featureReference>
			<viewingGroup>21010</viewingGroup>
			<displayPlane>underRadar</displayPlane>
			<drawingPriority>15</drawingPriority>
			<symbol reference="ICESHR01">
			</symbol>
		</pointInstruction>
	</xsl:template>
</xsl:transform>
