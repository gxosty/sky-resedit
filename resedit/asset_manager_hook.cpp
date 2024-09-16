#include "asset_manager_hook.hpp"

#include <Dobby/dobby.h>

#include <unordered_map>
#include <string>

namespace resedit::asset_manager_hook
{
	std::function<uint64_t(AAsset*, const std::string&, char* buffer, uint64_t)> _asset_read_callback;
	std::function<off_t(AAsset*, const std::string&)> _asset_get_length_callback;

	thread_local std::unordered_map<AAsset*, std::string> asset_filenames;

	AAsset* (*aassetmanager_open_orig)(AAssetManager* mgr, const char* filename, int mode) = nullptr;
	AAsset* _aassetmanager_open_hook(AAssetManager* mgr, const char* filename, int mode)
	{
		AAsset* asset = aassetmanager_open_orig(mgr, filename, mode);

		if (asset)
		{
			std::string str_filename = std::string(filename);

			if (!str_filename.empty())
				asset_filenames[asset] = str_filename;
		}

		return asset;
	}

	int (*aasset_read_orig)(AAsset* asset, void* buf, size_t count) = nullptr;
	int _aasset_read_hook(AAsset* asset, void* buf, size_t count)
	{
		if (_asset_read_callback && asset_filenames.contains(asset))
		{
			return _asset_read_callback(asset, asset_filenames[asset], (char*)buf, count);
		}

		return aasset_read_orig(asset, buf, count);
	}

	void (*aasset_close_orig)(AAsset* asset) = nullptr;
	void _aasset_close_hook(AAsset* asset)
	{
		if (asset_filenames.contains(asset))
		{
			asset_filenames.erase(asset);
		}

		aasset_close_orig(asset);
	}

	off_t (*aasset_get_length_orig)(AAsset* asset) = nullptr;
	off_t _aasset_get_length_hook(AAsset* asset)
	{
		if (_asset_get_length_callback && asset_filenames.contains(asset))
		{
			return _asset_get_length_callback(asset, asset_filenames[asset]);
		}

		return aasset_get_length_orig(asset);
	}

	int (*aasset_is_allocated_orig)(AAsset* asset) = nullptr;
	int _aasset_is_allocated_hook(AAsset* asset)
	{
		// force AAsset_read to be used
		return 0;
	}

	void set_asset_read_callback(
		std::function<uint64_t(AAsset*, const std::string&, char* buffer, uint64_t)> callback)
	{
		_asset_read_callback = callback;
	}

	void set_asset_get_length_callback(
		std::function<off_t(AAsset*, const std::string&)> callback)
	{
		_asset_get_length_callback = callback;
	}

	void apply_hooks()
	{
		DobbyHook((void*)AAssetManager_open, (void*)_aassetmanager_open_hook, (void**)&aassetmanager_open_orig);
		DobbyHook((void*)AAsset_read, (void*)_aasset_read_hook, (void**)&aasset_read_orig);
		DobbyHook((void*)AAsset_close, (void*)_aasset_close_hook, (void**)&aasset_close_orig);
		DobbyHook((void*)AAsset_getLength, (void*)_aasset_get_length_hook, (void**)&aasset_get_length_orig);
		DobbyHook((void*)AAsset_isAllocated, (void*)_aasset_is_allocated_hook, (void**)&aasset_is_allocated_orig);
	}

	void restore_hooks()
	{
		DobbyDestroy((void*)AAssetManager_open);
		DobbyDestroy((void*)AAsset_read);
		DobbyDestroy((void*)AAsset_close);
		DobbyDestroy((void*)AAsset_getLength);
		DobbyDestroy((void*)AAsset_isAllocated);
	}
}