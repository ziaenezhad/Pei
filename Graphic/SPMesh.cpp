#include "SPMesh.h"
#include "SPTexture.h"
#include "SPRenderingParameters.h"

namespace Pei
{
	namespace Graphic
	{
		extern LPDIRECT3DDEVICE9   d3dDevice;
		//-----------------------------------
		/*extern SPRenderer* renderer;
		LPDIRECT3DVERTEXSHADER9 vertexShader = 0;
		LPD3DXCONSTANTTABLE constTable = 0;
		D3DXHANDLE viewMatrixHandel = 0;*/
		//-----------------------------------
		SPMesh::SPMesh(const char* name)
			: SPResource(SPResource::Type_Mesh)
		{
			this->heart = 0;
			this->texture = 0;
			this->subsetsCount = 0;
			this->currentSubsetIndex = -1;

			if(name)
			{
				this->name = name;
				this->releaseRes();
				LPD3DXBUFFER materialsBuffer = 0;
				DWORD numMaterials = 0;
				std::string path = SPResourceManager::basePath + name;
				if(FAILED(D3DXLoadMeshFromX(path.c_str(),D3DXMESH_32BIT,d3dDevice,0,&materialsBuffer,0,&numMaterials,&this->heart)))
					errorHandler.PushError("SPMesh::LoadFromXFile",path.c_str(),"Error: LoadMeshFromX");
				this->subsetsCount = numMaterials;
				D3DXMATERIAL* materials = (D3DXMATERIAL*)materialsBuffer->GetBufferPointer();
				/*this->material = materials[0].MatD3D;
				this->material .Ambient = this->material.Diffuse;*/
				this->texture = SPResourceManager::GetTexture(materials[0].pTextureFilename);
				materialsBuffer->Release();

				//-----------------------------------
				/*LPD3DXBUFFER shader = 0;
				LPD3DXBUFFER errors = 0;
				if(FAILED(D3DXCompileShaderFromFile("shader.cpp",0,0,"Main","vs_2_0",D3DXSHADER_DEBUG,&shader,&errors,&constTable)))
					MessageBox(0,0,0,0);

				if(FAILED(d3dDevice->CreateVertexShader((DWORD*)shader->GetBufferPointer(),&vertexShader)))
					MessageBox(0,0,0,0);

				viewMatrixHandel = constTable->GetConstantByName(0,"ViewProjMatrix");
				constTable->SetDefaults(d3dDevice);*/
				//-----------------------------------
			}
		}
		SPMesh::~SPMesh()
		{
			this->releaseRes();
		}
		void SPMesh::releaseRes()
		{
			SAFE_RELEASE(this->heart);
		}
		void SPMesh::reloadRes()
		{
			if(this->name.length())
			{
				this->releaseRes();
				std::string path = SPResourceManager::basePath + name;
				if(FAILED(D3DXLoadMeshFromX(path.c_str(),D3DXMESH_32BIT,d3dDevice,0,0,0,0,&this->heart)))
					errorHandler.PushError("SPMesh::reloadRes",path.c_str(),"Error: LoadMeshFromX");
			}
		}
		void SPMesh::Render(SPRenderingParameters *rp)
		{
			if(this->heart)
			{
				D3DXMATRIX mWorld;
				D3DXMATRIX mTrans;
				D3DXMATRIX mRotate;
				D3DXMATRIX mScale;
				D3DXMatrixScaling(&mScale,rp->global_scale.x,rp->global_scale.y,rp->global_scale.z);
				D3DXMatrixTranslation( &mTrans,rp->global_position.x,rp->global_position.y,rp->global_position.z);
				D3DXMatrixRotationYawPitchRoll(&mRotate,rp->global_rotation.y,rp->global_rotation.x,rp->global_rotation.z);
				mWorld = mRotate*mScale*mTrans;
				d3dDevice->SetTransform( D3DTS_WORLD, &mWorld );
				if(rp->global_blendMode)
				{
					switch(rp->global_blendMode)
					{
					case SPRenderingParameters::BlendingMode_LinearDodge:
						d3dDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
						d3dDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);
						break;
					case SPRenderingParameters::BlendingMode_Darken:
						d3dDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_MIN);
						d3dDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_ONE);
						d3dDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);
						break;
					case SPRenderingParameters::BlendingMode_Lighten:
						d3dDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_MAX);
						d3dDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_ONE);
						d3dDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);
						break;
					case SPRenderingParameters::BlendingMode_Multiply:
						d3dDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_DESTCOLOR);
						d3dDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ZERO);
						break;
					case SPRenderingParameters::BlendingMode_Screen:
						d3dDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_INVDESTCOLOR);
						d3dDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);
						break;
					}
				}
				//----------------------------------------------
				d3dDevice->SetMaterial(&rp->global_material);
				d3dDevice->SetTexture(0,this->texture->heart);
				//---------------------------------------------
				/*D3DXMATRIX v;
				D3DXMATRIX p;
				D3DXMATRIX vp;
				d3dDevice->GetTransform(D3DTS_VIEW,&v);
				d3dDevice->GetTransform(D3DTS_PROJECTION,&p);
				vp = v*p;
				constTable->SetMatrix(d3dDevice,viewMatrixHandel,&vp);
				d3dDevice->SetVertexShader(vertexShader);*/
				//---------------------------------------------
				if(this->currentSubsetIndex == -1)
					for(unsigned int i = 0;i < this->subsetsCount;i++)
						this->heart->DrawSubset(i);
				else
					this->heart->DrawSubset(this->currentSubsetIndex);
				if(rp->global_blendMode)
				{
					d3dDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
					d3dDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
					d3dDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
				}
			}
		}
		bool SPMesh::Pick(POINT *screenPoint, SPRenderingParameters *rp)
		{
			D3DXMATRIX mWorld;
			D3DXMATRIX mTrans;
			D3DXMATRIX mRotate;
			D3DXMATRIX mScale;
			D3DXMatrixScaling(&mScale,rp->global_scale.x,rp->global_scale.y,rp->global_scale.z);
			D3DXMatrixTranslation( &mTrans,rp->global_position.x,rp->global_position.y,rp->global_position.z);
			D3DXMatrixRotationYawPitchRoll(&mRotate,rp->global_rotation.y,rp->global_rotation.x,rp->global_rotation.z);
			mWorld = mRotate*mScale*mTrans;
			d3dDevice->SetTransform( D3DTS_WORLD, &mWorld );

			D3DVIEWPORT9 vp;
			d3dDevice->GetViewport(&vp);
			D3DMATRIX matProj;
			d3dDevice->GetTransform(D3DTS_PROJECTION,&matProj);
			D3DXVECTOR3 v;
			v.x =  ( ( ( 2.0f * screenPoint->x ) / vp.Width  ) - 1 ) / matProj._11;
			v.y = -( ( ( 2.0f * screenPoint->y ) / vp.Height ) - 1 ) / matProj._22;
			v.z =  1.0f;

			D3DXMATRIX matView;
			d3dDevice->GetTransform(D3DTS_VIEW,&matView);
			D3DXMATRIX m;
			D3DXVECTOR3 rayOrigin,rayDir;
			D3DXMatrixInverse( &m, NULL, &matView );
			// Transform the screen space pick ray into 3D space
			rayDir.x  = v.x*m._11 + v.y*m._21 + v.z*m._31;
			rayDir.y  = v.x*m._12 + v.y*m._22 + v.z*m._32;
			rayDir.z  = v.x*m._13 + v.y*m._23 + v.z*m._33;
			rayOrigin.x = m._41;
			rayOrigin.y = m._42;
			rayOrigin.z = m._43;

			// Use inverse of matrix
			D3DXMATRIX matWorld;
			d3dDevice->GetTransform(D3DTS_WORLD,&matWorld);
			D3DXMATRIX matInverse;
			D3DXMatrixInverse(&matInverse,NULL,&matWorld);

			// Transform ray origin and direction by inv matrix
			D3DXVECTOR3 rayObjOrigin,rayObjDirection;

			D3DXVec3TransformCoord(&rayObjOrigin,&rayOrigin,&matInverse);
			D3DXVec3TransformNormal(&rayObjDirection,&rayDir,&matInverse);
			D3DXVec3Normalize(&rayObjDirection,&rayObjDirection);

			BOOL hasHit;
			float distanceToCollision;

			D3DXIntersect(this->heart, &rayObjOrigin, &rayObjDirection, &hasHit, NULL, NULL, NULL, &distanceToCollision, NULL, NULL);
			if(hasHit)
				return true;
			else
				return false;
		}
		/*void SPMesh::Save(const char* name)
		{
			if(name)
			{
				if(this->name.length() == 0)
					this->name = name;
				D3DXMATERIAL* materials = new D3DXMATERIAL[this->subsetsCount];
				SPMaterial material;
				for(unsigned int i = 0;i < this->subsetsCount; i++)
				{
					materials[i].MatD3D = material;
					materials[i].pTextureFilename = (LPSTR)this->texture->name.c_str();
				}
				std::string path = SPResourceManager::basePath + name;
				D3DXSaveMeshToX(path.c_str(),this->heart,0,materials,0,this->subsetsCount,D3DXF_FILEFORMAT_BINARY);
				delete[]  materials;
			}
		}
		void SPMesh::CreateBillboard(SPTexture* texture,SPRect* rect,SPVector3* centerPoint)
		{
			if(texture)
			{
				this->Release();
				bool flag_r = false;
				bool flag_c = false;
				if(!rect)
				{
					flag_r =true;
					rect = new SPRect(0,0,texture->width,texture->height);
				}
				if(!centerPoint)
				{
					flag_c = true;
					centerPoint = new SPVector3(texture->width / 2.0f , texture->height / 2.0f , 0);
					this->centerPoint.Set(0,0,0);
				}
				else
				{
					this->centerPoint = *centerPoint;
				}
				//----------------------------------
				this->texture = texture;
				if(FAILED(D3DXCreateMeshFVF(2,4,D3DXMESH_32BIT,SP_FVF_PNDT,d3dDevice,&this->heart)))
					errorHandler.PushError("SPMesh::CreateBillboard",0,"Error: CreateMeshFVF");
							
				//----------------------------------
				SPVertex_PNDT* vertexes;
				this->heart->LockVertexBuffer(0,(LPVOID*)&vertexes);
				for(int i=0;i<4;i++)
				{
					vertexes[i].diffuse = 0xFFFFFFFF;
					vertexes[i].nx = 0;	vertexes[i].ny = 0;	vertexes[i].nz = 1;
					vertexes[i].z = 0;
				}				
				vertexes[0].x = -centerPoint->x;
				vertexes[0].y = centerPoint->y - texture->height;
				vertexes[1].x = vertexes[0].x;
				vertexes[1].y = centerPoint->y;
				vertexes[2].x = texture->width - centerPoint->x;
				vertexes[2].y = vertexes[0].y;
				vertexes[3].x = vertexes[2].x;
				vertexes[3].y = centerPoint->y;

				vertexes[0].u = rect->left/(float)texture->width;
				vertexes[0].v = rect->bottom/(float)texture->height;				
				vertexes[1].u = vertexes[0].u;
				vertexes[1].v = rect->top/(float)texture->height;				
				vertexes[2].u = rect->right/(float)texture->width;
				vertexes[2].v = vertexes[0].v ;				
				vertexes[3].u = vertexes[2].u;
				vertexes[3].v = vertexes[1].v;
				this->heart->UnlockVertexBuffer();
				if(flag_r) SAFE_DELETE(rect);
				if(flag_c) SAFE_DELETE(centerPoint);
				
				//----------------------------------
				DWORD* data = 0;
				this->heart->LockIndexBuffer(0,(LPVOID*)&data);
				data[0] = 0;data[1] = 1;data[2] = 3;
				data[3] = 0;data[4] = 3;data[5] = 2;
				this->heart->UnlockIndexBuffer();

				//----------------------------------
				DWORD* attributes = 0;
				this->heart->LockAttributeBuffer(0,&attributes);
				attributes[0] = 0;
				attributes[1] = 0;
				this->heart->UnlockAttributeBuffer();
				this->subsetsCount = 2;
			}
			else
				errorHandler.PushEvent("SPMesh::CreateBillboard",0,"input texture is Null !");
		}
		void  SPMesh::SetCenterPoint(SPVector3* value)
		{
			if(this->heart)
			{
				if( this->heart->GetFVF() == SP_FVF_PNDT )
				{
					SPVertex_PNDT* vertexes;
					this->heart->LockVertexBuffer(0,(LPVOID*)&vertexes);
					for(unsigned int i=0; i < this->heart->GetNumVertices(); i++)
					{
						vertexes[i].x += value->x - this->centerPoint.x;
						vertexes[i].y += value->y - this->centerPoint.y;
						vertexes[i].z += value->z - this->centerPoint.z;
					}
					this->heart->UnlockVertexBuffer();
					this->centerPoint = *value;
				}
			}
		}*/
	}
}

//namespace Pei
//{
//	namespace Utility
//	{
//		void Serialize(SPArchive& archive,Pei::Graphic::SPMesh& object)
//		{
//			DWORD value = 0;
//			DWORD* data = 0;
//			unsigned int size = 0;
//			switch(archive.ioType)
//			{
//			case SPArchive::IO_Output:
//				{
//					if(object.heart)
//					{
//			//_____________________________________________
//						//1 . Saving Mesh
//						value = object.heart->GetFVF();
//						archive & value;
//						value = object.heart->GetNumFaces();
//						archive & value;
//						value = object.heart->GetNumVertices();
//						archive & value;
//						//-------------------------------------------------
//						//1-1 . Saving Vertexes
//						size = object.heart->GetNumVertices() * object.heart->GetNumBytesPerVertex();
//						object.heart->LockVertexBuffer(0,(LPVOID*)&data);
//						if(data)
//							archive.SaveBinary(data,size);
//						object.heart->UnlockVertexBuffer();
//						//-------------------------------------------------
//						//1-2 . Saving Indexes
//						object.heart->LockIndexBuffer(0,(LPVOID*)&data);
//						size = object.heart->GetNumFaces() * 3;
//						if(data)
//							archive.SaveBinary(data,size*sizeof(DWORD));				
//						object.heart->UnlockIndexBuffer();
//						//-------------------------------------------------
//						//1-3 . Saving Attributes
//						data = 0;
//						object.heart->LockAttributeBuffer(0,&data);
//						size = object.heart->GetNumFaces();
//						if(data)
//							archive.SaveBinary(data,size*sizeof(DWORD));				
//						object.heart->UnlockAttributeBuffer();
//			//_____________________________________________
//						//2 . Saving Textures And Materials
//						archive & object.textures;			
//						archive & object.materials;
//					}
//					break;
//				}
//			case SPArchive::IO_Input:
//				{
//					object.Release();
//					DWORD fvf,numFaces,numVertixes;
//					archive & fvf;
//					archive & numFaces;
//					archive & numVertixes;
//					if(FAILED(D3DXCreateMeshFVF(numFaces,numVertixes,D3DXMESH_32BIT,fvf,Pei::Graphic::d3dDevice,&object.heart)))
//					errorHandler.PushError("Serialize::IO_Input",0,"Error: CreateMeshFVF");
//					
//					size = object.heart->GetNumBytesPerVertex() * numVertixes;
//					object.heart->LockVertexBuffer(0,(LPVOID*)&data);
//					archive.LoadBinary(data,size);
//					object.heart->UnlockVertexBuffer();
//
//					size = numFaces * 3;
//					object.heart->LockIndexBuffer(0,(LPVOID*)&data);
//					archive.LoadBinary(data,size*sizeof(DWORD));
//					object.heart->UnlockIndexBuffer();
//
//					size = numFaces;
//					object.heart->LockAttributeBuffer(0,&data);
//					archive.LoadBinary(data,size*sizeof(DWORD));
//					object.heart->UnlockAttributeBuffer();
//					
//					archive & object.textures;
//					archive & object.materials;
//					break;
//				}
//			}
//		}
//	}
//}