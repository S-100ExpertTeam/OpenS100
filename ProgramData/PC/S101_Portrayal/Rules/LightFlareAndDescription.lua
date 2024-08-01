-- LightFlareAndDescription portrayal rules file.
-- #62
-- #170

-- Referenced CSPs.
require 'LITDSN02'

-- Main entry point for feature type.
function LightFlareAndDescription(feature, featurePortrayal, contextParameters, categoryOfLight, viewingGroup, priority)
	if feature.PrimitiveType ~= PrimitiveType.Point then
		error(feature.Code .. ' must be of spatial type point')
	end

	-- Colour 12 == magenta

	local colour = { [1] = 12 }

	if feature.colour[1] then
		colour = feature.colour
	end

	local rotation = 135

	if feature.flareBearing
	then
		rotation = feature.flareBearing
	else
		if contains(colour[1], { 1, 6, 11 }) then
			-- White, yellow or orange light.  Flare at 45 degrees if there is a colocated non-sectored light.
			for pointAssociation in feature:GetFlattenedSpatialAssociations() do
				local associatedFeatures = pointAssociation.AssociatedFeatures

				for _, af in ipairs(associatedFeatures) do
					if af ~= feature and contains(af.Code, { 'LightAllAround', 'LightAirObstruction', 'LightFogDetector' }) then
						rotation = 45
						break
					end
				end
			end
		end
	end

	local symbol = 'LITDEF11'

	if colour[1] == 1 then
		if colour[2] == 3 then
			-- White and red
			symbol = 'LIGHTS11'
		elseif colour[2] == 4 then
			-- White and green
			symbol = 'LIGHTS12'
		else
			-- White, yellow or orange
			symbol = 'LIGHTS13'
		end
	elseif (colour[1] == 5 and colour[2] == 6) then
		-- Blue and yellow
		symbol = 'LIGHTS13'
	elseif colour[1] == 3 then
		-- Red
		symbol = 'LIGHTS11'
	elseif colour[1] == 4 then
		-- Green
		symbol = 'LIGHTS12'
	elseif contains(colour[1], { 11, 6, 1 }) then
		-- White, yellow or orange
		symbol = 'LIGHTS13'
	end

	featurePortrayal:AddInstructions('Rotation:PortrayalCRS,' .. rotation)
	featurePortrayal:AddInstructions('PointInstruction:' .. symbol)

	featurePortrayal:AddInstructions('Rotation:PortrayalCRS,0;FontColor:CHBLK')

	if rotation == 45 then
		featurePortrayal:AddInstructions('LocalOffset:7.02,3.51;TextAlignHorizontal:Start;TextAlignVertical:Bottom')
	else
		featurePortrayal:AddInstructions('LocalOffset:7.02,0;TextAlignHorizontal:Start;TextAlignVertical:Center')
	end

	local description = LITDSN02(categoryOfLight, feature.rhythmOfLight, feature.colour, feature.height, feature['!valueOfNominalRange'], feature.status)
	-- issue #52, Call Activated [Signal Generation] (row 40 main)
	-- dataset 101GB005X01NE.000 will trigger
	--Debug.Break()
	--feature.signalGeneration = 5

	if contains(feature['!signalGeneration'], {3,5,6}) then
		--Debug.Break()
		featurePortrayal:AddTextInstruction(description .. '(man)', 23, 24, viewingGroup, priority, true)
	else
		featurePortrayal:AddTextInstruction(description, 23, 24, viewingGroup, priority, true)
	end
	-- end issue #52
end
