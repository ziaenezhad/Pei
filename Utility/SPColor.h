
#ifndef _SPColor_H
#define _SPColor_H

#include "SPArchive.h"
#include "../ExternalLibs.h"


namespace Pei{
	namespace Utility
	{		
		struct SPColor : public SPSerializable
		{
			unsigned long value;
			SPColor();
			SPColor(unsigned char R,unsigned char G,unsigned char B);
			SPColor(unsigned char R,unsigned char G,unsigned char B,unsigned char A);
			void Set(unsigned char R,unsigned char G,unsigned char B,unsigned char A);
			void Set(unsigned char R,unsigned char G,unsigned char B);			
			void Set(unsigned long value);
			unsigned char GetA();
			unsigned char GetR();
			unsigned char GetG();
			unsigned char GetB();
			void SetA(unsigned char value);
		protected:
			void serializeBase_SPColor(SPArchive& archive);
			virtual void serialize(SPArchive& archive);

		};
	}
}
#endif