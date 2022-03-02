-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2019/4/9)

-- Unsurveyed area main entry point.
function UnsurveyedArea(feature, featurePortrayal, contextParameters)
	featurePortrayal:AddInstructions('AlertReference:SafetyContour,101,101')

	if feature.PrimitiveType == PrimitiveType.Surface then
		-- Plain and symbolized boundaries use the same symbolization
		featurePortrayal:AddInstructions('ViewingGroup:11050;DrawingPriority:3;DisplayPlane:UnderRADAR')
		featurePortrayal:AddInstructions('ColorFill:NODTA')
		featurePortrayal:AddInstructions('AreaFillReference:NODATA03')
		featurePortrayal:SimpleLineStyle('solid',0.64,'CHGRD')
		featurePortrayal:AddInstructions('LineInstruction:_simple_')
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end
end
