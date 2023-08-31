<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:ice="http://www.iho.int/ice"
                xmlns:gml="http://www.opengis.net/gml/3.2" version="1.0">
    <xsl:import href="../areaSimpleColorFillTemplate.xsl"/>
    <xsl:import href="../areaSimpleSymbolFillTemplate.xsl"/>
    <xsl:template name="seaiceWmoIcesodTemplate" match="ice:seaice">
        <xsl:variable name="icesod" select="ice:seaice/ice:icesod"/>
        <xsl:variable name="id" select="ice:seaice/@gml:id"/>
        <xsl:variable name="viewingGroup" select="'IceStandardViewingGroup'"/>
        <xsl:variable name="drawingPriority" select="'1'"/>
        <xsl:variable name="displayPlane" select="'IceStandardDisplayPlane'"/>
        
        <xsl:choose>
            <xsl:when test="number($icesod)=1">
                <xsl:call-template name="areaSimpleColorFillTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="colorToken" select="'150 200 255'"/>
                    <xsl:with-param name="transparency" select="'0.5'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($icesod)=70">
                <xsl:call-template name="areaSimpleColorFillTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="colorToken" select="'150 200 255'"/>
                    <xsl:with-param name="transparency" select="'0.5'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($icesod)=80">
                <xsl:call-template name="areaSimpleColorFillTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="colorToken" select="'150 200 255'"/>
                    <xsl:with-param name="transparency" select="'0.5'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($icesod)=81">
                <xsl:call-template name="areaSimpleColorFillTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="colorToken" select="'240 210 250'"/>
                    <xsl:with-param name="transparency" select="'0.5'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($icesod)=82">
                <xsl:call-template name="areaSimpleColorFillTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="colorToken" select="'255 100 255'"/>
                    <xsl:with-param name="transparency" select="'0.5'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($icesod)=83">
                <xsl:call-template name="areaSimpleColorFillTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="colorToken" select="'170 040 240'"/>
                    <xsl:with-param name="transparency" select="'0.5'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($icesod)=84">
                <xsl:call-template name="areaSimpleColorFillTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="colorToken" select="'135 060 215'"/>
                    <xsl:with-param name="transparency" select="'0.5'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($icesod)=85">
                <xsl:call-template name="areaSimpleColorFillTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="colorToken" select="'220 080 235'"/>
                    <xsl:with-param name="transparency" select="'0.5'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($icesod)=86">
                <xsl:call-template name="areaSimpleColorFillTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="colorToken" select="'255 255 000'"/>
                    <xsl:with-param name="transparency" select="'0.5'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($icesod)=87">
                <xsl:call-template name="areaSimpleColorFillTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="colorToken" select="'155 210 000'"/>
                    <xsl:with-param name="transparency" select="'0.5'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($icesod)=88">
                <xsl:call-template name="areaSimpleColorFillTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="colorToken" select="'215 250 130'"/>
                    <xsl:with-param name="transparency" select="'0.5'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($icesod)=89">
                <xsl:call-template name="areaSimpleColorFillTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="colorToken" select="'175 250 000'"/>
                    <xsl:with-param name="transparency" select="'0.5'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($icesod)=91">
                <xsl:call-template name="areaSimpleColorFillTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="colorToken" select="'000 200 020'"/>
                    <xsl:with-param name="transparency" select="'0.5'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($icesod)=93">
                <xsl:call-template name="areaSimpleColorFillTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="colorToken" select="'000 120 000'"/>
                    <xsl:with-param name="transparency" select="'0.5'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($icesod)=94">
                <xsl:call-template name="areaSimpleColorFillTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="colorToken" select="'000 120 000'"/>
                    <xsl:with-param name="transparency" select="'0.5'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($icesod)=95">
                <xsl:call-template name="areaSimpleColorFillTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="colorToken" select="'180 100 050'"/>
                    <xsl:with-param name="transparency" select="'0.5'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($icesod)=96">
                <xsl:call-template name="areaSimpleColorFillTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="colorToken" select="'255 120 010'"/>
                    <xsl:with-param name="transparency" select="'0.5'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($icesod)=97">
                <xsl:call-template name="areaSimpleColorFillTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="colorToken" select="'200 000 000'"/>
                    <xsl:with-param name="transparency" select="'0.5'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($icesod)=98">
                <xsl:call-template name="areaSimpleSymbolFillTemplate">
                    <xsl:with-param name="featureReference" select="$id"/>
                    <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
                    <xsl:with-param name="displayPlane" select="$displayPlane"/>
                    <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
                    <xsl:with-param name="areaCRS" select="'Global'"/>
                    <xsl:with-param name="symbolReference" select="'../../Symbols/brgare.svg'"/>
                    <xsl:with-param name="rotation" select="'0.0'"/>
                    <xsl:with-param name="rotationCRS" select="'PortrayalCRS'"/>
                    <xsl:with-param name="scaleFactor" select="'1.0'"/>
                    <xsl:with-param name="offset1_x" select="'10.0'"/>
                    <xsl:with-param name="offset1_y" select="'10.0'"/>
                    <xsl:with-param name="offset2_x" select="'0.0'"/>
                    <xsl:with-param name="offset2_y" select="'0.0'"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="number($icesod)=99">
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