#pragma once
#include "GameObject.h"
#include "Objects/Player.h"

class Camera
{
public:


	void Init();
	void TickComp();
	void Render(HDC hDC, float alpha);


	void SetOwner(Player* P) { _owner = P; };
	GameObject* GetOwner() { return _owner; };

private:
	Player* _owner;
	
};

