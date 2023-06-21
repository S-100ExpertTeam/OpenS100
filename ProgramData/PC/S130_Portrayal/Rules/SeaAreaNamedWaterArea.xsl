<?xml version="1.0" encoding="UTF-8"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
   <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
   <xsl:template match="SeaAreaNamedWaterArea[@primitive='Surface']" priority="1">
     <lineInstruction>
       <featureReference>
         <xsl:value-of select="@id"/>
       </featureReference>
       <viewingGroup>36050</viewingGroup>
       <displayPlane>UNDERRADAR</displayPlane>
       <drawingPriority>6</drawingPriority>
       <lineStyleReference reference="ADMARE01"/>
     </lineInstruction>
<!--       <xsl:if test="maritimeResourceName!= ''">
         <textInstruction>
            <featureReference>
               <xsl:value-of select="@id"/>
            </featureReference>
            <viewingGroup>26</viewingGroup>
            <displayPlane>UNDERRADAR</displayPlane>
            <drawingPriority>9</drawingPriority>
            <textPoint horizontalAlignment="Center" verticalAlignment="Center">
               <element>
                  <text>
                     <xsl:apply-templates select="maritimeResourceName" mode="text"/>
                  </text>
                  <xsl:call-template name="textStyle">
                     <xsl:with-param name="style">default</xsl:with-param>
                  </xsl:call-template>
               </element>
               <offset>
                  <x>0</x>
                  <y>0</y>
               </offset>
               <areaPlacement placementMode="VisibleParts"/>
            </textPoint>
         </textInstruction>
      </xsl:if> -->

     <xsl:if test="maritimeResourceName!= ''">
       <textInstruction>
         <featureReference>
           <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>26</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>9</drawingPriority>
         <textPoint verticalAlignment="Center" horizontalAlignment="Center">
           <element>
             <text>
               <xsl:apply-templates select="maritimeResourceName" mode="text"/>
             </text>
             <xsl:call-template name="textStyle">
               <xsl:with-param name="style">default</xsl:with-param>
               <xsl:with-param name="foreground">default</xsl:with-param>
               <xsl:with-param name="bodySize">default</xsl:with-param>
             </xsl:call-template>
           </element>
           <offset>
             <x>3.51</x>
             <y>0</y>
           </offset>
         </textPoint>
       </textInstruction>
     </xsl:if>
   </xsl:template>
    </xsl:transform>
