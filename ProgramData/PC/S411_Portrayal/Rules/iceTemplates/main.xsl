<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0"   xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
								xmlns:ice="http://www.iho.int/ice" 
								xmlns:gml="http://www.opengis.net/gml/3.2" 
								xmlns:s100Symbol="http://www.iho.int/S100SymbolDefinition"
								xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" 
								xsi:noNamespaceSchemaLocation="S100Presentation.xsd">
	<xsl:output method="xml"  version="1.0" encoding="UTF-8" indent="yes"/>
	
	<xsl:include href="seaice.xsl"/>
	<xsl:include href="lacice.xsl"/>
	<xsl:include href="brgare.xsl"/>
	<xsl:include href="icelne.xsl"/>
	<xsl:include href="brglne.xsl"/>
	<xsl:include href="opnlne.xsl"/>
	<xsl:include href="lkilne.xsl"/>
	<xsl:include href="i_ridg.xsl"/>
	<xsl:include href="i_lead.xsl"/>
	<xsl:include href="i_fral.xsl"/>
	<xsl:include href="i_crac.xsl"/>
	<xsl:include href="icecom.xsl"/>
	<xsl:include href="icelea.xsl"/>
	<xsl:include href="icebrg.xsl"/>
	<xsl:include href="flobrg.xsl"/>
	<xsl:include href="icethk.xsl"/>
	<xsl:include href="iceshr.xsl"/>
	<xsl:include href="icediv.xsl"/>
	<xsl:include href="icerdg.xsl"/>
	<xsl:include href="icekel.xsl"/>
	<xsl:include href="icedft.xsl"/>
	<xsl:include href="icefra.xsl"/>
	<xsl:include href="icerft.xsl"/>
	<xsl:include href="jmdbrr.xsl"/>
	<xsl:include href="stgmlt.xsl"/>
	<xsl:include href="snwcvr.xsl"/>
	<xsl:include href="strptc.xsl"/>
	<xsl:include href="i_grhm.xsl"/>
	
	<!-- xsl:param name="iceclass" select="document('s100IceParameters.xml')/parameters/iceclass"/-->
	<!-- xsl:param name="outschemapath" select="document('s100IceParameters.xml')/parameters/outschemapath"/-->
	
	
	
	<!--xsl:apply-templates select="*"/-->
	<xsl:template match="/ice:IceDataSet">
		<xsl:element name="displayList">
		<!--displayList xsi:noNamespaceSchemaLocation="S100Presentation.xsd" -->
			
				<xsl:for-each select="ice:IceFeatureMember">
					<!-- xsl:value-of select="name(child::*[1])"></xsl:value-of-->
					<xsl:variable name="elementName" select="name(*[1])"/>		
					<xsl:choose>
						<xsl:when test="$elementName = 'ice:seaice'">
							<xsl:call-template name="seaiceTemplate"/>
						</xsl:when>
						<xsl:when test="$elementName = 'ice:lacice'">
							<xsl:call-template name="laciceTemplate"/>
						</xsl:when>
						<xsl:when test="$elementName = 'ice:brgare'">
							<xsl:call-template name="brgareTemplate"/>
						</xsl:when>
						<xsl:when test="$elementName = 'ice:icelne'">
							<xsl:call-template name="icelneTemplate"/>
						</xsl:when>
						<xsl:when test="$elementName = 'ice:brglne'">
							<xsl:call-template name="brglneTemplate"/>
						</xsl:when>
						<xsl:when test="$elementName = 'ice:opnlne'">
							<xsl:call-template name="opnlneTemplate"/>
						</xsl:when>
						<xsl:when test="$elementName = 'ice:lkilne'">
							<xsl:call-template name="lkilneTemplate"/>
						</xsl:when>
						<xsl:when test="$elementName = 'ice:i_ridg'">
							<xsl:call-template name="i_ridgTemplate"/>
						</xsl:when>
						<xsl:when test="$elementName = 'ice:i_lead'">
							<xsl:call-template name="i_leadTemplate"/>
						</xsl:when>
						<xsl:when test="$elementName = 'ice:i_fral'">
							<xsl:call-template name="i_fralTemplate"/>
						</xsl:when>
						<xsl:when test="$elementName = 'ice:i_crac'">
							<xsl:call-template name="i_cracTemplate"/>
						</xsl:when>
						<xsl:when test="$elementName = 'ice:icecom'">
							<xsl:call-template name="icecomTemplate"/>
						</xsl:when>
						<xsl:when test="$elementName = 'ice:icelea'">
							<xsl:call-template name="iceleaTemplate"/>
						</xsl:when>
						<xsl:when test="$elementName = 'ice:icebrg'">
							<xsl:call-template name="icebrgTemplate"/>
						</xsl:when>
						<xsl:when test="$elementName = 'ice:flobrg'">
							<xsl:call-template name="flobrgTemplate"/>
						</xsl:when>
						<xsl:when test="$elementName = 'ice:icethk'">
							<xsl:call-template name="icethkTemplate"/>
						</xsl:when>
						<xsl:when test="$elementName = 'ice:iceshr'">
							<xsl:call-template name="iceshrTemplate"/>
						</xsl:when>
						<xsl:when test="$elementName = 'ice:icediv'">
							<xsl:call-template name="icedivTemplate"/>
						</xsl:when>
						<xsl:when test="$elementName = 'ice:icerdg'">
							<xsl:call-template name="icerdgTemplate"/>
						</xsl:when>
						<xsl:when test="$elementName = 'ice:icekel'">
							<xsl:call-template name="icekelTemplate"/>
						</xsl:when>
						<xsl:when test="$elementName = 'ice:icedft'">
							<xsl:call-template name="icedftTemplate"/>
						</xsl:when>
						<xsl:when test="$elementName = 'ice:icefra'">
							<xsl:call-template name="icefraTemplate"/>
						</xsl:when>
						<xsl:when test="$elementName = 'ice:icerft'">
							<xsl:call-template name="icerftTemplate"/>
						</xsl:when>
						<xsl:when test="$elementName = 'ice:jmdbrr'">
							<xsl:call-template name="jmdbrrTemplate"/>
						</xsl:when>
						<xsl:when test="$elementName = 'ice:stgmlt'">
							<xsl:call-template name="stgmltTemplate"/>
						</xsl:when>
						<xsl:when test="$elementName = 'ice:snwcvr'">
							<xsl:call-template name="snwcvrTemplate"/>
						</xsl:when>
						<xsl:when test="$elementName = 'ice:strptc'">
							<xsl:call-template name="strptcTemplate"/>
						</xsl:when>
						<xsl:when test="$elementName = 'ice:i_grhm'">
							<xsl:call-template name="i_grhmTemplate"/>
						</xsl:when>
						<xsl:otherwise/>
					</xsl:choose>
				</xsl:for-each>
			<!--</displayList>-->	
		</xsl:element>
	</xsl:template>
</xsl:stylesheet>