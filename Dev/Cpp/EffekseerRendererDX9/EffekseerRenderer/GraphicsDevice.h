
#ifndef __EFFEKSEERRENDERER_DX9_GRAPHICS_DEVICE_H__
#define __EFFEKSEERRENDERER_DX9_GRAPHICS_DEVICE_H__

#include "../../EffekseerRendererCommon/EffekseerRenderer.CommonUtils.h"
#include <Effekseer.h>
#include <assert.h>
#include <d3d9.h>
#include <set>

namespace EffekseerRendererDX9
{
namespace Backend
{

using Direct3DVertexBuffer9Ptr = std::unique_ptr<IDirect3DVertexBuffer9, Effekseer::ReferenceDeleter<IDirect3DVertexBuffer9>>;
using Direct3DIndexBuffer9Ptr = std::unique_ptr<IDirect3DIndexBuffer9, Effekseer::ReferenceDeleter<IDirect3DIndexBuffer9>>;
using Direct3DDevice9Ptr = std::unique_ptr<IDirect3DDevice9, Effekseer::ReferenceDeleter<IDirect3DDevice9>>;
using Direct3DVertexShader9Ptr = std::unique_ptr<IDirect3DVertexShader9, Effekseer::ReferenceDeleter<IDirect3DVertexShader9>>;
using Direct3DPixelShader9Ptr = std::unique_ptr<IDirect3DPixelShader9, Effekseer::ReferenceDeleter<IDirect3DPixelShader9>>;
using Direct3DVertexDeclaration9Ptr = std::unique_ptr<IDirect3DVertexDeclaration9, Effekseer::ReferenceDeleter<IDirect3DVertexDeclaration9>>;
using Direct3DTexture9Ptr = std::unique_ptr<IDirect3DTexture9, Effekseer::ReferenceDeleter<IDirect3DTexture9>>;
using Direct3DSurface9Ptr = std::unique_ptr<IDirect3DSurface9, Effekseer::ReferenceDeleter<IDirect3DSurface9>>;
using Direct3DVertexDeclaration9Ptr = std::unique_ptr<IDirect3DVertexDeclaration9, Effekseer::ReferenceDeleter<IDirect3DVertexDeclaration9>>;

class GraphicsDevice;
class VertexBuffer;
class IndexBuffer;
class UniformBuffer;
class Shader;
class VertexLayout;
class FrameBuffer;
class Texture;
class RenderPass;
class PipelineState;
class UniformLayout;

using GraphicsDeviceRef = Effekseer::RefPtr<GraphicsDevice>;
using VertexBufferRef = Effekseer::RefPtr<VertexBuffer>;
using IndexBufferRef = Effekseer::RefPtr<IndexBuffer>;
using UniformBufferRef = Effekseer::RefPtr<UniformBuffer>;
using ShaderRef = Effekseer::RefPtr<Shader>;
using VertexLayoutRef = Effekseer::RefPtr<VertexLayout>;
using FrameBufferRef = Effekseer::RefPtr<FrameBuffer>;
using TextureRef = Effekseer::RefPtr<Texture>;
using RenderPassRef = Effekseer::RefPtr<RenderPass>;
using PipelineStateRef = Effekseer::RefPtr<PipelineState>;
using UniformLayoutRef = Effekseer::RefPtr<UniformLayout>;

class DeviceObject
{
private:
public:
	virtual void OnLostDevice();

	virtual void OnChangeDevice();

	virtual void OnResetDevice();
};

/**
	@brief	VertexBuffer of DirectX9
*/
class VertexBuffer
	: public DeviceObject,
	  public Effekseer::Backend::VertexBuffer
{
private:
	EffekseerRenderer::DirtiedBlock blocks_;

	GraphicsDevice* graphicsDevice_ = nullptr;
	Direct3DVertexBuffer9Ptr buffer_ = nullptr;
	int32_t size_ = 0;
	bool isDynamic_ = false;

public:
	VertexBuffer(GraphicsDevice* graphicsDevice);

	~VertexBuffer() override;

	bool Allocate(int32_t size, bool isDynamic);

	void Deallocate();

	void OnLostDevice() override;

	void OnChangeDevice() override;

	void OnResetDevice() override;

	bool Init(int32_t size, bool isDynamic);

	void UpdateData(const void* src, int32_t size, int32_t offset);

	IDirect3DVertexBuffer9* GetBuffer()
	{
		return buffer_.get();
	}
};

/**
	@brief	IndexBuffer of DirectX9
*/
class IndexBuffer
	: public DeviceObject,
	  public Effekseer::Backend::IndexBuffer
{
private:
	EffekseerRenderer::DirtiedBlock blocks_;

	GraphicsDevice* graphicsDevice_ = nullptr;
	Direct3DIndexBuffer9Ptr buffer_;
	int32_t stride_ = 0;

public:
	IndexBuffer(GraphicsDevice* graphicsDevice);

	~IndexBuffer() override;

	bool Allocate(int32_t elementCount, int32_t stride);

	void Deallocate();

	void OnLostDevice() override;

	void OnChangeDevice() override;

	void OnResetDevice() override;

	bool Init(int32_t elementCount, int32_t stride);

	void UpdateData(const void* src, int32_t size, int32_t offset);

	IDirect3DIndexBuffer9* GetBuffer()
	{
		return buffer_.get();
	};
};

class Texture
	: public DeviceObject,
	  public Effekseer::Backend::Texture
{
	Direct3DTexture9Ptr texture_ = nullptr;
	Direct3DSurface9Ptr surface_ = nullptr;
	GraphicsDevice* graphicsDevice_ = nullptr;
	std::function<void(IDirect3DTexture9*&)> onLostDevice_;
	std::function<void(IDirect3DTexture9*&)> onResetDevice_;
	IDirect3DTexture9* keyTexture_ = nullptr;

public:
	Texture(GraphicsDevice* graphicsDevice);
	~Texture() override;

	bool Init(const Effekseer::Backend::TextureParameter& param, const Effekseer::CustomVector<uint8_t>& initialData);

	bool Init(const Effekseer::Backend::DepthTextureParameter& param);

	bool Init(IDirect3DTexture9* texture, std::function<void(IDirect3DTexture9*&)> onLostDevice, std::function<void(IDirect3DTexture9*&)> onResetDevice);

	void OnLostDevice() override;

	void OnChangeDevice() override;

	void OnResetDevice() override;

	IDirect3DTexture9* GetTexture() const
	{
		return texture_.get();
	}
};

class VertexLayout
	: public DeviceObject,
	  public Effekseer::Backend::VertexLayout
{
private:
	GraphicsDevice* graphicsDevice_ = nullptr;
	Effekseer::CustomVector<Effekseer::Backend::VertexLayoutElement> elements_;
	Direct3DVertexDeclaration9Ptr vertexDeclaration_;

	bool Generate();

public:
	VertexLayout(GraphicsDevice* graphicsDevice);
	~VertexLayout() override;

	void MakeGenerated();

	bool Init(const Effekseer::Backend::VertexLayoutElement* elements, int32_t elementCount);

	void OnLostDevice() override;

	void OnChangeDevice() override;

	void OnResetDevice() override;

	IDirect3DVertexDeclaration9* GetVertexDeclaration() const
	{
		return vertexDeclaration_.get();
	}
};

class Shader
	: public DeviceObject,
	  public Effekseer::Backend::Shader
{
private:
	GraphicsDevice* graphicsDevice_ = nullptr;
	IDirect3DVertexShader9* vertexShader_ = nullptr;
	IDirect3DPixelShader9* pixelShader_ = nullptr;
	Effekseer::CustomVector<uint8_t> vertexShaderData_;
	Effekseer::CustomVector<uint8_t> pixelShaderData_;

	bool GenerateShaders();
	void ResetShaders();

public:
	Shader(GraphicsDevice* graphicsDevice);
	~Shader() override;
	bool Init(const void* vertexShaderData, int32_t vertexShaderDataSize, const void* pixelShaderData, int32_t pixelShaderDataSize);

	void OnLostDevice() override;

	void OnChangeDevice() override;

	void OnResetDevice() override;

	IDirect3DVertexShader9* GetVertexShader() const
	{
		return vertexShader_;
	}

	IDirect3DPixelShader9* GetPixelShader() const
	{
		return pixelShader_;
	}
};

/**
	@brief	GraphicsDevice of DirectX9
*/
class GraphicsDevice
	: public Effekseer::Backend::GraphicsDevice
{
private:
	Direct3DDevice9Ptr device_ = nullptr;
	std::set<DeviceObject*> objects_;

public:
	GraphicsDevice(IDirect3DDevice9* device);

	~GraphicsDevice() override;

	IDirect3DDevice9* GetDevice() const;

	void LostDevice();

	void ChangeDevice(IDirect3DDevice9* device);

	void ResetDevice();

	void Register(DeviceObject* deviceObject);

	void Unregister(DeviceObject* deviceObject);

	Effekseer::Backend::VertexBufferRef CreateVertexBuffer(int32_t size, const void* initialData, bool isDynamic) override;

	Effekseer::Backend::IndexBufferRef CreateIndexBuffer(int32_t elementCount, const void* initialData, Effekseer::Backend::IndexBufferStrideType stride) override;

	Effekseer::Backend::TextureRef CreateTexture(const Effekseer::Backend::TextureParameter& param, const Effekseer::CustomVector<uint8_t>& initialData) override;

	//! for DirectX9
	Effekseer::Backend::TextureRef CreateTexture(IDirect3DTexture9* texture, std::function<void(IDirect3DTexture9*&)> onLostDevice, std::function<void(IDirect3DTexture9*&)> onResetDevice);

	Effekseer::Backend::VertexLayoutRef CreateVertexLayout(const Effekseer::Backend::VertexLayoutElement* elements, int32_t elementCount) override;

	Effekseer::Backend::ShaderRef CreateShaderFromBinary(const void* vsData, int32_t vsDataSize, const void* psData, int32_t psDataSize) override;
};

} // namespace Backend

} // namespace EffekseerRendererDX9

#endif