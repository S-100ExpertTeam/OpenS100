<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:ice="http://www.iho.int/ice"
                xmlns:gml="http://www.opengis.net/gml/3.2" version="1.0">
    <xsl:import href="../pointSimpleSymbolTemplate.xsl"/>
    <xsl:template name="icedftTemplate" match="ice:icedft">
        <xsl:variable name="iceddr" select="ice:icedft/ice:iceddr"/>
        <xsl:variable name="id" select="ice:icedft/@gml:id"/>
        <xsl:variable name="viewingGroup" select="'IceStandardViewingGroup'"/>
        <xsl:variable name="drawingPriority" select="'1'"/>
        <xsl:variable name="displayPlane" select="'IceStandardDisplayPlane'"/>
        
        <xsl:choose>
            <xsl:when test="number($iceddr)=1">
                <xsl:call-template name="pointSimpleSymbolTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="symbolReference" select="'../../Symbols/icedft_no_ice_motion.svg'"/>
                    <xsl:with-param name="rotation" select="'0.0'"/>
                    <xsl:with-param name="scaleFactor" select="'1.0'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($iceddr)=2">
                <xsl:call-template name="pointSimpleSymbolTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="symbolReference" select="'../../Symbols/icedft_NE.svg'"/>
                    <xsl:with-param name="rotation" select="'0.0'"/>
                    <xsl:with-param name="scaleFactor" select="'1.0'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($iceddr)=3">
                <xsl:call-template name="pointSimpleSymbolTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="symbolReference" select="'../../Symbols/icedft_E.svg'"/>
                    <xsl:with-param name="rotation" select="'0.0'"/>
                    <xsl:with-param name="scaleFactor" select="'1.0'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($iceddr)=4">
                <xsl:call-template name="pointSimpleSymbolTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="symbolReference" select="'../../Symbols/icedft_SE.svg'"/>
                    <xsl:with-param name="rotation" select="'0.0'"/>
                    <xsl:with-param name="scaleFactor" select="'1.0'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($iceddr)=5">
                <xsl:call-template name="pointSimpleSymbolTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="symbolReference" select="'../../Symbols/icedft_S.svg'"/>
                    <xsl:with-param name="rotation" select="'0.0'"/>
                    <xsl:with-param name="scaleFactor" select="'1.0'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($iceddr)=6">
                <xsl:call-template name="pointSimpleSymbolTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="symbolReference" select="'../../Symbols/icedft_SW.svg'"/>
                    <xsl:with-param name="rotation" select="'0.0'"/>
                    <xsl:with-param name="scaleFactor" select="'1.0'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($iceddr)=7">
                <xsl:call-template name="pointSimpleSymbolTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="symbolReference" select="'../../Symbols/icedft_W.svg'"/>
                    <xsl:with-param name="rotation" select="'0.0'"/>
                    <xsl:with-param name="scaleFactor" select="'1.0'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($iceddr)=8">
                <xsl:call-template name="pointSimpleSymbolTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="symbolReference" select="'../../Symbols/icedft_NW.svg'"/>
                    <xsl:with-param name="rotation" select="'0.0'"/>
                    <xsl:with-param name="scaleFactor" select="'1.0'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($iceddr)=9">
                <xsl:call-template name="pointSimpleSymbolTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="symbolReference" select="'../../Symbols/icedft_N.svg'"/>
                    <xsl:with-param name="rotation" select="'0.0'"/>
                    <xsl:with-param name="scaleFactor" select="'1.0'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($iceddr)=10">
                <xsl:call-template name="pointSimpleSymbolTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="symbolReference" select="'../../Symbols/icedft_variable.svg'"/>
                    <xsl:with-param name="rotation" select="'0.0'"/>
                    <xsl:with-param name="scaleFactor" select="'1.0'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($iceddr)=99">
                <xsl:call-template name="pointSimpleSymbolTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="symbolReference" select="'../../Symbols/icedft_unknown.svg'"/>
                    <xsl:with-param name="rotation" select="'0.0'"/>
                    <xsl:with-param name="scaleFactor" select="'1.0'"/>
                </xsl:call-template>
            </xsl:when>
        </xsl:choose>
    </xsl:template>
</xsl:stylesheet>