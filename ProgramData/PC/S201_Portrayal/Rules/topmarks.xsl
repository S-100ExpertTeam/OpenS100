<?xml version="1.0" encoding="UTF-8"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform"  version="1.0"><xsl:output method="xml" encoding="UTF-8" indent="yes"/>

<!-- The topmark symbol is generated from the calling template by calling it like this 

			<xsl:call-template name="FixedTopmark">
			<xsl:with-param name="featureReference" select="@id"/>
			<xsl:with-param name="viewingGroup">27020</xsl:with-param>
			<xsl:with-param name="displayPlane">OVERRADAR</xsl:with-param>
			<xsl:with-param name="drawingPriority">80</xsl:with-param>
			<xsl:with-param name="tds"><xsl:value-of select="tds"/></xsl:with-param>
			</xsl:call-template>
			-->

	<!-- Named template to pick symbol for fixed, Beacon topmark -->
	<xsl:template name="FixedTopmark">
	<xsl:param name="featureReference"/> <!-- Id of parent feature -->
	<xsl:param name="viewingGroup"/> <!-- viewing group for drawing instructions -->
	<xsl:param name="displayPlane"/> <!-- displayPlane for drawing instructions -->
	<xsl:param name="drawingPriority"/> <!-- drawingPriorityfor drawing instructions -->
		<!-- pick the right symbol -->
		<xsl:variable name="symbol">
			<xsl:choose>
				<xsl:when test="topmarkDaymarkShape = '1'">TOPMAR22</xsl:when>
				<xsl:when test="topmarkDaymarkShape = '2'">TOPMAR24</xsl:when>
				<xsl:when test="topmarkDaymarkShape = '3'">TOPMAR30</xsl:when>
				<xsl:when test="topmarkDaymarkShape = '4'">TOPMAR32</xsl:when>
				<xsl:when test="topmarkDaymarkShape = '5'">TOPMAR33</xsl:when>
				<xsl:when test="topmarkDaymarkShape = '6'">TOPMAR34</xsl:when>
				<xsl:when test="topmarkDaymarkShape = '7'">TOPMAR85</xsl:when>
				<xsl:when test="topmarkDaymarkShape = '8'">TOPMAR86</xsl:when>
				<xsl:when test="topmarkDaymarkShape = '9'">TOPMAR36</xsl:when>
				<xsl:when test="topmarkDaymarkShape = '10'">TOPMAR28</xsl:when>
				<xsl:when test="topmarkDaymarkShape = '11'">TOPMAR27</xsl:when>
				<xsl:when test="topmarkDaymarkShape = '12'">TOPMAR14</xsl:when>
				<xsl:when test="topmarkDaymarkShape = '13'">TOPMAR25</xsl:when>
				<xsl:when test="topmarkDaymarkShape = '14'">TOPMAR26</xsl:when>
				<xsl:when test="topmarkDaymarkShape = '15'">TOPMAR88</xsl:when>
				<xsl:when test="topmarkDaymarkShape = '16'">TOPMAR87</xsl:when>
				<xsl:when test="topmarkDaymarkShape = '17'">TMARDEF1</xsl:when>
				<xsl:when test="topmarkDaymarkShape = '18'">TOPMAR30</xsl:when>
				<xsl:when test="topmarkDaymarkShape = '19'">TOPMAR33</xsl:when>
				<xsl:when test="topmarkDaymarkShape = '20'">TOPMAR34</xsl:when>
				<xsl:when test="topmarkDaymarkShape = '21'">TOPMAR33</xsl:when>
				<xsl:when test="topmarkDaymarkShape = '22'">TOPMAR34</xsl:when>
				<xsl:when test="topmarkDaymarkShape = '23'">TOPMAR34</xsl:when>
				<xsl:when test="topmarkDaymarkShape = '24'">TOPMAR22</xsl:when>
				<xsl:when test="topmarkDaymarkShape = '25'">TOPMAR24</xsl:when>
				<xsl:when test="topmarkDaymarkShape = '26'">TOPMAR30</xsl:when>
				<xsl:when test="topmarkDaymarkShape = '27'">TOPMAR86</xsl:when>
				<xsl:when test="topmarkDaymarkShape = '28'">TOPMAR89</xsl:when>
				<xsl:when test="topmarkDaymarkShape = '29'">TOPMAR22</xsl:when>
				<xsl:when test="topmarkDaymarkShape = '30'">TOPMAR86</xsl:when>
				<xsl:when test="topmarkDaymarkShape = '31'">TOPMAR14</xsl:when>
				<xsl:when test="topmarkDaymarkShape = '32'">TOPMAR30</xsl:when>
				<xsl:otherwise>TMARDEF1</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<!-- now add the point symbol drawing instruction to the output -->
		<pointInstruction>
			<featureReference><xsl:value-of select="@id"/></featureReference>
			<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
			<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
			<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
			<xsl:element name="symbol">
				<xsl:attribute name="reference"><xsl:value-of select="$symbol"/></xsl:attribute> 
			</xsl:element>
		</pointInstruction>
	</xsl:template>
	
	
	<!-- Named template to pick symbol for floating, Buoy or light vessel topmark -->
	<xsl:template name="FloatingTopmark">
	<xsl:param name="featureReference"/> <!-- Id of parent feature -->
	<xsl:param name="viewingGroup"/> <!-- viewing group for drawing instructions -->
	<xsl:param name="displayPlane"/> <!-- displayPlane for drawing instructions -->
	<xsl:param name="drawingPriority"/> <!-- drawingPriorityfor drawing instructions -->

		<!-- pick the right symbol -->
		<xsl:variable name="symbol">
			<xsl:choose>
				<xsl:when test="topmarkDaymarkShape = '1'">TOPMAR02</xsl:when>
				<xsl:when test="topmarkDaymarkShape = '2'">TOPMAR04</xsl:when>
				<xsl:when test="topmarkDaymarkShape = '3'">TOPMAR10</xsl:when>
				<xsl:when test="topmarkDaymarkShape = '4'">TOPMAR12</xsl:when>
				<xsl:when test="topmarkDaymarkShape = '5'">TOPMAR13</xsl:when>
				<xsl:when test="topmarkDaymarkShape = '6'">TOPMAR14</xsl:when>
				<xsl:when test="topmarkDaymarkShape = '7'">TOPMAR65</xsl:when>
				<xsl:when test="topmarkDaymarkShape = '8'">TOPMAR17</xsl:when>
				<xsl:when test="topmarkDaymarkShape = '9'">TOPMAR16</xsl:when>
				<xsl:when test="topmarkDaymarkShape = '10'">TOPMAR08</xsl:when>
				<xsl:when test="topmarkDaymarkShape = '11'">TOPMAR07</xsl:when>
				<xsl:when test="topmarkDaymarkShape = '12'">TOPMAR14</xsl:when>
				<xsl:when test="topmarkDaymarkShape = '13'">TOPMAR05</xsl:when>
				<xsl:when test="topmarkDaymarkShape = '14'">TOPMAR06</xsl:when>
				<xsl:when test="topmarkDaymarkShape = '18'">TOPMAR10</xsl:when>
				<xsl:when test="topmarkDaymarkShape = '19'">TOPMAR13</xsl:when>
				<xsl:when test="topmarkDaymarkShape = '20'">TOPMAR14</xsl:when>
				<xsl:when test="topmarkDaymarkShape = '21'">TOPMAR13</xsl:when>
				<xsl:when test="topmarkDaymarkShape = '22'">TOPMAR14</xsl:when>
				<xsl:when test="topmarkDaymarkShape = '23'">TOPMAR14</xsl:when>
				<xsl:when test="topmarkDaymarkShape = '24'">TOPMAR02</xsl:when>
				<xsl:when test="topmarkDaymarkShape = '25'">TOPMAR04</xsl:when>
				<xsl:when test="topmarkDaymarkShape = '26'">TOPMAR10</xsl:when>
				<xsl:when test="topmarkDaymarkShape = '27'">TOPMAR17</xsl:when>
				<xsl:when test="topmarkDaymarkShape = '28'">TOPMAR18</xsl:when>
				<xsl:when test="topmarkDaymarkShape = '29'">TOPMAR02</xsl:when>
				<xsl:when test="topmarkDaymarkShape = '30'">TOPMAR17</xsl:when>
				<xsl:when test="topmarkDaymarkShape = '31'">TOPMAR14</xsl:when>
				<xsl:when test="topmarkDaymarkShape = '32'">TOPMAR10</xsl:when>
				<xsl:otherwise>TMARDEF2</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		
		<!-- now add the point symbol drawing instruction to the output -->
		<pointInstruction>
			<featureReference><xsl:value-of select="@id"/></featureReference>
			<viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
			<displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
			<drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
			<xsl:element name="symbol">
				<xsl:attribute name="reference"><xsl:value-of select="$symbol"/></xsl:attribute> 
			</xsl:element>
		</pointInstruction>
		
	</xsl:template>

	
	</xsl:transform>
