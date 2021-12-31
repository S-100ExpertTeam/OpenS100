<?xml version="1.0" encoding="UTF-8"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
   <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
   <xsl:template match="RadioCallingInPoint[@primitive='Curve']" priority="1">
      <lineInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>25060</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>18</drawingPriority>
         <xsl:call-template name="simpleLineStyle">
            <xsl:with-param name="style">dash</xsl:with-param>
            <xsl:with-param name="width">0.32</xsl:with-param>
            <xsl:with-param name="colour">TRFCD</xsl:with-param>
         </xsl:call-template>
      </lineInstruction>
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>25060</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>18</drawingPriority>
         <symbol reference="RCLDEF01">
            <linePlacement placementMode="Relative">
               <offset>0.5</offset>
            </linePlacement>
         </symbol>
      </pointInstruction>
      <xsl:if test="featureName!= ''">
         <textInstruction>
            <featureReference>
               <xsl:value-of select="@id"/>
            </featureReference>
            <viewingGroup>11</viewingGroup>
            <displayPlane>OVERRADAR</displayPlane>
            <drawingPriority>18</drawingPriority>
            <textLine verticalAlignment="Center">
               <element>
                  <text>
                     <xsl:apply-templates select="featureName" mode="text">
                        <xsl:with-param name="prefix">Nr </xsl:with-param>
                     </xsl:apply-templates>
                  </text>
                  <xsl:call-template name="textStyle">
                     <xsl:with-param name="style">default</xsl:with-param>
                  </xsl:call-template>
               </element>
               <startOffset>0.5</startOffset>
               <placementMode>Relative</placementMode>
            </textLine>
         </textInstruction>
      </xsl:if>
   </xsl:template>
   <xsl:template match="RadioCallingInPoint[@primitive='Curve' and ((trafficFlow=1 or trafficFlow=2 or trafficFlow=3) and orientation/orientationValue!='')]" priority="5">
      <lineInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>25060</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>18</drawingPriority>
         <xsl:call-template name="simpleLineStyle">
            <xsl:with-param name="style">dash</xsl:with-param>
            <xsl:with-param name="width">0.32</xsl:with-param>
            <xsl:with-param name="colour">TRFCD</xsl:with-param>
         </xsl:call-template>
      </lineInstruction>
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>25060</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>18</drawingPriority>
         <symbol reference="RDOCAL02" rotation="{orientation/orientationValue}">
            <linePlacement placementMode="Relative">
               <offset>0.5</offset>
            </linePlacement>
         </symbol>
      </pointInstruction>
      <xsl:if test="featureName!= ''">
         <textInstruction>
            <featureReference>
               <xsl:value-of select="@id"/>
            </featureReference>
            <viewingGroup>11</viewingGroup>
            <displayPlane>OVERRADAR</displayPlane>
            <drawingPriority>18</drawingPriority>
            <textLine>
               <element>
                  <text>
                     <xsl:apply-templates select="featureName" mode="text">
                        <xsl:with-param name="prefix">Nr </xsl:with-param>
                     </xsl:apply-templates>
                  </text>
                  <xsl:call-template name="textStyle">
                     <xsl:with-param name="style">default</xsl:with-param>
                  </xsl:call-template>
               </element>
               <startOffset>0.5</startOffset>
               <placementMode>Relative</placementMode>
            </textLine>
         </textInstruction>
      </xsl:if>
      <xsl:if test="communicationInformation/communicationChannel!= ''">
         <textInstruction>
            <featureReference>
               <xsl:value-of select="@id"/>
            </featureReference>
            <viewingGroup>11</viewingGroup>
            <displayPlane>OVERRADAR</displayPlane>
            <drawingPriority>18</drawingPriority>
            <textLine>
               <element>
                  <text>
                     <xsl:apply-templates select="communicationInformation/communicationChannel" mode="text">
                        <xsl:with-param name="prefix">ch </xsl:with-param>
                     </xsl:apply-templates>
                  </text>
                  <xsl:call-template name="textStyle">
                     <xsl:with-param name="style">default</xsl:with-param>
                  </xsl:call-template>
               </element>
               <startOffset>0.5</startOffset>
               <placementMode>Relative</placementMode>
            </textLine>
         </textInstruction>
      </xsl:if>
   </xsl:template>
   <xsl:template match="RadioCallingInPoint[@primitive='Curve' and (trafficFlow=4 and orientation/orientationValue!='')]" priority="3">
      <lineInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>25060</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>18</drawingPriority>
         <xsl:call-template name="simpleLineStyle">
            <xsl:with-param name="style">dash</xsl:with-param>
            <xsl:with-param name="width">0.32</xsl:with-param>
            <xsl:with-param name="colour">TRFCD</xsl:with-param>
         </xsl:call-template>
      </lineInstruction>
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>25060</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>18</drawingPriority>
         <symbol reference="RDOCAL03" rotation="{orientation/orientationValue}">
            <linePlacement placementMode="Relative">
               <offset>0.5</offset>
            </linePlacement>
         </symbol>
      </pointInstruction>
      <xsl:if test="featureName!= ''">
         <textInstruction>
            <featureReference>
               <xsl:value-of select="@id"/>
            </featureReference>
            <viewingGroup>11</viewingGroup>
            <displayPlane>OVERRADAR</displayPlane>
            <drawingPriority>18</drawingPriority>
            <textLine>
               <element>
                  <text>
                     <xsl:apply-templates select="featureName" mode="text">
                        <xsl:with-param name="prefix">Nr </xsl:with-param>
                     </xsl:apply-templates>
                  </text>
                  <xsl:call-template name="textStyle">
                     <xsl:with-param name="style">default</xsl:with-param>
                  </xsl:call-template>
               </element>
               <startOffset>0.5</startOffset>
               <placementMode>Relative</placementMode>
            </textLine>
         </textInstruction>
      </xsl:if>
      <xsl:if test="communicationInformation/communicationChannel!= ''">
         <textInstruction>
            <featureReference>
               <xsl:value-of select="@id"/>
            </featureReference>
            <viewingGroup>11</viewingGroup>
            <displayPlane>OVERRADAR</displayPlane>
            <drawingPriority>18</drawingPriority>
            <textLine>
               <element>
                  <text>
                     <xsl:apply-templates select="communicationInformation/communicationChannel" mode="text">
                        <xsl:with-param name="prefix">ch </xsl:with-param>
                     </xsl:apply-templates>
                  </text>
                  <xsl:call-template name="textStyle">
                     <xsl:with-param name="style">default</xsl:with-param>
                  </xsl:call-template>
               </element>
               <startOffset>0.5</startOffset>
               <placementMode>Relative</placementMode>
            </textLine>
         </textInstruction>
      </xsl:if>
   </xsl:template>
   <xsl:template match="RadioCallingInPoint[@primitive='Point']" priority="1">
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>25060</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>18</drawingPriority>
         <symbol reference="RCLDEF01"/>
      </pointInstruction>
      <xsl:if test="featureName!= ''">
         <textInstruction>
            <featureReference>
               <xsl:value-of select="@id"/>
            </featureReference>
            <viewingGroup>11</viewingGroup>
            <displayPlane>OVERRADAR</displayPlane>
            <drawingPriority>18</drawingPriority>
            <textPoint verticalAlignment="Center">
               <element>
                  <text>
                     <xsl:apply-templates select="featureName" mode="text">
                        <xsl:with-param name="prefix">Nr </xsl:with-param>
                     </xsl:apply-templates>
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
   <xsl:template match="RadioCallingInPoint[@primitive='Point' and ((trafficFlow=1 or trafficFlow=2 or trafficFlow=3) and orientation/orientationValue!='')]" priority="5">
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>25060</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>18</drawingPriority>
         <symbol reference="RDOCAL02" rotation="{orientation/orientationValue}"/>
      </pointInstruction>
      <xsl:if test="featureName!= ''">
         <textInstruction>
            <featureReference>
               <xsl:value-of select="@id"/>
            </featureReference>
            <viewingGroup>11</viewingGroup>
            <displayPlane>OVERRADAR</displayPlane>
            <drawingPriority>18</drawingPriority>
            <textPoint>
               <element>
                  <text>
                     <xsl:apply-templates select="featureName" mode="text">
                        <xsl:with-param name="prefix">Nr </xsl:with-param>
                     </xsl:apply-templates>
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
      <xsl:if test="communicationInformation/communicationChannel!= ''">
         <textInstruction>
            <featureReference>
               <xsl:value-of select="@id"/>
            </featureReference>
            <viewingGroup>11</viewingGroup>
            <displayPlane>OVERRADAR</displayPlane>
            <drawingPriority>18</drawingPriority>
            <textPoint>
               <element>
                  <text>
                     <xsl:apply-templates select="communicationInformation/communicationChannel" mode="text">
                        <xsl:with-param name="prefix">ch </xsl:with-param>
                     </xsl:apply-templates>
                  </text>
                  <xsl:call-template name="textStyle">
                     <xsl:with-param name="style">default</xsl:with-param>
                  </xsl:call-template>
               </element>
               <offset>
                  <x>3.51</x>
                  <y>-3.51</y>
               </offset>
            </textPoint>
         </textInstruction>
      </xsl:if>
   </xsl:template>
   <xsl:template match="RadioCallingInPoint[@primitive='Point' and (trafficFlow=4 and orientation/orientationValue!='')]" priority="3">
      <pointInstruction>
         <featureReference>
            <xsl:value-of select="@id"/>
         </featureReference>
         <viewingGroup>25060</viewingGroup>
         <displayPlane>OVERRADAR</displayPlane>
         <drawingPriority>18</drawingPriority>
         <symbol reference="RDOCAL03" rotation="{orientation/orientationValue}"/>
      </pointInstruction>
      <xsl:if test="featureName!= ''">
         <textInstruction>
            <featureReference>
               <xsl:value-of select="@id"/>
            </featureReference>
            <viewingGroup>11</viewingGroup>
            <displayPlane>OVERRADAR</displayPlane>
            <drawingPriority>18</drawingPriority>
            <textPoint>
               <element>
                  <text>
                     <xsl:apply-templates select="featureName" mode="text">
                        <xsl:with-param name="prefix">Nr </xsl:with-param>
                     </xsl:apply-templates>
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
      <xsl:if test="communicationInformation/communicationChannel!= ''">
         <textInstruction>
            <featureReference>
               <xsl:value-of select="@id"/>
            </featureReference>
            <viewingGroup>11</viewingGroup>
            <displayPlane>OVERRADAR</displayPlane>
            <drawingPriority>18</drawingPriority>
            <textPoint>
               <element>
                  <text>
                     <xsl:apply-templates select="communicationInformation/communicationChannel" mode="text">
                        <xsl:with-param name="prefix">ch </xsl:with-param>
                     </xsl:apply-templates>
                  </text>
                  <xsl:call-template name="textStyle">
                     <xsl:with-param name="style">default</xsl:with-param>
                  </xsl:call-template>
               </element>
               <offset>
                  <x>3.51</x>
                  <y>-3.51</y>
               </offset>
            </textPoint>
         </textInstruction>
      </xsl:if>
   </xsl:template>
</xsl:transform>
