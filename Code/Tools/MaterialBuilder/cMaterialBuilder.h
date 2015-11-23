#ifndef EAE6320_MATERIALBUILDER_H
#define EAE6320_MATERIALBUILDER_H

// Header Files
//=============

#include "../BuilderHelper/cbBuilder.h"

// Class Declaration
//==================

namespace eae6320
{
	class cMaterialBuilder : public cbBuilder
	{
		// Interface
		//==========

	public:

		// Build
		//------

		virtual bool Build(const std::vector<std::string>& i_arguments);
	};
}

#endif	// EAE6320_MATERIALBUILDER_H