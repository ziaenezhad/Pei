#ifndef _SPSerializable_H
#define _SPSerializable_H

#include "../ExternalLibs.h"

namespace Pei {
	namespace Utility
	{
		class SPArchive;
		/// کلاس هایی که از این کلاس ارث می برند می توانند مستقیما به/از آرشیو منتقل شوند
		class SPSerializable
		{
		public:
			bool splitedSerialization;
			virtual void serialize(SPArchive& archive){}
			SPSerializable(){this->splitedSerialization = false;}
		};
	}
}
#endif