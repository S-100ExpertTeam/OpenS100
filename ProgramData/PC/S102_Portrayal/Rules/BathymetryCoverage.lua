-- BathymetryCoverage symbology rules file.

-- Main entry point for BathymetryCoverage.
function BathymetryCoverage(feature, featurePortrayal, contextParameters)
	featurePortrayal:AddInstructions('ViewingGroup:13030;DrawingPriority:3;DisplayPlane:UnderRadar')
	
	featurePortrayal:AddInstructions('CoverageColor:DEPIT,0;LookupEntry:Intertidal,,0,ltSemiInterval')
	if contextParameters.FourShades then
		featurePortrayal:AddInstructions('CoverageColor:DEPVS,0;LookupEntry:Shallow Water,0,' .. tostring(contextParameters.ShallowContour) .. ',geLtInterval')
		featurePortrayal:AddInstructions('CoverageColor:DEPMS,0;LookupEntry:Medium-Shallow Water,' .. tostring(contextParameters.ShallowContour) .. ',' .. tostring(contextParameters.SafetyContour) .. ',geLtInterval')
		featurePortrayal:AddInstructions('CoverageColor:DEPMD,0;LookupEntry:Medium-Deep Water,' .. tostring(contextParameters.SafetyContour) .. ',' .. tostring(contextParameters.DeepContour) .. ',geLtInterval')
		featurePortrayal:AddInstructions('CoverageColor:DEPDW,0;LookupEntry:Deep Water,'.. tostring(contextParameters.DeepContour) .. ',,geSemiInterval')
	else
		featurePortrayal:AddInstructions('CoverageColor:DEPVS,0;LookupEntry:Shallow Water,0,' .. tostring(contextParameters.SafetyContour) .. ',geLtInterval')
		featurePortrayal:AddInstructions('CoverageColor:DEPDW,0;LookupEntry:Deep Water,'.. tostring(contextParameters.SafetyContour) .. ',,geSemiInterval')
	end
	featurePortrayal:AddInstructions('CoverageFill:depth')
end
