#include "SPErrorHandler.h"
#include "SPFile.h"

//using namespace boost;

namespace Pei 
{
	namespace Utility
	{
		SPErrorHandler errorHandler;

		SPErrorHandler::SPErrorHandler()
		{
			this->logFilePath = File::Path::GetStartupPath()+"\\Soshiant.log";
			this->logingEnabled = true;
			this->PushEvent("SPErrorHandler","errorHandler","Starting ErrorHandling ...");
		}
		SPErrorHandler::~SPErrorHandler()
		{
			this->PushEvent("SPErrorHandler","errorHandler","Finishing ErrorHandling ...");
		}

		void SPErrorHandler::PushError(const char* partName,const char* instanseName,const char* message)
		{
			this->PushEvent(partName,instanseName,message);
			std::string str = "";
			str += "an error occured in [";
			str += partName;
			str += "] for [";
			str += instanseName;
			str += "]                      \n\n";
			str += "Messgae :\n\n";
			str += message;
			std::cout << str;
			MessageBox(0,str.c_str(),"[Soshiant GDK Error Handler]",0);
			exit(0);
		}


		void SPErrorHandler::PushEvent(const char* partName,const char* instanseName,const char* message)
		{
			//std::string str = "";
			//str += posix_time::to_simple_string(posix_time::microsec_clock::local_time());
			//str += "  ::  ";
			//str += instanseName;
			//str += "\t[Type:(";
			//str += partName;
			//str += ")] Reports : <";
			//str += message;
			//str += ">";
			//this->errorsStack.push_back(str);
			//std::ofstream file(this->logFilePath.c_str());
			//std::list<std::string>::iterator iter = this->errorsStack.begin();
			//file << "||<[   THE GOD EXISTS   ]>||\n";
			//file << "||<[ THE ISLAM IS RIGHT ]>||\n\n";
			//while(iter != this->errorsStack.end())
			//{
			//	file << (*iter++) + '\n';
			//}			
			////file << "\n\n|||| RAHBAR FAGHAT SEYED ALI ... |||";
			//file.close();
		}
	}
}