-- TOPMAR01 conditional symbology rules file.

local floatingTopmarks =
{
	[1] = 'TOPMAR02',
	[2] = 'TOPMAR04',
	[3] = 'TOPMAR10',
	[4] = 'TOPMAR12',
	[5] = 'TOPMAR13',
	[6] = 'TOPMAR14',
	[7] = 'TOPMAR65',
	[8] = 'TOPMAR17',
	[9] = 'TOPMAR16',
	[10] = 'TOPMAR08',
	[11] = 'TOPMAR07',
	[12] = 'TOPMAR14',
	[13] = 'TOPMAR05',
	[14] = 'TOPMAR06',
	[15] = 'TMARDEF2',
	[16] = 'TMARDEF2',
	[17] = 'TMARDEF2',
	[18] = 'TOPMAR10',
	[19] = 'TOPMAR13',
	[20] = 'TOPMAR14',
	[21] = 'TOPMAR13',
	[22] = 'TOPMAR14',
	[23] = 'TOPMAR14',
	[24] = 'TOPMAR02',
	[25] = 'TOPMAR04',
	[26] = 'TOPMAR10',
	[27] = 'TOPMAR17',
	[28] = 'TOPMAR18',
	[29] = 'TOPMAR02',
	[30] = 'TOPMAR17',
	[31] = 'TOPMAR14',
	[32] = 'TOPMAR10',
	[33] = 'TMARDEF2'
}
	
local rigidTopmarks =
{
	[1] = 'TOPMAR22',
	[2] = 'TOPMAR24',
	[3] = 'TOPMAR30',
	[4] = 'TOPMAR32',
	[5] = 'TOPMAR33',
	[6] = 'TOPMAR34',
	[7] = 'TOPMAR85',
	[8] = 'TOPMAR86',
	[9] = 'TOPMAR36',
	[10] = 'TOPMAR28',
	[11] = 'TOPMAR27',
	[12] = 'TOPMAR14',
	[13] = 'TOPMAR25',
	[14] = 'TOPMAR26',
	[15] = 'TOPMAR88',
	[16] = 'TOPMAR87',
	[17] = 'TMARDEF1',
	[18] = 'TOPMAR30',
	[19] = 'TOPMAR33',
	[20] = 'TOPMAR34',
	[21] = 'TOPMAR33',
	[22] = 'TOPMAR34',
	[23] = 'TOPMAR34',
	[24] = 'TOPMAR22',
	[25] = 'TOPMAR24',
	[26] = 'TOPMAR30',
	[27] = 'TOPMAR86',
	[28] = 'TOPMAR89',
	[29] = 'TOPMAR22',
	[30] = 'TOPMAR86',
	[31] = 'TOPMAR14',
	[32] = 'TOPMAR30',
	[33] = 'TMARDEF1'
}

-- Main entry point for CSP.
function TOPMAR01(feature, featurePortrayal, contextParameters, viewingGroup, isFloating)
	Debug.StartPerformance('Lua Code - TOPMAR01')

	if feature.topmark then
		if isFloating then
			topmarkSymbol = floatingTopmarks[feature.topmark.topmarkDaymarkShape] or 'TMARDEF2'
		else
			topmarkSymbol = rigidTopmarks[feature.topmark.topmarkDaymarkShape] or 'TMARDEF1'
		end

		featurePortrayal:AddInstructions('PointInstruction:' .. topmarkSymbol)
	end

	Debug.StopPerformance('Lua Code - TOPMAR01')
end
