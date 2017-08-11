#ifndef _SPMutex_H
#define _SPMutex_H

#include "../ExternalLibs.h"

namespace Pei
{
	namespace Utility
	{
		class SPMutex
		{
			CRITICAL_SECTION criticalSection;
		public:
			SPMutex(){InitializeCriticalSection(&criticalSection); }
			~SPMutex(){DeleteCriticalSection(&criticalSection);}
			inline void Lock(){EnterCriticalSection(&criticalSection);}
			inline void Unlock(){LeaveCriticalSection(&criticalSection);}
		};
	}
}
#endif