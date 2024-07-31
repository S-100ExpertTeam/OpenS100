-- #92
-- #155

-- Radar station main entry point.
function RadarStation(feature, featurePortrayal, contextParameters)
	local viewingGroup = 38010

	if feature.PrimitiveType == PrimitiveType.Point then
		-- Simplified and paper chart points use the same symbolization
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:38010;DrawingPriority:15;DisplayPlane:OverRadar')
		else
			featurePortrayal:AddInstructions('ViewingGroup:38010;DrawingPriority:15;DisplayPlane:UnderRadar')
		end
		
		if contains(2, feature.categoryOfRadarStation) then
			-- Coast Radar Station
			local communicationChannels = ''
			local contactDetails = feature:GetInformationAssociation('AdditionalInformation', 'theInformation', 'ContactDetails')
			if contactDetails then
				communicationChannels = safeConcat(contactDetails.communicationChannel, ",") .. safeConcat(feature.communicationChannel, ",")
			else
				communicationChannels = safeConcat(feature.communicationChannel, ",")
			end
			if communicationChannels ~= '' then
				communicationChannels = 'ch ' .. communicationChannels
			end
		
			featurePortrayal:AddInstructions('PointInstruction:RDOSTA02')
			if communicationChannels ~= '' then
				featurePortrayal:AddInstructions('LocalOffset:0,0;FontSize:10;FontColor:CHBLK')
				featurePortrayal:AddTextInstruction(EncodeString(communicationChannels), 11, 24, 38010, 15)
			end
			
		else
			featurePortrayal:AddInstructions('PointInstruction:POSGEN01')
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
