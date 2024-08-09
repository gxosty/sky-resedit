#include "resedit.hpp"

#include <Cipher/Cipher.h>
#include <Cipher/CipherUtils.h>
#include <misc/Logger.h>

#include <future>
#include <filesystem>

namespace resedit
{
	namespace fs = std::filesystem;

	State _state = State::NotInitialized;
	std::function<void(State)> state_change_callback;

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

	void _set_state(State state)
	{
		_state = state;

		if (state_change_callback.target<void(State)>())
		{
			state_change_callback(_state);
		}
	}

	State get_state()
	{
		return _state;
	}

	void initialize()
	{
		uintptr_t vfs_readfile_addr = CipherUtils::CipherScanPattern("", Flags::ReadAndExecute);

		if (vfs_readfile_addr)
		{
			_vfs_readfile_cipher_hook = (new CipherHook())
				->set_Hook((uintptr_t)_vfs_readfile_hook)
				->set_Callback((uintptr_t)&_vfs_readfile_orig)
				->set_Address(Cipher::get_libBase() + 0x1AA171C, false)
				->Fire();
		}



		if (!vfs_readfile_addr)
		{
			_set_state(State::HookError);
			return;
		}

		_set_state(State::Initialized);
		_vfs_readfile_promise.set_value();
	}

	void import_resource_pack()
	{

	}

	void remove_resource_pack(std::weak_ptr<core::ResourcePack> resource_pack)
	{
		_resource_pack_manager.remove(resource_pack);
	}

	bool move_resource_pack(std::weak_ptr<core::ResourcePack> resource_pack, core::ResourcePackManager::MoveDirection direction)
	{
		size_t idx = _resource_pack_manager.get_index(resource_pack);
		size_t new_idx = _resource_pack_manager.move(idx, direction);

		return idx != new_idx;
	}

	size_t get_resource_pack_count()
	{
		return _resource_pack_manager.count();
	}

	std::weak_ptr<core::ResourcePack> get_resource_pack(size_t idx)
	{
		return _resource_pack_manager.get(idx);
	}
}