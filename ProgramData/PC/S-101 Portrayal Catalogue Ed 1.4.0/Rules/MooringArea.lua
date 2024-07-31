-- Referenced portrayal rules.
require 'RESTRN01'

	--
	-- #309
	-- #344 bug fix against #309
	--

function MooringArea(feature, featurePortrayal, contextParameters)

	local viewingGroup = 26220

	featurePortrayal:AddInstructions('AlertReference:ProhAre,53023,53023')

	-- #309, "Visitors" or "Tankers" string placement to add to text below symbol
	local function AddVisitorTankerName(feature, localOffset)

		-- 1 : Small craft mooring area, 2 : mooring area for visitors, 3 : mooring area for tankers
		if contains(feature.categoryOfMooringArea, {1, 2, 3}) then
			local labels = {}
			
			if contains(1, feature.categoryOfMooringArea) then
				labels[#labels + 1] = "Small Crafts"
			end
			if contains(2, feature.categoryOfMooringArea) then
				labels[#labels + 1] = "Visitors"
			end
			if contains(3, feature.categoryOfMooringArea) then
				labels[#labels + 1] = "Tankers"
			end
			
			featurePortrayal:AddInstructions('LocalOffset:' .. localOffset .. ';TextAlignHorizontal:Center;TextAlignVertical:Top;FontSlant:Italics;FontColor:CHGRF')
			featurePortrayal:AddTextInstruction(safeConcat(labels, ","), 24, 24, viewingGroup, 18)
		end
	end	

	if feature.PrimitiveType == PrimitiveType.Point then
		-- Simplified and paper chart points use the same symbolization
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:26220;DrawingPriority:18;DisplayPlane:OverRadar')
		else
			featurePortrayal:AddInstructions('ViewingGroup:26220;DrawingPriority:18;DisplayPlane:OverRadar')
		end

		featurePortrayal:AddInstructions('PointInstruction:BOYMOR02')

		if feature.featureName[1] and feature.featureName[1].name then
			featurePortrayal:AddInstructions('LocalOffset:3,2;FontColor:CHBLK')
	    	featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters)), 26, 24, viewingGroup, 18)
		end

		AddVisitorTankerName(feature, '0,-2')

	elseif feature.PrimitiveType == PrimitiveType.Surface and contextParameters.PlainBoundaries then
		featurePortrayal:AddInstructions('ViewingGroup:26220;DrawingPriority:9;DisplayPlane:UnderRadar')
		featurePortrayal:AddInstructions('PointInstruction:BOYMOR51')
		featurePortrayal:SimpleLineStyle('dash',0.64,'CHGRF')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')

		RESTRN01(feature, featurePortrayal, contextParameters, viewingGroup)
		
		if feature.featureName[1] and feature.featureName[1].name then
			featurePortrayal:AddInstructions('LocalOffset:5,2;FontColor:CHBLK')
	    	featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters)), 26, 24, viewingGroup, 9)
		end

		AddVisitorTankerName(feature, '0,-6')
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		featurePortrayal:AddInstructions('ViewingGroup:26220;DrawingPriority:9;DisplayPlane:UnderRadar')
		featurePortrayal:AddInstructions('PointInstruction:BOYMOR51')
		featurePortrayal:AddInstructions('LineInstruction:NAVARE52')

		RESTRN01(feature, featurePortrayal, contextParameters, viewingGroup)

		if feature.featureName[1] and feature.featureName[1].name then
			featurePortrayal:AddInstructions('LocalOffset:5,2;FontColor:CHBLK')
	    	featurePortrayal:AddTextInstruction(EncodeString(GetFeatureName(feature, contextParameters)), 26, 24, viewingGroup, 9)
		end

		AddVisitorTankerName(feature, '0,-6')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end
	
	return viewingGroup
end
