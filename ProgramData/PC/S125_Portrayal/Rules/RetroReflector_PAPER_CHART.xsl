<?xml version="1.0" encoding="UTF-8"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
   <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
   <xsl:template match="RetroReflector[@primitive='Point']" priority="1">
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>27080</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>18</drawingPriority>
         <symbol reference="RETRFL01"/>
      </pointInstruction>
      <xsl:call-template name="AtoNStatusInformation">
         <xsl:with-param name="fid">
            <xsl:value-of select="@id"/>
         </xsl:with-param>
     </xsl:call-template>
   </xsl:template>
</xsl:transform>
