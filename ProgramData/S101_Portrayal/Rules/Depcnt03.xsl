<?xml version="1.0" encoding="UTF-8"?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
	<xsl:output method="xml" encoding="UTF-8" indent="yes"/>
	<!--
-->		
	<!--Template for: DepthContour-->
	<!-- Updated Sept 2016. Named template for depth contours. 
		contour labels not implemented yet, contours matching safety contour get drawn as safety 
		This make the contour labels one per contour as opposed to adding a label to each curve as would happen in depare04
		It leaves depare04 to only make safety contours on curves that are not contours that match SAFETY_CONTOUR -->
	<xsl:template name="DEPCNT03">
		<!-- The viewing group -->
		<xsl:param name="viewingGroup"/>
		<!-- The display plane -->
		<xsl:param name="displayPlane"/>
		<!-- The drawing priority -->
		<xsl:param name="drawingPriority"/>

		<!-- keep a reference to this feature to use when context is switched to curves -->
		<xsl:variable name="contour" select="."/>

		
		<!-- loop over each curve reference -->
		<xsl:for-each select="Curve">
			<!-- forward flag to use in the line instruction spatial reference -->
			<xsl:variable name="forward">
				<xsl:choose>
					<xsl:when test="@orientation = 'Forward' ">		
						<xsl:value-of select="true()" />
					</xsl:when>
					<xsl:otherwise>		
						<xsl:value-of select="false()" />
					</xsl:otherwise>
				</xsl:choose>
			</xsl:variable>
			<!-- retrieve the referenced curve using the index -->
			<xsl:variable name="curve" select="key('Curveids', @ref)"/>
			<!-- retrieve the curves spatial quality using the index of quality  -->
			<xsl:variable name="quality" select="key('qualityids', $curve/spatialQuality/@informationRef)"/>
			<xsl:variable name="linestyle">
				<xsl:choose>
					<xsl:when test="not($quality/qualityOfPosition) ">		
						<xsl:text>solid</xsl:text>
					</xsl:when>
					<xsl:when test="$quality/qualityOfPosition = 1 or $quality/qualityOfPosition = 10 or $quality/qualityOfPosition = 11 ">		
						<xsl:text>solid</xsl:text>
					</xsl:when>
					<xsl:otherwise>		
						<xsl:text>dash</xsl:text>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:variable>

			<!--check if Safety contour otherwise handle as simple contour -->
			<xsl:choose>
				<xsl:when test="$contour/valueOfDepthContour = $SAFETY_CONTOUR"> 
					<lineInstruction>
						<featureReference>
							<xsl:value-of select="$contour/@id"/>
						</featureReference>
						<xsl:element name="spatialReference">
							<xsl:attribute name="forward">
								<xsl:value-of select="$forward"/>
							</xsl:attribute>
							<xsl:value-of select="@ref"/>
						</xsl:element>
						<viewingGroup>13000</viewingGroup>
						<displayPlane>OVERRADAR</displayPlane>
						<drawingPriority>24</drawingPriority>
						<xsl:call-template name="simpleLineStyle">
							<xsl:with-param name="style" select="$linestyle"/>
							<xsl:with-param name="width">0.64</xsl:with-param>
							<xsl:with-param name="colour">DEPSC</xsl:with-param>
						</xsl:call-template>
					</lineInstruction>
				</xsl:when>
				<xsl:otherwise>
					<lineInstruction>
						<featureReference>
							<xsl:value-of select="$contour/@id"/>
						</featureReference>
						<xsl:element name="spatialReference">
							<xsl:attribute name="forward">
								<xsl:value-of select="$forward"/>
							</xsl:attribute>
							<xsl:value-of select="@ref"/>
						</xsl:element>
						<viewingGroup>
							<xsl:value-of select="$viewingGroup"/>
						</viewingGroup>
						<displayPlane>
							<xsl:value-of select="$displayPlane"/>
						</displayPlane>
						<drawingPriority>
							<xsl:value-of select="$drawingPriority"/>
						</drawingPriority>
						<xsl:call-template name="simpleLineStyle">
							<xsl:with-param name="style" select="$linestyle"/>
							<xsl:with-param name="width">0.32</xsl:with-param>
							<xsl:with-param name="colour">DEPCN</xsl:with-param>
						</xsl:call-template>
					</lineInstruction>
				</xsl:otherwise>
			</xsl:choose>

		</xsl:for-each> 

		<!-- generate contour labels for the entire feature in the appropriate viewing group so they can be turned on/off afterwards -->
		<xsl:choose>
			<xsl:when test="$contour/valueOfDepthContour = $SAFETY_CONTOUR ">
				<xsl:if test="count(Curve) = 0">
					<lineInstruction>
						<featureReference>
							<xsl:value-of select="$contour/@id"/>
						</featureReference>
						<viewingGroup>13000</viewingGroup>
						<displayPlane>OVERRADAR</displayPlane>
						<drawingPriority>24</drawingPriority>
						<xsl:call-template name="simpleLineStyle">
							<xsl:with-param name="style" select="'solid'"/>
							<xsl:with-param name="width">0.64</xsl:with-param>
							<xsl:with-param name="colour">DEPSC</xsl:with-param>
						</xsl:call-template>
					</lineInstruction>
				</xsl:if>
				<xsl:call-template name="SAFCON02">
					<xsl:with-param name="contour" select="$contour"/>
					<xsl:with-param name="curveid"/>
					<xsl:with-param name="viewingGroup" select="'33021'"/>
					<xsl:with-param name="drawingPriority" select="'24'"/>
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>		
				<xsl:if test="count(Curve) = 0">
					<lineInstruction>
						<featureReference>
							<xsl:value-of select="$contour/@id"/>
						</featureReference>
						<viewingGroup>
							<xsl:value-of select="$viewingGroup"/>
						</viewingGroup>
						<displayPlane>
							<xsl:value-of select="$displayPlane"/>
						</displayPlane>
						<drawingPriority>
							<xsl:value-of select="$drawingPriority"/>
						</drawingPriority>
						<xsl:call-template name="simpleLineStyle">
							<xsl:with-param name="style" select="'solid'"/>
							<xsl:with-param name="width">0.32</xsl:with-param>
							<xsl:with-param name="colour">DEPCN</xsl:with-param>
						</xsl:call-template>
					</lineInstruction>
				</xsl:if>
				<xsl:call-template name="SAFCON02">
					<xsl:with-param name="contour" select="$contour"/>
					<xsl:with-param name="curveid"/>
					<xsl:with-param name="viewingGroup" select="'33022'"/>
					<xsl:with-param name="drawingPriority" select="$drawingPriority"/>
				</xsl:call-template>
			</xsl:otherwise>
		</xsl:choose>


	</xsl:template>
</xsl:transform>
