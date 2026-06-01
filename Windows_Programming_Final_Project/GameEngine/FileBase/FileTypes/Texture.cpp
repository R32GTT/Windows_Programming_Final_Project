#include "pch.h"
#include "Texture.h"

Texture::Texture()
{
}

Texture::~Texture()
{
	SafeRelease(&_bitmap);
}

void Texture::Load(ID2D1RenderTarget* renderTarget, IWICImagingFactory* wicFactory, const std::wstring& path)
{
	if (!renderTarget || !wicFactory)
	{
		::MessageBox(nullptr, L"렌더타겟이나 WIC 팩토리가 nullptr입니다! (GameEngine::Init 확인 필요)", L"로드 에러", MB_OK);
		return;
	}

	IWICBitmapDecoder* decoder = nullptr;
	IWICBitmapFrameDecode* frame = nullptr;
	IWICFormatConverter* converter = nullptr;
	HRESULT hr = S_OK;

	// 2. 디코더 생성 (여기서 파일 경로가 틀리면 터집니다)
	hr = wicFactory->CreateDecoderFromFilename(
		path.c_str(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &decoder);

	if (FAILED(hr))
	{
		std::wstring errorMsg = L"이미지 파일을 찾을 수 없거나 열 수 없습니다.\n시도한 경로: " + path;
		::MessageBox(nullptr, errorMsg.c_str(), L"1단계 에러: 디코더 실패", MB_OK);
		goto RELEASE_RESOURCES; // 에러 났으니 메모리 정리 구역으로 이동
	}

	// 3. 첫 번째 프레임 가져오기
	hr = decoder->GetFrame(0, &frame);
	if (FAILED(hr))
	{
		::MessageBox(nullptr, L"이미지에서 프레임을 추출할 수 없습니다.", L"2단계 에러: 프레임 추출 실패", MB_OK);
		goto RELEASE_RESOURCES;
	}

	// 4. 포맷 컨버터 생성
	hr = wicFactory->CreateFormatConverter(&converter);
	if (FAILED(hr)) goto RELEASE_RESOURCES;

	// 5. 컨버터 초기화 (D2D가 인식할 수 있는 32비트 포맷으로 변환)
	hr = converter->Initialize(
		frame, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone,
		nullptr, 0.f, WICBitmapPaletteTypeMedianCut);
	if (FAILED(hr)) goto RELEASE_RESOURCES;

	// 6. 대망의 D2D 비트맵 생성!
	hr = renderTarget->CreateBitmapFromWicBitmap(converter, nullptr, &_bitmap);
	if (FAILED(hr) || _bitmap == nullptr)
	{
		::MessageBox(nullptr, L"D2D 비트맵 생성에 실패했습니다.", L"3단계 에러: 비트맵 생성 실패", MB_OK);
		goto RELEASE_RESOURCES;
	}

	// 정상적으로 생성되었다면 사이즈 저장
	if (_bitmap)
	{
		D2D1_SIZE_F size = _bitmap->GetSize();
		_size.x = size.width;
		_size.y = size.height;
	}


RELEASE_RESOURCES:
	SafeRelease(&converter);
	SafeRelease(&frame);
	SafeRelease(&decoder);
}


