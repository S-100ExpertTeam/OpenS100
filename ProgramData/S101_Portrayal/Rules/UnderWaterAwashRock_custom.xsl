<?xml version="1.0" encoding="UTF-8"?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
			<xsl:output method="xml" encoding="UTF-8" indent="yes"/>

	<!--Template for: Point UnderwaterAwashRock  updated sep 2016 to call QUAPNT02 -->
	<!-- Logic taken from S-52 csp OBSTRN06 -->
	<xsl:template match="UnderwaterAwashRock[@primitive='Point']" priority="1">
		<xsl:variable name="displayPlane">OVERRADAR</xsl:variable>
		<xsl:variable name="drawingPriority" select="12"/>
		<!-- There is a dedicated template for obstruction with a sounding value so this one will use the defaultClearanceDepth -->
		
		<!-- call template to determine if isolated Danger -->
		<xsl:variable name="viewingGroup">
			<xsl:call-template name="UDWHAZ05">
				<xsl:with-param name="depth" select="defaultClearanceDepth"/>
				<xsl:with-param name="deepestMinDepth" select="surroundingDepth"/>
				<xsl:with-param name="defaultViewingGroup" select="34050"/>
			</xsl:call-template>
		</xsl:variable>

		<xsl:choose>
			<xsl:when test="$viewingGroup = 14010 or $viewingGroup = 24020 ">
					<!-- isolated Danger point symbol -->
				<!-- danger under water Displaybase, SY(ISODGR01), scamin infinite for viewing group 14010,	Priority 8, Overradar -->
				<pointInstruction>
					<featureReference><xsl:value-of select="@id"/></featureReference>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane>OVERRADAR</displayPlane>
					<drawingPriority>24</drawingPriority>
					<xsl:element name="symbol">
						<xsl:attribute name="reference">ISODGR01</xsl:attribute> 
					</xsl:element>
				</pointInstruction>
			</xsl:when>
			<xsl:when test="waterLevelEffect = '3' ">
				<!-- always submerged -->
				<!-- Pick symbol UWTROC03 -->
				<pointInstruction>
					<featureReference><xsl:value-of select="@id"/></featureReference>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<xsl:element name="symbol">
						<xsl:attribute name="reference">UWTROC03</xsl:attribute> 
					</xsl:element>
				</pointInstruction>
			</xsl:when>
			<xsl:otherwise>
				<!-- default -->
				<!-- Pick symbol UWTROC04 -->
				<pointInstruction>
					<featureReference><xsl:value-of select="@id"/></featureReference>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<xsl:element name="symbol">
						<xsl:attribute name="reference">UWTROC04</xsl:attribute> 
					</xsl:element>
				</pointInstruction>
			</xsl:otherwise>
		</xsl:choose>
		<!-- add conditional low accuracy symbol instruction if quality is poor -->
		<xsl:call-template name="QUAPNT02">
			<xsl:with-param name="viewingGroup" select="$viewingGroup"/>
			<xsl:with-param name="displayPlane" select="$displayPlane"/>
			<xsl:with-param name="drawingPriority" select="$drawingPriority"/>
		</xsl:call-template>
	</xsl:template>
	
	<!--Template for: Point UnderwaterAwashRock with valsou defined -->
	<!-- Logic taken from S-52 csp OBSTRN06 -->
	<xsl:template match="UnderwaterAwashRock[@primitive='Point' and valueOfSounding != '']" priority="2">
		<xsl:variable name="displayPlane">OVERRADAR</xsl:variable>
		<xsl:variable name="drawingPriority" select="12"/>

		<!-- call template to determine if isolated Danger -->
		<xsl:variable name="viewingGroup">
			<xsl:call-template name="UDWHAZ05">
				<xsl:with-param name="depth" select="valueOfSounding"/>
				<xsl:with-param name="deepestMinDepth" select="surroundingDepth"/>
				<xsl:with-param name="defaultViewingGroup" select="34051"/>
			</xsl:call-template>
		</xsl:variable>
		
		<xsl:choose>
			<xsl:when test="$viewingGroup = 14010 or $viewingGroup = 24020 ">
				<!-- danger under water Displaybase, SY(ISODGR01), scamin infinite for viewing group 14010,	Priority 80, Overradar -->
					<!-- isolated Danger point symbol -->
				<pointInstruction>
					<featureReference><xsl:value-of select="@id"/></featureReference>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane>OVERRADAR</displayPlane>
					<drawingPriority>24</drawingPriority>
					<xsl:element name="symbol">
						<xsl:attribute name="reference">ISODGR01</xsl:attribute> 
					</xsl:element>
				</pointInstruction>
			</xsl:when>
			<xsl:when test="valueOfSounding > $SAFETY_DEPTH ">
				<!-- covers and uncovers or awash -->
				<!-- Pick symbol DANGER02 -->
				<!-- draw the sounding, call template or new dedicated sounding instruction -->
				<pointInstruction>
					<featureReference><xsl:value-of select="@id"/></featureReference>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<xsl:element name="symbol">
						<xsl:attribute name="reference">DANGER02</xsl:attribute> 
					</xsl:element>
				</pointInstruction>
				<!-- draw the sounding, call template or new dedicated sounding instruction  for now call the old sounding drawing csp -->
				<xsl:call-template name="SNDFRM04">
					<xsl:with-param name="viewingGroup" select="$viewingGroup"/>
					<xsl:with-param name="displayPlane" select="$displayPlane"/>
					<xsl:with-param name="drawingPriority">18</xsl:with-param>
					<xsl:with-param name="fid" select="@id"/>
					<xsl:with-param name="sid"/>
					<xsl:with-param name="depth" select="valueOfSounding"/>
					<xsl:with-param name="tecsou" select="techniqueOfVerticalMeasurement"/>
					<xsl:with-param name="quasou" select="qualityOfSoundingMeasurement"/>
					<xsl:with-param name="status" select="status"/>
					<xsl:with-param name="quapos"/>
				</xsl:call-template>

			</xsl:when>
			<xsl:when test="waterLevelEffect = '4' or waterLevelEffect = '5' ">
				<!-- covers and uncovers or awash -->
				<!-- Pick symbol UWTROC04 -->
				<pointInstruction>
					<featureReference><xsl:value-of select="@id"/></featureReference>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<xsl:element name="symbol">
						<xsl:attribute name="reference">UWTROC04</xsl:attribute> 
					</xsl:element>
				</pointInstruction>
			</xsl:when>
			<xsl:otherwise>
				<!-- default -->
				<!-- Pick symbol DANGER01 -->
				<!-- draw the sounding, call template or new dedicated sounding instruction -->
				<pointInstruction>
					<featureReference><xsl:value-of select="@id"/></featureReference>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<xsl:element name="symbol">
						<xsl:attribute name="reference">DANGER01</xsl:attribute> 
					</xsl:element>
				</pointInstruction>
				<!-- draw the sounding, call template or new dedicated sounding instruction  for now call the old sounding drawing csp -->
				<xsl:call-template name="SNDFRM04">
					<xsl:with-param name="viewingGroup" select="$viewingGroup"/>
					<xsl:with-param name="displayPlane" select="$displayPlane"/>
					<xsl:with-param name="drawingPriority">18</xsl:with-param>
					<xsl:with-param name="fid" select="@id"/>
					<xsl:with-param name="sid"/>
					<xsl:with-param name="depth" select="valueOfSounding"/>
					<xsl:with-param name="tecsou" select="techniqueOfVerticalMeasurement"/>
					<xsl:with-param name="quasou" select="qualityOfSoundingMeasurement"/>
					<xsl:with-param name="status" select="status"/>
					<xsl:with-param name="quapos"/>
				</xsl:call-template>
			</xsl:otherwise>
		</xsl:choose>
		<!-- add conditional low accuracy symbol instruction if quality is poor -->
		<xsl:call-template name="QUAPNT02">
			<xsl:with-param name="viewingGroup" select="$viewingGroup"/>
			<xsl:with-param name="displayPlane" select="$displayPlane"/>
			<xsl:with-param name="drawingPriority" select="$drawingPriority"/>
		</xsl:call-template>
	</xsl:template>
			
</xsl:transform>
