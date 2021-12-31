<?xml version="1.0" encoding="UTF-8"?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
	<xsl:output method="xml" encoding="UTF-8" indent="yes"/>

	<!-- Feb 2015 Named template for defining restricted area portrayal, based on S-52 ed 4 CSP by the same name -->
	<!-- Nov 2016 fixed typo in references to symbolizedBoundaries variable -->
	
	<xsl:template name="RESARE04">
		<xsl:param name="viewingGroup"/>
		<!-- The viewing group -->
		<xsl:param name="displayPlane"/>
		<!-- The display plane -->
		<xsl:param name="drawingPriority"/>
		<!-- The drawing priority -->
		<xsl:param name="symbolizedBoundaries"/>
		<!-- 'true' if symbolized area boundaries are to be drawn -->

		<xsl:choose>
			<xsl:when test="not(restriction)">
				<!-- no restrictions given -->
				<xsl:call-template name="RESARE04E">
					<xsl:with-param name="viewingGroup">
						<xsl:value-of select="$viewingGroup"/>
					</xsl:with-param>
					<xsl:with-param name="displayPlane">
						<xsl:value-of select="$displayPlane"/>
					</xsl:with-param>
					<xsl:with-param name="drawingPriority">
						<xsl:value-of select="$drawingPriority"/>
					</xsl:with-param>
					<xsl:with-param name="symbolizedBoundaries">
						<xsl:value-of select="$symbolizedBoundaries"/>
					</xsl:with-param>
				</xsl:call-template>
			</xsl:when>
			<xsl:when test="restriction = '7' or restriction = '8' or restriction = '14'">
				<!-- entry restricted or prohibited -->
				<xsl:call-template name="RESARE04A">
					<xsl:with-param name="viewingGroup">
						<xsl:value-of select="$viewingGroup"/>
					</xsl:with-param>
					<xsl:with-param name="drawingPriority">18</xsl:with-param>
					<xsl:with-param name="symbolizedBoundaries">
						<xsl:value-of select="$symbolizedBoundaries"/>
					</xsl:with-param>
				</xsl:call-template>
			</xsl:when>
			<xsl:when test="restriction = '1' or restriction = '2'">
				<!-- anchoring restricted or prohibited -->
				<xsl:call-template name="RESARE04B">
					<xsl:with-param name="viewingGroup">
						<xsl:value-of select="$viewingGroup"/>
					</xsl:with-param>
					<xsl:with-param name="displayPlane">
						<xsl:value-of select="$displayPlane"/>
					</xsl:with-param>
					<xsl:with-param name="drawingPriority">18</xsl:with-param>
					<xsl:with-param name="symbolizedBoundaries">
						<xsl:value-of select="$symbolizedBoundaries"/>
					</xsl:with-param>
				</xsl:call-template>
			</xsl:when>
			<xsl:when test="restriction = '3' or restriction = '4' or restriction = '5' or restriction = '6' or restriction = '24'">
				<!-- fishing restricted or prohibited -->
				<xsl:call-template name="RESARE04C">
					<xsl:with-param name="viewingGroup">
						<xsl:value-of select="$viewingGroup"/>
					</xsl:with-param>
					<xsl:with-param name="displayPlane">
						<xsl:value-of select="$displayPlane"/>
					</xsl:with-param>
					<xsl:with-param name="drawingPriority">18</xsl:with-param>
					<xsl:with-param name="symbolizedBoundaries">
						<xsl:value-of select="$symbolizedBoundaries"/>
					</xsl:with-param>
				</xsl:call-template>
			</xsl:when>
			<xsl:when test="restriction = '13' or restriction = '16' or restriction = '17' or restriction = '23' or restriction = '25' or restriction = '26' or restriction = '27' ">
				<!-- own ship restrictions -->
				<xsl:call-template name="RESARE04D">
					<xsl:with-param name="viewingGroup">
						<xsl:value-of select="$viewingGroup"/>
					</xsl:with-param>
					<xsl:with-param name="drawingPriority">18</xsl:with-param>
					<xsl:with-param name="symbolizedBoundaries">
						<xsl:value-of select="$symbolizedBoundaries"/>
					</xsl:with-param>
				</xsl:call-template>
			</xsl:when>
			<xsl:when test="restriction = '9' or restriction = '10' or restriction = '11' or restriction = '12' or restriction = '15' or restriction = '18' or restriction = '19' or restriction = '20' or restriction = '21' or restriction = '22' ">
				<!-- minor restrictions or information notices  -->
				<pointInstruction>
					<featureReference>
						<xsl:value-of select="@id"/>
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
					<xsl:element name="symbol">
						<xsl:attribute name="reference">INFARE51</xsl:attribute> 
						<areaPlacement placementMode="VisibleParts"/>
					</xsl:element>
				</pointInstruction>
				<xsl:if test="$symbolizedBoundaries = 'true'">
					<lineInstruction>
						<featureReference>
							<xsl:value-of select="@id"/>
						</featureReference>
						<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
						<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
						<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
						<lineStyleReference reference="CTYARE51"/>
					</lineInstruction>
				</xsl:if>
			</xsl:when>
			<xsl:otherwise>
				<!-- undefined restrictions exist -->
				<pointInstruction>
					<featureReference>
						<xsl:value-of select="@id"/>
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
					<xsl:element name="symbol">
						<xsl:attribute name="reference">RSRDEF51</xsl:attribute> 
						<areaPlacement placementMode="VisibleParts"/>
					</xsl:element>
				</pointInstruction>
				<xsl:if test="$symbolizedBoundaries = 'true'">
					<lineInstruction>
						<featureReference>
							<xsl:value-of select="@id"/>
						</featureReference>
						<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
						<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
						<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
						<lineStyleReference reference="CTYARE51"/>
					</lineInstruction>
				</xsl:if>

			</xsl:otherwise>  
		</xsl:choose>	
		<xsl:if test="$symbolizedBoundaries != 'true'">
			<lineInstruction>
				<featureReference>
							<xsl:value-of select="@id"/>
				</featureReference>
				<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
				<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
				<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
				 <xsl:call-template name="simpleLineStyle">
					<xsl:with-param name="style">dash</xsl:with-param>
					<xsl:with-param name="width">0.64</xsl:with-param>
					<xsl:with-param name="colour">CHMGD</xsl:with-param>
				 </xsl:call-template>
			</lineInstruction>
		</xsl:if>

	</xsl:template>

	<!-- Feb 2015 Named template for defining restricted area portrayal, based on S-52 ed 4 CSP by the same name -->
	<xsl:template name="RESARE04A">
		<xsl:param name="viewingGroup"/>
		<!-- The viewing group -->
		<xsl:param name="displayPlane"/>
		<!-- The display plane -->
		<xsl:param name="drawingPriority"/>
		<!-- The drawing priority -->
		<xsl:param name="symbolizedBoundaries"/>
		<!-- 'true' if symbolized area boundaries are to be drawn -->

		<xsl:choose>
			<xsl:when test=" restriction = '1' or restriction = '2' or restriction = '3' or restriction = '4'or restriction = '5' or restriction = '6' or restriction = '13' or restriction = '16' or restriction = '17' or restriction = '23' or restriction = '24' or restriction = '25' or restriction = '26' or restriction = '27' ">
				<!-- area where entry is prohibited or restricted or "to be avoided", with other cautions -->
				<pointInstruction>
					<featureReference><xsl:value-of select="@id"/></featureReference>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<xsl:element name="symbol">
						<xsl:attribute name="reference">ENTRES61</xsl:attribute> 
						<areaPlacement placementMode="VisibleParts"/>
					</xsl:element>
				</pointInstruction>
				
			</xsl:when>
			<xsl:when test=" categoryOfRestrictedArea and ( categoryOfRestrictedArea = '1' or categoryOfRestrictedArea = '8' or categoryOfRestrictedArea = '9' or categoryOfRestrictedArea = '12' or categoryOfRestrictedArea = '14' or categoryOfRestrictedArea = '18' or categoryOfRestrictedArea = '19' or categoryOfRestrictedArea = '21' or categoryOfRestrictedArea = '24' or categoryOfRestrictedArea = '25' or categoryOfRestrictedArea = '26') ">
				<!-- area where entry is prohibited or restricted or "to be avoided", with other cautions -->
				<pointInstruction>
					<featureReference><xsl:value-of select="@id"/></featureReference>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<xsl:element name="symbol">
						<xsl:attribute name="reference">ENTRES61</xsl:attribute> 
						<areaPlacement placementMode="VisibleParts"/>
					</xsl:element>
				</pointInstruction>
				
			</xsl:when>
			<xsl:when test=" restriction = '9' or restriction = '10' or restriction = '11' or restriction = '12' or restriction = '15' or restriction = '18' or restriction = '19' or restriction = '20' or restriction = '21' or restriction = '22' ">
				<!-- area where entry is prohibited or restricted or "to be avoided", with other information -->
				<pointInstruction>
					<featureReference><xsl:value-of select="@id"/></featureReference>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<xsl:element name="symbol">
						<xsl:attribute name="reference">ENTRES71</xsl:attribute> 
						<areaPlacement placementMode="VisibleParts"/>
					</xsl:element>
				</pointInstruction>
				
			</xsl:when>
			<xsl:when test=" categoryOfRestrictedArea and ( categoryOfRestrictedArea = '4' or categoryOfRestrictedArea = '5' or categoryOfRestrictedArea = '6' or categoryOfRestrictedArea = '7' or categoryOfRestrictedArea = '10' or categoryOfRestrictedArea = '20' or categoryOfRestrictedArea = '22' or categoryOfRestrictedArea = '23' ) ">
				<!-- area where entry is prohibited or restricted or "to be avoided", with other information -->
				<pointInstruction>
					<featureReference><xsl:value-of select="@id"/></featureReference>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<xsl:element name="symbol">
						<xsl:attribute name="reference">ENTRES71</xsl:attribute> 
						<areaPlacement placementMode="VisibleParts"/>
					</xsl:element>
				</pointInstruction>
			</xsl:when>
			<xsl:otherwise>
				<!-- area where entry is prohibited or restricted or "to be avoided" -->
				<pointInstruction>
					<featureReference><xsl:value-of select="@id"/></featureReference>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<xsl:element name="symbol">
						<xsl:attribute name="reference">ENTRES51</xsl:attribute> 
						<areaPlacement placementMode="VisibleParts"/>
					</xsl:element>
				</pointInstruction>

			</xsl:otherwise>  
		</xsl:choose>	

		<xsl:if test="$symbolizedBoundaries = 'true'">
			<!-- symbolized boundary of an area where entry is prohibited or restricted -->
			<lineInstruction>
				<featureReference><xsl:value-of select="@id"/></featureReference>
				<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
				<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
				<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
				<lineStyleReference reference="ENTRES51"/>
			</lineInstruction>
		</xsl:if>

	</xsl:template>
	
	<!-- Feb 2015 Named template for defining restricted area portrayal, based on S-52 ed 4 CSP by the same name -->
	<xsl:template name="RESARE04B">
		<xsl:param name="viewingGroup"/>
		<!-- The viewing group -->
		<xsl:param name="displayPlane"/>
		<!-- The display plane -->
		<xsl:param name="drawingPriority"/>
		<!-- The drawing priority -->
		<xsl:param name="symbolizedBoundaries"/>
		<!-- 'true' if symbolized area boundaries are to be drawn -->

		<xsl:choose>
			<xsl:when test=" restriction = '3' or restriction = '4'or restriction = '5' or restriction = '6' or restriction = '13' or restriction = '16' or restriction = '17' or restriction = '23' or restriction = '24' or restriction = '25' or restriction = '26' or restriction = '27' ">
				<!-- area where anchoring is prohibited or restricted, with other cautions -->
				<pointInstruction>
					<featureReference><xsl:value-of select="@id"/></featureReference>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<xsl:element name="symbol">
						<xsl:attribute name="reference">ACHRES61</xsl:attribute> 
						<areaPlacement placementMode="VisibleParts"/>
					</xsl:element>
				</pointInstruction>
				
			</xsl:when>
			<xsl:when test=" categoryOfRestrictedArea and ( categoryOfRestrictedArea = '1' or categoryOfRestrictedArea = '8' or categoryOfRestrictedArea = '9' or categoryOfRestrictedArea = '12' or categoryOfRestrictedArea = '14' or categoryOfRestrictedArea = '18' or categoryOfRestrictedArea = '19' or categoryOfRestrictedArea = '21' or categoryOfRestrictedArea = '24' or categoryOfRestrictedArea = '25' or categoryOfRestrictedArea = '26' ) ">
				<!-- area where anchoring is prohibited or restricted, with other cautions -->
				<pointInstruction>
					<featureReference><xsl:value-of select="@id"/></featureReference>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<xsl:element name="symbol">
						<xsl:attribute name="reference">ACHRES61</xsl:attribute> 
						<areaPlacement placementMode="VisibleParts"/>
					</xsl:element>
				</pointInstruction>
				
			</xsl:when>
			<xsl:when test=" restriction = '9' or restriction = '10' or restriction = '11' or restriction = '12' or restriction = '15' or restriction = '18' or restriction = '19' or restriction = '20' or restriction = '21' or restriction = '22' ">
				<!--area where anchoring is prohibited or restricted, with other information -->
				<pointInstruction>
					<featureReference><xsl:value-of select="@id"/></featureReference>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<xsl:element name="symbol">
						<xsl:attribute name="reference">ACHRES71</xsl:attribute> 
						<areaPlacement placementMode="VisibleParts"/>
					</xsl:element>
				</pointInstruction>
				
			</xsl:when>
			<xsl:when test=" categoryOfRestrictedArea and ( categoryOfRestrictedArea = '4' or categoryOfRestrictedArea = '5' or categoryOfRestrictedArea = '6' or categoryOfRestrictedArea = '7' or categoryOfRestrictedArea = '10' or categoryOfRestrictedArea = '20' or categoryOfRestrictedArea = '22' or categoryOfRestrictedArea = '23' )">
				<!-- area where anchoring is prohibited or restricted, with other information -->
				<pointInstruction>
					<featureReference><xsl:value-of select="@id"/></featureReference>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<xsl:element name="symbol">
						<xsl:attribute name="reference">ACHRES71</xsl:attribute> 
						<areaPlacement placementMode="VisibleParts"/>
					</xsl:element>
				</pointInstruction>
			</xsl:when>
			<xsl:otherwise>
				<!-- area where anchoring is prohibited or restricted -->
				<pointInstruction>
					<featureReference><xsl:value-of select="@id"/></featureReference>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<xsl:element name="symbol">
						<xsl:attribute name="reference">ACHRES51</xsl:attribute> 
						<areaPlacement placementMode="VisibleParts"/>
					</xsl:element>
				</pointInstruction>

			</xsl:otherwise>  
		</xsl:choose>	

		<xsl:if test="$symbolizedBoundaries = 'true'">
			<!-- boundary of an area where anchoring is prohibited or restricted -->
			<lineInstruction>
				<featureReference><xsl:value-of select="@id"/></featureReference>
				<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
				<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
				<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
				<lineStyleReference reference="ACHRES51"/>
			</lineInstruction>
		</xsl:if>

	</xsl:template>
	
	<!-- Feb 2015 Named template for defining restricted area portrayal, based on S-52 ed 4 CSP by the same name -->
	<xsl:template name="RESARE04C">
		<xsl:param name="viewingGroup"/>
		<!-- The viewing group -->
		<xsl:param name="displayPlane"/>
		<!-- The display plane -->
		<xsl:param name="drawingPriority"/>
		<!-- The drawing priority -->
		<xsl:param name="symbolizedBoundaries"/>
		<!-- 'true' if symbolized area boundaries are to be drawn -->

		<xsl:choose>
			<xsl:when test="restriction = '13' or restriction = '16' or restriction = '17' or restriction = '23' or restriction = '24' or restriction = '25' or restriction = '26' or restriction = '27' ">
				<!-- area where fishing or trawling is prohibited or restricted, with other cautions -->
				<pointInstruction>
					<featureReference><xsl:value-of select="@id"/></featureReference>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<xsl:element name="symbol">
						<xsl:attribute name="reference">FSHRES61</xsl:attribute> 
						<areaPlacement placementMode="VisibleParts"/>
					</xsl:element>
				</pointInstruction>
				
			</xsl:when>
			<xsl:when test=" categoryOfRestrictedArea and ( categoryOfRestrictedArea = '1' or categoryOfRestrictedArea = '8' or categoryOfRestrictedArea = '9' or categoryOfRestrictedArea = '12' or categoryOfRestrictedArea = '14' or categoryOfRestrictedArea = '18' or categoryOfRestrictedArea = '19' or categoryOfRestrictedArea = '21' or categoryOfRestrictedArea = '24' or categoryOfRestrictedArea = '25' or categoryOfRestrictedArea = '26' ) ">
				<!-- area where fishing or trawling is prohibited or restricted, with other cautions -->
				<pointInstruction>
					<featureReference><xsl:value-of select="@id"/></featureReference>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<xsl:element name="symbol">
						<xsl:attribute name="reference">FSHRES61</xsl:attribute> 
						<areaPlacement placementMode="VisibleParts"/>
					</xsl:element>
				</pointInstruction>
				
			</xsl:when>
			<xsl:when test=" restriction = '9' or restriction = '10' or restriction = '11' or restriction = '12' or restriction = '15' or restriction = '18' or restriction = '19' or restriction = '20' or restriction = '21' or restriction = '22' ">
				<!-- area where fishing or trawling is prohibited or restricted, with other information -->
				<pointInstruction>
					<featureReference><xsl:value-of select="@id"/></featureReference>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<xsl:element name="symbol">
						<xsl:attribute name="reference">FSHRES71</xsl:attribute> 
						<areaPlacement placementMode="VisibleParts"/>
					</xsl:element>
				</pointInstruction>
				
			</xsl:when>
			<xsl:when test=" categoryOfRestrictedArea and ( categoryOfRestrictedArea = '4' or categoryOfRestrictedArea = '5' or categoryOfRestrictedArea = '6' or categoryOfRestrictedArea = '7' or categoryOfRestrictedArea = '10' or categoryOfRestrictedArea = '20' or categoryOfRestrictedArea = '22' or categoryOfRestrictedArea = '23' )">
				<!-- area where fishing or trawling is prohibited or restricted, with other information -->
				<pointInstruction>
					<featureReference><xsl:value-of select="@id"/></featureReference>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<xsl:element name="symbol">
						<xsl:attribute name="reference">FSHRES71</xsl:attribute> 
						<areaPlacement placementMode="VisibleParts"/>
					</xsl:element>
				</pointInstruction>
			</xsl:when>
			<xsl:otherwise>
				<!-- area where fishing or trawling is prohibited or restricted -->
				<pointInstruction>
					<featureReference><xsl:value-of select="@id"/></featureReference>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<xsl:element name="symbol">
						<xsl:attribute name="reference">FSHRES51</xsl:attribute> 
						<areaPlacement placementMode="VisibleParts"/>
					</xsl:element>
				</pointInstruction>

			</xsl:otherwise>  
		</xsl:choose>	

		<xsl:if test="$symbolizedBoundaries = 'true'">
			<!-- boundary of an area where trawling or fishing is prohibited or restricted -->
			<lineInstruction>
				<featureReference><xsl:value-of select="@id"/></featureReference>
				<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
				<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
				<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
				<lineStyleReference reference="FSHRES51"/>
			</lineInstruction>
		</xsl:if>

	</xsl:template>

	<!-- Feb 2015 Named template for defining restricted area portrayal, based on S-52 ed 4 CSP by the same name -->
	<xsl:template name="RESARE04D">
		<xsl:param name="viewingGroup"/>
		<!-- The viewing group -->
		<xsl:param name="displayPlane"/>
		<!-- The display plane -->
		<xsl:param name="drawingPriority"/>
		<!-- The drawing priority -->
		<xsl:param name="symbolizedBoundaries"/>
		<!-- 'true' if symbolized area boundaries are to be drawn -->

		<xsl:choose>
			<xsl:when test=" restriction = '9' or restriction = '10' or restriction = '11' or restriction = '12' or restriction = '15' or restriction = '18' or restriction = '19' or restriction = '20' or restriction = '21' or restriction = '22' ">
				<!-- cautionary area with further information -->
				<pointInstruction>
					<featureReference><xsl:value-of select="@id"/></featureReference>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<xsl:element name="symbol">
						<xsl:attribute name="reference">CTYARE71</xsl:attribute> 
						<areaPlacement placementMode="VisibleParts"/>
					</xsl:element>
				</pointInstruction>
				
			</xsl:when>
			<xsl:when test=" categoryOfRestrictedArea and ( categoryOfRestrictedArea = '4' or categoryOfRestrictedArea = '5' or categoryOfRestrictedArea = '6' or categoryOfRestrictedArea = '7' or categoryOfRestrictedArea = '10' or categoryOfRestrictedArea = '20' or categoryOfRestrictedArea = '22' or categoryOfRestrictedArea = '23' )">
				<!-- cautionary area with further information -->
				<pointInstruction>
					<featureReference><xsl:value-of select="@id"/></featureReference>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<xsl:element name="symbol">
						<xsl:attribute name="reference">CTYARE71</xsl:attribute> 
						<areaPlacement placementMode="VisibleParts"/>
					</xsl:element>
				</pointInstruction>
			</xsl:when>
			<xsl:otherwise>
				<!-- cautionary area (e.g. ferry area) navigate with caution -->
				<pointInstruction>
					<featureReference><xsl:value-of select="@id"/></featureReference>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<xsl:element name="symbol">
						<xsl:attribute name="reference">CTYARE51</xsl:attribute> 
						<areaPlacement placementMode="VisibleParts"/>
					</xsl:element>
				</pointInstruction>

			</xsl:otherwise>  
		</xsl:choose>	

		<xsl:if test="$symbolizedBoundaries = 'true'">
			<!-- boundary of area to be navigated with caution -->
			<lineInstruction>
				<featureReference><xsl:value-of select="@id"/></featureReference>
				<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
				<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
				<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
				<lineStyleReference reference="CTYARE51"/>
			</lineInstruction>
		</xsl:if>

	</xsl:template>

	<!-- Feb 2015 Named template for defining restricted area portrayal, based on S-52 ed 4 CSP by the same name -->
	<xsl:template name="RESARE04E">
		<xsl:param name="viewingGroup"/>
		<!-- The viewing group -->
		<xsl:param name="displayPlane"/>
		<!-- The display plane -->
		<xsl:param name="drawingPriority"/>
		<!-- The drawing priority -->
		<xsl:param name="symbolizedBoundaries"/>
		<!-- 'true' if symbolized area boundaries are to be drawn -->

		<xsl:choose>
			<xsl:when test="not(categoryOfRestrictedArea)">
				<!-- area where fishing or trawling is prohibited or restricted, with other cautions -->
				<pointInstruction>
					<featureReference><xsl:value-of select="@id"/></featureReference>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<xsl:element name="symbol">
						<xsl:attribute name="reference">RSRDEF51</xsl:attribute> 
						<areaPlacement placementMode="VisibleParts"/>
					</xsl:element>
				</pointInstruction>
				
			</xsl:when>
			<xsl:when test=" categoryOfRestrictedArea and ( categoryOfRestrictedArea = '1' or categoryOfRestrictedArea = '8' or categoryOfRestrictedArea = '9' or categoryOfRestrictedArea = '12' or categoryOfRestrictedArea = '14' or categoryOfRestrictedArea = '18' or categoryOfRestrictedArea = '19' or categoryOfRestrictedArea = '21' or categoryOfRestrictedArea = '24' or categoryOfRestrictedArea = '25' or categoryOfRestrictedArea = '26' ) ">
				<!-- area where fishing or trawling is prohibited or restricted, with other cautions -->
				<xsl:choose>
				<xsl:when test=" categoryOfRestrictedArea and ( categoryOfRestrictedArea = '4' or categoryOfRestrictedArea = '5' or categoryOfRestrictedArea = '6' or categoryOfRestrictedArea = '7' or categoryOfRestrictedArea = '10' or categoryOfRestrictedArea = '20' or categoryOfRestrictedArea = '22' or categoryOfRestrictedArea = '23' )">
					<!-- cautionary area with further information -->
					<pointInstruction>
						<featureReference><xsl:value-of select="@id"/></featureReference>
						<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
						<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
						<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
						<xsl:element name="symbol">
							<xsl:attribute name="reference">CTYARE71</xsl:attribute> 
							<areaPlacement placementMode="VisibleParts"/>
						</xsl:element>
					</pointInstruction>
				</xsl:when>
				<xsl:otherwise>
					<!-- cautionary area (e.g. ferry area) navigate with caution -->
					<pointInstruction>
						<featureReference><xsl:value-of select="@id"/></featureReference>
						<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
						<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
						<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
						<xsl:element name="symbol">
							<xsl:attribute name="reference">CTYARE51</xsl:attribute> 
							<areaPlacement placementMode="VisibleParts"/>
						</xsl:element>
					</pointInstruction>

				</xsl:otherwise>  
				</xsl:choose>	
				
			</xsl:when>
			<xsl:when test=" categoryOfRestrictedArea and ( categoryOfRestrictedArea = '4' or categoryOfRestrictedArea = '5' or categoryOfRestrictedArea = '6' or categoryOfRestrictedArea = '7' or categoryOfRestrictedArea = '10' or categoryOfRestrictedArea = '20' or categoryOfRestrictedArea = '22' or categoryOfRestrictedArea = '23' ) ">
				<!-- cautionary area with further information -->
				<pointInstruction>
					<featureReference><xsl:value-of select="@id"/></featureReference>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<xsl:element name="symbol">
						<xsl:attribute name="reference">INFARE51</xsl:attribute> 
						<areaPlacement placementMode="VisibleParts"/>
					</xsl:element>
				</pointInstruction>
			</xsl:when>
			<xsl:otherwise>
				<!-- cautionary area (e.g. ferry area) navigate with caution -->
				<pointInstruction>
					<featureReference><xsl:value-of select="@id"/></featureReference>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<xsl:element name="symbol">
						<xsl:attribute name="reference">RSRDEF51</xsl:attribute> 
						<areaPlacement placementMode="VisibleParts"/>
					</xsl:element>
				</pointInstruction>

			</xsl:otherwise>  
		</xsl:choose>	

		<xsl:if test="$symbolizedBoundaries = 'true'">
			<!-- boundary of area to be navigated with caution -->
			<lineInstruction>
				<featureReference><xsl:value-of select="@id"/></featureReference>
				<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
				<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
				<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
				<lineStyleReference reference="CTYARE51"/>
			</lineInstruction>
		</xsl:if>

	</xsl:template>


</xsl:transform>
