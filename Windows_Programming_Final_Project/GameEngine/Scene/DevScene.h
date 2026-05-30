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
	virtual void Render(HDC hdc,float alpha) override;

private:

};
