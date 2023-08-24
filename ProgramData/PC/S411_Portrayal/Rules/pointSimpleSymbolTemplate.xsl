<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
    <!-- template to output a simple point symbol instruction -->
    <xsl:template name="pointSimpleSymbolTemplate">
        <xsl:param name="featureReference"/> <!--id of feature that instruction applies to -->
        <xsl:param name="viewingGroup"/>
        <xsl:param name="displayPlane"/>
        <xsl:param name="drawingPriority"/>
        <xsl:param name="symbolReference"/>
        <xsl:param name="rotation"/>
        <xsl:param name="scaleFactor"/>
        
        <!-- Do some validation then output the point instruction -->
        <xsl:choose>
            <xsl:when test="$featureReference != '' and $symbolReference != ''">
                <xsl:element name="pointInstruction">
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
                    <xsl:element name="drawingPriority"><xsl:value-of select="$drawingPriority"/></xsl:element>
                    <xsl:element name="symbol">
                        <xsl:element name="symbolReference"><xsl:value-of select="$symbolReference"/></xsl:element>
                        <xsl:element name="rotation"><xsl:value-of select="$rotation"/></xsl:element>
                        <xsl:element name="scaleFactor"><xsl:value-of select="$scaleFactor"/></xsl:element>
                    </xsl:element>
                </xsl:element>
            </xsl:when>
            <xsl:otherwise>
                <xsl:message terminate="yes">
                    Error: featureReference and symbol reference not defined!
                </xsl:message>
            </xsl:otherwise>
        </xsl:choose>
        
    </xsl:template>
</xsl:stylesheet>