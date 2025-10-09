<?xml version="1.0" encoding="ISO-8859-1"?>
<!-- 
<Dataset> 
  <Points>
    <Point id="p1"/>
  </Points>
  <Features>
    <NavwarnPart id="f0" primitive="Point">
		<Point ref="p1"/> 
-->

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
	<xsl:template match="NavwarnPart[@primitive='Point']" priority="1">
		<!-- Only output a drawing instruction if this NavwarnPart is the first one in the input document with the same Point -->
		<xsl:if test="../NavwarnPart[Point/@ref = current()/Point/@ref][1]/@id = @id">
		<pointInstruction>
			<featureReference>
				<xsl:value-of select="@id"/>
			</featureReference>
			<viewingGroup>31020</viewingGroup>
			<displayPlane>UNDERRADAR</displayPlane>
			<drawingPriority>15</drawingPriority>
			<symbol reference="NavigationalWarningFeaturePart">
			</symbol>
		</pointInstruction>
			<!-- If there is more than one NavwarnPart at the same location output the count as text -->
			<xsl:variable name="sameCount" select="count(../NavwarnPart[Point/@ref = current()/Point/@ref])"/>
			<xsl:if test="$sameCount &gt; 1">
				 <textInstruction>
					<featureReference>
					   <xsl:value-of select="@id"/>
					</featureReference>
					<viewingGroup>12210</viewingGroup>
					<displayPlane>UNDERRADAR</displayPlane>
					<drawingPriority>15</drawingPriority>
					<xsl:element name="textPoint">
						<xsl:attribute name="horizontalAlignment"><xsl:text>Center</xsl:text></xsl:attribute> 
						<xsl:attribute name="verticalAlignment"><xsl:text>Top</xsl:text></xsl:attribute> 
					   <element>
						  <text>
							<xsl:value-of select="$sameCount"/>
						  </text>
						  <xsl:call-template name="textStyle">
							 <xsl:with-param name="style">LightMagenta</xsl:with-param>
						  </xsl:call-template>
					   </element>
					   <offset>
						  <x>0</x>
						  <y>-2</y>
					   </offset>
					</xsl:element>
				 </textInstruction>
			</xsl:if>
		</xsl:if>
	</xsl:template>
	<xsl:template match="NavwarnPart[@primitive='Point' and restriction=1]" priority="2">
		<pointInstruction>
			<featureReference>
				<xsl:value-of select="@id"/>
			</featureReference>
			<viewingGroup>31020</viewingGroup>
			<displayPlane>UNDERRADAR</displayPlane>
			<drawingPriority>15</drawingPriority>
			<symbol reference="NavigationalWarningFeaturePart">
			</symbol>
		</pointInstruction>
		<pointInstruction>
			<featureReference>
				<xsl:value-of select="@id"/>
			</featureReference>
			<viewingGroup>31020</viewingGroup>
			<displayPlane>UNDERRADAR</displayPlane>
			<drawingPriority>15</drawingPriority>
			<symbol reference="ApproximatePosition">
			</symbol>
		</pointInstruction>
	</xsl:template>
  <xsl:template match="NavwarnPart[@primitive='Curve']" priority="1">
    <lineInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>31020</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>15</drawingPriority>
      <lineStyleReference reference="NavigationalWarningFeaturePartLine"/>
    </lineInstruction>
  </xsl:template>
  <xsl:template match="NavwarnPart[@primitive='Surface']" priority="1">
    <pointInstruction>
    <featureReference>
      <xsl:value-of select="@id"/>
    </featureReference>
    <viewingGroup>31020</viewingGroup>
    <displayPlane>OVERRADAR</displayPlane>
    <drawingPriority>15</drawingPriority>
    <symbol reference="NavigationalWarningFeaturePartPoint"/>
    </pointInstruction>
    <lineInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>31020</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>15</drawingPriority>
         <xsl:call-template name="simpleLineStyle">
            <xsl:with-param name="style">dash</xsl:with-param>
            <xsl:with-param name="width">0.64</xsl:with-param>
            <xsl:with-param name="colour">CHMGD</xsl:with-param>
         </xsl:call-template>
      <!-- <lineStyleReference reference="NavigationalWarningFeaturePartLine"/> -->
    </lineInstruction>
    <!--<areaInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>31020</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>15</drawingPriority>
      <colorFill>
        <color transparency="0.80">NWMGD</color>
      </colorFill>
    </areaInstruction>-->
  </xsl:template>
  <xsl:template match="NavwarnPart[@primitive='Surface' and restriction=1]" priority="2">
    <pointInstruction>
    <featureReference>
      <xsl:value-of select="@id"/>
    </featureReference>
    <viewingGroup>31020</viewingGroup>
    <displayPlane>OVERRADAR</displayPlane>
    <drawingPriority>15</drawingPriority>
    <symbol reference="NavigationalWarningFeaturePartPoint"/>
    </pointInstruction>
    <lineInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>31020</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>15</drawingPriority>
      <lineStyleReference reference="NavigationalWarningFeaturePartLine"/>
    </lineInstruction>
    <!--<areaInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>31020</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>15</drawingPriority>
      <colorFill>
        <color transparency="0.80">NWMGD</color>
      </colorFill>
    </areaInstruction>-->
  </xsl:template>

</xsl:transform>
