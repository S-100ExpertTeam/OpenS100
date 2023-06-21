<?xml version="1.0" encoding="UTF-8"?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
	<xsl:output method="xml" encoding="UTF-8" indent="yes"/>

	<!-- Mar 25, 2015 Named template to output Seabed area text for surface characteristics -->
	<xsl:template name="natsur01">
	<xsl:param name="viewingGroup"/> <!-- The viewing group -->
	<xsl:param name="displayPlane"/> <!-- The display plane -->
	<xsl:param name="drawingPriority"/> <!-- The drawing priority -->

      <xsl:if test="surfaceCharacteristics!= ''">
         <textInstruction>
            <featureReference>
               <xsl:value-of select="@id"/>
            </featureReference>
            <viewingGroup><xsl:value-of select="$viewingGroup"/></viewingGroup>
            <displayPlane><xsl:value-of select="$displayPlane"/></displayPlane>
            <drawingPriority><xsl:value-of select="$drawingPriority"/></drawingPriority>
            <textPoint horizontalAlignment="Center" verticalAlignment="Center">
               <element>
                  <text>
						<xsl:call-template name="natsurText"/>
                  </text>
                  <xsl:call-template name="textStyle">
                     <xsl:with-param name="style">default</xsl:with-param>
                  </xsl:call-template>
               </element>
               <offset>
                  <x>0</x>
                  <y>0</y>
               </offset>
               <areaPlacement placementMode="VisibleParts"/>
            </textPoint>
         </textInstruction>
      </xsl:if>
	
	</xsl:template>
	
	<!-- Mar 25, 2015 Named template for defining Seabed area text for surface characteristics -->
	<xsl:template name="natsurText">

		<!-- Make a list of Layers, assuming there are no more than 99 layers use '__' for no layer -->
		<!-- This will be used to determine when to separate by space or slash -->
		<xsl:variable name="layerList">
			<xsl:for-each select="surfaceCharacteristics">
			<xsl:sort select="underlyingLayer" order="ascending" />
			<xsl:choose>
				<xsl:when test="number(./underlyingLayer) = ./underlyingLayer "> <!-- if number -->
					<xsl:value-of select="format-number(./underlyingLayer, '00','dformat')"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:text>__</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
			</xsl:for-each>
		</xsl:variable>

		<xsl:for-each select="surfaceCharacteristics">
		<xsl:sort select="underlyingLayer" order="ascending" />
		
			<!-- describe one layer -->
			<!-- translate the nature of surface qualifiers -->
			<xsl:for-each select="natureOfSurfaceQualifyingTerms">
				<xsl:choose>
					<xsl:when test=". = 1 ">
						<xsl:text>f</xsl:text> <!-- fine -->
					</xsl:when>
					<xsl:when test=". = 2  ">
						<xsl:text>m</xsl:text> <!-- medium -->
					</xsl:when>
					<xsl:when test=". = 3  ">
						<xsl:text>c</xsl:text> <!-- coarse -->
					</xsl:when>
					<xsl:when test=".= 4 ">
						<xsl:text>bk</xsl:text> <!-- broken -->
					</xsl:when>
					<xsl:when test=". = 5 ">
						<xsl:text>sy</xsl:text> <!-- sticky -->
					</xsl:when>
					<xsl:when test=". = 6">
						<xsl:text>so</xsl:text> <!-- soft -->
					</xsl:when>
					<xsl:when test=".= 7 ">
						<xsl:text>sf</xsl:text> <!-- Stiff -->
					</xsl:when>
					<xsl:when test=". = 8 "> <!-- volcanic -->
						<xsl:text>v</xsl:text>
					</xsl:when>
					<xsl:when test=". = 9 "> <!-- calcareous -->
						<xsl:text>ca</xsl:text>
					</xsl:when>
					<xsl:when test=". = 10 "> <!-- hard -->
						<xsl:text>h</xsl:text>
					</xsl:when>
					<xsl:otherwise>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:for-each>
					
			<!-- translate the natureOfSurface -->
			<xsl:choose>
				<xsl:when test="natureOfSurface = 1 ">
					<xsl:text>M</xsl:text>
				</xsl:when>
					<xsl:when test="natureOfSurface = 2  ">
						<xsl:text>Cy</xsl:text>
					</xsl:when>
					<xsl:when test="natureOfSurface = 3  ">
						<xsl:text>Si</xsl:text>
					</xsl:when>
					<xsl:when test="natureOfSurface = 4 ">
						<xsl:text>S</xsl:text>
					</xsl:when>
					<xsl:when test="natureOfSurface = 5 ">
						<xsl:text>St</xsl:text>
					</xsl:when>
					<xsl:when test="natureOfSurface = 6">
						<xsl:text>G</xsl:text>
					</xsl:when>
					<xsl:when test="natureOfSurface = 7 ">
						<xsl:text>P</xsl:text>
					</xsl:when>
					<xsl:when test="natureOfSurface = 8 ">
						<xsl:text>Cb</xsl:text>
					</xsl:when>
					<xsl:when test="natureOfSurface = 9 ">
						<xsl:text>R</xsl:text>
					</xsl:when>
					<xsl:when test="natureOfSurface = 11 ">
						<xsl:text>R</xsl:text>
					</xsl:when>
					<xsl:when test="natureOfSurface = 14 ">
						<xsl:text>Co</xsl:text>
					</xsl:when>
					<xsl:when test="natureOfSurface = 17 ">
						<xsl:text>Sh</xsl:text>
					</xsl:when>
					<xsl:when test="natureOfSurface = 18 ">
						<xsl:text>R</xsl:text>
					</xsl:when>
					<xsl:otherwise>
					</xsl:otherwise>
				</xsl:choose>

				<!-- make a 2 character string for the current layer to compare with the next one in the list -->
				<xsl:variable name="currentLayer" >
					<xsl:choose>
						<xsl:when test="number(./underlyingLayer) = ./underlyingLayer "> <!-- if number -->
							<xsl:value-of select="format-number(./underlyingLayer, '00','dformat')"/>
						</xsl:when>
						<xsl:otherwise>
							<xsl:text>__</xsl:text>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:variable>
				
				<!-- If the layer is the same as the next one, separate with a space, otherwise separate with a slash '/' if there is no more then no separator is needed -->
				<xsl:choose>
					<xsl:when test="position() = last() "> <!-- end of the list -->
					</xsl:when>
					<xsl:when test="substring($layerList,position() * 2+1,2) = $currentLayer ">
						<xsl:text> </xsl:text>
					</xsl:when>
					<xsl:otherwise>
						<xsl:text>/</xsl:text>
					</xsl:otherwise>
				</xsl:choose>
		</xsl:for-each>
	
	</xsl:template>
			
</xsl:transform>
