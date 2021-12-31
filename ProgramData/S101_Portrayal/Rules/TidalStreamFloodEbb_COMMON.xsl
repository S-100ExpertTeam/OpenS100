<?xml version="1.0" encoding="UTF-8"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
   <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
   <xsl:template match="TidalStreamFloodEbb[@primitive='Surface' and (categoryOfTidalStream=1 and orientation/orientationValue!='')]" priority="3">
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>33060</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>12</drawingPriority>
         <symbol reference="FLDSTR01" rotation="{orientation/orientationValue}">
            <areaPlacement placementMode="VisibleParts"/>
         </symbol>
      </pointInstruction>
      <xsl:if test="currentVelocity/velocityMaximum!= ''">
         <textInstruction>
            <featureReference>
               <xsl:value-of select="@id"/>
            </featureReference>
            <viewingGroup>31</viewingGroup>
            <displayPlane>UNDERRADAR</displayPlane>
            <drawingPriority>12</drawingPriority>
            <textPoint>
               <element>
                  <text>
                     <xsl:apply-templates select="currentVelocity/velocityMaximum" mode="text"/>
                  </text>
                  <xsl:call-template name="textStyle">
                     <xsl:with-param name="style">default</xsl:with-param>
                  </xsl:call-template>
               </element>
               <offset>
                  <x>3.51</x>
                  <y>3.51</y>
               </offset>
               <areaPlacement placementMode="VisibleParts"/>
            </textPoint>
         </textInstruction>
      </xsl:if>
   </xsl:template>
   <xsl:template match="TidalStreamFloodEbb[@primitive='Point' and (categoryOfTidalStream=1 and orientation/orientationValue!='')]" priority="3">
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>33060</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>12</drawingPriority>
         <symbol reference="FLDSTR01" rotation="{orientation/orientationValue}"/>
      </pointInstruction>
      <xsl:if test="currentVelocity/velocityMaximum!= ''">
         <textInstruction>
            <featureReference>
               <xsl:value-of select="@id"/>
            </featureReference>
            <viewingGroup>31</viewingGroup>
            <displayPlane>UNDERRADAR</displayPlane>
            <drawingPriority>12</drawingPriority>
            <textPoint>
               <element>
                  <text>
                     <xsl:apply-templates select="currentVelocity/velocityMaximum" mode="text"/>
                  </text>
                  <xsl:call-template name="textStyle">
                     <xsl:with-param name="style">default</xsl:with-param>
                  </xsl:call-template>
               </element>
               <offset>
                  <x>3.51</x>
                  <y>3.51</y>
               </offset>
            </textPoint>
         </textInstruction>
      </xsl:if>
   </xsl:template>
   <xsl:template match="TidalStreamFloodEbb[@primitive='Surface' and (categoryOfTidalStream=2 and orientation/orientationValue!='')]" priority="3">
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>33060</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>12</drawingPriority>
         <symbol reference="EBBSTR01" rotation="{orientation/orientationValue}">
            <areaPlacement placementMode="VisibleParts"/>
         </symbol>
      </pointInstruction>
      <xsl:if test="currentVelocity/velocityMaximum!= ''">
         <textInstruction>
            <featureReference>
               <xsl:value-of select="@id"/>
            </featureReference>
            <viewingGroup>31</viewingGroup>
            <displayPlane>UNDERRADAR</displayPlane>
            <drawingPriority>12</drawingPriority>
            <textPoint>
               <element>
                  <text>
                     <xsl:apply-templates select="currentVelocity/velocityMaximum" mode="text"/>
                  </text>
                  <xsl:call-template name="textStyle">
                     <xsl:with-param name="style">default</xsl:with-param>
                  </xsl:call-template>
               </element>
               <offset>
                  <x>3.51</x>
                  <y>3.51</y>
               </offset>
               <areaPlacement placementMode="VisibleParts"/>
            </textPoint>
         </textInstruction>
      </xsl:if>
   </xsl:template>
   <xsl:template match="TidalStreamFloodEbb[@primitive='Point' and (categoryOfTidalStream=2 and orientation/orientationValue!='')]" priority="3">
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>33060</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>12</drawingPriority>
         <symbol reference="EBBSTR01" rotation="{orientation/orientationValue}"/>
      </pointInstruction>
      <xsl:if test="currentVelocity/velocityMaximum!= ''">
         <textInstruction>
            <featureReference>
               <xsl:value-of select="@id"/>
            </featureReference>
            <viewingGroup>31</viewingGroup>
            <displayPlane>UNDERRADAR</displayPlane>
            <drawingPriority>12</drawingPriority>
            <textPoint>
               <element>
                  <text>
                     <xsl:apply-templates select="currentVelocity/velocityMaximum" mode="text"/>
                  </text>
                  <xsl:call-template name="textStyle">
                     <xsl:with-param name="style">default</xsl:with-param>
                  </xsl:call-template>
               </element>
               <offset>
                  <x>3.51</x>
                  <y>3.51</y>
               </offset>
            </textPoint>
         </textInstruction>
      </xsl:if>
   </xsl:template>
   <xsl:template match="TidalStreamFloodEbb[@primitive='Surface' and (categoryOfTidalStream=3 and orientation/orientationValue!='')]" priority="3">
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>33060</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>12</drawingPriority>
         <symbol reference="CURENT01" rotation="{orientation/orientationValue}">
            <areaPlacement placementMode="VisibleParts"/>
         </symbol>
      </pointInstruction>
      <xsl:if test="currentVelocity/velocityMaximum!= ''">
         <textInstruction>
            <featureReference>
               <xsl:value-of select="@id"/>
            </featureReference>
            <viewingGroup>31</viewingGroup>
            <displayPlane>UNDERRADAR</displayPlane>
            <drawingPriority>12</drawingPriority>
            <textPoint>
               <element>
                  <text>
                     <xsl:apply-templates select="currentVelocity/velocityMaximum" mode="text"/>
                  </text>
                  <xsl:call-template name="textStyle">
                     <xsl:with-param name="style">default</xsl:with-param>
                  </xsl:call-template>
               </element>
               <offset>
                  <x>3.51</x>
                  <y>3.51</y>
               </offset>
               <areaPlacement placementMode="VisibleParts"/>
            </textPoint>
         </textInstruction>
      </xsl:if>
   </xsl:template>
   <xsl:template match="TidalStreamFloodEbb[@primitive='Point' and (categoryOfTidalStream=3 and orientation/orientationValue!='')]" priority="3">
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>33060</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>12</drawingPriority>
         <symbol reference="CURENT01" rotation="{orientation/orientationValue}"/>
      </pointInstruction>
      <xsl:if test="currentVelocity/velocityMaximum!= ''">
         <textInstruction>
            <featureReference>
               <xsl:value-of select="@id"/>
            </featureReference>
            <viewingGroup>31</viewingGroup>
            <displayPlane>UNDERRADAR</displayPlane>
            <drawingPriority>12</drawingPriority>
            <textPoint>
               <element>
                  <text>
                     <xsl:apply-templates select="currentVelocity/velocityMaximum" mode="text"/>
                  </text>
                  <xsl:call-template name="textStyle">
                     <xsl:with-param name="style">default</xsl:with-param>
                  </xsl:call-template>
               </element>
               <offset>
                  <x>3.51</x>
                  <y>3.51</y>
               </offset>
            </textPoint>
         </textInstruction>
      </xsl:if>
   </xsl:template>
   <xsl:template match="TidalStreamFloodEbb[@primitive='Point']" priority="1">
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>33060</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>12</drawingPriority>
         <symbol reference="CURDEF01"/>
      </pointInstruction>
   </xsl:template>
</xsl:transform>
