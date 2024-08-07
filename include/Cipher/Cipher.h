#pragma once

#include <vector>
#include <string>


enum Section {
    BOOTLOADER_ROD = 0,
    BOOTLOADER_RWP,
    BOOTLOADER_RXP,
};

/**
 * @brief Provides an API for Canvas like memory scanning within the game's library.
 *
 * This class offers methods for obtaining game version information, detecting beta status,
 * retrieving library base addresses, scanning memory regions for specific patterns,
 * and accessing configuration paths and asset data. It serves as an API for the mod loader.
 */
class Cipher {
public:
    /**
     * @brief Retrieves the game version.
     * @return The game version as a 32-bit unsigned integer.
     */
    static std::uint32_t getGameVersion();

    /**
     * @brief Checks if the game is the beta build.
     * @return True if the game is in beta, false otherwise.
     */
    static bool isGameBeta();

    /**
     * @brief Retrieves the base address of the game's library.
     * @return The base address of the game library as an std::uintptr_t.
     */
    static std::uintptr_t get_libBase();

    /**
     * @brief Retrieves the name of the game's library.
     * @return A string containing the name of the game library.
     */
    static const char *get_libName();

    /**
     * @brief Scans the game's library for a specific byte array using wildcard masking.
     * @param _bytes The byte array to search for.
     * @param _mask The _mask specifying which bytes to compare in the _bytes.
     * @return The address of the first occurrence of the bytes.
     */
    static std::uintptr_t CipherScan(const char *_bytes, const char *_mask); //librange

    /**
     * @brief Scans a specified memory region for a specific byte array using wildcard masking.
     * @param _start The start address of the memory region to scan.
     * @param _size The size of the memory region to scan.
     * @param _bytes The byte bytes to search for.
     * @param _mask The mask specifying which bytes to compare in the _bytes.
     * @return The address of the first occurrence of the bytes.
     */
    static std::uintptr_t CipherScan(std::uintptr_t _start, const size_t _size, const char *_bytes, const char *_mask);

    /**
     * @brief Scans the game's library for all occurrences of a specific byte array using wildcard masking.
     * @param _bytes The byte bytes to search for.
     * @param _mask The mask specifying which bytes to compare in the bytes.
     * @return A vector containing the addresses of all occurrences of the bytes.
     */
    static std::vector<std::uintptr_t> CipherScanAll(const char *_bytes, const char *_mask); //librange

    /**
     * @brief Scans a specified memory region for all occurrences of a specific byte array using wildcard masking.
     * @param _start The start address of the memory region to scan.
     * @param _end The end address of the memory region to scan.
     * @param _bytes The bytes to search for.
     * @param _mask The mask specifying which bytes to compare in the bytes.
     * @return A vector containing the addresses of all occurrences of the bytes.
     */
    static std::vector<std::uintptr_t> CipherScanAll(std::uintptr_t _start, std::uintptr_t _end, const char *_bytes, const char *_mask);

    /**
     * @brief Scans a specified memory section for a specific byte array using wildcard masking.
     * @param _bytes The bytes to search for.
     * @param _mask The mask specifying which bytes to compare in the bytes.
     * @param _section The section of memory to scan (default is BOOTLOADER_RXP).
     * @return The address of the first occurrence of the bytes within the specified memory section.
     */
    static std::uintptr_t CipherScanSegments(const char *_bytes, const char *_mask, const Section &_section = BOOTLOADER_RXP);

    /**
     * @brief Scans the game's library for a specific pattern using IDA-style pattern format.
     * @param _pattern The IDA-style pattern to search for.
     * @return The address of the first occurrence of the pattern.
     */
    static std::uintptr_t CipherScanIdaPattern(const std::string &_pattern); //librange

    /**
     * @brief Scans a specified memory region for a specific pattern using IDA-style pattern format.
     * @param _start The start address of the memory region to scan.
     * @param _end The end address of the memory region to scan.
     * @param _pattern The IDA-style pattern to search for.
     * @return The address of the first occurrence of the pattern.
     */
    static uintptr_t CipherScanIdaPattern(const std::uintptr_t _start, const std::uintptr_t _end, const std::string &_pattern);

    /**
     * @brief Scans the game's library for all occurrences of a specific pattern using IDA-style pattern format.
     * @param _pattern The IDA-style pattern to search for.
     * @return A vector containing the addresses of all occurrences of the pattern.
     */
    static std::vector<std::uintptr_t> CipherScanIdaPatternAll(const std::string &_pattern); //librange

    /**
     * @brief Scans a specified memory region for all occurrences of a specific pattern using IDA-style pattern format.
     * @param _start The start address of the memory region to scan.
     * @param _end The end address of the memory region to scan.
     * @param _pattern The IDA-style pattern to search for.
     * @return A vector containing the addresses of all occurrences of the pattern.
     */
    static std::vector<std::uintptr_t> CipherScanIdaPatternAll(const std::uintptr_t _start, const std::uintptr_t _end, const std::string &_pattern);

    /**
     * @brief Retrieves the path to the mod's configuration file.
     * @return A pointer to a string containing the path to the configuration file.
     */
    static const char *getConfigPath();

    /**
     * @brief Reads asset data from a specified path.
     * @param _assetPath The path to the asset.
     * @return A pointer to a char array containing the asset data.
     */
    static char* read_asset(char* _assetPath);
};

enum Types {
    e_patch,
    e_hook
};

/**
 * @brief Base class for cipher implementations.
 */
class CipherBase {
public:
    /**
     * @brief Constructor for CipherBase class.
     */
    CipherBase();
    /**
     * @brief Destructor for CipherBase class (pure virtual).
     */
    virtual ~CipherBase() = 0;

    /**
     * @brief Set the name of the associated library.
     * @param _libName Name of the library.
     * @return Pointer to the current CipherBase chatInfo.
     */
    CipherBase* set_libName(const char* _libName);
    /**
     * @brief Set the address of the cipher using a symbol name.
     * @param _symbol Symbol name to set the address.
     * @return Pointer to the current CipherBase chatInfo.
     */
    CipherBase* set_Address(const char* _symbol);
    /**
     * @brief Set the address of the cipher.
     * @param _address Address to set.
     * @param _isLocal Flag indicating if the address is local to the library.
     * @return Pointer to the current CipherBase chatInfo.
     */
    CipherBase* set_Address(std::uintptr_t _address, bool _isLocal = true);
    /**
     * @brief Set the address of the cipher using bytes and a mask.
     * @param _bytes Bytes representing the address.
     * @param _mask Mask to apply to the bytes.
     * @return Pointer to the current CipherBase chatInfo.
     */
    CipherBase* set_Address(const char* _bytes, const char* _mask);

    /**
     * @brief Set the lock status of the cipher.
     * @param _isLocked Lock status to set.
     * @return Pointer to the current CipherBase chatInfo.
     */
    CipherBase* set_Lock(bool _isLocked);
    /**
     * @brief Trigger the cipher.
     * @return Pointer to the triggered CipherBase chatInfo.
     */
    virtual CipherBase* Fire() = 0;
    /**
     * @brief Restore the cipher.
     */
    virtual void Restore() = 0;
};


/**
 * @brief Represents a hook applied to the game's memory.
 * Inherits from CipherBase.
 */
class CipherHook : public CipherBase {
public:
    /**
     * @brief Constructs a new CipherHook object.
     */
    CipherHook();

    /**
     * @brief Destroys the CipherHook object and restores the hook.
     */
    ~CipherHook() override;

    /**
     * @brief Sets the address of the hook function.
     * @param _hook The address of the hook function.
     * @return A pointer to the current CipherHook object.
     */
    CipherHook* set_Hook(std::uintptr_t _hook);

    /**
     * @brief Sets the address of the callback function.
     * @param _callback The address of the callback function.
     * @return A pointer to the current CipherHook object.
     */
    CipherHook* set_Callback(std::uintptr_t _callback);

    /**
     * @brief Applies the hook to the game's memory.
     * @return A pointer to the current CipherHook object.
     */
    CipherHook* Fire() override;

    /**
     * @brief Restores the hook.
     */
    void Restore() override;
};

/**
 * @brief Represents a patch applied to the game's memory.
 * Inherits from CipherBase.
 */
class CipherPatch : public CipherBase {
public:
    /**
     * @brief Constructs a new CipherPatch object.
     */
    CipherPatch();

    /**
     * @brief Destroys the CipherPatch object and restores the patch.
     */
    ~CipherPatch() override;

    /**
     * @brief Applies the patch to the game's memory.
     * @return A pointer to the current CipherPatch object.
     */
    CipherPatch* Fire() override;

    /**
     * @brief Sets the opcode for the patch.
     * @param _hex The opcode in hexadecimal format.
     * @return A pointer to the current CipherPatch object.
     */
    CipherPatch* set_Opcode(std::string _hex);

    /**
     * @brief Restores the patch.
     */
    void Restore() override;
};
