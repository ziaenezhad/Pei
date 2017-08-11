#include "SPResource.h"
#include "Graphic/SPMesh.h"
#include "Graphic/SPTexture.h"
#include "Graphic/SPRenderer.h"
#include "Utility/Pei.Utility.h"

using namespace Pei::Utility;

namespace Pei
{
	Graphic::SPMesh* lastMesh = 0;
	Graphic::SPTexture* lastTexture = 0;
	Graphic::SPRenderer* lastRenderer = 0;

	SPResource::SPResource(SPResource::Types type)
	{
		this->name = "";
		this->type = type;
		this->resGroupID = SPResourceManager::resGroupID;
		switch(type)
		{
		case SPResource::Type_Mesh :
			if(lastMesh)
				lastMesh->next = (Graphic::SPMesh*)this;
			this->prev = lastMesh;
			lastMesh = (Graphic::SPMesh*)this;
			this->next = 0;
			break;
		case SPResource::Type_Texture :				
			if(lastTexture)
				lastTexture->next = (Graphic::SPTexture*)this;
			this->prev = lastTexture;
			lastTexture = (Graphic::SPTexture*)this;
			this->next = 0;
			break;
		case SPResource::Type_Renderer :
			if(lastRenderer)
				lastRenderer->next = (Graphic::SPRenderer*)this;
			this->prev = lastRenderer;
			lastRenderer = (Graphic::SPRenderer*)this;
			this->next = 0;
			break;
		}
		
	}
	//------------------------------------------------------
	std::string SPResourceManager::basePath = "Res\\";
	unsigned char SPResourceManager::resGroupID = 0;

	void SPResourceManager::releaseAllResources()
	{
		SPResource* current = lastMesh;
		while(current){current->releaseRes();current = current->prev;}
		current = lastTexture;
		while(current){current->releaseRes();current = current->prev;}
		current = lastRenderer;
		while(current){current->releaseRes();current = current->prev;}
	}
	void SPResourceManager::reloadAllResources()
	{
		SPResource* current = lastMesh;
		while(current){current->reloadRes();current = current->prev;}
		current = lastTexture;
		while(current){current->reloadRes();current = current->prev;}
		current = lastRenderer;
		while(current){current->reloadRes();current = current->prev;}
	}
	Graphic::SPMesh* SPResourceManager::GetMesh(const char* name)
	{
		if(name)
		{
			
			Graphic::SPMesh* current = lastMesh;
			while(current)
			{
				if(current->name.compare(name) == 0)
					return current;
				current = (Graphic::SPMesh*)current->prev;
			}
			return new Graphic::SPMesh(name);
		}
		return 0;
	}
	Graphic::SPTexture* SPResourceManager::GetTexture(const char* name)
	{
		if(name)
		{
			
			Graphic::SPTexture* current = lastTexture;
			while(current)
			{
				if(current->name.compare(name) == 0)
					return current;
				current = (Graphic::SPTexture*)current->prev;
			}
			return new Graphic::SPTexture(name);
		}
		return 0;
	}
	void SPResourceManager::RemoveResources(unsigned char resGroupID)
	{
		
		SPResource* current = lastMesh;
		while(current)
		{
			if(current->resGroupID == resGroupID)
			{
				if(current->next)
					current->next->prev = current->prev;
				if(current->prev)
					current->prev->next = current->next;
				Graphic::SPMesh* temp = (Graphic::SPMesh*)current;
				current = current->prev;
				delete temp;
				continue;
			}
			current = current->prev;
		}
		
		current = lastTexture;
		while(current)
		{
			if(current->resGroupID == resGroupID)
			{
				if(current->next)
					current->next->prev = current->prev;
				if(current->prev)
					current->prev->next = current->next;
				Graphic::SPTexture* temp = (Graphic::SPTexture*)current;
				current = current->prev;
				delete temp;
				continue;
			}
			current = current->prev;
		}
	}
}