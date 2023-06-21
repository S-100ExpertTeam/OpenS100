<?xml version="1.0" encoding="UTF-8"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
  
  <xsl:template match="CurrentNonGravitational[@primitive='Point']" priority="1">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="WNDFRM61"/>
    </pointInstruction>
    
      <xsl:if test="featureName!= ''">
         <textInstruction>
            <featureReference>
               <xsl:value-of select="@id"/>
            </featureReference>
            <viewingGroup>26</viewingGroup>
            <displayPlane>OVERRADAR</displayPlane>
            <drawingPriority>12</drawingPriority>
            <textPoint horizontalAlignment="Right" verticalAlignment="Center">
               <element>
                  <text>
                     <xsl:apply-templates select="featureName" mode="text"/>
                  </text>
                  <xsl:call-template name="textStyle">
                     <xsl:with-param name="style">default</xsl:with-param>
                  </xsl:call-template>
               </element>
               <offset>
                  <x>0</x>
                  <y>5</y>
               </offset>
            </textPoint>
         </textInstruction>
      </xsl:if>    
      
  </xsl:template>
  
 
   <xsl:template match="CurrentNonGravitational[@primitive='Curve']" priority="1">
      <lineInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>25060</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>18</drawingPriority>
         <xsl:call-template name="simpleLineStyle">
            <xsl:with-param name="style">dash</xsl:with-param>
            <xsl:with-param name="width">0.96</xsl:with-param>
            <xsl:with-param name="colour">CHRED</xsl:with-param>
         </xsl:call-template>
      </lineInstruction>            
   </xsl:template>  

  <xsl:template match="CurrentNonGravitational[@primitive='Surface']" priority="1">
    <lineInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>21010</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>15</drawingPriority>
      <xsl:call-template name="simpleLineStyle">
        <xsl:with-param name="style">dash</xsl:with-param>
        <xsl:with-param name="width">0.32</xsl:with-param>
        <xsl:with-param name="colour">UINFM</xsl:with-param>
      </xsl:call-template>
    </lineInstruction>
    <areaInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>12010</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>9</drawingPriority>
      <colorFill>
        <color>
          <token>CURSR</token>
          <transparency>0.75</transparency>
        </color>
      </colorFill>
    </areaInstruction>
  </xsl:template>
        
</xsl:transform>