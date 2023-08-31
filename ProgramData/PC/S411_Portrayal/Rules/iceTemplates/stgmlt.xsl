<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:ice="http://www.iho.int/ice"
                xmlns:gml="http://www.opengis.net/gml/3.2" version="1.0">
    <xsl:import href="../pointSimpleSymbolTemplate.xsl"/>
    <xsl:import href="../pointSimpleLabelTemplate.xsl"/>
    <xsl:template name="stgmltTemplate" match="ice:stgmlt">
        <xsl:variable name="icemlt" select="ice:stgmlt/ice:icemlt"/>
        <xsl:variable name="id" select="ice:stgmlt/@gml:id"/>
        <xsl:variable name="viewingGroup" select="'IceStandardViewingGroup'"/>
        <xsl:variable name="drawingPriority" select="'1'"/>
        <xsl:variable name="displayPlane" select="'IceStandardDisplayPlane'"/>
        
        <xsl:call-template name="pointSimpleSymbolTemplate">
            <xsl:with-param name="featureReference" select="$id"/>
            <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
            <xsl:with-param name="displayPlane" select="$displayPlane"/>
            <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
            <xsl:with-param name="symbolReference" select="'../../Symbols/stgmlt.svg'"/>
            <xsl:with-param name="rotation" select="'0.0'"/>
            <xsl:with-param name="scaleFactor" select="'1.0'"/>
        </xsl:call-template>
        
        <xsl:call-template name="pointSimpleLabelTemplate">
            <xsl:with-param name="featureReference" select="$id"/>
            <xsl:with-param name="viewingGroup" select="$viewingGroup"/>
            <xsl:with-param name="displayPlane" select="$displayPlane"/>
            <xsl:with-param name="drawingPriority" select="$drawingPriority"/>
            <xsl:with-param name="text" select="$icemlt"/>
            <xsl:with-param name="horizontalAlignment" select="'Center'"/>
            <xsl:with-param name="verticalAlignment" select="'Center'"/>
            <xsl:with-param name="bodySize" select="'3.0'"/>
            <!--<xsl:with-param name="verticalOffset"/>-->
            <!--<xsl:with-param name="textColor"/>
            <xsl:with-param name="textTransparency"/>
            <xsl:with-param name="fontSerifs"/>
            <xsl:with-param name="fontWeight"/>
            <xsl:with-param name="fontSlant"/>
            <xsl:with-param name="fontProportion"/>
            <xsl:with-param name="rotation"/>-->
        </xsl:call-template>
    </xsl:template>
</xsl:stylesheet>