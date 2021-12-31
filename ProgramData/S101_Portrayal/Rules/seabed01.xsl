<?xml version="1.0" encoding="UTF-8"?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
			<xsl:output method="xml" encoding="UTF-8" indent="yes"/>
	<!-- Jan 26, 2015 Named template for defining Seabed depth colours, based on S-52 CSP by the same name -->
	<!-- Jul 20, 2015, updated to fix bug in DEPMS logic -->
	<!-- Sep, 2016, updated to test if min/max depths are actual numbers -->
	<xsl:template name="seabed01">
	<xsl:param name="viewingGroup"/> <!-- The viewing group -->
	<xsl:param name="displayPlane"/> <!-- The display plane -->
	<xsl:param name="drawingPriority"/> <!-- The drawing priority -->

		<xsl:variable name="drval1">
			<xsl:choose>
				<xsl:when test="depthRangeMinimumValue and number(depthRangeMinimumValue) = number(depthRangeMinimumValue) "> <!-- min depth is defined -->
					<xsl:value-of select="number(depthRangeMinimumValue)"/>
				</xsl:when>
				<xsl:otherwise>-1</xsl:otherwise>
			</xsl:choose>	
		</xsl:variable>
		<xsl:variable name="drval2">
			<xsl:choose>
				<xsl:when test="depthRangeMaximumValue and number(depthRangeMaximumValue) = number(depthRangeMaximumValue) "> <!-- max depth is defined -->
					<xsl:value-of select="number(depthRangeMaximumValue)"/>
				</xsl:when>
				<xsl:otherwise>					
					<xsl:value-of select="$drval1 + 0.01"/>
				</xsl:otherwise>
			</xsl:choose>	
		</xsl:variable>
		
		<!--In xslt you can't change the value of a variable once it is set so the logic is reversed from seabed01 such that the default 'DEPIT' gets assigned if no other match is made -->
		<!--Check deeper than Safety contour, otherwise determine shallow colours -->
		<xsl:choose>
			<xsl:when test="$drval1 >= $SAFETY_CONTOUR and $drval2 > $SAFETY_CONTOUR"> <!-- Deeper than safety contour -->
				<xsl:choose>
					<xsl:when test="$TWO_SHADES = 'true' or ($drval1 >= $DEEP_CONTOUR and $drval2 > $DEEP_CONTOUR)"> 
						<!-- Area fill with DEPDW -->
					  <areaInstruction>
						<featureReference><xsl:value-of select="@id"/></featureReference>
						<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
						<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
						<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
						<colorFill>
						  <color>DEPDW</color>
						</colorFill>
					  </areaInstruction>
					</xsl:when>
					<xsl:otherwise>
						<!-- Area fill with DEPMD -->
					  <areaInstruction>
						<featureReference><xsl:value-of select="@id"/></featureReference>
						<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
						<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
						<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
						<colorFill>
						  <color>DEPMD</color>
						</colorFill>
					  </areaInstruction>
					</xsl:otherwise>
				</xsl:choose>	
			</xsl:when>
			<xsl:otherwise>
				<!-- Shallower than safety contour -->
				<xsl:choose>
					<xsl:when test="not($TWO_SHADES = 'true') and $drval1 >= $SHALLOW_CONTOUR and $drval2 > $SHALLOW_CONTOUR "> <!-- medium shallow  -->
						<!-- Area fill with DEPMS -->
					  <areaInstruction>
						<featureReference><xsl:value-of select="@id"/></featureReference>
						<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
						<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
						<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
						<colorFill>
						  <color>DEPMS</color>
						</colorFill>
					  </areaInstruction>
					</xsl:when>
					<xsl:when test="$drval1 >= 0 and $drval2 > 0"> <!-- Very shallow -->
						<!-- Area fill with DEPVS -->
					  <areaInstruction>
						<featureReference><xsl:value-of select="@id"/></featureReference>
						<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
						<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
						<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
						<colorFill>
						  <color>DEPVS</color>
						</colorFill>
					  </areaInstruction>
					</xsl:when>
					<xsl:otherwise>
						<!-- Area fill with DEPIT -->
					  <areaInstruction>
						<featureReference><xsl:value-of select="@id"/></featureReference>
						<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
						<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
						<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
						<colorFill>
						  <color>DEPIT</color>
						</colorFill>
					  </areaInstruction>
					</xsl:otherwise>
				</xsl:choose>
				<!-- Pattern fill with DIAMOND1 applicable only shoaler than safety contour-->
				<xsl:if test="$SHALLOW_PATTERN = 'true'">
					<!-- Pattern fill with DIAMOND1 -->
					<areaInstruction>
						<featureReference><xsl:value-of select="@id"/></featureReference>
						<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
						<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
						<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
						<areaFillReference reference="DIAMOND1"/>
					</areaInstruction>
				</xsl:if>
			</xsl:otherwise>
		</xsl:choose>	
	</xsl:template>
			
</xsl:transform>
