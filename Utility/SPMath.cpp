#include "SPMath.h"

namespace Pei
{
	namespace Utility
	{
		float Math::Random()
		{
			return (rand()% 1000)/1000.0f;
		}
	}
}