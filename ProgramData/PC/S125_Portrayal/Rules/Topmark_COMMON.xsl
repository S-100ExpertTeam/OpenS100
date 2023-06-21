<?xml version="1.0" encoding="UTF-8"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="UTF-8" indent="yes"/>

  <xsl:template match="Topmark[@primitive='Point']" priority="2">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TMARDEF1"/>
    </pointInstruction>
    <xsl:call-template name="AtoNStatusInformation">
      <xsl:with-param name="fid">
         <xsl:value-of select="@id"/>
      </xsl:with-param>
  </xsl:call-template>
  </xsl:template>


  <xsl:template match="Topmark[@primitive='Point' and topmarkDaymarkShape=1]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR22"/>
    </pointInstruction>
    <xsl:call-template name="AtoNStatusInformation">
      <xsl:with-param name="fid">
         <xsl:value-of select="@id"/>
      </xsl:with-param>
  </xsl:call-template>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and topmarkDaymarkShape=2]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR24"/>
    </pointInstruction>
    <xsl:call-template name="AtoNStatusInformation">
      <xsl:with-param name="fid">
         <xsl:value-of select="@id"/>
      </xsl:with-param>
  </xsl:call-template>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and topmarkDaymarkShape=3]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR30"/>
    </pointInstruction>
    <xsl:call-template name="AtoNStatusInformation">
      <xsl:with-param name="fid">
         <xsl:value-of select="@id"/>
      </xsl:with-param>
  </xsl:call-template>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and topmarkDaymarkShape=4]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR32"/>
    </pointInstruction>
    <xsl:call-template name="AtoNStatusInformation">
      <xsl:with-param name="fid">
         <xsl:value-of select="@id"/>
      </xsl:with-param>
  </xsl:call-template>
  <xsl:call-template name="AtoNStatusInformation">
    <xsl:with-param name="fid">
       <xsl:value-of select="@id"/>
    </xsl:with-param>
</xsl:call-template>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and topmarkDaymarkShape=5]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR33"/>
    </pointInstruction>
    <xsl:call-template name="AtoNStatusInformation">
      <xsl:with-param name="fid">
         <xsl:value-of select="@id"/>
      </xsl:with-param>
  </xsl:call-template>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and topmarkDaymarkShape=6]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR34"/>
    </pointInstruction>
    <xsl:call-template name="AtoNStatusInformation">
      <xsl:with-param name="fid">
         <xsl:value-of select="@id"/>
      </xsl:with-param>
  </xsl:call-template>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and topmarkDaymarkShape=7]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR85"/>
    </pointInstruction>
    <xsl:call-template name="AtoNStatusInformation">
      <xsl:with-param name="fid">
         <xsl:value-of select="@id"/>
      </xsl:with-param>
  </xsl:call-template>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and topmarkDaymarkShape=8]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR86"/>
    </pointInstruction>
    <xsl:call-template name="AtoNStatusInformation">
      <xsl:with-param name="fid">
         <xsl:value-of select="@id"/>
      </xsl:with-param>
  </xsl:call-template>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and topmarkDaymarkShape=9]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR36"/>
    </pointInstruction>
    <xsl:call-template name="AtoNStatusInformation">
      <xsl:with-param name="fid">
         <xsl:value-of select="@id"/>
      </xsl:with-param>
  </xsl:call-template>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and topmarkDaymarkShape=10]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR28"/>
    </pointInstruction>
    <xsl:call-template name="AtoNStatusInformation">
      <xsl:with-param name="fid">
         <xsl:value-of select="@id"/>
      </xsl:with-param>
  </xsl:call-template>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and topmarkDaymarkShape=11]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR27"/>
    </pointInstruction>
    <xsl:call-template name="AtoNStatusInformation">
      <xsl:with-param name="fid">
         <xsl:value-of select="@id"/>
      </xsl:with-param>
  </xsl:call-template>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and topmarkDaymarkShape=12]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR14"/>
    </pointInstruction>
          <xsl:call-template name="AtoNStatusInformation">
         <xsl:with-param name="fid">
            <xsl:value-of select="@id"/>
         </xsl:with-param>
     </xsl:call-template>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and topmarkDaymarkShape=13]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR25"/>
    </pointInstruction>
    <xsl:call-template name="AtoNStatusInformation">
      <xsl:with-param name="fid">
         <xsl:value-of select="@id"/>
      </xsl:with-param>
  </xsl:call-template>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and topmarkDaymarkShape=14]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR26"/>
    </pointInstruction>
    <xsl:call-template name="AtoNStatusInformation">
      <xsl:with-param name="fid">
         <xsl:value-of select="@id"/>
      </xsl:with-param>
  </xsl:call-template>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and topmarkDaymarkShape=15]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR88"/>
    </pointInstruction>
    <xsl:call-template name="AtoNStatusInformation">
      <xsl:with-param name="fid">
         <xsl:value-of select="@id"/>
      </xsl:with-param>
  </xsl:call-template>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and topmarkDaymarkShape=16]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR87"/>
    </pointInstruction>
    <xsl:call-template name="AtoNStatusInformation">
      <xsl:with-param name="fid">
         <xsl:value-of select="@id"/>
      </xsl:with-param>
  </xsl:call-template>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and topmarkDaymarkShape=17]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TMARDEF1"/>
    </pointInstruction>
   <xsl:call-template name="AtoNStatusInformation">
      <xsl:with-param name="fid">
         <xsl:value-of select="@id"/>
      </xsl:with-param>
  </xsl:call-template>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and topmarkDaymarkShape=18]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR30"/>
    </pointInstruction>
    <xsl:call-template name="AtoNStatusInformation">
      <xsl:with-param name="fid">
         <xsl:value-of select="@id"/>
      </xsl:with-param>
  </xsl:call-template>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and topmarkDaymarkShape=19]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR33"/>
    </pointInstruction>
    <xsl:call-template name="AtoNStatusInformation">
      <xsl:with-param name="fid">
         <xsl:value-of select="@id"/>
      </xsl:with-param>
  </xsl:call-template>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and topmarkDaymarkShape=20]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR34"/>
    </pointInstruction>
    <xsl:call-template name="AtoNStatusInformation">
      <xsl:with-param name="fid">
         <xsl:value-of select="@id"/>
      </xsl:with-param>
  </xsl:call-template>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and topmarkDaymarkShape=21]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR33"/>
    </pointInstruction>
    <xsl:call-template name="AtoNStatusInformation">
      <xsl:with-param name="fid">
         <xsl:value-of select="@id"/>
      </xsl:with-param>
  </xsl:call-template>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and topmarkDaymarkShape=22]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR34"/>
    </pointInstruction>
    <xsl:call-template name="AtoNStatusInformation">
      <xsl:with-param name="fid">
         <xsl:value-of select="@id"/>
      </xsl:with-param>
  </xsl:call-template>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and topmarkDaymarkShape=23]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR34"/>
    </pointInstruction>
    <xsl:call-template name="AtoNStatusInformation">
      <xsl:with-param name="fid">
         <xsl:value-of select="@id"/>
      </xsl:with-param>
  </xsl:call-template>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and topmarkDaymarkShape=24]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR22"/>
    </pointInstruction>
    <xsl:call-template name="AtoNStatusInformation">
      <xsl:with-param name="fid">
         <xsl:value-of select="@id"/>
      </xsl:with-param>
  </xsl:call-template>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and topmarkDaymarkShape=25]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR24"/>
    </pointInstruction>
    <xsl:call-template name="AtoNStatusInformation">
      <xsl:with-param name="fid">
         <xsl:value-of select="@id"/>
      </xsl:with-param>
  </xsl:call-template>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and topmarkDaymarkShape=26]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR30"/>
    </pointInstruction>
    <xsl:call-template name="AtoNStatusInformation">
      <xsl:with-param name="fid">
         <xsl:value-of select="@id"/>
      </xsl:with-param>
  </xsl:call-template>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and topmarkDaymarkShape=27]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR86"/>
    </pointInstruction>
    <xsl:call-template name="AtoNStatusInformation">
      <xsl:with-param name="fid">
         <xsl:value-of select="@id"/>
      </xsl:with-param>
  </xsl:call-template>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and topmarkDaymarkShape=28]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR89"/>
    </pointInstruction>
    <xsl:call-template name="AtoNStatusInformation">
      <xsl:with-param name="fid">
         <xsl:value-of select="@id"/>
      </xsl:with-param>
  </xsl:call-template>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and topmarkDaymarkShape=29]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR22"/>
    </pointInstruction>
    <xsl:call-template name="AtoNStatusInformation">
      <xsl:with-param name="fid">
         <xsl:value-of select="@id"/>
      </xsl:with-param>
  </xsl:call-template>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and topmarkDaymarkShape=30]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR86"/>
    </pointInstruction>
    <xsl:call-template name="AtoNStatusInformation">
      <xsl:with-param name="fid">
         <xsl:value-of select="@id"/>
      </xsl:with-param>
  </xsl:call-template>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and topmarkDaymarkShape=31]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR14"/>
    </pointInstruction>
    <xsl:call-template name="AtoNStatusInformation">
      <xsl:with-param name="fid">
         <xsl:value-of select="@id"/>
      </xsl:with-param>
  </xsl:call-template>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and topmarkDaymarkShape=32]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR30"/>
    </pointInstruction>
    <xsl:call-template name="AtoNStatusInformation">
      <xsl:with-param name="fid">
         <xsl:value-of select="@id"/>
      </xsl:with-param>
  </xsl:call-template>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and topmarkDaymarkShape=1]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR02"/>
    </pointInstruction>
    <xsl:call-template name="AtoNStatusInformation">
      <xsl:with-param name="fid">
         <xsl:value-of select="@id"/>
      </xsl:with-param>
  </xsl:call-template>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and topmarkDaymarkShape=2]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR04"/>
    </pointInstruction>
    <xsl:call-template name="AtoNStatusInformation">
      <xsl:with-param name="fid">
         <xsl:value-of select="@id"/>
      </xsl:with-param>
  </xsl:call-template>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and topmarkDaymarkShape=3]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR10"/>
    </pointInstruction>
    <xsl:call-template name="AtoNStatusInformation">
      <xsl:with-param name="fid">
         <xsl:value-of select="@id"/>
      </xsl:with-param>
  </xsl:call-template>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and topmarkDaymarkShape=4]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR12"/>
    </pointInstruction>
    <xsl:call-template name="AtoNStatusInformation">
      <xsl:with-param name="fid">
         <xsl:value-of select="@id"/>
      </xsl:with-param>
  </xsl:call-template>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and topmarkDaymarkShape=5]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR13"/>
    </pointInstruction>
    <xsl:call-template name="AtoNStatusInformation">
      <xsl:with-param name="fid">
         <xsl:value-of select="@id"/>
      </xsl:with-param>
  </xsl:call-template>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and topmarkDaymarkShape=6]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR14"/>
    </pointInstruction>
    <xsl:call-template name="AtoNStatusInformation">
      <xsl:with-param name="fid">
         <xsl:value-of select="@id"/>
      </xsl:with-param>
  </xsl:call-template>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and topmarkDaymarkShape=7]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR65"/>
    </pointInstruction>
    <xsl:call-template name="AtoNStatusInformation">
      <xsl:with-param name="fid">
         <xsl:value-of select="@id"/>
      </xsl:with-param>
  </xsl:call-template>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and topmarkDaymarkShape=8]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR17"/>
    </pointInstruction>
    <xsl:call-template name="AtoNStatusInformation">
      <xsl:with-param name="fid">
         <xsl:value-of select="@id"/>
      </xsl:with-param>
  </xsl:call-template>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and topmarkDaymarkShape=9]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR16"/>
    </pointInstruction>
    <xsl:call-template name="AtoNStatusInformation">
      <xsl:with-param name="fid">
         <xsl:value-of select="@id"/>
      </xsl:with-param>
  </xsl:call-template>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and topmarkDaymarkShape=10]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR08"/>
    </pointInstruction>
    <xsl:call-template name="AtoNStatusInformation">
      <xsl:with-param name="fid">
         <xsl:value-of select="@id"/>
      </xsl:with-param>
  </xsl:call-template>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and topmarkDaymarkShape=11]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR07"/>
    </pointInstruction>
    <xsl:call-template name="AtoNStatusInformation">
      <xsl:with-param name="fid">
         <xsl:value-of select="@id"/>
      </xsl:with-param>
  </xsl:call-template>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and topmarkDaymarkShape=12]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR14"/>
    </pointInstruction>
    <xsl:call-template name="AtoNStatusInformation">
      <xsl:with-param name="fid">
         <xsl:value-of select="@id"/>
      </xsl:with-param>
  </xsl:call-template>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and topmarkDaymarkShape=13]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR05"/>
    </pointInstruction>
    <xsl:call-template name="AtoNStatusInformation">
      <xsl:with-param name="fid">
         <xsl:value-of select="@id"/>
      </xsl:with-param>
  </xsl:call-template>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and topmarkDaymarkShape=14]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR06"/>
    </pointInstruction>
    <xsl:call-template name="AtoNStatusInformation">
      <xsl:with-param name="fid">
         <xsl:value-of select="@id"/>
      </xsl:with-param>
  </xsl:call-template>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and topmarkDaymarkShape=18]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR10"/>
    </pointInstruction>
    <xsl:call-template name="AtoNStatusInformation">
      <xsl:with-param name="fid">
         <xsl:value-of select="@id"/>
      </xsl:with-param>
  </xsl:call-template>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and topmarkDaymarkShape=19]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR13"/>
    </pointInstruction>
    <xsl:call-template name="AtoNStatusInformation">
      <xsl:with-param name="fid">
         <xsl:value-of select="@id"/>
      </xsl:with-param>
  </xsl:call-template>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and topmarkDaymarkShape=20]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR14"/>
    </pointInstruction>
    <xsl:call-template name="AtoNStatusInformation">
      <xsl:with-param name="fid">
         <xsl:value-of select="@id"/>
      </xsl:with-param>
  </xsl:call-template>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and topmarkDaymarkShape=21]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR13"/>
    </pointInstruction>
    <xsl:call-template name="AtoNStatusInformation">
      <xsl:with-param name="fid">
         <xsl:value-of select="@id"/>
      </xsl:with-param>
  </xsl:call-template>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and topmarkDaymarkShape=22]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR14"/>
    </pointInstruction>
    <xsl:call-template name="AtoNStatusInformation">
      <xsl:with-param name="fid">
         <xsl:value-of select="@id"/>
      </xsl:with-param>
  </xsl:call-template>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and topmarkDaymarkShape=23]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR14"/>
    </pointInstruction>
    <xsl:call-template name="AtoNStatusInformation">
      <xsl:with-param name="fid">
         <xsl:value-of select="@id"/>
      </xsl:with-param>
  </xsl:call-template>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and topmarkDaymarkShape=24]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR02"/>
    </pointInstruction>
    <xsl:call-template name="AtoNStatusInformation">
      <xsl:with-param name="fid">
         <xsl:value-of select="@id"/>
      </xsl:with-param>
  </xsl:call-template>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and topmarkDaymarkShape=25]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR04"/>
    </pointInstruction>
    <xsl:call-template name="AtoNStatusInformation">
      <xsl:with-param name="fid">
         <xsl:value-of select="@id"/>
      </xsl:with-param>
  </xsl:call-template>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and topmarkDaymarkShape=26]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR10"/>
    </pointInstruction>
    <xsl:call-template name="AtoNStatusInformation">
      <xsl:with-param name="fid">
         <xsl:value-of select="@id"/>
      </xsl:with-param>
  </xsl:call-template>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and topmarkDaymarkShape=27]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR17"/>
    </pointInstruction>
    <xsl:call-template name="AtoNStatusInformation">
      <xsl:with-param name="fid">
         <xsl:value-of select="@id"/>
      </xsl:with-param>
  </xsl:call-template>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and topmarkDaymarkShape=28]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR18"/>
    </pointInstruction>
    <xsl:call-template name="AtoNStatusInformation">
      <xsl:with-param name="fid">
         <xsl:value-of select="@id"/>
      </xsl:with-param>
  </xsl:call-template>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and topmarkDaymarkShape=29]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR02"/>
    </pointInstruction>
    <xsl:call-template name="AtoNStatusInformation">
      <xsl:with-param name="fid">
         <xsl:value-of select="@id"/>
      </xsl:with-param>
  </xsl:call-template>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and topmarkDaymarkShape=30]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR17"/>
    </pointInstruction>
    <xsl:call-template name="AtoNStatusInformation">
      <xsl:with-param name="fid">
         <xsl:value-of select="@id"/>
      </xsl:with-param>
  </xsl:call-template>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and topmarkDaymarkShape=31]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR14"/>
    </pointInstruction>
    <xsl:call-template name="AtoNStatusInformation">
      <xsl:with-param name="fid">
         <xsl:value-of select="@id"/>
      </xsl:with-param>
  </xsl:call-template>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and topmarkDaymarkShape=32]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR10"/>
    </pointInstruction>
    <xsl:call-template name="AtoNStatusInformation">
      <xsl:with-param name="fid">
         <xsl:value-of select="@id"/>
      </xsl:with-param>
  </xsl:call-template>
  </xsl:template>

</xsl:transform>