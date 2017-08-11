#ifndef _SPVector3_H
#define _SPVector3_H

#include "../Utility/Pei.Utility.h"

using namespace Pei::Utility;

namespace Pei{
	namespace Graphic
	{
		struct SPVector3 : public D3DXVECTOR3 , public SPSerializable
		{
			void serialize(SPArchive& archive){archive & this->x; archive & this->y; archive & this->z;}
		public:
			SPVector3(){this->x=0;this->y=0;this->z=0;}
			SPVector3(float x,float y,float z){this->Set(x,y,z);}
			inline void Set(float x,float y,float z){this->x=x;this->y=y;this->z=z;}			
			inline void operator*=(const SPVector3& v){this->x *= v.x; this->y*=v.y;this->z*=v.z;}
			inline void operator=(const D3DXVECTOR3& v){this->x = v.x; this->y=v.y;this->z=v.z;}
		};
	}
}
#endif