#pragma once
#include "Scene.h"

class PlayScene : public Scene
{
	using Super = Scene;
	PlayScene();
	virtual ~PlayScene();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(ID2D1RenderTarget* renderTarget, float alpha) override;

};