<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
    <xsl:template name="lineSimpleStyleTemplate">
        <xsl:param name="featureReference"/> <!-- id of feature that instruction applies to -->
        <xsl:param name="viewingGroup"/>
        <xsl:param name="displayPlane"/>
        <xsl:param name="drawingPriority"/>
        <xsl:param name="capStyle" select="'Butt'"/>
        <xsl:param name="joinStyle" select="'Miter'"/>
        <xsl:param name="intervalLength"/>
        <xsl:param name="offset" select="'0.0'"/>
        <xsl:param name="penWidth" select="'1.0'"/>
        <xsl:param name="colorToken" select="'128 128 128'"/>
        <xsl:param name="transparency" select="'0.0'"/>
        <xsl:param name="dashStart"/>
        <xsl:param name="dashLength"/>
        
        <!-- do some validation then output the point instruction -->
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
                    <xsl:element name="simpleLineStyle">
                        <xsl:element name="capStyle">
                            <xsl:value-of select="$capStyle"/>
                        </xsl:element>
                        <xsl:element name="joinStyle">
                            <xsl:value-of select="$joinStyle"/>
                        </xsl:element>
                        <xsl:element name="intervalLength">
                            <xsl:value-of select="$intervalLength"/>
                        </xsl:element>
                        <xsl:element name="offset">
                            <xsl:value-of select="$offset"/>
                        </xsl:element>
                        <xsl:element name="pen">
                            <xsl:attribute name="width">
                                <xsl:value-of select="$penWidth"/>
                            </xsl:attribute>
                            <xsl:element name="color">
                                <xsl:element name="token">
                                    <xsl:value-of select="$colorToken"/>
                                </xsl:element>
                                <xsl:element name="transparency">
                                    <xsl:value-of select="$transparency"/>
                                </xsl:element>
                            </xsl:element>
                        </xsl:element>
                        <xsl:element name="dash">
                            <xsl:element name="start"></xsl:element>
                            <xsl:element name="length"></xsl:element>
                        </xsl:element>
                    </xsl:element>
                </xsl:element>
            </xsl:when>         
            <xsl:otherwise>
                <xsl:message terminate="yes"> Error: featureReference and fill color token not defined! </xsl:message>
            </xsl:otherwise>
        </xsl:choose> 
    </xsl:template>   
</xsl:stylesheet>