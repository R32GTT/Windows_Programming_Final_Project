#pragma once
class Scene;

class SceneManager
{
	DECLARE_SINGLE(SceneManager);


public:
	void init();
	void Update();
	void Render(HDC hdc,float alpha);

	void Clear();

	void ChangeScene(SceneType sceneType);
	void ChangeScene(SceneType sceneType, const std::wstring& mapFilePath);
	Scene* GetCurrentScene() { return _scene; };

	Vec2<int> GetCameraPos() { return cameraPos; };
	void SetCameraPos(Vec2<int> pos) { cameraPos = pos; };

private:
	Scene* _scene;
	SceneType _sceneType = SceneType::NONE;

	Vec2<int> cameraPos{};
};

