<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
    xmlns:ice="http://www.iho.int/ice"
    xmlns:gml="http://www.opengis.net/gml/3.2" version="1.0">
    <xsl:import href="../pointSimpleSymbolTemplate.xsl"/>
    <xsl:template name="strptcTemplate" match="ice:strptc">
        <xsl:call-template name="pointSimpleSymbolTemplate">
            <xsl:with-param name="featureReference" select="ice:strptc/@gml:id" />
            <xsl:with-param name="viewingGroup" select="'IceStandardViewingGroup'"/>
            <xsl:with-param name="displayPlane" select="'IceStandardDisplayPlane'"/>
            <xsl:with-param name="drawingPriority" select="'1'"/>
            <xsl:with-param name="symbolReference" select="'../../Symbols/strptc.svg'"/>
            <xsl:with-param name="rotation" select="'0.0'"/>
            <xsl:with-param name="scaleFactor" select="'1.0'"/>
        </xsl:call-template>
    </xsl:template>
</xsl:stylesheet>