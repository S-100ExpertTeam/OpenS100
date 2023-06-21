<?xml version="1.0" encoding="UTF-8"?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
			<xsl:output method="xml" encoding="UTF-8" indent="yes"/>

		
	<!--Template for: Point Wreck with valsou defined -->
	<!-- Logic taken from S-52 csp WRECKS04 -->
	<xsl:template match="Wreck[@primitive='Point' and valueOfSounding != '']" priority="2">
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
					<!-- isolated Danger point symbol -->
				<!-- danger under water Displaybase, SY(ISODGR01), scamin infinite for viewing group 14010,	Priority 80, Overradar -->
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
		<!-- add conditional low accuracy symbol instruction if quality is poor -->
		<xsl:call-template name="QUAPNT02">
			<xsl:with-param name="viewingGroup" select="$viewingGroup"/>
			<xsl:with-param name="displayPlane" select="$displayPlane"/>
			<xsl:with-param name="drawingPriority" select="$drawingPriority"/>
		</xsl:call-template>
	</xsl:template>
	
	<!--Template for: Point Wreck without valsou  -->
	<!-- Logic taken from S-52 csp WRECKS04 -->
	<xsl:template match="Wreck[@primitive='Point']" priority="1">
		<xsl:variable name="displayPlane">UNDERRADAR</xsl:variable>
		<xsl:variable name="drawingPriority" select="12"/>
				
		<!-- call template to determine if isolated Danger -->
		<!-- This returns a viewing group, isolated dangers have a dedicated viewing group -->
		<xsl:variable name="viewingGroup">
			<xsl:call-template name="UDWHAZ05">
				<xsl:with-param name="depth" select="defaultClearanceDepth"/>
				<xsl:with-param name="deepestMinDepth" select="surroundingDepth"/>
				<xsl:with-param name="defaultViewingGroup" select="34050"/><!-- VG to use if not dangerous -->
			</xsl:call-template>
		</xsl:variable>
		
		<!-- Add a conditional instruction on if quality is poor, show low accuracy symbol -->

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
			<xsl:when test="categoryOfWreck = '1' and waterLevelEffect = '3' ">
				<!-- non dangerous or always submerged -->
				<!-- Pick symbol WRECKS04 -->
				<pointInstruction>
					<featureReference><xsl:value-of select="@id"/></featureReference>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<xsl:element name="symbol">
						<xsl:attribute name="reference">WRECKS04</xsl:attribute> 
					</xsl:element>
				</pointInstruction>
			</xsl:when>
			<xsl:when test="categoryOfWreck = '2' and waterLevelEffect = '3' ">
				<!-- non dangerous or always submerged -->
				<!-- Pick symbol WRECKS04 -->
				<pointInstruction>
					<featureReference><xsl:value-of select="@id"/></featureReference>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<xsl:element name="symbol">
						<xsl:attribute name="reference">WRECKS05</xsl:attribute> 
					</xsl:element>
				</pointInstruction>
			</xsl:when>
			<xsl:when test="categoryOfWreck = '4' or categoryOfWreck = '5' or waterLevelEffect = '1' or waterLevelEffect = '2' or waterLevelEffect = '3' or waterLevelEffect = '4' ">
				<!-- wreck showing mast or any portion of superstructure, sometimes dry -->
				<!-- Pick symbol WRECKS01 -->
				<pointInstruction>
					<featureReference><xsl:value-of select="@id"/></featureReference>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<xsl:element name="symbol">
						<xsl:attribute name="reference">WRECKS01</xsl:attribute> 
					</xsl:element>
				</pointInstruction>
			</xsl:when>
			<xsl:otherwise>
				<!-- default -->
				<!-- Pick symbol WRECKS05 -->
				<pointInstruction>
					<featureReference><xsl:value-of select="@id"/></featureReference>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<xsl:element name="symbol">
						<xsl:attribute name="reference">WRECKS05</xsl:attribute> 
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

		<!--Template for: Area Wreck with valsou defined -->
	<!-- Logic taken from S-52 csp WRECKS04 -->
	<xsl:template match="Wreck[@primitive='Surface' and valueOfSounding != '']" priority="2">
		<xsl:variable name="displayPlane">UNDERRADAR</xsl:variable>
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
					<lineInstruction>
						<featureReference><xsl:value-of select="@id"/></featureReference>
						<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
						<displayPlane>OVERRADAR</displayPlane>
						<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
						<xsl:call-template name="simpleLineStyle">
							<xsl:with-param name="style">dot</xsl:with-param>
							<xsl:with-param name="width">0.64</xsl:with-param>
							<xsl:with-param name="colour">CHBLK</xsl:with-param>
						</xsl:call-template>
					</lineInstruction>
				<!-- draw isolated Danger symbol at centre -->					
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
					<xsl:with-param name="displayPlane" select="$displayPlane"/>
					<xsl:with-param name="drawingPriority" select="$drawingPriority +1"/>
					<xsl:with-param name="lineStyle">LOWACC41</xsl:with-param>
				</xsl:call-template>
			</xsl:when>
			<xsl:when test="valueOfSounding > $SAFETY_DEPTH ">
					<!--<QualityLine>
						<poorStyle>LOWACC31</poorstyle>
						<goodStyle>LS(DASH,2,CHBLK)</goodStyle>
					</QualityLine>-->
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
				<!-- draw the sounding, call template or new dedicated sounding instruction -->
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
			<xsl:otherwise>
				<!-- default 
					<QualityLine>
						<poorStyle>LOWACC31</poorstyle>
						<goodStyle>LS(DOTT,2,CHBLK)</goodStyle>
					</QualityLine>-->
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
				<!-- draw the sounding, call template or new dedicated sounding instruction -->
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
		<xsl:if test="not($viewingGroup = 14010 or $viewingGroup = 24020 ) ">
			<!-- overide any low quality curves with LOWACC31 -->
			<xsl:call-template name="QUALIN03">
				<xsl:with-param name="viewingGroup" select="$viewingGroup"/>
				<xsl:with-param name="displayPlane" select="$displayPlane"/>
				<xsl:with-param name="drawingPriority" select="$drawingPriority +1"/>
				<xsl:with-param name="lineStyle">LOWACC31</xsl:with-param>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>
	
	<!--Template for: Area Wreck without valsou  -->
	<!-- Logic taken from S-52 csp WRECKS04 -->
	<xsl:template match="Wreck[@primitive='Surface']" priority="1">
		<xsl:variable name="displayPlane">UNDERRADAR</xsl:variable>
		<xsl:variable name="drawingPriority" select="12"/>

		<!-- call template to determine if isolated Danger -->
		<xsl:variable name="viewingGroup">
			<xsl:call-template name="UDWHAZ05">
				<xsl:with-param name="depth" select="defaultClearanceDepth"/>
				<xsl:with-param name="deepestMinDepth" select="surroundingDepth"/>
				<xsl:with-param name="defaultViewingGroup" select="34050"/>
			</xsl:call-template>
		</xsl:variable>
		
		<!-- Add a conditional instruction on if quality is poor, show low accuracy symbol 
		<LowQualityInstruction/> -->

		<xsl:choose>
			<xsl:when test="$viewingGroup = 14010 or $viewingGroup = 24020 ">
					<!-- isolated Danger linestyle but different viewing group etc 
					<QualityLine>
						<poorStyle>LOWACC41</poorstyle>
						<goodStyle>LS(DOTT,2,CHBLK)</goodStyle>
					</QualityLine> -->
					<lineInstruction>
						<featureReference><xsl:value-of select="@id"/></featureReference>
						<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
						<displayPlane>OVERRADAR</displayPlane>
						<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
						<xsl:call-template name="simpleLineStyle">
							<xsl:with-param name="style">dot</xsl:with-param>
							<xsl:with-param name="width">0.64</xsl:with-param>
							<xsl:with-param name="colour">CHBLK</xsl:with-param>
						</xsl:call-template>
					</lineInstruction>
				<!-- draw isolated Danger symbol at centre -->					
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
					<xsl:with-param name="displayPlane" select="$displayPlane"/>
					<xsl:with-param name="drawingPriority" select="$drawingPriority +1"/>
					<xsl:with-param name="lineStyle">LOWACC41</xsl:with-param>
				</xsl:call-template>
			</xsl:when>
			<xsl:when test="waterLevelEffect = '1' or waterLevelEffect = '2' ">
				<!-- partly submerged or always dry 
					<QualityLine>
						<poorStyle>LOWACC31</poorstyle>
						<goodStyle>LS(SOLD,2,CSTLN)</goodStyle>
					</QualityLine> -->
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
					<!-- AC CHBRN -->
					<areaInstruction>
						<featureReference><xsl:value-of select="@id"/></featureReference>
						<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
						<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
						<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
						<colorFill>
						  <color>CHBRN</color>
						</colorFill>
					</areaInstruction>
			</xsl:when>
			<xsl:when test="waterLevelEffect = '4' ">
				<!-- covers and uncovers 
					<QualityLine>
						<poorStyle>LOWACC31</poorstyle>
						<goodStyle>LS(DASH,2,CSTLN)</goodStyle>
					</QualityLine>-->
					<!-- AC DEPIT -->
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
					<QualityLine>
						<poorStyle>LOWACC31</poorstyle>
						<goodStyle>LS(DOTT,2,CSTLN)</goodStyle>
					</QualityLine> -->
					<!-- AC DEPVS -->
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
							<xsl:with-param name="colour">CSTLN</xsl:with-param>
						</xsl:call-template>
					</lineInstruction>
			</xsl:otherwise>
		</xsl:choose>
		<xsl:if test="not($viewingGroup = 14010 or $viewingGroup = 24020) ">
			<!-- overide any low quality curves with LOWACC31 -->
			<xsl:call-template name="QUALIN03">
				<xsl:with-param name="viewingGroup" select="$viewingGroup"/>
				<xsl:with-param name="displayPlane" select="$displayPlane"/>
				<xsl:with-param name="drawingPriority" select="$drawingPriority +1"/>
				<xsl:with-param name="lineStyle">LOWACC31</xsl:with-param>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

</xsl:transform>


		
		
		
