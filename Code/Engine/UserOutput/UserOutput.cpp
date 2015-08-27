#include "UserOutput.h"

namespace eae6320
{
	namespace UserOutput
	{
		void Print(std::string i_pMessage)
		{
			OutputDebugString(i_pMessage.c_str());
		}
	}
}