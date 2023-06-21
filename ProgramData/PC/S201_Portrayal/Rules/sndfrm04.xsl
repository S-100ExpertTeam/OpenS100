<?xml version="1.0" encoding="UTF-8"?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
			<xsl:output method="xml" encoding="UTF-8" indent="yes"/>
	<!-- Sept, 2016 added a second template SNDFRM04P for use with multipoint soundings, using augmented point geometry -->
	<!-- Nov, 2016 added logic to handle decimal only soundings value less than 1 -->

	<!-- Mar 17, 2015 Named template for turning a sounding into symbols -->
	<xsl:template name="SNDFRM04">
	<xsl:param name="viewingGroup"/> <!-- The viewing group -->
	<xsl:param name="displayPlane"/> <!-- The display plane -->
	<xsl:param name="drawingPriority"/> <!-- The drawing priority -->
	<xsl:param name="fid"/> <!-- If feature, then feature id -->
	<xsl:param name="sid"/> <!-- If spatial, then spatial id -->
	<xsl:param name="depth"/> <!-- The depth value -->
	<xsl:param name="tecsou"/> <!-- technique of sounding measurement -->
	<xsl:param name="quasou"/> <!-- quality of sounding measurement -->
	<xsl:param name="status"/> <!-- status of the object -->
	<xsl:param name="quapos"/> <!-- spatial quality of position -->

	<xsl:if test="number($depth) = $depth">  <!-- if depth is not a number do nothing -->

		<!-- variable to hold adjusted depth -->
		<xsl:variable name="depthval">
			<xsl:choose>
				<xsl:when test="$depth > 31 ">
					<!-- truncate the depth to an integer -->
					<xsl:value-of select="floor($depth)"/>
				</xsl:when>
				<xsl:when test="0 > $depth ">
					<!-- turn it positive -->
					<xsl:value-of select="0 - $depth"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select="$depth"/>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		
		<!-- variable to hold prefix -->
		<xsl:variable name="prefix">
			<xsl:choose>
				<xsl:when test="$depth > $SAFETY_DEPTH">
					<!-- truncate the depth to an integer -->
					<xsl:text>SOUNDG</xsl:text>
				</xsl:when>
				<xsl:otherwise>
					<xsl:text>SOUNDS</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<xsl:if test="$tecsou !='' and ($tecsou = '4' or $tecsou = '6')">
				<pointInstruction>
					<featureReference><xsl:value-of select="$fid"/></featureReference>
					<xsl:if test="$sid != '' "><spatialReference><xsl:value-of select="$sid"/></spatialReference></xsl:if>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<xsl:element name="symbol">
						<xsl:attribute name="reference"><xsl:value-of select="concat($prefix,'B1')"/></xsl:attribute> 
					</xsl:element>
				</pointInstruction>
		</xsl:if>
		<xsl:if test="($quasou !='' and ($quasou = '3' or $quasou = '4' or $quasou = '5' or $quasou = '8' or $quasou = '9')) 
		or $status = '18' or ($quapos !='' and not($quapos = '1' or $quapos = '10' or $quapos = '11' ) )">
				<pointInstruction>
					<featureReference><xsl:value-of select="$fid"/></featureReference>
										<xsl:if test="$sid != '' "><spatialReference><xsl:value-of select="$sid"/></spatialReference></xsl:if>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<xsl:element name="symbol">
						<xsl:attribute name="reference"><xsl:value-of select="concat($prefix,'C2')"/></xsl:attribute> 
					</xsl:element>
				</pointInstruction>
		</xsl:if>
		
		<xsl:if test="0 > $depth">
				<pointInstruction>
					<featureReference><xsl:value-of select="$fid"/></featureReference>
										<xsl:if test="$sid != '' "><spatialReference><xsl:value-of select="$sid"/></spatialReference></xsl:if>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<xsl:element name="symbol">
						<xsl:attribute name="reference"><xsl:value-of select="concat($prefix,'A1')"/></xsl:attribute> 
					</xsl:element>
				</pointInstruction>
		</xsl:if>

		<xsl:choose>
			<xsl:when test="1 > $depthval "> 		<!-- algorithm 1a Value is a decimal -->
				<xsl:variable name="digits" select="$depthval * 10" />
				<pointInstruction>
					<featureReference><xsl:value-of select="$fid"/></featureReference>
										<xsl:if test="$sid != '' "><spatialReference><xsl:value-of select="$sid"/></spatialReference></xsl:if>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<xsl:element name="symbol">
						<xsl:attribute name="reference"><xsl:value-of select="concat($prefix,'1','0')"/></xsl:attribute> 
					</xsl:element>
				</pointInstruction>
				<pointInstruction>
					<featureReference><xsl:value-of select="$fid"/></featureReference>
										<xsl:if test="$sid != '' "><spatialReference><xsl:value-of select="$sid"/></spatialReference></xsl:if>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<xsl:element name="symbol">
						<xsl:attribute name="reference"><xsl:value-of select="concat($prefix,'5',substring($digits,1,1))"/></xsl:attribute> 
					</xsl:element>
				</pointInstruction>
			</xsl:when>
			<xsl:when test="10 > $depthval "> 		<!-- algorithm 1b -->
				<xsl:variable name="digits" select="$depthval * 10" />
				<pointInstruction>
					<featureReference><xsl:value-of select="$fid"/></featureReference>
										<xsl:if test="$sid != '' "><spatialReference><xsl:value-of select="$sid"/></spatialReference></xsl:if>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<xsl:element name="symbol">
						<xsl:attribute name="reference"><xsl:value-of select="concat($prefix,'1',substring($digits,1,1))"/></xsl:attribute> 
					</xsl:element>
				</pointInstruction>
				<xsl:if test="($depthval mod 1) > 0">
				<pointInstruction>
					<featureReference><xsl:value-of select="$fid"/></featureReference>
										<xsl:if test="$sid != '' "><spatialReference><xsl:value-of select="$sid"/></spatialReference></xsl:if>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<xsl:element name="symbol">
						<xsl:attribute name="reference"><xsl:value-of select="concat($prefix,'5',substring($digits,2,1))"/></xsl:attribute> 
					</xsl:element>
				</pointInstruction>
				</xsl:if>
			</xsl:when>
			<xsl:when test="31 > $depthval and ($depthval mod 1) > 0"> <!-- algorithm 2 -->
				<xsl:variable name="digits" select="$depthval * 10" />
				<pointInstruction>
					<featureReference><xsl:value-of select="$fid"/></featureReference>
										<xsl:if test="$sid != '' "><spatialReference><xsl:value-of select="$sid"/></spatialReference></xsl:if>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<xsl:element name="symbol">
						<xsl:attribute name="reference"><xsl:value-of select="concat($prefix,'2',substring($digits,1,1))"/></xsl:attribute> 
					</xsl:element>
				</pointInstruction>
				<pointInstruction>
					<featureReference><xsl:value-of select="$fid"/></featureReference>
										<xsl:if test="$sid != '' "><spatialReference><xsl:value-of select="$sid"/></spatialReference></xsl:if>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<xsl:element name="symbol">
						<xsl:attribute name="reference"><xsl:value-of select="concat($prefix,'1',substring($digits,2,1))"/></xsl:attribute> 
					</xsl:element>
				</pointInstruction>
				<pointInstruction>
					<featureReference><xsl:value-of select="$fid"/></featureReference>
										<xsl:if test="$sid != '' "><spatialReference><xsl:value-of select="$sid"/></spatialReference></xsl:if>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<xsl:element name="symbol">
						<xsl:attribute name="reference"><xsl:value-of select="concat($prefix,'5',substring($digits,3,1))"/></xsl:attribute> 
					</xsl:element>
				</pointInstruction>
			</xsl:when>
			<xsl:when test="100 > $depth "> <!-- algorithm 3 -->
				<pointInstruction>
					<featureReference><xsl:value-of select="$fid"/></featureReference>
										<xsl:if test="$sid != '' "><spatialReference><xsl:value-of select="$sid"/></spatialReference></xsl:if>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<xsl:element name="symbol">
						<xsl:attribute name="reference"><xsl:value-of select="concat($prefix,'1',substring($depthval,1,1))"/></xsl:attribute> 
					</xsl:element>
				</pointInstruction>
				<pointInstruction>
					<featureReference><xsl:value-of select="$fid"/></featureReference>
										<xsl:if test="$sid != '' "><spatialReference><xsl:value-of select="$sid"/></spatialReference></xsl:if>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<xsl:element name="symbol">
						<xsl:attribute name="reference"><xsl:value-of select="concat($prefix,'0',substring($depthval,2,1))"/></xsl:attribute> 
					</xsl:element>
				</pointInstruction>
			</xsl:when>
			<xsl:when test="1000 > $depth "> <!-- algorithm 4 -->
				<pointInstruction>
					<featureReference><xsl:value-of select="$fid"/></featureReference>
										<xsl:if test="$sid != '' "><spatialReference><xsl:value-of select="$sid"/></spatialReference></xsl:if>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<xsl:element name="symbol">
						<xsl:attribute name="reference"><xsl:value-of select="concat($prefix,'2',substring($depthval,1,1))"/></xsl:attribute> 
					</xsl:element>
				</pointInstruction>
				<pointInstruction>
					<featureReference><xsl:value-of select="$fid"/></featureReference>
										<xsl:if test="$sid != '' "><spatialReference><xsl:value-of select="$sid"/></spatialReference></xsl:if>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<xsl:element name="symbol">
						<xsl:attribute name="reference"><xsl:value-of select="concat($prefix,'1',substring($depthval,2,1))"/></xsl:attribute> 
					</xsl:element>
				</pointInstruction>
				<pointInstruction>
					<featureReference><xsl:value-of select="$fid"/></featureReference>
										<xsl:if test="$sid != '' "><spatialReference><xsl:value-of select="$sid"/></spatialReference></xsl:if>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<xsl:element name="symbol">
						<xsl:attribute name="reference"><xsl:value-of select="concat($prefix,'0',substring($depthval,3,1))"/></xsl:attribute> 
					</xsl:element>
				</pointInstruction>
			</xsl:when>
			<xsl:when test="10000 > $depth "> <!-- algorithm 5 -->
				<pointInstruction>
					<featureReference><xsl:value-of select="$fid"/></featureReference>
										<xsl:if test="$sid != '' "><spatialReference><xsl:value-of select="$sid"/></spatialReference></xsl:if>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<xsl:element name="symbol">
						<xsl:attribute name="reference"><xsl:value-of select="concat($prefix,'2',substring($depthval,1,1))"/></xsl:attribute> 
					</xsl:element>
				</pointInstruction>
				<pointInstruction>
					<featureReference><xsl:value-of select="$fid"/></featureReference>
										<xsl:if test="$sid != '' "><spatialReference><xsl:value-of select="$sid"/></spatialReference></xsl:if>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<xsl:element name="symbol">
						<xsl:attribute name="reference"><xsl:value-of select="concat($prefix,'1',substring($depthval,2,1))"/></xsl:attribute> 
					</xsl:element>
				</pointInstruction>
				<pointInstruction>
					<featureReference><xsl:value-of select="$fid"/></featureReference>
										<xsl:if test="$sid != '' "><spatialReference><xsl:value-of select="$sid"/></spatialReference></xsl:if>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<xsl:element name="symbol">
						<xsl:attribute name="reference"><xsl:value-of select="concat($prefix,'0',substring($depthval,3,1))"/></xsl:attribute> 
					</xsl:element>
				</pointInstruction>
				<pointInstruction>
					<featureReference><xsl:value-of select="$fid"/></featureReference>
										<xsl:if test="$sid != '' "><spatialReference><xsl:value-of select="$sid"/></spatialReference></xsl:if>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<xsl:element name="symbol">
						<xsl:attribute name="reference"><xsl:value-of select="concat($prefix,'4',substring($depthval,4,1))"/></xsl:attribute> 
					</xsl:element>
				</pointInstruction>
			</xsl:when>
			<xsl:otherwise> <!-- algorithm 6 -->
				<pointInstruction>
					<featureReference><xsl:value-of select="$fid"/></featureReference>
										<xsl:if test="$sid != '' "><spatialReference><xsl:value-of select="$sid"/></spatialReference></xsl:if>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<xsl:element name="symbol">
						<xsl:attribute name="reference"><xsl:value-of select="concat($prefix,'3',substring($depthval,1,1))"/></xsl:attribute> 
					</xsl:element>
				</pointInstruction>
				<pointInstruction>
					<featureReference><xsl:value-of select="$fid"/></featureReference>
										<xsl:if test="$sid != '' "><spatialReference><xsl:value-of select="$sid"/></spatialReference></xsl:if>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<xsl:element name="symbol">
						<xsl:attribute name="reference"><xsl:value-of select="concat($prefix,'2',substring($depthval,2,1))"/></xsl:attribute> 
					</xsl:element>
				</pointInstruction>
				<pointInstruction>
					<featureReference><xsl:value-of select="$fid"/></featureReference>
										<xsl:if test="$sid != '' "><spatialReference><xsl:value-of select="$sid"/></spatialReference></xsl:if>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<xsl:element name="symbol">
						<xsl:attribute name="reference"><xsl:value-of select="concat($prefix,'1',substring($depthval,3,1))"/></xsl:attribute> 
					</xsl:element>
				</pointInstruction>
				<pointInstruction>
					<featureReference><xsl:value-of select="$fid"/></featureReference>
										<xsl:if test="$sid != '' "><spatialReference><xsl:value-of select="$sid"/></spatialReference></xsl:if>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<xsl:element name="symbol">
						<xsl:attribute name="reference"><xsl:value-of select="concat($prefix,'0',substring($depthval,4,1))"/></xsl:attribute> 
					</xsl:element>
				</pointInstruction>
				<pointInstruction>
					<featureReference><xsl:value-of select="$fid"/></featureReference>
										<xsl:if test="$sid != '' "><spatialReference><xsl:value-of select="$sid"/></spatialReference></xsl:if>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<xsl:element name="symbol">
						<xsl:attribute name="reference"><xsl:value-of select="concat($prefix,'4',substring($depthval,5,1))"/></xsl:attribute> 
					</xsl:element>
				</pointInstruction>
			</xsl:otherwise>
		</xsl:choose>
		
	</xsl:if>		
	</xsl:template>

	
	<!-- Sept, 2016 Named template for soundings, using augmented point geometry -->
	<xsl:template name="SNDFRM04P">
	<xsl:param name="viewingGroup"/> <!-- The viewing group -->
	<xsl:param name="displayPlane"/> <!-- The display plane -->
	<xsl:param name="drawingPriority"/> <!-- The drawing priority -->
	<xsl:param name="fid"/> <!-- If feature, then feature id -->
	<xsl:param name="sid"/> <!-- If spatial, then spatial id -->
	<xsl:param name="depth"/> <!-- The depth value -->
	<xsl:param name="tecsou"/> <!-- technique of sounding measurement -->
	<xsl:param name="quasou"/> <!-- quality of sounding measurement -->
	<xsl:param name="status"/> <!-- status of the object -->
	<xsl:param name="quapos"/> <!-- spatial quality of position -->
	<xsl:param name="x"/> <!-- the x position of the sounding -->
	<xsl:param name="y"/> <!-- the y position of the sounding -->

	<xsl:if test="number($depth) = $depth">  <!-- if depth is not a number do nothing -->

		<!-- variable to hold adjusted depth -->
		<xsl:variable name="depthval">
			<xsl:choose>
				<xsl:when test="$depth > 31 ">
					<!-- truncate the depth to an integer -->
					<xsl:value-of select="floor($depth)"/>
				</xsl:when>
				<xsl:when test="0 > $depth ">
					<!-- turn it positive -->
					<xsl:value-of select="0 - $depth"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select="$depth"/>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		
		<!-- variable to hold prefix -->
		<xsl:variable name="prefix">
			<xsl:choose>
				<xsl:when test="$depth > $SAFETY_DEPTH">
					<!-- truncate the depth to an integer -->
					<xsl:text>SOUNDG</xsl:text>
				</xsl:when>
				<xsl:otherwise>
					<xsl:text>SOUNDS</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<xsl:if test="$tecsou !='' and ($tecsou = '4' or $tecsou = '6')">
   				<augmentedPoint>
					<featureReference><xsl:value-of select="$fid"/></featureReference>
					<xsl:if test="$sid != '' "><spatialReference><xsl:value-of select="$sid"/></spatialReference></xsl:if>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<crs>GeographicCRS</crs>
					<position>
						<x><xsl:value-of select="$x"/></x>
						<y><xsl:value-of select="$y"/></y>
					</position>
					<xsl:element name="symbol">
						<xsl:attribute name="reference"><xsl:value-of select="concat($prefix,'B1')"/></xsl:attribute> 
					</xsl:element>
				</augmentedPoint>
		</xsl:if>
		<xsl:if test="($quasou !='' and ($quasou = '3' or $quasou = '4' or $quasou = '5' or $quasou = '8' or $quasou = '9')) 
		or $status = '18' or ($quapos !='' and not($quapos = '1' or $quapos = '10' or $quapos = '11' ) )">
				<augmentedPoint>
					<featureReference><xsl:value-of select="$fid"/></featureReference>
					<xsl:if test="$sid != '' "><spatialReference><xsl:value-of select="$sid"/></spatialReference></xsl:if>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<crs>GeographicCRS</crs>
					<position>
						<x><xsl:value-of select="$x"/></x>
						<y><xsl:value-of select="$y"/></y>
					</position>
					<xsl:element name="symbol">
						<xsl:attribute name="reference"><xsl:value-of select="concat($prefix,'C2')"/></xsl:attribute> 
					</xsl:element>
				</augmentedPoint>
		</xsl:if>
		
		<xsl:if test="0 > $depth">
				<augmentedPoint>
					<featureReference><xsl:value-of select="$fid"/></featureReference>
					<xsl:if test="$sid != '' "><spatialReference><xsl:value-of select="$sid"/></spatialReference></xsl:if>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<crs>GeographicCRS</crs>
					<position>
						<x><xsl:value-of select="$x"/></x>
						<y><xsl:value-of select="$y"/></y>
					</position>
					<xsl:element name="symbol">
						<xsl:attribute name="reference"><xsl:value-of select="concat($prefix,'A1')"/></xsl:attribute> 
					</xsl:element>
				</augmentedPoint>
		</xsl:if>

		<xsl:choose>
			<xsl:when test="1 > $depthval "> 		<!-- algorithm 1a -->
				<xsl:variable name="digits" select="$depthval * 10" />
				<augmentedPoint>
					<featureReference><xsl:value-of select="$fid"/></featureReference>
					<xsl:if test="$sid != '' "><spatialReference><xsl:value-of select="$sid"/></spatialReference></xsl:if>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<crs>GeographicCRS</crs>
					<position>
						<x><xsl:value-of select="$x"/></x>
						<y><xsl:value-of select="$y"/></y>
					</position>
					<xsl:element name="symbol">
						<xsl:attribute name="reference"><xsl:value-of select="concat($prefix,'1','0')"/></xsl:attribute> 
					</xsl:element>
				</augmentedPoint>
				<augmentedPoint>
					<featureReference><xsl:value-of select="$fid"/></featureReference>
					<xsl:if test="$sid != '' "><spatialReference><xsl:value-of select="$sid"/></spatialReference></xsl:if>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<crs>GeographicCRS</crs>
					<position>
						<x><xsl:value-of select="$x"/></x>
						<y><xsl:value-of select="$y"/></y>
					</position>
					<xsl:element name="symbol">
						<xsl:attribute name="reference"><xsl:value-of select="concat($prefix,'5',substring($digits,1,1))"/></xsl:attribute> 
					</xsl:element>
				</augmentedPoint>
			</xsl:when>
			<xsl:when test="10 > $depthval "> 		<!-- algorithm 1b -->
				<xsl:variable name="digits" select="$depthval * 10" />
				<augmentedPoint>
					<featureReference><xsl:value-of select="$fid"/></featureReference>
					<xsl:if test="$sid != '' "><spatialReference><xsl:value-of select="$sid"/></spatialReference></xsl:if>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<crs>GeographicCRS</crs>
					<position>
						<x><xsl:value-of select="$x"/></x>
						<y><xsl:value-of select="$y"/></y>
					</position>
					<xsl:element name="symbol">
						<xsl:attribute name="reference"><xsl:value-of select="concat($prefix,'1',substring($digits,1,1))"/></xsl:attribute> 
					</xsl:element>
				</augmentedPoint>
				<xsl:if test="($depthval mod 1) > 0">
				<augmentedPoint>
					<featureReference><xsl:value-of select="$fid"/></featureReference>
					<xsl:if test="$sid != '' "><spatialReference><xsl:value-of select="$sid"/></spatialReference></xsl:if>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<crs>GeographicCRS</crs>
					<position>
						<x><xsl:value-of select="$x"/></x>
						<y><xsl:value-of select="$y"/></y>
					</position>
					<xsl:element name="symbol">
						<xsl:attribute name="reference"><xsl:value-of select="concat($prefix,'5',substring($digits,2,1))"/></xsl:attribute> 
					</xsl:element>
				</augmentedPoint>
				</xsl:if>
			</xsl:when>
			<xsl:when test="31 > $depthval and ($depthval mod 1) > 0"> <!-- algorithm 2 -->
				<xsl:variable name="digits" select="$depthval * 10" />
				<augmentedPoint>
					<featureReference><xsl:value-of select="$fid"/></featureReference>
					<xsl:if test="$sid != '' "><spatialReference><xsl:value-of select="$sid"/></spatialReference></xsl:if>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<crs>GeographicCRS</crs>
					<position>
						<x><xsl:value-of select="$x"/></x>
						<y><xsl:value-of select="$y"/></y>
					</position>
					<xsl:element name="symbol">
						<xsl:attribute name="reference"><xsl:value-of select="concat($prefix,'2',substring($digits,1,1))"/></xsl:attribute> 
					</xsl:element>
				</augmentedPoint>
				<augmentedPoint>
					<featureReference><xsl:value-of select="$fid"/></featureReference>
					<xsl:if test="$sid != '' "><spatialReference><xsl:value-of select="$sid"/></spatialReference></xsl:if>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<crs>GeographicCRS</crs>
					<position>
						<x><xsl:value-of select="$x"/></x>
						<y><xsl:value-of select="$y"/></y>
					</position>
					<xsl:element name="symbol">
						<xsl:attribute name="reference"><xsl:value-of select="concat($prefix,'1',substring($digits,2,1))"/></xsl:attribute> 
					</xsl:element>
				</augmentedPoint>
				<augmentedPoint>
					<featureReference><xsl:value-of select="$fid"/></featureReference>
					<xsl:if test="$sid != '' "><spatialReference><xsl:value-of select="$sid"/></spatialReference></xsl:if>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<crs>GeographicCRS</crs>
					<position>
						<x><xsl:value-of select="$x"/></x>
						<y><xsl:value-of select="$y"/></y>
					</position>
					<xsl:element name="symbol">
						<xsl:attribute name="reference"><xsl:value-of select="concat($prefix,'5',substring($digits,3,1))"/></xsl:attribute> 
					</xsl:element>
				</augmentedPoint>
			</xsl:when>
			<xsl:when test="100 > $depth "> <!-- algorithm 3 -->
				<augmentedPoint>
					<featureReference><xsl:value-of select="$fid"/></featureReference>
					<xsl:if test="$sid != '' "><spatialReference><xsl:value-of select="$sid"/></spatialReference></xsl:if>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<crs>GeographicCRS</crs>
					<position>
						<x><xsl:value-of select="$x"/></x>
						<y><xsl:value-of select="$y"/></y>
					</position>
					<xsl:element name="symbol">
						<xsl:attribute name="reference"><xsl:value-of select="concat($prefix,'1',substring($depthval,1,1))"/></xsl:attribute> 
					</xsl:element>
				</augmentedPoint>
				<augmentedPoint>
					<featureReference><xsl:value-of select="$fid"/></featureReference>
					<xsl:if test="$sid != '' "><spatialReference><xsl:value-of select="$sid"/></spatialReference></xsl:if>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<crs>GeographicCRS</crs>
					<position>
						<x><xsl:value-of select="$x"/></x>
						<y><xsl:value-of select="$y"/></y>
					</position>
					<xsl:element name="symbol">
						<xsl:attribute name="reference"><xsl:value-of select="concat($prefix,'0',substring($depthval,2,1))"/></xsl:attribute> 
					</xsl:element>
				</augmentedPoint>
			</xsl:when>
			<xsl:when test="1000 > $depth "> <!-- algorithm 4 -->
				<augmentedPoint>
					<featureReference><xsl:value-of select="$fid"/></featureReference>
					<xsl:if test="$sid != '' "><spatialReference><xsl:value-of select="$sid"/></spatialReference></xsl:if>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<crs>GeographicCRS</crs>
					<position>
						<x><xsl:value-of select="$x"/></x>
						<y><xsl:value-of select="$y"/></y>
					</position>
					<xsl:element name="symbol">
						<xsl:attribute name="reference"><xsl:value-of select="concat($prefix,'2',substring($depthval,1,1))"/></xsl:attribute> 
					</xsl:element>
				</augmentedPoint>
				<augmentedPoint>
					<featureReference><xsl:value-of select="$fid"/></featureReference>
					<xsl:if test="$sid != '' "><spatialReference><xsl:value-of select="$sid"/></spatialReference></xsl:if>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<crs>GeographicCRS</crs>
					<position>
						<x><xsl:value-of select="$x"/></x>
						<y><xsl:value-of select="$y"/></y>
					</position>
					<xsl:element name="symbol">
						<xsl:attribute name="reference"><xsl:value-of select="concat($prefix,'1',substring($depthval,2,1))"/></xsl:attribute> 
					</xsl:element>
				</augmentedPoint>
				<augmentedPoint>
					<featureReference><xsl:value-of select="$fid"/></featureReference>
					<xsl:if test="$sid != '' "><spatialReference><xsl:value-of select="$sid"/></spatialReference></xsl:if>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<crs>GeographicCRS</crs>
					<position>
						<x><xsl:value-of select="$x"/></x>
						<y><xsl:value-of select="$y"/></y>
					</position>
					<xsl:element name="symbol">
						<xsl:attribute name="reference"><xsl:value-of select="concat($prefix,'0',substring($depthval,3,1))"/></xsl:attribute> 
					</xsl:element>
				</augmentedPoint>
			</xsl:when>
			<xsl:when test="10000 > $depth "> <!-- algorithm 5 -->
				<augmentedPoint>
					<featureReference><xsl:value-of select="$fid"/></featureReference>
					<xsl:if test="$sid != '' "><spatialReference><xsl:value-of select="$sid"/></spatialReference></xsl:if>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<crs>GeographicCRS</crs>
					<position>
						<x><xsl:value-of select="$x"/></x>
						<y><xsl:value-of select="$y"/></y>
					</position>
					<xsl:element name="symbol">
						<xsl:attribute name="reference"><xsl:value-of select="concat($prefix,'2',substring($depthval,1,1))"/></xsl:attribute> 
					</xsl:element>
				</augmentedPoint>
				<augmentedPoint>
					<featureReference><xsl:value-of select="$fid"/></featureReference>
					<xsl:if test="$sid != '' "><spatialReference><xsl:value-of select="$sid"/></spatialReference></xsl:if>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<crs>GeographicCRS</crs>
					<position>
						<x><xsl:value-of select="$x"/></x>
						<y><xsl:value-of select="$y"/></y>
					</position>
					<xsl:element name="symbol">
						<xsl:attribute name="reference"><xsl:value-of select="concat($prefix,'1',substring($depthval,2,1))"/></xsl:attribute> 
					</xsl:element>
				</augmentedPoint>
				<augmentedPoint>
					<featureReference><xsl:value-of select="$fid"/></featureReference>
					<xsl:if test="$sid != '' "><spatialReference><xsl:value-of select="$sid"/></spatialReference></xsl:if>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<crs>GeographicCRS</crs>
					<position>
						<x><xsl:value-of select="$x"/></x>
						<y><xsl:value-of select="$y"/></y>
					</position>
					<xsl:element name="symbol">
						<xsl:attribute name="reference"><xsl:value-of select="concat($prefix,'0',substring($depthval,3,1))"/></xsl:attribute> 
					</xsl:element>
				</augmentedPoint>
				<augmentedPoint>
					<featureReference><xsl:value-of select="$fid"/></featureReference>
					<xsl:if test="$sid != '' "><spatialReference><xsl:value-of select="$sid"/></spatialReference></xsl:if>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<crs>GeographicCRS</crs>
					<position>
						<x><xsl:value-of select="$x"/></x>
						<y><xsl:value-of select="$y"/></y>
					</position>
					<xsl:element name="symbol">
						<xsl:attribute name="reference"><xsl:value-of select="concat($prefix,'4',substring($depthval,4,1))"/></xsl:attribute> 
					</xsl:element>
				</augmentedPoint>
			</xsl:when>
			<xsl:otherwise> <!-- algorithm 6 -->
				<augmentedPoint>
					<featureReference><xsl:value-of select="$fid"/></featureReference>
					<xsl:if test="$sid != '' "><spatialReference><xsl:value-of select="$sid"/></spatialReference></xsl:if>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<crs>GeographicCRS</crs>
					<position>
						<x><xsl:value-of select="$x"/></x>
						<y><xsl:value-of select="$y"/></y>
					</position>
					<xsl:element name="symbol">
						<xsl:attribute name="reference"><xsl:value-of select="concat($prefix,'3',substring($depthval,1,1))"/></xsl:attribute> 
					</xsl:element>
				</augmentedPoint>
				<augmentedPoint>
					<featureReference><xsl:value-of select="$fid"/></featureReference>
					<xsl:if test="$sid != '' "><spatialReference><xsl:value-of select="$sid"/></spatialReference></xsl:if>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<crs>GeographicCRS</crs>
					<position>
						<x><xsl:value-of select="$x"/></x>
						<y><xsl:value-of select="$y"/></y>
					</position>
					<xsl:element name="symbol">
						<xsl:attribute name="reference"><xsl:value-of select="concat($prefix,'2',substring($depthval,2,1))"/></xsl:attribute> 
					</xsl:element>
				</augmentedPoint>
				<augmentedPoint>
					<featureReference><xsl:value-of select="$fid"/></featureReference>
					<xsl:if test="$sid != '' "><spatialReference><xsl:value-of select="$sid"/></spatialReference></xsl:if>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<crs>GeographicCRS</crs>
					<position>
						<x><xsl:value-of select="$x"/></x>
						<y><xsl:value-of select="$y"/></y>
					</position>
					<xsl:element name="symbol">
						<xsl:attribute name="reference"><xsl:value-of select="concat($prefix,'1',substring($depthval,3,1))"/></xsl:attribute> 
					</xsl:element>
				</augmentedPoint>
				<augmentedPoint>
					<featureReference><xsl:value-of select="$fid"/></featureReference>
					<xsl:if test="$sid != '' "><spatialReference><xsl:value-of select="$sid"/></spatialReference></xsl:if>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<crs>GeographicCRS</crs>
					<position>
						<x><xsl:value-of select="$x"/></x>
						<y><xsl:value-of select="$y"/></y>
					</position>
					<xsl:element name="symbol">
						<xsl:attribute name="reference"><xsl:value-of select="concat($prefix,'0',substring($depthval,4,1))"/></xsl:attribute> 
					</xsl:element>
				</augmentedPoint>
				<augmentedPoint>
					<featureReference><xsl:value-of select="$fid"/></featureReference>
					<xsl:if test="$sid != '' "><spatialReference><xsl:value-of select="$sid"/></spatialReference></xsl:if>
					<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
					<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
					<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
					<crs>GeographicCRS</crs>
					<position>
						<x><xsl:value-of select="$x"/></x>
						<y><xsl:value-of select="$y"/></y>
					</position>
					<xsl:element name="symbol">
						<xsl:attribute name="reference"><xsl:value-of select="concat($prefix,'4',substring($depthval,5,1))"/></xsl:attribute> 
					</xsl:element>
				</augmentedPoint>
			</xsl:otherwise>
		</xsl:choose>
		
	</xsl:if>		
	</xsl:template>
	
</xsl:transform>
