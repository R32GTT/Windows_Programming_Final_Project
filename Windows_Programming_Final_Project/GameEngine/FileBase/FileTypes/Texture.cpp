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
	if(!renderTarget || !wicFactory) return;

	IWICBitmapDecoder* decoder = nullptr;
	IWICBitmapFrameDecode* frame = nullptr;
	IWICFormatConverter* converter = nullptr;

	HRESULT hr = wicFactory->CreateDecoderFromFilename(
		path.c_str(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &decoder);

	if (SUCCEEDED(hr))
		hr = decoder->GetFrame(0, &frame);

	if (SUCCEEDED(hr))
		hr = wicFactory->CreateFormatConverter(&converter);

	if (SUCCEEDED(hr))
	{
		hr = converter->Initialize(
			frame, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone,
			nullptr, 0.f, WICBitmapPaletteTypeMedianCut);
	}

	if (SUCCEEDED(hr) && _bitmap)
	{
		D2D1_SIZE_F size = _bitmap->GetSize();
		_size.x = size.width;
		_size.y = size.height;
	}
	else
	{
		::MessageBox(nullptr, path.c_str(), L"Image Load Failed!", NULL);
	}

	SafeRelease(&converter);
	SafeRelease(&frame);
	SafeRelease(&decoder);
		
}

