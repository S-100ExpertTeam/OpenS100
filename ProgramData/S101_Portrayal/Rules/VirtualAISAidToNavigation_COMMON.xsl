<?xml version="1.0" encoding="ISO-8859-1"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
   <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
   <xsl:template match="VirtualAisAidToNavigation[@primitive='Point']" priority="1">
     <pointInstruction>
       <featureReference>
         <xsl:value-of select="@id"/>
       </featureReference>
       <viewingGroup>34050</viewingGroup>
       <displayPlane>OVERRADAR</displayPlane>
       <drawingPriority>12</drawingPriority>
       <symbol reference="VIRATN01"/>
     </pointInstruction>
   </xsl:template>
</xsl:transform>
