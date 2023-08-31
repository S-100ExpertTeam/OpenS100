<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:ice="http://www.iho.int/ice"
                xmlns:gml="http://www.opengis.net/gml/3.2" version="1.0">
    <xsl:import href="../areaSimpleColorFillTemplate.xsl"/>
    <xsl:import href="../areaSimpleSymbolFillTemplate.xsl"/>
    <xsl:template name="seaiceWmoIceactTemplate" match="ice:seaice">
        
        <xsl:variable name="iceact" select="ice:seaice/ice:iceact"/>
        <xsl:variable name="id" select="ice:seaice/@gml:id"/>
        <xsl:variable name="viewingGroup" select="'IceStandardViewingGroup'"/>
        <xsl:variable name="drawingPriority" select="'1'"/>
        <xsl:variable name="displayPlane" select="'IceStandardDisplayPlane'"/>
        
        <xsl:choose>
            <xsl:when test="number($iceact)=1">
                <xsl:call-template name="areaSimpleColorFillTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="colorToken" select="'000 100 255'"/>
                    <xsl:with-param name="transparency" select="'0.5'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($iceact)=2">
                <xsl:call-template name="areaSimpleColorFillTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="colorToken" select="'150 200 255'"/>
                    <xsl:with-param name="transparency" select="'0.5'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($iceact)=3">
                <xsl:call-template name="areaSimpleColorFillTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="colorToken" select="'150 200 255'"/>
                    <xsl:with-param name="transparency" select="'0.5'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($iceact)=10">
                <xsl:call-template name="areaSimpleColorFillTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="colorToken" select="'140 255 160'"/>
                    <xsl:with-param name="transparency" select="'0.5'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($iceact)=12">
                <xsl:call-template name="areaSimpleColorFillTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="colorToken" select="'140 255 160'"/>
                    <xsl:with-param name="transparency" select="'0.5'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($iceact)=13">
                <xsl:call-template name="areaSimpleColorFillTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="colorToken" select="'140 255 160'"/>
                    <xsl:with-param name="transparency" select="'0.5'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($iceact)=20">
                <xsl:call-template name="areaSimpleColorFillTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="colorToken" select="'140 255 160'"/>
                    <xsl:with-param name="transparency" select="'0.5'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($iceact)=23">
                <xsl:call-template name="areaSimpleColorFillTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="colorToken" select="'140 255 160'"/>
                    <xsl:with-param name="transparency" select="'0.5'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($iceact)=24">
                <xsl:call-template name="areaSimpleColorFillTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="colorToken" select="'255 255 000'"/>
                    <xsl:with-param name="transparency" select="'0.5'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($iceact)=30">
                <xsl:call-template name="areaSimpleColorFillTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="colorToken" select="'140 255 160'"/>
                    <xsl:with-param name="transparency" select="'0.5'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($iceact)=34">
                <xsl:call-template name="areaSimpleColorFillTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="colorToken" select="'255 255 000'"/>
                    <xsl:with-param name="transparency" select="'0.5'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($iceact)=35">
                <xsl:call-template name="areaSimpleColorFillTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="colorToken" select="'255 255 000'"/>
                    <xsl:with-param name="transparency" select="'0.5'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($iceact)=40">
                <xsl:call-template name="areaSimpleColorFillTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="colorToken" select="'255 255 000'"/>
                    <xsl:with-param name="transparency" select="'0.5'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($iceact)=45">
                <xsl:call-template name="areaSimpleColorFillTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="colorToken" select="'255 255 000'"/>
                    <xsl:with-param name="transparency" select="'0.5'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($iceact)=46">
                <xsl:call-template name="areaSimpleColorFillTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="colorToken" select="'255 255 000'"/>
                    <xsl:with-param name="transparency" select="'0.5'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($iceact)=50">
                <xsl:call-template name="areaSimpleColorFillTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="colorToken" select="'255 255 000'"/>
                    <xsl:with-param name="transparency" select="'0.5'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($iceact)=56">
                <xsl:call-template name="areaSimpleColorFillTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="colorToken" select="'255 255 000'"/>
                    <xsl:with-param name="transparency" select="'0.5'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($iceact)=57">
                <xsl:call-template name="areaSimpleColorFillTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="colorToken" select="'255-125-007'"/>
                    <xsl:with-param name="transparency" select="'0.5'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($iceact)=60">
                <xsl:call-template name="areaSimpleColorFillTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="colorToken" select="'255 255 000'"/>
                    <xsl:with-param name="transparency" select="'0.5'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($iceact)=67">
                <xsl:call-template name="areaSimpleColorFillTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="colorToken" select="'255-125-007'"/>
                    <xsl:with-param name="transparency" select="'0.5'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($iceact)=68">
                <xsl:call-template name="areaSimpleColorFillTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="colorToken" select="'255-125-007'"/>
                    <xsl:with-param name="transparency" select="'0.5'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($iceact)=70">
                <xsl:call-template name="areaSimpleColorFillTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="colorToken" select="'255-125-007'"/>
                    <xsl:with-param name="transparency" select="'0.5'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($iceact)=78">
                <xsl:call-template name="areaSimpleColorFillTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="colorToken" select="'255-125-007'"/>
                    <xsl:with-param name="transparency" select="'0.5'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($iceact)=79">
                <xsl:call-template name="areaSimpleColorFillTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="colorToken" select="'255 000 000'"/>
                    <xsl:with-param name="transparency" select="'0.5'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($iceact)=80">
                <xsl:call-template name="areaSimpleColorFillTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="colorToken" select="'255-125-007'"/>
                    <xsl:with-param name="transparency" select="'0.5'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($iceact)=81">
                <xsl:call-template name="areaSimpleColorFillTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="colorToken" select="'255 000 000'"/>
                    <xsl:with-param name="transparency" select="'0.5'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($iceact)=89">
                <xsl:call-template name="areaSimpleColorFillTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="colorToken" select="'255 000 000'"/>
                    <xsl:with-param name="transparency" select="'0.5'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($iceact)=90">
                <xsl:call-template name="areaSimpleColorFillTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="colorToken" select="'255 000 000'"/>
                    <xsl:with-param name="transparency" select="'0.5'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($iceact)=91">	
                <xsl:call-template name="areaSimpleColorFillTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="colorToken" select="'255 000 000'"/>
                    <xsl:with-param name="transparency" select="'0.5'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($iceact)=92">
                <xsl:call-template name="areaSimpleColorFillTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="colorToken" select="'145 000 000'"/>
                    <xsl:with-param name="transparency" select="'0.5'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($iceact)=99">
                <xsl:call-template name="areaSimpleSymbolFillTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="areaCRS" select="'Global'"/>
                    <xsl:with-param name="symbolReference" select="'../../Symbols/iceact_unknown.svg'"/>
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