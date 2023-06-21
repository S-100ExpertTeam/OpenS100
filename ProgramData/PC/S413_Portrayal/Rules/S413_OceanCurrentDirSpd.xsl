<?xml version="1.0" encoding="utf-8"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="UTF-8" indent="yes"/>

  <xsl:template match="OceanCurrent[@primitive='Point']" priority="1">
    <xsl:call-template name="select_OceanCurrentDirSpd">
      <xsl:with-param name="viewingGroup">13030</xsl:with-param>
      <xsl:with-param name="displayPlane">UNDERRADAR</xsl:with-param>
      <xsl:with-param name="drawingPriority">10</xsl:with-param>
    </xsl:call-template>
  </xsl:template>
  
  <xsl:template name="select_OceanCurrentDirSpd">
    <xsl:param name="viewingGroup"/>
    <!--  The viewing group  -->
    <xsl:param name="displayPlane"/>
    <!--  The display plane  -->
    <xsl:param name="drawingPriority"/>
    <xsl:variable name="Speed">
      <xsl:choose>
        <xsl:when test="oceanCurrentSpeed and number(oceanCurrentSpeed) = number(oceanCurrentSpeed) ">
          <xsl:value-of select="number(oceanCurrentSpeed)"/>
        </xsl:when>
        <xsl:otherwise>-1</xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:variable name="Direction">
      <xsl:choose>
        <xsl:when test="oceanCurrentDirection and number(oceanCurrentDirection) = number(oceanCurrentDirection) ">
          <xsl:value-of select="number(oceanCurrentDirection)"/>
        </xsl:when>
        <xsl:otherwise>0</xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:variable name="Scale">
      <xsl:choose>
        <xsl:when test="oceanCurrentSpeed and number(oceanCurrentSpeed) = number(oceanCurrentSpeed) and number(oceanCurrentSpeed) > 13">
          <xsl:value-of select="13 * 0.4"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="number(oceanCurrentSpeed) * 0.4"></xsl:value-of>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:choose>
      <xsl:when test="0.49 >= $Speed">
        <pointInstruction>
          <featureReference>
            <xsl:value-of select="@id"/>
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
          <!--<symbol reference="SCAROW01" rotation="$surfaceCurrentDirection"/>-->
          <xsl:element name="symbol">
            <xsl:attribute name="reference">SCAROW01</xsl:attribute>
            <xsl:attribute name="rotation">
              <xsl:value-of select="$Direction"/>
            </xsl:attribute>
            <xsl:attribute name="scaleFactor">
              <xsl:value-of select="$Scale"></xsl:value-of>
            </xsl:attribute>
          </xsl:element>
        </pointInstruction>
      </xsl:when>
      <xsl:when test="0.99 >= $Speed">
        <pointInstruction>
          <featureReference>
            <xsl:value-of select="@id"/>
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
          <xsl:element name="symbol">
            <xsl:attribute name="reference">SCAROW02</xsl:attribute>
            <xsl:attribute name="rotation">
              <xsl:value-of select="$Direction"/>
            </xsl:attribute>
            <xsl:attribute name="scaleFactor">
              <xsl:value-of select="$Scale"></xsl:value-of>
            </xsl:attribute>
          </xsl:element>
        </pointInstruction>
      </xsl:when>
      <xsl:otherwise>
        <pointInstruction>
          <featureReference>
            <xsl:value-of select="@id"/>
          </featureReference>
          <viewingGroup>
          </viewingGroup>
          <xsl:value-of select="$viewingGroup"/>
          <displayPlane>
            <xsl:value-of select="$displayPlane"/>
          </displayPlane>
          <drawingPriority>
            <xsl:value-of select="$drawingPriority"/>
          </drawingPriority>
          <xsl:element name="symbol">
            <xsl:attribute name="reference">SCAROW09</xsl:attribute>
            <xsl:attribute name="rotation">
              <xsl:value-of select="$Direction"/>
            </xsl:attribute>
            <xsl:attribute name="scaleFactor">
              <xsl:value-of select="$Scale"></xsl:value-of>
            </xsl:attribute>
          </xsl:element>
        </pointInstruction>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>
  
  <!--<xsl:template match="S413_OceanCurrentDirSpd[@primitive='Point']" priority="1">
    <xsl:variable name="Speed" select="speed"/>
    <xsl:variable name="Direction" select="direction"/>
    <xsl:variable name="scale">
      <xsl:choose>
        <xsl:when test="$Speed >= 1">
          <xsl:value-of select="speed * 0.5"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="0.5"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:choose>
      <xsl:when test="0.49 >= $Speed">
        <pointInstruction>
          <featureReference>
            <xsl:value-of select="@id"/>
          </featureReference>
          <viewingGroup>26010</viewingGroup>
          <displayPlane>UNDERRADAR</displayPlane>
          <drawingPriority>15</drawingPriority>
          <xsl:element name="symbol">
            <xsl:attribute name="reference">SCAROW01</xsl:attribute>
            <xsl:attribute name="rotation">
              <xsl:value-of select="$Direction"/>
            </xsl:attribute>
            <xsl:attribute name="scaleFactor">
              <xsl:value-of select="$scale"></xsl:value-of>
            </xsl:attribute>
          </xsl:element>
        </pointInstruction>
      </xsl:when>
      <xsl:when test="0.99 >= $Speed">
        <pointInstruction>
          <featureReference>
            <xsl:value-of select="@id"/>
          </featureReference>
          <viewingGroup>26010</viewingGroup>
          <displayPlane>UNDERRADAR</displayPlane>
          <drawingPriority>15</drawingPriority>
          <xsl:element name="symbol">
            <xsl:attribute name="reference">SCAROW02</xsl:attribute>
            <xsl:attribute name="rotation">
              <xsl:value-of select="$Direction"/>
            </xsl:attribute>
            <xsl:attribute name="scaleFactor">
              <xsl:value-of select="$scale"></xsl:value-of>
            </xsl:attribute>
          </xsl:element>
        </pointInstruction>
      </xsl:when>
      <xsl:otherwise>
        <pointInstruction>
          <featureReference>
            <xsl:value-of select="@id"/>
          </featureReference>
          <viewingGroup>26010</viewingGroup>
          <displayPlane>UNDERRADAR</displayPlane>
          <drawingPriority>15</drawingPriority>
          <xsl:element name="symbol">
            <xsl:attribute name="reference">SCAROW09</xsl:attribute>
            <xsl:attribute name="rotation">
              <xsl:value-of select="$Direction"/>
            </xsl:attribute>
            <xsl:attribute name="scaleFactor">
              <xsl:value-of select="$scale"></xsl:value-of>
            </xsl:attribute>
          </xsl:element>
        </pointInstruction>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>-->
  
</xsl:transform>