<?xml version="1.0" encoding="utf-8" ?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output indent="yes" version="1.0"/>

  <xsl:template name="SimpleLineTemplate">
    <xsl:param name="featureReference"></xsl:param>
    <xsl:param name="viewingGroup"></xsl:param>
    <xsl:param name="displayPlane"></xsl:param>
    <xsl:param name="drawingPriority"></xsl:param>

    <xsl:param name="capStyle"></xsl:param>
    <xsl:param name="joinStyle"></xsl:param>
    <xsl:param name="intervalLength"></xsl:param>
    <xsl:param name="offset"></xsl:param>
    <xsl:param name="penWidth"></xsl:param>
    <xsl:param name="colorToken"></xsl:param>
    <xsl:param name="trasparency"></xsl:param>    

    <xsl:choose>
      <xsl:when test="$featureReference!=''">
        <xsl:element name="lineInstruction">
          <xsl:element name="featureReference">
            <xsl:value-of select="$featureReference"/>
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
            <xsl:element name="intervalLength">
              <xsl:value-of select="$intervalLength"/>
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
      </xsl:when>
    </xsl:choose>
  </xsl:template>
</xsl:transform>