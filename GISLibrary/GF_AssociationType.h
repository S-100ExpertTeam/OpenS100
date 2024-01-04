#pragma once

#include "GF_AssociationRole.h"

namespace GF
{

	class AssociationType :
		public NamedType
	{
	public:
		AssociationType();
		AssociationType(const AssociationType& other);
		virtual ~AssociationType();

	public:
		AssociationRole role;

	public:
		virtual AssociationType operator=(const AssociationType& other);

	public:
		std::string GetRole();
		void SetRole(std::string code);
	};

}