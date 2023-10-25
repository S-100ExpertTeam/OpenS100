-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)

-- Fog signal main entry point.
function FogSignal(feature, featurePortrayal, contextParameters)
	local viewingGroup
	
	if feature.PrimitiveType == PrimitiveType.Point then
		-- Simplified and paper chart points use the same symbolization
		viewingGroup = 27080
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:27080;DrawingPriority:18;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:27080;DrawingPriority:18;DisplayPlane:UnderRADAR')
		end
		featurePortrayal:AddInstructions('PointInstruction:FOGSIG01')
	
		-- issue #52, Call Activated [Signal Generation] (row 40 main)	
		-- see dataset 1012J005X0001.000 for example
		-- debug feature.signalGeneration = 5
		if contains(feature.signalGeneration, {3,5,6}) then
		featurePortrayal:AddInstructions('LocalOffset:-14,-5;FontSize:10')
		featurePortrayal:AddTextInstruction('(man)', 23, 24, viewingGroup, 18)
	end
	-- end issue #52
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
