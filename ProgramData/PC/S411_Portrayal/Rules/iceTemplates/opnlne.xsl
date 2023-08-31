<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:ice="http://www.iho.int/ice"
    xmlns:gml="http://www.opengis.net/gml/3.2" version="1.0">
    <xsl:import href="../lineSimpleStyleTemplate.xsl"/>
    <xsl:template name="opnlneTemplate" match="ice:opnlne">
        <xsl:call-template name="lineSimpleStyleTemplate">
            <xsl:with-param name="featureReference" select="ice:opnlne/@gml:id"/>
            <xsl:with-param name="viewingGroup" select="'IceStandardViewingGroup'"/>
            <xsl:with-param name="displayPlane" select="'IceStandardDisplayPlane'"/>
            <xsl:with-param name="drawingPriority" select="'1'"/>
            <xsl:with-param name="capStyle" select="'Butt'"/>
            <xsl:with-param name="joinStyle" select="'Miter'"/>
            <xsl:with-param name="offset" select="'0.0'"/>
            <xsl:with-param name="penWidth" select="'1.0'"/>
            <xsl:with-param name="colorToken" select="'255 0 0'"/>
            <xsl:with-param name="transparency" select="'0.0'"/>
        </xsl:call-template>
    </xsl:template>
</xsl:stylesheet>
