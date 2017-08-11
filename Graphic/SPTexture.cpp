#include "SPTexture.h"
#include "SPRenderer.h"


namespace Pei
{
	namespace Graphic
	{
		extern LPDIRECT3DDEVICE9   d3dDevice;
		SPTexture::SPTexture(const char* name)
			: SPResource(SPResource::Type_Texture)
		{
			this->heart = 0;
			this->width = 0;
			this->height = 0;

			if(name)
			{
				this->name = name;
				this->releaseRes();
				//D3DXIMAGE_INFO info;
				std::string path = SPResourceManager::basePath + name;
				/*if(FAILED(D3DXGetImageInfoFromFileA(path.c_str(),&info)))
					errorHandler.PushError("SPTexture::LoadFromFile",path.c_str(),"Error: GetImageInfoFromFile");
				if(FAILED(D3DXCreateTextureFromFileExA(d3dDevice,path.c_str(),info.Width,info.Height,5,0,D3DFMT_A8R8G8B8,D3DPOOL_DEFAULT,D3DX_DEFAULT,D3DX_DEFAULT,0,&info,0,&this->heart)))			
					errorHandler.PushError("SPTexture::LoadFromFile",path.c_str(),"Error: CreateTextureFromFile");*/
				if(FAILED(D3DXCreateTextureFromFileA(d3dDevice,path.c_str(),&this->heart)))			
					errorHandler.PushError("SPTexture::LoadFromFile",path.c_str(),"Error: CreateTextureFromFile");
				//this->width = info.Width;
				//this->height = info.Height;
			}
		}	
		SPTexture::~SPTexture()
		{
			this->releaseRes();
		}
		void SPTexture::reloadRes()
		{
			if(this->name.length())
			{ 
				std::string path = SPResourceManager::basePath + this->name;				
				/*if(FAILED(D3DXCreateTextureFromFileExA(d3dDevice,path.c_str(),this->width,this->height,5,0,D3DFMT_A8R8G8B8,D3DPOOL_DEFAULT,D3DX_DEFAULT,D3DX_DEFAULT,0,0,0,&this->heart)))			
					errorHandler.PushError("SPTexture::LoadFromFile",path.c_str(),"Error: CreateTextureFromFile");*/
				if(FAILED(D3DXCreateTextureFromFileA(d3dDevice,path.c_str(),&this->heart)))			
					errorHandler.PushError("SPTexture::reloadRes",path.c_str(),"Error: CreateTextureFrom temp File :");
			}
		}
		void SPTexture::releaseRes()
		{
			SAFE_RELEASE(this->heart);
		}
	}
}

//namespace Pei
//{
//	namespace Utility
//	{
//		void Serialize(SPArchive& archive,Pei::Graphic::SPTexture& object)
//		{
//			archive & object.width;
//			archive & object.height;			
//			unsigned int size = 0;
//			switch(archive.ioType)
//			{
//			case SPArchive::IO_Output:
//				{
//					if(object.heart)
//					{
//						LPD3DXBUFFER pBuffer = 0;
//						D3DXSaveTextureToFileInMemory(&pBuffer, D3DXIFF_DDS, object.heart, 0);
//						size = pBuffer->GetBufferSize();
//						archive & size;
//						archive.SaveBinary(pBuffer->GetBufferPointer(), size);
//						SAFE_RELEASE(pBuffer);
//					}
//					else
//						archive & size;
//					break;
//				}
//			case SPArchive::IO_Input:
//				{
//					archive & size;
//					if( size )
//					{
//						char* buffer = new char[size];
//						archive.LoadBinary(buffer,size);
//						if(FAILED(D3DXCreateTextureFromFileInMemoryEx(Pei::Graphic::d3dDevice,buffer,size,object.width,object.height,D3DX_DEFAULT,0,D3DFMT_A8R8G8B8,D3DPOOL_DEFAULT,D3DX_DEFAULT,D3DX_DEFAULT,0,0,0,&object.heart)))
//							errorHandler.PushError("SPTexture::serializeBase_SPTexture","SPTexture","Error: CreateTextureFromFileInMemory.");
//						delete[] buffer;
//					}
//					 break;
//				}
//			}
//		}
//	}
//}