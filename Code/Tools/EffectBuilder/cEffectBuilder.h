#ifndef EFFECTBUILDER_H
#define EFFECTBUILDER_H

// Header Files
//=============

#include "../BuilderHelper/cbBuilder.h"

// Class Declaration
//==================

namespace eae6320
{
	class cEffectBuilder : public cbBuilder
	{
		// Interface
		//==========

	public:

		// Build
		//------

		virtual bool Build(const std::vector<std::string>& i_arguments);
	};
}

#endif	// EFFECTBUILDER_H