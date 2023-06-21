<?xml version="1.0" encoding="UTF-8"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
   <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
   <xsl:template match="JointManagementZone[@primitive='Point']" priority="1">
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>40000</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>24</drawingPriority>
         <symbol reference="MSISYM01"/>
      </pointInstruction>
   </xsl:template>
   <xsl:template match="JointManagementZone[@primitive='Curve']" priority="1">
      <lineInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>40000</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>24</drawingPriority>
        <xsl:call-template name="simpleLineStyle">
          <xsl:with-param name="style">solid</xsl:with-param>
          <xsl:with-param name="width">0.64</xsl:with-param>
          <xsl:with-param name="colour">CHRED</xsl:with-param>
        </xsl:call-template>
      </lineInstruction>
   </xsl:template>
   <xsl:template match="JointManagementZone[@primitive='Surface']" priority="1">
      <lineInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>40000</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>24</drawingPriority>
        <xsl:call-template name="simpleLineStyle">
          <xsl:with-param name="style">solid</xsl:with-param>
          <xsl:with-param name="width">0.64</xsl:with-param>
          <xsl:with-param name="colour">CHRED</xsl:with-param>
        </xsl:call-template>
      </lineInstruction>
   </xsl:template>
</xsl:transform>
