#include "pch.h"
#include "GameEngine.h"
#include "Managers.h"	//각종 매니저들 모아둔 헤더파일


GameEngine::GameEngine()
{
}

GameEngine::~GameEngine()
{
	SafeRelease(&_renderTarget);
	SafeRelease(&_d2dFactory);
	SafeRelease(&_wicFactory);
}

void GameEngine::Init(HWND hWnd)
{
	memhWnd = hWnd;
	GetClientRect(hWnd, &rect);

	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &_d2dFactory);
	if (FAILED(hr))
	{
		::MessageBox(hWnd, L"D2D 팩토리 생성에 실패했습니다.", L"초기화 에러", MB_OK);
		return;
	}


	hr = _d2dFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(hWnd, D2D1::SizeU(rect.right, rect.bottom)),
		&_renderTarget
	);
	if (FAILED(hr))
	{
		::MessageBox(hWnd, L"D2D 렌더 타겟 생성에 실패했습니다.", L"초기화 에러", MB_OK);
		return;
	}

	hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&_wicFactory));
	if (FAILED(hr))
	{
		::MessageBox(hWnd, L"WIC 팩토리 생성에 실패했습니다. (COM 초기화를 확인하세요)", L"초기화 에러", MB_OK);
		return;
	}

	GET_SINGLE(FileManager)->Init(hWnd, L"../Resources", _renderTarget, _wicFactory);

	GET_SINGLE(TimeManager)->Init();
	GET_SINGLE(InputManager)->Init(hWnd);
	GET_SINGLE(SceneManager)->init();

	GET_SINGLE(SceneManager)->ChangeScene(SceneType::DEVSCENE);

}

void GameEngine::Update()
{

	
	GET_SINGLE(TimeManager)->Update();
	float dt = GET_SINGLE(TimeManager)->GetDeltaTime();

	if (dt > 0.25f)
		dt = 0.25f;

	_accumulator += dt;

	while (_accumulator >= _FIXED_DT)
	{

		GET_SINGLE(InputManager)->Update();
		GET_SINGLE(SceneManager)->Update();

		_accumulator -= _FIXED_DT;
	}

	_alpha = _accumulator / _FIXED_DT;
}

void GameEngine::Render()
{
	if (!_renderTarget) return;

	_renderTarget->BeginDraw();

	_renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));



	GET_SINGLE(SceneManager)->Render(_renderTarget, _alpha);

	unsigned int fps = GET_SINGLE(TimeManager)->GetFps();
	float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();

	{
		
	}
	HRESULT hr = _renderTarget->EndDraw();

	if (hr == D2DERR_RECREATE_TARGET)
	{
		SafeRelease(&_renderTarget);
		_d2dFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(memhWnd, D2D1::SizeU(rect.right, rect.bottom)),
			&_renderTarget
		);
	}
}
