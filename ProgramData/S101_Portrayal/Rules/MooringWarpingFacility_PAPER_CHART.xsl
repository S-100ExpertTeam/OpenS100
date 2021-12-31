<?xml version="1.0" encoding="UTF-8"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
   <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
   <xsl:template match="MooringWarpingFacility[@primitive='Point' and (categoryOfMooringWarpingFacility=7 and buoyShape=3)]" priority="3">
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>27010</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>24</drawingPriority>
         <symbol reference="BOYMOR01"/>
      </pointInstruction>
   </xsl:template>
   <xsl:template match="MooringWarpingFacility[@primitive='Point' and (categoryOfMooringWarpingFacility=7 and buoyShape=6)]" priority="3">
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>27010</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>24</drawingPriority>
         <symbol reference="BOYMOR03"/>
      </pointInstruction>
   </xsl:template>
</xsl:transform>
