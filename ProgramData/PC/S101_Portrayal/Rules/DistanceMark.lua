-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)
-- #65
-- #187

-- Distance mark main entry point.
function DistanceMark(feature, featurePortrayal, contextParameters)
	local viewingGroup

	if feature.PrimitiveType == PrimitiveType.Point then
		viewingGroup = 27030
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('ViewingGroup:27030;DrawingPriority:21;DisplayPlane:OverRADAR')
		else
			featurePortrayal:AddInstructions('ViewingGroup:27030;DrawingPriority:21;DisplayPlane:UnderRADAR')
		end
		local textColor = 'CHMGD'
		local structures = feature:GetFeatureAssociations('StructureEquipment')
		local noStructure = next(structures) == nil
		if noStructure then
			-- no StructureEquipment relationship, add a symbol
			featurePortrayal:AddInstructions('PointInstruction:DISMAR06')
		else
			-- TODO: assign viewing group from structure so that text turns off with symbol?
			textColor = 'CHBLK'
		end

		if feature.measuredDistanceValue
		then
			local label = ''
			
			local distanceUnitOfMeasurement = feature.measuredDistanceValue.distanceUnitOfMeasurement
			if distanceUnitOfMeasurement
			then
				if distanceUnitOfMeasurement == 1	then
					label = 'm'
				elseif distanceUnitOfMeasurement == 2 then
					label = 'yd'
				elseif distanceUnitOfMeasurement == 3 then
					label = 'km'
				elseif distanceUnitOfMeasurement == 4 then
					label = 'SM'
				elseif distanceUnitOfMeasurement == 5 then
					label = 'M'
				end
			end
			
			local waterwayDistance = feature.measuredDistanceValue.waterwayDistance
			if waterwayDistance
			then
				label = label .. tostring(waterwayDistance)
			end
			
			if noStructure then
				featurePortrayal:AddInstructions('LocalOffset:3.15,0')
			else
				featurePortrayal:AddInstructions('LocalOffset:-7.02,0;TextAlignHorizontal:End')
			end
		
			featurePortrayal:AddInstructions('TextAlignVertical:Center;FontWeight:Light;FontColor:' .. textColor)
			featurePortrayal:AddTextInstruction(EncodeString(label), 21, 24, viewingGroup, 21)
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	return viewingGroup
end
