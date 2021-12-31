<?xml version="1.0" encoding="UTF-8"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
   <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
   <xsl:template match="WeedKelp[@primitive='Surface']" priority="1">
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>34020</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>9</drawingPriority>
         <symbol reference="WEDKLP03">
            <areaPlacement placementMode="VisibleParts"/>
         </symbol>
      </pointInstruction>
      <lineInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>34020</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>9</drawingPriority>
         <xsl:call-template name="simpleLineStyle">
            <xsl:with-param name="style">dash</xsl:with-param>
            <xsl:with-param name="width">0.32</xsl:with-param>
            <xsl:with-param name="colour">CHGRF</xsl:with-param>
         </xsl:call-template>
      </lineInstruction>
   </xsl:template>
   <xsl:template match="WeedKelp[@primitive='Point']" priority="1">
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>34020</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>9</drawingPriority>
         <symbol reference="WEDKLP03"/>
      </pointInstruction>
   </xsl:template>
</xsl:transform>
