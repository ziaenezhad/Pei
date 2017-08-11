#ifndef _SPTexture_H
#define _SPTexture_H

#include "../Utility/Pei.Utility.h"
#include "../SPResource.h"

using namespace Pei::Utility;
namespace Pei
{
	namespace Graphic
	{
		class SPTexture : public SPResource
		{
			friend class SPResourceManager;
			void releaseRes();
			void reloadRes();
			SPTexture(const char* name = 0);
			~SPTexture();
		public:
			LPDIRECT3DTEXTURE9   heart;
			unsigned int   width;
			unsigned int   height;
		};
	}
}
#endif