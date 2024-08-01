-- UpdateInformation conditional symbology rules file.
-- #119
-- #192

-- Main entry point for CSP.
function UpdateInformation(feature, featurePortrayal, contextParameters)
	local viewingGroup = 90040
	local updateType = feature.updateType
	
	-- Place feature on top of everything except text
	featurePortrayal:AddInstructions('ViewingGroup:90040;DrawingPriority:30')

	if feature.PrimitiveType == PrimitiveType.Point then
		if contextParameters.RadarOverlay then
			featurePortrayal:AddInstructions('DisplayPlane:OverRadar')
		else
			featurePortrayal:AddInstructions('DisplayPlane:UnderRadar')
		end
		
		if updateType == 1 then
			-- insert
			featurePortrayal:AddInstructions('PointInstruction:CHRVID01')
		elseif updateType == 2 then
			-- delete
			featurePortrayal:AddInstructions('PointInstruction:CHRVDEL1')
		elseif updateType == 3 then
			-- modify
			featurePortrayal:AddInstructions('PointInstruction:CHRVDEL1;PointInstruction:CHRVID01')
		elseif updateType == 4 then
			-- move
			error('Point geometry is invalid for UpdateInformation with updateType=4')
		else
			-- undefined updateType
			error('UpdateInformation feature with undefined updateType')
		end
	elseif feature.PrimitiveType == PrimitiveType.Curve then
		featurePortrayal:AddInstructions('DisplayPlane:UnderRadar')
		if updateType == 1 then
			-- insert
			featurePortrayal:AddInstructions('LineInstructionUnsuppressed:CHRVID02')
		elseif updateType == 2 then
			-- delete
			featurePortrayal:AddInstructions('LineInstructionUnsuppressed:CHRVDEL2')
		elseif updateType == 3 then
			-- modify
			featurePortrayal:AddInstructions('LineInstructionUnsuppressed:CHRVDEL2')
			featurePortrayal:AddInstructions('LineInstructionUnsuppressed:CHRVID02')
		elseif updateType == 4 then
			-- move
			featurePortrayal:SimpleLineStyle('dash',0.32,'CHCOR')
			featurePortrayal:AddInstructions('LineInstructionUnsuppressed:_simple_')
		else
			-- undefined updateType
			error('UpdateInformation feature with undefined updateType')
		end
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		featurePortrayal:AddInstructions('DisplayPlane:UnderRadar')
		if updateType == 1 then
			-- insert
			featurePortrayal:AddInstructions('LineInstructionUnsuppressed:CHRVID02;PointInstruction:CHRVID01')
		elseif updateType == 2 then
			-- delete
			featurePortrayal:AddInstructions('LineInstructionUnsuppressed:CHRVDEL2;PointInstruction:CHRVDEL1')
		elseif updateType == 3 then
			-- modify
			featurePortrayal:AddInstructions('LineInstructionUnsuppressed:CHRVDEL2')
			featurePortrayal:AddInstructions('LineInstructionUnsuppressed:CHRVID02')
			featurePortrayal:AddInstructions('PointInstruction:CHRVID01')
			featurePortrayal:AddInstructions('PointInstruction:CHRVDEL1')
		elseif updateType == 4 then
			-- move
			error('Surface geometry is invalid for UpdateInformation with updateType=4')
		else
			-- undefined updateType
			error('UpdateInformation feature with undefined updateType')
		end
	elseif feature.PrimitiveType == PrimitiveType.None then
		if updateType ~= 3 and updateType ~= 4 then
			error('Invalid primitive type or mariner settings passed to portrayal')
		end
	else
		--  MultiPoint geometry is not valid for UpdateInformation
		error('Invalid primitive type or mariner settings passed to portrayal')
	end
	return viewingGroup
end
