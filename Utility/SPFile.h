#ifndef _SPFile_H
#define _SPFile_H

#include "../ExternalLibs.h"

namespace Pei {
	namespace Utility
	{
		namespace File
		{
			namespace Path
			{
				std::string GetStartupPath();
				std::string GetSystemPath();
				std::string GetParentDirectory(std::string path);
				std::string GetFullPath(char* path);
				std::string GetName(std::string path);
				bool IsLocalPath(char* path);
			}
			bool CreateFolder(const char* name);
			void RemoveFolder(const char* name);
			void RemoveFile(const char* name);
		}
	}
}

#endif