<?xml version="1.0" encoding="UTF-8"?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
	<xsl:output method="xml" encoding="UTF-8" indent="yes"/>
	<xsl:template match="*[@primitive='Point']">
		<pointInstruction>
			<featureReference>
				<xsl:value-of select="@id"/>
			</featureReference>
			<viewingGroup>21010</viewingGroup>
			<displayPlane>OVERRADAR</displayPlane>
			<drawingPriority>24</drawingPriority>
			<symbol reference="USRPNT01"/>
		</pointInstruction>
		<xsl:if test="featureName!= ''">
			<textInstruction>
				<featureReference>
					<xsl:value-of select="@id"/>
				</featureReference>
				<viewingGroup>26</viewingGroup>
				<displayPlane>UNDERRADAR</displayPlane>
				<drawingPriority>24</drawingPriority>
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
						<x>2</x>
						<y>0</y>
					</offset>
					<areaPlacement placementMode="VisibleParts"/>
				</textPoint>
			</textInstruction>
		</xsl:if>
	</xsl:template>
	<xsl:template match="*[@primitive='Curve']">
		<lineInstruction>
			<featureReference>
				<xsl:value-of select="@id"/>
			</featureReference>
			<viewingGroup>36050</viewingGroup>
			<displayPlane>UNDERRADAR</displayPlane>
			<drawingPriority>21</drawingPriority>
			<xsl:call-template name="simpleLineStyle">
				<xsl:with-param name="style">solid</xsl:with-param>
				<xsl:with-param name="width">0.32</xsl:with-param>
				<xsl:with-param name="colour">CHGRF</xsl:with-param>
			</xsl:call-template>
		</lineInstruction>
		<xsl:if test="featureName!= ''">
			<textInstruction>
				<featureReference>
					<xsl:value-of select="@id"/>
				</featureReference>
				<viewingGroup>26</viewingGroup>
				<displayPlane>UNDERRADAR</displayPlane>
				<drawingPriority>24</drawingPriority>
				<textPoint horizontalAlignment="Center" verticalAlignment="Center">
					<element>
						<text>
							<xsl:apply-templates select="featureName" mode="text"/>
						</text>
						<xsl:call-template name="textStyle">
							<xsl:with-param name="style">default</xsl:with-param>
						</xsl:call-template>
					</element>
					<offset>
						<x>2</x>
						<y>0</y>
					</offset>
					<areaPlacement placementMode="VisibleParts"/>
				</textPoint>
			</textInstruction>
		</xsl:if>
	</xsl:template>
	<xsl:template match="*[@primitive='Surface']">
		<areaInstruction>
			<featureReference>
				<xsl:value-of select="@id"/>
			</featureReference>
			<viewingGroup>53010</viewingGroup>
			<displayPlane>OVERRADAR</displayPlane>
			<drawingPriority>12</drawingPriority>
			<colorFill>
				<color transparency="0.75">DNGHL</color>
			</colorFill>
		</areaInstruction>
		<lineInstruction>
			<featureReference>
				<xsl:value-of select="@id"/>
			</featureReference>
			<viewingGroup>53010</viewingGroup>
			<displayPlane>OVERRADAR</displayPlane>
			<drawingPriority>21</drawingPriority>
			<xsl:call-template name="simpleLineStyle">
				<xsl:with-param name="style">dash</xsl:with-param>
				<xsl:with-param name="width">0.32</xsl:with-param>
				<xsl:with-param name="colour">DNGHL</xsl:with-param>
			</xsl:call-template>
		</lineInstruction>
		<xsl:if test="featureName!= ''">
			<textInstruction>
				<featureReference>
					<xsl:value-of select="@id"/>
				</featureReference>
				<viewingGroup>26</viewingGroup>
				<displayPlane>UNDERRADAR</displayPlane>
				<drawingPriority>24</drawingPriority>
				<textPoint horizontalAlignment="Center" verticalAlignment="Center">
					<element>
						<text>
							<xsl:apply-templates select="featureName" mode="text"/>
						</text>
						<xsl:call-template name="textStyle">
							<xsl:with-param name="style">default</xsl:with-param>
						</xsl:call-template>
					</element>
					<offset>
						<x>2</x>
						<y>0</y>
					</offset>
					<areaPlacement placementMode="VisibleParts"/>
				</textPoint>
			</textInstruction>
		</xsl:if>
	</xsl:template>
</xsl:transform>
