#ifndef _SPRenderer_H
#define _SPRenderer_H

#include "../SPResource.h"
using namespace Pei::Utility;

namespace Pei
{
	namespace Graphic
	{
		class SPCamera;
		struct SPVector3;
		class SPRenderer : public SPResource
		{
			friend class             SPTexture;
			friend class             SPTextureStack;
		public:		
			enum FillMode
			{
				FillMode_Solid,
				FillMode_Point,
				FillMode_Wireframe
			};
		private:
			bool                     lightEnabled;
			bool                     alphaBlendEnabled;
			bool                     alphaTestEnabled;
			bool                     textureSamplerEnabled;
			FillMode                 fillMode;
			SPCamera*                camera;
			D3DVIEWPORT9             viewPort;
			LPDIRECT3DSURFACE9       backbuffer;
			LPDIRECT3DSWAPCHAIN9     swapchain;
			D3DPRESENT_PARAMETERS    presentParameters;
			LPDIRECT3DVERTEXBUFFER9  lineBuffer;

			void releaseRes();
			void reloadRes();
		public:
			SPRenderer(HWND window,unsigned int width = 640,unsigned int height = 480,bool fullscreen = false);
			~SPRenderer();

			/// استفاده نکنید
			void Release();

			void StartRendering();
			void StopRendering();

			/// پاک کردن بکبافر و زد بافر
			/// by Zero value backbuffer will cleared with black color
			/// note : use only between StartRendering and StopRendering couple
			void ClearBackBuffer(Pei::Utility::SPColor* color = 0);

			void Fullscreen(bool value = true);
			/// resize
			void ResetBackBufferSize(unsigned int width,unsigned int height);

			void SetWindowHandle(HWND handle);

			void SetFillMode(FillMode mode);
			void SetLightEnable(bool value);
			void SetAlphaTestEnable(bool value);
			void SetAlphaBlendEnable(bool value);
			void SetTextureSamplerEnabled(bool value);

			void DrawLine(SPVector3* moveTo,SPVector3* lineTo,SPColor* color);
			void DrawRegularPolygon(SPVector3* position,float radius,float rotation,unsigned short vertexesCount,SPColor* color);

			// note : the old camera pointer will be lost
			void SetCamera(SPCamera* camera){if(camera)this->camera = camera;}
			SPCamera* GetCamera(){return this->camera;}
		};
	}
}
#endif