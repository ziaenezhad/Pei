#include "SPLoopManager.h"

namespace Pei
{
	namespace Utility
	{
		SPLoopManager::SPLoopManager()
		{
			this->_allowSleep = false;
			this->_sleepLenght = 0;
			this->_loopCounter = 0;
			this->_periodRate = 60;//Hz
			this->_pps = this->_periodRate;
			this->_resulation = 255;
			this->_loopTimer.Start();
		}

		void SPLoopManager::BeginLoop()
		{
			this->_loopCounter++;
		}

		void SPLoopManager::EndLoop()
		{
			if(this->_loopCounter >= this->_resulation)
			{
				//Time Of each loop Cycle in milisecound
				this->_pps =  this->_loopCounter/(this->_loopTimer.GetTimePassedFromMemory()/1000.0f) ;
				this->_sleepLenght = 0;

				if(this->_allowSleep)
					if(this->_pps > this->_periodRate)
						this->_sleepLenght = 1;

				this->_loopCounter = 0;
				this->_loopTimer.MemorizeThisTime();

				//system("cls");
				//std::cout<<"FPS = ["<<this->_pps<<"]\n";
			}

			if(this->_allowSleep && this->_sleepLenght)
			{
				Sleep(this->_sleepLenght);
			}
		}
	}
}
