<?xml version="1.0" encoding="utf-8" ?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:import href="./templates/TextTemplate.xsl"/>  
  
  <xsl:template match="UnderKeelClearanceControlPointInformationBox">

    <xsl:variable name="temp">
      <xsl:value-of select="name"/>
      <xsl:text>&#10;ETA:&#x20;&#x20;&#x20;</xsl:text>
      <xsl:value-of select="substring(ETA, 9, 2)"/>
      <xsl:text>/</xsl:text>
      <xsl:value-of select="substring(ETA, 12, 2)"/>
      <xsl:value-of select="substring(ETA, 15, 2)"/>
      <xsl:text>&#10;Open:&#x20;&#x20;</xsl:text>
      <xsl:value-of select="substring(TimeWindowOpen, 9, 2)"/>
      <xsl:text>/</xsl:text>
      <xsl:value-of select="substring(TimeWindowOpen, 12, 2)"/>
      <xsl:value-of select="substring(TimeWindowOpen, 15, 2)"/>
      <xsl:text>&#10;Close:&#x20;</xsl:text>
      <xsl:value-of select="substring(TimeWindowClose, 9, 2)"/>
      <xsl:text>/</xsl:text>
      <xsl:value-of select="substring(TimeWindowClose, 12, 2)"/>
      <xsl:value-of select="substring(TimeWindowClose, 15, 2)"/>
    </xsl:variable> 
    
    <xsl:call-template name="textTemplate">
      <xsl:with-param name="featureReference" select="@id"/>
      <xsl:with-param name="viewingGroup" select="33021"/>
      <xsl:with-param name="displayPlane" select="'UnderRadar'"/>
      <xsl:with-param name="drawingPriority" select="8"/>

      <xsl:with-param name="text" select="$temp"/>       
      <xsl:with-param name="bodySize" select="10"/>

      <xsl:with-param name="verticalAlignment" select="'Top'"/>
      <xsl:with-param name="horizontalAlignment" select="'Start'"/>
      
      <xsl:with-param name="token_f" select="'BLK'"/>
      <xsl:with-param name="transparency_f" select="1"/>
      <xsl:with-param name="token_b" select="'WHITE'"/>
      <xsl:with-param name="transparency_b" select="1"/>      
      <xsl:with-param name="serifs" select="0"/>
      <xsl:with-param name="weight" select="'Light'"/>
      <xsl:with-param name="slant" select="'Upright'"/>
      <xsl:with-param name="proportion" select="'MonoSpaced'"/>
      <xsl:with-param name="offsetX" select="30"/>
      <xsl:with-param name="offsetY" select="20"/>
      <xsl:with-param name="rotation" select="0"/>
    </xsl:call-template>
  </xsl:template>
</xsl:transform>