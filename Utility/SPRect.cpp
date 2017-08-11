#include "SPRect.h"

namespace Pei
{
	namespace Utility
	{
		SPRect::SPRect()
		{
			this->top = 0;
			this->left = 0;
			this->right = 0;
			this->bottom = 0;
			this->width = 0;
			this->height = 0;
		}
		SPRect::SPRect(long x,long y,long width,long height)
		{
			this->top = y;
			this->left = x;
			this->right = width + x;
			this->bottom = height + y;
			this->width = width;
			this->height = height;
		}
		void SPRect::Set(long x,long y,long width,long height)
		{
			this->top = y;
			this->left = x;
			this->right = width + x;
			this->bottom = height + y;
			this->width = width;
			this->height = height;
		}
		bool SPRect::IsInside(float x,float y)
		{
			return true;//(x > this->x && x < this->width && y > this->y && y < this->height);
		}		
		void SPRect::serializeBase_SPRect(SPArchive& archive)
		{
			archive & this->top;
			archive & this->left;
			archive & this->right;
			archive & this->bottom;
			archive & this->width;
			archive & this->height;
		}
		void SPRect::serialize(SPArchive& archive)
		{
			serializeBase_SPRect(archive);
		}
	}
}
