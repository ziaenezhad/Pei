#include "SPString.h"

namespace Pei
{
	namespace Utility
	{
		namespace String
		{
			std::string ToString(double input)
			{
				std::stringstream output;
				output<<input;
				return output.str();
			}
		}
	}
}