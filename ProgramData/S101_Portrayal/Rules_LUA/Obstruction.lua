-- Converter Version: 0.99
-- Feature Catalogue Version: 1.0.0 (2018/9/6)

-- Referenced portrayal rules.
require 'OBSTRN07'

-- Obstruction main entry point.
function Obstruction(feature, featurePortrayal, contextParameters)
	if feature.PrimitiveType == PrimitiveType.Point and contextParameters.SIMPLIFIED_POINTS then
		if feature.categoryOfObstruction == 7 and feature.valueOfSounding then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:34051;DrawingPriority:4;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:34051;DrawingPriority:4;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:FOULGND1')
		elseif feature.categoryOfObstruction == 8 and feature.valueOfSounding then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:34051;DrawingPriority:4;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:34051;DrawingPriority:4;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:FLTHAZ02')
		elseif feature.categoryOfObstruction == 9 and feature.valueOfSounding then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:34051;DrawingPriority:4;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:34051;DrawingPriority:4;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:ACHARE02')
		elseif feature.categoryOfObstruction == 10 and feature.valueOfSounding then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:34051;DrawingPriority:4;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:34051;DrawingPriority:4;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:FLTHAZ02')
		elseif feature.categoryOfObstruction == 7 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:34050;DrawingPriority:4;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:34050;DrawingPriority:4;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:FOULGND1')
		elseif feature.categoryOfObstruction == 8 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:34050;DrawingPriority:4;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:34050;DrawingPriority:4;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:FLTHAZ02')
		elseif feature.categoryOfObstruction == 9 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:34050;DrawingPriority:4;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:34050;DrawingPriority:4;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:ACHARE02')
		elseif feature.categoryOfObstruction == 10 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:34050;DrawingPriority:4;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:34050;DrawingPriority:4;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:FLTHAZ02')
		elseif feature.waterLevelEffect == 7 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:4;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:4;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:FLTHAZ02')
		else
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:34050;DrawingPriority:4;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:34050;DrawingPriority:4;DisplayPlane:UnderRADAR')
			end
			OBSTRN07(feature, featurePortrayal, contextParameters)
		end
	elseif feature.PrimitiveType == PrimitiveType.Point then
		if feature.categoryOfObstruction == 7 and feature.valueOfSounding then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:34051;DrawingPriority:4;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:34051;DrawingPriority:4;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:FOULGND1')
		elseif feature.categoryOfObstruction == 8 and feature.valueOfSounding then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:34051;DrawingPriority:4;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:34051;DrawingPriority:4;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:FLTHAZ02')
		elseif feature.categoryOfObstruction == 9 and feature.valueOfSounding then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:34051;DrawingPriority:4;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:34051;DrawingPriority:4;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:ACHARE02')
		elseif feature.categoryOfObstruction == 10 and feature.valueOfSounding then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:34051;DrawingPriority:4;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:34051;DrawingPriority:4;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:FLTHAZ02')
		elseif feature.categoryOfObstruction == 7 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:34050;DrawingPriority:4;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:34050;DrawingPriority:4;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:FOULGND1')
		elseif feature.categoryOfObstruction == 8 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:34050;DrawingPriority:4;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:34050;DrawingPriority:4;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:FLTHAZ02')
		elseif feature.categoryOfObstruction == 9 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:34050;DrawingPriority:4;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:34050;DrawingPriority:4;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:ACHARE02')
		elseif feature.categoryOfObstruction == 10 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:34050;DrawingPriority:4;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:34050;DrawingPriority:4;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:FLTHAZ02')
		elseif feature.waterLevelEffect == 7 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:4;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:4;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:AddInstructions('PointInstruction:FLTHAZ02')
		else
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:34050;DrawingPriority:4;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:34050;DrawingPriority:4;DisplayPlane:UnderRADAR')
			end
			OBSTRN07(feature, featurePortrayal, contextParameters)
		end
	elseif feature.PrimitiveType == PrimitiveType.Curve then
		if feature.categoryOfObstruction == 8 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:4;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:4;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:SimpleLineStyle('dash',0.32,'CSTLN')
			featurePortrayal:AddInstructions('LineInstruction:')
		elseif feature.categoryOfObstruction == 9 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:4;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:4;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:SimpleLineStyle('dash',0.32,'CHMGD')
			featurePortrayal:AddInstructions('LineInstruction:')
		elseif feature.categoryOfObstruction == 10 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:4;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:4;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:SimpleLineStyle('dash',0.32,'CSTLN')
			featurePortrayal:AddInstructions('LineInstruction:')
		elseif feature.waterLevelEffect == 7 then
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:4;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:4;DisplayPlane:UnderRADAR')
			end
			featurePortrayal:SimpleLineStyle('dash',0.32,'CSTLN')
			featurePortrayal:AddInstructions('LineInstruction:')
		else
			if contextParameters.RADAR_OVERLAY then
				featurePortrayal:AddInstructions('ViewingGroup:34050;DrawingPriority:4;DisplayPlane:OverRADAR')
			else
				featurePortrayal:AddInstructions('ViewingGroup:34050;DrawingPriority:4;DisplayPlane:UnderRADAR')
			end
			OBSTRN07(feature, featurePortrayal, contextParameters)
		end
	elseif feature.PrimitiveType == PrimitiveType.Surface and contextParameters.PLAIN_BOUNDARIES then
		if feature.categoryOfObstruction == 7 and feature.valueOfSounding then
			featurePortrayal:AddInstructions('ViewingGroup:34051;DrawingPriority:4;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:FOULGND1')
			featurePortrayal:SimpleLineStyle('dash',0.32,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:')
		elseif feature.categoryOfObstruction == 7 then
			featurePortrayal:AddInstructions('ViewingGroup:34050;DrawingPriority:4;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:FOULGND1')
			featurePortrayal:SimpleLineStyle('dash',0.32,'CHGRD')
			featurePortrayal:AddInstructions('LineInstruction:')
		elseif feature.categoryOfObstruction == 8 then
			featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:4;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:FLTHAZ02')
			featurePortrayal:SimpleLineStyle('dash',0.32,'CSTLN')
			featurePortrayal:AddInstructions('LineInstruction:')
		elseif feature.categoryOfObstruction == 9 then
			featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:4;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:ACHARE02')
			featurePortrayal:SimpleLineStyle('dash',0.32,'CHMGD')
			featurePortrayal:AddInstructions('LineInstruction:')
		elseif feature.categoryOfObstruction == 10 then
			featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:4;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:FLTHAZ02')
			featurePortrayal:SimpleLineStyle('dash',0.32,'CSTLN')
			featurePortrayal:AddInstructions('LineInstruction:')
		elseif feature.waterLevelEffect == 7 then
			featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:4;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:FLTHAZ02')
			featurePortrayal:SimpleLineStyle('dash',0.32,'CSTLN')
			featurePortrayal:AddInstructions('LineInstruction:')
		else
			featurePortrayal:AddInstructions('ViewingGroup:34050;DrawingPriority:4;DisplayPlane:UnderRADAR')
			OBSTRN07(feature, featurePortrayal, contextParameters)
		end
	elseif feature.PrimitiveType == PrimitiveType.Surface then
		if feature.categoryOfObstruction == 7 and feature.valueOfSounding then
			featurePortrayal:AddInstructions('ViewingGroup:34051;DrawingPriority:4;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:FOULGND1')
			featurePortrayal:AddInstructions('LineInstruction:NAVARE51')
		elseif feature.categoryOfObstruction == 7 then
			featurePortrayal:AddInstructions('ViewingGroup:34050;DrawingPriority:4;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:FOULGND1')
			featurePortrayal:AddInstructions('LineInstruction:NAVARE51')
		elseif feature.categoryOfObstruction == 8 then
			featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:4;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:FLTHAZ02')
			featurePortrayal:SimpleLineStyle('dash',0.32,'CSTLN')
			featurePortrayal:AddInstructions('LineInstruction:')
		elseif feature.categoryOfObstruction == 9 then
			featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:4;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:ACHARE02')
			featurePortrayal:SimpleLineStyle('dash',0.32,'CHMGD')
			featurePortrayal:AddInstructions('LineInstruction:')
		elseif feature.categoryOfObstruction == 10 then
			featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:4;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:FLTHAZ02')
			featurePortrayal:SimpleLineStyle('dash',0.32,'CSTLN')
			featurePortrayal:AddInstructions('LineInstruction:')
		elseif feature.waterLevelEffect == 7 then
			featurePortrayal:AddInstructions('ViewingGroup:12410;DrawingPriority:4;DisplayPlane:UnderRADAR')
			featurePortrayal:AddInstructions('PointInstruction:FLTHAZ02')
			featurePortrayal:SimpleLineStyle('dash',0.32,'CSTLN')
			featurePortrayal:AddInstructions('LineInstruction:')
		else
			featurePortrayal:AddInstructions('ViewingGroup:34050;DrawingPriority:4;DisplayPlane:UnderRADAR')
			OBSTRN07(feature, featurePortrayal, contextParameters)
		end
	else
		error('Invalid primitive type or mariner settings passed to portrayal')
	end
end
