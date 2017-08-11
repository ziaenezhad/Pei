#include "SPRenderingParameters.h"

namespace Pei
{
	namespace Utility
	{
		void Serialize(SPArchive& archive,Pei::Graphic::SPMaterial& object)
		{
			SERIALIZE_OBJECT(archive,object);
		}
	}
}
namespace Pei
{
	namespace Graphic
	{
		SPMaterial::SPMaterial()
		{
			this->Set(1,1,1,1);
			this->Specular = this->Diffuse;
			this->Emissive.a = 1;
			this->Emissive.r = 0;
			this->Emissive.g = 0;
			this->Emissive.b = 0;
		}
		SPRenderingParameters::SPRenderingParameters() : material()
		{
			//this->flagRSCP = false;
			this->scale.Set(1.0f,1.0f,1.0f);
			this->global_scale.Set(1,1,1);
			this->blendMode = BlendingMode_Normal;
			this->visible = true;
		}
		void SPRenderingParameters::operator =(const SPRenderingParameters& rp)
		{
			this->position        = rp.position;
			this->rotation        = rp.rotation;
			this->scale           = rp.scale;
			this->visible         = rp.visible;
			this->blendMode       = rp.blendMode;
			this->material		  = rp.material;
		}
		void SPRenderingParameters::serializeBase_SPRenderingParameters(SPArchive& archive)
		{
			//archive & this->centerPoint;
			archive & this->position;
			archive & this->rotation;
			archive & this->scale;
			archive & this->material;
			archive & this->visible;
			SERIALIZE_ENUM(archive,BlendingMode,this->blendMode);
		}
		void SPRenderingParameters::serialize(SPArchive& archive)
		{
			serializeBase_SPRenderingParameters(archive);			
		}
	}
}