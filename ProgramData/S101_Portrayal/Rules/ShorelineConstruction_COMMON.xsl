<?xml version="1.0" encoding="UTF-8"?>

<!-- modified Sept 2016, point SLCONS doesn't need to call SLCONS04 it just calls QUAPNT02 -->
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
   <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
   <xsl:template match="ShorelineConstruction[@primitive='Surface']" priority="1">
      <xsl:call-template name="SLCONS04">
         <xsl:with-param name="viewingGroup">12410</xsl:with-param>
         <xsl:with-param name="displayPlane">UNDERRADAR</xsl:with-param>
         <xsl:with-param name="drawingPriority">21</xsl:with-param>
      </xsl:call-template>
   </xsl:template>
   <xsl:template match="ShorelineConstruction[@primitive='Curve']" priority="1">
      <xsl:call-template name="SLCONS04">
         <xsl:with-param name="viewingGroup">12410</xsl:with-param>
         <xsl:with-param name="displayPlane">OVERRADAR</xsl:with-param>
         <xsl:with-param name="drawingPriority">21</xsl:with-param>
      </xsl:call-template>
   </xsl:template>
   <xsl:template match="ShorelineConstruction[@primitive='Point']" priority="1">
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>12410</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>24</drawingPriority>
         <symbol reference="MORFAC03"/>
      </pointInstruction>
      <xsl:call-template name="QUAPNT02">
         <xsl:with-param name="viewingGroup">31011</xsl:with-param>
         <xsl:with-param name="displayPlane">OVERRADAR</xsl:with-param>
         <xsl:with-param name="drawingPriority">24</xsl:with-param>
      </xsl:call-template>
   </xsl:template>
</xsl:transform>
