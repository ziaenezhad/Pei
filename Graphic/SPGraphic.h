#ifndef _SPGraphic_H
#define _SPGraphic_H

namespace Pei
{
	namespace Graphic
	{
		struct SPRenderingParameters;
		class SPGraphic abstract
		{
		public:
			enum Types : unsigned char
			{
				Type_None,
				Type_Mesh,
				Type_FlashPlayer,
				Type_VideoPlayer,
				Type_ParticleSystem,
			};
			Types type;
			virtual void Render(SPRenderingParameters* rp) = 0;
			virtual bool Pick(POINT* screenPoint,SPRenderingParameters* rp) = 0;
		};
	}
}
#endif