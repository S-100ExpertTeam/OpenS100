<?xml version="1.0" encoding="UTF-8"?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
  <xsl:template match="CoastguardStation[@primitive='Point']" priority="1">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>12310</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>15</drawingPriority>
      <symbol reference="CostGuardStattion"/>
    </pointInstruction>
    <xsl:if test="communicationChannel!= ''">
      <textInstruction>
        <featureReference>
          <xsl:value-of select="@id"/>
        </featureReference>
        <viewingGroup>21</viewingGroup>
        <displayPlane>OVERRADAR</displayPlane>
        <drawingPriority>15</drawingPriority>
        <textPoint>
          <element>
            <text>
              <xsl:apply-templates select="communicationChannel" mode="text">
                <xsl:with-param name="prefix">Prod </xsl:with-param>
              </xsl:apply-templates>
            </text>
            <xsl:call-template name="textStyle">
              <xsl:with-param name="style">default</xsl:with-param>
            </xsl:call-template>
          </element>
          <offset>
            <x>5.51</x>
            <y>4.51</y>
          </offset>
          <areaPlacement placementMode="VisibleParts"/>
        </textPoint>
      </textInstruction>
    </xsl:if>
  </xsl:template>
</xsl:transform>
