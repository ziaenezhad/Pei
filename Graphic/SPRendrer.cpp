
#include "SPMesh.h"
#include "SPCamera.h"
#include "SPRenderer.h"
#include <boost/thread/mutex.hpp>

using namespace Pei::Utility;

namespace Pei
{
	namespace Graphic
	{
		int						   renderersCounter        = 0;
		LPDIRECT3D9                direct3D                = NULL;
		LPDIRECT3DDEVICE9          d3dDevice               = NULL;
		LPD3DXSPRITE	           spriteHandler           = NULL;
		DWORD                      d3dDeviceBehaviorFlags  = 0;	
		SPRenderer*                fullscreened            = 0;
		D3DCAPS9                   d3dCaps;
		D3DDISPLAYMODE             displayMode;
		D3DLIGHT9                  light;
		
		boost::mutex               mutex;
		//_________________________________________________________________________________
		//---------------------------------------------------------------------------------
		//_________________________________________________________________________________
		//---------------------------------------------------------------------------------
		SPRenderer::SPRenderer(HWND window, unsigned int width, unsigned int height, bool fullscreen)
			: SPResource(SPResource::Type_Renderer)
		{	
			mutex.lock();
			if(fullscreen)
				fullscreened = this;

			this->camera = new SPCamera(width,height);
			this->lightEnabled = false;
			this->alphaTestEnabled = false;
			this->alphaBlendEnabled = false;
			this->textureSamplerEnabled = false;
			this->backbuffer = 0;

			ZeroMemory(&this->presentParameters, sizeof(this->presentParameters));
			this->presentParameters.BackBufferWidth        = width;
			this->presentParameters.BackBufferHeight       = height;
			this->presentParameters.hDeviceWindow          = window;
			this->presentParameters.Windowed               = !fullscreen;
			this->presentParameters.SwapEffect             = D3DSWAPEFFECT_DISCARD;
			this->presentParameters.AutoDepthStencilFormat = D3DFMT_D16;
			this->presentParameters.BackBufferFormat       = displayMode.Format;
			this->presentParameters.EnableAutoDepthStencil = true;
			this->presentParameters.MultiSampleType        = D3DMULTISAMPLE_NONE;
			//this->presentParameters.MultiSampleType        = D3DMULTISAMPLE_4_SAMPLES;


			if( ! direct3D )
			{				
				direct3D = Direct3DCreate9( D3D_SDK_VERSION );
				if( ! direct3D )
					errorHandler.PushError("SPRenderer::SPRenderer","SPRenderer","Error cant Create Direct3D");
				if(FAILED(direct3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &displayMode )))
					errorHandler.PushError("SPRenderer::SPRenderer","SPRenderer","Error cant GetAdapterDisplayMode");
				if(direct3D->CheckDeviceFormat(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,displayMode.Format,D3DUSAGE_DEPTHSTENCIL,D3DRTYPE_SURFACE,D3DFMT_D16) == D3DERR_NOTAVAILABLE )
					errorHandler.PushError("SPRenderer::SPRenderer","SPRenderer","Error We need at least a 16-bit z-buffer! this device cant support .");
				if(FAILED(direct3D->GetDeviceCaps( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3dCaps )))
					errorHandler.PushError("SPRenderer::SPRenderer","SPRenderer","Error cant GetDeviceCaps");
				if(d3dCaps.VertexProcessingCaps)
					d3dDeviceBehaviorFlags |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
				else
					d3dDeviceBehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
			}
			if( ! d3dDevice )
			{
				if(FAILED(direct3D->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,window,d3dDeviceBehaviorFlags,&this->presentParameters,&d3dDevice)))
					errorHandler.PushError("SPRenderer::SPRenderer","SPRenderer","Error cant CreateDevice");
				if(FAILED(d3dDevice->GetSwapChain(0,&this->swapchain)))
					errorHandler.PushError("SPRenderer::SPRenderer","SPRenderer","Error cant Get Zeor Level SwapChain");
			}
			else
			{
				if(FAILED(d3dDevice->CreateAdditionalSwapChain(&this->presentParameters,&this->swapchain)))
					errorHandler.PushError("SPRenderer::SPRenderer","SPRenderer","Error cant CreateAdditionalSwapChain");
			}
			if(FAILED(d3dDevice->CreateVertexBuffer(2*sizeof(SPVertex_PD),D3DUSAGE_DYNAMIC,SP_FVF_PD,D3DPOOL_DEFAULT,&this->lineBuffer,0)))
				errorHandler.PushError("SPRenderer::SPRenderer","SPRenderer","Error cant CreateLineBuffer");
			if(FAILED(this->swapchain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &this->backbuffer)))
				errorHandler.PushError("SPRenderer::SPRenderer","SPRenderer","Error cant GetBackBuffer");
			d3dDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE );
			renderersCounter++;
			mutex.unlock();
		}
		SPRenderer::~SPRenderer()
		{
			this->Release();
		}
		void SPRenderer::Release()
		{
			this->releaseRes();
			if( -- renderersCounter == 0 )
			{
				//SPResourceManager::releaseAllResources();
				SAFE_RELEASE(d3dDevice);
				SAFE_RELEASE(direct3D);
			}
		}
		void SPRenderer::reloadRes()
		{
			errorHandler.PushEvent("SPRenderer::reload","SPRenderer","a Renderer going to reloaded");
			if(fullscreened)
			{
				if(fullscreened != this)
					return;
				if(FAILED(d3dDevice->GetSwapChain(0,&this->swapchain)))
					errorHandler.PushError("SPRenderer::reload","SPRenderer","Error cant Get Zeor Level SwapChain");
			}
			else
			{
				if(FAILED(d3dDevice->CreateAdditionalSwapChain(&this->presentParameters,&this->swapchain)))
					errorHandler.PushError("SPRenderer::reload","SPRenderer","Error cant CreateAdditionalSwapChain");
			}			
			if(FAILED(d3dDevice->CreateVertexBuffer(2*sizeof(SPVertex_PD),D3DUSAGE_DYNAMIC,SP_FVF_PD,D3DPOOL_DEFAULT,&this->lineBuffer,0)))
				errorHandler.PushError("SPRenderer::reload","SPRenderer","Error cant CreateLineBuffer");
			if(FAILED(this->swapchain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &this->backbuffer)))
				errorHandler.PushError("SPRenderer::reload","SPRenderer","Error cant GetBackBuffer");
			//_________________________________________________________________________________
			//Resting States :
			this->SetFillMode(this->fillMode);
			this->SetLightEnable(this->lightEnabled);
			this->SetAlphaTestEnable(this->alphaTestEnabled);
			this->SetAlphaBlendEnable(this->alphaBlendEnabled);
			this->SetTextureSamplerEnabled(this->textureSamplerEnabled);
		}
		void SPRenderer::releaseRes()
		{
			errorHandler.PushEvent("SPRenderer::Release","SPRenderer","a Renderer going to Release");
			SAFE_RELEASE(this->swapchain);
			SAFE_RELEASE(this->backbuffer);
			SAFE_RELEASE(this->lineBuffer);
		}

		void SPRenderer::ResetBackBufferSize(unsigned int width, unsigned int height)
		{
			Sleep(100);
			this->presentParameters.BackBufferWidth  = width;
			this->presentParameters.BackBufferHeight = height;

			LPDIRECT3DSWAPCHAIN9 zeroSwap = 0;
			d3dDevice->GetSwapChain(0,&zeroSwap);
			D3DPRESENT_PARAMETERS dpp;
			zeroSwap->GetPresentParameters(&dpp);

			SPResourceManager::releaseAllResources();
			if(fullscreened)
			{
				if(FAILED(d3dDevice->Reset(&fullscreened->presentParameters)))
					errorHandler.PushError("SPRenderer::ResetBackBufferSize","SPRenderer","Error cant ResetDevice");
			}
			else
			{
				dpp.Windowed = true;
				if(FAILED(d3dDevice->Reset(&dpp)))
					errorHandler.PushError("SPRenderer::ResetBackBufferSize","SPRenderer","Error cant ResetDevice");
			}
			SPResourceManager::reloadAllResources();
			//_________________________________________________________________________________
			//Resting States :
			d3dDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE );
			//---------------------------------------------------------------------------------
			errorHandler.PushEvent("SPRenderer::ResetBackBufferSize","SPRenderer","Device Reseted");
		}

		void SPRenderer::Fullscreen(bool value)
		{
			if(this->presentParameters.Windowed != !value)				
			{
				if(!this->presentParameters.Windowed)
					fullscreened = 0;
				else
				{
					if(fullscreened)
						fullscreened->Fullscreen(false);
					fullscreened = this;
				}
				this->presentParameters.Windowed = !value;
				this->ResetBackBufferSize(this->presentParameters.BackBufferWidth,this->presentParameters.BackBufferHeight);
			}
		}
		void SPRenderer::StartRendering()
		{
			if(this->swapchain)
			{	
				mutex.lock();	
				d3dDevice->SetTransform( D3DTS_VIEW, &this->camera->matView);
				d3dDevice->SetTransform( D3DTS_PROJECTION, &this->camera->matProjection);

				d3dDevice->SetRenderTarget(0,this->backbuffer);
				d3dDevice->BeginScene();
			}
		}

		void SPRenderer::StopRendering()
		{
			if(this->swapchain)
			{
				d3dDevice->EndScene();	
				if(this->swapchain->Present(0,0,this->presentParameters.hDeviceWindow,0,0) == D3DERR_DEVICELOST)
					if( d3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET )
						this->ResetBackBufferSize(this->presentParameters.BackBufferWidth,this->presentParameters.BackBufferHeight);		
				mutex.unlock();
			}
		}		
		void SPRenderer::ClearBackBuffer(Pei::Utility::SPColor* color)
		{
			if(this->swapchain)
			{
				if(!color)
					d3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,0, 1.0f, 0);
				else
					d3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, color->value, 1.0f, 0);
			}
		}
		void SPRenderer::SetWindowHandle(HWND handle)
		{
			this->presentParameters.hDeviceWindow = handle;
		}

		void SPRenderer::SetLightEnable(bool value)
		{
			this->lightEnabled = value;
			d3dDevice->SetRenderState(D3DRS_LIGHTING,value);
			if(value)
			{					
				ZeroMemory(&light,sizeof(light));
				light.Type = D3DLIGHT_DIRECTIONAL;
				light.Diffuse.r = 1;light.Diffuse.g = 1;light.Diffuse.b = 1;light.Diffuse.a = 1;
				light.Ambient = light.Diffuse;
				light.Specular = light.Diffuse;
				light.Direction.x= 0.5f;
				light.Direction.y= 0.5f;
				light.Direction.z= 0.5f;
				d3dDevice->SetLight(0,&light);
				d3dDevice->LightEnable(0,true);
			}
		}
		void SPRenderer::SetAlphaBlendEnable(bool value)
		{
			this->alphaBlendEnabled = value;
			d3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,value);
			if(value)
			{
				d3dDevice->SetRenderState( D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL );
				d3dDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
				d3dDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
				d3dDevice->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);
				d3dDevice->SetTextureStageState(0,D3DTSS_ALPHAARG2,D3DTA_DIFFUSE);
				d3dDevice->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_MODULATE);
				d3dDevice->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);
				d3dDevice->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_DIFFUSE);
				d3dDevice->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_MODULATE);
			}
		}
		void SPRenderer::SetAlphaTestEnable(bool value)
		{
			this->alphaTestEnabled = value;
			d3dDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
			d3dDevice->SetRenderState(D3DRS_ALPHAREF, 0x00);
			d3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, value);
		}
		void SPRenderer::SetTextureSamplerEnabled(bool value)
		{
			this->textureSamplerEnabled = value;
			if(value)
			{
				d3dDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
				d3dDevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
				d3dDevice->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_LINEAR);

			}
			else
			{
				d3dDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_NONE);
				d3dDevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_NONE);
				d3dDevice->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_NONE);
			}
		}
		void SPRenderer::SetFillMode(FillMode mode)
		{
			this->fillMode = mode;
			switch(mode)
			{
			case FillMode_Point:
				d3dDevice->SetRenderState(D3DRS_FILLMODE,D3DFILL_POINT);
				break;
			case FillMode_Solid:
				d3dDevice->SetRenderState(D3DRS_FILLMODE,D3DFILL_SOLID);
				break;
			case FillMode_Wireframe:
				d3dDevice->SetRenderState(D3DRS_FILLMODE,D3DFILL_WIREFRAME);
				break;
			}
		}
		void SPRenderer::DrawLine(SPVector3* moveTo,SPVector3* lineTo,SPColor* color)
		{		
			SPVertex_PD* data = 0;
			this->lineBuffer->Lock(0,2*sizeof(SPVertex_PD),(void**)&data,0);
			data[0].x = moveTo->x;
			data[0].y = moveTo->y;
			data[0].z = moveTo->z;
			data[0].diffuse = color->value;
			data[1].diffuse = color->value;
			data[1].z = lineTo->z;
			data[1].y = lineTo->y;
			data[1].x = lineTo->x;
			this->lineBuffer->Unlock();
			D3DXMATRIX translation;
			D3DXMatrixTranslation(&translation,0,0,0);
			d3dDevice->SetTransform(D3DTS_WORLD,&translation);
			d3dDevice->SetTexture(0,0);
			d3dDevice->SetStreamSource( 0, this->lineBuffer,0,sizeof(SPVertex_PD));
			d3dDevice->SetFVF( SP_FVF_PD );
			d3dDevice->DrawPrimitive( D3DPT_LINELIST,0, 1);
		}
		void SPRenderer::DrawRegularPolygon(SPVector3* position,float radius,float rotation,unsigned short vertexesCount,SPColor* color)
		{
			SPVector3 moveTo = *position ;
			SPVector3 lineTo;
			for(int i=0;i<vertexesCount+1;i++)
			{
				lineTo.Set(cos(rotation) * radius + position->x , sin(rotation) * radius + position->y ,0);
				this->DrawLine(&moveTo,&lineTo,color);
				moveTo = lineTo;
				rotation +=(float)( 2 * Math::PI /vertexesCount );
			}
		}
	}
}