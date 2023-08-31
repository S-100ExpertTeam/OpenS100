<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:ice="http://www.iho.int/ice"
                xmlns:gml="http://www.opengis.net/gml/3.2" version="1.0">
    <xsl:import href="../lineComplexStyleTemplate.xsl"/>
    <xsl:template name="i_cracTemplate" match="ice:i_crac">
        <xsl:call-template name="lineComplexStyleTemplate">
            <xsl:with-param name="featureReference">
                <xsl:attribute name="reference">
                    <xsl:value-of select="ice:i_crac/@gml:id"/>
                </xsl:attribute>
            </xsl:with-param>
            <xsl:with-param name="viewingGroup" select="'IceStandardViewingGroup'"/>
            <xsl:with-param name="displayPlane" select="'IceStandardDisplayPlane'"/>
            <xsl:with-param name="drawingPriority" select="'1'"/>
            <xsl:with-param name="symbolReference" select="'../../LineStyles/i_crac.svg'"/>
            <xsl:with-param name="scaleFactor" select="'1.0'"/>
        </xsl:call-template>
    </xsl:template>
</xsl:stylesheet>