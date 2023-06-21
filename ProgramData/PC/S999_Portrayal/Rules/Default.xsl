<?xml version="1.0" encoding="UTF-8"?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
	<xsl:output method="xml" encoding="UTF-8" indent="yes"/>
	<xsl:template match="*[@primitive='Curve']">
		<lineInstruction>
			<featureReference>
				<xsl:value-of select="@id"/>
			</featureReference>
			<viewingGroup>36050</viewingGroup>
			<displayPlane>UNDERRADAR</displayPlane>
			<drawingPriority>6</drawingPriority>
			<xsl:call-template name="simpleLineStyle">
				<xsl:with-param name="style">dash</xsl:with-param>
				<xsl:with-param name="width">0.32</xsl:with-param>
				<xsl:with-param name="colour">CHGRF</xsl:with-param>
			</xsl:call-template>
		</lineInstruction>
	</xsl:template>
	<xsl:template match="*[@primitive='Point']">
		<pointInstruction>
			<featureReference>
				<xsl:value-of select="@id"/>
			</featureReference>
			<viewingGroup>21010</viewingGroup>
			<displayPlane>OVERRADAR</displayPlane>
			<drawingPriority>15</drawingPriority>
			<symbol reference="USRPNT02"/>
		</pointInstruction>
	</xsl:template>
	<xsl:template match="*[@primitive='Surface']">
		<areaInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>53010</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>24</drawingPriority>
         <colorFill>
            <color transparency="0.75">DNGHL</color>
         </colorFill>
      </areaInstruction>
      <lineInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>53010</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>24</drawingPriority>
         <xsl:call-template name="simpleLineStyle">
            <xsl:with-param name="style">solid</xsl:with-param>
            <xsl:with-param name="width">0.32</xsl:with-param>
            <xsl:with-param name="colour">DNGHL</xsl:with-param>
         </xsl:call-template>
      </lineInstruction>
	</xsl:template>
</xsl:transform>
