#include "resedit.hpp"
#include "core/resource_pack_manager.hpp"

#include <Cipher/Cipher.h>
#include <misc/Logger.h>

#include <future>
#include <filesystem>

namespace resedit
{
	namespace fs = std::filesystem;

	core::ResourcePackManager _resource_pack_manager;
	CipherBase* _vfs_readfile_cipher_hook = nullptr;
	std::promise<void> _vfs_readfile_promise;
	std::future<void> _vfs_readfile_future = _vfs_readfile_promise.get_future();

	uint64_t (*_vfs_readfile_orig)(void* this_, char* buffer, uint64_t max_len);
	uint64_t _vfs_readfile_hook(void* this_, char* buffer, uint64_t max_len)
	{
		_vfs_readfile_future.wait();

		uint64_t written_len = _vfs_readfile_orig(this_, buffer, max_len);

		AssetData asset_data{
			std::string(*(const char**)this_),
			buffer,
			max_len,
			&written_len
		};

		if (_resource_pack_manager.handle_asset(asset_data))
		{
			LOGI("Modified: %s", *(const char**)this_);
		}

		return written_len;
	}

	void initialize()
	{
		_vfs_readfile_cipher_hook = (new CipherHook())
			->set_Hook((uintptr_t)_vfs_readfile_hook)
			->set_Callback((uintptr_t)&_vfs_readfile_orig)
			->set_Address(Cipher::get_libBase() + 0x1AA171C, false)
			->Fire();

		_vfs_readfile_promise.set_value();
	}
}