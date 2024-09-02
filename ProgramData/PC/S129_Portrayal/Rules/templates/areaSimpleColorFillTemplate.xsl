<?xml version="1.0" encoding="utf-8" ?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output indent="yes" version="1.0"/>
  
  <xsl:template name="AreaSimpleColorFillTemplate">
    <xsl:param name="featureReference"/>
    <xsl:param name="viewingGroup"/>
    <xsl:param name="displayPlane"/>
    <xsl:param name="drawingPriority"/>
    <xsl:param name="colorToken"/>
    <xsl:param name="transparency"/>
    <xsl:param name="areaFillReference"/>

    <xsl:choose>
      <xsl:when test="$featureReference!=''">
        <xsl:element name="areaInstruction">
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
		  <xsl:choose>
			  <xsl:when test="$colorToken != ''">
				  <xsl:element name="colorFill">
					<xsl:element name="color">
					  <xsl:attribute name="transparency">
						<xsl:value-of select="$transparency"/>
					  </xsl:attribute>
					  <xsl:value-of select="$colorToken"/>
					</xsl:element>
				  </xsl:element>
			  </xsl:when>
			  <xsl:otherwise>
				  <xsl:element name="areaFillReference">
					<xsl:attribute name="reference">
					  <xsl:value-of select="$areaFillReference"/>
					</xsl:attribute>
				  </xsl:element>
			  </xsl:otherwise>
		  </xsl:choose>
        </xsl:element>
      </xsl:when>
    </xsl:choose>
  </xsl:template>
</xsl:transform>
