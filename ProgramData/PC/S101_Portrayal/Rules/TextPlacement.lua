function TextPlacement(feature, featurePortrayal, contextParameters)
	-- NOTE
	-- TextPlacement can place a structure name without a direct association [Structure <=> Equipment <=> TextPlacement]
	--
	-- NOTE 2
	-- TextPlacement information is processed / stored when the feature(s) to which it is associated (directly or indirectly)
	-- are processed. The implementation is in PortrayalModel.lua, mostly as part of featurePortrayal:AddTextInstruction() and
	-- featurePortrayal:GetFeatureName(). This rule merely orders the previously stored drawing instructions and
	-- adds them to the TextPlacement's featurePortrayal. The feature(s) to which TextPlacement is directly or indirectly
	-- associated must be emitted prior to calling this routine.
	local offset = 0
	local function AddDrawingInstructions(tableName)
		for _, te in ipairs(rawget(feature, tableName)) do
			local lastIndex = #te[2]
			for index, di in ipairs(te[2]) do
				if index == lastIndex then
					-- Insert vertical offset
					if offset ~= 0 then
						featurePortrayal:AddInstructions('TextVerticalOffset:' .. offset)
					end
					offset = offset - 3.51
				end
				featurePortrayal:AddInstructions(di)
			end
		end
	end
	
	AddDrawingInstructions('_name')
	AddDrawingInstructions('_featureCharacteristics')
	AddDrawingInstructions('_lightCharacteristics')
	
	return -1
end
