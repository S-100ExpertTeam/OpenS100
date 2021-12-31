<?xml version="1.0" encoding="UTF-8"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
   <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
   <xsl:template match="M_QUAL[@primitive='Surface']" priority="1">
      <areaInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>31010</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>12</drawingPriority>
         <areaFillReference reference="NODATA03"/>
      </areaInstruction>
      <lineInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>31010</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>12</drawingPriority>
         <xsl:call-template name="simpleLineStyle">
            <xsl:with-param name="style">dash</xsl:with-param>
            <xsl:with-param name="width">0.64</xsl:with-param>
            <xsl:with-param name="colour">CHGRD</xsl:with-param>
         </xsl:call-template>
      </lineInstruction>
   </xsl:template>
   <xsl:template match="M_QUAL[@primitive='Surface' and CATZOC=1]" priority="2">
      <areaInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>31010</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>12</drawingPriority>
         <areaFillReference reference="DQUALA11"/>
      </areaInstruction>
      <lineInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>31010</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>12</drawingPriority>
         <xsl:call-template name="simpleLineStyle">
            <xsl:with-param name="style">dash</xsl:with-param>
            <xsl:with-param name="width">0.64</xsl:with-param>
            <xsl:with-param name="colour">CHGRD</xsl:with-param>
         </xsl:call-template>
      </lineInstruction>
   </xsl:template>
   <xsl:template match="M_QUAL[@primitive='Surface' and CATZOC=2]" priority="2">
      <areaInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>31010</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>12</drawingPriority>
         <areaFillReference reference="DQUALA21"/>
      </areaInstruction>
      <lineInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>31010</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>12</drawingPriority>
         <xsl:call-template name="simpleLineStyle">
            <xsl:with-param name="style">dash</xsl:with-param>
            <xsl:with-param name="width">0.64</xsl:with-param>
            <xsl:with-param name="colour">CHGRD</xsl:with-param>
         </xsl:call-template>
      </lineInstruction>
   </xsl:template>
   <xsl:template match="M_QUAL[@primitive='Surface' and CATZOC=3]" priority="2">
      <areaInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>31010</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>12</drawingPriority>
         <areaFillReference reference="DQUALB01"/>
      </areaInstruction>
      <lineInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>31010</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>12</drawingPriority>
         <xsl:call-template name="simpleLineStyle">
            <xsl:with-param name="style">dash</xsl:with-param>
            <xsl:with-param name="width">0.64</xsl:with-param>
            <xsl:with-param name="colour">CHGRD</xsl:with-param>
         </xsl:call-template>
      </lineInstruction>
   </xsl:template>
   <xsl:template match="M_QUAL[@primitive='Surface' and CATZOC=4]" priority="2">
      <areaInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>31010</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>12</drawingPriority>
         <areaFillReference reference="DQUALC01"/>
      </areaInstruction>
      <lineInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>31010</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>12</drawingPriority>
         <xsl:call-template name="simpleLineStyle">
            <xsl:with-param name="style">dash</xsl:with-param>
            <xsl:with-param name="width">0.64</xsl:with-param>
            <xsl:with-param name="colour">CHGRD</xsl:with-param>
         </xsl:call-template>
      </lineInstruction>
   </xsl:template>
   <xsl:template match="M_QUAL[@primitive='Surface' and CATZOC=5]" priority="2">
      <areaInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>31010</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>12</drawingPriority>
         <areaFillReference reference="DQUALD01"/>
      </areaInstruction>
      <lineInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>31010</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>12</drawingPriority>
         <xsl:call-template name="simpleLineStyle">
            <xsl:with-param name="style">dash</xsl:with-param>
            <xsl:with-param name="width">0.64</xsl:with-param>
            <xsl:with-param name="colour">CHGRD</xsl:with-param>
         </xsl:call-template>
      </lineInstruction>
   </xsl:template>
   <xsl:template match="M_QUAL[@primitive='Surface' and CATZOC=6]" priority="2">
      <areaInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>31010</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>12</drawingPriority>
         <areaFillReference reference="DQUALU01"/>
      </areaInstruction>
      <lineInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>31010</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>12</drawingPriority>
         <xsl:call-template name="simpleLineStyle">
            <xsl:with-param name="style">dash</xsl:with-param>
            <xsl:with-param name="width">0.64</xsl:with-param>
            <xsl:with-param name="colour">CHGRD</xsl:with-param>
         </xsl:call-template>
      </lineInstruction>
   </xsl:template>
</xsl:transform>
