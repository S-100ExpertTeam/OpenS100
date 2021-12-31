<?xml version="1.0" encoding="UTF-8"?>

<!-- Modified Sept 2016 to call the DEPARE04 template to look for safety contours around safe depth regions -->	

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
   <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
   <xsl:template match="DredgedArea[@primitive='Surface']" priority="1">
      <xsl:call-template name="seabed01">
         <xsl:with-param name="viewingGroup">13030</xsl:with-param>
         <xsl:with-param name="displayPlane">UNDERRADAR</xsl:with-param>
         <xsl:with-param name="drawingPriority">3</xsl:with-param>
      </xsl:call-template>
      <areaInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>13030</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>3</drawingPriority>
         <areaFillReference reference="DRGARE01"/>
      </areaInstruction>
      <lineInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>13030</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>3</drawingPriority>
         <xsl:call-template name="simpleLineStyle">
            <xsl:with-param name="style">dash</xsl:with-param>
            <xsl:with-param name="width">0.32</xsl:with-param>
            <xsl:with-param name="colour">CHGRF</xsl:with-param>
         </xsl:call-template>
      </lineInstruction>
      <xsl:if test="restriction">
         <xsl:call-template name="RESCSP03">
            <xsl:with-param name="viewingGroup">13030</xsl:with-param>
            <xsl:with-param name="displayPlane">UNDERRADAR</xsl:with-param>
            <xsl:with-param name="drawingPriority">3</xsl:with-param>
         </xsl:call-template>
      </xsl:if>
	  
	  <xsl:call-template name="DEPARE04"/>

   </xsl:template>
</xsl:transform>
