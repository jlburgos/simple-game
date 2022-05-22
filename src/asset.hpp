#pragma once

#include "custom_sdl_pointers.hpp"

class Asset
{
private:
	//const char** filename = nullptr;
  shared_surface_ptr surface = nullptr;
  shared_texture_ptr texture = nullptr;
  shared_renderer_ptr renderer = nullptr;
  SDL_Rect spriteSrcInTexture = {0,0,0,0};
  SDL_Rect spriteDstOnSurface = {0,0,0,0};

public:
	Asset() = delete;
	Asset(shared_renderer_ptr _renderer, const char* filename);
	~Asset() = default;

	shared_texture_ptr get_texture() const;
	shared_renderer_ptr get_renderer() const;
	SDL_Rect get_src_texture_rect() const;
	SDL_Rect get_dst_surface_rect() const;

	int scale_texture(const float scale);

	class AssetException : public std::runtime_error
	{
	public:
		explicit AssetException(const char* msg);
	};
};

using shared_asset_ptr = std::shared_ptr<Asset>;
std::shared_ptr<Asset> mk_shared_asset_ptr(Asset* asset);
