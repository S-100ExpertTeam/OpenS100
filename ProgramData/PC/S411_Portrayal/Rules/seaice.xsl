<?xml version="1.0" encoding="iso-8859-1"?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
	<xsl:output method="xml" encoding="UTF-8" indent="yes"/>

	<xsl:template match="SEAICE[@primitive='Surface' and ICEACT=1]" priority="2">
		<areaInstruction>
			<featureReference>
				<xsl:value-of select="@id"/>
			</featureReference>
			<viewingGroup>15010</viewingGroup>
			<displayPlane>OVERRADAR</displayPlane>
			<drawingPriority>15</drawingPriority>
			<colorFill>
				<color transparency="0.50">ICEACT01</color>
			</colorFill>
		</areaInstruction>
	</xsl:template>
	
	<xsl:template match="SEAICE[@primitive='Surface' and ICEACT=2]" priority="2">
		<areaInstruction>
			<featureReference>
				<xsl:value-of select="@id"/>
			</featureReference>
			<viewingGroup>15010</viewingGroup>
			<displayPlane>OVERRADAR</displayPlane>
			<drawingPriority>15</drawingPriority>
			<colorFill>
				<color transparency="0.50">ICEACT02</color>
			</colorFill>
		</areaInstruction>
	</xsl:template>

	<xsl:template match="SEAICE[@primitive='Surface' and ICEACT=3]" priority="2">
		<areaInstruction>
			<featureReference>
				<xsl:value-of select="@id"/>
			</featureReference>
			<viewingGroup>15010</viewingGroup>
			<displayPlane>OVERRADAR</displayPlane>
			<drawingPriority>15</drawingPriority>
			<colorFill>
				<color transparency="0.50">ICEACT03</color>
			</colorFill>
		</areaInstruction>
	</xsl:template>

	<xsl:template match="SEAICE[@primitive='Surface' and ICEACT=10]" priority="2">
		<areaInstruction>
			<featureReference>
				<xsl:value-of select="@id"/>
			</featureReference>
			<viewingGroup>15010</viewingGroup>
			<displayPlane>OVERRADAR</displayPlane>
			<drawingPriority>15</drawingPriority>
			<colorFill>
				<color transparency="0.50">ICEACT10</color>
			</colorFill>
		</areaInstruction>
	</xsl:template>

	<xsl:template match="SEAICE[@primitive='Surface' and ICEACT=12]" priority="2">
		<areaInstruction>
			<featureReference>
				<xsl:value-of select="@id"/>
			</featureReference>
			<viewingGroup>15010</viewingGroup>
			<displayPlane>OVERRADAR</displayPlane>
			<drawingPriority>15</drawingPriority>
			<colorFill>
				<color transparency="0.50">ICEACT12</color>
			</colorFill>
		</areaInstruction>
	</xsl:template>

	<xsl:template match="SEAICE[@primitive='Surface' and ICEACT=13]" priority="2">
		<areaInstruction>
			<featureReference>
				<xsl:value-of select="@id"/>
			</featureReference>
			<viewingGroup>15010</viewingGroup>
			<displayPlane>OVERRADAR</displayPlane>
			<drawingPriority>15</drawingPriority>
			<colorFill>
				<color transparency="0.50">ICEACT13</color>
			</colorFill>
		</areaInstruction>
	</xsl:template>

	<xsl:template match="SEAICE[@primitive='Surface' and ICEACT=20]" priority="2">
		<areaInstruction>
			<featureReference>
				<xsl:value-of select="@id"/>
			</featureReference>
			<viewingGroup>15010</viewingGroup>
			<displayPlane>OVERRADAR</displayPlane>
			<drawingPriority>15</drawingPriority>
			<colorFill>
				<color transparency="0.50">ICEACT20</color>
			</colorFill>
		</areaInstruction>
	</xsl:template>

	<xsl:template match="SEAICE[@primitive='Surface' and ICEACT=23]" priority="2">
		<areaInstruction>
			<featureReference>
				<xsl:value-of select="@id"/>
			</featureReference>
			<viewingGroup>15010</viewingGroup>
			<displayPlane>OVERRADAR</displayPlane>
			<drawingPriority>15</drawingPriority>
			<colorFill>
				<color transparency="0.50">ICEACT23</color>
			</colorFill>
		</areaInstruction>
	</xsl:template>

	<xsl:template match="SEAICE[@primitive='Surface' and ICEACT=24]" priority="2">
		<areaInstruction>
			<featureReference>
				<xsl:value-of select="@id"/>
			</featureReference>
			<viewingGroup>15010</viewingGroup>
			<displayPlane>OVERRADAR</displayPlane>
			<drawingPriority>15</drawingPriority>
			<colorFill>
				<color transparency="0.50">ICEACT24</color>
			</colorFill>
		</areaInstruction>
	</xsl:template>

	<xsl:template match="SEAICE[@primitive='Surface' and ICEACT=30]" priority="2">
		<areaInstruction>
			<featureReference>
				<xsl:value-of select="@id"/>
			</featureReference>
			<viewingGroup>15010</viewingGroup>
			<displayPlane>OVERRADAR</displayPlane>
			<drawingPriority>15</drawingPriority>
			<colorFill>
				<color transparency="0.50">ICEACT30</color>
			</colorFill>
		</areaInstruction>
	</xsl:template>

	<xsl:template match="SEAICE[@primitive='Surface' and ICEACT=34]" priority="2">
		<areaInstruction>
			<featureReference>
				<xsl:value-of select="@id"/>
			</featureReference>
			<viewingGroup>15010</viewingGroup>
			<displayPlane>OVERRADAR</displayPlane>
			<drawingPriority>15</drawingPriority>
			<colorFill>
				<color transparency="0.50">ICEACT34</color>
			</colorFill>
		</areaInstruction>
	</xsl:template>

	<xsl:template match="SEAICE[@primitive='Surface' and ICEACT=35]" priority="2">
		<areaInstruction>
			<featureReference>
				<xsl:value-of select="@id"/>
			</featureReference>
			<viewingGroup>15010</viewingGroup>
			<displayPlane>OVERRADAR</displayPlane>
			<drawingPriority>15</drawingPriority>
			<colorFill>
				<color transparency="0.50">ICEACT35</color>
			</colorFill>
		</areaInstruction>
	</xsl:template>

	<xsl:template match="SEAICE[@primitive='Surface' and ICEACT=40]" priority="2">
		<areaInstruction>
			<featureReference>
				<xsl:value-of select="@id"/>
			</featureReference>
			<viewingGroup>15010</viewingGroup>
			<displayPlane>OVERRADAR</displayPlane>
			<drawingPriority>15</drawingPriority>
			<colorFill>
				<color transparency="0.50">ICEACT40</color>
			</colorFill>
		</areaInstruction>
	</xsl:template>

	<xsl:template match="SEAICE[@primitive='Surface' and ICEACT=45]" priority="2">
		<areaInstruction>
			<featureReference>
				<xsl:value-of select="@id"/>
			</featureReference>
			<viewingGroup>15010</viewingGroup>
			<displayPlane>OVERRADAR</displayPlane>
			<drawingPriority>15</drawingPriority>
			<colorFill>
				<color transparency="0.50">ICEACT45</color>
			</colorFill>
		</areaInstruction>
	</xsl:template>

	<xsl:template match="SEAICE[@primitive='Surface' and ICEACT=46]" priority="2">
		<areaInstruction>
			<featureReference>
				<xsl:value-of select="@id"/>
			</featureReference>
			<viewingGroup>15010</viewingGroup>
			<displayPlane>OVERRADAR</displayPlane>
			<drawingPriority>15</drawingPriority>
			<colorFill>
				<color transparency="0.50">ICEACT46</color>
			</colorFill>
		</areaInstruction>
	</xsl:template>

	<xsl:template match="SEAICE[@primitive='Surface' and ICEACT=50]" priority="2">
		<areaInstruction>
			<featureReference>
				<xsl:value-of select="@id"/>
			</featureReference>
			<viewingGroup>15010</viewingGroup>
			<displayPlane>OVERRADAR</displayPlane>
			<drawingPriority>15</drawingPriority>
			<colorFill>
				<color transparency="0.50">ICEACT50</color>
			</colorFill>
		</areaInstruction>
	</xsl:template>

	<xsl:template match="SEAICE[@primitive='Surface' and ICEACT=56]" priority="2">
		<areaInstruction>
			<featureReference>
				<xsl:value-of select="@id"/>
			</featureReference>
			<viewingGroup>15010</viewingGroup>
			<displayPlane>OVERRADAR</displayPlane>
			<drawingPriority>15</drawingPriority>
			<colorFill>
				<color transparency="0.50">ICEACT56</color>
			</colorFill>
		</areaInstruction>
	</xsl:template>

	<xsl:template match="SEAICE[@primitive='Surface' and ICEACT=57]" priority="2">
		<areaInstruction>
			<featureReference>
				<xsl:value-of select="@id"/>
			</featureReference>
			<viewingGroup>15010</viewingGroup>
			<displayPlane>OVERRADAR</displayPlane>
			<drawingPriority>15</drawingPriority>
			<colorFill>
				<color transparency="0.50">ICEACT57</color>
			</colorFill>
		</areaInstruction>
	</xsl:template>

	<xsl:template match="SEAICE[@primitive='Surface' and ICEACT=60]" priority="2">
		<areaInstruction>
			<featureReference>
				<xsl:value-of select="@id"/>
			</featureReference>
			<viewingGroup>15010</viewingGroup>
			<displayPlane>OVERRADAR</displayPlane>
			<drawingPriority>15</drawingPriority>
			<colorFill>
				<color transparency="0.50">ICEACT60</color>
			</colorFill>
		</areaInstruction>
	</xsl:template>

	<xsl:template match="SEAICE[@primitive='Surface' and ICEACT=67]" priority="2">
		<areaInstruction>
			<featureReference>
				<xsl:value-of select="@id"/>
			</featureReference>
			<viewingGroup>15010</viewingGroup>
			<displayPlane>OVERRADAR</displayPlane>
			<drawingPriority>15</drawingPriority>
			<colorFill>
				<color transparency="0.50">ICEACT67</color>
			</colorFill>
		</areaInstruction>
	</xsl:template>

	<xsl:template match="SEAICE[@primitive='Surface' and ICEACT=68]" priority="2">
		<areaInstruction>
			<featureReference>
				<xsl:value-of select="@id"/>
			</featureReference>
			<viewingGroup>15010</viewingGroup>
			<displayPlane>OVERRADAR</displayPlane>
			<drawingPriority>15</drawingPriority>
			<colorFill>
				<color transparency="0.50">ICEACT68</color>
			</colorFill>
		</areaInstruction>
	</xsl:template>

	<xsl:template match="SEAICE[@primitive='Surface' and ICEACT=70]" priority="2">
		<areaInstruction>
			<featureReference>
				<xsl:value-of select="@id"/>
			</featureReference>
			<viewingGroup>15010</viewingGroup>
			<displayPlane>OVERRADAR</displayPlane>
			<drawingPriority>15</drawingPriority>
			<colorFill>
				<color transparency="0.50">ICEACT70</color>
			</colorFill>
		</areaInstruction>
	</xsl:template>

	<xsl:template match="SEAICE[@primitive='Surface' and ICEACT=78]" priority="2">
		<areaInstruction>
			<featureReference>
				<xsl:value-of select="@id"/>
			</featureReference>
			<viewingGroup>15010</viewingGroup>
			<displayPlane>OVERRADAR</displayPlane>
			<drawingPriority>15</drawingPriority>
			<colorFill>
				<color transparency="0.50">ICEACT78</color>
			</colorFill>
		</areaInstruction>
	</xsl:template>

	<xsl:template match="SEAICE[@primitive='Surface' and ICEACT=79]" priority="2">
		<areaInstruction>
			<featureReference>
				<xsl:value-of select="@id"/>
			</featureReference>
			<viewingGroup>15010</viewingGroup>
			<displayPlane>OVERRADAR</displayPlane>
			<drawingPriority>15</drawingPriority>
			<colorFill>
				<color transparency="0.50">ICEACT79</color>
			</colorFill>
		</areaInstruction>
	</xsl:template>

	<xsl:template match="SEAICE[@primitive='Surface' and ICEACT=80]" priority="2">
		<areaInstruction>
			<featureReference>
				<xsl:value-of select="@id"/>
			</featureReference>
			<viewingGroup>15010</viewingGroup>
			<displayPlane>OVERRADAR</displayPlane>
			<drawingPriority>15</drawingPriority>
			<colorFill>
				<color transparency="0.50">ICEACT80</color>
			</colorFill>
		</areaInstruction>
	</xsl:template>

	<xsl:template match="SEAICE[@primitive='Surface' and ICEACT=81]" priority="2">
		<areaInstruction>
			<featureReference>
				<xsl:value-of select="@id"/>
			</featureReference>
			<viewingGroup>15010</viewingGroup>
			<displayPlane>OVERRADAR</displayPlane>
			<drawingPriority>15</drawingPriority>
			<colorFill>
				<color transparency="0.50">ICEACT81</color>
			</colorFill>
		</areaInstruction>
	</xsl:template>

	<xsl:template match="SEAICE[@primitive='Surface' and ICEACT=89]" priority="2">
		<areaInstruction>
			<featureReference>
				<xsl:value-of select="@id"/>
			</featureReference>
			<viewingGroup>15010</viewingGroup>
			<displayPlane>OVERRADAR</displayPlane>
			<drawingPriority>15</drawingPriority>
			<colorFill>
				<color transparency="0.50">ICEACT89</color>
			</colorFill>
		</areaInstruction>
	</xsl:template>

	<xsl:template match="SEAICE[@primitive='Surface' and ICEACT=90]" priority="2">
		<areaInstruction>
			<featureReference>
				<xsl:value-of select="@id"/>
			</featureReference>
			<viewingGroup>15010</viewingGroup>
			<displayPlane>OVERRADAR</displayPlane>
			<drawingPriority>15</drawingPriority>
			<colorFill>
				<color transparency="0.50">ICEACT90</color>
			</colorFill>
		</areaInstruction>
	</xsl:template>

	<xsl:template match="SEAICE[@primitive='Surface' and ICEACT=91]" priority="2">
		<areaInstruction>
			<featureReference>
				<xsl:value-of select="@id"/>
			</featureReference>
			<viewingGroup>15010</viewingGroup>
			<displayPlane>OVERRADAR</displayPlane>
			<drawingPriority>15</drawingPriority>
			<colorFill>
				<color transparency="0.50">ICEACT91</color>
			</colorFill>
		</areaInstruction>
	</xsl:template>

	<xsl:template match="SEAICE[@primitive='Surface' and ICEACT=92]" priority="2">
		<areaInstruction>
			<featureReference>
				<xsl:value-of select="@id"/>
			</featureReference>
			<viewingGroup>15010</viewingGroup>
			<displayPlane>OVERRADAR</displayPlane>
			<drawingPriority>15</drawingPriority>
			<colorFill>
				<color transparency="0.50">ICEACT92</color>
			</colorFill>
		</areaInstruction>
	</xsl:template>

	<xsl:template match="SEAICE[@primitive='Surface' and ICEACT=99]" priority="2">
		<pointInstruction>
			<featureReference>
				<xsl:value-of select="@id"/>
			</featureReference>
			<viewingGroup>12210</viewingGroup>
			<displayPlane>UNDERRADAR</displayPlane>
			<drawingPriority>15</drawingPriority>
			<symbol reference="iceact_unknown"/>
		</pointInstruction>
		<areaInstruction>
			<featureReference>
				<xsl:value-of select="@id"/>
			</featureReference>
			<viewingGroup>15010</viewingGroup>
			<displayPlane>OVERRADAR</displayPlane>
			<drawingPriority>15</drawingPriority>
			<colorFill>
				<color transparency="0">ICEACT99</color>
			</colorFill>
		</areaInstruction>
	</xsl:template>

</xsl:transform>
