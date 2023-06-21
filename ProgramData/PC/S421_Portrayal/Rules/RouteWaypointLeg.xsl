<?xml version="1.0" encoding="UTF-8"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
  <xsl:template match="RouteWaypointLeg[@primitive='Curve']" priority="1">
    <!-- TextSelect : <1: TextPoint, else: TextLine> -->
    <!-- FontSelect : <1: FontCharacteristics, else: FontReferences> -->
    <xsl:param  name="s421TextSelect">2</xsl:param>
    <xsl:param name="s421FontSelect">2</xsl:param>
    <lineInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>52240</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <lineStyleReference reference="RTEACTLEGLINE"/>
    </lineInstruction>
    <!-- routeWaypointLegIssue : Text Display Method 1-->
    <xsl:if test="routeWaypointLegIssue != ''">
      <xsl:choose>
        <xsl:when test="$s421TextSelect = 1">
          <!-- TextPoint -->
          <textInstruction>
            <featureReference>
              <xsl:value-of select="@id"/>
            </featureReference>
            <viewingGroup>52240</viewingGroup>
            <displayPlane>OVERRADAR</displayPlane>
            <drawingPriority>24</drawingPriority>
            <textPoint horizontalAlignment="Start" verticalAlignment="Bottom" rotation="0.0" >
              <element verticalOffset="0.0">
                <text>Shallow Water</text>
                <bodySize>10</bodySize>
                <flags>
                  <flag>UpperLine</flag>
                </flags>
                <foreground transparency="0.5">AA44A8</foreground>
                <background transparency="0.5">AA44A8</background>
                <xsl:choose>
                  <xsl:when test="$s421FontSelect = 1">
                    <!-- FontCharateristics -->
                    <fontCharacteristics>
                      <weight>Medium</weight>
                      <slant>Upright</slant>
                      <serifs>false</serifs>
                      <proportion>Proportional</proportion>
                    </fontCharacteristics>
                  </xsl:when>
                  <xsl:otherwise>
                    <!-- FontReference -->
                    <fontReference reference="TextRef01">
                      <weight>Medium</weight>
                      <slant>Upright</slant>
                    </fontReference>
                  </xsl:otherwise>
                </xsl:choose>
              </element>
              <offset>
                <x>0.5</x>
                <y>1.0</y>
              </offset>
              <areaPlacement placementMode="VisibleParts"/>
            </textPoint>
          </textInstruction>
        </xsl:when>
        <xsl:otherwise>
          <!-- TextLine -->
          <textInstruction>
            <featureReference>
              <xsl:value-of select="@id"/>
            </featureReference>
            <viewingGroup>21010</viewingGroup>
            <displayPlane>OVERRADAR</displayPlane>
            <drawingPriority>24</drawingPriority>
            <textPoint horizontalAlignment="Start" verticalAlignment="Bottom" >
              <element>
                <text>
                  <xsl:apply-templates select="routeWaypointLegIssue" mode="text"/>
                </text>
                <bodySize>10</bodySize>
                <flags>
                  <flag>UpperLine</flag>
                </flags>
                <foreground transparency="0.5">AA44A8</foreground>
                <background transparency="0.5">AA44A8</background>
                <xsl:choose>
                  <xsl:when test="$s421FontSelect = 1">
                    <!-- FontCharateristics -->
                    <fontCharacteristics>
                      <weight>Medium</weight>
                      <slant>Upright</slant>
                      <serifs>false</serifs>
                      <proportion>Proportional</proportion>
                    </fontCharacteristics>
                  </xsl:when>
                  <xsl:otherwise>
                    <!-- FontReference -->
                    <fontReference reference="TextRef01">
                      <weight>Medium</weight>
                      <slant>Upright</slant>
                    </fontReference>
                  </xsl:otherwise>
                </xsl:choose>
              </element>
              <startOffset>2.0</startOffset>
              <endOffset>2.0</endOffset>
              <placementMode>Relative</placementMode>
            </textPoint>
          </textInstruction>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:if>
  </xsl:template>
</xsl:transform>

