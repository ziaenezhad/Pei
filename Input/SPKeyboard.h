#ifndef _SPKeyboard_H
#define _SPKeyboard_H

#include "../ExternalLibs.h"
#include "../Utility/SPEvent.h"

namespace Pei
{
	namespace Input
	{
		struct SPKeyboard
		{
			enum Keys : unsigned char
			{
				Key_ESCAPE = 1,
				Key_1 = 2,
				Key_2 = 3,
				Key_3 = 4,
				Key_4 = 5,
				Key_5 = 6,
				Key_6 = 7,
				Key_7 = 8,
				Key_8 = 9,
				Key_9 = 10,
				Key_0 = 11,
				Key_MINUS = 12,
				Key_EQUAL = 13,
				Key_BACKSPACE = 14,
				Key_TAB = 15,
				Key_Q = 16,
				Key_W = 17,
				Key_E = 18,
				Key_R = 19,
				Key_T = 20,
				Key_Y = 21,
				Key_U = 22,
				Key_I = 23,
				Key_O = 24,
				Key_P = 25,
				Key_LBRACE = 26,
				Key_RBRACE = 27,
				Key_ENTER = 28,
				Key_LCTRL = 29,
				Key_A = 30,
				Key_S = 31,
				Key_D = 32,
				Key_F = 33,
				Key_G = 34,
				Key_H = 35,
				Key_J = 36,
				Key_K = 37,
				Key_L = 38,
				Key_COLON = 39,
				Key_QUOTE = 40,
				Key_LSHIFT = 42,
				Key_BSLASH = 43,
				Key_Z = 44,
				Key_X = 45,
				Key_C = 46,
				Key_V = 47,
				Key_B = 48,
				Key_N = 49,
				Key_M = 50,
				Key_COMMA = 51,
				Key_PERIOD = 52,
				Key_FSLASH = 53,
				Key_RSHIFT = 54,
				Key_NUMMULT = 55,
				Key_LALT = 56,
				Key_SPACE = 57,
				Key_CAPSLOCK = 58,
				Key_F1 = 59,
				Key_F2 = 60,
				Key_F3 = 61,
				Key_F4 = 62,
				Key_F5 = 63,
				Key_F6 = 64,
				Key_F7 = 65,
				Key_F8 = 66,
				Key_F9 = 67,
				Key_NUMLOCK = 69,
				Key_SCROLL = 70,
				Key_NUM7 = 71,
				Key_NUM8 = 72,
				Key_NUM9 = 73,
				Key_NUMMINUS = 74,
				Key_NUM4 = 75,
				Key_NUM5 = 76,
				Key_NUM6 = 77,
				Key_NUMPLUS = 78,
				Key_NUM1 = 79,
				Key_NUM2 = 80,
				Key_NUM3 = 81,
				Key_NUM0 = 82,
				Key_NUMPERIOD = 83,
				Key_NUMENTER = 156,
				Key_RCTRL = 157,
				Key_NUMDIV = 181,
				Key_PRINT = 183,
				Key_RALT = 184,
				Key_BREAK = 197,
				Key_HOME = 199,
				Key_UP = 200,
				Key_PGUP = 201,
				Key_LEFT = 203,
				Key_RIGHT = 205,
				Key_END = 207,
				Key_DOWN = 208,
				Key_PGDN = 209,
				Key_INSERT = 210,
				Key_DELETE = 211,
				Key_LWINDOWS = 219,
				Key_RWINDOWS = 220,
				Key_CONTEXT = 221
			};
			class OnKeyDown : public sigc::signal<void,Keys>
			{
			public:	template<typename Type>	void operator +=(Type* slot){this->connect(sigc::mem_fun1(slot, &Type::OnKeyDown));};
			};
			class OnKeyUp : public sigc::signal<void,Keys>
			{
			public:	template<typename Type>	void operator +=(Type* slot){this->connect(sigc::mem_fun1(slot, &Type::OnKeyUp));};
			};
			OnKeyUp onKeyUp;
			OnKeyDown onKeyDown;
			unsigned char keyStates[256];
			SPKeyboard(HWND window);
			~SPKeyboard();
			inline bool IsKeyDown(Keys key){if(keyStates[key])return true ; else return false;}
			void Update();
		private:
			IDirectInputDevice8* device;
			unsigned char keyHistory[256];
		};
	}
}
#endif