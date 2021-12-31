<?xml version="1.0" encoding="UTF-8"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
   <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
   <xsl:template match="Fairway[@primitive='Surface']" priority="1">
      <xsl:if test="featureName!= ''">
         <textInstruction>
            <featureReference>
               <xsl:value-of select="@id"/>
            </featureReference>
            <viewingGroup>26</viewingGroup>
            <displayPlane>UNDERRADAR</displayPlane>
            <drawingPriority>12</drawingPriority>
            <textPoint verticalAlignment="Center">
               <element>
                  <text>
                     <xsl:apply-templates select="featureName" mode="text"/>
                  </text>
                  <xsl:call-template name="textStyle">
                     <xsl:with-param name="style">default</xsl:with-param>
                  </xsl:call-template>
               </element>
               <offset>
                  <x>7.02</x>
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
         <viewingGroup>26050</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>12</drawingPriority>
         <xsl:call-template name="simpleLineStyle">
            <xsl:with-param name="style">dash</xsl:with-param>
            <xsl:with-param name="width">0.32</xsl:with-param>
            <xsl:with-param name="colour">CHGRD</xsl:with-param>
         </xsl:call-template>
      </lineInstruction>
      <xsl:call-template name="RESCSP03">
         <xsl:with-param name="viewingGroup">26050</xsl:with-param>
         <xsl:with-param name="displayPlane">UNDERRADAR</xsl:with-param>
         <xsl:with-param name="drawingPriority">12</xsl:with-param>
      </xsl:call-template>
   </xsl:template>
   <xsl:template match="Fairway[@primitive='Surface' and (orientation/orientationValue!='' and (trafficFlow=1 or trafficFlow=2 or trafficFlow=3))]" priority="5">
      <xsl:if test="featureName!= ''">
         <textInstruction>
            <featureReference>
               <xsl:value-of select="@id"/>
            </featureReference>
            <viewingGroup>26</viewingGroup>
            <displayPlane>UNDERRADAR</displayPlane>
            <drawingPriority>12</drawingPriority>
            <textPoint verticalAlignment="Center">
               <element>
                  <text>
                     <xsl:apply-templates select="featureName" mode="text"/>
                  </text>
                  <xsl:call-template name="textStyle">
                     <xsl:with-param name="style">default</xsl:with-param>
                  </xsl:call-template>
               </element>
               <offset>
                  <x>7.02</x>
                  <y>0</y>
               </offset>
               <areaPlacement placementMode="VisibleParts"/>
            </textPoint>
         </textInstruction>
      </xsl:if>
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>26050</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>12</drawingPriority>
         <symbol reference="FAIRWY51" rotation="{orientation/orientationValue}">
            <areaPlacement placementMode="VisibleParts"/>
         </symbol>
      </pointInstruction>
      <lineInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>26050</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>12</drawingPriority>
         <xsl:call-template name="simpleLineStyle">
            <xsl:with-param name="style">dash</xsl:with-param>
            <xsl:with-param name="width">0.32</xsl:with-param>
            <xsl:with-param name="colour">CHGRD</xsl:with-param>
         </xsl:call-template>
      </lineInstruction>
      <xsl:call-template name="RESCSP03">
         <xsl:with-param name="viewingGroup">26050</xsl:with-param>
         <xsl:with-param name="displayPlane">UNDERRADAR</xsl:with-param>
         <xsl:with-param name="drawingPriority">12</xsl:with-param>
      </xsl:call-template>
   </xsl:template>
   <xsl:template match="Fairway[@primitive='Surface' and (orientation/orientationValue!='' and trafficFlow=4)]" priority="3">
      <xsl:if test="featureName!= ''">
         <textInstruction>
            <featureReference>
               <xsl:value-of select="@id"/>
            </featureReference>
            <viewingGroup>26</viewingGroup>
            <displayPlane>UNDERRADAR</displayPlane>
            <drawingPriority>12</drawingPriority>
            <textPoint verticalAlignment="Center">
               <element>
                  <text>
                     <xsl:apply-templates select="featureName" mode="text"/>
                  </text>
                  <xsl:call-template name="textStyle">
                     <xsl:with-param name="style">default</xsl:with-param>
                  </xsl:call-template>
               </element>
               <offset>
                  <x>7.02</x>
                  <y>0</y>
               </offset>
               <areaPlacement placementMode="VisibleParts"/>
            </textPoint>
         </textInstruction>
      </xsl:if>
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>26050</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>12</drawingPriority>
         <symbol reference="FAIRWY52" rotation="{orientation/orientationValue}">
            <areaPlacement placementMode="VisibleParts"/>
         </symbol>
      </pointInstruction>
      <lineInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>26050</viewingGroup>
         <displayPlane>UNDERRADAR</displayPlane>
         <drawingPriority>12</drawingPriority>
         <xsl:call-template name="simpleLineStyle">
            <xsl:with-param name="style">dash</xsl:with-param>
            <xsl:with-param name="width">0.32</xsl:with-param>
            <xsl:with-param name="colour">CHGRD</xsl:with-param>
         </xsl:call-template>
      </lineInstruction>
      <xsl:call-template name="RESCSP03">
         <xsl:with-param name="viewingGroup">26050</xsl:with-param>
         <xsl:with-param name="displayPlane">UNDERRADAR</xsl:with-param>
         <xsl:with-param name="drawingPriority">12</xsl:with-param>
      </xsl:call-template>
   </xsl:template>
</xsl:transform>
