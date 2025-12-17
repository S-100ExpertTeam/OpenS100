<?xml version="1.0" encoding="UTF-8"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="UTF-8" indent="yes"/>

  <!-- <xsl:template match="Topmark[@primitive='Point']" priority="1">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="WNDMIL12"/>
    </pointInstruction>
  </xsl:template>
  
  <xsl:template match="Topmark[@primitive='Point'and installationType=1]" priority="2">
    <xsl:call-template name="FixedTopmark">
      <xsl:with-param name="featureReference" select="@id"/>
      <xsl:with-param name="viewingGroup">27010</xsl:with-param>
      <xsl:with-param name="displayPlane">OVERRADAR</xsl:with-param>
      <xsl:with-param name="drawingPriority">24</xsl:with-param>
    </xsl:call-template>
  </xsl:template> -->

  <xsl:template match="Topmark[@primitive='Point']" priority="1">
    <xsl:call-template name="FloatingTopmark">
      <xsl:with-param name="featureReference" select="@id"/>
      <xsl:with-param name="viewingGroup">27010</xsl:with-param>
      <xsl:with-param name="displayPlane">OVERRADAR</xsl:with-param>
      <xsl:with-param name="drawingPriority">24</xsl:with-param>
    </xsl:call-template>
  </xsl:template>

</xsl:transform>

<!--

<?xml version="1.0" encoding="UTF-8"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="UTF-8" indent="yes"/>

  <xsl:template match="Topmark[@primitive='Point']" priority="1">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="WNDMIL12"/>
    </pointInstruction>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and installationType=1]" priority="2">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TMARDEF1"/>
    </pointInstruction>
  </xsl:template>


  <xsl:template match="Topmark[@primitive='Point' and installationType=1 and topmarkDaymarkShape=1]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR22"/>
    </pointInstruction>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and installationType=1 and topmarkDaymarkShape=2]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR24"/>
    </pointInstruction>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and installationType=1 and topmarkDaymarkShape=3]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR30"/>
    </pointInstruction>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and installationType=1 and topmarkDaymarkShape=4]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR32"/>
    </pointInstruction>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and installationType=1 and topmarkDaymarkShape=5]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR33"/>
    </pointInstruction>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and installationType=1 and topmarkDaymarkShape=6]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR34"/>
    </pointInstruction>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and installationType=1 and topmarkDaymarkShape=7]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR85"/>
    </pointInstruction>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and installationType=1 and topmarkDaymarkShape=8]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR86"/>
    </pointInstruction>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and installationType=1 and topmarkDaymarkShape=9]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR36"/>
    </pointInstruction>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and installationType=1 and topmarkDaymarkShape=10]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR28"/>
    </pointInstruction>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and installationType=1 and topmarkDaymarkShape=11]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR27"/>
    </pointInstruction>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and installationType=1 and topmarkDaymarkShape=12]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR14"/>
    </pointInstruction>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and installationType=1 and topmarkDaymarkShape=13]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR25"/>
    </pointInstruction>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and installationType=1 and topmarkDaymarkShape=14]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR26"/>
    </pointInstruction>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and installationType=1 and topmarkDaymarkShape=15]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR88"/>
    </pointInstruction>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and installationType=1 and topmarkDaymarkShape=16]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR87"/>
    </pointInstruction>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and installationType=1 and topmarkDaymarkShape=17]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TMARDEF1"/>
    </pointInstruction>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and installationType=1 and topmarkDaymarkShape=18]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR30"/>
    </pointInstruction>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and installationType=1 and topmarkDaymarkShape=19]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR33"/>
    </pointInstruction>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and installationType=1 and topmarkDaymarkShape=20]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR34"/>
    </pointInstruction>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and installationType=1 and topmarkDaymarkShape=21]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR33"/>
    </pointInstruction>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and installationType=1 and topmarkDaymarkShape=22]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR34"/>
    </pointInstruction>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and installationType=1 and topmarkDaymarkShape=23]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR34"/>
    </pointInstruction>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and installationType=1 and topmarkDaymarkShape=24]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR22"/>
    </pointInstruction>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and installationType=1 and topmarkDaymarkShape=25]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR24"/>
    </pointInstruction>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and installationType=1 and topmarkDaymarkShape=26]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR30"/>
    </pointInstruction>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and installationType=1 and topmarkDaymarkShape=27]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR86"/>
    </pointInstruction>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and installationType=1 and topmarkDaymarkShape=28]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR89"/>
    </pointInstruction>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and installationType=1 and topmarkDaymarkShape=29]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR22"/>
    </pointInstruction>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and installationType=1 and topmarkDaymarkShape=30]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR86"/>
    </pointInstruction>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and installationType=1 and topmarkDaymarkShape=31]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR14"/>
    </pointInstruction>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and installationType=1 and topmarkDaymarkShape=32]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR30"/>
    </pointInstruction>
  </xsl:template>


  <xsl:template match="Topmark[@primitive='Point' and installationType=2]" priority="2">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TMARDEF1"/>
    </pointInstruction>
  </xsl:template>


  <xsl:template match="Topmark[@primitive='Point' and installationType=2 and topmarkDaymarkShape=1]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR02"/>
    </pointInstruction>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and installationType=2 and topmarkDaymarkShape=2]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR04"/>
    </pointInstruction>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and installationType=2 and topmarkDaymarkShape=3]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR10"/>
    </pointInstruction>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and installationType=2 and topmarkDaymarkShape=4]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR12"/>
    </pointInstruction>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and installationType=2 and topmarkDaymarkShape=5]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR13"/>
    </pointInstruction>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and installationType=2 and topmarkDaymarkShape=6]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR14"/>
    </pointInstruction>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and installationType=2 and topmarkDaymarkShape=7]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR65"/>
    </pointInstruction>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and installationType=2 and topmarkDaymarkShape=8]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR17"/>
    </pointInstruction>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and installationType=2 and topmarkDaymarkShape=9]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR16"/>
    </pointInstruction>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and installationType=2 and topmarkDaymarkShape=10]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR08"/>
    </pointInstruction>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and installationType=2 and topmarkDaymarkShape=11]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR07"/>
    </pointInstruction>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and installationType=2 and topmarkDaymarkShape=12]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR14"/>
    </pointInstruction>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and installationType=2 and topmarkDaymarkShape=13]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR05"/>
    </pointInstruction>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and installationType=2 and topmarkDaymarkShape=14]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR06"/>
    </pointInstruction>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and installationType=2 and topmarkDaymarkShape=18]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR10"/>
    </pointInstruction>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and installationType=2 and topmarkDaymarkShape=19]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR13"/>
    </pointInstruction>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and installationType=2 and topmarkDaymarkShape=20]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR14"/>
    </pointInstruction>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and installationType=2 and topmarkDaymarkShape=21]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR13"/>
    </pointInstruction>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and installationType=2 and topmarkDaymarkShape=22]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR14"/>
    </pointInstruction>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and installationType=2 and topmarkDaymarkShape=23]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR14"/>
    </pointInstruction>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and installationType=2 and topmarkDaymarkShape=24]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR02"/>
    </pointInstruction>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and installationType=2 and topmarkDaymarkShape=25]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR04"/>
    </pointInstruction>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and installationType=2 and topmarkDaymarkShape=26]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR10"/>
    </pointInstruction>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and installationType=2 and topmarkDaymarkShape=27]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR17"/>
    </pointInstruction>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and installationType=2 and topmarkDaymarkShape=28]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR18"/>
    </pointInstruction>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and installationType=2 and topmarkDaymarkShape=29]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR02"/>
    </pointInstruction>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and installationType=2 and topmarkDaymarkShape=30]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR17"/>
    </pointInstruction>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and installationType=2 and topmarkDaymarkShape=31]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR14"/>
    </pointInstruction>
  </xsl:template>

  <xsl:template match="Topmark[@primitive='Point' and installationType=2 and topmarkDaymarkShape=32]" priority="3">
    <pointInstruction>
      <featureReference>
        <xsl:value-of select="@id"/>
      </featureReference>
      <viewingGroup>32220</viewingGroup>
      <displayPlane>OVERRADAR</displayPlane>
      <drawingPriority>12</drawingPriority>
      <symbol reference="TOPMAR10"/>
    </pointInstruction>
  </xsl:template>


</xsl:transform>

-->