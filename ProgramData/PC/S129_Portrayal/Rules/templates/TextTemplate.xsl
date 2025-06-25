<?xml version="1.0" encoding="utf-8" ?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output indent="yes" version="1.0"/>

  <xsl:template name="textTemplate">
    <xsl:param name="featureReference"></xsl:param>
    <xsl:param name="viewingGroup"></xsl:param>
    <xsl:param name="displayPlane"></xsl:param>
    <xsl:param name="drawingPriority"></xsl:param>

    <xsl:param name="text"></xsl:param>
    <xsl:param name="verticalAlignment"></xsl:param>
    <xsl:param name="horizontalAlignment"></xsl:param>
    <xsl:param name="bodySize"></xsl:param>
    
    <xsl:param name="token_f"></xsl:param>
    <xsl:param name="transparency_f"></xsl:param>
    <xsl:param name="token_b"></xsl:param>
    <xsl:param name="transparency_b"></xsl:param>
    <xsl:param name="serifs"></xsl:param>
    <xsl:param name="weight"></xsl:param>
    <xsl:param name="slant"></xsl:param>
    <xsl:param name="proportion"></xsl:param>
    <xsl:param name="offsetX"></xsl:param>
    <xsl:param name="offsetY"></xsl:param>
    <xsl:param name="rotation"></xsl:param>

    <xsl:choose>
      <xsl:when test="$featureReference!=''">
        <xsl:element name="textInstruction">
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
          <xsl:element name="textPoint" >
            <xsl:attribute name="verticalAlignment">
              <xsl:value-of select="$verticalAlignment"/>
            </xsl:attribute>
            <xsl:attribute name="horizontalAlignment">
              <xsl:value-of select="$horizontalAlignment"/>
            </xsl:attribute>
              <xsl:element name="element">
                <xsl:element name="text">
                  <xsl:value-of select="$text"/>
                </xsl:element>
                <xsl:element name="bodySize">
                  <xsl:value-of select="$bodySize"/>
                </xsl:element>
                
                <xsl:element name="fontCharacteristics">
                  <xsl:element name="serifs">
                    <xsl:value-of select="$serifs"/>
                  </xsl:element>
                  <xsl:element name="weight">
                    <xsl:value-of select="$weight"/>
                  </xsl:element>
                  <xsl:element name="slant">
                    <xsl:value-of select="$slant"/>
                  </xsl:element>
                  <xsl:element name="proportion">
                    <xsl:value-of select="$proportion"/>
                  </xsl:element>
                </xsl:element>
                
                <xsl:element name="foreground">
                  <xsl:element name="token">
                    <xsl:value-of select="$token_f"/>
                  </xsl:element>
                  <xsl:element name="transparency">
                    <xsl:value-of select="$transparency_f"/>
                  </xsl:element>
                </xsl:element>
                <xsl:element name="background">
                  <xsl:element name="token">
                    <xsl:value-of select="$token_b"/>
                  </xsl:element>
                  <xsl:element name="transparency">
                    <xsl:value-of select="$transparency_b"/>
                  </xsl:element>
                </xsl:element>               
              </xsl:element>
              <xsl:element name="offset">
              <xsl:element name="x">
                <xsl:value-of select="$offsetX"/>
              </xsl:element>
              <xsl:element name="y">
                <xsl:value-of select="$offsetY"/>
              </xsl:element>
            </xsl:element>
              <xsl:element name="rotation">
                <xsl:value-of select="$rotation"/>
              </xsl:element>
              
            </xsl:element>        
            
          </xsl:element>

        
      </xsl:when>
    </xsl:choose>

  </xsl:template>  
</xsl:transform>