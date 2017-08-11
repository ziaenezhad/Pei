#ifndef _SPLight_H
#define _SPLight_H

#include "../ExternalLibs.h"
namespace Pei {
	namespace Graphic
	{
		class SPLight
		{
		public:
			D3DLIGHT9		heart;
			D3DLIGHTTYPE	type;
			int				lightID;

			SPLight(int lightID, D3DLIGHTTYPE type, D3DXVECTOR3 position, D3DXVECTOR3 direction, double range);
			~SPLight();

			void Register();
			void setColor(D3DCOLORVALUE color) { heart.Diffuse = color; };
			D3DCOLORVALUE getColor() { return heart.Diffuse; }
			void TurnOn();
			void TurnOff();

		};
	}
}; //namespace

#endif