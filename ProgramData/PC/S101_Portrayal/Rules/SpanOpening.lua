require 'S101AttributeSupport'

-- from S-57 BRIDGE, CATBRG=2 (opening bridge)
function SpanOpening(feature, featurePortrayal, contextParameters)
	featurePortrayal:AddInstructions('AlertReference:NavHazard')
	
	if feature.PrimitiveType == PrimitiveType.Curve then
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:24;DisplayPlane:OverRadar')
		else
			featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:24;DisplayPlane:UnderRadar')
		end
		featurePortrayal:SimpleLineStyle('solid',1.6,'CHGRD')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
		featurePortrayal:AddInstructions('PointInstruction:BRIDGE01')

	elseif feature.PrimitiveType == PrimitiveType.Surface then
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:24;DisplayPlane:OverRadar')
		else
			featurePortrayal:AddInstructions('ViewingGroup:12210;DrawingPriority:24;DisplayPlane:UnderRadar')
		end
		featurePortrayal:AddInstructions('PointInstruction:BRIDGE01')
		featurePortrayal:SimpleLineStyle('solid',1.28,'CHGRD')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end
	
	-- Binds horizontalClearanceFixed, verticalClearanceClosed, verticalClearanceOpen
	if HasClearance(feature) then
		if feature.PrimitiveType == PrimitiveType.Curve then
			featurePortrayal:AddInstructions('TextAlignHorizontal:Center;TextAlignVertical:Center')
			PortrayClearances(feature, featurePortrayal, contextParameters, 12210, 0, -3.51)
		else
			PortrayClearances(feature, featurePortrayal, contextParameters, 12210, 3.51, 0)
		end
	end

	return 12210
end
