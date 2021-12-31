<?xml version="1.0" encoding="UTF-8"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
   <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
   <xsl:template match="RecommendedTrack[@primitive='Surface']" priority="1">
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>25020</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>18</drawingPriority>
         <symbol reference="RECDEF51">
            <areaPlacement placementMode="VisibleParts"/>
         </symbol>
      </pointInstruction>
      <lineInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>25020</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>18</drawingPriority>
         <lineStyleReference reference="NAVARE51"/>
      </lineInstruction>
   </xsl:template>
   <xsl:template match="RecommendedTrack[@primitive='Surface' and (orientation/orientationValue!='' and categoryOfRecommendedTrack=1 and (trafficFlow=1 or trafficFlow=2 or trafficFlow=3))]" priority="6">
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>25020</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>18</drawingPriority>
         <symbol reference="RECTRC58" rotation="{orientation/orientationValue}">
            <areaPlacement placementMode="VisibleParts"/>
         </symbol>
      </pointInstruction>
      <xsl:if test="orientation/orientationValue!= ''">
         <textInstruction>
            <featureReference>
               <xsl:value-of select="@id"/>
            </featureReference>
            <viewingGroup>11</viewingGroup>
            <displayPlane>UNDERRADAR</displayPlane>
            <drawingPriority>18</drawingPriority>
            <textPoint verticalAlignment="Center">
               <element>
                  <text>
                     <xsl:apply-templates select="orientation/orientationValue" mode="text"/>
                  </text>
                  <xsl:call-template name="textStyle">
                     <xsl:with-param name="style">default</xsl:with-param>
                  </xsl:call-template>
               </element>
               <offset>
                  <x>14.04</x>
                  <y>0</y>
               </offset>
               <areaPlacement placementMode="VisibleParts"/>
            </textPoint>
         </textInstruction>
      </xsl:if>
      <lineInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>25020</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>18</drawingPriority>
         <lineStyleReference reference="NAVARE51"/>
      </lineInstruction>
   </xsl:template>
   <xsl:template match="RecommendedTrack[@primitive='Surface' and (orientation/orientationValue!='' and categoryOfRecommendedTrack=1 and trafficFlow=4)]" priority="4">
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>25020</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>18</drawingPriority>
         <symbol reference="RECTRC56" rotation="{orientation/orientationValue}">
            <areaPlacement placementMode="VisibleParts"/>
         </symbol>
      </pointInstruction>
      <xsl:if test="orientation/orientationValue!= ''">
         <textInstruction>
            <featureReference>
               <xsl:value-of select="@id"/>
            </featureReference>
            <viewingGroup>11</viewingGroup>
            <displayPlane>UNDERRADAR</displayPlane>
            <drawingPriority>18</drawingPriority>
            <textPoint verticalAlignment="Center">
               <element>
                  <text>
                     <xsl:apply-templates select="orientation/orientationValue" mode="text"/>
                  </text>
                  <xsl:call-template name="textStyle">
                     <xsl:with-param name="style">default</xsl:with-param>
                  </xsl:call-template>
               </element>
               <offset>
                  <x>14.04</x>
                  <y>0</y>
               </offset>
               <areaPlacement placementMode="VisibleParts"/>
            </textPoint>
         </textInstruction>
      </xsl:if>
      <lineInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>25020</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>18</drawingPriority>
         <lineStyleReference reference="NAVARE51"/>
      </lineInstruction>
   </xsl:template>
   <xsl:template match="RecommendedTrack[@primitive='Surface' and (orientation/orientationValue!='' and categoryOfRecommendedTrack=2 and (trafficFlow=1 or trafficFlow=2 or trafficFlow=3))]" priority="6">
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>25020</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>18</drawingPriority>
         <symbol reference="RECTRC57" rotation="{orientation/orientationValue}">
            <areaPlacement placementMode="VisibleParts"/>
         </symbol>
      </pointInstruction>
      <xsl:if test="orientation/orientationValue!= ''">
         <textInstruction>
            <featureReference>
               <xsl:value-of select="@id"/>
            </featureReference>
            <viewingGroup>11</viewingGroup>
            <displayPlane>UNDERRADAR</displayPlane>
            <drawingPriority>18</drawingPriority>
            <textPoint verticalAlignment="Center">
               <element>
                  <text>
                     <xsl:apply-templates select="orientation/orientationValue" mode="text"/>
                  </text>
                  <xsl:call-template name="textStyle">
                     <xsl:with-param name="style">default</xsl:with-param>
                  </xsl:call-template>
               </element>
               <offset>
                  <x>14.04</x>
                  <y>0</y>
               </offset>
               <areaPlacement placementMode="VisibleParts"/>
            </textPoint>
         </textInstruction>
      </xsl:if>
      <lineInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>25020</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>18</drawingPriority>
         <lineStyleReference reference="NAVARE51"/>
      </lineInstruction>
   </xsl:template>
   <xsl:template match="RecommendedTrack[@primitive='Surface' and (orientation/orientationValue!='' and categoryOfRecommendedTrack=2 and trafficFlow=4)]" priority="4">
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>25020</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>18</drawingPriority>
         <symbol reference="RECTRC55" rotation="{orientation/orientationValue}">
            <areaPlacement placementMode="VisibleParts"/>
         </symbol>
      </pointInstruction>
      <xsl:if test="orientation/orientationValue!= ''">
         <textInstruction>
            <featureReference>
               <xsl:value-of select="@id"/>
            </featureReference>
            <viewingGroup>11</viewingGroup>
            <displayPlane>UNDERRADAR</displayPlane>
            <drawingPriority>18</drawingPriority>
            <textPoint verticalAlignment="Center">
               <element>
                  <text>
                     <xsl:apply-templates select="orientation/orientationValue" mode="text"/>
                  </text>
                  <xsl:call-template name="textStyle">
                     <xsl:with-param name="style">default</xsl:with-param>
                  </xsl:call-template>
               </element>
               <offset>
                  <x>14.04</x>
                  <y>0</y>
               </offset>
               <areaPlacement placementMode="VisibleParts"/>
            </textPoint>
         </textInstruction>
      </xsl:if>
      <lineInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>25020</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>18</drawingPriority>
         <lineStyleReference reference="NAVARE51"/>
      </lineInstruction>
   </xsl:template>
   <xsl:template match="RecommendedTrack[@primitive='Surface' and (orientation/orientationValue!='' and (trafficFlow=1 or trafficFlow=2 or trafficFlow=3))]" priority="5">
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>25020</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>18</drawingPriority>
         <symbol reference="RECTRC57" rotation="{orientation/orientationValue}">
            <areaPlacement placementMode="VisibleParts"/>
         </symbol>
      </pointInstruction>
      <xsl:if test="orientation/orientationValue!= ''">
         <textInstruction>
            <featureReference>
               <xsl:value-of select="@id"/>
            </featureReference>
            <viewingGroup>11</viewingGroup>
            <displayPlane>UNDERRADAR</displayPlane>
            <drawingPriority>18</drawingPriority>
            <textPoint verticalAlignment="Center">
               <element>
                  <text>
                     <xsl:apply-templates select="orientation/orientationValue" mode="text"/>
                  </text>
                  <xsl:call-template name="textStyle">
                     <xsl:with-param name="style">default</xsl:with-param>
                  </xsl:call-template>
               </element>
               <offset>
                  <x>14.04</x>
                  <y>0</y>
               </offset>
               <areaPlacement placementMode="VisibleParts"/>
            </textPoint>
         </textInstruction>
      </xsl:if>
      <lineInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>25020</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>18</drawingPriority>
         <lineStyleReference reference="NAVARE51"/>
      </lineInstruction>
   </xsl:template>
   <xsl:template match="RecommendedTrack[@primitive='Surface' and (orientation/orientationValue!='' and trafficFlow=4)]" priority="3">
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>25020</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>18</drawingPriority>
         <symbol reference="RECTRC55" rotation="{orientation/orientationValue}">
            <areaPlacement placementMode="VisibleParts"/>
         </symbol>
      </pointInstruction>
      <xsl:if test="orientation/orientationValue!= ''">
         <textInstruction>
            <featureReference>
               <xsl:value-of select="@id"/>
            </featureReference>
            <viewingGroup>11</viewingGroup>
            <displayPlane>UNDERRADAR</displayPlane>
            <drawingPriority>18</drawingPriority>
            <textPoint verticalAlignment="Center">
               <element>
                  <text>
                     <xsl:apply-templates select="orientation/orientationValue" mode="text"/>
                  </text>
                  <xsl:call-template name="textStyle">
                     <xsl:with-param name="style">default</xsl:with-param>
                  </xsl:call-template>
               </element>
               <offset>
                  <x>14.04</x>
                  <y>0</y>
               </offset>
               <areaPlacement placementMode="VisibleParts"/>
            </textPoint>
         </textInstruction>
      </xsl:if>
      <lineInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>25020</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>18</drawingPriority>
         <lineStyleReference reference="NAVARE51"/>
      </lineInstruction>
   </xsl:template>
</xsl:transform>
