#pragma once

#include <android/asset_manager.h>

#include <string>
#include <functional>
#include <cstdint>

namespace resedit::asset_manager_hook
{
	extern AAsset* (*aassetmanager_open_orig)(AAssetManager* mgr, const char* filename, int mode);
	extern void    (*aasset_close_orig)(AAsset* asset);
	extern int     (*aasset_read_orig)(AAsset* asset, void* buf, size_t count);
	extern off_t   (*aasset_get_length_orig)(AAsset* asset);
	extern int     (*aasset_is_allocated_orig)(AAsset* asset);

	void set_asset_read_callback(
		std::function<uint64_t(AAsset*, const std::string&, char* buffer, uint64_t)> callback);

	void set_asset_get_length_callback(
		std::function<off_t(AAsset*, const std::string&)> callback);

	void apply_hooks();
	void restore_hooks();
}