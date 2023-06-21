<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
  <xsl:template name="select_fillcolor">
    <xsl:param name="viewingGroup"/>
    <!--  The viewing group  -->
    <xsl:param name="displayPlane"/>
    <!--  The display plane  -->
    <xsl:param name="drawingPriority"/>
    <xsl:variable name="Elevation">
      <xsl:value-of select="elevation"/>
      <!--<xsl:choose>
        <xsl:when test="elevation and number(elevation) = number(elevation) ">
          <xsl:value-of select="number(elevation)"/>
        </xsl:when>
        <xsl:otherwise>-1</xsl:otherwise>
      </xsl:choose>-->
    </xsl:variable>
    <xsl:variable name="Color">
      <xsl:choose>
        <xsl:when test="$Elevation > 32.021">
          <xsl:value-of select="'BA001'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 31.571">
          <xsl:value-of select="'BA002'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 31.121">
          <xsl:value-of select="'BA003'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 30.671">
          <xsl:value-of select="'BA004'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 30.221">
          <xsl:value-of select="'BA005'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 29.771">
          <xsl:value-of select="'BA006'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 29.321">
          <xsl:value-of select="'BA007'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 28.871">
          <xsl:value-of select="'BA008'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 28.421">
          <xsl:value-of select="'BA009'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 27.971">
          <xsl:value-of select="'BA010'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 27.521">
          <xsl:value-of select="'BA011'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 27.071">
          <xsl:value-of select="'BA012'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 26.621">
          <xsl:value-of select="'BA013'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 26.171">
          <xsl:value-of select="'BA014'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 25.721">
          <xsl:value-of select="'BA015'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 25.271">
          <xsl:value-of select="'BA016'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 24.821">
          <xsl:value-of select="'BA017'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 24.371">
          <xsl:value-of select="'BA018'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 23.921">
          <xsl:value-of select="'BA019'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 23.471">
          <xsl:value-of select="'BA020'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 23.021">
          <xsl:value-of select="'BA021'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 22.571">
          <xsl:value-of select="'BA022'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 22.121">
          <xsl:value-of select="'BA023'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 21.671">
          <xsl:value-of select="'BA024'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 21.221">
          <xsl:value-of select="'BA025'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 20.771">
          <xsl:value-of select="'BA026'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 20.321">
          <xsl:value-of select="'BA027'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 19.871">
          <xsl:value-of select="'BA028'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 19.421">
          <xsl:value-of select="'BA029'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 18.971">
          <xsl:value-of select="'BA030'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 18.521">
          <xsl:value-of select="'BA031'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 18.071">
          <xsl:value-of select="'BA032'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 17.621">
          <xsl:value-of select="'BA033'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 17.171">
          <xsl:value-of select="'BA034'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 16.721">
          <xsl:value-of select="'BA035'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 16.271">
          <xsl:value-of select="'BA036'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 15.821">
          <xsl:value-of select="'BA037'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 15.371">
          <xsl:value-of select="'BA038'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 14.921">
          <xsl:value-of select="'BA039'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 14.471">
          <xsl:value-of select="'BA040'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 14.021">
          <xsl:value-of select="'BA041'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 13.571">
          <xsl:value-of select="'BA042'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 13.121">
          <xsl:value-of select="'BA043'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 12.671">
          <xsl:value-of select="'BA044'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 12.221">
          <xsl:value-of select="'BA045'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 11.771">
          <xsl:value-of select="'BA046'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 11.321">
          <xsl:value-of select="'BA047'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 10.871">
          <xsl:value-of select="'BA048'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 10.421">
          <xsl:value-of select="'BA049'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 9.97100000000004">
          <xsl:value-of select="'BA050'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 9.52100000000004">
          <xsl:value-of select="'BA051'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 9.07100000000004">
          <xsl:value-of select="'BA052'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 8.62100000000004">
          <xsl:value-of select="'BA053'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 8.17100000000004">
          <xsl:value-of select="'BA054'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 7.72100000000004">
          <xsl:value-of select="'BA055'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 7.27100000000004">
          <xsl:value-of select="'BA056'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 6.82100000000004">
          <xsl:value-of select="'BA057'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 6.37100000000004">
          <xsl:value-of select="'BA058'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 5.92100000000004">
          <xsl:value-of select="'BA059'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 5.47100000000004">
          <xsl:value-of select="'BA060'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 5.02100000000004">
          <xsl:value-of select="'BA061'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 4.57100000000004">
          <xsl:value-of select="'BA062'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 4.12100000000004">
          <xsl:value-of select="'BA063'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 3.67100000000004">
          <xsl:value-of select="'BA064'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 3.22100000000004">
          <xsl:value-of select="'BA065'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 2.77100000000004">
          <xsl:value-of select="'BA066'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 2.32100000000004">
          <xsl:value-of select="'BA067'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 1.87100000000004">
          <xsl:value-of select="'BA068'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 1.42100000000004">
          <xsl:value-of select="'BA069'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 0.971000000000036">
          <xsl:value-of select="'BA070'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 0.521000000000036">
          <xsl:value-of select="'BA071'"/>
        </xsl:when>
        <xsl:when test="$Elevation > 0.0710000000000363">
          <xsl:value-of select="'BA072'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -0.378999999999964">
          <xsl:value-of select="'BA073'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -0.828999999999964">
          <xsl:value-of select="'BA074'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -1.27899999999996">
          <xsl:value-of select="'BA075'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -1.72899999999996">
          <xsl:value-of select="'BA076'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -2.17899999999996">
          <xsl:value-of select="'BA077'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -2.62899999999996">
          <xsl:value-of select="'BA078'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -3.07899999999996">
          <xsl:value-of select="'BA079'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -3.52899999999996">
          <xsl:value-of select="'BA080'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -3.97899999999996">
          <xsl:value-of select="'BA081'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -4.42899999999996">
          <xsl:value-of select="'BA082'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -4.87899999999996">
          <xsl:value-of select="'BA083'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -5.32899999999997">
          <xsl:value-of select="'BA084'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -5.77899999999997">
          <xsl:value-of select="'BA085'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -6.22899999999997">
          <xsl:value-of select="'BA086'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -6.67899999999997">
          <xsl:value-of select="'BA087'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -7.12899999999997">
          <xsl:value-of select="'BA088'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -7.57899999999997">
          <xsl:value-of select="'BA089'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -8.02899999999997">
          <xsl:value-of select="'BA090'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -8.47899999999997">
          <xsl:value-of select="'BA091'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -8.92899999999996">
          <xsl:value-of select="'BA092'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -9.37899999999996">
          <xsl:value-of select="'BA093'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -9.82899999999996">
          <xsl:value-of select="'BA094'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -10.279">
          <xsl:value-of select="'BA095'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -10.729">
          <xsl:value-of select="'BA096'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -11.179">
          <xsl:value-of select="'BA097'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -11.629">
          <xsl:value-of select="'BA098'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -12.079">
          <xsl:value-of select="'BA099'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -12.529">
          <xsl:value-of select="'BA100'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -12.979">
          <xsl:value-of select="'BA101'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -13.429">
          <xsl:value-of select="'BA102'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -13.879">
          <xsl:value-of select="'BA103'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -14.329">
          <xsl:value-of select="'BA104'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -14.779">
          <xsl:value-of select="'BA105'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -15.229">
          <xsl:value-of select="'BA106'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -15.679">
          <xsl:value-of select="'BA107'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -16.129">
          <xsl:value-of select="'BA108'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -16.579">
          <xsl:value-of select="'BA109'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -17.029">
          <xsl:value-of select="'BA110'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -17.479">
          <xsl:value-of select="'BA111'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -17.929">
          <xsl:value-of select="'BA112'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -18.379">
          <xsl:value-of select="'BA113'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -18.829">
          <xsl:value-of select="'BA114'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -19.279">
          <xsl:value-of select="'BA115'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -19.7289999999999">
          <xsl:value-of select="'BA116'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -20.1789999999999">
          <xsl:value-of select="'BA117'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -20.6289999999999">
          <xsl:value-of select="'BA118'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -21.0789999999999">
          <xsl:value-of select="'BA119'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -21.5289999999999">
          <xsl:value-of select="'BA120'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -21.9789999999999">
          <xsl:value-of select="'BA121'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -22.4289999999999">
          <xsl:value-of select="'BA122'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -22.8789999999999">
          <xsl:value-of select="'BA123'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -23.3289999999999">
          <xsl:value-of select="'BA124'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -23.7789999999999">
          <xsl:value-of select="'BA125'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -24.2289999999999">
          <xsl:value-of select="'BA126'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -24.6789999999999">
          <xsl:value-of select="'BA127'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -25.1289999999999">
          <xsl:value-of select="'BA128'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -25.5789999999999">
          <xsl:value-of select="'BA129'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -26.0289999999999">
          <xsl:value-of select="'BA130'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -26.4789999999999">
          <xsl:value-of select="'BA131'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -26.9289999999999">
          <xsl:value-of select="'BA132'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -27.3789999999999">
          <xsl:value-of select="'BA133'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -27.8289999999999">
          <xsl:value-of select="'BA134'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -28.2789999999999">
          <xsl:value-of select="'BA135'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -28.7289999999999">
          <xsl:value-of select="'BA136'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -29.1789999999999">
          <xsl:value-of select="'BA137'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -29.6289999999999">
          <xsl:value-of select="'BA138'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -30.0789999999999">
          <xsl:value-of select="'BA139'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -30.5289999999999">
          <xsl:value-of select="'BA140'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -30.9789999999999">
          <xsl:value-of select="'BA141'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -31.4289999999999">
          <xsl:value-of select="'BA142'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -31.8789999999999">
          <xsl:value-of select="'BA143'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -32.3289999999999">
          <xsl:value-of select="'BA144'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -32.7789999999999">
          <xsl:value-of select="'BA145'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -33.2289999999999">
          <xsl:value-of select="'BA146'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -33.6789999999999">
          <xsl:value-of select="'BA147'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -34.1289999999999">
          <xsl:value-of select="'BA148'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -34.5789999999999">
          <xsl:value-of select="'BA149'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -35.0289999999999">
          <xsl:value-of select="'BA150'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -35.4789999999999">
          <xsl:value-of select="'BA151'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -35.929">
          <xsl:value-of select="'BA152'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -36.379">
          <xsl:value-of select="'BA153'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -36.829">
          <xsl:value-of select="'BA154'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -37.279">
          <xsl:value-of select="'BA155'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -37.729">
          <xsl:value-of select="'BA156'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -38.179">
          <xsl:value-of select="'BA157'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -38.629">
          <xsl:value-of select="'BA158'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -39.079">
          <xsl:value-of select="'BA159'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -39.529">
          <xsl:value-of select="'BA160'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -39.979">
          <xsl:value-of select="'BA161'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -40.429">
          <xsl:value-of select="'BA162'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -40.879">
          <xsl:value-of select="'BA163'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -41.329">
          <xsl:value-of select="'BA164'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -41.779">
          <xsl:value-of select="'BA165'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -42.229">
          <xsl:value-of select="'BA166'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -42.679">
          <xsl:value-of select="'BA167'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -43.129">
          <xsl:value-of select="'BA168'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -43.579">
          <xsl:value-of select="'BA169'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -44.029">
          <xsl:value-of select="'BA170'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -44.479">
          <xsl:value-of select="'BA171'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -44.929">
          <xsl:value-of select="'BA172'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -45.379">
          <xsl:value-of select="'BA173'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -45.829">
          <xsl:value-of select="'BA174'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -46.279">
          <xsl:value-of select="'BA175'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -46.729">
          <xsl:value-of select="'BA176'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -47.179">
          <xsl:value-of select="'BA177'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -47.629">
          <xsl:value-of select="'BA178'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -48.079">
          <xsl:value-of select="'BA179'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -48.529">
          <xsl:value-of select="'BA180'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -48.979">
          <xsl:value-of select="'BA181'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -49.429">
          <xsl:value-of select="'BA182'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -49.879">
          <xsl:value-of select="'BA183'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -50.329">
          <xsl:value-of select="'BA184'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -50.779">
          <xsl:value-of select="'BA185'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -51.229">
          <xsl:value-of select="'BA186'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -51.6790000000001">
          <xsl:value-of select="'BA187'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -52.1290000000001">
          <xsl:value-of select="'BA188'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -52.5790000000001">
          <xsl:value-of select="'BA189'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -53.0290000000001">
          <xsl:value-of select="'BA190'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -53.4790000000001">
          <xsl:value-of select="'BA191'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -53.9290000000001">
          <xsl:value-of select="'BA192'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -54.3790000000001">
          <xsl:value-of select="'BA193'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -54.8290000000001">
          <xsl:value-of select="'BA194'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -55.2790000000001">
          <xsl:value-of select="'BA195'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -55.7290000000001">
          <xsl:value-of select="'BA196'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -56.1790000000001">
          <xsl:value-of select="'BA197'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -56.6290000000001">
          <xsl:value-of select="'BA198'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -57.0790000000001">
          <xsl:value-of select="'BA199'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -57.5290000000001">
          <xsl:value-of select="'BA200'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -57.9790000000001">
          <xsl:value-of select="'BA201'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -58.4290000000001">
          <xsl:value-of select="'BA202'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -58.8790000000001">
          <xsl:value-of select="'BA203'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -59.3290000000001">
          <xsl:value-of select="'BA204'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -59.7790000000001">
          <xsl:value-of select="'BA205'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -60.2290000000001">
          <xsl:value-of select="'BA206'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -60.6790000000001">
          <xsl:value-of select="'BA207'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -61.1290000000001">
          <xsl:value-of select="'BA208'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -61.5790000000001">
          <xsl:value-of select="'BA209'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -62.0290000000001">
          <xsl:value-of select="'BA210'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -62.4790000000001">
          <xsl:value-of select="'BA211'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -62.9290000000001">
          <xsl:value-of select="'BA212'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -63.3790000000001">
          <xsl:value-of select="'BA213'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -63.8290000000001">
          <xsl:value-of select="'BA214'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -64.2790000000001">
          <xsl:value-of select="'BA215'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -64.7290000000001">
          <xsl:value-of select="'BA216'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -65.1790000000001">
          <xsl:value-of select="'BA217'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -65.6290000000001">
          <xsl:value-of select="'BA218'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -66.0790000000001">
          <xsl:value-of select="'BA219'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -66.5290000000001">
          <xsl:value-of select="'BA220'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -66.9790000000001">
          <xsl:value-of select="'BA221'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -67.4290000000001">
          <xsl:value-of select="'BA222'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -67.8790000000001">
          <xsl:value-of select="'BA223'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -68.3290000000001">
          <xsl:value-of select="'BA224'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -68.7790000000002">
          <xsl:value-of select="'BA225'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -69.2290000000002">
          <xsl:value-of select="'BA226'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -69.6790000000002">
          <xsl:value-of select="'BA227'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -70.1290000000002">
          <xsl:value-of select="'BA228'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -70.5790000000002">
          <xsl:value-of select="'BA229'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -71.0290000000002">
          <xsl:value-of select="'BA230'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -71.4790000000002">
          <xsl:value-of select="'BA231'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -71.9290000000002">
          <xsl:value-of select="'BA232'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -72.3790000000002">
          <xsl:value-of select="'BA233'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -72.8290000000002">
          <xsl:value-of select="'BA234'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -73.2790000000002">
          <xsl:value-of select="'BA235'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -73.7290000000002">
          <xsl:value-of select="'BA236'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -74.1790000000002">
          <xsl:value-of select="'BA237'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -74.6290000000002">
          <xsl:value-of select="'BA238'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -75.0790000000002">
          <xsl:value-of select="'BA239'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -75.5290000000002">
          <xsl:value-of select="'BA240'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -75.9790000000002">
          <xsl:value-of select="'BA241'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -76.4290000000002">
          <xsl:value-of select="'BA242'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -76.8790000000002">
          <xsl:value-of select="'BA243'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -77.3290000000002">
          <xsl:value-of select="'BA244'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -77.7790000000002">
          <xsl:value-of select="'BA245'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -78.2290000000002">
          <xsl:value-of select="'BA246'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -78.6790000000002">
          <xsl:value-of select="'BA247'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -79.1290000000002">
          <xsl:value-of select="'BA248'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -79.5790000000002">
          <xsl:value-of select="'BA249'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -80.0290000000002">
          <xsl:value-of select="'BA250'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -80.4790000000002">
          <xsl:value-of select="'BA251'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -80.9290000000002">
          <xsl:value-of select="'BA252'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -81.3790000000002">
          <xsl:value-of select="'BA253'"/>
        </xsl:when>
        <xsl:when test="$Elevation > -81.8290000000002">
          <xsl:value-of select="'BA254'"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="'BA255'"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:choose>
      <xsl:when test="elevation and number(elevation) = number(elevation)">
        <areaInstruction>
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
          <!--<symbol reference="SAFCON01" rotation="$surfaceCurrentDirection"/>-->
          <colorFill>
            <color>
              <xsl:value-of select="$Color"/>
            </color>
          </colorFill>
        </areaInstruction>
      </xsl:when>
    </xsl:choose>
    
  </xsl:template>
</xsl:transform>