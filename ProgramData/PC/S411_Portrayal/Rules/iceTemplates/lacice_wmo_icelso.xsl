<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
    xmlns:ice="http://www.iho.int/ice"
    xmlns:gml="http://www.opengis.net/gml/3.2" version="1.0">
    <xsl:import href="../areaSimpleColorFillTemplate.xsl"/>
    <xsl:import href="../areaSimpleSymbolFillTemplate.xsl"/>
    <xsl:template name="laciceWmoIcelsoTemplate" match="ice:lacice">
        <xsl:variable name="icelso" select="ice:lacice/ice:icelso"/>
        <xsl:variable name="id" select="ice:lacice/@gml:id"/>
        <xsl:variable name="viewingGroup" select="'IceStandardViewingGroup'"/>
        <xsl:variable name="drawingPriority" select="'1'"/>
        <xsl:variable name="displayPlane" select="'IceStandardDisplayPlane'"/>
        
        <xsl:choose>
            <xsl:when test="number($icelso)=1">
                <xsl:call-template name="areaSimpleColorFillTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="colorToken" select="'150 200 255'"/>
                    <xsl:with-param name="transparency" select="'0.5'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($icelso)=2">
                <xsl:call-template name="areaSimpleColorFillTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="colorToken" select="'240 210 250'"/>
                    <xsl:with-param name="transparency" select="'0.5'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($icelso)=3">
                <xsl:call-template name="areaSimpleColorFillTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="colorToken" select="'255 100 255'"/>
                    <xsl:with-param name="transparency" select="'0.5'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($icelso)=4">
                <xsl:call-template name="areaSimpleColorFillTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="colorToken" select="'170 040 240'"/>
                    <xsl:with-param name="transparency" select="'0.5'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($icelso)=5">
                <xsl:call-template name="areaSimpleColorFillTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="colorToken" select="'135 060 215'"/>
                    <xsl:with-param name="transparency" select="'0.5'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($icelso)=70">
                <xsl:call-template name="areaSimpleColorFillTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="colorToken" select="'150 200 255'"/>
                    <xsl:with-param name="transparency" select="'0.5'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($icelso)=99">
                <xsl:call-template name="areaSimpleSymbolFillTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="areaCRS" select="'Global'"/>
                    <xsl:with-param name="symbolReference" select="'../../Symbols/icesod_unknown.svg'"/>
                    <xsl:with-param name="rotation" select="'0.0'"/>
                    <xsl:with-param name="rotationCRS" select="'PortrayalCRS'"/>
                    <xsl:with-param name="scaleFactor" select="'1.0'"/>
                    <xsl:with-param name="offset1_x" select="'10.0'"/>
                    <xsl:with-param name="offset1_y" select="'10.0'"/>
                    <xsl:with-param name="offset2_x" select="'0.0'"/>
                    <xsl:with-param name="offset2_y" select="'0.0'"/>
                </xsl:call-template>
            </xsl:when>
        </xsl:choose>
    </xsl:template>
</xsl:stylesheet>