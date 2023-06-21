<?xml version="1.0" encoding="UTF-8"?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
			<xsl:output method="xml" encoding="UTF-8" indent="yes"/>

	<!-- Updated colour to symbol lookup Sept 2016 -->
	<!-- Added test for colocated non sectored lights Nov 2016 -->

	<!-- create key index to quickly lookup lights using the point reference id as the lookup key -->	
	<xsl:key name="NonSectoredLights" match="/Dataset/Features/Light " use="Point/@ref"/>
	
	<!--Template for: Lights-->
	<xsl:template name="LIGHTS">

		<!-- Determine the orientation of the symbol -->
		<xsl:variable name="Orient">
			<xsl:choose>
				<xsl:when test="flareAngle !='' ">
					<xsl:value-of select="flareAngle"/>
				</xsl:when>
				<!-- Check if other non sector lights are at the same location, using same point -->
				<xsl:when test=" count(key('NonSectoredLights', Point/@ref)) &gt; 1  and (colour = '1' or colour = '6' or colour = '11') ">
					<xsl:text>45</xsl:text>
				</xsl:when>
				<xsl:otherwise>
					<xsl:text>135</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		
		<!-- Determine the symbol -->
		<xsl:variable name="Symbol">
			<xsl:choose>
				<xsl:when test="colour = '3' ">
					<xsl:text>LIGHTS11</xsl:text>
				</xsl:when>
				<xsl:when test="colour = '4' ">
					<xsl:text>LIGHTS12</xsl:text>
				</xsl:when>
				<xsl:when test="colour = '11' ">
					<xsl:text>LIGHTS13</xsl:text>
				</xsl:when>
				<xsl:when test="colour = '5' and colour = '6' ">
					<xsl:text>LIGHTS13</xsl:text>
				</xsl:when>
				<xsl:when test="colour = '6' ">
					<xsl:text>LIGHTS13</xsl:text>
				</xsl:when>
				<xsl:when test="colour = '1' ">
					<xsl:text>LIGHTS13</xsl:text>
				</xsl:when>
				<xsl:otherwise>
					<xsl:text>LITDEF11</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
        <pointInstruction>
			<featureReference><xsl:value-of select="@id"/></featureReference>
            <viewingGroup>27071</viewingGroup>
            <displayPlane>OVERRADAR</displayPlane>
            <drawingPriority>24</drawingPriority>
			<xsl:element name="symbol">
				<xsl:attribute name="reference"><xsl:value-of select="$Symbol"/></xsl:attribute> 
				<xsl:attribute name="rotation"><xsl:value-of select="$Orient"/></xsl:attribute> 
			</xsl:element>
        </pointInstruction>

		<xsl:variable name="At45">
			<xsl:choose>
				<xsl:when test="$Orient = '45' ">
					<xsl:text>true</xsl:text>
				</xsl:when>
				<xsl:otherwise>
					<xsl:text>false</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<!-- Add light description -->
		<xsl:call-template name="LIGHTDESCRIPTION">
			<xsl:with-param name="flareAt45" select="$At45"/>
		</xsl:call-template>

	</xsl:template>
	<!--Template for: Lights-->
	<xsl:template name="majorLights">
	
		<xsl:variable name="colourToken">
			<xsl:choose>
				<xsl:when test="colour = '1' and colour = '3'" >
					<xsl:text>LITRD</xsl:text>
				</xsl:when> 
				<xsl:when test=" colour = '3'" >
					<xsl:text>LITRD</xsl:text>
				</xsl:when> 
				<xsl:when test="colour = '1' and colour = '4'" >
					<xsl:text>LITGN</xsl:text>
				</xsl:when> 
				<xsl:when test="colour = '4'" >
					<xsl:text>LITGN</xsl:text>
				</xsl:when> 
				<xsl:when test="colour = '5' and colour = '6' " >
					<xsl:text>LITYW</xsl:text>
				</xsl:when> 
				<xsl:when test="colour = '11' or colour = '6' or colour = '1' " >
					<xsl:text>LITYW</xsl:text>
				</xsl:when> 
				<xsl:otherwise>
					<xsl:text>CHMGD</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<!--  An augmented Path for the sector arc Local CRS -->
   		<augmentedPath>
			<featureReference><xsl:value-of select="@id"/></featureReference>
            <viewingGroup>27071</viewingGroup>
            <displayPlane>OVERRADAR</displayPlane>
            <drawingPriority>24</drawingPriority>
			<crs>LocalCRS</crs>
			<!-- add a linestlye element -->
			<xsl:call-template name="simpleLineStyle">
				<xsl:with-param name="style">solid</xsl:with-param>
				<xsl:with-param name="width">1.28</xsl:with-param>
				<xsl:with-param name="colour">OUTLW</xsl:with-param>
			</xsl:call-template>
			<path>
				<arcByRadius> 
					<center> 
						<x>0</x> 
						<y>0</y> 
					</center> 
					<sector> 
						<startAngle>0</startAngle> 
						<angularDistance>360</angularDistance> 
					</sector> 
					<radius>26</radius> 
				</arcByRadius> 
			</path> 
		</augmentedPath>
   		<augmentedPath>
			<featureReference><xsl:value-of select="@id"/></featureReference>
            <viewingGroup>27071</viewingGroup>
            <displayPlane>OVERRADAR</displayPlane>
            <drawingPriority>24</drawingPriority>
			<crs>LocalCRS</crs>
			<!-- add a linestlye element -->
			<xsl:call-template name="simpleLineStyle">
				<xsl:with-param name="style">solid</xsl:with-param>
				<xsl:with-param name="width">0.64</xsl:with-param>
				<xsl:with-param name="colour"><xsl:value-of select="$colourToken"/></xsl:with-param>
			</xsl:call-template>
			<path>
				<arcByRadius> 
					<center> 
						<x>0</x> 
						<y>0</y> 
					</center> 
					<sector> 
						<startAngle>0</startAngle> 
						<angularDistance>360</angularDistance> 
					</sector> 
					<radius>26</radius> 
				</arcByRadius> 
			</path> 
		</augmentedPath>


		<!-- Add light description -->
		<xsl:call-template name="LIGHTDESCRIPTION">
		</xsl:call-template>

	</xsl:template>

	<xsl:template match="Light[@primitive = 'Point']" priority="1">
		<xsl:choose>
			<xsl:when test="majorLight = 'true' ">
				<!-- Call a common lights template for major allround lights -->
				<xsl:call-template name="majorLights"/>
			</xsl:when>
			<!--<xsl:when test="valueOfNominalRange  &gt;= 10 and not(categoryOfLight = 5) and not(lightCharacteristic = 12) ">
				<xsl:call-template name="majorLights"/>
			</xsl:when>  -->
			<xsl:otherwise>
				<!-- Call a common lights template -->
				<xsl:call-template name="LIGHTS"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

  <xsl:template match="Light[@primitive = 'Point' and categoryOfLight = 6 ]" priority="2">
	<!--<xsl:template match="LightAirObstruction[@primitive = 'Point']" priority="1">-->
		<!-- Call a common lights template -->
		<xsl:call-template name="LIGHTS"/>
	</xsl:template>

  <xsl:template match="Light[@primitive = 'Point' and categoryOfLight = 7 ]" priority="2">
	<!--<xsl:template match="LightFogDetector[@primitive = 'Point']" priority="1">-->
		<xsl:choose>
			<xsl:when test="majorLight = 'true' ">
				<!-- Call a common lights template for major allround lights -->
				<xsl:call-template name="majorLights"/>
			</xsl:when>
			<!--<xsl:when test="valueOfNominalRange  &gt;= 10 and not(categoryOfLight = 5) and not(lightCharacteristic = 12) ">
				<xsl:call-template name="majorLights"/>
			</xsl:when>  -->
			<xsl:otherwise>
				<!-- Call a common lights template -->
				<xsl:call-template name="LIGHTS"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
	
	<!--Template for: spot light or flood Light-->
	<xsl:template match="Light[@primitive = 'Point' and (categoryOfLight = 11 or categoryOfLight = 8) ]" priority="2">
        <pointInstruction>
			<featureReference><xsl:value-of select="@id"/></featureReference>
            <viewingGroup>27070</viewingGroup>
            <displayPlane>OVERRADAR</displayPlane>
            <drawingPriority>24</drawingPriority>
			<xsl:element name="symbol">
				<xsl:attribute name="reference">LIGHTS82</xsl:attribute> 
			</xsl:element>
        </pointInstruction>
	</xsl:template>

  <!--Template for: strip Light-->
  <xsl:template match="Light[@primitive = 'Point' and categoryOfLight = 9]" priority="2">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>27070</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>24</drawingPriority>
      <xsl:element name="symbol">
        <xsl:attribute name="reference">LIGHTS81</xsl:attribute>
      </xsl:element>
    </pointInstruction>
  </xsl:template>


</xsl:transform>
