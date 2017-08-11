#include "SPColor.h"

// maps unsigned 8 bits/channel to D3DCOLOR
#define COLORVAL_ARGB(a,r,g,b) \
    ((unsigned long)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))
namespace Pei
{
	namespace Utility
	{
		SPColor::SPColor()
		{
			this->value = 0xFFFFFFFF;
		}
		SPColor::SPColor(unsigned char R,unsigned char G,unsigned char B)
		{
			this->Set(R,G,B);
		}
		SPColor::SPColor(unsigned char R,unsigned char G,unsigned char B,unsigned char A)
		{
			this->Set(R,G,B,A);
		}
		void SPColor::Set(unsigned char R,unsigned char G,unsigned char B,unsigned char A)
		{
			this->value = COLORVAL_ARGB(A,R,G,B);
		}
		void SPColor::Set(unsigned char R,unsigned char G,unsigned char B)
		{
			this->value = COLORVAL_ARGB(255,R,G,B);
		}
		void SPColor::Set(unsigned long value)
		{
			this->value = value;
		}
		unsigned char SPColor::GetA()
		{
			return (unsigned char)((value & 0xFF000000) /0x00FFFFFF);
		}
		unsigned char SPColor::GetR()
		{
			return (unsigned char)((value & 0x00FF0000) /0x0000FFFF);
		}
		unsigned char SPColor::GetG()
		{
			return(unsigned char)((value & 0x0000FF00) /0x000000FF);
		}
		unsigned char SPColor::GetB()
		{
			return (unsigned char) (value & 0x000000FF);
		}
		void SPColor::serializeBase_SPColor(SPArchive& archive)
		{
			archive & this->value;
		}
		void SPColor::serialize(SPArchive& archive)
		{
			serializeBase_SPColor(archive);
		}
		void SPColor::SetA(unsigned char value)
		{
			this->Set(GetR(),GetG(),GetB(),value);
		}
	}
}