<?xml version="1.0" encoding="UTF-8"?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
    <xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes"/>
    <xsl:template name="AtoNStatusInformation">
        <xsl:param name = "fid" />
        <xsl:for-each select="atonStatusInformation">
            <xsl:variable name="atonStatusID" select="@informationRef"/>
            <xsl:for-each select="/Dataset/InformationTypes/AtoNStatusInformation[@id=$atonStatusID]">
            <xsl:choose>
                <xsl:when test="changeTypes=1">
                    <pointInstruction>
                        <featureReference>
                            <xsl:copy-of select="$fid"/>
                        </featureReference>
                        <viewingGroup>27020</viewingGroup>
                        <displayPlane>OVERRADAR</displayPlane>
                        <drawingPriority>24</drawingPriority>
                        <symbol reference="CHNGTC01"/>
                    </pointInstruction>
                </xsl:when>
                <xsl:when test="changeTypes=2">
                    <pointInstruction>
                        <featureReference>
                            <xsl:copy-of select="$fid"/>
                        </featureReference>
                        <viewingGroup>27020</viewingGroup>
                        <displayPlane>OVERRADAR</displayPlane>
                        <drawingPriority>24</drawingPriority>
                        <symbol reference="CHNGPC01"/>
                    </pointInstruction>
                </xsl:when>
                <xsl:when test="changeTypes=3">
                    <pointInstruction>
                        <featureReference>
                            <xsl:copy-of select="$fid"/>
                        </featureReference>
                        <viewingGroup>27020</viewingGroup>
                        <displayPlane>OVERRADAR</displayPlane>
                        <drawingPriority>24</drawingPriority>
                        <symbol reference="CHNGAC01"/>
                    </pointInstruction>
                </xsl:when>
                <xsl:when test="changeTypes=4">
                    <pointInstruction>
                        <featureReference>
                            <xsl:copy-of select="$fid"/>
                        </featureReference>
                        <viewingGroup>27020</viewingGroup>
                        <displayPlane>OVERRADAR</displayPlane>
                        <drawingPriority>24</drawingPriority>
                        <symbol reference="CHNGDC01"/>
                    </pointInstruction>
                </xsl:when>
            </xsl:choose>
            </xsl:for-each>
        </xsl:for-each>
    </xsl:template>
</xsl:transform>