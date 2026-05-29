#pragma once
#include "GameObject.h"
#include "Objects/Player.h"

class Camera
{
public:


	void Init();
	void TickComp();
	void Render(HDC hDC);


	void SetOwner(Player* P) { _owner = P; };
	GameObject* GetOwner() { return _owner; }; // 좋지 않은 방법. GAMEOBJECT 산하에 있는 애들중 움직일 수 있는 애들만 넘겨둘까 싶음.

private:
	Player* _owner;
};

