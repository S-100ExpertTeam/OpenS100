<?xml version="1.0" encoding="UTF-8"?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
    <xsl:output method="xml" encoding="UTF-8" indent="yes"/>

    <!-- Template for text style elements of a text instruction -->
    <xsl:template name="textStyle">
	<xsl:param name="style"  select="default"/> <!-- set to 'default' if param not given -->

		<xsl:choose>
			<xsl:when test="$style = 'default'">
				<xsl:element name="bodySize">10</xsl:element>				
				<xsl:element name="foreground">CHBLK</xsl:element>				
				<xsl:element name="fontCharacteristics"> <!-- The font -->
					<xsl:element name="weight">Medium</xsl:element>				
					<xsl:element name="slant">Upright</xsl:element> 				
					<xsl:element name="serifs">0</xsl:element> <!-- boolean no serifs -->				
					<xsl:element name="proportion">MonoSpaced</xsl:element>			
				</xsl:element>		<!-- The font -->		
			</xsl:when>
			<xsl:when test="$style = 'Italics'">  <!-- Added Apr 2023 -->
				<xsl:element name="bodySize">10</xsl:element>				
				<xsl:element name="foreground">CHBLK</xsl:element>				
				<xsl:element name="fontCharacteristics"> <!-- The font -->
					<xsl:element name="weight">Medium</xsl:element>				
					<xsl:element name="slant">Italics</xsl:element> 				
					<xsl:element name="serifs">0</xsl:element> <!-- boolean no serifs -->				
					<xsl:element name="proportion">MonoSpaced</xsl:element>			
				</xsl:element>		<!-- The font -->		
			</xsl:when>
			<xsl:when test="$style = 'Light'">  <!-- Added Apr 2023 -->
				<xsl:element name="bodySize">10</xsl:element>				
				<xsl:element name="foreground">CHBLK</xsl:element>				
				<xsl:element name="fontCharacteristics"> <!-- The font -->
					<xsl:element name="weight">Light</xsl:element>				
					<xsl:element name="slant">Upright</xsl:element> 				
					<xsl:element name="serifs">0</xsl:element> <!-- boolean no serifs -->				
					<xsl:element name="proportion">MonoSpaced</xsl:element>			
				</xsl:element>		<!-- The font -->		
			</xsl:when>
			<xsl:when test="$style = 'Magenta'">  <!-- Added Apr 2023 -->
				<xsl:element name="bodySize">10</xsl:element>				
				<xsl:element name="foreground">CHMGD</xsl:element>				
				<xsl:element name="fontCharacteristics"> <!-- The font -->
					<xsl:element name="weight">Medium</xsl:element>				
					<xsl:element name="slant">Upright</xsl:element> 				
					<xsl:element name="serifs">0</xsl:element> <!-- boolean no serifs -->				
					<xsl:element name="proportion">MonoSpaced</xsl:element>			
				</xsl:element>		<!-- The font -->		
			</xsl:when>
			<xsl:when test="$style = 'LightMagenta'">  <!-- Added Apr 2023 -->
				<xsl:element name="bodySize">10</xsl:element>				
				<xsl:element name="foreground">CHMGD</xsl:element>				
				<xsl:element name="fontCharacteristics"> <!-- The font -->
					<xsl:element name="weight">Light</xsl:element>				
					<xsl:element name="slant">Upright</xsl:element> 				
					<xsl:element name="serifs">0</xsl:element> <!-- boolean no serifs -->				
					<xsl:element name="proportion">MonoSpaced</xsl:element>			
				</xsl:element>		<!-- The font -->		
			</xsl:when>
			<xsl:when test="$style = 'LightMagentaFaint'">  <!-- Added Apr 2023 -->
				<xsl:element name="bodySize">10</xsl:element>				
				<xsl:element name="foreground">CHMGF</xsl:element>				
				<xsl:element name="fontCharacteristics"> <!-- The font -->
					<xsl:element name="weight">Light</xsl:element>				
					<xsl:element name="slant">Upright</xsl:element> 				
					<xsl:element name="serifs">0</xsl:element> <!-- boolean no serifs -->				
					<xsl:element name="proportion">MonoSpaced</xsl:element>			
				</xsl:element>		<!-- The font -->		
			</xsl:when>
			<xsl:otherwise>
				<xsl:text>error:unrecogonized text style parameter:</xsl:text> <xsl:value-of select="$style"/>
			</xsl:otherwise>
		</xsl:choose>
	
				
	
    </xsl:template>
	
</xsl:transform>