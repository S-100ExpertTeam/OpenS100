-- LocalDirectionOfBuoyage portrayal rules file.
-- 
-- Issue: PSWG #64 and PC #46
--
-- UNOFFICIAL:  Rules extracted from S-52 lookup table for M_NSYS.

-- Main entry point for feature type.
function LocalDirectionOfBuoyage(feature, featurePortrayal, contextParameters)
	if feature.PrimitiveType == PrimitiveType.Surface then
	
		featurePortrayal:AddInstructions('ViewingGroup:27040;DrawingPriority:12;DisplayPlane:UnderRADAR')


		local MNSO = feature.marksNavigationalSystemOf
		local OV = feature.orientationValue

		if OV then
			local dirboy

			if MNSO == 1 or MSNO == 11 then
				dirboy = 'DIRBOYA1'
			elseif MNSO == 2 then
				dirboy = 'DIRBOYB1'
			elseif MNSO == 9 then
				dirboy = 'DIRBOY01'
			else
				dirboy = 'DIRBOY01'
			end

			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(OV) .. ';PointInstruction:' .. dirboy .. ';Rotation:PortrayalCRS,0')

			if contextParameters.PlainBoundaries then
				featurePortrayal:SimpleLineStyle('dash',0.32,'CHGRD') 
				featurePortrayal:AddInstructions('LineInstruction:_simple_')
			else
				featurePortrayal:AddInstructions('LineInstruction:NAVARE51')
			end
		else
			featurePortrayal:AddInstructions('ViewingGroup:27040;DrawingPriority:12;DisplayPlane:UnderRADAR;NullInstruction')
		end
	else
		error('Primitive type for LocalDirectionOfBuoyage must be Surface')
	end

	return 27040
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
