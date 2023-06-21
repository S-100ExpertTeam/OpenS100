<?xml version="1.0" encoding="UTF-8"?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
	<xsl:output method="xml" encoding="UTF-8" indent="yes"/>

	<!-- first draft prepared by Hugh Astle (Teledyne CARIS)  Sept 2016 -->	
	<!-- create key indexes to quickly lookup curves, these could be moved to a common location if desired but can be accessed from anywhere -->	
	<xsl:key name="depthAreas" match="/Dataset/Features/DepthArea | /Dataset/Features/DredgedArea " use="Surface/@ref"/>
	<xsl:key name="datalimits" match="/Dataset/Features/DataCoverage" use="Surface/@ref"/>
	<xsl:key name="surfaces" match="/Dataset/Surfaces/Surface" use="@id"/>
	<xsl:key name="surfaceCurveRef" match="/Dataset/Surfaces/Surface/Ring/Curve" use="concat(@ref,'|',@orientation)"/>
	<xsl:key name="contourCurveRef" match="/Dataset/Features/DepthContour" use="Curve/@ref"/>

	<!-- Only called from a Depth area or Dredged Area, only process depth areas that are safe (DRVAL1 >= SAFETY_CONTOUR) -->
	<!-- This template is written with the assumption that CompositeCurves will not be used in portrayal -->	
	<xsl:template name="DEPARE04">
		<xsl:if test="depthRangeMinimumValue >= $SAFETY_CONTOUR ">		

			<!-- Process all the curves referenced from the surface and look for another safe depth/deredged area on the other side -->
			<!-- When no safe water is found on the other side treat the curve as a safety contour -->
			<xsl:variable name="surface" select="key('surfaces', Surface/@ref)"/>

			<!-- For each referenced curve, flip the orientation and navigate the hierarchy, testing for related depth areas -->
			<xsl:apply-templates select="$surface/Ring/Curve" mode="DEPARE04" >
				<xsl:with-param name="feature" select="." />
			</xsl:apply-templates>
		</xsl:if>
	</xsl:template>

	<!-- This template only matches curve references from within a surface ring and only when executed using the DEPARE04 mode -->
	<xsl:template match="Ring/Curve" mode="DEPARE04">
		<xsl:param name="feature" />

		<!-- Use the index of depth contours to find a contour referencing this curve -->
		<xsl:variable name="contour" select="key('contourCurveRef', @ref)[1]" />
		<!-- If there is a shared contour set the loc_valdco otherwise it will be empty -->
		<xsl:variable name="loc_valdco">
			<!-- look for an associated contour -->
			<!-- Check if valdco is populated, if so get it otherwise set it to 0 -->
			<xsl:choose>
				<xsl:when test="not($contour) "> 
				</xsl:when>
				<xsl:when test="$contour/valueOfDepthContour != '' "> 
					<xsl:value-of select="$contour/valueOfDepthContour" />
				</xsl:when>
				<xsl:otherwise>		
					<xsl:value-of select="0" />
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<!-- flip the orientation because we are looking for the surface on the other side -->
		<xsl:variable name="flippedOrientation">
			<xsl:choose>
				<xsl:when test="@orientation = 'Forward' ">		
					<xsl:value-of select="'Reverse'" />
				</xsl:when>
				<xsl:otherwise>		
					<xsl:value-of select="'Forward'" />
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<!-- Check if there is a direct relation to a depth(or dredged area) surface on the other side and if it is also safe -->
		<xsl:variable name="safeBoundary" >
			<!-- loop over all the surfaces that are referencing the current curve in the other direction -->
			<!-- use the index of surface to curve references to optimize this query -->
			<xsl:for-each select="key('surfaceCurveRef', concat(@ref,'|',$flippedOrientation))">
				<!-- look for a depth/dredeged area that is associated with the surface referencing the same curve -->
				<!-- use the index of depth and dredged area relationships to surface ids -->
				<!-- There should be at most one but the for-each will set the context to that feature if found -->
				<xsl:for-each select="key('depthAreas', ../../@id)">
					<xsl:if test="depthRangeMinimumValue !='' and depthRangeMinimumValue >= $SAFETY_CONTOUR" >
						<!-- found safe water on the other side as well so set safeBoundary to SAFE -->
					  <xsl:text>SAFE</xsl:text>
					</xsl:if>
				</xsl:for-each> 
			</xsl:for-each> 
		</xsl:variable>
		<xsl:variable name="datalimitids">
			<!-- loop over all the surfaces that are referencing the current curve in the same direction -->
			<xsl:for-each select="key('surfaceCurveRef', concat(@ref,'|',@orientation))">
				<!-- look for a data limit area that using the surface, if found list the id -->
				<xsl:for-each select="key('datalimits', ../../@id)">
					<xsl:value-of select="@id" /><xsl:text>,</xsl:text>
				</xsl:for-each> 
			</xsl:for-each> 
		</xsl:variable>
		<xsl:variable name="loc_safety">
			<!-- Check if valdco is populated, if so get it otherwise set it to 0 -->
			<xsl:choose>
				<xsl:when test="$loc_valdco = $SAFETY_CONTOUR "> 
					<!-- If it is on a contour which is SAFETY_CONTOUR then depcnt03 will symbolize it -->
					<xsl:value-of select="'false'" />
				</xsl:when>
				<xsl:when test="$safeBoundary and contains($safeBoundary,'SAFE') "> 
					<!-- found a safe depth on the other side -->
					<xsl:value-of select="'false'" />
				</xsl:when>
				<xsl:when test="$datalimitids and $datalimitids !='' "> 
					<!-- Don't need a safety contour along data limit, could be continued in next cell -->
					<!-- ***???  but what if next cell is unsafe water, rare but possible case not handled -->
					<xsl:value-of select="'false'" />
				</xsl:when>
				<xsl:otherwise>		
					<!-- We have an unsafe boundary so show it as a safety contour -->
					<xsl:value-of select="'true'" />
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>



		<xsl:if test="$loc_safety = 'true'">		
			<!-- we have a safety contour curve output a line instruction with the spatial reference -->
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
			<!-- Test if the curve is low quality, because a dashed line will be used then -->
			<!--  get a reference to the actual curve to see if is has an information ref -->
			<xsl:variable name="curve" select="key('Curveids', @ref)"/>
			<!--  get a reference to the associated spatial quality object from the index -->
			<xsl:variable name="quality" select="key('qualityids', $curve/spatialQuality/@informationRef)"/>
			<!--  set the linestyle to use for the curve -->
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


			<lineInstruction>
				<featureReference>
					<xsl:value-of select="$feature/@id"/>
				</featureReference>
				<xsl:element name="spatialReference">
					<xsl:attribute name="forward">
						<xsl:value-of select="$forward"/>
					</xsl:attribute>
					<xsl:value-of select="@ref"/>
				</xsl:element>
				<viewingGroup>13010</viewingGroup>
				<displayPlane>OVERRADAR</displayPlane>
				<drawingPriority>24</drawingPriority>
				<xsl:call-template name="simpleLineStyle">
					<xsl:with-param name="style" select="$linestyle"/>
					<xsl:with-param name="width">0.64</xsl:with-param>
					<xsl:with-param name="colour">DEPSC</xsl:with-param>
				</xsl:call-template>
			</lineInstruction>

			<xsl:if test="$loc_valdco and number($loc_valdco) = number($loc_valdco) ">
				<!--  the curve is a safety contour and has a numeric value so add labels to the appropriate viewing group -->
				<xsl:call-template name="SAFCON02">
					<xsl:with-param name="contour" select="$contour"/>
					<xsl:with-param name="curveid" select="@ref"/>
					<xsl:with-param name="viewingGroup" select="'33021'"/>
					<xsl:with-param name="drawingPriority" select="'24'"/>
				</xsl:call-template>
			</xsl:if>
		</xsl:if>


	</xsl:template>

</xsl:transform>
