#include "SPTimer.h"

namespace Pei
{
	namespace Utility
	{
		SPTimer::SPTimer()
		{
			this->_baseTime = 0;
			this->_delta = 0;
			this->_memory = 0;
			this->_running = false;
		}

		void SPTimer::Reset()
		{
			this->_delta = 0;
			this->Start();
		}

		void SPTimer::Start()
		{
			this->_running = true;
			this->_baseTime  = timeGetTime() + this->_delta;			
		}
		unsigned int SPTimer::GetTime()
		{
			if(this->_running)
			{
				this->_delta = timeGetTime() - this->_baseTime ;
				return this->_delta;
			}
			else
			{
				return this->_delta;
			}
		}

		void SPTimer::Pause()
		{
			this->GetTime();
			this->_running = false;
		}

		void SPTimer::Stop()
		{
			this->_delta = 0;
			this->_running = false;
		}

		
		void SPTimer::MemorizeThisTime()
		{
			this->_memory = this->GetTime();
		}

		unsigned int SPTimer::GetMemory()
		{
			return this->_memory;
		}
		
		unsigned int SPTimer::GetTimePassedFromMemory()
		{
			this->_delta = this->GetTime() - this->_memory;
			return this->_delta;
		}
	}
}