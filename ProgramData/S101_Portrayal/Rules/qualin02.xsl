<?xml version="1.0" encoding="UTF-8"?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
    <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
    <!--Template for: Coastline and land area boundaries -->
        <!--<xsl:call-template name="QUAPOS01">
            <xsl:with-param name="viewingGroup">12410</xsl:with-param>
            <xsl:with-param name="displayPlane">OVERRADAR</xsl:with-param>
            <xsl:with-param name="drawingPriority">70</xsl:with-param>
        </xsl:call-template>-->
<!--Jan 28, 2015, basic coastline and radar conspicuous presentation,  spatial quality not implemented yet -->
	<xsl:template name="QUALIN02">
	<xsl:param name="viewingGroup"/> <!-- The viewing group -->
	<xsl:param name="displayPlane"/> <!-- The display plane -->
	<xsl:param name="drawingPriority"/> <!-- The drawing priority -->
		<xsl:choose>
			<xsl:when test="local-name() = 'Coastline' and radarConspicuous = '1'"> 
				<lineInstruction>
					<featureReference>
						<xsl:value-of select="@id"/>
					</featureReference>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<lineStyle>
						<pen width="0.96">
							<color>CHMGF</color>
						</pen>
					</lineStyle>
				</lineInstruction>
				<lineInstruction>
					<featureReference>
						<xsl:value-of select="@id"/>
					</featureReference>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<lineStyle>
						<pen width="0.32">
							<color>CSTLN</color>
						</pen>
					</lineStyle>
				</lineInstruction>
			</xsl:when>
			<xsl:otherwise>
				<lineInstruction>
					<featureReference>
						<xsl:value-of select="@id"/>
					</featureReference>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<lineStyle>
						<pen width="0.32">
							<color>CSTLN</color>
						</pen>
					</lineStyle>
				</lineInstruction>
			</xsl:otherwise>
		</xsl:choose>	
		
    </xsl:template>
</xsl:transform>
