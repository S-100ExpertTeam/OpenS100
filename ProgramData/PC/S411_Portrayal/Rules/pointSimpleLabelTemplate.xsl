<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
    <xsl:template name="pointSimpleLabelTemplate">
        <xsl:param name="featureReference"/> <!--id of feature that instruction applies to -->
        <xsl:param name="viewingGroup"/>
        <xsl:param name="displayPlane"/>
        <xsl:param name="drawingPriority"/>
        <xsl:param name="text"/>
        <xsl:param name="horizontalAlignment" select="'Start'"/>
        <xsl:param name="verticalAlignment" select="'Bottom'"/>
        <xsl:param name="bodySize" select="'3.0'"/>
        <xsl:param name="verticalOffset" select="'0.0'"/>
        <xsl:param name="flag"/>
        <xsl:param name="textColor" select="'0 0 0'"/>        
        <xsl:param name="textTransparency" select="'0.0'"/>
        <xsl:param name="backgroundColor"/>
        <xsl:param name="backgroundTransparency"/>
        <xsl:param name="fontSerifs" select="'false'"/>
        <xsl:param name="fontWeight" select="'Medium'"/>
        <xsl:param name="fontSlant" select="'Upright'"/>
        <xsl:param name="fontProportion" select="'Proportional'"/>
        <xsl:param name="offset_X" select="'0.0'"/>
        <xsl:param name="offset_Y" select="'0.0'"/>
        <xsl:param name="rotation" select="'0.0'"/>
        
        <xsl:choose>
            <xsl:when test="$featureReference != ''">
                <xsl:element name="textInstruction">
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
                    <xsl:element name="displayPriority">
                        <xsl:value-of select="$drawingPriority"/>
                    </xsl:element>
                    <xsl:element name="textPoint">
                        <xsl:attribute name="horizontalAlignment">
                            <xsl:value-of select="$horizontalAlignment"/>
                        </xsl:attribute>
                        <xsl:attribute name="verticalAlignment">
                            <xsl:value-of select="$verticalAlignment"/>
                        </xsl:attribute>
                        <xsl:element name="element">
                            <xsl:element name="text">
                                <xsl:value-of select="$text"/>
                            </xsl:element>
                            <xsl:element name="bodySize">
                                <xsl:value-of select="$bodySize"/>
                            </xsl:element>
                            <xsl:element name="verticalOffset">
                                <xsl:value-of select="$verticalOffset"/>
                            </xsl:element>
                            <xsl:element name="flags">
                                <xsl:element name="flag">
                                    <xsl:value-of select="$flag"/>
                                </xsl:element>
                            </xsl:element>
                            <xsl:element name="foreground">
                                <xsl:element name="token">
                                    <xsl:value-of select="$textColor"/>
                                </xsl:element>
                                <xsl:element name="trasparency">
                                    <xsl:value-of select="$textTransparency"/>
                                </xsl:element>
                            </xsl:element>
                            <xsl:element name="background">
                                <xsl:element name="token">
                                    <xsl:value-of select="$backgroundColor"/>
                                </xsl:element>
                                <xsl:element name="transparency">
                                    <xsl:value-of select="$backgroundTransparency"/>
                                </xsl:element>
                            </xsl:element>
                            <xsl:element name="font">
                                <xsl:element name="serifs">
                                    <xsl:value-of select="$fontSerifs"/>
                                </xsl:element>
                                <xsl:element name="weight">
                                    <xsl:value-of select="$fontWeight"/>
                                </xsl:element>
                                <xsl:element name="slant">
                                    <xsl:value-of select="$fontSlant"/>
                                </xsl:element>
                                <xsl:element name="proportion">
                                    <xsl:value-of select="$fontProportion"/>
                                </xsl:element>
                            </xsl:element>
                        </xsl:element>
                        <xsl:element name="offset">
                            <xsl:element name="x">
                                <xsl:value-of select="$offset_X"/>
                            </xsl:element>
                            <xsl:element name="y">
                                <xsl:value-of select="$offset_Y"/>
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
</xsl:stylesheet>