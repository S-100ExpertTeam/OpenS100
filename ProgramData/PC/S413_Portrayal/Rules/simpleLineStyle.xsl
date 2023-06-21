<?xml version="1.0" encoding="UTF-8"?>
<xsl:transform
    version="1.0"
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

    <xsl:output method="xml" encoding="UTF-8" indent="yes" omit-xml-declaration="no" />


<!--        Called in line drawing instruction templates as follows
			<xsl:call-template name="simpleLineStyle">
				<xsl:with-param name="style">dash</xsl:with-param>
				<xsl:with-param name="width">0.32</xsl:with-param>
				<xsl:with-param name="colour">CHMGD</xsl:with-param>
			</xsl:call-template>
		-->
		
	<!-- Named template to generate a simple lineStyle -->
	<xsl:template name="simpleLineStyle">
	<xsl:param name="style"/> <!-- The line style, solid or dash or dot -->
	<xsl:param name="width"/> <!-- line width in mm -->
	<xsl:param name="colour"/> <!-- colour token -->
		<xsl:element name="lineStyle">
			<xsl:if test="$style = 'dash'" >
				<xsl:element name="intervalLength">5.4</xsl:element>
			</xsl:if>
			<xsl:if test="$style = 'dot'" >
				<xsl:element name="intervalLength">1.8</xsl:element>
			</xsl:if>
			<xsl:element name="pen">
				<xsl:attribute name="width"><xsl:value-of select="format-number($width,'####.##')"/></xsl:attribute>
				<xsl:element name="color">
					<xsl:value-of select="$colour"/>
				</xsl:element>
			</xsl:element>
			<xsl:if test="$style = 'dash'" >
				<xsl:element name="dash">
					<!-- S-52 DASH defined as 3.6mm dash followed by 1.8mm space  -->
					<xsl:element name="start">0</xsl:element>
					<xsl:element name="length">3.6</xsl:element>
				</xsl:element>
			</xsl:if>
			<xsl:if test="$style = 'dot'" >
				<xsl:element name="dash">
					<!-- S-52 DOTT defined as 0.6mm dash followed by 1.2mm space  -->
					<xsl:element name="start">0</xsl:element>
					<xsl:element name="length">0.6</xsl:element>
				</xsl:element>
			</xsl:if>

		</xsl:element>
	</xsl:template>
		
</xsl:transform>
		