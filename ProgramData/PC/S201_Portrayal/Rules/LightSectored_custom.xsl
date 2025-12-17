<?xml version="1.0" encoding="UTF-8"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="UTF-8" indent="yes"/>

  <!--  template for sectored lights -->
  <!--  First draft April 14, 2015 -->
  <!--  Added support for directional and Moire lights May 30, 2016 -->
  <xsl:template match="Light[@primitive='Point' and (categoryOfLight=1 or categoryOfLight=16)]" priority="1">
    <xsl:variable name="viewingGroup">27070</xsl:variable>
    <xsl:variable name="displayPlane">OVERRADAR</xsl:variable>
    <xsl:variable name="drawingPriority" select="24"/>
    <xsl:variable name="fid" select="@id"/>

    <!--  Loop over each light sector, with a sector limit -->
    <xsl:variable name="crs">
      <xsl:choose>
        <xsl:when test="$FULL_SECTORS = 'true' " >
          <xsl:text>GeographicCRS</xsl:text>
        </xsl:when>
        <xsl:otherwise>
          <xsl:text>LocalCRS</xsl:text>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:variable name="sectorLimitTwo">
      <xsl:value-of select="sectorLimitTwo"/>
    </xsl:variable>
    <xsl:variable name="sectorLimitOne">
      <xsl:value-of select="sectorLimitOne"/>
    </xsl:variable>
    <xsl:variable name="length">
      <xsl:choose>
        <xsl:when test="$FULL_SECTORS = 'true' and valueOfNominalRange " >
          <xsl:value-of select="valueOfNominalRange * 1852"/> 
        </xsl:when>
        <xsl:when test="$FULL_SECTORS = 'true' " >
          <xsl:value-of select="9 * 1852"/> 
        </xsl:when>
        <xsl:otherwise>
          <xsl:text>25</xsl:text>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <!--<xsl:variable name="length">
      <xsl:choose>
        <xsl:when test="$FULL_SECTORS = 'true' and valueOfNominalRange " >
          <xsl:value-of select="valueOfNominalRange * 1852"/> Nautical Miles*1852 = Metres
        </xsl:when>
        <xsl:when test="$FULL_SECTORS = 'true' " >
          <xsl:value-of select="9 * 1852"/> Nautical Miles*1852 = Metres
        </xsl:when>
        <xsl:otherwise>
          <xsl:text>25</xsl:text> 25 mm on the screen
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>-->


    <!--  An augmented ray for each sector leg with Local CRS -->
    <augmentedRay>
      <featureReference>
        <xsl:value-of select="$fid"/>
      </featureReference>
      <viewingGroup>
        <xsl:value-of select="$viewingGroup"/>
      </viewingGroup>
      <displayPlane>
        <xsl:value-of select="$displayPlane"/>
      </displayPlane>
      <drawingPriority>
        <xsl:value-of select="$drawingPriority"/>
      </drawingPriority>
      <crs>
        <xsl:value-of select="$crs"/>
      </crs>
       <!--LocalCRS,  geographicCRS when using actual VALNMR
       add a linestlye element--> 
      <xsl:call-template name="simpleLineStyle">
        <xsl:with-param name="style">dash</xsl:with-param>
        <xsl:with-param name="width">0.32</xsl:with-param>
        <xsl:with-param name="colour">CHBLK</xsl:with-param>
      </xsl:call-template>
      <direction>
          <!--need to flip the bearing by 180 because values are seaward bearing-->
        <xsl:choose>
          <xsl:when test="$sectorLimitOne >= 180 " >
            <xsl:value-of select="$sectorLimitOne - 180"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="$sectorLimitOne + 180"/>
          </xsl:otherwise>
        </xsl:choose>
      </direction>
        <!--direction from sector limit +/- 180--> 
      <length>
        <xsl:value-of select="$length"/>
      </length>
        <!--Length fixed or nominal range--> 
    </augmentedRay>
    <!--  An augmented ray for each sector leg with Local CRS -->
    <augmentedRay>
      <featureReference>
        <xsl:value-of select="$fid"/>
      </featureReference>
      <viewingGroup>
        <xsl:value-of select="$viewingGroup"/>
      </viewingGroup>
      <displayPlane>
        <xsl:value-of select="$displayPlane"/>
      </displayPlane>
      <drawingPriority>
        <xsl:value-of select="$drawingPriority"/>
      </drawingPriority>
      <crs>
        <xsl:value-of select="$crs"/>
      </crs>
       <!--LocalCRS,  geographicCRS when using actual VALNMR
       add a linestlye element--> 
      <xsl:call-template name="simpleLineStyle">
        <xsl:with-param name="style">dash</xsl:with-param>
        <xsl:with-param name="width">0.32</xsl:with-param>
        <xsl:with-param name="colour">CHBLK</xsl:with-param>
      </xsl:call-template>
      <direction>
          <!--need to flip the bearing by 180 because values are seaward bearing-->
        <xsl:choose>
          <xsl:when test="$sectorLimitTwo >= 180 " >
            <xsl:value-of select="$sectorLimitTwo - 180"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="$sectorLimitTwo + 180"/>
          </xsl:otherwise>
        </xsl:choose>
      </direction>
        <!--direction from sector limit +/- 180--> 
      <length>
        <xsl:value-of select="$length"/>
      </length>
        <!--Length fixed or nominal range--> 
    </augmentedRay>

    <xsl:variable name="radius">
      <xsl:choose>
        <xsl:when test="sectorExtension" >
          <xsl:value-of select="sectorExtension + 20"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:text>20</xsl:text>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:variable name="lineStyle">
      <xsl:choose>
        <xsl:when test="lightVisibility = '7' or lightVisibility = '8' or lightVisibility = '3'" >
          <xsl:text>dash</xsl:text>
        </xsl:when>
        <xsl:otherwise>
          <xsl:text>solid</xsl:text>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:variable name="lineWidth">
      <xsl:choose>
        <xsl:when test="$lineStyle = 'dash'" >
          <xsl:text>0.32</xsl:text>
        </xsl:when>
        <xsl:otherwise>
          <xsl:text>0.64</xsl:text>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:variable name="colourToken">
      <xsl:choose>
        <xsl:when test="$lineStyle = 'dash'" >
          <xsl:text>CHBLK</xsl:text>
        </xsl:when>
        <xsl:when test="colour = '1' and colour = '3'" >
          <xsl:text>LITRD</xsl:text>
        </xsl:when>
        <xsl:when test="colour = '3'" >
          <xsl:text>LITRD</xsl:text>
        </xsl:when>
        <xsl:when test="colour = '1' and colour = '4'" >
          <xsl:text>LITGN</xsl:text>
        </xsl:when>
        <xsl:when test="colour = '4'" >
          <xsl:text>LITGN</xsl:text>
        </xsl:when>
        <xsl:when test="colour = '5' and colour = '6' " >
          <xsl:text>LITYW</xsl:text>
        </xsl:when>
        <xsl:when test="colour = '11' or colour = '6' or colour = '1' " >
          <xsl:text>LITYW</xsl:text>
        </xsl:when>
        <xsl:otherwise>
          <xsl:text>CHMGD</xsl:text>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <!--  An augmented Path for the sector arc Local CRS -->
    <xsl:if test="$lineStyle = 'solid'">
      <augmentedPath>
        <featureReference>
          <xsl:value-of select="$fid"/>
        </featureReference>
        <viewingGroup>
          <xsl:value-of select="$viewingGroup"/>
        </viewingGroup>
        <displayPlane>
          <xsl:value-of select="$displayPlane"/>
        </displayPlane>
        <drawingPriority>
          <xsl:value-of select="$drawingPriority"/>
        </drawingPriority>
        <crs>LocalCRS</crs>
         <!--add a linestlye element--> 
        <xsl:call-template name="simpleLineStyle">
          <xsl:with-param name="style">solid</xsl:with-param>
          <xsl:with-param name="width">1.28</xsl:with-param>
          <xsl:with-param name="colour">OUTLW</xsl:with-param>
        </xsl:call-template>
        <path>
          <arcByRadius>
            <center>
              <x>0</x>
              <y>0</y>
            </center>
            <sector>
              <startAngle>
                <xsl:choose>
                  <xsl:when test="sectorLimitOne >= 180 " >
                    <xsl:value-of select="sectorLimitOne - 180"/>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:value-of select="sectorLimitOne + 180"/>
                  </xsl:otherwise>
                </xsl:choose>
              </startAngle>
              <angularDistance>
                <xsl:choose>
                  <xsl:when test="sectorLimitTwo > sectorLimitOne " >
                    <xsl:value-of select="sectorLimitTwo - sectorLimitOne"/>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:value-of select="sectorLimitTwo - sectorLimitOne + 360"/>
                  </xsl:otherwise>
                </xsl:choose>
              </angularDistance>
            </sector>
            <radius>
              <xsl:value-of select="$radius"/>
            </radius>
          </arcByRadius>
        </path>
      </augmentedPath>
    </xsl:if>

    <!--  An augmented Path for the sector arc Local CRS -->
    <augmentedPath>
      <featureReference>
        <xsl:value-of select="$fid"/>
      </featureReference>
      <viewingGroup>
        <xsl:value-of select="$viewingGroup"/>
      </viewingGroup>
      <displayPlane>
        <xsl:value-of select="$displayPlane"/>
      </displayPlane>
      <drawingPriority>
        <xsl:value-of select="$drawingPriority"/>
      </drawingPriority>
      <crs>LocalCRS</crs>
       <!--add a linestlye element--> 
      <xsl:call-template name="simpleLineStyle">
        <xsl:with-param name="style">
          <xsl:value-of select="$lineStyle"/>
        </xsl:with-param>
        <xsl:with-param name="width">
          <xsl:value-of select="$lineWidth"/>
        </xsl:with-param>
        <xsl:with-param name="colour">
          <xsl:value-of select="$colourToken"/>
        </xsl:with-param>
      </xsl:call-template>
      <path>
        <arcByRadius>
          <center>
            <x>0</x>
            <y>0</y>
          </center>
          <sector>
            <startAngle>
              <xsl:choose>
                <xsl:when test="sectorLimitOne >= 180 " >
                  <xsl:value-of select="sectorLimitOne - 180"/>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:value-of select="sectorLimitOne + 180"/>
                </xsl:otherwise>
              </xsl:choose>
            </startAngle>
            <angularDistance>
              <xsl:choose>
                <xsl:when test="sectorLimitTwo > sectorLimitOne " >
                  <xsl:value-of select="sectorLimitTwo - sectorLimitOne"/>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:value-of select="sectorLimitTwo - sectorLimitOne + 360"/>
                </xsl:otherwise>
              </xsl:choose>
            </angularDistance>
          </sector>
          <radius>
            <xsl:value-of select="$radius"/>
          </radius>
        </arcByRadius>
      </path>
    </augmentedPath>

    <!--  Loop over each light sector, with a directional character and orientation not null -->
    <!--<xsl:for-each select="sectorCharacteristic/lightSector[directionalCharacter/orientation/orientationValue and directionalCharacter/orientation/orientationValue != '']">

      --><!--  Draw a directional line (augmented ray) to show the direction LS(DASH,1,CHBLK) --><!--
      --><!--  remember bearing is from seaward so we want 180 from that --><!--
      --><!--  Use VALNMR but if there is none use '9'  -->
      <xsl:variable name="SeawardOrient">
        <xsl:value-of select="orientation"/>
      </xsl:variable>
      <xsl:variable name="FromLightOrient">
        <xsl:choose>
          <xsl:when test="$SeawardOrient >= 180 " >
            <xsl:value-of select="$SeawardOrient - 180"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="$SeawardOrient + 180"/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:variable>
      <!--<xsl:variable name="length">
        <xsl:choose>
          <xsl:when test="valueOfNominalRange" >
            <xsl:value-of select="valueOfNominalRange * 1852"/>
              --><!--Nautical Miles*1852 = Metres--><!--  
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="9 * 1852"/>
              --><!--Nautical Miles*1852 = Metres--><!--
          </xsl:otherwise>
        </xsl:choose>
      </xsl:variable>-->

      --><!--  An augmented ray for each sector leg with Local CRS --><!--
      <augmentedRay>
        <featureReference>
          <xsl:value-of select="$fid"/>
        </featureReference>
        <viewingGroup>
          <xsl:value-of select="$viewingGroup"/>
        </viewingGroup>
        <displayPlane>
          <xsl:value-of select="$displayPlane"/>
        </displayPlane>
        <drawingPriority>
          <xsl:value-of select="$drawingPriority"/>
        </drawingPriority>
        <crs>
          <xsl:text>GeographicCRS</xsl:text>
        </crs>
        --><!-- add a linestlye element --><!--
        <xsl:call-template name="simpleLineStyle">
          <xsl:with-param name="style">dash</xsl:with-param>
          <xsl:with-param name="width">0.32</xsl:with-param>
          <xsl:with-param name="colour">CHBLK</xsl:with-param>
        </xsl:call-template>
        <direction>
          --><!--  need to flip the bearing by 180 because values are seaward bearing--><!--
          <xsl:value-of select="$FromLightOrient"/>
        </direction>
        <length>
          <xsl:value-of select="$length"/>
        </length>
        --><!--  Length fixed or nominal range --><!--
      </augmentedRay>


      --><!--  Lookup colour to get light flare symbol --><!--
      <xsl:variable name="Symbol">
        <xsl:choose>
          <xsl:when test="colour = '3' ">
            <xsl:text>LIGHTS11</xsl:text>
          </xsl:when>
          <xsl:when test="colour = '4' ">
            <xsl:text>LIGHTS12</xsl:text>
          </xsl:when>
          <xsl:when test="colour = '11' ">
            <xsl:text>LIGHTS13</xsl:text>
          </xsl:when>
          <xsl:when test="colour = '6' ">
            <xsl:text>LIGHTS13</xsl:text>
          </xsl:when>
          <xsl:when test="colour = '1' ">
            <xsl:text>LIGHTS13</xsl:text>
          </xsl:when>
          <xsl:when test="colour = '5' and colour = '6' ">
            <xsl:text>LIGHTS13</xsl:text>
          </xsl:when>
          <xsl:otherwise>
            <xsl:text>LITDEF11</xsl:text>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:variable>
      --><!--  Draw selected light flare symbol using orientation/orientationValue adjusted by 180 --><!--
      <pointInstruction>
        <featureReference>
          <xsl:value-of select="$fid"/>
        </featureReference>
        <viewingGroup>27070</viewingGroup>
        <displayPlane>OVERRADAR</displayPlane>
        <drawingPriority>24</drawingPriority>
        <xsl:element name="symbol">
          <xsl:attribute name="reference">
            <xsl:value-of select="$Symbol"/>
          </xsl:attribute>
          <xsl:attribute name="rotation">
            <xsl:value-of select="$FromLightOrient"/>
          </xsl:attribute>
        </xsl:element>
      </pointInstruction>

      -->
    
    <!--  add orientation text using seaward orientation "TE('%03.0lf deg','ORIENT', 3,3,3, '15110', 3,1, CHELK, 23)"  -->
      <textInstruction>
        <featureReference>
          <xsl:value-of select="$fid"/>
        </featureReference>
        <viewingGroup>23</viewingGroup>
        <displayPlane>OVERRADAR</displayPlane>
        <drawingPriority>24</drawingPriority>
        <textPoint horizontalAlignment="Start" verticalAlignment="Top">
          <element>
            <text>
              <xsl:value-of select="format-number($SeawardOrient, '000','dformat')"/>
              <xsl:text> deg</xsl:text>
            </text>
            <bodySize>10</bodySize>
            <foreground>CHBLK</foreground>
            <fontCharacteristics>
              <weight>Medium</weight>
              <slant>Upright</slant>
              <serifs>0</serifs>
              <proportion>MonoSpaced</proportion>
            </fontCharacteristics>
          </element>
          <offset>
            <x>10.53</x>
             <!--3 * 10 * 0.351--> 
            <y>-3.51</y>
          </offset>
        </textPoint>
      </textInstruction>

       <!--Add light description--> 
      <xsl:call-template name="LIGHTDESCRIPTION">
        <xsl:with-param name="catLight" select="1"/>
        <xsl:with-param name="lightStatus" select="status"/>
        <xsl:with-param name="colour" select="colour"/>
        <xsl:with-param name="signalGroup" select="signalGroup"/>
        <xsl:with-param name="signalPeriod" select="signalPeriod"/>
        <xsl:with-param name="height" select="height"/>
        <xsl:with-param name="nominalRange" select="valueOfNominalRange"/>
        <xsl:with-param name="lightChar" select="lightCharacteristic"/>
        <xsl:with-param name="fid" select="$fid"/>
        <xsl:with-param name="flareAt45" select="'false'"/>
      </xsl:call-template>

    <!--</xsl:for-each>-->

  </xsl:template>

</xsl:transform>
