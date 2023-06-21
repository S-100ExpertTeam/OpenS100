<?xml version="1.0" encoding="UTF-8"?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
    <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
	<!-- Jan 29, 2015 Named template for obstructions, based on S-52 CSPs obstrn and udwhaz -->
	<!-- Plan for S-101 is to have actual depth or clearance depth instead of searching surrounding areas and making assumptions about the depth of the feature, this should be done and verified by the HO -->

	<!-- Old udwhaz logic to determine if underwater hazard is isolated danger -->
	<!-- Meant to be executed within a variable, returns the viewing group which indicates whether it is isolated danger or not -->
	<xsl:template name="UDWHAZ05">
	<xsl:param name="depth" select="-15"/> 
	<!-- depth to test (valsou or default clearance), if not provided default to -15 -->
	<xsl:param name="deepestMinDepth" select="-15"/> <!-- The deepest drval1 of surrounding water -->
	<xsl:param name="defaultViewingGroup" select="34050"/> <!-- The default viewing group -->

		<xsl:choose>
			<xsl:when test="$depth > $SAFETY_CONTOUR"> 				
				<xsl:value-of select="$defaultViewingGroup"/>
			</xsl:when>
			<xsl:when test="$deepestMinDepth >= $SAFETY_CONTOUR and (waterLevelEffect = '1' or waterLevelEffect = '2') ">
				<!-- danger above water Displaybase, viewing group 14050 -->
				<xsl:text>14050</xsl:text>
			</xsl:when>
			<xsl:when test="$deepestMinDepth >= $SAFETY_CONTOUR or not($deepestMinDepth !='') ">
				<!-- if no surrounding depth was provided then can only assume it is an isolated danger -->
				<!-- danger under water Displaybase, SY(ISODGR01), scamin infinite,
					Priority 80, Overradar, viewing group 14010 -->
				<xsl:text>14010</xsl:text>
			</xsl:when>
			<xsl:when test="$deepestMinDepth >= 0 and $SHOW_ISOLATED_DANGERS_IN_SHALLOW_WATERS = 'true' and (waterLevelEffect = '1' or waterLevelEffect = '2')">
				<!-- above water danger in shallow water viewing group 24050 -->
				<xsl:text>24050</xsl:text>
			</xsl:when>
			<xsl:when test="$deepestMinDepth >= 0 and $SHOW_ISOLATED_DANGERS_IN_SHALLOW_WATERS = 'true' ">
				<!-- danger under shallow water SY(ISODGR01),Priority 80, Overradar, viewing group 24020 -->
				<xsl:text>24020</xsl:text>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$defaultViewingGroup"/>
			</xsl:otherwise>
		</xsl:choose>

	</xsl:template>

	<!--Template for: Point obstruction -->
	<!-- Logic taken from S-52 csp OBSTRN07 -->
	<xsl:template match="Obstruction[@primitive='Point']" priority="1">

		<xsl:variable name="displayPlane">UNDERRADAR</xsl:variable>
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
			<xsl:when test="not(categoryOfObstruction = '6') and (waterLevelEffect = '1' or waterLevelEffect = '2') ">
				<!-- partly submerged or always dry -->
				<!-- Pick symbol OBSTRN11 -->
				<pointInstruction>
					<featureReference><xsl:value-of select="@id"/></featureReference>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<xsl:element name="symbol">
						<xsl:attribute name="reference">OBSTRN11</xsl:attribute> 
					</xsl:element>
				</pointInstruction>
			</xsl:when>
			<xsl:when test="not(categoryOfObstruction = '6') and (waterLevelEffect = '4' or waterLevelEffect = '5') ">
				<!-- covers and uncovers or awash -->
				<!-- Pick symbol OBSTRN03 -->
				<pointInstruction>
					<featureReference><xsl:value-of select="@id"/></featureReference>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<xsl:element name="symbol">
						<xsl:attribute name="reference">OBSTRN03</xsl:attribute> 
					</xsl:element>
				</pointInstruction>
			</xsl:when>
			<xsl:otherwise>
				<!-- default -->
				<!-- Pick symbol OBSTRN01 -->
				<pointInstruction>
					<featureReference><xsl:value-of select="@id"/></featureReference>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<xsl:element name="symbol">
						<xsl:attribute name="reference">OBSTRN01</xsl:attribute> 
					</xsl:element>
				</pointInstruction>
			</xsl:otherwise>
		</xsl:choose>
		<!-- Add a conditional instruction on if quality is poor, show low accuracy symbol -->
		<xsl:call-template name="QUAPNT02">
			<xsl:with-param name="viewingGroup" select="$viewingGroup"/>
			<xsl:with-param name="displayPlane" select="$displayPlane"/>
			<xsl:with-param name="drawingPriority" select="$drawingPriority"/>
		</xsl:call-template>
		
	</xsl:template>
	
	<!--Template for: Point obstruction with valsou defined -->
	<!-- Logic taken from S-52 csp OBSTRN06 -->
	<xsl:template match="Obstruction[@primitive='Point' and valueOfSounding != '']" priority="2">
		<xsl:variable name="displayPlane">UNDERRADAR</xsl:variable>
		<xsl:variable name="drawingPriority" select="12"/>

				
		<!-- call template to determine if isolated Danger -->
		<xsl:variable name="viewingGroup">
			<xsl:call-template name="UDWHAZ05">
				<xsl:with-param name="depth" select="valueOfSounding"/>
				<xsl:with-param name="deepestMinDepth" select="surroundingDepth"/>
				<xsl:with-param name="defaultViewingGroup" select="34050"/>
			</xsl:call-template>
		</xsl:variable>
		
		<!-- Add a conditional instruction on if quality is poor, show low accuracy symbol 
		<LowQualityInstruction/>-->

		<xsl:choose>
			<xsl:when test="$viewingGroup = 14010 or $viewingGroup = 24020">
					<!-- isolated Danger point symbol -->
				<!-- danger under water Displaybase, SY(ISODGR01), Priority 8, Overradar -->
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
				<!-- Pick symbol DANGER02 -->
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
					<xsl:with-param name="viewingGroup" select="'33010'"/>
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
			<xsl:when test="not(categoryOfObstruction = '6') and (waterLevelEffect = '1' or waterLevelEffect = '2') ">
				<!-- partly submerged or always dry -->
				<!-- Pick symbol OBSTRN11 -->
				<pointInstruction>
					<featureReference><xsl:value-of select="@id"/></featureReference>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<xsl:element name="symbol">
						<xsl:attribute name="reference">OBSTRN11</xsl:attribute> 
					</xsl:element>
				</pointInstruction>
			</xsl:when>
			<xsl:when test="not(categoryOfObstruction = '6') and (waterLevelEffect = '4' or waterLevelEffect = '5') ">
				<!-- covers and uncovers or awash -->
				<!-- Pick symbol DANGER03 -->
				<pointInstruction>
					<featureReference><xsl:value-of select="@id"/></featureReference>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<xsl:element name="symbol">
						<xsl:attribute name="reference">DANGER03</xsl:attribute> 
					</xsl:element>
				</pointInstruction>
				<!-- draw the sounding, call template or new dedicated sounding instruction  for now call the old sounding drawing csp -->
				<xsl:call-template name="SNDFRM04">
					<xsl:with-param name="viewingGroup" select="'33010'"/>
					<xsl:with-param name="displayPlane" select="'OVERRADAR'"/>
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
			<xsl:otherwise>
				<!-- default -->
				<!-- Pick symbol DANGER01 -->
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
					<xsl:with-param name="viewingGroup" select="'33010'"/>
					<xsl:with-param name="displayPlane" select="'OVERRADAR'"/>
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
		<!-- Add a conditional instruction on if quality is poor, show low accuracy symbol -->
		<xsl:call-template name="QUAPNT02">
			<xsl:with-param name="viewingGroup" select="$viewingGroup"/>
			<xsl:with-param name="displayPlane" select="$displayPlane"/>
			<xsl:with-param name="drawingPriority" select="$drawingPriority"/>
		</xsl:call-template>
	</xsl:template>

	
		<!--Template for: Line obstruction with valsou defined -->
	<!-- Logic taken from S-52 csp OBSTRN06 continuation B -->
	<xsl:template match="Obstruction[@primitive='Curve' and valueOfSounding != '']" priority="2">
		<xsl:variable name="displayPlane">UNDERRADAR</xsl:variable>
		<xsl:variable name="drawingPriority" select="12"/>
		
		<!-- call template to determine if isolated Danger -->
		<xsl:variable name="viewingGroup">
			<xsl:call-template name="UDWHAZ05">
				<xsl:with-param name="depth" select="valueOfSounding"/>
				<xsl:with-param name="deepestMinDepth" select="surroundingDepth"/>
				<xsl:with-param name="defaultViewingGroup" select="34050"/>
			</xsl:call-template>
		</xsl:variable>

		<xsl:choose>
			<xsl:when test="$viewingGroup = 14010 or $viewingGroup = 24020 ">
					<!-- isolated Danger line style or low accuracy -->
					<lineInstruction>
					<featureReference><xsl:value-of select="@id"/></featureReference>
						<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
						<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
						<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
						<xsl:call-template name="simpleLineStyle">
							<xsl:with-param name="style">dot</xsl:with-param>
							<xsl:with-param name="width">0.64</xsl:with-param>
							<xsl:with-param name="colour">CHBLK</xsl:with-param>
						</xsl:call-template>
					</lineInstruction>
					<!-- Add isolated danger symbol at midpoint of line -->
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
				<!-- overide any low quality curves with LOWACC41 -->
				<xsl:call-template name="QUALIN03">
					<xsl:with-param name="viewingGroup" select="$viewingGroup"/>
					<xsl:with-param name="displayPlane">OVERRADAR</xsl:with-param>
					<xsl:with-param name="drawingPriority">25</xsl:with-param>
					<xsl:with-param name="lineStyle">LOWACC41</xsl:with-param>
				</xsl:call-template>

			</xsl:when>
			<xsl:when test="valueOfSounding > $SAFETY_DEPTH ">
				<lineInstruction>
					<featureReference><xsl:value-of select="@id"/></featureReference>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<xsl:call-template name="simpleLineStyle">
						<xsl:with-param name="style">dash</xsl:with-param>
						<xsl:with-param name="width">0.64</xsl:with-param>
						<xsl:with-param name="colour">CHBLK</xsl:with-param>
					</xsl:call-template>
				</lineInstruction>
				<xsl:call-template name="SNDFRM04">
					<xsl:with-param name="viewingGroup" select="$viewingGroup"/>
					<xsl:with-param name="displayPlane" select="$displayPlane"/>
					<xsl:with-param name="drawingPriority" select="$drawingPriority"/>
					<xsl:with-param name="fid" select="@id"/>
					<xsl:with-param name="sid"/>
					<xsl:with-param name="depth" select="valueOfSounding"/>
					<xsl:with-param name="tecsou" select="techniqueOfVerticalMeasurement"/>
					<xsl:with-param name="quasou" select="qualityOfSoundingMeasurement"/>
					<xsl:with-param name="status" select="status"/>
					<xsl:with-param name="quapos"/>
				</xsl:call-template>
				<!-- overide any low quality curves with LOWACC31 -->
				<xsl:call-template name="QUALIN03">
					<xsl:with-param name="viewingGroup" select="$viewingGroup"/>
					<xsl:with-param name="displayPlane">OVERRADAR</xsl:with-param>
					<xsl:with-param name="drawingPriority" select="$drawingPriority + 1"/>
					<xsl:with-param name="lineStyle">LOWACC31</xsl:with-param>
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<!-- valsou is less than SAFETY_DEPTH -->
				<lineInstruction>
					<featureReference><xsl:value-of select="@id"/></featureReference>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<xsl:call-template name="simpleLineStyle">
						<xsl:with-param name="style">dot</xsl:with-param>
						<xsl:with-param name="width">0.64</xsl:with-param>
						<xsl:with-param name="colour">CHBLK</xsl:with-param>
					</xsl:call-template>
				</lineInstruction>
				<xsl:call-template name="SNDFRM04">
					<xsl:with-param name="viewingGroup" select="$viewingGroup"/>
					<xsl:with-param name="displayPlane" select="$displayPlane"/>
					<xsl:with-param name="drawingPriority" select="$drawingPriority"/>
					<xsl:with-param name="fid" select="@id"/>
					<xsl:with-param name="sid"/>
					<xsl:with-param name="depth" select="valueOfSounding"/>
					<xsl:with-param name="tecsou" select="techniqueOfVerticalMeasurement"/>
					<xsl:with-param name="quasou" select="qualityOfSoundingMeasurement"/>
					<xsl:with-param name="status" select="status"/>
					<xsl:with-param name="quapos"/>
				</xsl:call-template>
				<!-- overide any low quality curves with LOWACC31 -->
				<xsl:call-template name="QUALIN03">
					<xsl:with-param name="viewingGroup" select="$viewingGroup"/>
					<xsl:with-param name="displayPlane">OVERRADAR</xsl:with-param>
					<xsl:with-param name="drawingPriority" select="$drawingPriority + 1"/>
					<xsl:with-param name="lineStyle">LOWACC31</xsl:with-param>
				</xsl:call-template>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

		<!--Template for: Line/curve obstruction -->
	<!-- Logic taken from S-52 csp OBSTRN06 -->
	<xsl:template match="Obstruction[@primitive='Curve']" priority="1">
		<xsl:variable name="displayPlane">UNDERRADAR</xsl:variable>
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
				<!-- isolated Danger line style or low accuracy -->
				<lineInstruction>
					<featureReference><xsl:value-of select="@id"/></featureReference>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<xsl:call-template name="simpleLineStyle">
						<xsl:with-param name="style">dot</xsl:with-param>
						<xsl:with-param name="width">0.64</xsl:with-param>
						<xsl:with-param name="colour">CHBLK</xsl:with-param>
					</xsl:call-template>
				</lineInstruction>
					<!-- Add isolated danger symbol at midpoint of line -->
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
				<!-- overide any low quality curves with LOWACC41 -->
				<xsl:call-template name="QUALIN03">
					<xsl:with-param name="viewingGroup" select="$viewingGroup"/>
					<xsl:with-param name="displayPlane">OVERRADAR</xsl:with-param>
					<xsl:with-param name="drawingPriority">25</xsl:with-param>
					<xsl:with-param name="lineStyle">LOWACC41</xsl:with-param>
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<!-- default -->
				<!-- <QualityLine>
						<poorStyle>LOWACC31</poorstyle>
						<goodStyle>LS(DOTT,2,CHBLK)</goodStyle>
				</QualityLine> -->
				<lineInstruction>
					<featureReference><xsl:value-of select="@id"/></featureReference>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<xsl:call-template name="simpleLineStyle">
						<xsl:with-param name="style">dot</xsl:with-param>
						<xsl:with-param name="width">0.64</xsl:with-param>
						<xsl:with-param name="colour">CHBLK</xsl:with-param>
					</xsl:call-template>
				</lineInstruction>
				<!-- overide any low quality curves with LOWACC31 -->
				<xsl:call-template name="QUALIN03">
					<xsl:with-param name="viewingGroup" select="$viewingGroup"/>
					<xsl:with-param name="displayPlane" select="$displayPlane"/>
					<xsl:with-param name="drawingPriority" select="$drawingPriority + 1"/>
					<xsl:with-param name="lineStyle">LOWACC31</xsl:with-param>
				</xsl:call-template>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
	
		<!--Template for: Area/surface obstruction -->
	<!-- Logic taken from S-52 csp OBSTRN06 -->
	<xsl:template match="Obstruction[@primitive='Surface']" priority="1">
		<xsl:variable name="displayPlane">UNDERRADAR</xsl:variable>
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
					<!-- isolated Danger line style or low accuracy 
					Solid fill DEPVS
					Area pattern FOULAR01
					Boundary line style LS(DOTT,2,CHBLK)
					Centred symbol for isolated danger -->
					<!--Draw Area pattern  FOULAR01-->
					  <areaInstruction>
						<featureReference><xsl:value-of select="@id"/></featureReference>
						<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
						<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
						<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
						<colorFill>
						  <color>DEPVS</color>
						</colorFill>
					  </areaInstruction>
					<areaInstruction>
						<featureReference><xsl:value-of select="@id"/></featureReference>
						<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
						<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
						<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
						<areaFillReference reference="FOULAR01"/>
					</areaInstruction>
					<lineInstruction>
						<featureReference><xsl:value-of select="@id"/></featureReference>
						<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
						<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
						<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
						<xsl:call-template name="simpleLineStyle">
							<xsl:with-param name="style">dot</xsl:with-param>
							<xsl:with-param name="width">0.64</xsl:with-param>
							<xsl:with-param name="colour">CHBLK</xsl:with-param>
						</xsl:call-template>
					</lineInstruction>
					<!-- Add isolated danger symbol at midpoint of line -->
				<!-- danger under water Displaybase, SY(ISODGR01), scamin infinite for viewing group 14010,	Priority 8, Overradar -->
				<pointInstruction>
					<featureReference><xsl:value-of select="@id"/></featureReference>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane>OVERRADAR</displayPlane>
					<drawingPriority>24</drawingPriority>
					<xsl:element name="symbol">
						<xsl:attribute name="reference">ISODGR01</xsl:attribute> 
						<areaPlacement placementMode="VisibleParts"/>
					</xsl:element>
				</pointInstruction>
				<!-- overide any low quality curves with LOWACC41 -->
				<xsl:call-template name="QUALIN03">
					<xsl:with-param name="viewingGroup" select="$viewingGroup"/>
					<xsl:with-param name="displayPlane">OVERRADAR</xsl:with-param>
					<xsl:with-param name="drawingPriority">25</xsl:with-param>
					<xsl:with-param name="lineStyle">LOWACC41</xsl:with-param>
				</xsl:call-template>
			</xsl:when>
			<xsl:when test="categoryOfObstruction=6">
					<!-- foul area 
					Area pattern FOULAR01
					Boundary line style LS(DOTT,2,CHBLK) -->
					<areaInstruction>
						<featureReference><xsl:value-of select="@id"/></featureReference>
						<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
						<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
						<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
						<areaFillReference reference="FOULAR01"/>
					</areaInstruction>
					<lineInstruction>
						<featureReference><xsl:value-of select="@id"/></featureReference>
						<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
						<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
						<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
						<xsl:call-template name="simpleLineStyle">
							<xsl:with-param name="style">dot</xsl:with-param>
							<xsl:with-param name="width">0.64</xsl:with-param>
							<xsl:with-param name="colour">CHBLK</xsl:with-param>
						</xsl:call-template>
					</lineInstruction>
			</xsl:when>
			<xsl:when test="waterLevelEffect=1 or waterLevelEffect=2">
					<!-- Partly submerged at high water or always dry 
					AC(CHBRN)
					Boundary line style LS(SOLD,2,CSTLN)-->
					<areaInstruction>
						<featureReference><xsl:value-of select="@id"/></featureReference>
						<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
						<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
						<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
						<colorFill>
						  <color>CHBRN</color>
						</colorFill>
					</areaInstruction>
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
			<xsl:when test="waterLevelEffect=4">
					<!-- covers and uncovers 
					AC(DEPIT)
					Boundary line style LS(DASH,2,CSTLN) -->
					<areaInstruction>
						<featureReference><xsl:value-of select="@id"/></featureReference>
						<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
						<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
						<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
						<colorFill>
						  <color>DEPIT</color>
						</colorFill>
					</areaInstruction>
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
				<!-- default 
				AC(DEPVS)
				LS(DOTT,2,CHBLK) -->
					<areaInstruction>
						<featureReference><xsl:value-of select="@id"/></featureReference>
						<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
						<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
						<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
						<colorFill>
						  <color>DEPVS</color>
						</colorFill>
					</areaInstruction>
					<lineInstruction>
						<featureReference><xsl:value-of select="@id"/></featureReference>
						<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
						<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
						<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
						<xsl:call-template name="simpleLineStyle">
							<xsl:with-param name="style">dot</xsl:with-param>
							<xsl:with-param name="width">0.64</xsl:with-param>
							<xsl:with-param name="colour">CHBLK</xsl:with-param>
						</xsl:call-template>
					</lineInstruction>
				</xsl:otherwise>
		</xsl:choose>
		<xsl:if test="not($viewingGroup = 14010 or $viewingGroup = 24020) ">
			<!-- overide any low quality curves with LOWACC31 -->
			<xsl:call-template name="QUALIN03">
				<xsl:with-param name="viewingGroup" select="$viewingGroup"/>
				<xsl:with-param name="displayPlane" select="$displayPlane"/>
				<xsl:with-param name="drawingPriority" select="$drawingPriority + 1"/>
				<xsl:with-param name="lineStyle">LOWACC31</xsl:with-param>
			</xsl:call-template>
		</xsl:if>
		
	</xsl:template>
	
		<!--Template for: Area/surface obstruction -->
	<!-- Logic taken from S-52 csp OBSTRN06 -->
	<xsl:template match="Obstruction[@primitive='Surface' and valueOfSounding != '']" priority="2">
		<xsl:variable name="displayPlane">UNDERRADAR</xsl:variable>
		<xsl:variable name="drawingPriority" select="12"/>
		
		<!-- call template to determine if isolated Danger -->
		<xsl:variable name="viewingGroup">
			<xsl:call-template name="UDWHAZ05">
				<xsl:with-param name="depth" select="valueOfSounding"/>
				<xsl:with-param name="deepestMinDepth" select="surroundingDepth"/>
				<xsl:with-param name="defaultViewingGroup" select="34050"/>
			</xsl:call-template>
		</xsl:variable>

		<xsl:choose>
			<xsl:when test="$viewingGroup = 14010 or $viewingGroup = 24020 ">
					<!-- isolated Danger line style or low accuracy 
					Solid fill DEPVS
					Area pattern FOULAR01
					Boundary line style LS(DOTT,2,CHBLK)
					Centred symbol for isolated danger -->
				<!-- danger under water Displaybase, SY(ISODGR01), scamin infinite for viewing group 14010,	Priority 8, Overradar -->
					  <areaInstruction>
						<featureReference><xsl:value-of select="@id"/></featureReference>
						<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
						<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
						<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
						<colorFill>
						  <color>DEPVS</color>
						</colorFill>
					  </areaInstruction>
					<areaInstruction>
						<featureReference><xsl:value-of select="@id"/></featureReference>
						<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
						<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
						<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
						<areaFillReference reference="FOULAR01"/>
					</areaInstruction>
					<lineInstruction>
						<featureReference><xsl:value-of select="@id"/></featureReference>
						<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
						<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
						<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
						<xsl:call-template name="simpleLineStyle">
							<xsl:with-param name="style">dot</xsl:with-param>
							<xsl:with-param name="width">0.64</xsl:with-param>
							<xsl:with-param name="colour">CHBLK</xsl:with-param>
						</xsl:call-template>
					</lineInstruction>
					<!-- Add isolated danger symbol at centre -->
				<!-- danger under water Displaybase, SY(ISODGR01), scamin infinite for viewing group 14010,	Priority 8, Overradar -->
				<pointInstruction>
					<featureReference><xsl:value-of select="@id"/></featureReference>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane>OVERRADAR</displayPlane>
					<drawingPriority>24</drawingPriority>
					<xsl:element name="symbol">
						<xsl:attribute name="reference">ISODGR01</xsl:attribute> 
						<areaPlacement placementMode="VisibleParts"/>
					</xsl:element>
				</pointInstruction>
				<!-- overide any low quality curves with LOWACC41 -->
				<xsl:call-template name="QUALIN03">
					<xsl:with-param name="viewingGroup" select="$viewingGroup"/>
					<xsl:with-param name="displayPlane">OVERRADAR</xsl:with-param>
					<xsl:with-param name="drawingPriority" select="$drawingPriority+1"/>
					<xsl:with-param name="lineStyle">LOWACC41</xsl:with-param>
				</xsl:call-template>
			</xsl:when>
			<xsl:when test="valueOfSounding > $SAFETY_DEPTH ">
					<!--LS(DASH,2,CHGRD)
					Draw sounding in centre of area -->
					<lineInstruction>
					<featureReference><xsl:value-of select="@id"/></featureReference>
						<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
						<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
						<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
						<xsl:call-template name="simpleLineStyle">
							<xsl:with-param name="style">dash</xsl:with-param>
							<xsl:with-param name="width">0.64</xsl:with-param>
							<xsl:with-param name="colour">CHGRD</xsl:with-param>
						</xsl:call-template>
					</lineInstruction>
				<xsl:call-template name="SNDFRM04">
					<xsl:with-param name="viewingGroup" select="$viewingGroup"/>
					<xsl:with-param name="displayPlane" select="$displayPlane"/>
					<xsl:with-param name="drawingPriority" select="$drawingPriority"/>
					<xsl:with-param name="fid" select="@id"/>
					<xsl:with-param name="sid"/>
					<xsl:with-param name="depth" select="valueOfSounding"/>
					<xsl:with-param name="tecsou" select="techniqueOfVerticalMeasurement"/>
					<xsl:with-param name="quasou" select="qualityOfSoundingMeasurement"/>
					<xsl:with-param name="status" select="status"/>
					<xsl:with-param name="quapos"/>
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<!-- default 
				LS(DOTT,2,CHBLK)
				Draw sounding in centre of area -->
				<lineInstruction>
					<featureReference><xsl:value-of select="@id"/></featureReference>
						<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
						<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
						<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
						<xsl:call-template name="simpleLineStyle">
							<xsl:with-param name="style">dot</xsl:with-param>
							<xsl:with-param name="width">0.64</xsl:with-param>
							<xsl:with-param name="colour">CHBLK</xsl:with-param>
						</xsl:call-template>
				</lineInstruction>
				<xsl:call-template name="SNDFRM04">
					<xsl:with-param name="viewingGroup" select="$viewingGroup"/>
					<xsl:with-param name="displayPlane" select="$displayPlane"/>
					<xsl:with-param name="drawingPriority" select="$drawingPriority"/>
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
		<xsl:if test="not($viewingGroup = 14010 or $viewingGroup = 24020) ">
			<!-- overide any low quality curves with LOWACC31 -->
			<xsl:call-template name="QUALIN03">
				<xsl:with-param name="viewingGroup" select="$viewingGroup"/>
				<xsl:with-param name="displayPlane" select="$displayPlane"/>
				<xsl:with-param name="drawingPriority" select="$drawingPriority + 1"/>
				<xsl:with-param name="lineStyle">LOWACC31</xsl:with-param>
			</xsl:call-template>
		</xsl:if>
		
	</xsl:template>
	
</xsl:transform>
