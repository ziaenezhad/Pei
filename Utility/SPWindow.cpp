#include "SPWindow.h"


namespace Pei
{
	namespace Utility
	{
		WNDCLASSEX* SPWindow::_windowClass = 0;
		LRESULT CALLBACK SPWindow::staticWinMsgHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
		{
			SPWindow* pWnd;

			if (uMsg == WM_NCCREATE)
			{
				// get the pointer to the window from lpCreateParams which was set in CreateWindow
				SetWindowLong(hwnd, GWL_USERDATA, (long)((LPCREATESTRUCT(lParam))->lpCreateParams));
			}
			// get the pointer to the window
			pWnd = (SPWindow*)GetWindowLong(hwnd, GWL_USERDATA);

			// if we have the pointer, go to the message handler of the window
			// else, use DefWindowProc
			if (pWnd)
				return pWnd->winMsgHandler(hwnd, uMsg, wParam, lParam);
			else
				return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}

		LRESULT CALLBACK SPWindow::winMsgHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
		{
			switch (uMsg)
			{
			case WM_KEYDOWN:
				switch( wParam )
				{
				case VK_F4:
					//MessageBox(0,"adjhgadjkhfg",0,0);
					this->FullScreen(true);
					break;
				case VK_LEFT:
					this->SetPosition(this->GetPositionX()-1,this->GetPositionY());
					break;
				case VK_RIGHT:			
					this->SetPosition(this->GetPositionX()+1,this->GetPositionY());
					break;
				case VK_SPACE:
					this->Center();
					break;
				case VK_UP:
					this->FullScreen();
					break;
				case VK_DOWN:
					this->FullScreen(false);
					break;
				case VK_ESCAPE:
					this->Close(0);
					break;
				}
				break;
			case WM_PAINT:
				break;
			case WM_CLOSE:				
				this->_windowClosed = true;
				break;
			case WM_DESTROY:
				this->_windowClosed = true;
				break;
			default:
				return DefWindowProc(hwnd, uMsg, wParam, lParam);
			}

			return 0;
		}

		bool SPWindow::registerWindow(HINSTANCE hInstance)
		{
			_windowClass = new WNDCLASSEX;
			// Fill in the window class structure with default parameters 
		 
			_windowClass->cbSize = sizeof(WNDCLASSEX);							// size of structure 
			_windowClass->style = CS_HREDRAW | CS_VREDRAW;						// redraw if size changes 
			_windowClass->lpfnWndProc = SPWindow::staticWinMsgHandler;			// points to window procedure 
			_windowClass->cbClsExtra = 0;											// no extra class memory 
			_windowClass->cbWndExtra = 0;											// no extra window memory 
			_windowClass->hInstance = hInstance;									// handle to instance 
			_windowClass->hIcon = LoadIcon(NULL, IDI_APPLICATION);				// predefined app-> icon 
			_windowClass->hCursor = LoadCursor(NULL, IDC_ARROW);					// predefined arrow 
			_windowClass->hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);	// black background brush 
			_windowClass->lpszMenuName = NULL;									// name of menu resource 
			_windowClass->lpszClassName = "SPWindow" ;			                // name of window class 
			_windowClass->hIconSm = LoadIcon(NULL, IDI_APPLICATION);				// small class icon 
		 
			// Register the window class. 
			if (RegisterClassEx(_windowClass) == 0)
				return false;
			else
				return true;
		}

		SPWindow::SPWindow(HINSTANCE hInstance,std::string title,int x,int y, unsigned int width,unsigned int height,DWORD style)
		{
			if(!SPWindow::_windowClass)
				SPWindow::registerWindow(hInstance);

			this->_handle = CreateWindow(SPWindow::_windowClass->lpszClassName, title.c_str(), style, x, y,width, height, NULL, NULL, hInstance,(void *)this);
			//this->SetStyle(WS_OVERLAPPEDWINDOW|WS_VISIBLE);
			this->_windowClosed = false;
			if( x == -1 && y == -1)
				this->Center();
			ShowWindow(this->_handle,SW_SHOW);
		}
		SPWindow::~SPWindow()
		{
			this->Close();
		}

		void SPWindow::SetTitle(std::string title)
		{
			SetWindowText(this->_handle,title.c_str());
		}

		void SPWindow::ProcessMessages()
		{
			MSG msg;
			BOOL ret = GetMessage( &msg, NULL, 0, 0 );
			if(ret)
			{
				TranslateMessage(&msg); 
				DispatchMessage(&msg); 
			}
		}

		void SPWindow::Center()
		{
			this->SetPosition(this->GetScreenWidth()/2-this->GetWindowWidth()/2,this->GetScreenHeight()/2-this->GetWindowHeight()/2);
		}

		void SPWindow::SetStyle(DWORD style)
		{
			SetWindowLong(this->_handle, GWL_STYLE, style);
		}

		void SPWindow::FullScreen(bool value,unsigned int width,unsigned int height,DWORD bitsPerPixle)
		{
			if(value != _isFullScreen)
			{
				if(value)
				{
					if(width == 0) width = this->GetWindowWidth();
					if(height == 0) height = this->GetWindowHeight();
					this->SetStyle(WS_POPUPWINDOW|WS_VISIBLE);
					this->SetPosition(0,0);
					DEVMODE dm;
					memset(&dm, 0, sizeof(dm));
					dm.dmSize = sizeof(dm);
					dm.dmPelsWidth = width;
					dm.dmPelsHeight = height;
					dm.dmBitsPerPel = 16;
					dm.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

					if (ChangeDisplaySettings(&dm, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
						this->FullScreen(false);
						//Error
					}
					this->SetTopWindow();
					this->_isFullScreen = true;
				}
				else
				{
					ChangeDisplaySettings(NULL,NULL);
					this->SetStyle(WS_GAMEFRAME|WS_VISIBLE);
					this->SetTopWindow(false);
					this->Center();			
					this->_isFullScreen = false;
				}
			}
		}

		void SPWindow::SetPosition(int x,int y)
		{
			SetWindowPos(this->_handle,0,x,y,0,0,SWP_NOSIZE|SWP_NOZORDER);
		}

		void SPWindow::SetSize(unsigned int width,unsigned height)
		{
			SetWindowPos(this->_handle,0,0,0,width,height,SWP_NOMOVE|SWP_NOZORDER);
		}

		void SPWindow::SetTopWindow(bool value)
		{
			if(value)
				SetWindowPos(this->_handle,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
			else
				SetWindowPos(this->_handle,HWND_NOTOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);		
		}

		unsigned int SPWindow::GetWindowWidth()
		{
			RECT rect;
			GetWindowRect(this->_handle,&rect);
			return (unsigned int)(rect.right-rect.left);
		}

		unsigned int SPWindow::GetWindowHeight()
		{
			RECT rect;
			GetWindowRect(this->_handle,&rect);
			return (unsigned int)(rect.bottom-rect.top);
		}

		int SPWindow::GetPositionX()
		{
			RECT rect;
			GetWindowRect(this->_handle,&rect);
			return (unsigned int)(rect.left);
		}

		int SPWindow::GetPositionY()
		{
			RECT rect;
			GetWindowRect(this->_handle,&rect);
			return (unsigned int)(rect.top);
		}

		
		unsigned int SPWindow::GetScreenWidth()
		{
			return GetSystemMetrics(SM_CXSCREEN);
		}

		unsigned int SPWindow::GetScreenHeight()
		{
			return GetSystemMetrics(SM_CYSCREEN);
		}

		void SPWindow::Close(int exitCode)
		{
			DestroyWindow(this->_handle);
		}
	}
}