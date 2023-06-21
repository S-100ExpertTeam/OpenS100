<?xml version="1.0" encoding="UTF-8"?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
    <xsl:output method="xml" encoding="UTF-8" indent="yes"/>

	<!-- updated Dec 15, 2016 typo with missing linestyle -->
	
	<!-- create key indexes to quickly lookup curves, these could be moved to a common location if desired but can be accessed from anywhere -->	
	<xsl:key name="Curveids" match="/Dataset/Curves/Curve" use="@id"/>
	<xsl:key name="CompositeCurveids" match="/Dataset/CompositeCurves/CompositeCurve" use="@id"/>
	<xsl:key name="qualityids" match="/Dataset/InformationTypes/SpatialQuality" use="@id"/>
	
    <!--Template for: Coastline and land area boundaries and shoreline constructions also other lines and areas -->
        <!--<xsl:call-template name="QUALIN03">
            <xsl:with-param name="viewingGroup">12410</xsl:with-param>
            <xsl:with-param name="displayPlane">OVERRADAR</xsl:with-param>
            <xsl:with-param name="drawingPriority">22</xsl:with-param>
        </xsl:call-template>-->
<!--Sept 2016,  coastline with radar conspicuous is drawn by a coastline template before calling this to add low accuracy coast -->
	<xsl:template name="QUALIN03">
	<xsl:param name="viewingGroup"/> <!-- The viewing group -->
	<xsl:param name="displayPlane"/> <!-- The display plane -->
	<xsl:param name="drawingPriority"/> <!-- The drawing priority -->
	<xsl:param name="lineStyle" select="LOWACC21" /> <!-- The linestyle for low quality curves, default LOWACC21  -->

    <xsl:variable name="fid" select="@id"/>
	
	<!-- for each curve apply templates with mode=QUALIN03.  The composite curve template will apply the templates to it's children using the same mode
	the simple curve template will test the related quality and if not high will issue the low accuracy linestyle.
	The feature id, viewing group and priority can be passed as parameters to these templates.-->
		<xsl:for-each select="Curve">
			<xsl:apply-templates select="key('Curveids', @ref)" mode="QUALIN03" >
				<xsl:with-param name="fid" select="$fid" />
				<xsl:with-param name="viewingGroup" select="$viewingGroup" />
				<xsl:with-param name="displayPlane" select="$displayPlane" />
				<xsl:with-param name="drawingPriority" select="$drawingPriority" />
				<xsl:with-param name="orient" select="@orientation" />
				<xsl:with-param name="lineStyle" select="$lineStyle" />
			</xsl:apply-templates>
		</xsl:for-each> 
		<xsl:for-each select="CompositeCurve">
			<xsl:apply-templates select="key('CompositeCurveids', @ref)" mode="QUALIN03" >
				<xsl:with-param name="fid" select="$fid" />
				<xsl:with-param name="viewingGroup" select="$viewingGroup" />
				<xsl:with-param name="displayPlane" select="$displayPlane" />
				<xsl:with-param name="drawingPriority" select="$drawingPriority" />
				<xsl:with-param name="orient" select="@orientation" />
			</xsl:apply-templates>
		</xsl:for-each> 
		<xsl:for-each select="Surface">
			<xsl:for-each select="key('surfaces', @ref)">
				<xsl:for-each select="Ring/Curve">
					<xsl:apply-templates select="key('Curveids', @ref)" mode="QUALIN03" >
						<xsl:with-param name="fid" select="$fid" />
						<xsl:with-param name="viewingGroup" select="$viewingGroup" />
						<xsl:with-param name="displayPlane" select="$displayPlane" />
						<xsl:with-param name="drawingPriority" select="$drawingPriority" />
						<xsl:with-param name="orient" select="@orientation" />
					</xsl:apply-templates>
				</xsl:for-each> 
				<xsl:for-each select="Ring/CompositeCurve">
					<xsl:apply-templates select="key('CompositeCurveids', @ref)" mode="QUALIN03" >
						<xsl:with-param name="fid" select="$fid" />
						<xsl:with-param name="viewingGroup" select="$viewingGroup" />
						<xsl:with-param name="displayPlane" select="$displayPlane" />
						<xsl:with-param name="drawingPriority" select="$drawingPriority" />
						<xsl:with-param name="orient" select="@orientation" />
					</xsl:apply-templates>
				</xsl:for-each> 
			</xsl:for-each> 
		</xsl:for-each> 

    </xsl:template>

	<!--  template to match a curve when testing quality of line position -->	
	<xsl:template match="Curves/Curve" mode="QUALIN03">
		<xsl:param name="fid" />	
		<xsl:param name="viewingGroup" />	
		<xsl:param name="displayPlane" />	
		<xsl:param name="drawingPriority" />	
		<xsl:param name="orient" />	
		<xsl:param name="lineStyle" />
		
			<!--  get a reference to the associated spatial quality object from the index -->
			<xsl:variable name="quality" select="key('qualityids', spatialQuality/@informationRef)"/>
			<!--  need to translate the orientation into a boolean forward value -->
			<xsl:variable name="forward">
				<xsl:choose>
					<xsl:when test="$orient = 'Forward' ">		
						<xsl:value-of select="'true'" />
					</xsl:when>
					<xsl:otherwise>		
						<xsl:value-of select="'false'" />
					</xsl:otherwise>
				</xsl:choose>
			</xsl:variable>
			<xsl:choose>
				<!--  when there is no quality indicator or the quality is good draw 1 pixel CSTLN line for this curve -->
				<xsl:when test="not($quality/qualityOfPosition) or $quality/qualityOfPosition = 1 or $quality/qualityOfPosition = 10 or $quality/qualityOfPosition = 11 ">		
				</xsl:when>
				<!--  otherwise when the quality is not good use the low quality linestyle -->
				<xsl:otherwise>		
					<lineInstruction>
						<featureReference><xsl:value-of select="$fid"/></featureReference>
						<xsl:element name="spatialReference">
							<xsl:attribute name="forward"><xsl:value-of select="$forward"/></xsl:attribute>
							<xsl:value-of select="@id"/>
						</xsl:element>
						<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
						<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
						<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
						<xsl:element name="lineStyleReference">
							<xsl:attribute name="reference"><xsl:value-of select="$lineStyle"/></xsl:attribute>
						</xsl:element>
					</lineInstruction>
				</xsl:otherwise>
			</xsl:choose>

    </xsl:template>

	<!--  Template for composite curves, could be deleted if presentation input is simplified with only simple curves -->
	<xsl:template match="CompositeCurves/CompositeCurve" mode="QUALIN03">
		<xsl:param name="fid" />	
		<xsl:param name="viewingGroup" />	
		<xsl:param name="displayPlane" />	
		<xsl:param name="drawingPriority" />	
		<xsl:param name="orient" />	
		<!--  loop over the simple curve references -->
		<xsl:for-each select="Curve">
			<xsl:variable name="orientation">
				<xsl:choose>
					<xsl:when test="$orient = 'Forward' ">		
						<xsl:value-of select="@orientation" />
					</xsl:when>
					<xsl:when test="$orient = 'Reverse' and @orientation = 'Forward' ">		
						<xsl:value-of select="$orient" />
					</xsl:when>
					<xsl:otherwise>		
						<xsl:value-of select="'Forward'" />
					</xsl:otherwise>
				</xsl:choose>
			</xsl:variable>
			<!--  apply the simple curve template (above) with mode QUALIN03 -->
			<xsl:apply-templates select="key('Curveids', @ref)" mode="QUALIN03" >
				<xsl:with-param name="fid" select="$fid" />
				<xsl:with-param name="viewingGroup" select="$viewingGroup" />
				<xsl:with-param name="displayPlane" select="$displayPlane" />
				<xsl:with-param name="drawingPriority" select="$drawingPriority" />
				<xsl:with-param name="orient" select="$orientation" />
			</xsl:apply-templates>
		</xsl:for-each> 
		<!--  loop over the CompositeCurve references -->
		<xsl:for-each select="CompositeCurve">
			<xsl:variable name="orientation">
				<xsl:choose>
					<xsl:when test="$orient = 'Forward' ">		
						<xsl:value-of select="@orientation" />
					</xsl:when>
					<xsl:when test="$orient = 'Reverse' and @orientation = 'Forward' ">		
						<xsl:value-of select="$orient" />
					</xsl:when>
					<xsl:otherwise>		
						<xsl:value-of select="'Forward'" />
					</xsl:otherwise>
				</xsl:choose>
			</xsl:variable>
			<!--  recursively call this same template -->
			<xsl:apply-templates select="key('CompositeCurveids', @ref)" mode="QUALIN03" >
				<xsl:with-param name="fid" select="$fid" />
				<xsl:with-param name="viewingGroup" select="$viewingGroup" />
				<xsl:with-param name="displayPlane" select="$displayPlane" />
				<xsl:with-param name="drawingPriority" select="$drawingPriority" />
				<xsl:with-param name="orient" select="$orientation" />
			</xsl:apply-templates>
		</xsl:for-each> 

    </xsl:template>
</xsl:transform>
