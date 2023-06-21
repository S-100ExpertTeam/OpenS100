<?xml version="1.0" encoding="UTF-8"?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
			<xsl:output method="xml" encoding="UTF-8" indent="yes"/>
	<!-- Jan 26, 2015 Named template for restriction area symbols, based on S-52 CSP by the same name -->
	<!-- Nov, 2016 do nothing if there is no restriction value -->
	<xsl:template name="RESCSP03">
	<xsl:param name="viewingGroup"/> <!-- The viewing group -->
	<xsl:param name="displayPlane"/> <!-- The display plane -->
	<xsl:param name="drawingPriority"/> <!-- The drawing priority -->
		<!-- If  (majorRestriction=Entry)  draw ENTRES51
		Else if (majorRestriction=Anchoring)  draw ACHRES51
		Else if (majorRestriction=Fishing)  draw FSHRES51
		Else if (majorRestriction=Caution)  draw CTYARE51
		Else if (majorRestriction=Informative)  draw INFARE51

		If cautionaryRestrictions draw offset caution symbol '!'.
		If informativeRestrictions draw offset information symbol 'i'. -->
		<xsl:if test="restriction != ''">
		<xsl:variable name="centreSymbol">
			<xsl:choose>
				<xsl:when test="restriction = 7 or restriction = 8 or restriction = 14 "> <!-- No Entry -->
					<xsl:choose>
						<xsl:when test=" 7 > restriction  or restriction = 13 or restriction = 16 or restriction = 17 or restriction = 23 or restriction = 24 or restriction = 25 or restriction = 26 or restriction = 27 "> 
							<xsl:text>ENTRES61</xsl:text>
						</xsl:when>
						<xsl:when test="restriction = 9 or restriction = 10 or restriction = 11 or restriction = 12 or restriction = 15 or restriction = 18 or restriction = 19 or restriction = 20 or restriction = 21 or restriction = 22"> 
							<xsl:text>ENTRES71</xsl:text>
						</xsl:when>
						<xsl:otherwise>
							<!-- draw centred symbol ENTRES51 -->
							<xsl:text>ENTRES51</xsl:text>
						</xsl:otherwise>
					</xsl:choose>	
				</xsl:when>
				<xsl:when test="restriction = 1 or restriction = 2"> <!-- No Anchoring -->
					<xsl:choose>
						<xsl:when test="restriction = 3 or restriction = 4 or restriction = 5 or restriction = 6 or restriction = 13 or restriction = 16 or restriction = 17 or restriction = 23 or restriction = 24 or restriction = 25 or restriction = 26 or restriction = 27 "> 
							<xsl:text>ACHRES61</xsl:text>
						</xsl:when>
						<xsl:when test="restriction = 9 or restriction = 10 or restriction = 11 or restriction = 12 or restriction = 15 or restriction = 18 or restriction = 19 or restriction = 20 or restriction = 21 or restriction = 22"> 
							<xsl:text>ACHRES71</xsl:text>
						</xsl:when>
						<xsl:otherwise>
							<!-- draw centred symbol ACHRES51 -->
							<xsl:text>ACHRES51</xsl:text>
						</xsl:otherwise>
					</xsl:choose>	
				</xsl:when>
				<xsl:when test="restriction = 3 or restriction = 4 or restriction = 5 or restriction = 6 or restriction = 24"> <!-- No Fishing -->
					<xsl:choose>
						<xsl:when test="restriction = 13 or restriction = 16 or restriction = 17 or restriction = 23 or restriction = 25 or restriction = 26 or restriction = 27 "> 
							<xsl:text>FSHRES61</xsl:text>
						</xsl:when>
						<xsl:when test="restriction = 9 or restriction = 10 or restriction = 11 or restriction = 12 or restriction = 15 or restriction = 18 or restriction = 19 or restriction = 20 or restriction = 21 or restriction = 22"> 
							<xsl:text>FSHRES71</xsl:text>
						</xsl:when>
						<xsl:otherwise>
							<!-- draw centred symbol FSHRES51 -->
							<xsl:text>FSHRES51</xsl:text>
						</xsl:otherwise>
					</xsl:choose>	
				</xsl:when>
				<xsl:when test="restriction = 13 or restriction = 16 or restriction = 17 or restriction = 23 or restriction = 25 or restriction = 26 or restriction = 27 "> <!-- Caution, restrictions -->
					<xsl:choose>
						<xsl:when test="restriction = 9 or restriction = 10 or restriction = 11 or restriction = 12 or restriction = 15 or restriction = 18 or restriction = 19 or restriction = 20 or restriction = 21 or restriction = 22"> 
							<xsl:text>CTYARE71</xsl:text>
						</xsl:when>
						<xsl:otherwise>
							<!-- draw centred symbol CTYARE51 -->
							<xsl:text>CTYARE51</xsl:text>
						</xsl:otherwise>
					</xsl:choose>	
				</xsl:when>
				<xsl:when test="restriction = 9 or restriction = 10 or restriction = 11 or restriction = 12 or restriction = 15 or restriction = 18 or restriction = 19 or restriction = 20 or restriction = 21 or restriction = 22"> <!-- Information about restrictions -->
						<!-- draw centred symbol INFARE51 -->
						<xsl:text>INFARE51</xsl:text>
				</xsl:when>
				<xsl:otherwise>
						<!-- draw centred symbol unknown restrictions RSRDEF51 -->
						<xsl:text>RSRDEF51</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<pointInstruction>
			<featureReference><xsl:value-of select="@id"/></featureReference>
			<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
			<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
			<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
			<xsl:element name="symbol">
				<xsl:attribute name="reference"><xsl:value-of select="$centreSymbol"/></xsl:attribute> 
				<areaPlacement placementMode="VisibleParts"/>
			</xsl:element>
		</pointInstruction>

		</xsl:if>	

	</xsl:template>
			
</xsl:transform>
