#ifndef _SPWindow_H
#define _SPWindow_H

#include "../ExternalLibs.h"

namespace Pei
{
	namespace Utility
	{
		class SPWindow
		{
			static WNDCLASSEX* _windowClass;
			static const DWORD WS_GAMEFRAME = WS_OVERLAPPED|WS_CAPTION|WS_THICKFRAME|WS_SYSMENU ; 
			bool _windowClosed;
			bool _isFullScreen;
			HWND _handle;

			static LRESULT CALLBACK staticWinMsgHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);		
			LRESULT CALLBACK winMsgHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
			static bool registerWindow(HINSTANCE hInstance);

		public:
			SPWindow(HINSTANCE hInstance,std::string title,int x = -1,int y = -1, unsigned int width = 800,unsigned int height = 600 ,DWORD style = WS_GAMEFRAME);		
			~SPWindow();

			
			void Center();
			void Close(int exitCode = 0);
			void SetTitle(std::string title);
			HWND GetHandle(){return this->_handle;}
			bool IsFullScreen(){return this->_isFullScreen;}
			bool IsWindowClosed(){return this->_windowClosed;}

			void SetStyle(DWORD style);
			void FullScreen(bool value = true,unsigned int width = 0,unsigned int height = 0,DWORD bitsPerPixle = 8);
			void SetSize(unsigned int width,unsigned int height);
			void SetPosition(int x, int y);
			void SetTopWindow(bool value = true);

			unsigned int GetWindowWidth();
			unsigned int GetWindowHeight();
			int GetPositionX();
			int GetPositionY();


			static void ProcessMessages();
			static unsigned int GetScreenWidth();		
			static unsigned int GetScreenHeight();
		};
	}
}
#endif