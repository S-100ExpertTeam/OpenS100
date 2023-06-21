<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
  <xsl:template name="select_arrow">
    <xsl:param name="viewingGroup"/>
    <!--  The viewing group  -->
    <xsl:param name="displayPlane"/>
    <!--  The display plane  -->
    <xsl:param name="drawingPriority"/>
    <xsl:variable name="speed">
      <xsl:choose>
        <xsl:when test="surfaceCurrentSpeed and number(surfaceCurrentSpeed) = number(surfaceCurrentSpeed) ">
          <xsl:value-of select="number(surfaceCurrentSpeed)"/>
        </xsl:when>
        <xsl:otherwise>-1</xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:variable name="Direction">
      <xsl:choose>
        <xsl:when test="surfaceCurrentDirection and number(surfaceCurrentDirection) = number(surfaceCurrentDirection) ">
          <xsl:value-of select="number(surfaceCurrentDirection)"/>
        </xsl:when>
        <xsl:otherwise>0</xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:variable name="scale">
      <xsl:choose>
        <xsl:when test="surfaceCurrentSpeed and number(surfaceCurrentSpeed) = number(surfaceCurrentSpeed) and number(surfaceCurrentSpeed) > 13">
          <xsl:value-of select="13 * 0.4"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="number(surfaceCurrentSpeed) * 0.4"></xsl:value-of>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:choose>
      <xsl:when test="0.49 >= $speed">
        <pointInstruction>
          <featureReference>
            <xsl:value-of select="@id"/>
          </featureReference>
          <viewingGroup>
            <xsl:value-of select="$viewingGroup"/>
          </viewingGroup>
          <displayPlane>
            <xsl:value-of select="$displayPlane"/>
          </displayPlane>
          <drawingPriority>
            <xsl:value-of select="$drawingPriority"/>
          </drawingPriority>
          <!--<symbol reference="SCAROW01" rotation="$surfaceCurrentDirection"/>-->
          <xsl:element name="symbol">
            <xsl:attribute name="reference">SCAROW01</xsl:attribute>
            <xsl:attribute name="rotation">
              <xsl:value-of select="$Direction"/>
            </xsl:attribute>
            <xsl:attribute name="scaleFactor">
				      <xsl:value-of select="$scale"></xsl:value-of>
            </xsl:attribute>
          </xsl:element>
        </pointInstruction>
      </xsl:when>
      <xsl:when test="0.99 >= $speed">
        <pointInstruction>
          <featureReference>
            <xsl:value-of select="@id"/>
          </featureReference>
          <viewingGroup>
            <xsl:value-of select="$viewingGroup"/>
          </viewingGroup>
          <displayPlane>
            <xsl:value-of select="$displayPlane"/>
          </displayPlane>
          <drawingPriority>
            <xsl:value-of select="$drawingPriority"/>
          </drawingPriority>
          <xsl:element name="symbol">
            <xsl:attribute name="reference">SCAROW02</xsl:attribute>
            <xsl:attribute name="rotation">
              <xsl:value-of select="$Direction"/>
            </xsl:attribute>
            <xsl:attribute name="scaleFactor">
              <xsl:value-of select="$scale"></xsl:value-of>
            </xsl:attribute>
          </xsl:element>
        </pointInstruction>
      </xsl:when>
      <xsl:when test="1.99 >= $speed">
        <pointInstruction>
          <featureReference>
            <xsl:value-of select="@id"/>
          </featureReference>
          <viewingGroup>
            <xsl:value-of select="$viewingGroup"/>
          </viewingGroup>
          <displayPlane>
            <xsl:value-of select="$displayPlane"/>
          </displayPlane>
          <drawingPriority>
            <xsl:value-of select="$drawingPriority"/>
          </drawingPriority>
          <xsl:element name="symbol">
            <xsl:attribute name="reference">SCAROW03</xsl:attribute>
            <xsl:attribute name="rotation">
              <xsl:value-of select="$Direction"/>
            </xsl:attribute>
            <xsl:attribute name="scaleFactor">
              <xsl:value-of select="$scale"></xsl:value-of>
            </xsl:attribute>
          </xsl:element>
        </pointInstruction>
      </xsl:when>
      <xsl:when test="2.99 >= $speed">
        <pointInstruction>
          <featureReference>
            <xsl:value-of select="@id"/>
          </featureReference>
          <viewingGroup>
            <xsl:value-of select="$viewingGroup"/>
          </viewingGroup>
          <displayPlane>
            <xsl:value-of select="$displayPlane"/>
          </displayPlane>
          <drawingPriority>
            <xsl:value-of select="$drawingPriority"/>
          </drawingPriority>
          <xsl:element name="symbol">
            <xsl:attribute name="reference">SCAROW04</xsl:attribute>
            <xsl:attribute name="rotation">
              <xsl:value-of select="$Direction"/>
            </xsl:attribute>
            <xsl:attribute name="scaleFactor">
              <xsl:value-of select="$scale"></xsl:value-of>
            </xsl:attribute>
          </xsl:element>
        </pointInstruction>
      </xsl:when>
      <xsl:when test="4.99 >= $speed">
        <pointInstruction>
          <featureReference>
            <xsl:value-of select="@id"/>
          </featureReference>
          <viewingGroup>
            <xsl:value-of select="$viewingGroup"/>
          </viewingGroup>
          <displayPlane>
            <xsl:value-of select="$displayPlane"/>
          </displayPlane>
          <drawingPriority>
            <xsl:value-of select="$drawingPriority"/>
          </drawingPriority>
          <xsl:element name="symbol">
            <xsl:attribute name="reference">SCAROW05</xsl:attribute>
            <xsl:attribute name="rotation">
              <xsl:value-of select="$Direction"/>
            </xsl:attribute>
            <xsl:attribute name="scaleFactor">
              <xsl:value-of select="$scale"></xsl:value-of>
            </xsl:attribute>
          </xsl:element>
        </pointInstruction>
      </xsl:when>
      <xsl:when test="6.99 >= $speed">
        <pointInstruction>
          <featureReference>
            <xsl:value-of select="@id"/>
          </featureReference>
          <viewingGroup>
            <xsl:value-of select="$viewingGroup"/>
          </viewingGroup>
          <displayPlane>
            <xsl:value-of select="$displayPlane"/>
          </displayPlane>
          <drawingPriority>
            <xsl:value-of select="$drawingPriority"/>
          </drawingPriority>
          <xsl:element name="symbol">
            <xsl:attribute name="reference">SCAROW06</xsl:attribute>
            <xsl:attribute name="rotation">
              <xsl:value-of select="$Direction"/>
            </xsl:attribute>
            <xsl:attribute name="scaleFactor">
              <xsl:value-of select="$scale"></xsl:value-of>
            </xsl:attribute>
          </xsl:element>
        </pointInstruction>
      </xsl:when>
      <xsl:when test="9.99 >= $speed">
        <pointInstruction>
          <featureReference>
            <xsl:value-of select="@id"/>
          </featureReference>
          <viewingGroup>
            <xsl:value-of select="$viewingGroup"/>
          </viewingGroup>
          <displayPlane>
            <xsl:value-of select="$displayPlane"/>
          </displayPlane>
          <drawingPriority>
            <xsl:value-of select="$drawingPriority"/>
          </drawingPriority>
          <xsl:element name="symbol">
            <xsl:attribute name="reference">SCAROW07</xsl:attribute>
            <xsl:attribute name="rotation">
              <xsl:value-of select="$Direction"/>
            </xsl:attribute>
            <xsl:attribute name="scaleFactor">
              <xsl:value-of select="$scale"></xsl:value-of>
            </xsl:attribute>
          </xsl:element>
        </pointInstruction>
      </xsl:when>
      <xsl:when test="12.99 >= $speed">
        <pointInstruction>
          <featureReference>
            <xsl:value-of select="@id"/>
          </featureReference>
          <viewingGroup>
            <xsl:value-of select="$viewingGroup"/>
          </viewingGroup>
          <displayPlane>
            <xsl:value-of select="$displayPlane"/>
          </displayPlane>
          <drawingPriority>
            <xsl:value-of select="$drawingPriority"/>
          </drawingPriority>
          <xsl:element name="symbol">
            <xsl:attribute name="reference">SCAROW08</xsl:attribute>
            <xsl:attribute name="rotation">
              <xsl:value-of select="$Direction"/>
            </xsl:attribute>
            <xsl:attribute name="scaleFactor">
              <xsl:value-of select="$scale"></xsl:value-of>
            </xsl:attribute>
          </xsl:element>
        </pointInstruction>
      </xsl:when>
      <xsl:otherwise>
        <pointInstruction>
          <featureReference>
            <xsl:value-of select="@id"/>
          </featureReference>
          <viewingGroup>
          </viewingGroup>
            <xsl:value-of select="$viewingGroup"/>
          <displayPlane>
            <xsl:value-of select="$displayPlane"/>
          </displayPlane>
          <drawingPriority>
            <xsl:value-of select="$drawingPriority"/>
          </drawingPriority>
          <xsl:element name="symbol">
            <xsl:attribute name="reference">SCAROW09</xsl:attribute>
            <xsl:attribute name="rotation">
              <xsl:value-of select="$Direction"/>
            </xsl:attribute>
            <xsl:attribute name="scaleFactor">
              <xsl:value-of select="$scale"></xsl:value-of>
            </xsl:attribute>
          </xsl:element>
        </pointInstruction>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>
</xsl:transform>