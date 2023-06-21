<?xml version="1.0" encoding="UTF-8"?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
			<xsl:output method="xml" encoding="UTF-8" indent="yes"/>
	<!-- Sept 2016 handle point or multipoint soundings	-->
	
	<!-- create key indexes to quickly lookup MultiPoints, these could be moved to a common location if desired but can be accessed from anywhere -->	
	<xsl:key name="MultiPointids" match="/Dataset/MultiPoints/MultiPoint" use="@id"/>

	<xsl:template name="SOUNDG02">
	<xsl:param name="viewingGroup"/> <!-- The viewing group -->
	<xsl:param name="displayPlane"/> <!-- The display plane -->
	<xsl:param name="drawingPriority"/> <!-- The drawing priority -->

	<xsl:variable name="fid" select="@id"/>

	<xsl:if test="Point">
		<xsl:variable name="point" select="key('pointids', ./Point/@ref)"/>
		<xsl:variable name="zval">
			<xsl:value-of select="$point/Coordinate3D/z"/>
		</xsl:variable>
		<xsl:variable name="iref">
			<xsl:value-of select="$point/spatialQuality/@informationRef"/>
		</xsl:variable>
		<xsl:variable name="qpos">
			<xsl:if test="$iref !='' ">
				<xsl:value-of select="key('qualityids', $iref)/qualityOfPosition"/>
			</xsl:if>
		</xsl:variable>
		<xsl:call-template name="SNDFRM04">
			<xsl:with-param name="viewingGroup" select="$viewingGroup"/>
			<xsl:with-param name="displayPlane" select="$displayPlane"/>
			<xsl:with-param name="drawingPriority" select="$drawingPriority"/>
			<xsl:with-param name="fid" select="@id"/>
			<xsl:with-param name="sid" select="Point/@ref"/>
			<xsl:with-param name="depth" select="$zval"/>
			<xsl:with-param name="tecsou" select="techniqueOfVerticalMeasurement"/>
			<xsl:with-param name="quasou" select="qualityOfSoundingMeasurement"/>
			<xsl:with-param name="status" select="status"/>
			<xsl:with-param name="quapos" select="$qpos"/>
		</xsl:call-template>
	</xsl:if>
	<xsl:if test="MultiPoint">
		<xsl:variable name="tecsou" select="techniqueOfVerticalMeasurement"/>
		<xsl:variable name="quasou" select="qualityOfSoundingMeasurement"/>
		<xsl:variable name="status" select="status"/>

		<xsl:for-each select="MultiPoint">
			<xsl:variable name="mpoint" select="key('MultiPointids', @ref)"/>
			<xsl:variable name="iref">
				<xsl:value-of select="$mpoint/spatialQuality/@informationRef"/>
			</xsl:variable>
			<xsl:variable name="qpos">
				<xsl:if test="$iref !='' ">
					<xsl:value-of select="key('qualityids', $iref)/qualityOfPosition"/>
				</xsl:if>
			</xsl:variable>
			<xsl:for-each select="$mpoint/Coordinate3D">
				<xsl:call-template name="SNDFRM04">
					<xsl:with-param name="viewingGroup" select="$viewingGroup"/>
					<xsl:with-param name="displayPlane" select="$displayPlane"/>
					<xsl:with-param name="drawingPriority" select="$drawingPriority"/>
					<xsl:with-param name="fid" select="$fid"/>
					<xsl:with-param name="sid" select="$mpoint/@id"/>
					<xsl:with-param name="depth" select="z"/>
					<xsl:with-param name="tecsou" select="$tecsou"/>
					<xsl:with-param name="quasou" select="$quasou"/>
					<xsl:with-param name="status" select="$status"/>
					<xsl:with-param name="quapos" select="$qpos"/>
					<xsl:with-param name="x" select="x"/>
					<xsl:with-param name="y" select="y"/>
				</xsl:call-template>
			</xsl:for-each> 
		</xsl:for-each> 
	</xsl:if>

	</xsl:template>
			
</xsl:transform>
