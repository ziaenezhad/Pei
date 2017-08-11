#ifndef _SPCamera_H
#define _SPCamera_H

#include "SPVector3.h"

namespace Pei
{
	namespace Graphic
	{
		class SPRenderer;
		class SPCamera
		{
			friend class SPRenderer;
		public:
			enum ProjectionTypes
			{
				ProjectionType_Perspective,
				ProjectionType_Orthographic,
			};
		protected:
			unsigned int    width;
			unsigned int    height;
			SPVector3		position;
			SPVector3		look;
			SPVector3		up;
			SPVector3		right;
			SPRenderer*     rendere;
			D3DXMATRIX      matView;
			D3DXMATRIX      matProjection;
			ProjectionTypes projectionType;
			float           nearClippingPlane;
			float           farClippingPlane;
			float           angle;
			void resetProjection();
		public:
			SPCamera(unsigned int width,unsigned int height,ProjectionTypes projectionType = ProjectionType_Perspective);
			void SetProjection(ProjectionTypes type,float angle = 45,float nearClippingPlane = 1,float farClippingPlane = 64000.0f);
			void SetCameraSize(unsigned int width,unsigned int height);
			ProjectionTypes GetProjectionType(){return this->projectionType;}

			void SetLook(SPVector3* look){this->look = (*look);this->UpdateView();}
			void SetPosition(SPVector3* position){this->position = (*position);this->UpdateView();}
			void SetUp(SPVector3* up){this->up = (*up);this->UpdateView();}
			SPVector3* GetLookAt(){return &this->look;}
			SPVector3* GetPosition(){return &this->position;}
			SPVector3* GetUp(){return &this->up;}
			void SetPositionX(float x){this->position.x = x;this->UpdateView();}
			void SetPositionY(float y){this->position.y = y;this->UpdateView();}
			void SetPositionZ(float z){this->position.z = z;this->UpdateView();}
			///Moving Left/Right(-/+)
			void Strafe(float units);
			///Moving Up/Down(+/-)
			void Fly(float units);
			///Moving forward/backward(+/-)
			void Walk(float units);
			///rotate on Up Vector
			void Yaw(float angle);
			///Rotate on Look Vector
			void Roll(float angle);
			///Rotate on Right Vector
			void Pitch(float angle);
			void UpdateView();
		};
	}
}
#endif