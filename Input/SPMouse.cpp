#include "SPMouse.h"
#include "../Utility/Pei.Utility.h"
using namespace Pei::Utility;

namespace Pei
{
	namespace Input
	{
		extern IDirectInput8* directInput;
		SPMouse::SPMouse(HWND window)
		{
			this->window = window;
			if(!directInput)
				if(FAILED(DirectInput8Create( GetModuleHandle(0), DIRECTINPUT_VERSION,IID_IDirectInput8, (void**)&directInput, 0 )))
					errorHandler.PushError("SPMouse::SPMouse",0,"FAILED(DirectInput8Create)");
			if(FAILED(directInput->CreateDevice(GUID_SysMouse, &this->device,0)))
				errorHandler.PushError("SPMouse::SPMouse",0,"FAILED(CreateDevice)");			
			this->device->SetDataFormat( &c_dfDIMouse );
			this->device->SetCooperativeLevel( window , DISCL_FOREGROUND | DISCL_NONEXCLUSIVE );
			this->device->Acquire();

			for(unsigned char i = 0; i<4;i++)
				this->buttonsHistory[i] = 0;
		}
		SPMouse::~SPMouse()
		{
			SAFE_RELEASE(this->device);
		}
		void SPMouse::Update()
		{
			this->device->Poll();
			if(FAILED(this->device->GetDeviceState(sizeof(DIMOUSESTATE),(void*)&this->mouseState)))
				this->device->Acquire();

			if(this->mouseState.lX || this->mouseState.lY || this->mouseState.lZ)
			{				
				GetCursorPos(&this->mouseState.position);
				ScreenToClient(window, &this->mouseState.position);
				this->onMouseMove.emit(this->mouseState);
			}	
			for ( int i = 0 ; i < 4; i++ ) 
			{
				if(this->mouseState.rgbButtons[i])
				{
					if(!this->buttonsHistory[i])
					{
						this->buttonsHistory[i] = 255;
						this->onMouseDown.emit(this->mouseState);
					}
				}
				else if(this->buttonsHistory[i])
				{
					this->buttonsHistory[i] = 0;
					this->onMouseUp.emit(this->mouseState);
				}
			}
		}
	}
}