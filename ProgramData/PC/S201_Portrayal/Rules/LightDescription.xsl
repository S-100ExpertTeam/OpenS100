<?xml version="1.0" encoding="UTF-8"?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
    <xsl:output method="xml" encoding="UTF-8" indent="yes"/>

	<!-- Template to output the text for light -->
	<!-- original draft March 16, 2015 -->
	<!-- Updated May 30, 2016 to accept parameters to work in context other than LIGHTS -->
	<xsl:template name="LIGHTDESCRIPTION">
	<xsl:param name="catLight" select="categoryOfLight"/>
	<xsl:param name="lightStatus" select="status"/>
	<xsl:param name="colour" select="colour"/>
	<xsl:param name="signalGroup" select="rhythmOfLight/signalGroup"/>
	<xsl:param name="signalPeriod" select="rhythmOfLight/signalPeriod"/>
	<xsl:param name="height" select="height"/>
	<xsl:param name="nominalRange" select="valueOfNominalRange"/>
	<xsl:param name="lightChar" select="rhythmOfLight/lightCharacteristic"/>
	<xsl:param name="fid" select="@id"/>
	<xsl:param name="flareAt45" select="'false'"/>
	
		<!-- Determine abbreviation for Category of Light -->
		<xsl:variable name="CATLIT">
			<xsl:choose>
				<xsl:when test="$catLight = '1' ">
					<xsl:text>Dir </xsl:text>
				</xsl:when>
				<xsl:when test="$catLight = '5' ">
					<xsl:text>Aero </xsl:text>
				</xsl:when>
				<xsl:when test="$catLight = '7' ">
					<xsl:text>Fog Det Lt </xsl:text>
				</xsl:when>
				<xsl:otherwise>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<!-- Determine abbreviation for Status -->
		<xsl:variable name="STATUS">
			<xsl:choose>
				<xsl:when test="$lightStatus = '2' ">
					<xsl:text> (occas)</xsl:text>
				</xsl:when>
				<xsl:when test="$lightStatus = '7' ">
					<xsl:text> (temp)</xsl:text>
				</xsl:when>
				<xsl:when test="$lightStatus = '8' ">
					<xsl:text> (priv)</xsl:text>
				</xsl:when>
				<xsl:when test="$lightStatus = '11' ">
					<xsl:text> (exting)</xsl:text>
				</xsl:when>
				<xsl:when test="$lightStatus = '17' ">
					<xsl:text> (U)</xsl:text>
				</xsl:when>
				<xsl:otherwise>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		
		<!-- Determine abbreviation for colour -->
		<xsl:variable name="COLOUR">
			<xsl:choose>
				<xsl:when test="$colour = '1' ">
					<xsl:text> W </xsl:text>
				</xsl:when>
				<xsl:when test="$colour = '2'">
					<xsl:text> B </xsl:text>
				</xsl:when>
				<xsl:when test="$colour = '3'">
					<xsl:text> R </xsl:text>
				</xsl:when>	
				<xsl:when test="$colour = '4'">
					<xsl:text> G </xsl:text>
				</xsl:when>	
				<xsl:when test="$colour = '5'">
					<xsl:text> Bu </xsl:text>
				</xsl:when>	
				<xsl:when test="$colour = '6'">
					<xsl:text> Y </xsl:text>
				</xsl:when>	
				<xsl:when test="$colour = '7'">
					<xsl:text> Gy </xsl:text>
				</xsl:when>	
				<xsl:when test="$colour = '8'">
					<xsl:text> Br </xsl:text>
				</xsl:when>	
				<xsl:when test="$colour= '9'">
					<xsl:text> Y </xsl:text>
				</xsl:when>
				<xsl:when test="$colour= '10'">
					<xsl:text> Vi </xsl:text>
				</xsl:when>
				<xsl:when test="$colour = '11'">
					<xsl:text> Y </xsl:text>
				</xsl:when>
				<xsl:when test="$colour= '12'">
					<xsl:text> Mag </xsl:text>
				</xsl:when>
				<xsl:when test="$colour = '13'">
					<xsl:text> R </xsl:text>
				</xsl:when>
				<xsl:otherwise>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<!-- Determine signal Group-->
		<xsl:variable name="SIGGRP">
			<xsl:choose>
				<xsl:when test="$signalGroup = '' ">
				</xsl:when>
				<xsl:when test="$signalGroup = '()' ">
				</xsl:when>
				<xsl:when test="$signalGroup = '(1)' ">
				</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select="$signalGroup"/>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		
		<!-- Determine signal period -->
		<xsl:variable name="SIGPER">
			<xsl:choose>
				<xsl:when test="$signalPeriod">
					<xsl:value-of select="$signalPeriod"/><xsl:text>s</xsl:text>
				</xsl:when>
				<xsl:otherwise>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		
		<!-- Determine height -->
		<xsl:variable name="HEIGHT">
			<xsl:choose>
				<xsl:when test="$height">
					<xsl:value-of select="$height"/><xsl:text>m</xsl:text>
				</xsl:when>
				<xsl:otherwise>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		
		<!-- Determine Value of Nominal Range -->
		<xsl:variable name="VALNMR">
			<xsl:choose>
				<xsl:when test="$nominalRange">
					<xsl:value-of select="$nominalRange"/><xsl:text>M</xsl:text>
				</xsl:when>
				<xsl:otherwise>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<!-- Determine abbreviation for Light Characteristics -->
		<xsl:variable name="LITCHR">
			<xsl:choose>
				<xsl:when test="$lightChar = '1' ">
					<xsl:text>F</xsl:text>
				</xsl:when>
				<xsl:when test="$lightChar = '2' ">
					<xsl:text>Fl</xsl:text>
				</xsl:when>
				<xsl:when test="$lightChar = '3' ">
					<xsl:text>LFI</xsl:text>
				</xsl:when>
				<xsl:when test="$lightChar = '4' ">
					<xsl:text>Q</xsl:text>
				</xsl:when>
				<xsl:when test="$lightChar = '5' ">
					<xsl:text>VQ</xsl:text>
				</xsl:when>
				<xsl:when test="$lightChar = '6' ">
					<xsl:text>UQ</xsl:text>
				</xsl:when>
				<xsl:when test="$lightChar = '7' ">
					<xsl:text>Iso</xsl:text>
				</xsl:when>
				<xsl:when test="$lightChar = '8' ">
					<xsl:text>Oc</xsl:text>
				</xsl:when>
				<xsl:when test="$lightChar = '9' ">
					<xsl:text>IQ</xsl:text>
				</xsl:when>
				<xsl:when test="$lightChar = '10' ">
					<xsl:text>IVQ</xsl:text>
				</xsl:when>
				<xsl:when test="$lightChar = '11' ">
					<xsl:text>IUQ</xsl:text>
				</xsl:when>
				<xsl:when test="$lightChar = '12' ">
					<xsl:text>Mo</xsl:text>
				</xsl:when>
				<xsl:when test="$lightChar = '13' ">
					<xsl:text>FFl</xsl:text>
				</xsl:when>
				<xsl:when test="$lightChar = '14' ">
					<xsl:text>Fl+LFl</xsl:text>
				</xsl:when>
				<xsl:when test="$lightChar = '15' ">
					<xsl:text>Fl</xsl:text>
				</xsl:when>
				<xsl:when test="$lightChar = '16' ">
					<xsl:text>FLFl</xsl:text>
				</xsl:when>
				<xsl:when test="$lightChar = '17' ">
					<xsl:text>AlOc</xsl:text>
				</xsl:when>
				<xsl:when test="$lightChar = '18' ">
					<xsl:text>AlLFl</xsl:text>
				</xsl:when>
				<xsl:when test="$lightChar = '19' ">
					<xsl:text>AlFl</xsl:text>
				</xsl:when>
				<xsl:when test="$lightChar = '20' ">
					<xsl:text>Al</xsl:text>
				</xsl:when>
				<xsl:when test="$lightChar = '25' ">
					<xsl:text>Q+LFl</xsl:text>
				</xsl:when>
				<xsl:when test="$lightChar = '26' ">
					<xsl:text>VQ+LFl</xsl:text>
				</xsl:when>
				<xsl:when test="$lightChar = '27' ">
					<xsl:text>UQ+LFl</xsl:text>
				</xsl:when>
				<xsl:when test="$lightChar = '28' ">
					<xsl:text>Al</xsl:text>
				</xsl:when>
				<xsl:when test="$lightChar = '29' ">
					<xsl:text>AlF Fl</xsl:text>
				</xsl:when>
				<xsl:otherwise>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<!-- Determine vertical alignment -->
		<xsl:variable name="VERTICAL">
			<xsl:choose>
				<xsl:when test="$flareAt45 = 'true'">
					<xsl:text>Bottom</xsl:text>
				</xsl:when>
				<xsl:otherwise><xsl:text>Center</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<!-- Flare at 45 -> TX('LITDSN', 3,1,3, '15110', 2,-1, CHBLK, 23)  bottom-->		
		<!-- otherwise TX('LITDSN', 3,2,3, '15110', 2,0, CHBLK, 23)  centre -->		
		
			<textInstruction>
				<featureReference><xsl:value-of select="$fid"/></featureReference>
				<viewingGroup>23</viewingGroup>
				<displayPlane>OVERRADAR</displayPlane>
				<drawingPriority>24</drawingPriority>
				<textPoint horizontalAlignment="Start" verticalAlignment="{$VERTICAL}">
					<element>
						<text><xsl:value-of select="$CATLIT"/><xsl:value-of select="$LITCHR"/><xsl:value-of select="$SIGGRP"/><xsl:value-of select="$COLOUR"/><xsl:value-of select="$SIGPER"/><xsl:value-of select="$HEIGHT"/><xsl:value-of select="$VALNMR"/><xsl:value-of select="$STATUS"/></text>
						<bodySize>10</bodySize>
						<foreground>CHBLK</foreground>
						<fontCharacteristics>
							<weight>Medium</weight>
							<slant>Upright</slant>
							<serifs>0</serifs>
							<proportion>MonoSpaced</proportion>
						</fontCharacteristics>
					</element>
					<offset>
						<x>7.02</x> <!-- 2 * 10 * 0.351 -->
						<xsl:choose>
							<xsl:when test="$VERTICAL = 'Bottom' ">
								<y>3.51</y>
							</xsl:when>
							<xsl:otherwise>
								<y>0</y>
							</xsl:otherwise>
						</xsl:choose>
					</offset>
				</textPoint>
			</textInstruction>

	</xsl:template>

</xsl:transform>
