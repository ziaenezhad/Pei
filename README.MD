# Helper Classes for 3D Game Development

![](https://raw.githubusercontent.com/sajjad-shirazy/Pei/4dbb298f/MSVS2008/ClassDiagram1.png)

## Dependencies
- stdlib
- sigc++
- windows
- d3d9
- d3dx9
- dinput

## Usage
```c++
#include "../Pei.h"

using namespace Pei::Graphic;
using namespace Pei::Utility;
using namespace Pei::Input;
SPRenderer* renderer = 0;
class Entity : public SPSlotHost
{
public:
	void OnMouseMove(SPMouse::MouseState mouseState)
	{
		system("cls");
		std::cout<<"MouseMove X: ["<<mouseState.position.x<<"]"<<std::endl;
		std::cout<<"MouseMove Y: ["<<mouseState.position.y<<"]"<<std::endl;
	}
	void OnMouseDown(SPMouse::MouseState mouseState)
	{
		system("cls");
		std::cout<<"MouseDown  : ["<<mouseState.rgbButtons[0]<<"]"<<std::endl;
		std::cout<<"MouseMove X: ["<<mouseState.position.x<<"]"<<std::endl;
		std::cout<<"MouseMove Y: ["<<mouseState.position.y<<"]"<<std::endl;
	}
	void OnMouseUp(SPMouse::MouseState mouseState)
	{
		system("cls");
		std::cout<<"MouseUp    : ["<<mouseState.rgbButtons[0]<<"]"<<std::endl;
		std::cout<<"MouseMove X: ["<<mouseState.position.x<<"]"<<std::endl;
		std::cout<<"MouseMove Y: ["<<mouseState.position.y<<"]"<<std::endl;
	}
	void OnKeyDown(SPKeyboard::Keys key)
	{
		static bool grid = false;
		if(key == SPKeyboard::Key_W)
		{
			grid = !grid;
			if(grid)
				renderer->SetFillMode(SPRenderer::FillMode_Wireframe);
			else
				renderer->SetFillMode(SPRenderer::FillMode_Solid);
		}
	}

};

int main()
{
	SPLoopManager loopman;
	loopman.AllowSleep(true);

	
	SPWindow win(0,"; )");
	HWND window = win.GetHandle();
	SPMouse mouse(window);
	SPKeyboard keyboard(window);
	Entity *en = new Entity();
	mouse.onMouseMove += en;
	mouse.onMouseDown += en;
	mouse.onMouseUp += en;
	keyboard.onKeyDown += en;

	renderer = new SPRenderer(win.GetHandle());
	//renderer->Fullscreen();
	SPMesh* plane = Pei::SPResourceManager::GetMesh("goll.x");
	SPRenderingParameters rp;
	renderer->SetLightEnable(true);
	renderer->SetAlphaTestEnable(false);
	renderer->SetAlphaBlendEnable(false);
	renderer->SetTextureSamplerEnabled(true);
	//renderer->SetFillMode(SPRenderer::FillMode_Wireframe);
	renderer->GetCamera()->Walk(-350);
	renderer->GetCamera()->Fly(60);
	renderer->GetCamera()->Pitch(0.2f);
	//renderer->GetCamera()->SetCameraSize(1366,768);
	//rp.position.z += 2500;
	rp.Transform();
	plane->SetCurrentSubsetIndex(0);

	SPColor color(255,0,255);

	while(!win.IsWindowClosed())
	{		
		loopman.BeginLoop();
		SPWindow::ProcessMessages();
		renderer->StartRendering();
		renderer->ClearBackBuffer(&color);
		rp.rotation.y += 0.005f;
		rp.Transform();
		
		renderer->GetCamera()->Walk(0.2f);
		plane->Render(&rp);
		renderer->StopRendering();
		//mouse.Update();
		keyboard.Update();
		loopman.EndLoop();
	}
	Pei::SPResourceManager::RemoveResources(0);
	return 0;
}
```

## Result
![](https://cdn.rawgit.com/sajjad-shirazy/Pei/f59c8199/Screenshots/Screenshot%20(4).png)
![](https://cdn.rawgit.com/sajjad-shirazy/Pei/f59c8199/Screenshots/Screenshot%20(6).png)
![](https://cdn.rawgit.com/sajjad-shirazy/Pei/f59c8199/Screenshots/Screenshot%20(7).png)

## Classes
### Utility
- Pei::Utility::SPArchive
- Pei::Utility::SPArchive::IOType
- Pei::Utility::SPErrorHandler
- Pei::Utility::SPLoopManager
- Pei::Utility::SPSerializable
- Pei::Utility::SPTimer
- Pei::Utility::SPWindow
- Pei::Utility::SPColor
- Pei::Utility::SPRect
- Pei::Utility::SPSlotHost

### Graphic
- Pei::Graphic::SPCamera
- Pei::Graphic::SPCamera::ProjectionTypes
- Pei::Graphic::SPMesh
- Pei::Graphic::SPRenderer
- Pei::Graphic::SPRenderer::FillMode
- Pei::Graphic::SPTexture
- Pei::Graphic::SPVertex_PNDT
- Pei::Graphic::SPVertex_PD
- Pei::Graphic::SPMaterial
- Pei::Graphic::SPRenderingParameters
- Pei::Graphic::SPRenderingParameters::BlendingMode
- Pei::Graphic::SPVector3

### Resource
- Pei::SPResource
- Pei::SPResource::Types
- Pei::SPResourceManager

### Input
- Pei::Input::SPKeyboard
- Pei::Input::SPKeyboard::Keys
- Pei::Input::SPKeyboard::OnKeyDown
- Pei::Input::SPKeyboard::OnKeyUp
- Pei::Input::SPMouse
- Pei::Input::SPMouse::Buttons
- Pei::Input::SPMouse::MouseState
- Pei::Input::SPMouse::OnMouseMove
- Pei::Input::SPMouse::OnMouseDown
- Pei::Input::SPMouse::OnMouseUp