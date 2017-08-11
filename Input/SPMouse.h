#ifndef _SPMouse_H
#define _SPMouse_H

#include "../ExternalLibs.h"

namespace Pei
{
	namespace Input
	{
		struct SPMouse
		{
			enum Buttons : unsigned char
			{
				Button_Left = 0,
				Button_Right = 1,
				Button_Middle = 2,
				Button_None = 4
			};
			struct MouseState : DIMOUSESTATE
			{
				POINT position;
			};
			class OnMouseMove : public sigc::signal<void,const MouseState&>
			{
			public:	template<typename Type>	void operator +=(Type* slot){this->connect(sigc::mem_fun1(slot, &Type::OnMouseMove));};
			};
			class OnMouseDown : public sigc::signal<void,const MouseState&>
			{
			public:	template<typename Type>	void operator +=(Type* slot){this->connect(sigc::mem_fun1(slot, &Type::OnMouseDown));};
			};
			class OnMouseUp : public sigc::signal<void,const MouseState&>
			{
			public:	template<typename Type>	void operator +=(Type* slot){this->connect(sigc::mem_fun1(slot, &Type::OnMouseUp));};
			};
			OnMouseUp onMouseUp;
			MouseState mouseState;
			OnMouseMove onMouseMove;
			OnMouseDown onMouseDown;
			SPMouse(HWND window);
			~SPMouse();
			inline bool IsKeyDown(Buttons button){if(mouseState.rgbButtons[button])return true ; else return false;}
			void Update();
		private:
			IDirectInputDevice8* device;
			unsigned char buttonsHistory[4];
			HWND window;
		};
	}
}
#endif