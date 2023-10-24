--[[
This file contains the global functions that define the Lua portrayal API.
These functions are called by the host program.
--]]

portrayalContext = nil

function PortrayalInitializeContextParameters(contextParameters)
	Debug.StartPerformance('Lua Code - Total')
	CheckType(contextParameters, 'array:ContextParameter')

	Debug.StartPerformance('Lua Code - PortrayalInitializeContextParameters')

	portrayalContext = PortrayalModel.CreatePortrayalContext()

	local pccp = portrayalContext.ContextParameters._underlyingTable

	pccp._parameterTypes = {}

	for _, cp in ipairs(contextParameters) do
		pccp[cp.Name] = cp.DefaultValue
		pccp._parameterTypes[cp.Name] = cp.ParameterType
	end

	Debug.StopPerformance('Lua Code - PortrayalInitializeContextParameters')
	Debug.StopPerformance('Lua Code - Total')
end

function PortrayalCreateContextParameter(contextParameterName, parameterType, defaultValue)
	CheckType(contextParameterName, 'string')
	CheckType(parameterType, 'string')

	-- Case insensitive because the docs show leading capitalization but
	-- ConvertEncodedValue requires lower, as does S100CD.xsd
	local cpt = string.lower(parameterType)
	
	-- Allow types to be described per 9-13.3.30 ParameterType despite
	-- 9a-14.1.3 requiring types to be described per S100_CD_AttributeValueType
	if cpt == 'double' then
		cpt = 'real'
	elseif cpt == 'string' then
		cpt = 'text'
	end
	
	if cpt ~= 'boolean' and
	   cpt ~= 'integer' and
	   cpt ~= 'real' and
	   cpt ~= 'text' and
	   cpt ~= 'date' then
		error('Invalid parameter type:' .. cpt)
	end

	return { Type = 'ContextParameter', Name = contextParameterName, ParameterType = cpt, DefaultValue = ConvertEncodedValue(cpt, defaultValue) }
end

function PortrayalSetContextParameter(contextParameterName, value)
	Debug.StartPerformance('Lua Code - Total')
	CheckType(contextParameterName, 'string')
	CheckType(value, 'string')

	if not portrayalContext then
		error('Portrayal context not initialized.')
	end

	local pccp = portrayalContext.ContextParameters._underlyingTable

	portrayalContext.ContextParameters[contextParameterName] = ConvertEncodedValue(pccp._parameterTypes[contextParameterName], value)

	Debug.StopPerformance('Lua Code - Total')
end

local nilAttribute = {}

local function LookupAttributeValue(container, attributeCode, HostGetSimpleAttribute, HostGetComplexAttributeCount)
	local attributeMetatable =
	{
		__index = function (container, attributeCode)
			return LookupAttributeValue(container, attributeCode, HostGetSimpleAttribute, HostGetComplexAttributeCount)
		end
	}

	local attributeCode1 = attributeCode

	local nilIfMissing = false

	-- Is this a "nil if missing" attribute?
	if attributeCode:sub(1, 1) == '!' then
		nilIfMissing = true

		attributeCode = attributeCode:sub(2)
	end

	local unknownIfUnknown = false

	-- Is this a "unknown if unknown" attribute?
	if attributeCode:sub(1, 1) == '@' then
		unknownIfUnknown = true

		attributeCode = attributeCode:sub(2)
	end

	local attributePath = {}

	local topContainer = container

	while rawget(topContainer, 'Parent') do
		table.insert(attributePath, 1, topContainer.AttributeCode .. ':' .. topContainer.Index)
			
		topContainer = rawget(topContainer, 'Parent')
	end

	attributePath = table.concat(attributePath, ';')

	local ti = GetTypeInfo()

	local containerTypeInfo

	if container.Type == 'ComplexAttributeValue' then
		containerTypeInfo = GetComplexAttributeTypeInfo(container.AttributeCode)
	elseif container.Type == 'Feature' then
		containerTypeInfo = GetFeatureTypeInfo(container.Code)
	elseif container.Type == 'Information' then
		containerTypeInfo = GetInformationTypeInfo(container.Code)
	else
		error('Not implemented')
	end

	--Debug.Break()

	if containerTypeInfo == nil or not containerTypeInfo.AttributeBindings[attributeCode] then
		if nilIfMissing then
			return nil
		else
			error('Invalid attribute code "' .. attributeCode .. '" specified.', 3)
		end
	end

	local simpleAttributeTypeInfo
	local complexAttributeTypeInfo

	if ti.SimpleAttributeInfos[attributeCode] then
		simpleAttributeTypeInfo = GetSimpleAttributeTypeInfo(attributeCode)
	else
		complexAttributeTypeInfo = GetComplexAttributeTypeInfo(attributeCode)
	end

	if complexAttributeTypeInfo then
		Debug.StopPerformance('Lua Code - Total')
		local attributeCount = HostGetComplexAttributeCount(container.ID, attributePath, attributeCode)
		Debug.StartPerformance('Lua Code - Total')

		if containerTypeInfo.AttributeBindings[attributeCode].UpperMultiplicity == 1 then
			-- Single valued
			if attributeCount ~= 0 then
				local value = { Type = 'ComplexAttributeValue', Parent = container, ID = container.ID, AttributeCode = attributeCode, Index = 1 }

				setmetatable(value, attributeMetatable)

				container[attributeCode] = value
				container['!' .. attributeCode] = value
			end
		else
			-- Array
			local values = {}

			for i = 1, attributeCount do
				local value = { Type = 'ComplexAttributeValue', Parent = container, ID = container.ID, AttributeCode = attributeCode, Index = i }

				setmetatable(value, attributeMetatable)

				values[#values + 1] = value
			end

			container[attributeCode] = values
			container['!' .. attributeCode] = values
		end
	else
		Debug.StopPerformance('Lua Code - Total')
		local values = HostGetSimpleAttribute(container.ID, attributePath, attributeCode)
		Debug.StartPerformance('Lua Code - Total')

		--Debug.Break()

		if containerTypeInfo.AttributeBindings[attributeCode].UpperMultiplicity == 1 then
			-- Single valued
			local value

			if #values > 0 then
				value = ConvertEncodedValue(simpleAttributeTypeInfo.ValueType, values[1])
			end

			container['@' .. attributeCode] = value

			if value == unknownValue then
				value = nil
			end

			container[attributeCode] = value
			container['!' .. attributeCode] = value
		else
			-- Array
			local convertedValues = {}

			for i, value in ipairs(values) do
				local convertedValue = ConvertEncodedValue(simpleAttributeTypeInfo.ValueType, value)

				convertedValues['@' .. i] = convertedValue

				if convertedValue == unknownValue then
					convertedValue = nil
				end

				convertedValues[i] = convertedValue
			end

			container[attributeCode] = convertedValues
			container['!' .. attributeCode] = convertedValues
		end
	end

	return rawget(container, attributeCode1)
end

PrimitiveType =
{
	None       = { Type = 'PrimitiveType', Value = 0, Name = 'None' },
	Point      = { Type = 'PrimitiveType', Value = 1, Name = 'Point' },
	MultiPoint = { Type = 'PrimitiveType', Value = 2, Name = 'MultiPoint' },
	Curve      = { Type = 'PrimitiveType', Value = 3, Name = 'Curve' },
	Surface    = { Type = 'PrimitiveType', Value = 4, Name = 'Surface' },
	Coverage   = { Type = 'PrimitiveType', Value = 5, Name = 'Coverage' }
}

SpatialType =
{
	Point          = { Type = 'SpatialType', Value = 1, Name = 'Point' },
	MultiPoint     = { Type = 'SpatialType', Value = 2, Name = 'MultiPoint' },
	Curve          = { Type = 'SpatialType', Value = 3, Name = 'Curve' },
	CompositeCurve = { Type = 'SpatialType', Value = 4, Name = 'CompositeCurve' },
	Surface        = { Type = 'SpatialType', Value = 5, Name = 'Surface' }
}

Orientation =
{
	Forward = { Type = 'Orientation', Value = 1, Name = 'Forward' },
	Reverse = { Type = 'Orientation', Value = 2, Name = 'Reverse' }
}

Interpolation =
{
	None                             = { Type = 'Interpolation', value = 0, Name = 'None' },
	Linear                           = { Type = 'Interpolation', value = 1, Name = 'Linear' },
	Geodesic                         = { Type = 'Interpolation', value = 2, Name = 'Geodesic' },
	Arc3Points                       = { Type = 'Interpolation', value = 3, Name = 'Arc3Points' },
	Loxodromic                       = { Type = 'Interpolation', value = 4, Name = 'Loxodromic' },
	Elliptical                       = { Type = 'Interpolation', value = 5, Name = 'Elliptical' },
	Conic                            = { Type = 'Interpolation', value = 6, Name = 'Conic' },
	CircularArcCenterPointWithRadius = { Type = 'Interpolation', value = 7, Name = 'CircularArcCenterPointWithRadius' }
}

--
-- Type Information Creation Functions
--

function CreateItem(code, name, definition, remarks, alias)
	CheckType(code, 'string')
	CheckType(name, 'string')
	CheckType(definition, 'string')
	CheckTypeOrNil(remarks, 'string')
	CheckTypeOrNil(alias, 'array:string')

	return { Type = 'Item', Code = code, Name = name, Definition = definition, Remarks = remarks, Alias  = alias }
end

function CreateNamedType(item, abstract, attributeBindings)
	CheckType(item, 'Item')
	CheckType(abstract, 'boolean')
	CheckType(attributeBindings, 'array:AttributeBinding')

	for _, ab in ipairs(attributeBindings) do
		attributeBindings[ab.AttributeCode] = ab
	end

	return DerivedType{ Type = 'NamedType', Base = item, Abstract = abstract, AttributeBindings = attributeBindings }
end

function CreateObjectType(namedType, informationBindings)
	CheckType(namedType, 'NamedType')
	CheckType(informationBindings, 'array:InformationBinding')

	return DerivedType{ Type = 'ObjectType', Base = namedType, InformationBindings = informationBindings }
end

function CreateInformationType(objectType, superType, subType)
	CheckType(objectType, 'ObjectType')
	CheckTypeOrNil(superType, 'string')
	CheckTypeOrNil(subType, 'array:string')

	return DerivedType{ Type = 'InformationType', Base = objectType, SuperType = superType, SubType = subType }
end

function CreateFeatureType(objectType, featureUseType, permittedPrimitives, featureBindings, superType, subType)
	CheckType(objectType, 'ObjectType')
	CheckType(featureUseType, 'string')
	CheckType(permittedPrimitives, 'array:string')
	CheckType(featureBindings, 'array:FeatureBinding')
	CheckTypeOrNil(superType, 'string')
	CheckTypeOrNil(subType, 'array:string')

	return DerivedType{ Type = 'FeatureType', Base = objectType, FeatureUseType = featureUseType, PermittedPrimitives = permittedPrimitives, FeatureBindings = featureBindings, SuperType = superType, SubType = subType }
end

function CreateInformationAssociation(namedType, roles, superType, subType)
	CheckType(namedType, 'NamedType')
	CheckType(roles, 'array:string')
	CheckTypeOrNil(superType, 'string')
	CheckTypeOrNil(subType, 'array:string')

	return DerivedType{ Type = 'InformationAssociation', Base = namedType, Roles = roles, SuperType = superType, SubType = subType }
end

function CreateFeatureAssociation(namedType, roles, superType, subType)
	CheckType(namedType, 'NamedType')
	CheckType(roles, 'array:string')
	CheckTypeOrNil(superType, 'string')
	CheckTypeOrNil(subType, 'array:string')

	return DerivedType{ Type = 'FeatureAssociation', Base = namedType, Roles = roles, SuperType = superType, SubType = subType }
end

function CreateRole(item)
	CheckType(item, 'Item')

	return DerivedType{ Type = 'Role', Base = item }
end

--
--
--

function CreateAttributeConstraints(stringLength, textPattern, rangeLower, rangeUpper, closure, precision)
	CheckTypeOrNil(stringLength, 'number')
	CheckTypeOrNil(textPattern, 'string')
	CheckTypeOrNil(rangeLower, 'string')
	CheckTypeOrNil(rangeUpper, 'string')
	CheckTypeOrNil(rangeClosure, 'string')
	CheckTypeOrNil(precision, 'number')

	return { Type = 'AttributeConstraints', StringLength = stringLength, TextPattern = textPattern, RangeLower = rangeLower, RangeUpper = rangeUpper, RangeClosure = rangeClosure, Precision = precision }
end

--
--
--

function CreateSimpleAttribute(item, valueType, uom, quantitySpecification, attributeContraints, listedValues)
	CheckType(item, 'Item')
	CheckType(valueType, 'string')
	CheckTypeOrNil(uom, 'string')
	CheckTypeOrNil(quantitySpecification, 'string')
	CheckTypeOrNil(attributeContraints, 'AttributeConstraints')
	CheckType(listedValues, 'array:ListedValue')

	return DerivedType{ Type = 'SimpleAttribute', Base = item, ValueType = valueType, Uom = uom, QuantitySpecification = quantitySpecification, AttributeContraints = attributeContraints, ListedValues = listedValues }
end

--
--
--

function CreateComplexAttribute(item, subAttributeBindings)
	CheckType(item, 'Item')
	CheckType(subAttributeBindings, 'array:AttributeBinding')

	for _, ab in ipairs(subAttributeBindings) do
		subAttributeBindings[ab.AttributeCode] = ab
	end

	return DerivedType{ Type = 'ComplexAttribute', Base = item, AttributeBindings = subAttributeBindings }
end

--
--
--

function CreateListedValue(label, definition, code, remarks, aliases)
	CheckType(label, 'string')
	CheckType(definition, 'string')
	CheckType(code, 'number')
	CheckTypeOrNil(remarks, 'string')
	CheckTypeOrNil(aliases, 'array:string')

	return { Type = 'ListedValue', Label = label, Definition = definition, Code = code, Remarks = remarks, Aliases = aliases }
end

--
--
--

function CreateAttributeBinding(attributeCode, lowerMultiplicity, upperMultiplicity, sequential, permittedValues)
	CheckType(attributeCode, 'string')
	CheckType(lowerMultiplicity, 'number')
	CheckTypeOrNil(upperMultiplicity, 'number')
	CheckType(sequential, 'boolean')
	CheckType(permittedValues, 'array:number')

	return { Type = 'AttributeBinding', AttributeCode = attributeCode, LowerMultiplicity = lowerMultiplicity, UpperMultiplicity = upperMultiplicity, Sequential = sequential, PermittedValues = permittedValues }
end

--
--
--

function CreateInformationBinding(informationTypeCodes, lowerMultiplicity, upperMultiplicity, roleType, role, association)
	CheckType(informationTypeCodes, 'array:string')
	CheckType(lowerMultiplicity, 'number')
	CheckTypeOrNil(upperMultiplicity, 'number')
	CheckType(roleType, 'string')
	CheckTypeOrNil(role, 'string')
	CheckType(association, 'string')

	return { Type = 'InformationBinding', InformationTypeCodes = informationTypeCodes, LowerMultiplicity = lowerMultiplicity, UpperMultiplicity = upperMultiplicity, RoleType = roleType, Role = role, Association = association }
end

--
--
--

function CreateFeatureBinding(featureTypeCodes, lowerMultiplicity, upperMultiplicity, roleType, role, association)
	CheckType(featureTypeCodes, 'array:string')
	CheckType(lowerMultiplicity, 'number')
	CheckTypeOrNil(upperMultiplicity, 'number')
	CheckType(roleType, 'string')
	CheckType(role, 'string')
	CheckType(association, 'string')

	return { Type = 'FeatureBinding', FeatureTypeCodes = featureTypeCodes, LowerMultiplicity = lowerMultiplicity, UpperMultiplicity = upperMultiplicity, RoleType = roleType, Role = role, Association = association }
end

--
--
--

local featureCache = {}
local informationCache = {}
spatialCache = {}

function CreateFeature(featureID, featureCode)
	Debug.StartPerformance('Lua Code - Total')
	local featureMetatable =
	{
		__index = function (t, k)
			if k == 'Spatial' or k == 'Point' or k == 'MultiPoint' or k == 'Curve' or k == 'CompositeCurve' or k == 'Surface' then
				local spatial = t:GetSpatial()

				--if spatial ~= nil then
					--t['SpatialType'] = spatial.SpatialType
				--end

				if spatial and (k == 'Spatial' or spatial.SpatialType.Name == k) then
					return spatial
				end
			elseif k == 'PrimitiveType' then
				local pt = PrimitiveType.None
				local sa = t:GetSpatialAssociation()

				if sa ~= nil then
					if sa.SpatialType == SpatialType.Point then
						pt = PrimitiveType.Point
					elseif sa.SpatialType == SpatialType.MultiPoint then
						pt = PrimitiveType.MultiPoint
					elseif sa.SpatialType == SpatialType.Curve or sa.SpatialType == SpatialType.CompositeCurve then
						pt = PrimitiveType.Curve
					elseif sa.SpatialType == SpatialType.Surface then
						pt = PrimitiveType.Surface
					end
				end
				
				t['PrimitiveType'] = pt

				return pt
			elseif k == 'SpatialAssociations' then
				return t:GetSpatialAssociations()
			elseif k ~= 'FlattenedSpatialAssociations' then
				local av = LookupAttributeValue(t, k, HostFeatureGetSimpleAttribute, HostFeatureGetComplexAttributeCount)

				if av ~= nil then
					return av
				end
			end
		end
	}

	local feature = featureCache[featureID]

	if feature then
		return feature
	end
	
	feature = { Type = 'Feature', ID = featureID, Code = featureCode, InformationAssociations = {}, FeatureAssociations = {} }

	featureCache[featureID] = feature

	function feature:GetInformationAssociations(associationCode, roleCode)
		-- Allow for passing in of informationTypeCode
		CheckSelf(self, 'Feature')
		CheckType(associationCode, 'string')
		CheckTypeOrNil(roleCode, 'string')

		local tuple = associationCode .. '|' .. (roleCode or '')

		local ias = self.InformationAssociations[tuple]

		if not ias then
			Debug.StopPerformance('Lua Code - Total')
			local informationIDs = HostFeatureGetAssociatedInformationIDs(self.ID, associationCode, roleCode)
			Debug.StartPerformance('Lua Code - Total')

			ias = {}

			for _, informationID in ipairs(informationIDs) do
				Debug.StopPerformance('Lua Code - Total')
				local code = HostInformationTypeGetCode(informationID)
				Debug.StartPerformance('Lua Code - Total')
				ias[#ias + 1] = CreateInformation(informationID, code)
			end

			self.InformationAssociations[tuple] = ias
		end

		return ias
	end

	function feature:GetInformationAssociation(associationCode, roleCode, informationTypeCode)
		CheckSelf(self, 'Feature')
		CheckType(associationCode, 'string')
		CheckTypeOrNil(roleCode, 'string')
		CheckTypeOrNil(informationTypeCode, 'string')

		local ias = self:GetInformationAssociations(associationCode, roleCode)
		
		if #ias ~= 0 then
			if informationTypeCode then
				for _, ia in ipairs(ias) do
					if ia.Code == informationTypeCode then
						return ia
					end
				end
			else
				return ias[1]
			end
		end
	end

	function feature:GetFeatureAssociations(associationCode, roleCode)
		-- Allow for passing in of featureTypeCode
		CheckSelf(self, 'Feature')
		CheckType(associationCode, 'string')
		CheckTypeOrNil(roleCode, 'string')

		local tuple = associationCode .. '|' .. (roleCode or '')

		local fas = self.FeatureAssociations[tuple]

		if not fas then
			Debug.StopPerformance('Lua Code - Total')
			local featureIDs = HostFeatureGetAssociatedFeatureIDs(self.ID, associationCode, roleCode)
			Debug.StartPerformance('Lua Code - Total')

			fas = {}

			for _, featureID in ipairs(featureIDs) do
				Debug.StopPerformance('Lua Code - Total')
				local code = HostFeatureGetCode(featureID)
				Debug.StartPerformance('Lua Code - Total')
				fas[#fas + 1] = CreateFeature(featureID, code)
			end

			self.FeatureAssociations[tuple] = fas
		end

		return fas
	end

	function feature:GetFeatureAssociation(associationCode, roleCode, featureTypeCode)
		CheckSelf(self, 'Feature')
		CheckType(associationCode, 'string')
		CheckTypeOrNil(roleCode, 'string')

		local fas = self:GetFeatureAssociations(associationCode, roleCode)

		if fas then
			if featureTypeCode then
				for _, fa in ipairs(fas) do
					if fa.Code == featureTypeCode then
						return fa
					end
				end
			else
				return fas[1]
			end
		end
	end

	function feature:GetSpatialAssociations()
		CheckSelf(self, 'Feature')

		local sas = rawget(self, 'SpatialAssociations')

		Debug.StopPerformance('Lua Code - Total')
		sas = sas or HostFeatureGetSpatialAssociations(self.ID)
		Debug.StartPerformance('Lua Code - Total')

		self['SpatialAssociations'] = sas

		CheckTypeOrNil(sas, 'array:SpatialAssociation')

		return sas
	end

	function feature:GetSpatialAssociation()
		CheckSelf(self, 'Feature')

		-- TODO: Pick single association based on current scale.
		local sas = self:GetSpatialAssociations()

		if sas ~= nil then
			return sas[1]
		end
	end

	function feature:GetSpatial()
		CheckSelf(self, 'Feature')

		local sa = self:GetSpatialAssociation()

		if sa ~= nil then
			self['Spatial'] = sa.Spatial

			self[sa.SpatialType.Name] = self['Spatial']

			return self['Spatial']
		end
	end

	-- Returns an iterator that returns all spatial associations to points, multi points and curves
	-- associated to the feature.  Surface and composite curves return only their ultimate simple curves.
	-- This only works for features with a single spatial association.
	function feature:GetFlattenedSpatialAssociations()
		local fsa = self['FlattenedSpatialAssociations']

		if not fsa then
			self['FlattenedSpatialAssociations'] = {}

			fsa = self['FlattenedSpatialAssociations']

			local function FlattenCompositeCurve(compositeCurve)
				for _, curveElement in ipairs(compositeCurve.CurveAssociations) do
					if curveElement.SpatialType == SpatialType.CompositeCurve then
						FlattenCompositeCurve(curveElement.Spatial)
					else
						fsa[#fsa + 1] = curveElement
					end
				end
			end

			local spatialType = self:GetSpatialAssociation().SpatialType

			if contains(spatialType, { SpatialType.Point, SpatialType.MultiPoint, SpatialType.Curve }) then
				fsa[#fsa + 1] = self:GetSpatialAssociation()
			elseif spatialType == SpatialType.CompositeCurve then
				FlattenCompositeCurve(self.CompositeCurve)
			elseif spatialType == SpatialType.Surface then
				if self.Surface.ExteriorRing.SpatialType == SpatialType.CompositeCurve then
					FlattenCompositeCurve(self.Surface.ExteriorRing.Spatial)
				else
					fsa[#fsa + 1] = self.Surface.ExteriorRing
				end

				for _, ring in ipairs(self.Surface.InteriorRings) do
					if ring.SpatialType == SpatialType.CompositeCurve then
						FlattenCompositeCurve(ring.Spatial)
					else
						fsa[#fsa + 1] = ring
					end
				end
			end
		end

		local i = 0

		return function()
			i = i + 1

			return fsa[i]
		end
	end

	setmetatable(feature, featureMetatable)
	
	Debug.StopPerformance('Lua Code - Total')

	return feature
end

function CreateInformation(informationID, informationCode)
	Debug.StartPerformance('Lua Code - Total')

	local informationMetatable =
	{
		__index = function (t, k)
			local av = LookupAttributeValue(t, k, HostInformationTypeGetSimpleAttribute, HostInformationTypeGetComplexAttributeCount)

			if av ~= nil then
				return av
			end
		end
	}

	local information = informationCache[informationID];

	if information then
		return information
	end

	information = { Type = 'Information', ID = informationID, Code = informationCode }

	informationCache[informationID] = information

	function information:GetInformationAssociations(associationCode, roleCode)
		error('information:GetInformationAssociations not implemented.')
	end

	setmetatable(information, informationMetatable)

	Debug.StopPerformance('Lua Code - Total')

	return information
end

function CreateSpatialAssociation(spatialType, spatialID, orientation, scaleMinimum, scaleMaximum)
	Debug.StartPerformance('Lua Code - Total')

	local spatialAssociationMetatable =
	{
		__index = function (t, k)
			if k == 'Spatial' then
				Debug.StartPerformance('Lua Code - Spatial')
				local spatial = spatialCache[t.SpatialID]

				if not spatial then
					Debug.StartPerformance('Lua Code - HostGetSpatial')
					Debug.StopPerformance('Lua Code - Total')
					spatial = HostGetSpatial(t.SpatialID) or nilMarker
					Debug.StartPerformance('Lua Code - Total')
					Debug.StopPerformance('Lua Code - HostGetSpatial')

					spatialCache[t.SpatialID] = spatial

					if spatial ~= nilMarker then
						CheckType(spatial, 'Spatial')
						spatial['SpatialID'] = t.SpatialID
				
						t['Spatial'] = spatial
					else
						--Debug.Break()
					end
				else
					--Debug.Break()
				end

				Debug.StopPerformance('Lua Code - Spatial')
				if spatial ~= nilMarker then
					return spatial
				end
			elseif k == 'AssociatedFeatures' then
				return t:GetAssociatedFeatures()
			end
		end
	}

	if type(spatialType) == 'string' then
		spatialType = SpatialType[spatialType]
	end

	if type(orientation) == 'string' then
		orientation = Orientation[orientation]
	end

	CheckType(spatialType, 'SpatialType')
	CheckTypeOrNil(orientation, 'Orientation')
	CheckTypeOrNil(scaleMinimum, 'number')
	CheckTypeOrNil(scaleMaximum, 'number')

	local spatialAssociation = { Type = 'SpatialAssociation', SpatialType = spatialType, SpatialID = spatialID, Orientation = orientation, ScaleMinimum = scaleMinimum, ScaleMaximum = scaleMaximum, InformationAssociations = {} }

	function spatialAssociation:GetAssociatedFeatures()
		Debug.StopPerformance('Lua Code - Total')
		local featureIDs = HostSpatialGetAssociatedFeatureIDs(self.SpatialID)
		Debug.StartPerformance('Lua Code - Total')

		self.AssociatedFeatures = {}

		for _, featureID in ipairs(featureIDs) do
			self.AssociatedFeatures[#self.AssociatedFeatures + 1] = featureCache[featureID];

			CheckType(featureCache[featureID], 'Feature');
		end

		return self.AssociatedFeatures
	end

	function spatialAssociation:GetInformationAssociations(associationCode, roleCode)
		CheckSelf(self, 'SpatialAssociation')
		CheckType(associationCode, 'string')
		CheckTypeOrNil(roleCode, 'string')

		local tuple = associationCode .. '|' .. (roleCode or '')

		local ias = self.InformationAssociations[tuple]

		if not ias then
			Debug.StopPerformance('Lua Code - Total')
			local informationIDs = HostSpatialGetAssociatedInformationIDs(self.SpatialID, associationCode, roleCode)
			Debug.StartPerformance('Lua Code - Total')

			ias = {}

			for _, informationID in ipairs(informationIDs) do
				Debug.StopPerformance('Lua Code - Total')
				local code = HostInformationTypeGetCode(informationID)
				Debug.StartPerformance('Lua Code - Total')
				ias[#ias + 1] = CreateInformation(informationID, code)
			end

			self.InformationAssociations[tuple] = ias
		end

		return ias
	end

	function spatialAssociation:GetInformationAssociation(associationCode, roleCode, informationTypeCode)
		CheckSelf(self, 'SpatialAssociation')
		CheckType(associationCode, 'string')
		CheckTypeOrNil(roleCode, 'string')
		CheckTypeOrNil(informationTypeCode, 'string')

		local ias = self:GetInformationAssociations(associationCode, roleCode)

		if #ias ~= 0 then
			if informationTypeCode then
				for _, ia in ipairs(ias) do
					if ia.Code == informationTypeCode then
						return ia
					end
				end
			else
				return ias[1]
			end
		end
	end

	setmetatable(spatialAssociation, spatialAssociationMetatable)

	Debug.StopPerformance('Lua Code - Total')

	return spatialAssociation
end

local function CreateSpatial(spatialType, spatial)
	CheckType(spatialType, 'SpatialType')

	local spatial = { Type = 'Spatial', SpatialType = spatialType, Spatial = spatial, InformationAssociations = {} }

	function spatial:GetInformationAssociations(associationCode, roleCode)
		CheckSelf(self, 'Spatial')
		CheckType(associationCode, 'string')
		CheckTypeOrNil(roleCode, 'string')

		local tuple = associationCode .. '|' .. (roleCode or '')

		local ias = self.InformationAssociations[tuple]

		if not ias then
			Debug.StopPerformance('Lua Code - Total')
			local informationIDs = HostSpatialGetAssociatedInformationIDs(self.SpatialID, associationCode, roleCode)
			Debug.StartPerformance('Lua Code - Total')

			ias = {}

			for _, informationID in ipairs(informationIDs) do
				Debug.StopPerformance('Lua Code - Total')
				local code = HostInformationTypeGetCode(informationID)
				Debug.StartPerformance('Lua Code - Total')
				ias[#ias + 1] = CreateInformation(informationID, code)
			end

			self.InformationAssociations[tuple] = ias
		end

		return ias
	end

	function spatial:GetInformationAssociation(associationCode, roleCode, informationTypeCode)
		CheckSelf(self, 'Spatial')
		CheckType(associationCode, 'string')
		CheckTypeOrNil(roleCode, 'string')
		CheckTypeOrNil(informationTypeCode, 'string')

		local ias = self:GetInformationAssociations(associationCode, roleCode)

		if #ias ~= 0 then
			if informationTypeCode then
				for _, ia in ipairs(ias) do
					if ia.Code == informationTypeCode then
						return ia
					end
				end
			else
				return ias[1]
			end
		end
	end

	return spatial
end

function CreatePoint(x, y, z)
	Debug.StartPerformance('Lua Code - Total')

	CheckType(x, 'string')
	CheckType(y, 'string')
	CheckTypeOrNil(z, 'string')

	local point = CreateSpatial(SpatialType.Point, { X = tonumber(x), Y = tonumber(y), Z = tonumber(z), ScaledX = StringToScaledDecimal(x), ScaledY = StringToScaledDecimal(y), ScaledZ = StringToScaledDecimal(z) })

	point.X = point.Spatial.X
	point.Y = point.Spatial.Y
	point.Z = point.Spatial.Z
	point.ScaledX = point.Spatial.ScaledX
	point.ScaledY = point.Spatial.ScaledY
	point.ScaledZ = point.Spatial.ScaledZ

	Debug.StopPerformance('Lua Code - Total')

	return point
end

function CreateMultiPoint(points)
	Debug.StartPerformance('Lua Code - Total')

	CheckType(points, 'array:Spatial')

	local multiPoint = CreateSpatial(SpatialType.MultiPoint, points)

	multiPoint.Points = multiPoint.Spatial

	Debug.StopPerformance('Lua Code - Total')

	return multiPoint
end

function CreateCurveSegment(controlPoints, interpolation)
	Debug.StartPerformance('Lua Code - Total')

	interpolation = interpolation or Interpolation.Loxodromic

	if type(interpolation) == 'string' then
		interpolation = Interpolation[interpolation]
	end

	CheckType(controlPoints, 'array:Spatial')
	CheckType(interpolation, 'Interpolation')

	Debug.StopPerformance('Lua Code - Total')

	return { Type = 'CurveSegment', ControlPoints = controlPoints, Interpolation = interpolation }
end

function CreateCurve(startPoint, endPoint, segments)
	Debug.StartPerformance('Lua Code - Total')

	CheckType(startPoint, 'SpatialAssociation')
	CheckType(endPoint, 'SpatialAssociation')
	CheckType(segments, 'array:CurveSegment')

	local curve = CreateSpatial(SpatialType.Curve, { StartPoint = startPoint, EndPoint = endPoint, Segments = segments })

	curve.StartPoint = curve.Spatial.StartPoint
	curve.EndPoint = curve.Spatial.EndPoint
	curve.Segments = curve.Spatial.Segments

	Debug.StopPerformance('Lua Code - Total')

	return curve
end

function CreateCompositeCurve(curveAssociations)
	Debug.StartPerformance('Lua Code - Total')

	CheckType(curveAssociations, 'array:SpatialAssociation')

	local compositeCurve = CreateSpatial(SpatialType.CompositeCurve, curveAssociations)

	compositeCurve.CurveAssociations = compositeCurve.Spatial

	Debug.StopPerformance('Lua Code - Total')

	return compositeCurve
end

function CreateSurface(exteriorRing, interiorRings)
	Debug.StartPerformance('Lua Code - Total')

	CheckType(exteriorRing, 'SpatialAssociation')
	CheckTypeOrNil(interiorRings, 'array:SpatialAssociation')

	local surface = CreateSpatial(SpatialType.Surface, { ExteriorRing = exteriorRing, InteriorRings = interiorRings or {} })

	surface.ExteriorRing = surface.Spatial.ExteriorRing
	surface.InteriorRings = surface.Spatial.InteriorRings

	Debug.StopPerformance('Lua Code - Total')

	return surface
end

function GetUnknownAttributeString()
	return '13BD40516CF742E886D5B4125DBB89742A043D0050E44B568CBB1FDDA8B464FF'
end

function EncodeDEFString(input)
	CheckType(input, 'string')

	input = input:gsub('&', '&a')
	input = input:gsub(';', '&s')
	input = input:gsub(':', '&c')
	input = input:gsub(',', '&m')

	return input
end

function DecodeDEFString(encodedString)
	CheckType(encodedString, 'string')

	encodedString = encodedString:gsub('&s', ';')
	encodedString = encodedString:gsub('&c', ':')
	encodedString = encodedString:gsub('&m', ',')
	encodedString = encodedString:gsub('&a', '&')

	return encodedString
end

local function JsonAppend(jsonTable, text)
	jsonTable[#jsonTable + 1] = text	
end

local function ConvertToJSONInternal(jsonTable, data)
	if type(data) == 'table' then
		if #data ~= 0 then
			JsonAppend(jsonTable, '[')

			for i, value in ipairs(data) do
				ConvertToJSONInternal(jsonTable, value)

				if i ~= #data then
					JsonAppend(jsonTable, ', ')
				end
			end

			JsonAppend(jsonTable, ']')
		else
			JsonAppend(jsonTable, '{')

			local first = true

			for key, value in pairs(data) do
				if type(value) ~= 'function' then
					if first then
						first = false
					else
						JsonAppend(jsonTable, ', ')
					end

					JsonAppend(jsonTable, '"')
					JsonAppend(jsonTable, key)
					JsonAppend(jsonTable, '" : ')

					ConvertToJSONInternal(jsonTable, value)
				end
			end

			JsonAppend(jsonTable, '}')
		end
	elseif type(data) == 'number' then
		JsonAppend(jsonTable, tostring(data))
	elseif type(data) == 'string' then
		JsonAppend(jsonTable, '"' .. data .. '"')
	elseif type(data) == 'boolean' then
		JsonAppend(jsonTable, data and 'true' or 'false')
	elseif data == nil then
		JsonAppend(jsonTable, 'null')
	else
		error('Unexpected type "' .. type(data) .. '" encountered.')
	end
end

function ConvertToJSON(data)
	if type(data) ~= 'table' then
		error('ConvertToJSON only supports table types', 2)
	end

	local jsonTable = {}

	ConvertToJSONInternal(jsonTable, data)

	return table.concat(jsonTable)
end

local function ConvertToJSONInternal1(data)
	if type(data) == 'table' then
		if #data ~= 0 then
			JsonAppend('[')

			for i, value in ipairs(data) do
				ConvertToJSONInternal(value)

				if i ~= #data then
					JsonAppend(', ')
				end
			end

			JsonAppend(']')
		else
			JsonAppend('{')

			local first = true

			for key, value in pairs(data) do
				if type(value) ~= 'function' then
					if first then
						first = false
					else
						JsonAppend(', ')
					end

					JsonAppend('"')
					JsonAppend(key)
					JsonAppend('" : ')

					ConvertToJSONInternal(value)
				end
			end

			JsonAppend('}')
		end
	elseif type(data) == 'number' then
		JsonAppend(tostring(data))
	elseif type(data) == 'string' then
		JsonAppend('"' .. data .. '"')
	elseif type(data) == 'boolean' then
		JsonAppend(data and 'true' or 'false')
	elseif data == nil then
		JsonAppend('null')
	else
		error('Unexpected type "' .. type(data) .. '" encountered.')
	end
end

function ConvertToJSON1(data)
	if type(data) ~= 'table' then
		error('ConvertToJSON only supports table types', 2)
	end

	ConvertToJSONInternal(data)
end
