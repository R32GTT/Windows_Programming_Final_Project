#pragma once
#include "GameObject.h"
#include "Objects/Player.h"

class Camera
{
public:


	void Init();
	void TickComp();
	void Render(HDC hDC, float alpha);


	void SetOwner(GameObject* P);
	GameObject* GetOwner() { return _owner; };

	void SavePrevPos() { _prevPos = _pos; };
	Vec2F GetRenderPos(float alpha) { return _prevPos * (1.0f - alpha) + _pos * alpha; };
	
	void TriggerShake(float duration, float magnitude)
	{
		_shakeTimer = duration;
		_shakeDuration = duration;
		_shakeMagnitude = magnitude;
	}

	void SetPos(Vec2F POS) { _pos = POS; };

	Vec2F GetPos() { return _pos; };

private:
	GameObject* _owner = nullptr;

	Vec2F _pos{};
	Vec2F _prevPos{};
	
	float _shakeTimer = 0.0f;
	float _shakeDuration = 0.0f;
	float _shakeMagnitude = 0.0f;

};

