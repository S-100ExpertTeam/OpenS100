<?xml version="1.0" encoding="utf-8" ?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output indent="yes" version="1.0"/>

  <xsl:template name="AreaHatchFillTemplate">
    <xsl:param name="featureReference"></xsl:param>
    <xsl:param name="viewingGroup"/>
    <xsl:param name="displayPlane"/>
    <xsl:param name="drawingPriority"/>
    <xsl:param name="x"/>
    <xsl:param name="y"/>
    <xsl:param name="distance"/>
    <xsl:param name="offset"></xsl:param>
    <xsl:param name="penWidth"></xsl:param>
    <xsl:param name="colorToken"></xsl:param>
    <xsl:param name="trasparency"></xsl:param>
    <xsl:param name="capStyle"></xsl:param>
    <xsl:param name="joinStyle"></xsl:param>
    
    <xsl:choose>
      <xsl:when test="$featureReference!=''">
        <xsl:element name="areaInstruction">
          <xsl:element name="featureReference">
            <xsl:attribute name="reference">
              <xsl:value-of select="$featureReference"/>
            </xsl:attribute>
          </xsl:element>
          <xsl:element name="viewingGroup">
            <xsl:value-of select="$viewingGroup"/>
          </xsl:element>
          <xsl:element name="displayPlane">
            <xsl:value-of select="$displayPlane"/>
          </xsl:element>
          <xsl:element name="drawingPriority">
            <xsl:value-of select="$drawingPriority"/>
          </xsl:element>
          <xsl:element name="hatchFill">
            <xsl:element name="hatch">
              <xsl:element name="direction">
                <xsl:element name="vector">
                  <xsl:element name="x">
                    <xsl:value-of select="$x"/>
                  </xsl:element>
                  <xsl:element name="y">
                    <xsl:value-of select="$y"/>
                  </xsl:element>
                </xsl:element>
              </xsl:element>
                <xsl:element name="distance">
                  <xsl:value-of select="$distance"/>
                </xsl:element>
                <xsl:element name="lineStyle">
                  <xsl:element name="offset">
                    <xsl:value-of select="$offset"/>
                  </xsl:element>
                  <xsl:element name="capStyle">
                    <xsl:value-of select="$capStyle"/>
                  </xsl:element>
                  <xsl:element name="joinStyle">
                    <xsl:value-of select="$joinStyle"/>
                  </xsl:element>
                  <xsl:element name="pen">
                    <xsl:attribute name="width">
                      <xsl:value-of select="$penWidth"/>
                    </xsl:attribute>
                    <xsl:element name="color">
                      <xsl:element name="token">
                        <xsl:value-of select="$colorToken"/>
                      </xsl:element>
                      <xsl:element name="trasparency">
                        <xsl:value-of select="$trasparency"/>
                      </xsl:element>
                    </xsl:element>
                  </xsl:element>
                </xsl:element>
               </xsl:element>              
            </xsl:element>
          </xsl:element>       
      </xsl:when>
    </xsl:choose>
  </xsl:template>
</xsl:transform>

