#pragma once
#include <string>
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
	void ChangeScene(SceneType sceneType, const std::wstring& mapFilePath, const std::wstring& chapterFilePath);
	void ChangeMap(const MapData& nextMapData);
	void ResetCurrentMap();
	void RequestMapChange(const std::string& targetMapName = "");
	Scene* GetCurrentScene() { return _scene; };

	Vec2F GetCameraPos() { return cameraPos; };
	void SetCameraPos(Vec2F pos) { cameraPos = pos; };

	void SetCamOwner(GameObject* object);
	
	Vec2F ToRenderPos(Vec2F alphaPos);

	//점수 시스템 로직
	void ResetAllScore();
	void OnEnemyKilled();
	int GetTotalScore() { return static_cast<int>(_totalScore); }
	int GetCurrentCombo() { return _currentCombo; }

	//PlayScene에서 점수를 그리기 위해 필요한 정보
	float GetComboTimer() { return _comboTimer; }
	bool GetIsGameEnded() { return _isGameEnded; }


private:
	void ExecuteMapChange();

	bool _isMapChangeRequested = false;
	std::string _nextMapName = "";

	Scene* _scene;
	SceneType _sceneType = SceneType::NONE;

	Vec2F cameraPos{};
	Vec2F winSizeV{ WinSizeX,WinSizeY };
	static constexpr Vec2F halfWinSizeV{ WinSizeX / 2.0f,WinSizeY / 2.0f };

	//점수 및 상태 시스텝 변수
	bool _isGameEnded = false; //게임이 모두 끝나고 정산창을 띄울지 여부

	float _totalScore = 0.f;
	float _savedScore = 0.f;

	int _currentCombo = 0;
	float _comboTimer = 0.f;

	static constexpr float COMBO_TIME_LIMIT = 3.0f;
	static constexpr float SCORE_TIME_PENALTY = 3.0f;
	static constexpr float BASE_KILL_SCORE = 3.0f;
	static constexpr float COMBO_BONUS_SCORE = 3.0f;


};

