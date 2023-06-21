-- LocalDirectionOfBuoyage portrayal rules file.

-- UNOFFICIAL:  Rules extracted from S-52 lookup table for M_NSYS.

-- Main entry point for feature type.
function LocalDirectionOfBuoyage(feature, featurePortrayal, contextParameters)
	if feature.PrimitiveType ~= PrimitiveType.Surface then
		error('Primitive type for LocalDirectionOfBuoyage must be Surface')
	end

	featurePortrayal:AddInstructions('ViewingGroup:27040;DrawingPriority:12;DisplayPlane:UnderRADAR')

	if feature.orientationValue then
		local dirboy

		if feature.marksNavigationalSystemOf == 1 then
			dirboy = 'DIRBOYA1'
		elseif feature.marksNavigationalSystemOf == 2 then
			dirboy = 'DIRBOYB1'
		elseif feature.marksNavigationalSystemOf == 11 then
			-- S-52 does not define symbol for CEVNI system of marks.
			dirboy = 'testPCB'
		else
			dirboy = 'DIRBOY01'
		end

		featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue) .. ';PointInstruction:' .. dirboy)

		if contextParameters.PlainBoundaries then
			featurePortrayal:SimpleLineStyle('dash',0.32,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:_simple_')
		else
			featurePortrayal:AddInstructions('LineInstruction:NAVARE51')
		end
	else
		featurePortrayal:AddInstructions('LineInstruction:MARSYS51')
	end
end

--[[
S-52 Lookup tables

Plain

"M_NSYS","","LC(MARSYS51)","4","S","STANDARD","27040"
"M_NSYS","MARSYS1ORIENT","SY(DIRBOYA1,ORIENT);LS(DASH,1,CHGRD)","4","S","STANDARD","27040"
"M_NSYS","MARSYS2ORIENT","SY(DIRBOYB1,ORIENT);LS(DASH,1,CHGRD)","4","S","STANDARD","27040"
"M_NSYS","ORIENT","SY(DIRBOY01,ORIENT);LS(DASH,1,CHGRD)","4","S","STANDARD","27040"

Symoblized

"M_NSYS","","LC(MARSYS51)","4","S","STANDARD","27040"
"M_NSYS","MARSYS1ORIENT","SY(DIRBOYA1,ORIENT);LC(NAVARE51)","4","S","STANDARD","27040"
"M_NSYS","MARSYS2ORIENT","SY(DIRBOYB1,ORIENT);LC(NAVARE51)","4","S","STANDARD","27040"
"M_NSYS","ORIENT","SY(DIRBOY01,ORIENT);LC(NAVARE51)","4","S","STANDARD","27040"
--]]
