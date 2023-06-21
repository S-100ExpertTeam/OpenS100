<?xml version="1.0" encoding="UTF-8"?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
	<xsl:output method="xml" encoding="UTF-8" indent="yes"/>
	<xsl:template match="featureName" mode="text">
		<xsl:param name="prefix"/>
		<xsl:param name="suffix"/>
		<!-- features can have more than one featureName, the name used for display should have "displayName = 'true'" but if there is no featureName with displayName = 'true' this will output the first featureName in the list. If there is more than one featureName with displayName = 'true' all of them will be output, DCEG says only one should be true -->
			<xsl:if test="displayName = 'true' or (position() = 1 and count(../featureName/displayName[. = 'true']) = 0  )">
				<xsl:if test="$prefix">
					<xsl:value-of select="$prefix"/>
				</xsl:if>
				<xsl:value-of select="name"/>
				<xsl:if test="$suffix">
					<xsl:value-of select="$suffix"/>
				</xsl:if>
			</xsl:if>
	</xsl:template>
	<xsl:template match="verticalClearanceFixed/clearanceValueVertical" mode="text">
		<xsl:param name="prefix"/>
		<xsl:param name="suffix"/>
		<xsl:if test="$prefix">
			<xsl:value-of select="$prefix"/>
		</xsl:if>
		<xsl:value-of select="format-number(., '##.#','dformat')"/>
		<xsl:if test="$suffix">
			<xsl:value-of select="$suffix"/>
		</xsl:if>
	</xsl:template>
	<xsl:template match="verticalClearanceClosed/clearanceValueVertical" mode="text">
		<xsl:param name="prefix"/>
		<xsl:param name="suffix"/>
		<xsl:if test="$prefix">
			<xsl:value-of select="$prefix"/>
		</xsl:if>
		<xsl:value-of select="format-number(., '##.#','dformat')"/>
		<xsl:if test="$suffix">
			<xsl:value-of select="$suffix"/>
		</xsl:if>
	</xsl:template>
	<xsl:template match="verticalClearanceOpen/clearanceValueVertical" mode="text">
		<xsl:param name="prefix"/>
		<xsl:param name="suffix"/>
		<xsl:if test="$prefix">
			<xsl:value-of select="$prefix"/>
		</xsl:if>
		<xsl:value-of select="format-number(., '##.#','dformat')"/>
		<xsl:if test="$suffix">
			<xsl:value-of select="$suffix"/>
		</xsl:if>
	</xsl:template>
	<xsl:template match="verticalClearanceSafe/verticalUncertainty" mode="text">
		<xsl:param name="prefix"/>
		<xsl:param name="suffix"/>
		<xsl:if test="$prefix">
			<xsl:value-of select="$prefix"/>
		</xsl:if>
		<xsl:value-of select="format-number(., '##.#','dformat')"/>
		<xsl:if test="$suffix">
			<xsl:value-of select="$suffix"/>
		</xsl:if>
	</xsl:template>
	<xsl:template match="currentVelocity/velocityMaximum" mode="text">
		<xsl:param name="prefix"/>
		<xsl:param name="suffix" select="' kn'"/> <!-- set to ' kn' if param not given -->
		<xsl:if test="$prefix">
			<xsl:value-of select="$prefix"/>
		</xsl:if>
		<xsl:value-of select="format-number(., '##.#','dformat')"/>
		<xsl:if test="$suffix">
			<xsl:value-of select="$suffix"/>
		</xsl:if>
	</xsl:template>
	<xsl:template match="information/text" mode="text">
		<xsl:param name="prefix"/>
		<xsl:param name="suffix"/>
		<xsl:if test="$prefix">
			<xsl:value-of select="$prefix"/>
		</xsl:if>
		<xsl:value-of select="."/>
		<xsl:if test="$suffix">
			<xsl:value-of select="$suffix"/>
		</xsl:if>
	</xsl:template>
	<xsl:template match="orientation/orientationValue" mode="text">
		<xsl:param name="prefix"/>
		<xsl:param name="suffix" select="' deg'"/> <!-- set to ' deg' if param not given -->
		<xsl:if test="$prefix">
			<xsl:value-of select="$prefix"/>
		</xsl:if>
		<xsl:value-of select="format-number(., '000','dformat')"/>
		<xsl:if test="$suffix">
			<xsl:value-of select="$suffix"/>
		</xsl:if>
	</xsl:template>
	<xsl:template match="elevation" mode="text">
		<xsl:param name="prefix"/>
		<xsl:param name="suffix" select="' m'"/> <!-- set to ' m' if param not given -->
		<xsl:if test="$prefix">
			<xsl:value-of select="$prefix"/>
		</xsl:if>
		<xsl:value-of select="format-number(., '###','dformat')"/>
		<xsl:if test="$suffix">
			<xsl:value-of select="$suffix"/>
		</xsl:if>
	</xsl:template>
	<xsl:template match="valueOfMagneticVariation" mode="text">
		<xsl:param name="prefix"/>
		<xsl:param name="suffix"/>
		<xsl:if test="$prefix">
			<xsl:value-of select="$prefix"/>
		</xsl:if>
		<xsl:value-of select="format-number(., '.##','dformat')"/>
		<xsl:if test="$suffix">
			<xsl:value-of select="$suffix"/>
		</xsl:if>
	</xsl:template>
	<xsl:template match="communicationInformation/communicationChannel" mode="text">
		<xsl:param name="prefix"/>
		<xsl:param name="suffix"/>
		<xsl:if test="$prefix">
			<xsl:value-of select="$prefix"/>
		</xsl:if>
		<xsl:value-of select="."/>
		<xsl:if test="$suffix">
			<xsl:value-of select="$suffix"/>
		</xsl:if>
	</xsl:template>
	<xsl:template match="depthRangeMinimumValue" mode="text">
		<xsl:param name="prefix"/>
		<xsl:param name="suffix"/>
		<xsl:if test="$prefix">
			<xsl:value-of select="$prefix"/>
		</xsl:if>
		<xsl:value-of select="format-number(., '###.#','dformat')"/>
		<xsl:if test="$suffix">
			<xsl:value-of select="$suffix"/>
		</xsl:if>
	</xsl:template>
	<xsl:template match="usrmrk" mode="text">
		<xsl:param name="prefix"/>
		<xsl:param name="suffix"/>
		<xsl:if test="$prefix">
			<xsl:value-of select="$prefix"/>
		</xsl:if>
		<xsl:value-of select="."/>
		<xsl:if test="$suffix">
			<xsl:value-of select="$suffix"/>
		</xsl:if>
	</xsl:template>
</xsl:transform>
