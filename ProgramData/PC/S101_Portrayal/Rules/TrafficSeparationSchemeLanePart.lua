-- Referenced portrayal rules.
require 'RESTRN01'

-- Traffic separation scheme lane part main entry point.
function TrafficSeparationSchemeLanePart(feature, featurePortrayal, contextParameters)
	if feature.PrimitiveType == PrimitiveType.Surface and contextParameters.PlainBoundaries then
		if feature.orientationValue then
			featurePortrayal:AddInstructions('ViewingGroup:25010;DrawingPriority:18;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:TSSLPT51')
			featurePortrayal:AddInstructions('Rotation:PortrayalCRS,0')
			RESTRN01(feature, featurePortrayal, contextParameters)
		else
			featurePortrayal:AddInstructions('ViewingGroup:25010;DrawingPriority:18;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:CTNARE51')
			RESTRN01(feature, featurePortrayal, contextParameters)
		end
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		if feature.orientationValue then
			featurePortrayal:AddInstructions('ViewingGroup:25010;DrawingPriority:18;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('Rotation:GeographicCRS,' .. tostring(feature.orientationValue) .. '')
			featurePortrayal:AddInstructions('PointInstruction:TSSLPT51')
			featurePortrayal:AddInstructions('Rotation:PortrayalCRS,0')
			RESTRN01(feature, featurePortrayal, contextParameters)
		else
			featurePortrayal:AddInstructions('ViewingGroup:25010;DrawingPriority:18;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('AreaFillReference:TSSJCT02')
			featurePortrayal:AddInstructions('PointInstruction:CTNARE51')
			RESTRN01(feature, featurePortrayal, contextParameters)
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end

	local informations = feature:GetInformationAssociations('AdditionalInformation')
	local text

	if informations then
		for _, information in ipairs(informations) do
			if not text then
				local text = GetInformationText(information, contextParameters)

				if text then
					featurePortrayal:AddInstructions('LocalOffset:0,7.02;TextAlignHorizontal:Center;FontSize:10')
					featurePortrayal:AddTextInstruction(EncodeString(text), 24, 24, 25010, 18)
				end
			end
		end
	end

	return 25010
end
