<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:ice="http://www.iho.int/ice"
                xmlns:gml="http://www.opengis.net/gml/3.2" version="1.0">
    <xsl:import href="../pointSimpleSymbolTemplate.xsl"/>
    <xsl:template name="icebrgTemplate" match="ice:icebrg">
        <xsl:variable name="icebsz" select="ice:icebrg/ice:icebsz"/>
        <xsl:variable name="id" select="ice:icebrg/@gml:id"/>
        <xsl:variable name="viewingGroup" select="'IceStandardViewingGroup'"/>
        <xsl:variable name="drawingPriority" select="'1'"/>
        <xsl:variable name="displayPlane" select="'IceStandardDisplayPlane'"/>
        
        <xsl:choose>
            <xsl:when test="number($icebsz)=1">
                <xsl:call-template name="pointSimpleSymbolTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="symbolReference" select="'../../Symbols/icebrg_growler.svg'"/>
                    <xsl:with-param name="rotation" select="'0.0'"/>
                    <xsl:with-param name="scaleFactor" select="'1.0'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($icebsz)=2">
                <xsl:call-template name="pointSimpleSymbolTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="symbolReference" select="'../../Symbols/icebrg_bergy_bit.svg'"/>
                    <xsl:with-param name="rotation" select="'0.0'"/>
                    <xsl:with-param name="scaleFactor" select="'1.0'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($icebsz)=3">
                <xsl:call-template name="pointSimpleSymbolTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="symbolReference" select="'../../Symbols/icebrg_small.svg'"/>
                    <xsl:with-param name="rotation" select="'0.0'"/>
                    <xsl:with-param name="scaleFactor" select="'1.0'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($icebsz)=4">
                <xsl:call-template name="pointSimpleSymbolTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="symbolReference" select="'../../Symbols/icebrg_medium.svg'"/>
                    <xsl:with-param name="rotation" select="'0.0'"/>
                    <xsl:with-param name="scaleFactor" select="'1.0'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($icebsz)=5">
                <xsl:call-template name="pointSimpleSymbolTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="symbolReference" select="'../../Symbols/icebrg_large.svg'"/>
                    <xsl:with-param name="rotation" select="'0.0'"/>
                    <xsl:with-param name="scaleFactor" select="'1.0'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($icebsz)=6">
                <xsl:call-template name="pointSimpleSymbolTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="symbolReference" select="'../../Symbols/icebrg_very_large.svg'"/>
                    <xsl:with-param name="rotation" select="'0.0'"/>
                    <xsl:with-param name="scaleFactor" select="'1.0'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($icebsz)=7">
                <xsl:call-template name="pointSimpleSymbolTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="symbolReference" select="'../../Symbols/icebrg_ice_island_fragment.svg'"/>
                    <xsl:with-param name="rotation" select="'0.0'"/>
                    <xsl:with-param name="scaleFactor" select="'1.0'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($icebsz)=8">
                <xsl:call-template name="pointSimpleSymbolTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="symbolReference" select="'../../Symbols/icebrg_ice_island.svg'"/>
                    <xsl:with-param name="rotation" select="'0.0'"/>
                    <xsl:with-param name="scaleFactor" select="'1.0'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($icebsz)=9">
                <xsl:call-template name="pointSimpleSymbolTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="symbolReference" select="'../../Symbols/icebrg_radar_target.svg'"/>
                    <xsl:with-param name="rotation" select="'0.0'"/>
                    <xsl:with-param name="scaleFactor" select="'1.0'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($icebsz)=99">
                <xsl:call-template name="pointSimpleSymbolTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="symbolReference" select="'../../Symbols/icebrg_unknown.svg'"/>
                    <xsl:with-param name="rotation" select="'0.0'"/>
                    <xsl:with-param name="scaleFactor" select="'1.0'"/>
                </xsl:call-template>
            </xsl:when>
        </xsl:choose>
    </xsl:template>
</xsl:stylesheet>