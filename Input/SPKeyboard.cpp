#include "SPKeyboard.h"
#include "../Utility/Pei.Utility.h"
using namespace Pei::Utility;

namespace Pei
{
	namespace Input
	{
		IDirectInput8* directInput = 0;
		SPKeyboard::SPKeyboard(HWND window)
		{
			if(!directInput)
				if(FAILED(DirectInput8Create( GetModuleHandle(0), DIRECTINPUT_VERSION,IID_IDirectInput8, (void**)&directInput, 0 )))
					errorHandler.PushError("SPKeyboard::SPKeyboard",0,"FAILED(DirectInput8Create)");
			if(FAILED(directInput->CreateDevice(GUID_SysKeyboard, &this->device,0)))
				errorHandler.PushError("SPKeyboard::SPKeyboard",0,"FAILED(CreateDevice)");			
			this->device->SetDataFormat( &c_dfDIKeyboard );
			this->device->SetCooperativeLevel( window , DISCL_FOREGROUND | DISCL_NONEXCLUSIVE );
			this->device->Acquire();
			for(unsigned char i = 0; i<255;i++)
				this->keyHistory[i] = 0;
		}
		SPKeyboard::~SPKeyboard()
		{
			SAFE_RELEASE(this->device);
		}
		void SPKeyboard::Update()
		{
			this->device->Poll();
			if(FAILED(this->device->GetDeviceState(256,(void*)&this->keyStates)))
				this->device->Acquire();

			for (unsigned char i = 0; i < 255; i++) 
			{
				//check for key press
				if (this->keyStates[i]) 
				{
					if(!this->keyHistory[i])
					{
						this->onKeyDown.emit((Keys)i);
						this->keyHistory[i] = 255;
					}
				} 
				//check for release
				else if (this->keyHistory[i]) 
				{
					this->onKeyUp.emit((Keys)i);
					this->keyHistory[i] = 0;
				}
			}
		}
	}
}