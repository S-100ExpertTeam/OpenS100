<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
    <xsl:template name="lineComplexStyleTemplate">
        <xsl:param name="featureReference"/> <!-- id of feature that instruction applies to -->
        <xsl:param name="viewingGroup"/>
        <xsl:param name="displayPlane"/>
        <xsl:param name="drawingPriority"/>
        <xsl:param name="symbolReference"/>
        <xsl:param name="scaleFactor" select="'1.0'"/>
        <xsl:param name="overrideAllColorToken"/>
        <xsl:param name="overrideAllTransparency"/>
        <xsl:param name="overrideToken"/>
        <xsl:param name="overrideTransparency"/>
        
        <xsl:choose>
            <xsl:when test="$featureReference != ''">
                <xsl:element name="lineInstruction">
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
                    <xsl:element name="complexLineStyle">
                        <xsl:element name="symbolReference">
                            <xsl:value-of select="$symbolReference"/>
                        </xsl:element>
                        <xsl:element name="scaleFactor">
                            <xsl:value-of select="$scaleFactor"/>
                        </xsl:element>
                        <xsl:element name="overrideAll">
                            <xsl:element name="token">
                                <xsl:value-of select="$overrideAllColorToken"/>
                            </xsl:element>
                            <xsl:element name="transparency">
                                <xsl:value-of select="$overrideAllTransparency"/>
                            </xsl:element>
                        </xsl:element>
                        <xsl:element name="override">
                            <xsl:element name="token">
                                <xsl:value-of select="$overrideToken"/>
                            </xsl:element>
                            <xsl:element name="transparency">
                                <xsl:value-of select="$overrideTransparency"/>
                            </xsl:element>
                        </xsl:element>
                    </xsl:element>
                </xsl:element>
            </xsl:when>
        </xsl:choose>
    </xsl:template>
</xsl:stylesheet>