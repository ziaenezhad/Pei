#include "SPLight.h"

namespace Pei {
	namespace Graphic
	{
		extern LPDIRECT3DDEVICE9   d3dDevice;
		SPLight::SPLight(int lightID, D3DLIGHTTYPE type, D3DXVECTOR3 position, D3DXVECTOR3 direction, double range)
		{
			this->lightID = lightID;
			ZeroMemory( &heart, sizeof(D3DLIGHT9) );

			heart.Diffuse.r = heart.Ambient.r = 1.0f;
			heart.Diffuse.g = heart.Ambient.g = 1.0f;
			heart.Diffuse.b = heart.Ambient.b = 1.0f;
			heart.Diffuse.a = heart.Ambient.a = 1.0f;

			switch(type)
			{
				case D3DLIGHT_POINT:
					heart.Type = D3DLIGHT_POINT;
					heart.Position = position;
					heart.Attenuation0 = 0.1f;
					heart.Range = (float)range;
					break;

				case D3DLIGHT_SPOT:
					heart.Type = D3DLIGHT_SPOT;
					heart.Position = position;
					heart.Direction = direction;
					heart.Range = (float)range;
					heart.Theta = 0.5f;
					heart.Phi = 1.0f;
					heart.Falloff = 1.0f;
					heart.Attenuation0 = 1.0f;
					break;

				case D3DLIGHT_DIRECTIONAL:
				default:
					heart.Type = D3DLIGHT_DIRECTIONAL;
					heart.Range = (float)range;
					//create a normalized direction 
					D3DXVec3Normalize( (D3DXVECTOR3*)&heart.Direction, &direction );
					break;
			}
			Register();
			TurnOn();
		}

		SPLight::~SPLight(){}
		void SPLight::TurnOn()
		{
			if(d3dDevice)
				d3dDevice->LightEnable(lightID,true);
		}
		void SPLight::TurnOff()
		{
			if(d3dDevice)
				d3dDevice->LightEnable(lightID,false);
		}
		void SPLight::Register()
		{
			if(d3dDevice)
				d3dDevice->SetLight(lightID, &heart);
		}
	}
};
