#pragma once

class GameObject
{
public:

	GameObject();
	virtual ~GameObject();

	virtual void Init();
	virtual void Update();
	virtual void Render(HDC hdc, float alpha);
	virtual bool CheckDead();

	bool operator==(const GameObject& rhs) { return _id == rhs._id; };

	Vec2<float> GetRenderPos(float alpha);
	Vec2<float> GetPos() { return pos; }
	Vec2F GetPrevPos() { return prevPos; };
	Vec2F GetHalfSize() { return _halfSize; };
	unsigned int GetID() const { return _id; };

	Vec2I GetScreenPos(float alpha);

	void SetPos(Vec2<float> POS) { pos = POS; };
	void SavePrevPos() { prevPos = pos; };

	//게임 오브젝트의 위치를 설정하는 함수 추가
	//추가 수정이 필요하다 
	void SetPosition(Vec2<float> _pos);
	

	//게임 오브젝트의 크기를 설정하는 함수
	//추가 수정이 필요하다 
	void SetSize(float x, float y);

	//halfsize를 저장할 함수 추가
	void SetHalfSize(Vec2<float> halfsize);


	Layers GetLayer() { return layer; };

protected:
	Vec2<float> pos{};
	Vec2<float> prevPos{};
	Vec2<float> facingDir{};
	Vec2<float> movingDir{};
	Vec2<float> _halfSize{};

	float speed{};

	static unsigned int _sNextId;

	unsigned int _id{};

	Layers layer = Layers::ACTORS;

	OBJECTTYPE type = OBJECTTYPE::NONE;
};
