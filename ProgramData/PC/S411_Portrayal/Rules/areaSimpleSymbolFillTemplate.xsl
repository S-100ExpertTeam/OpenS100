<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
    <!-- template to output a symbol polygon fill instruction -->
    <!-- for polygon outline use line instruction with feature reference to polygon feature -->
    <xsl:template name="areaSimpleSymbolFillTemplate">
        <xsl:param name="featureReference"/>
        <!-- id of feature that instruction applies to -->
        <xsl:param name="viewingGroup"/>
        <xsl:param name="displayPlane"/>
        <xsl:param name="drawingPriority"/>
        <xsl:param name="areaCRS"/>
        <xsl:param name="symbolReference"/>
        <xsl:param name="rotation"/>
        <xsl:param name="rotationCRS"/>
        <xsl:param name="scaleFactor"/>
        <xsl:param name="offset1_x"/>
        <xsl:param name="offset1_y"/>
        <xsl:param name="offset2_x"/>
        <xsl:param name="offset2_y"/>
        
        <!-- do some validation then output the point instruction -->
        <xsl:choose>
            <xsl:when test="$featureReference != '' and $symbolReference != ''  ">
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
                    <xsl:element name="symbolFill">
                        <xsl:element name="areaCRS">
                            <xsl:value-of select="$areaCRS"/>
                        </xsl:element>
                        <xsl:element name="symbol">
                            <xsl:element name="symbolReference">
                                <xsl:value-of select="$symbolReference"/>
                            </xsl:element>
                            <xsl:element name="rotation">
                                <xsl:value-of select="$rotation"/>
                            </xsl:element>
                            <xsl:element name="rotationCRS">
                                <xsl:value-of select="$rotationCRS"/>
                            </xsl:element>
                            <xsl:element name="scaleFactor">
                                <xsl:value-of select="$scaleFactor"/>
                            </xsl:element>
                        </xsl:element>
                        <xsl:element name="v1">
                            <xsl:element name="x">
                                <xsl:value-of select="$offset1_x"/>
                            </xsl:element>
                            <xsl:element name="y">
                                <xsl:value-of select="$offset1_y"/>
                            </xsl:element>
                        </xsl:element>
                        <xsl:element name="v2">
                            <xsl:element name="x">
                                <xsl:value-of select="$offset2_x"/>
                            </xsl:element>
                            <xsl:element name="y">
                                <xsl:value-of select="$offset2_y"/>
                            </xsl:element>
                        </xsl:element>
                    </xsl:element>
                </xsl:element>
            </xsl:when>
            
            <xsl:otherwise>
                <xsl:message terminate="yes"> Error: featureReference and fill color token not
                    defined! </xsl:message>
            </xsl:otherwise>
        </xsl:choose>
        
    </xsl:template>
    
</xsl:stylesheet>