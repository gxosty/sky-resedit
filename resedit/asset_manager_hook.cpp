#include "asset_manager_hook.hpp"
#include "core/resource_pack_manager.hpp"

#include <misc/Logger.h>

#include <Dobby/dobby.h>

#include <unordered_map>
#include <string>

namespace resedit
{
	extern core::ResourcePackManager _resource_pack_manager;
}

namespace resedit::asset_manager_hook
{
	std::function<uint64_t(AAsset*, const std::string&, bool, char*, uint64_t)> _asset_read_callback;
	std::function<off_t(AAsset*, const std::string&, bool)> _asset_get_length_callback;

	thread_local std::unordered_map<AAsset*, std::string> asset_filenames;
	thread_local std::unordered_map<AAsset*, std::string> injected_asset_filenames;
	thread_local uintptr_t injected_aasset_ptr = 0x15125; // random starting number

	AAsset* (*aassetmanager_open_orig)(AAssetManager* mgr, const char* filename, int mode) = nullptr;
	AAsset* _aassetmanager_open_hook(AAssetManager* mgr, const char* filename, int mode)
	{
		AAsset* asset = aassetmanager_open_orig(mgr, filename, mode);
		std::string str_filename = std::string(filename);

		if (asset)
		{
			if (!str_filename.empty())
				asset_filenames[asset] = str_filename;
		}
		else
		{
			if (resedit::_resource_pack_manager.any_edit_for_asset(str_filename))
			{
				LOGI("Opening fake AAsset* for injected asset: %s", filename);
				asset = (AAsset*)injected_aasset_ptr++;
				injected_asset_filenames[asset] = str_filename;
			}
			else
			{
				LOGI("Couldn't AAssetManager_open -> %s", filename);
			}
		}

		return asset;
	}

	int (*aasset_read_orig)(AAsset* asset, void* buf, size_t count) = nullptr;
	int _aasset_read_hook(AAsset* asset, void* buf, size_t count)
	{
		if (_asset_read_callback && asset_filenames.contains(asset))
		{
			return _asset_read_callback(asset, asset_filenames[asset], false, (char*)buf, count);
		}
		else if (_asset_read_callback && injected_asset_filenames.contains(asset))
		{
			return _asset_read_callback(asset, injected_asset_filenames[asset], true, (char*)buf, count);
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
		else if (injected_asset_filenames.contains(asset))
		{
			injected_asset_filenames.erase(asset);
			return;
		}

		aasset_close_orig(asset);
	}

	off_t (*aasset_get_length_orig)(AAsset* asset) = nullptr;
	off_t _aasset_get_length_hook(AAsset* asset)
	{
		if (_asset_get_length_callback && asset_filenames.contains(asset))
		{
			return _asset_get_length_callback(asset, asset_filenames[asset], false);
		}
		else
		{
			return _asset_get_length_callback(asset, injected_asset_filenames[asset], true);
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
		std::function<uint64_t(AAsset*, const std::string&, bool, char* buffer, uint64_t)> callback)
	{
		_asset_read_callback = callback;
	}

	void set_asset_get_length_callback(
		std::function<off_t(AAsset*, const std::string&, bool)> callback)
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