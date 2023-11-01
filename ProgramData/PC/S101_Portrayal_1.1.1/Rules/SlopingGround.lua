-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)

-- Issue #161, PSWG #108, significant changes from conveted S-52

-- Sloping ground main entry point.
function SlopingGround(feature, featurePortrayal, contextParameters)
	local viewingGroup


	if feature.PrimitiveType == PrimitiveType.Point then
		-- Simplified and paper chart points use the same symbolization

		local symbol = 'HILTOP01'

		if feature.visualProminence == 1 or feature.radarConspicuous == true then
			symbol = 'HILTOP11'
		end

		viewingGroup = 32010
		featurePortrayal:AddInstructions('ViewingGroup:32010;DrawingPriority:9;DisplayPlane:UnderRADAR')
		featurePortrayal:AddInstructions('PointInstruction:' .. symbol)

	elseif feature.PrimitiveType == PrimitiveType.Surface then
	
		viewingGroup = 32010

		-- not Visual or Radar conspicuous
		local fillColor = 'LANDF'
		local lineColor = 'CHGRD'
	
		if  feature.visualProminence == 1 or feature.radarConspicuous == true then
				fillColor = 'CHGRD'
				lineColor = 'CHBLK'
		end

		featurePortrayal:AddInstructions('ViewingGroup:32010;DrawingPriority:9;DisplayPlane:UnderRADAR')
		featurePortrayal:AddInstructions('ColorFill:' .. fillColor)
		featurePortrayal:SimpleLineStyle('solid',0.32,lineColor)
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
	
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	
	return viewingGroup
end
