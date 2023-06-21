<?xml version="1.0" encoding="UTF-8"?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
	<xsl:output method="xml" encoding="UTF-8" indent="yes"/>
		<xsl:template match="RestrictedAreaNavigational[@primitive='Surface']" priority="1">
			<pointInstruction>
				<featureReference>
					<xsl:value-of select="@id"/>
				</featureReference>
        <viewingGroup>12710</viewingGroup>
        <displayPlane>OVERRADAR</displayPlane>
        <drawingPriority>15</drawingPriority>
				<symbol reference="RestrictedAreaNavigational"/>
			</pointInstruction>
			<lineInstruction>
				<featureReference>
					<xsl:value-of select="@id"/>
				</featureReference>
        <viewingGroup>12710</viewingGroup>
        <displayPlane>OVERRADAR</displayPlane>
        <drawingPriority>15</drawingPriority>
        <lineStyle>
          <capStyle/>
          <joinStyle/>
          <offset/>
          <pen width="0.32">
            <color>TESTI</color>
          </pen>
        </lineStyle>
			</lineInstruction>
			<areaInstruction>
				<featureReference>
					<xsl:value-of select="@id"/>
				</featureReference>
        <viewingGroup>12710</viewingGroup>
        <displayPlane>OVERRADAR</displayPlane>
        <drawingPriority>15</drawingPriority>
        <colorFill>
          <color transparency="0.70">TESTJ</color>
        </colorFill>
			</areaInstruction>
		</xsl:template>
</xsl:transform>
