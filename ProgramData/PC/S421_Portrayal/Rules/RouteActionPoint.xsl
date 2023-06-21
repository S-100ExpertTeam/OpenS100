<?xml version="1.0" encoding="UTF-8"?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
  <!-- Surface: AreaFillGroup  : 
	 1: color
	 2: areaFillReference
	 3: pixmap - no working
	 4: symbol
	 5: hatch-->
  <xsl:param  name="s421AreaFillGroupSelect">0</xsl:param>
  <!-- TextSelect : <1: TextPoint, else: TextLine> -->
  <!-- FontSelect : <1: FontCharacteristics, else: FontReferences> -->
  <xsl:param  name="s421TextSelect">2</xsl:param>
  <xsl:param name="s421FontSelect">2</xsl:param>

  <xsl:template match="RouteActionPoint[@primitive='Point']" priority="1">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>52270</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="RTEACTPOT01"/>
    </pointInstruction>
    <!-- routeActionPointRequiredActionDescription : Text Display Method -->
    <xsl:if test="routeActionPointRequredActionDescription != ''">
      <xsl:choose>
        <xsl:when test="$s421TextSelect = 1">
          <!-- TextPoint -->
          <textInstruction>
            <featureReference>
              <xsl:value-of select="@id"/>
            </featureReference>
            <viewingGroup>52270</viewingGroup>
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
            <viewingGroup>52270</viewingGroup>
            <displayPlane>OVERRADAR</displayPlane>
            <drawingPriority>24</drawingPriority>
            <textPoint horizontalAlignment="Start" verticalAlignment="Bottom">
              <element>
                <text>
                  <xsl:apply-templates select="routeActionPointRequredActionDescription" mode="text"/>
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
  <xsl:template match="RouteActionPoint[@primitive='Curve']" priority="1">
    <lineInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>52270</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <lineStyleReference reference="RTEACTPOTLINE"/>
    </lineInstruction>
    <!-- routeActionPointRequiredActionDescription : Text Display Method -->
    <xsl:if test="routeActionPointRequiredActionDescription != ''">
      <xsl:choose>
        <xsl:when test="$s421TextSelect = 1">
          <!-- TextPoint -->
          <textInstruction>
            <featureReference>
              <xsl:value-of select="@id"/>
            </featureReference>
            <viewingGroup>52270</viewingGroup>
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
            <viewingGroup>52270</viewingGroup>
            <displayPlane>OVERRADAR</displayPlane>
            <drawingPriority>24</drawingPriority>
            <textPoint horizontalAlignment="Start" verticalAlignment="Bottom">
              <element>
                <text>
                  <xsl:apply-templates select="routeActionPointRequiredActionDescription" mode="text"/>
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
  <xsl:template match="RouteActionPoint[@primitive='Surface']" priority="1">
    <lineInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>52270</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <xsl:call-template name="simpleLineStyle">
        <xsl:with-param name="style">solid</xsl:with-param>
        <xsl:with-param name="width">0.64</xsl:with-param>
        <xsl:with-param name="colour">PLRTE</xsl:with-param>
      </xsl:call-template>
    </lineInstruction>

    <areaInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>52270</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <xsl:choose>
        <xsl:when test="$s421AreaFillGroupSelect = 1">
          <!-- 1: color -->
          <colorFill>
            <color transparency="0.5">#AA44A8</color>
          </colorFill>
        </xsl:when>
        <xsl:when test="$s421AreaFillGroupSelect = 2">
          <!-- 2: areaFillReference -->
          <areaFillReference reference="RTEACTPOT01"/>
        </xsl:when>
        <xsl:when test="$s421AreaFillGroupSelect = 3">
          <!-- 3: pixmap -->
          <pixmapFill>
            <areaCRS>Global</areaCRS>
            <pixmap reference="RTEACTPOT01">
              <overrideAll transparency="0.5">#AA44A8</overrideAll>
              <override>
                <override transparency="0.5">#AA44A8</override>
                <color transparency="0.5">#AA44A8</color>
              </override>
            </pixmap>
          </pixmapFill>
        </xsl:when>
        <xsl:when test="$s421AreaFillGroupSelect = 4">
          <!-- 4: symbol -->
          <symbolFill clipSymbols="true">
            <areaCRS>Global</areaCRS>
            <symbol reference="LineRef01" rotation="0.0" rotationCRS="LocalCRS" scaleFactor="1.0">
              <offset>
                <x>53.000595</x>
                <y>0.251287</y>
              </offset>
              <overrideAll transparency="0.5">#AA44A8</overrideAll>
              <override>
                <override transparency="0.5">#AA44A8</override>
                <color transparency="0.5">#AA44A8</color>
              </override>
              <linePlacement placementMode="Relative">
                <offset>0.5</offset>
              </linePlacement>
              <areaPlacement placementMode="VisibleParts"/>
            </symbol>
            <v1>
              <x>53.000595</x>
              <y>0.251287</y>
            </v1>
            <v2>
              <x>55.441282</x>
              <y>4.085683</y>
            </v2>
          </symbolFill>
        </xsl:when>
        <xsl:when test="$s421AreaFillGroupSelect = 5">
          <!-- 5: hatch -->
          <hatchFill>
            <areaCRS>Global</areaCRS>
            <hatch>
              <lineStyle capStyle="Round" joinStyle="Round" offset="0.01">
                <intervalLength>0.5</intervalLength>
                <pen width="0.2">
                  <color transparency="0.5">#AA44A8</color>
                </pen>
                <dash>
                  <start>53.000595</start>
                  <length>0.2</length>
                </dash>
                <symbol reference="LineRef01" rotation="0.0" scaleFactor="1.0" crsType="LocalCRS" >
                  <position>53.000595</position>
                </symbol>
              </lineStyle>
              <direction>
                <x>53.000595</x>
                <y>0.251287</y>
              </direction>
              <distance>0.50</distance>
            </hatch>
          </hatchFill>
        </xsl:when>
        <xsl:otherwise>
          <!-- etc : none -->
        </xsl:otherwise>
      </xsl:choose>
    </areaInstruction>

    <!-- routeActionPointRequiredActionDescription : Text Display Method -->
    <xsl:if test="routeActionPointRequiredActionDescription != ''">
      <xsl:choose>
        <xsl:when test="$s421TextSelect = 1">
          <!-- TextPoint -->
          <textInstruction>
            <featureReference>
              <xsl:value-of select="@id"/>
            </featureReference>
            <viewingGroup>52270</viewingGroup>
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
            <viewingGroup>52270</viewingGroup>
            <displayPlane>OVERRADAR</displayPlane>
            <drawingPriority>24</drawingPriority>
            <textPoint horizontalAlignment="Start" verticalAlignment="Bottom">
              <element>
                <text>
                  <xsl:apply-templates select="routeActionPointRequiredActionDescription" mode="text"/>
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