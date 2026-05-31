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

	unsigned int GetID() const { return _id; };

	void SetPos(Vec2<float> POS) { pos = POS; };
	void SavePrevPos() { prevPos = pos; };

	//게임 오브젝트의 위치를 설정하는 함수 추가
	//void SetPosition(float _x, float _y);

	//게임 오브젝트의 크기를 설정하는 함수
	//void SetSize(int _width, int _height);


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
