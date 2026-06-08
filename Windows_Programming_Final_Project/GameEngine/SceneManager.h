#pragma once
class Scene;

struct MapData;
class GameObject;

class SceneManager
{
	DECLARE_SINGLE(SceneManager);


public:
	void init();
	void Update();
	void Render(ID2D1RenderTarget* renderTarget,float alpha);

	void Clear();

	void ChangeScene(SceneType sceneType);
	void ChangeScene(SceneType sceneType, const std::wstring& mapFilePath);
	void ChangeMap(const MapData& nextMapData);
	Scene* GetCurrentScene() { return _scene; };

	Vec2F GetCameraPos() { return cameraPos; };
	void SetCameraPos(Vec2F pos) { cameraPos = pos; };

	void SetCamOwner(GameObject* object);
	
	Vec2F ToRenderPos(Vec2F alphaPos);

private:
	Scene* _scene;
	SceneType _sceneType = SceneType::NONE;

	Vec2F cameraPos{};
	Vec2F winSizeV{ WinSizeX,WinSizeY };
	static constexpr Vec2F halfWinSizeV{ WinSizeX / 2.0f,WinSizeY / 2.0f };
};

