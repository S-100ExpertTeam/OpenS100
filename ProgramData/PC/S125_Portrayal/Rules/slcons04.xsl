<?xml version="1.0" encoding="UTF-8"?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
			<xsl:output method="xml" encoding="UTF-8" indent="yes"/>
	<!-- Jan 29, 2015 Named template for shoreline construction portrayal -->
	<xsl:template name="SLCONS04">
	<xsl:param name="viewingGroup"/> <!-- The viewing group -->
	<xsl:param name="displayPlane"/> <!-- The display plane -->
	<xsl:param name="drawingPriority"/> <!-- The drawing priority -->

		<xsl:choose>
			<xsl:when test="condition = '1' or condition = '2' ">
				<!-- under construction or ruined LS(DASH,1,CSTLN)-->
				<lineInstruction>
					<featureReference><xsl:value-of select="@id"/></featureReference>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					 <xsl:call-template name="simpleLineStyle">
						<xsl:with-param name="style">dash</xsl:with-param>
						<xsl:with-param name="width">0.32</xsl:with-param>
						<xsl:with-param name="colour">CSTLN</xsl:with-param>
					 </xsl:call-template>
				</lineInstruction>
			</xsl:when>
			<xsl:when test="categoryOfShorelineConstruction = '6' or categoryOfShorelineConstruction = '15' or categoryOfShorelineConstruction = '16' ">
				<!-- wharf LS(SOLD,4,CSTLN)-->
				<lineInstruction>
					<featureReference><xsl:value-of select="@id"/></featureReference>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					 <xsl:call-template name="simpleLineStyle">
						<xsl:with-param name="style">solid</xsl:with-param>
						<xsl:with-param name="width">0.64</xsl:with-param>
						<xsl:with-param name="colour">CSTLN</xsl:with-param>
					 </xsl:call-template>
				</lineInstruction>
			</xsl:when>
			<xsl:when test="waterLevelEffect = '3' or waterLevelEffect = '4' ">
				<!-- always underwater or covers and uncovers LS(DASH,2,CSTLN -->
				<lineInstruction>
					<featureReference><xsl:value-of select="@id"/></featureReference>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					 <xsl:call-template name="simpleLineStyle">
						<xsl:with-param name="style">dash</xsl:with-param>
						<xsl:with-param name="width">0.64</xsl:with-param>
						<xsl:with-param name="colour">CSTLN</xsl:with-param>
					 </xsl:call-template>
				</lineInstruction>
			</xsl:when>
			<xsl:otherwise>
				<!-- default LS(SOLD,2,CSTLN) -->
				<lineInstruction>
					<featureReference><xsl:value-of select="@id"/></featureReference>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					 <xsl:call-template name="simpleLineStyle">
						<xsl:with-param name="style">solid</xsl:with-param>
						<xsl:with-param name="width">0.64</xsl:with-param>
						<xsl:with-param name="colour">CSTLN</xsl:with-param>
					 </xsl:call-template>
				</lineInstruction>
			</xsl:otherwise>
		</xsl:choose>
		
		<!-- call the template to check line quality of each component curve and override linestyle with low accuracy if needed -->
		<xsl:call-template name="QUALIN03">
			<xsl:with-param name="viewingGroup" select="$viewingGroup"/>
			<xsl:with-param name="displayPlane" select="$displayPlane"/>
			<xsl:with-param name="drawingPriority" select="$drawingPriority + 1"/>
		</xsl:call-template>

	</xsl:template>
			
</xsl:transform>
