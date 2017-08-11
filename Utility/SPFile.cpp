#include "SPFile.h"

namespace Pei {
	namespace Utility
	{
		namespace File
		{			
			bool CreateFolder(const char* name)
			{
				if(CreateDirectory(name,0))
					return true;
				return false;
			}

			void RemoveFolder(const char* name)
			{
				RemoveDirectory(name);
			}

			void RemoveFile(const char* name)
			{
				DeleteFile(name);
			}

			namespace Path
			{
				//returns Parent Directory path without
				std::string GetParentDirectory(std::string path)
				{
					std::string output ;
					int i = path.length() - 1;
					while(path[i] !='\\')
						i--;
					return path.substr(0,i) ;		 
				}

				
				std::string GetSystemPath()
				{
					char buffer[1024];
					GetSystemDirectoryA(buffer,1024);
					return buffer;
				}
				
				std::string GetStartupPath()
				{
					 char buffer[1024];
					 GetCurrentDirectory(1024,buffer);
					 return buffer;
				}

				std::string GetFullPath(const char* path)
				{
					return GetStartupPath()+"\\"+path;
				}

				/// Return filename without path
				std::string GetName(std::string path)
				{
					int i = path.size()-1;
					while(path[i]!='\\')
						i--;
					return path.substr(i+1,path.size()-1);
				}
				
				bool IsLocalPath(const char* path)
				{
					if(path[1] == ':')
						return true;
					else
						return false;
				}
			}
		}
	}
}