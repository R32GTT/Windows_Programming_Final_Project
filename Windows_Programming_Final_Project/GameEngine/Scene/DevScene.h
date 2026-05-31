#pragma once
#include "Scene.h"

class DevScene : public Scene
{
	using Super = Scene;
public:

	DevScene();
	virtual ~DevScene();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(ID2D1RenderTarget* renderTarget,float alpha) override;

private:

};
