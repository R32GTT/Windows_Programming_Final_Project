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

	


	Layers GetLayer() { return layer; };

protected:
	Vec2<float> pos{};
	Vec2<float> prevPos{};
	Vec2<float> facingDir{};
	Vec2<float> movingDir{};
	float speed{};

	static unsigned int _sNextId;

	unsigned int _id{};

	Layers layer = Layers::ACTORS;

	OBJECTTYPE type = OBJECTTYPE::NONE;
};
