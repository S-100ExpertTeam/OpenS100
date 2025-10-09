<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
  <xsl:template name="select_arrow">
    <xsl:param name="viewingGroup"/>
    <!--  The viewing group  -->
    <xsl:param name="displayPlane"/>
    <!--  The display plane  -->
    <xsl:param name="drawingPriority"/>
    <!-- don't need variable direction? -->
    <xsl:variable name="speed">
      <xsl:choose>
        <xsl:when test="surfaceCurrentSpeed and number(surfaceCurrentSpeed) = number(surfaceCurrentSpeed) ">
          <xsl:value-of select="number(surfaceCurrentSpeed)"/>
        </xsl:when>
        <xsl:otherwise>-1</xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:variable name="direction">
      <xsl:choose>
        <xsl:when test="surfaceCurrentDirection and number(surfaceCurrentDirection) = number(surfaceCurrentDirection) ">
          <xsl:value-of select="number(surfaceCurrentDirection)"/>
        </xsl:when>
        <xsl:otherwise>0</xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:variable name="scaleFloor"><xsl:value-of select="0.40"/></xsl:variable>
    <xsl:variable name="scaleCeiling"><xsl:value-of select="2.60"/></xsl:variable>
    <xsl:variable name="scaleFactorIntermediate"><xsl:value-of select="0.20"/></xsl:variable>
    <coverageInstruction>
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
      <coverageFill>
        <attributeCode>surfaceCurrentSpeed</attributeCode>
        <placement>directPosition</placement>
        <!--
          <lookup>
          <label>SurfaceCurrentSpeedBandNoData</label>
          <range>
            <xsl:attribute name="upper">0.00</xsl:attribute>
            <xsl:attribute name="closure">ltSemiInterval</xsl:attribute>
          </range>
        </lookup>
        -->
        <lookup>
          <label>SurfaceCurrentSpeedBand1</label>
          <range>
            <xsl:attribute name="lower">0.00</xsl:attribute>
            <xsl:attribute name="upper">0.50</xsl:attribute>
            <xsl:attribute name="closure">geLtInterval</xsl:attribute>  
          </range>
          <symbol>
              <xsl:attribute name="reference">SCAROW01</xsl:attribute>
              <xsl:attribute name="rotationCRS">GeographicCRS</xsl:attribute>
            <xsl:attribute name="defaultScaleFactor"><xsl:value-of select="$scaleFloor"/></xsl:attribute>
            <rotationAttribute>surfaceCurrentDirection</rotationAttribute>
            <rotationFactor>1.0</rotationFactor>
          </symbol>
        </lookup>
        <lookup>
          <label>SurfaceCurrentSpeedBand2</label>
          <range>
            <xsl:attribute name="lower">0.50</xsl:attribute>
            <xsl:attribute name="upper">1.00</xsl:attribute>
            <xsl:attribute name="closure">geLtInterval</xsl:attribute>  
          </range>
          <symbol>
            <xsl:attribute name="reference">SCAROW02</xsl:attribute>
            <xsl:attribute name="rotationCRS">GeographicCRS</xsl:attribute>
            <xsl:attribute name="defaultScaleFactor"><xsl:value-of select="$scaleFloor"/></xsl:attribute>
            <rotationAttribute>surfaceCurrentDirection</rotationAttribute>
            <rotationFactor>1.0</rotationFactor>
          </symbol>
        </lookup>
        <lookup>
          <label>SurfaceCurrentSpeedBand3</label>
          <range>
            <xsl:attribute name="lower">1.00</xsl:attribute>
            <xsl:attribute name="upper">2.00</xsl:attribute>
            <xsl:attribute name="closure">geLtInterval</xsl:attribute>  
          </range>
          <symbol>
            <xsl:attribute name="reference">SCAROW03</xsl:attribute>
            <xsl:attribute name="rotationCRS">GeographicCRS</xsl:attribute>
            <xsl:attribute name="defaultScaleFactor"><xsl:value-of select="$scaleFloor"/></xsl:attribute>
            <rotationAttribute>surfaceCurrentDirection</rotationAttribute>
            <rotationFactor>1.0</rotationFactor>
          </symbol>
        </lookup>
        <lookup>
          <label>SurfaceCurrentSpeedBand4</label>
          <range>
            <xsl:attribute name="lower">2.00</xsl:attribute>
            <xsl:attribute name="upper">3.00</xsl:attribute>
            <xsl:attribute name="closure">geLtInterval</xsl:attribute>  
          </range>
          <symbol>
            <xsl:attribute name="reference">SCAROW04</xsl:attribute>
            <xsl:attribute name="rotationCRS">GeographicCRS</xsl:attribute>
            <rotationAttribute>surfaceCurrentDirection</rotationAttribute>
            <rotationFactor>1.0</rotationFactor>
            <scaleAttribute>surfaceCurrentSpeed</scaleAttribute>
            <scaleFactor><xsl:value-of select="$scaleFactorIntermediate"/></scaleFactor>
          </symbol>
        </lookup>
        <lookup>
          <label>SurfaceCurrentSpeedBand5</label>
          <range>
            <xsl:attribute name="lower">3.00</xsl:attribute>
            <xsl:attribute name="upper">5.00</xsl:attribute>
            <xsl:attribute name="closure">geLtInterval</xsl:attribute>  
          </range>
          <symbol>
            <xsl:attribute name="reference">SCAROW05</xsl:attribute>
            <xsl:attribute name="rotationCRS">GeographicCRS</xsl:attribute>
            <rotationAttribute>surfaceCurrentDirection</rotationAttribute>
            <rotationFactor>1.0</rotationFactor>
            <scaleAttribute>surfaceCurrentSpeed</scaleAttribute>
            <scaleFactor><xsl:value-of select="$scaleFactorIntermediate"/></scaleFactor>
          </symbol>
        </lookup>
        <lookup>
          <label>SurfaceCurrentSpeedBand6</label>
          <range>
            <xsl:attribute name="lower">5.00</xsl:attribute>
            <xsl:attribute name="upper">7.00</xsl:attribute>
            <xsl:attribute name="closure">geLtInterval</xsl:attribute>  
          </range>
          <symbol>
            <xsl:attribute name="reference">SCAROW06</xsl:attribute>
            <xsl:attribute name="rotationCRS">GeographicCRS</xsl:attribute>
            <rotationAttribute>surfaceCurrentDirection</rotationAttribute>
            <rotationFactor>1.0</rotationFactor>
            <scaleAttribute>surfaceCurrentSpeed</scaleAttribute>
            <scaleFactor><xsl:value-of select="$scaleFactorIntermediate"/></scaleFactor>
          </symbol>
        </lookup>
        <lookup>
          <label>SurfaceCurrentSpeedBand7</label>
          <range>
            <xsl:attribute name="lower">7.00</xsl:attribute>
            <xsl:attribute name="upper">10.00</xsl:attribute>
            <xsl:attribute name="closure">geLtInterval</xsl:attribute>  
          </range>
          <symbol>
            <xsl:attribute name="reference">SCAROW07</xsl:attribute>
            <xsl:attribute name="rotationCRS">GeographicCRS</xsl:attribute>
            <rotationAttribute>surfaceCurrentDirection</rotationAttribute>
            <rotationFactor>1.0</rotationFactor>
            <scaleAttribute>surfaceCurrentSpeed</scaleAttribute>
            <scaleFactor><xsl:value-of select="$scaleFactorIntermediate"/></scaleFactor>
          </symbol>
        </lookup>
        <lookup>
          <label>SurfaceCurrentSpeedBand8</label>
          <range>
            <xsl:attribute name="lower">10.00</xsl:attribute>
            <xsl:attribute name="upper">13.00</xsl:attribute>
            <xsl:attribute name="closure">geLtInterval</xsl:attribute>  
          </range>
          <symbol>
            <xsl:attribute name="reference">SCAROW08</xsl:attribute>
            <xsl:attribute name="rotationCRS">GeographicCRS</xsl:attribute>
            <rotationAttribute>surfaceCurrentDirection</rotationAttribute>
            <rotationFactor>1.0</rotationFactor>
            <scaleAttribute>surfaceCurrentSpeed</scaleAttribute>
            <scaleFactor><xsl:value-of select="$scaleFactorIntermediate"/></scaleFactor>
          </symbol>
        </lookup>
        <lookup>
          <label>SurfaceCurrentSpeedBand9</label>
          <range>
            <xsl:attribute name="lower">13.00</xsl:attribute>
            <xsl:attribute name="closure">geSemiInterval</xsl:attribute>  
          </range>
          <symbol>
            <xsl:attribute name="reference">SCAROW09</xsl:attribute>
            <xsl:attribute name="rotationCRS">GeographicCRS</xsl:attribute>
            <xsl:attribute name="defaultScaleFactor"><xsl:value-of select="$scaleCeiling"/></xsl:attribute>
            <rotationAttribute>surfaceCurrentDirection</rotationAttribute>
            <rotationFactor>1.0</rotationFactor>
          </symbol>
        </lookup>
      </coverageFill>	
    </coverageInstruction>
  </xsl:template>
</xsl:transform>