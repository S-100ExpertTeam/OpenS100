<?xml version="1.0" encoding="UTF-8"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
   <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
   <xsl:template match="FoulGround[@primitive='Point']" priority="1">
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>34050</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>12</drawingPriority>
         <symbol reference="FOULGND1"/>
      </pointInstruction>
   </xsl:template>
   <xsl:template match="FoulGround[@primitive='Point' and valueOfSounding != '']" priority="2">
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>34051</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>12</drawingPriority>
         <symbol reference="FOULGND1"/>
      </pointInstruction>
   </xsl:template>
   
   <xsl:template match="FoulGround[@primitive='Curve']" priority="1">
		<lineInstruction>
			<featureReference><xsl:value-of select="@id"/></featureReference>
			<viewingGroup>34050</viewingGroup>
			<displayPlane>OVERRADAR</displayPlane>
			<drawingPriority>12</drawingPriority>
			<xsl:call-template name="simpleLineStyle">
				<xsl:with-param name="style">dot</xsl:with-param>
				<xsl:with-param name="width">2</xsl:with-param>
				<xsl:with-param name="colour">CHBLK</xsl:with-param>
			</xsl:call-template>
		</lineInstruction>
   </xsl:template>

</xsl:transform>
