<?xml version="1.0" encoding="UTF-8"?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
    <xsl:output method="xml" encoding="UTF-8" indent="yes"/>

	<!-- create key indexes to quickly lookup points, these could be moved to a common location if desired but can be accessed from anywhere -->	
	<xsl:key name="pointids" match="/Dataset/Points/Point" use="@id"/>
	
	<!--Sept 2016, first implementation -->
	<xsl:template name="QUAPNT02">
	<xsl:param name="viewingGroup"/> <!-- The viewing group -->
	<xsl:param name="displayPlane"/> <!-- The display plane -->
	<xsl:param name="drawingPriority"/> <!-- The drawing priority -->

	    <xsl:variable name="fid" select="@id"/>

		<!-- use for-each to set the context to the Point reference-->
		<xsl:for-each select="Point">
			<!-- use for-each to set the context to the Point record -->
			<xsl:for-each select="key('pointids', @ref)">
				<!-- use for-each to set the context to the spatialQuality record -->
				<xsl:for-each select="key('qualityids', spatialQuality/@informationRef)">
					<xsl:if test="not(qualityOfPosition = 1 or qualityOfPosition = 10 or qualityOfPosition = 11) ">
					  <pointInstruction>
						<featureReference>
							<xsl:value-of select="$fid"/>
						</featureReference>
						<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
						<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
						<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
						<symbol reference="LOWACC01"/>
					  </pointInstruction>
					</xsl:if>

				</xsl:for-each> 
			</xsl:for-each> 
		</xsl:for-each> 
	
    </xsl:template>
</xsl:transform>
