#pragma once

class FlipBook;

class GameObject
{
public:

	GameObject();
	virtual ~GameObject();

	virtual void Init();
	virtual void Update();
	virtual void Render(ID2D1RenderTarget* renderTarget, float alpha);
	virtual bool CheckDead();

	bool operator==(const GameObject& rhs) { return _id == rhs._id; };

	Vec2<float> GetRenderPos(float alpha);
	Vec2<float> GetPos() { return pos; }
	Vec2F GetPrevPos() { return prevPos; };
	Vec2F GetHalfSize() { return _halfSize; };
	Layers GetLayer() { return layer; };
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


	void PlayAnimation(FlipBook* anim);
	void UpdateAnimation(float dt);
	void RenderAnimation(ID2D1RenderTarget* renderTarget, float renderX, float renderY);





protected:
	Vec2F pos{};
	Vec2F prevPos{};
	Vec2F facingDir{};
	Vec2F movingDir{};
	Vec2F _halfSize{};
	Vec2F _renderOffset{};

	float _rotationAngle{};
	float speed{};

	static unsigned int _sNextId;

	unsigned int _id{};

	Layers layer = Layers::ACTORS;
	OBJECTTYPE type = OBJECTTYPE::NONE;

	FlipBook*	_currAnim{};
	int			_currFrame{};
	float		_animTimer{};

};
