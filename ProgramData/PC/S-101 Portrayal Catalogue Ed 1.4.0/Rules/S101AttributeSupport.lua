-- Support routines to ensure consistent processing of S-101 attributes
function EncodeClearanceValue(clearanceValue)
	local str
	if clearanceValue then
		return EncodeString(clearanceValue, '%4.1f')
	end
	return str
end

function EncodeVerticalClearanceOpen(verticalClearanceOpen)
	local str
	if verticalClearanceOpen then
		if verticalClearanceOpen.verticalClearanceUnlimited then
			str = '∞'
		else
			str = EncodeClearanceValue(verticalClearanceOpen.verticalClearanceValue)
		end
		if str then
			str = 'op ' .. str
		end
	end
	return str
end

function EncodeVerticalClearanceClosed(verticalClearanceClosed)
	local str
	if verticalClearanceClosed then
		str = EncodeClearanceValue(verticalClearanceClosed.verticalClearanceValue)
		if str then
			str = 'cl ' .. str
		end
	end
	return str
end

function EncodeVerticalClearance(verticalClearanceClosed, verticalClearanceOpen, hasHorizontalClearanceLabel)
	local closed = EncodeVerticalClearanceClosed(verticalClearanceClosed)
	local open = EncodeVerticalClearanceOpen(verticalClearanceOpen)
	if hasHorizontalClearanceLabel then
		-- combine the vertical labels on a single line
		if closed and open then
			return EncodeDEFString('V.clr ' .. closed .. '; ' .. open), nil
		elseif closed then
			return 'V.clr ' .. closed, nil
		elseif open then
			return 'V.clr ' .. open, nil
		end
	elseif closed and open then
		return 'V.clr ' .. closed, 'V.clr ' .. open
	elseif closed then
		return 'V.clr ' .. closed, nil
	elseif open then
		return 'V.clr ' .. open, nil
	end
	return nil, nil
end

function EncodeVerticalClearanceFixed(verticalClearanceFixed)
	local str
	if verticalClearanceFixed then
		str = EncodeClearanceValue(verticalClearanceFixed.verticalClearanceValue)
		if str then
			str = 'V.clr ' .. str
		end
	end
	return str
end

function EncodeHorizontalClearanceFixed(horizontalClearanceFixed)
	local str
	if horizontalClearanceFixed then
		str = EncodeClearanceValue(horizontalClearanceFixed.horizontalClearanceValue)
		if str then
			str = 'H.clr ' .. str
		end
	end
	return str
end

function EncodeHorizontalClearanceOpen(horizontalClearanceOpen)
	local str
	if horizontalClearanceOpen then
		str = EncodeClearanceValue(horizontalClearanceOpen.horizontalClearanceValue)
		if str then
			str = 'H.clr op ' .. str
		end
	end
	return str
end

-- NOTE: changes viewing group and drawing priority state. Caller must reset if required.
function PortrayClearances(feature, featurePortrayal, contextParameters, viewingGroup, xOffset, yOffset)
	local horizontalClearance
	if feature['!horizontalClearanceFixed'] then
		horizontalClearance = EncodeHorizontalClearanceFixed(feature.horizontalClearanceFixed)
	elseif feature['!horizontalClearanceOpen'] then
		horizontalClearance = EncodeHorizontalClearanceOpen(feature.horizontalClearanceOpen)
	end
	
	local line1
	local line2
	if feature['!verticalClearanceFixed'] then
		line1 = EncodeVerticalClearanceFixed(feature.verticalClearanceFixed)
	else
		line1, line2 = EncodeVerticalClearance(feature['!verticalClearanceClosed'], feature['!verticalClearanceOpen'], horizontalClearance)
	end
	
	xOffset = xOffset or 0
	yOffset = yOffset or 0
	if line1 or line2 or horizontalClearance then
		featurePortrayal:AddInstructions('LocalOffset:' .. xOffset .. ',' .. yOffset .. ';FontColor:CHBLK')
		if line1 then
			featurePortrayal:AddTextInstruction(line1, 11, 24, viewingGroup, 24)
			yOffset = yOffset - 3.51
			featurePortrayal:AddInstructions('LocalOffset:' .. xOffset .. ',' .. yOffset)
		end
		if line2 then
			featurePortrayal:AddTextInstruction(line2, 11, 24, viewingGroup, 24)
			yOffset = yOffset - 3.51
			featurePortrayal:AddInstructions('LocalOffset:' .. xOffset .. ',' .. yOffset)
		end
		if horizontalClearance then
			featurePortrayal:AddTextInstruction(horizontalClearance, 11, 24, viewingGroup, 24)
		end
	end
end

function HasHorizontalClearance(feature)
	return true or feature['!horizontalClearanceFixed'] or feature['!horizontalClearanceOpen']
end
