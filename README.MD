# Helper Classes for 3D Game Development

![](https://raw.githubusercontent.com/sajjad-shirazy/Pei/4dbb298f/MSVS2008/ClassDiagram1.png)

## Dependencies
- stdlib
- sigc++
- windows
- d3d9
- d3dx9
- dinput

## Classes
### Utility
Pei::Utility::SPArchive
Pei::Utility::SPArchive::IOType
Pei::Utility::SPErrorHandler
Pei::Utility::SPLoopManager
Pei::Utility::SPSerializable
Pei::Utility::SPTimer
Pei::Utility::SPWindow
Pei::Utility::SPColor
Pei::Utility::SPRect
Pei::Utility::SPSlotHost

### Graphic
Pei::Graphic::SPCamera
Pei::Graphic::SPCamera::ProjectionTypes
Pei::Graphic::SPMesh
Pei::Graphic::SPRenderer
Pei::Graphic::SPRenderer::FillMode
Pei::Graphic::SPTexture
Pei::Graphic::SPVertex_PNDT
Pei::Graphic::SPVertex_PD
Pei::Graphic::SPMaterial
Pei::Graphic::SPRenderingParameters
Pei::Graphic::SPRenderingParameters::BlendingMode
Pei::Graphic::SPVector3

### Resource
Pei::SPResource
Pei::SPResource::Types
Pei::SPResourceManager

### Input
Pei::Input::SPKeyboard
Pei::Input::SPKeyboard::Keys
Pei::Input::SPKeyboard::OnKeyDown
Pei::Input::SPKeyboard::OnKeyUp
Pei::Input::SPMouse
Pei::Input::SPMouse::Buttons
Pei::Input::SPMouse::MouseState
Pei::Input::SPMouse::OnMouseMove
Pei::Input::SPMouse::OnMouseDown
Pei::Input::SPMouse::OnMouseUp