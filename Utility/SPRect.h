#ifndef _SPRect_H
#define _SPRect_H

#include "SPArchive.h"
#include "../ExternalLibs.h"

namespace Pei {
	namespace Utility
	{
		struct SPRect : public RECT,public SPSerializable
		{
		protected:
			unsigned int width;
			unsigned int height;
			void serializeBase_SPRect(SPArchive& archive);
			virtual void serialize(SPArchive& archive);
		public:
			SPRect();
			SPRect(long x,long y,long width,long height);

			void Set(long x,long y,long width,long height);

			inline unsigned int& GetWidth(){return this->width;}
			inline unsigned int& GetHeight(){return this->height;}

			bool IsInside(float x,float y);

		};
	}
}
#endif
