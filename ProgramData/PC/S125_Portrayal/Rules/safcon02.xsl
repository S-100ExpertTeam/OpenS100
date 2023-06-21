<?xml version="1.0" encoding="UTF-8"?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
			<xsl:output method="xml" encoding="UTF-8" indent="yes"/>
	<!-- Sept 2016  -->
	<xsl:template name="SAFCON02">
	<xsl:param name="contour"/> <!-- The contour being annotated -->
	<xsl:param name="curveid"/> <!-- The curve being annotated as safety contour -->
	<xsl:param name="viewingGroup"/> <!-- The viewing group to use for the labels -->
	<xsl:param name="drawingPriority"/> <!-- The priority to use for the labels -->

		<xsl:if test="$contour !='' and $contour/valueOfDepthContour &gt; 0 and  $contour/valueOfDepthContour &lt; 99999">  <!-- if we have a contour and curve -->
			<xsl:variable name="fid" select="$contour/@id"/>
			<xsl:variable name="displayPlane" select="'OVERRADAR'"/>
			<!-- variable to hold integer depth -->
			<xsl:variable name="integerdepthval" select="floor($contour/valueOfDepthContour)"/>
			<!-- variable to hold fractional depth scaled to one digit integer -->
			<xsl:variable name="fractiondepthval" select="floor(10 * ($contour/valueOfDepthContour - $integerdepthval))"/>
			<xsl:variable name="prefix" select="'SAFCON'"/>
			<xsl:choose>
				<xsl:when test="10 > $integerdepthval ">
					<!-- symbols safcon0x + integer, safcon6x + fraction -->
					<pointInstruction>
						<featureReference><xsl:value-of select="$fid"/></featureReference>
						<xsl:if test="$curveid != '' "><spatialReference><xsl:value-of select="$curveid"/></spatialReference></xsl:if>
						<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
						<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
						<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
						<xsl:element name="symbol">
							<xsl:attribute name="reference"><xsl:value-of select="concat($prefix,'0',$integerdepthval)"/></xsl:attribute> 
							<xsl:element name="linePlacement">
								<xsl:attribute name="placementMode">Relative</xsl:attribute> 
								<xsl:element name="offset">0.5</xsl:element>
							</xsl:element>
						</xsl:element>
					</pointInstruction>
					<xsl:if test="$fractiondepthval > 0 ">
						<pointInstruction>
							<featureReference><xsl:value-of select="$fid"/></featureReference>
							<xsl:if test="$curveid != '' "><spatialReference><xsl:value-of select="$curveid"/></spatialReference></xsl:if>
							<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
							<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
							<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
							<xsl:element name="symbol">
								<xsl:attribute name="reference"><xsl:value-of select="concat($prefix,'6',$fractiondepthval)"/></xsl:attribute> 
							<xsl:element name="linePlacement">
								<xsl:attribute name="placementMode">Relative</xsl:attribute> 
								<xsl:element name="offset">0.5</xsl:element>
							</xsl:element>
							</xsl:element>
						</pointInstruction>
					</xsl:if>
				</xsl:when>
				<xsl:when test="31 > $integerdepthval and $fractiondepthval > 0 ">
					<!-- symbols safcon2x + 10s digit , safcon1x + 1s digit, safcon5x + fraction -->
					<pointInstruction>
						<featureReference><xsl:value-of select="$fid"/></featureReference>
						<xsl:if test="$curveid != '' "><spatialReference><xsl:value-of select="$curveid"/></spatialReference></xsl:if>
						<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
						<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
						<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
						<xsl:element name="symbol">
							<xsl:attribute name="reference"><xsl:value-of select="concat($prefix,'2',substring($integerdepthval,1,1))"/></xsl:attribute> 
							<xsl:element name="linePlacement">
								<xsl:attribute name="placementMode">Relative</xsl:attribute> 
								<xsl:element name="offset">0.5</xsl:element>
							</xsl:element>
						</xsl:element>
					</pointInstruction>
					<pointInstruction>
						<featureReference><xsl:value-of select="$fid"/></featureReference>
						<xsl:if test="$curveid != '' "><spatialReference><xsl:value-of select="$curveid"/></spatialReference></xsl:if>
						<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
						<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
						<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
						<xsl:element name="symbol">
							<xsl:attribute name="reference"><xsl:value-of select="concat($prefix,'1',substring($integerdepthval,2,1))"/></xsl:attribute> 
							<xsl:element name="linePlacement">
								<xsl:attribute name="placementMode">Relative</xsl:attribute> 
								<xsl:element name="offset">0.5</xsl:element>
							</xsl:element>
						</xsl:element>
					</pointInstruction>
					<pointInstruction>
						<featureReference><xsl:value-of select="$fid"/></featureReference>
						<xsl:if test="$curveid != '' "><spatialReference><xsl:value-of select="$curveid"/></spatialReference></xsl:if>
						<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
						<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
						<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
						<xsl:element name="symbol">
							<xsl:attribute name="reference"><xsl:value-of select="concat($prefix,'5',$fractiondepthval)"/></xsl:attribute> 
							<xsl:element name="linePlacement">
								<xsl:attribute name="placementMode">Relative</xsl:attribute> 
								<xsl:element name="offset">0.5</xsl:element>
							</xsl:element>
						</xsl:element>
					</pointInstruction>
				</xsl:when>
				<xsl:when test="100 > $integerdepthval ">
					<!-- symbols safcon2x + 10s digit , safcon1x + 1s digit -->
					<pointInstruction>
						<featureReference><xsl:value-of select="$fid"/></featureReference>
						<xsl:if test="$curveid != '' "><spatialReference><xsl:value-of select="$curveid"/></spatialReference></xsl:if>
						<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
						<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
						<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
						<xsl:element name="symbol">
							<xsl:attribute name="reference"><xsl:value-of select="concat($prefix,'2',substring($integerdepthval,1,1))"/></xsl:attribute> 
							<xsl:element name="linePlacement">
								<xsl:attribute name="placementMode">Relative</xsl:attribute> 
								<xsl:element name="offset">0.5</xsl:element>
							</xsl:element>
						</xsl:element>
					</pointInstruction>
					<pointInstruction>
						<featureReference><xsl:value-of select="$fid"/></featureReference>
						<xsl:if test="$curveid != '' "><spatialReference><xsl:value-of select="$curveid"/></spatialReference></xsl:if>
						<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
						<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
						<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
						<xsl:element name="symbol">
							<xsl:attribute name="reference"><xsl:value-of select="concat($prefix,'1',substring($integerdepthval,2,1))"/></xsl:attribute> 
							<xsl:element name="linePlacement">
								<xsl:attribute name="placementMode">Relative</xsl:attribute> 
								<xsl:element name="offset">0.5</xsl:element>
							</xsl:element>
						</xsl:element>
					</pointInstruction>
				</xsl:when>
				<xsl:when test="1000 > $integerdepthval ">
					<!-- symbols safcon8x + 100s digit ,safcon0x + 10s digit , safcon9x + 1s digit -->
					<pointInstruction>
						<featureReference><xsl:value-of select="$fid"/></featureReference>
						<xsl:if test="$curveid != '' "><spatialReference><xsl:value-of select="$curveid"/></spatialReference></xsl:if>
						<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
						<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
						<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
						<xsl:element name="symbol">
							<xsl:attribute name="reference"><xsl:value-of select="concat($prefix,'8',substring($integerdepthval,1,1))"/></xsl:attribute> 
							<xsl:element name="linePlacement">
								<xsl:attribute name="placementMode">Relative</xsl:attribute> 
								<xsl:element name="offset">0.5</xsl:element>
							</xsl:element>
						</xsl:element>
					</pointInstruction>
					<pointInstruction>
						<featureReference><xsl:value-of select="$fid"/></featureReference>
						<xsl:if test="$curveid != '' "><spatialReference><xsl:value-of select="$curveid"/></spatialReference></xsl:if>
						<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
						<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
						<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
						<xsl:element name="symbol">
							<xsl:attribute name="reference"><xsl:value-of select="concat($prefix,'0',substring($integerdepthval,2,1))"/></xsl:attribute> 
							<xsl:element name="linePlacement">
								<xsl:attribute name="placementMode">Relative</xsl:attribute> 
								<xsl:element name="offset">0.5</xsl:element>
							</xsl:element>
						</xsl:element>
					</pointInstruction>
					<pointInstruction>
						<featureReference><xsl:value-of select="$fid"/></featureReference>
						<xsl:if test="$curveid != '' "><spatialReference><xsl:value-of select="$curveid"/></spatialReference></xsl:if>
						<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
						<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
						<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
						<xsl:element name="symbol">
							<xsl:attribute name="reference"><xsl:value-of select="concat($prefix,'9',substring($integerdepthval,3,1))"/></xsl:attribute> 
							<xsl:element name="linePlacement">
								<xsl:attribute name="placementMode">Relative</xsl:attribute> 
								<xsl:element name="offset">0.5</xsl:element>
							</xsl:element>
						</xsl:element>
					</pointInstruction>
				</xsl:when>
				<xsl:when test="10000 > $integerdepthval ">
					<!-- symbols safcon3x + 1000s digit ,safcon2x + 100s digit ,safcon1x + 10s digit , safcon7x + 1s digit -->
					<pointInstruction>
						<featureReference><xsl:value-of select="$fid"/></featureReference>
						<xsl:if test="$curveid != '' "><spatialReference><xsl:value-of select="$curveid"/></spatialReference></xsl:if>
						<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
						<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
						<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
						<xsl:element name="symbol">
							<xsl:attribute name="reference"><xsl:value-of select="concat($prefix,'3',substring($integerdepthval,1,1))"/></xsl:attribute> 
							<xsl:element name="linePlacement">
								<xsl:attribute name="placementMode">Relative</xsl:attribute> 
								<xsl:element name="offset">0.5</xsl:element>
							</xsl:element>
						</xsl:element>
					</pointInstruction>
					<pointInstruction>
						<featureReference><xsl:value-of select="$fid"/></featureReference>
						<xsl:if test="$curveid != '' "><spatialReference><xsl:value-of select="$curveid"/></spatialReference></xsl:if>
						<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
						<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
						<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
						<xsl:element name="symbol">
							<xsl:attribute name="reference"><xsl:value-of select="concat($prefix,'2',substring($integerdepthval,2,1))"/></xsl:attribute> 
							<xsl:element name="linePlacement">
								<xsl:attribute name="placementMode">Relative</xsl:attribute> 
								<xsl:element name="offset">0.5</xsl:element>
							</xsl:element>
						</xsl:element>
					</pointInstruction>
					<pointInstruction>
						<featureReference><xsl:value-of select="$fid"/></featureReference>
						<xsl:if test="$curveid != '' "><spatialReference><xsl:value-of select="$curveid"/></spatialReference></xsl:if>
						<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
						<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
						<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
						<xsl:element name="symbol">
							<xsl:attribute name="reference"><xsl:value-of select="concat($prefix,'1',substring($integerdepthval,3,1))"/></xsl:attribute> 
							<xsl:element name="linePlacement">
								<xsl:attribute name="placementMode">Relative</xsl:attribute> 
								<xsl:element name="offset">0.5</xsl:element>
							</xsl:element>
						</xsl:element>
					</pointInstruction>
					<pointInstruction>
						<featureReference><xsl:value-of select="$fid"/></featureReference>
						<xsl:if test="$curveid != '' "><spatialReference><xsl:value-of select="$curveid"/></spatialReference></xsl:if>
						<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
						<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
						<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
						<xsl:element name="symbol">
							<xsl:attribute name="reference"><xsl:value-of select="concat($prefix,'7',substring($integerdepthval,4,1))"/></xsl:attribute> 
							<xsl:element name="linePlacement">
								<xsl:attribute name="placementMode">Relative</xsl:attribute> 
								<xsl:element name="offset">0.5</xsl:element>
							</xsl:element>
						</xsl:element>
					</pointInstruction>
				</xsl:when>
				<xsl:when test="100000 > $integerdepthval ">
					<!-- symbols safcon4x + 10000s digit safcon3x + 1000s digit ,safcon2x + 100s digit ,safcon1x + 10s digit , safcon7x + 1s digit -->
					<pointInstruction>
						<featureReference><xsl:value-of select="$fid"/></featureReference>
						<xsl:if test="$curveid != '' "><spatialReference><xsl:value-of select="$curveid"/></spatialReference></xsl:if>
						<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
						<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
						<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
						<xsl:element name="symbol">
							<xsl:attribute name="reference"><xsl:value-of select="concat($prefix,'4',substring($integerdepthval,1,1))"/></xsl:attribute> 
							<xsl:element name="linePlacement">
								<xsl:attribute name="placementMode">Relative</xsl:attribute> 
								<xsl:element name="offset">0.5</xsl:element>
							</xsl:element>
						</xsl:element>
					</pointInstruction>
					<pointInstruction>
						<featureReference><xsl:value-of select="$fid"/></featureReference>
						<xsl:if test="$curveid != '' "><spatialReference><xsl:value-of select="$curveid"/></spatialReference></xsl:if>
						<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
						<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
						<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
						<xsl:element name="symbol">
							<xsl:attribute name="reference"><xsl:value-of select="concat($prefix,'3',substring($integerdepthval,2,1))"/></xsl:attribute> 
							<xsl:element name="linePlacement">
								<xsl:attribute name="placementMode">Relative</xsl:attribute> 
								<xsl:element name="offset">0.5</xsl:element>
							</xsl:element>
						</xsl:element>
					</pointInstruction>
					<pointInstruction>
						<featureReference><xsl:value-of select="$fid"/></featureReference>
						<xsl:if test="$curveid != '' "><spatialReference><xsl:value-of select="$curveid"/></spatialReference></xsl:if>
						<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
						<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
						<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
						<xsl:element name="symbol">
							<xsl:attribute name="reference"><xsl:value-of select="concat($prefix,'2',substring($integerdepthval,3,1))"/></xsl:attribute> 
							<xsl:element name="linePlacement">
								<xsl:attribute name="placementMode">Relative</xsl:attribute> 
								<xsl:element name="offset">0.5</xsl:element>
							</xsl:element>
						</xsl:element>
					</pointInstruction>
					<pointInstruction>
						<featureReference><xsl:value-of select="$fid"/></featureReference>
						<xsl:if test="$curveid != '' "><spatialReference><xsl:value-of select="$curveid"/></spatialReference></xsl:if>
						<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
						<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
						<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
						<xsl:element name="symbol">
							<xsl:attribute name="reference"><xsl:value-of select="concat($prefix,'1',substring($integerdepthval,4,1))"/></xsl:attribute> 
							<xsl:element name="linePlacement">
								<xsl:attribute name="placementMode">Relative</xsl:attribute> 
								<xsl:element name="offset">0.5</xsl:element>
							</xsl:element>
						</xsl:element>
					</pointInstruction>
					<pointInstruction>
						<featureReference><xsl:value-of select="$fid"/></featureReference>
						<xsl:if test="$curveid != '' "><spatialReference><xsl:value-of select="$curveid"/></spatialReference></xsl:if>
						<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
						<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
						<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
						<xsl:element name="symbol">
							<xsl:attribute name="reference"><xsl:value-of select="concat($prefix,'7',substring($integerdepthval,5,1))"/></xsl:attribute> 
							<xsl:element name="linePlacement">
								<xsl:attribute name="placementMode">Relative</xsl:attribute> 
								<xsl:element name="offset">0.5</xsl:element>
							</xsl:element>
						</xsl:element>
					</pointInstruction>
				</xsl:when>
				<xsl:otherwise>
				</xsl:otherwise>
			</xsl:choose>

		</xsl:if>		
	</xsl:template>
</xsl:transform>
