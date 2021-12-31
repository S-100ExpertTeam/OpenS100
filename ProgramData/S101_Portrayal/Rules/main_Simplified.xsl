<?xml version="1.0" encoding="UTF-8"?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
			<xsl:output method="xml" encoding="UTF-8" indent="yes"/>
			<xsl:decimal-format name="dformat" decimal-separator="." grouping-separator=","/>
			
			<!--Include templates/rules for: attributes-->
			<xsl:include href="attributeRules.xsl"/>
			
			<!--Include templates/rules for: styles-->
			<xsl:include href="textStyle.xsl"/>
			<xsl:include href="simpleLineStyle.xsl"/>
			
			<!--Include templates/rules for: csps-->
			<xsl:include href="Obstruction_custom.xsl"/>
			<xsl:include href="UnderWaterAwashRock_custom.xsl"/>
			<xsl:include href="LightDescription.xsl"/>
			<xsl:include href="Lights.xsl"/>
			<xsl:include href="LightSectored_custom.xsl"/>
			<xsl:include href="topmarks.xsl"/>
			<xsl:include href="seabed01.xsl"/>
			<xsl:include href="natsur01.xsl"/>
			<xsl:include href="rescsp03.xsl"/>
			<xsl:include href="slcons04.xsl"/>
			<xsl:include href="depare04.xsl"/>
			<xsl:include href="Depcnt03.xsl"/>
			<xsl:include href="qualin03.xsl"/>
			<xsl:include href="quapnt02.xsl"/>
			<xsl:include href="safcon02.xsl"/>
			<xsl:include href="soundg02.xsl"/>
			<xsl:include href="sndfrm04.xsl"/>
			<xsl:include href="Wreck_custom.xsl"/>
			<xsl:include href="RestrictedArea_Custom.xsl"/>



      <xsl:include href="TextPlacement.xsl"/>
			<!--Include templates/rules for new S-101 features-->
			<xsl:include href="FoulGround_COMMON.xsl"/>
				<xsl:include href="FoulGround_PLAIN_BOUNDARIES.xsl"/>
				<xsl:include href="QualityOfBathymetricData_COMMON.xsl"/>
				
			<!--Include templates/rules for: Default-->
			<xsl:include href="Default_PLAIN_BOUNDARIES.xsl"/>
			<!--Include templates/rules for: Default-->
			<xsl:include href="Default_COMMON.xsl"/>
			<!--Include templates/rules for: AnchorageArea-->
			<xsl:include href="AnchorageArea_PLAIN_BOUNDARIES.xsl"/>
			<!--Include templates/rules for: AnchorageArea-->
			<xsl:include href="AnchorageArea_COMMON.xsl"/>
			<!--Include templates/rules for: AnchorBerth-->
			<xsl:include href="AnchorBerth_COMMON.xsl"/>
			<!--Include templates/rules for: AdministrationArea-->
			<xsl:include href="AdministrationArea_PLAIN_BOUNDARIES.xsl"/>
			<!--Include templates/rules for: AirportAirfield-->
			<xsl:include href="AirportAirfield_COMMON.xsl"/>
			<!--Include templates/rules for: ArchipelagicSeaLane-->
			<xsl:include href="ArchipelagicSeaLane_COMMON.xsl"/>
			<!--Include templates/rules for: ArchipelagicSeaLaneAxis-->
			<xsl:include href="ArchipelagicSeaLaneAxis_COMMON.xsl"/>
			<!--Include templates/rules for: BeaconCardinal-->
			<xsl:include href="BeaconCardinal_SIMPLIFIED.xsl"/>
			<!--Include templates/rules for: BeaconIsolatedDanger-->
			<xsl:include href="BeaconIsolatedDanger_SIMPLIFIED.xsl"/>
			<!--Include templates/rules for: BeaconLateral-->
			<xsl:include href="BeaconLateral_SIMPLIFIED.xsl"/>
			<!--Include templates/rules for: BeaconSafeWater-->
			<xsl:include href="BeaconSafeWater_SIMPLIFIED.xsl"/>
			<!--Include templates/rules for: BeaconSpecialPurposeGeneral-->
			<xsl:include href="BeaconSpecialPurposeGeneral_SIMPLIFIED.xsl"/>
			<!--Include templates/rules for: Berth-->
			<xsl:include href="Berth_COMMON.xsl"/>
			<!--Include templates/rules for: BuoyCardinal-->
			<xsl:include href="BuoyCardinal_SIMPLIFIED.xsl"/>
			<!--Include templates/rules for: BuoyInstallation-->
			<xsl:include href="BuoyInstallation_SIMPLIFIED.xsl"/>
			<!--Include templates/rules for: BuoyIsolatedDanger-->
			<xsl:include href="BuoyIsolatedDanger_SIMPLIFIED.xsl"/>
			<!--Include templates/rules for: BuoyLateral-->
			<xsl:include href="BuoyLateral_SIMPLIFIED.xsl"/>
			<!--Include templates/rules for: BuoySafeWater-->
			<xsl:include href="BuoySafeWater_SIMPLIFIED.xsl"/>
			<!--Include templates/rules for: BuoySpecialPurposeGeneral-->
			<xsl:include href="BuoySpecialPurposeGeneral_SIMPLIFIED.xsl"/>
			<!--Include templates/rules for: Bridge-->
			<xsl:include href="Bridge_COMMON.xsl"/>
			<!--Include templates/rules for: BuiltUpArea-->
			<xsl:include href="BuiltUpArea_COMMON.xsl"/>
			<!--Include templates/rules for: Building-->
			<xsl:include href="Building_COMMON.xsl"/>
			<!--Include templates/rules for: Canal-->
			<xsl:include href="Canal_COMMON.xsl"/>
			<!--Include templates/rules for: Causeway-->
			<xsl:include href="Causeway_COMMON.xsl"/>
			<!--Include templates/rules for: CableArea-->
			<xsl:include href="CableArea_PLAIN_BOUNDARIES.xsl"/>
			<!--Include templates/rules for: CableOverhead-->
			<xsl:include href="CableOverhead_COMMON.xsl"/>
			<!--Include templates/rules for: CableSubmarine-->
			<xsl:include href="CableSubmarine_COMMON.xsl"/>
			<!--Include templates/rules for: CoastguardStation-->
			<xsl:include href="CoastguardStation_COMMON.xsl"/>
			<!--Include templates/rules for: Checkpoint-->
			<xsl:include href="Checkpoint_COMMON.xsl"/>
			<!--Include templates/rules for: Coastline-->
			<xsl:include href="Coastline_COMMON.xsl"/>
			<!--Include templates/rules for: Conveyor-->
			<xsl:include href="Conveyor_COMMON.xsl"/>
			<!--Include templates/rules for: ContiguousZone-->
			<xsl:include href="ContiguousZone_COMMON.xsl"/>
			<!--Include templates/rules for: ContinentalShelfArea-->
			<xsl:include href="ContinentalShelfArea_COMMON.xsl"/>
			<!--Include templates/rules for: Crane-->
			<xsl:include href="Crane_COMMON.xsl"/>
			<!--Include templates/rules for: CautionArea-->
			<xsl:include href="CautionArea_PLAIN_BOUNDARIES.xsl"/>
			<!--Include templates/rules for: CautionArea-->
			<xsl:include href="CautionArea_COMMON.xsl"/>
			<!--Include templates/rules for: Landmark-->
			<xsl:include href="Landmark_COMMON.xsl"/>
			<!--Include templates/rules for: CargoTranshipmentArea-->
			<xsl:include href="CargoTranshipmentArea_PLAIN_BOUNDARIES.xsl"/>
			<!--Include templates/rules for: CargoTranshipmentArea-->
			<xsl:include href="CargoTranshipmentArea_COMMON.xsl"/>
			<!--Include templates/rules for: CurrentNonGravitational-->
			<xsl:include href="CurrentNonGravitational_COMMON.xsl"/>
			<!--Include templates/rules for: CustomZone-->
			<xsl:include href="CustomZone_COMMON.xsl"/>
			<!--Include templates/rules for: Dam-->
			<xsl:include href="Dam_COMMON.xsl"/>
			<!--Include templates/rules for: DepthArea-->
			<xsl:include href="DepthArea_COMMON.xsl"/>
			<!--Include templates/rules for: DepthContour-->
			<xsl:include href="DepthContour_COMMON.xsl"/>
			<!--Include templates/rules for: DistanceMark-->
			<xsl:include href="DistanceMark_COMMON.xsl"/>
			<!--Include templates/rules for: DumpingGround-->
			<xsl:include href="DumpingGround_PLAIN_BOUNDARIES.xsl"/>
			<!--Include templates/rules for: DumpingGround-->
			<xsl:include href="DumpingGround_COMMON.xsl"/>
			<!--Include templates/rules for: DockArea-->
			<xsl:include href="DockArea_COMMON.xsl"/>
			<!--Include templates/rules for: DredgedArea-->
			<xsl:include href="DredgedArea_COMMON.xsl"/>
			<!--Include templates/rules for: DryDock-->
			<xsl:include href="DryDock_COMMON.xsl"/>
			<!--Include templates/rules for: DeepWaterRouteCentreline-->
			<xsl:include href="DeepWaterRouteCentreline_COMMON.xsl"/>
			<!--Include templates/rules for: DeepWaterRoutePart-->
			<xsl:include href="DeepWaterRoutePart_PLAIN_BOUNDARIES.xsl"/>
			<!--Include templates/rules for: Dyke-->
			<xsl:include href="Dyke_COMMON.xsl"/>
			<!--Include templates/rules for: ExclusiveEconomicZone-->
			<xsl:include href="ExclusiveEconomicZone_COMMON.xsl"/>
			<!--Include templates/rules for: Fairway-->
			<xsl:include href="Fairway_PLAIN_BOUNDARIES.xsl"/>
			<!--Include templates/rules for: FerryRoute-->
			<xsl:include href="FerryRoute_PLAIN_BOUNDARIES.xsl"/>
			<!--Include templates/rules for: FerryRoute-->
			<xsl:include href="FerryRoute_COMMON.xsl"/>
			<!--Include templates/rules for: FloatingDock-->
			<xsl:include href="FloatingDock_COMMON.xsl"/>
			<!--Include templates/rules for: FenceWall-->
			<xsl:include href="FenceWall_COMMON.xsl"/>
			<!--Include templates/rules for: FogSignal-->
			<xsl:include href="FogSignal_COMMON.xsl"/>
			<!--Include templates/rules for: FortifiedStructure-->
			<xsl:include href="FortifiedStructure_COMMON.xsl"/>
			<!--Include templates/rules for: FreePortArea-->
			<xsl:include href="FreePortArea_COMMON.xsl"/>
			<!--Include templates/rules for: FishingFacility-->
			<xsl:include href="FishingFacility_PLAIN_BOUNDARIES.xsl"/>
			<!--Include templates/rules for: FishingFacility-->
			<xsl:include href="FishingFacility_COMMON.xsl"/>
			<!--Include templates/rules for: FishingGround-->
			<xsl:include href="FishingGround_COMMON.xsl"/>
			<!--Include templates/rules for: FisheryZone-->
			<xsl:include href="FisheryZone_COMMON.xsl"/>
			<!--Include templates/rules for: Gate-->
			<xsl:include href="Gate_COMMON.xsl"/>
			<!--Include templates/rules for: Gridiron-->
			<xsl:include href="Gridiron_COMMON.xsl"/>
			<!--Include templates/rules for: HarbourAreaAdministrative-->
			<xsl:include href="HarbourAreaAdministrative_PLAIN_BOUNDARIES.xsl"/>
			<!--Include templates/rules for: HarbourFacility-->
			<xsl:include href="HarbourFacility_COMMON.xsl"/>
			<!--Include templates/rules for: Hulk-->
			<xsl:include href="Hulk_COMMON.xsl"/>
			<!--Include templates/rules for: IceArea-->
			<xsl:include href="IceArea_COMMON.xsl"/>
			<!--Include templates/rules for: IncinerationArea-->
			<xsl:include href="IncinerationArea_PLAIN_BOUNDARIES.xsl"/>
			<!--Include templates/rules for: IncinerationArea-->
			<xsl:include href="IncinerationArea_COMMON.xsl"/>
			<!--Include templates/rules for: InshoreTrafficZone-->
			<xsl:include href="InshoreTrafficZone_PLAIN_BOUNDARIES.xsl"/>
			<!--Include templates/rules for: Lake-->
			<xsl:include href="Lake_COMMON.xsl"/>
			<!--Include templates/rules for: LightFloat-->
			<xsl:include href="LightFloat_SIMPLIFIED.xsl"/>
			<!--Include templates/rules for: LightVessel-->
			<xsl:include href="LightVessel_SIMPLIFIED.xsl"/>
			<!--Include templates/rules for: LandArea-->
			<xsl:include href="LandArea_COMMON.xsl"/>
			<!--Include templates/rules for: LandElevation-->
			<xsl:include href="LandElevation_COMMON.xsl"/>
			<!--Include templates/rules for: Landmark-->
			<xsl:include href="Landmark_SIMPLIFIED.xsl"/>
			<!--Include templates/rules for: LandRegion-->
			<xsl:include href="LandRegion_COMMON.xsl"/>
			<!--Include templates/rules for: LocalMagneticAnomaly-->
			<xsl:include href="LocalMagneticAnomaly_PLAIN_BOUNDARIES.xsl"/>
			<!--Include templates/rules for: LocalMagneticAnomaly-->
			<xsl:include href="LocalMagneticAnomaly_COMMON.xsl"/>
			<!--Include templates/rules for: LogPond-->
			<xsl:include href="LogPond_COMMON.xsl"/>
			<!--Include templates/rules for: LockBasin-->
			<xsl:include href="LockBasin_COMMON.xsl"/>
			<!--Include templates/rules for: MagneticVariation-->
			<xsl:include href="MagneticVariation_COMMON.xsl"/>
			<!--Include templates/rules for: MarineFarmCulture-->
			<xsl:include href="MarineFarmCulture_PLAIN_BOUNDARIES.xsl"/>
			<!--Include templates/rules for: MarineFarmCulture-->
			<xsl:include href="MarineFarmCulture_COMMON.xsl"/>
			<!--Include templates/rules for: MilitaryPracticeArea-->
			<xsl:include href="MilitaryPracticeArea_PLAIN_BOUNDARIES.xsl"/>
			<!--Include templates/rules for: MilitaryPracticeArea-->
			<xsl:include href="MilitaryPracticeArea_COMMON.xsl"/>
			<!--Include templates/rules for: MooringWarpingFacility-->
			<xsl:include href="MooringWarpingFacility_COMMON.xsl"/>
			<!--Include templates/rules for: M_ACCY-->
			<xsl:include href="M_ACCY_COMMON.xsl"/>
			<!--Include templates/rules for: DataCoverage-->
			<xsl:include href="DataCoverage_COMMON.xsl"/>
			<!--Include templates/rules for: M_CSCL-->
			<xsl:include href="M_CSCL_COMMON.xsl"/>
			<!--Include templates/rules for: M_HOPA-->
			<xsl:include href="M_HOPA_COMMON.xsl"/>
			<!--Include templates/rules for: M_NPUB-->
			<xsl:include href="M_NPUB_COMMON.xsl"/>
			<!--Include templates/rules for: NavigationalSystemOfMarks-->
			<xsl:include href="NavigationalSystemOfMarks_COMMON.xsl"/>
			<!--Include templates/rules for: LocalDirectionOfBuoyage-->
			<xsl:include href="LocalDirectionOfBuoyage_PLAIN_BOUNDARIES.xsl"/>
			<!--Include templates/rules for: M_QUAL-->
			<xsl:include href="M_QUAL_COMMON.xsl"/>
			<!--Include templates/rules for: SoundingDatum-->
			<xsl:include href="SoundingDatum_COMMON.xsl"/>
			<!--Include templates/rules for: M_SREL-->
			<xsl:include href="M_SREL_COMMON.xsl"/>
			<!--Include templates/rules for: verticalDatumOfData-->
			<xsl:include href="verticalDatumOfData_COMMON.xsl"/>
			<!--Include templates/rules for: NavigationLine-->
			<xsl:include href="NavigationLine_COMMON.xsl"/>
			<!--Include templates/rules for: Obstruction-->
			<xsl:include href="Obstruction_PLAIN_BOUNDARIES.xsl"/>
			<!--Include templates/rules for: Obstruction-->
			<xsl:include href="Obstruction_COMMON.xsl"/>
			<!--Include templates/rules for: OffshorePlatform-->
			<xsl:include href="OffshorePlatform_COMMON.xsl"/>
			<!--Include templates/rules for: OilBarrier-->
			<xsl:include href="OilBarrier_COMMON.xsl"/>
			<!--Include templates/rules for: OffshoreProductionArea-->
			<xsl:include href="OffshoreProductionArea_PLAIN_BOUNDARIES.xsl"/>
			<!--Include templates/rules for: PilotBoardingPlace-->
			<xsl:include href="PilotBoardingPlace_PLAIN_BOUNDARIES.xsl"/>
			<!--Include templates/rules for: PilotBoardingPlace-->
			<xsl:include href="PilotBoardingPlace_COMMON.xsl"/>
			<!--Include templates/rules for: Pile-->
			<xsl:include href="Pile_COMMON.xsl"/>
			<!--Include templates/rules for: SubmarinePipelineArea-->
			<xsl:include href="SubmarinePipelineArea_PLAIN_BOUNDARIES.xsl"/>
			<!--Include templates/rules for: SubmarinePipelineArea-->
			<xsl:include href="SubmarinePipelineArea_COMMON.xsl"/>
			<!--Include templates/rules for: PipelineOverhead-->
			<xsl:include href="PipelineOverhead_COMMON.xsl"/>
			<!--Include templates/rules for: PipelineSubmarineOnLand-->
			<xsl:include href="PipelineSubmarineOnLand_COMMON.xsl"/>
			<!--Include templates/rules for: Pontoon-->
			<xsl:include href="Pontoon_COMMON.xsl"/>
			<!--Include templates/rules for: PrecautionaryArea-->
			<xsl:include href="PrecautionaryArea_PLAIN_BOUNDARIES.xsl"/>
			<!--Include templates/rules for: PrecautionaryArea-->
			<xsl:include href="PrecautionaryArea_COMMON.xsl"/>
			<!--Include templates/rules for: ProductionStorageArea-->
			<xsl:include href="ProductionStorageArea_COMMON.xsl"/>
			<!--Include templates/rules for: PylonBridgeSupport-->
			<xsl:include href="PylonBridgeSupport_COMMON.xsl"/>
			<!--Include templates/rules for: RadarLine-->
			<xsl:include href="RadarLine_COMMON.xsl"/>
			<!--Include templates/rules for: RadarReflector-->
			<xsl:include href="RadarReflector_COMMON.xsl"/>
			<!--Include templates/rules for: RadarRange-->
			<xsl:include href="RadarRange_COMMON.xsl"/>
			<!--Include templates/rules for: RadarStation-->
			<xsl:include href="RadarStation_COMMON.xsl"/>
			<!--Include templates/rules for: Railway-->
			<xsl:include href="Railway_COMMON.xsl"/>
			<!--Include templates/rules for: Rapids-->
			<xsl:include href="Rapids_COMMON.xsl"/>
			<!--Include templates/rules for: RecommendedRouteCentreline-->
			<xsl:include href="RecommendedRouteCentreline_COMMON.xsl"/>
			<!--Include templates/rules for: RecommendedTrafficLanePart-->
			<xsl:include href="RecommendedTrafficLanePart_COMMON.xsl"/>
			<!--Include templates/rules for: RadioCallingInPoint-->
			<xsl:include href="RadioCallingInPoint_COMMON.xsl"/>
			<!--Include templates/rules for: RadioStation-->
			<xsl:include href="RadioStation_COMMON.xsl"/>
			<!--Include templates/rules for: RecommendedTrack-->
			<xsl:include href="RecommendedTrack_PLAIN_BOUNDARIES.xsl"/>
			<!--Include templates/rules for: RecommendedTrack-->
			<xsl:include href="RecommendedTrack_COMMON.xsl"/>
			<!--Include templates/rules for: RestrictedArea-->
			<xsl:include href="RestrictedArea_PLAIN_BOUNDARIES.xsl"/>
			<!--Include templates/rules for: RestrictedArea-->
			<xsl:include href="RestrictedArea_COMMON.xsl"/>
			<!--Include templates/rules for: RetroReflector-->
			<xsl:include href="RetroReflector_SIMPLIFIED.xsl"/>
			<!--Include templates/rules for: River-->
			<xsl:include href="River_COMMON.xsl"/>
			<!--Include templates/rules for: Road-->
			<xsl:include href="Road_COMMON.xsl"/>
			<!--Include templates/rules for: RescueStation-->
			<xsl:include href="RescueStation_COMMON.xsl"/>
			<!--Include templates/rules for: RadarTransponderBeacon-->
			<xsl:include href="RadarTransponderBeacon_COMMON.xsl"/>
			<!--Include templates/rules for: Runway-->
			<xsl:include href="Runway_COMMON.xsl"/>
			<!--Include templates/rules for: SeabedArea-->
			<xsl:include href="SeabedArea_COMMON.xsl"/>
			<!--Include templates/rules for: SeaAreaNamedWaterArea-->
			<xsl:include href="SeaAreaNamedWaterArea_COMMON.xsl"/>
			<!--Include templates/rules for: SiloTank-->
			<xsl:include href="SiloTank_COMMON.xsl"/>
			<!--Include templates/rules for: SignalStationTraffic-->
			<xsl:include href="SignalStationTraffic_COMMON.xsl"/>
			<!--Include templates/rules for: SignalStationWarning-->
			<xsl:include href="SignalStationWarning_COMMON.xsl"/>
			<!--Include templates/rules for: ShorelineConstruction-->
			<xsl:include href="ShorelineConstruction_COMMON.xsl"/>
			<!--Include templates/rules for: SlopingGround-->
			<xsl:include href="SlopingGround_COMMON.xsl"/>
			<!--Include templates/rules for: SlopeTopline-->
			<xsl:include href="SlopeTopline_COMMON.xsl"/>
			<!--Include templates/rules for: SmallCraftFacility-->
			<xsl:include href="SmallCraftFacility_COMMON.xsl"/>
			<!--Include templates/rules for: Sandwave-->
			<xsl:include href="Sandwave_PLAIN_BOUNDARIES.xsl"/>
			<!--Include templates/rules for: Sandwave-->
			<xsl:include href="Sandwave_COMMON.xsl"/>
			<!--Include templates/rules for: Sounding-->
			<xsl:include href="Sounding_COMMON.xsl"/>
			<!--Include templates/rules for: SeaplaneLandingArea-->
			<xsl:include href="SeaplaneLandingArea_PLAIN_BOUNDARIES.xsl"/>
			<!--Include templates/rules for: SeaplaneLandingArea-->
			<xsl:include href="SeaplaneLandingArea_COMMON.xsl"/>
			<!--Include templates/rules for: Spring-->
			<xsl:include href="Spring_COMMON.xsl"/>
			<!--Include templates/rules for: StraightTerritorialSeaBaseline-->
			<xsl:include href="StraightTerritorialSeaBaseline_COMMON.xsl"/>
			<!--Include templates/rules for: SubmarineTransitLane-->
			<xsl:include href="SubmarineTransitLane_PLAIN_BOUNDARIES.xsl"/>
			<!--Include templates/rules for: SweptArea-->
			<xsl:include href="SweptArea_PLAIN_BOUNDARIES.xsl"/>
			<!--Include templates/rules for: TerritorialSeaArea-->
			<xsl:include href="TerritorialSeaArea_COMMON.xsl"/>
			<!--Include templates/rules for: Tideway-->
			<xsl:include href="Tideway_COMMON.xsl"/>
			<!--Include templates/rules for: TrafficSeparationLine-->
			<xsl:include href="TrafficSeparationLine_COMMON.xsl"/>
			<!--Include templates/rules for: TrafficSeparationZone-->
			<xsl:include href="TrafficSeparationZone_COMMON.xsl"/>
			<!--Include templates/rules for: TrafficSeparationSchemeBoundary-->
			<xsl:include href="TrafficSeparationSchemeBoundary_COMMON.xsl"/>
			<!--Include templates/rules for: TrafficSeparationSchemeCrossing-->
			<xsl:include href="TrafficSeparationSchemeCrossing_COMMON.xsl"/>
			<!--Include templates/rules for: TrafficSeparationSchemeLanePart-->
			<xsl:include href="TrafficSeparationSchemeLanePart_PLAIN_BOUNDARIES.xsl"/>
			<!--Include templates/rules for: TrafficSeparationSchemeLanePart-->
			<xsl:include href="TrafficSeparationSchemeLanePart_COMMON.xsl"/>
			<!--Include templates/rules for: TrafficSeparationSchemeRoundabout-->
			<xsl:include href="TrafficSeparationSchemeRoundabout_PLAIN_BOUNDARIES.xsl"/>
			<!--Include templates/rules for: TidalStreamFloodEbb-->
			<xsl:include href="TidalStreamFloodEbb_PLAIN_BOUNDARIES.xsl"/>
			<!--Include templates/rules for: TidalStreamFloodEbb-->
			<xsl:include href="TidalStreamFloodEbb_COMMON.xsl"/>
			<!--Include templates/rules for: TidalStreamPanelData-->
			<xsl:include href="TidalStreamPanelData_PLAIN_BOUNDARIES.xsl"/>
			<!--Include templates/rules for: TidalStreamPanelData-->
			<xsl:include href="TidalStreamPanelData_COMMON.xsl"/>
			<!--Include templates/rules for: TS_PNH-->
			<xsl:include href="TS_PNH_PLAIN_BOUNDARIES.xsl"/>
			<!--Include templates/rules for: TS_PNH-->
			<xsl:include href="TS_PNH_COMMON.xsl"/>
			<!--Include templates/rules for: TS_PRH-->
			<xsl:include href="TS_PRH_PLAIN_BOUNDARIES.xsl"/>
			<!--Include templates/rules for: TS_PRH-->
			<xsl:include href="TS_PRH_COMMON.xsl"/>
			<!--Include templates/rules for: TS_TIS-->
			<xsl:include href="TS_TIS_PLAIN_BOUNDARIES.xsl"/>
			<!--Include templates/rules for: TS_TIS-->
			<xsl:include href="TS_TIS_COMMON.xsl"/>
			<!--Include templates/rules for: Tunnel-->
			<xsl:include href="Tunnel_COMMON.xsl"/>
			<!--Include templates/rules for: TwoWayRoutePart-->
			<xsl:include href="TwoWayRoutePart_PLAIN_BOUNDARIES.xsl"/>
			<!--Include templates/rules for: T_HMON-->
			<xsl:include href="T_HMON_PLAIN_BOUNDARIES.xsl"/>
			<!--Include templates/rules for: T_HMON-->
			<xsl:include href="T_HMON_COMMON.xsl"/>
			<!--Include templates/rules for: T_NHMN-->
			<xsl:include href="T_NHMN_PLAIN_BOUNDARIES.xsl"/>
			<!--Include templates/rules for: T_NHMN-->
			<xsl:include href="T_NHMN_COMMON.xsl"/>
			<!--Include templates/rules for: T_TIMS-->
			<xsl:include href="T_TIMS_PLAIN_BOUNDARIES.xsl"/>
			<!--Include templates/rules for: T_TIMS-->
			<xsl:include href="T_TIMS_COMMON.xsl"/>
			<!--Include templates/rules for: UnsurveyedArea-->
			<xsl:include href="UnsurveyedArea_COMMON.xsl"/>
			<!--Include templates/rules for: Vegetation-->
			<xsl:include href="Vegetation_COMMON.xsl"/>
			<!--Include templates/rules for: Waterfall-->
			<xsl:include href="Waterfall_COMMON.xsl"/>
			<!--Include templates/rules for: WaterTurbulence-->
			<xsl:include href="WaterTurbulence_COMMON.xsl"/>
			<!--Include templates/rules for: WeedKelp-->
			<xsl:include href="WeedKelp_COMMON.xsl"/>
			<!--Include templates/rules for: Wreck-->
			<xsl:include href="Wreck_PLAIN_BOUNDARIES.xsl"/>
			<!--Include templates/rules for: Wreck-->
			<xsl:include href="Wreck_COMMON.xsl"/>
			<!--Include templates/rules for: dnghlt-->
			<xsl:include href="dnghlt_COMMON.xsl"/>
			<!--Include templates/rules for: indhlt-->
			<xsl:include href="indhlt_COMMON.xsl"/>
			<!--Include templates/rules for: marfea-->
			<xsl:include href="marfea_COMMON.xsl"/>
			<!--Include templates/rules for: marnot-->
			<xsl:include href="marnot_COMMON.xsl"/>
			<!--Include templates/rules for: mnufea-->
			<xsl:include href="mnufea_PLAIN_BOUNDARIES.xsl"/>
      <!--Include templates/rules for: information area-->
      <xsl:include href="informationArea_COMMON.xsl"/>
  <!--Include templates/rules for: Quality of Survey-->
  <xsl:include href="qualityOfSurvey_COMMON.xsl"/>


  <xsl:param name="TWO_SHADES">true</xsl:param>
			<xsl:param name="SAFETY_DEPTH" select="30.0"/>
			<xsl:param name="SAFETY_CONTOUR" select="30.0"/>
			<xsl:param name="SHALLOW_CONTOUR" select="2.0"/>
			<xsl:param name="DEEP_CONTOUR" select="30.0"/>
			<xsl:param name="SHALLOW_PATTERN">false</xsl:param>
			<xsl:param name="SHOW_ISOLATED_DANGERS_IN_SHALLOW_WATERS">false</xsl:param>
			<xsl:param name="FULL_SECTORS">false</xsl:param>
			
			<xsl:template match="/">
				<displayList>
					<xsl:apply-templates select="Dataset/Features/*"/>
					</displayList>
				</xsl:template>
			</xsl:transform>
		
