<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
    <!-- template to output a simple polygon fill instruction -->
    <!-- for polygon outline use line instruction with feature reference to polygon feature -->
    <xsl:template name="areaSimpleColorFillTemplate">
        <xsl:param name="featureReference"/><!-- id of feature that instruction applies to -->
        <xsl:param name="viewingGroup"/>
        <xsl:param name="displayPlane"/>
        <xsl:param name="drawingPriority"/>
        <xsl:param name="colorToken" select="'128 128 128'"/>
        <xsl:param name="transparency" select="0.0"/> 

        <!-- do some validation then output the point instruction -->
        <xsl:choose>
            <xsl:when test="$featureReference != ''">
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
                   <xsl:element name="colorFill">
                       <xsl:element name="color">
                           <xsl:element name="token">
                               <xsl:value-of select="$colorToken"/>
                           </xsl:element>
                           <xsl:element name="transparency">
                               <xsl:value-of select="$transparency"/>
                           </xsl:element>
                       </xsl:element>
                   </xsl:element>
                </xsl:element>
            </xsl:when>
            <xsl:when test="$featureReference != ''">
                
            </xsl:when>
            <xsl:otherwise>
                <xsl:message terminate="yes"> Error: featureReference and fill color token not defined!</xsl:message>
            </xsl:otherwise>
        </xsl:choose>

    </xsl:template>

</xsl:stylesheet>
