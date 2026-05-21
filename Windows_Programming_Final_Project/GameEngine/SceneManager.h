#pragma once
class SceneManager
{
	DECLARE_SINGLE(SceneManager);


public:
	void init();
	void Update();
	void Render(HDC hdc);

	void Clear();

	void ChangeScene();

private:

};

