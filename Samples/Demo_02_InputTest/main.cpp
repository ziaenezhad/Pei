#include <Pei.h>

//#pragma comment(lib, "Pei.lib")
//#pragma comment(lib, "d3d9.lib")
//#pragma comment(lib, "d3dx9.lib")
//#pragma comment(lib, "dinput8.lib")
//#pragma comment(lib, "dxguid.lib")
//#pragma comment(lib, "winmm.lib")

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
		mouse.Update();
		keyboard.Update();
		loopman.EndLoop();
	}
	Pei::SPResourceManager::RemoveResources(0);
	return 0;
}
