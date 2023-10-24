-- NIWC LANT Modified from: Crane portrayal dKart rules file.
-- Feature Catalogue Version: 1.0.1 (2021/11/03) and later
-- dKart improvment: Determine S-101 portrayal for Crane curve feature and visual conspicuous.
--
-- Issues: PSWG #75, PC #106
-- #165
-- #210
-- #256

-- Crane main entry point.
function Crane(feature, featurePortrayal, contextParameters)

	local viewingGroup = 32440
	local symbol = '1'
	local colour = 'LANDF'

	if feature.visualProminence == 1 then
		viewingGroup = 22220
		symbol = '3'
		colour = 'CHBLK'
	end
	
	-- #256
	if feature.inTheWater then
		viewingGroup = 12200
	end

	if feature.PrimitiveType == PrimitiveType.Point then
		-- Simplified and paper chart points use the same symbolization
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:'.. viewingGroup .. ';DrawingPriority:12;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:'.. viewingGroup .. ';DrawingPriority:12;DisplayPlane:UnderRADAR')
		end
		featurePortrayal:AddInstructions('PointInstruction:CRANES0' .. symbol)
	elseif feature.PrimitiveType == PrimitiveType.Surface  then 
		-- PlainBoundaries and Patterns border use the same symbolization
		featurePortrayal:AddInstructions('ViewingGroup:'.. viewingGroup .. ';DrawingPriority:12;DisplayPlane:UnderRADAR')
		featurePortrayal:AddInstructions('ColorFill:CHBRN')
		featurePortrayal:SimpleLineStyle('solid',0.32, colour)
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
	--elseif feature.PrimitiveType == PrimitiveType.Curve then
		-- dKart improvment: Determine S-101 portrayal for Crane curve feature.
		--featurePortrayal:AddInstructions('ViewingGroup:'.. viewingGroup ..';DrawingPriority:12;DisplayPlane:UnderRADAR')
		--featurePortrayal:SimpleLineStyle('solid',0.64,'LANDF')
		--featurePortrayal:AddInstructions('LineInstruction:_simple_;LinePlacement:Relative,0.5')
		--featurePortrayal:AddInstructions('PointInstruction:CRANES1' .. symbol)
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup

end
