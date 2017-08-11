#include "SPCamera.h"

namespace Pei
{
	namespace Graphic
	{
		SPCamera::SPCamera(unsigned int width,unsigned int height,ProjectionTypes projectionType)
		{
			this->up.Set(0,1,0);
			this->look.Set(0,0,1);
			this->position.Set(0,0,-1);
			this->right.Set(1,0,0);
			this->width = width;
			this->height = height;
			this->SetProjection(projectionType);
			this->UpdateView();
		}
		void SPCamera::resetProjection()
		{
			if( this->projectionType == ProjectionType_Perspective)
				D3DXMatrixPerspectiveFovLH( &this->matProjection, D3DXToRadian( angle ),width/(float)height, nearClippingPlane, farClippingPlane );
			else
				D3DXMatrixOrthoLH(&this->matProjection,(float)width,(float)height,nearClippingPlane,farClippingPlane);
		}
		void SPCamera::SetProjection(ProjectionTypes type,float angle,float nearClippingPlane,float farClippingPlane)
		{
			this->angle = angle;
			this->projectionType = type;
			this->farClippingPlane = farClippingPlane;
			this->nearClippingPlane = nearClippingPlane;
			this->resetProjection();
		}
		void SPCamera::SetCameraSize(unsigned int width,unsigned int height)
		{
			this->width = width;
			this->height = height;
			this->resetProjection();
		}
		void SPCamera::UpdateView()
		{
			//D3DXMatrixIdentity( &this->matView );

			D3DXVec3Normalize( &this->look, &this->look );
			D3DXVec3Cross( &this->up, &this->look, &this->right );
			D3DXVec3Normalize( &this->up, &this->up );
			D3DXVec3Cross( &this->right ,&this->up, &this->look);
			D3DXVec3Normalize( &this->right, &this->right );

			this->matView._11 = this->right.x;
			this->matView._12 = this->up.x;
			this->matView._13 = this->look.x;
			this->matView._14 = 0.0f;

			this->matView._21 = this->right.y;
			this->matView._22 = this->up.y;
			this->matView._23 = this->look.y;
			this->matView._24 = 0.0f;

			this->matView._31 = this->right.z;
			this->matView._32 = this->up.z;
			this->matView._33 = this->look.z;
			this->matView._34 = 0.0f;

			this->matView._41 = -D3DXVec3Dot( &this->right,&this->position );
			this->matView._42 = -D3DXVec3Dot( &this->up, &this->position);
			this->matView._43 = -D3DXVec3Dot( &this->look, &this->position);
			this->matView._44 =  1.0f;
		}
		void SPCamera::Pitch(float angle)
		{
			D3DXMATRIX matRotation;
			D3DXMatrixRotationAxis( &matRotation, &this->right,angle );
			D3DXVec3TransformCoord( &this->look, &this->look, &matRotation );
			D3DXVec3TransformCoord( &this->up, &this->up, &matRotation );
			this->UpdateView();
		}
		void SPCamera::Roll(float angle)
		{		
			D3DXMATRIX matRotation;
			D3DXMatrixRotationAxis( &matRotation, &this->look,angle );
			D3DXVec3TransformCoord( &this->up, &this->up, &matRotation );
			D3DXVec3TransformCoord( &this->right, &this->right, &matRotation );
			this->UpdateView();
		}
		void SPCamera::Yaw(float angle)
		{	
			this->up.Set(0,1,0);
			D3DXMATRIX matRotation;
			D3DXMatrixRotationAxis( &matRotation, &this->up,angle );
			D3DXVec3TransformCoord( &this->look, &this->look, &matRotation );
			D3DXVec3TransformCoord( &this->right, &this->right, &matRotation );
			this->UpdateView();
		}
		void SPCamera::Fly(float units)
		{
			this->position += this->up * units;
			this->UpdateView();
		}
		void SPCamera::Strafe(float units)
		{
			this->position += this->right * units;
			this->UpdateView();
		}
		void SPCamera::Walk(float units)
		{
			this->position += this->look * units;
			this->UpdateView();
		}
	}
}