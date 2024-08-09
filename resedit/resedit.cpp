#include "resedit.hpp"
#include "core/resource_pack_manager.hpp"

#include <Cipher/Cipher.h>
#include <misc/Logger.h>

namespace resedit
{
	core::ResourcePackManager _resource_pack_manager;
	CipherBase* _vfx_readfile_cipher_hook = nullptr;

	uint64_t (*_vfs_readfile_orig)(void* this_, char* buffer, uint64_t max_len);
	uint64_t _vfs_readfile_hook(void* this_, char* buffer, uint64_t max_len)
	{
		uint64_t written_len = _vfs_readfile_orig(this_, buffer, max_len);

		LOGI("%s", *(const char**)this_);

		return written_len;
	}

	void initialize()
	{
		_vfx_readfile_cipher_hook = (new CipherHook())
			->set_Hook((uintptr_t)_vfs_readfile_hook)
			->set_Callback((uintptr_t)&_vfs_readfile_orig)
			->set_Address(Cipher::get_libBase() + 0x1AA171C, false)
			->Fire();
		return;
	}
}