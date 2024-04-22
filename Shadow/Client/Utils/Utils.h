#pragma once

#pragma region DirectClassAccess

template <typename Ret, typename Type>
// The type and offset
Ret& DirectAccess(Type* type, size_t offset) {
    union {
        size_t raw;
        Type* source;
        Ret* target;
    } u;
    u.source = type;
    u.raw += offset;
    return *u.target;
}

#define AS_FIELD(type, name, fn) __declspec(property(get = fn, put = set##name)) type name
#define DEF_FIELD_RW(type, name) __declspec(property(get = get##name, put = set##name)) type name

#define FAKE_FIELD(type, name)                                                                                       \
AS_FIELD(type, name, get##name);                                                                                     \
type get##name()

#define BUILD_ACCESS(type, name, offset)                                                                             \
AS_FIELD(type, name, get##name);                                                                                     \
type get##name() const { return DirectAccess<type>(this, offset); }												 \
void set##name(type v) const { DirectAccess<type>(this, offset) = v; }

#pragma endregion

namespace Utils {

#pragma region Memory & Debug
#define log(str) OutputDebugString(str)

    // CallFunc is what we'll be using to call the original functions.
    // CallVFunc is what we'll be using to call the vftables by there index.

    // Combine is what we''ll be using to combine strings, ints, arrays etc together.

    // HookFunction is just a clean MinHook util it'll initialize minhook for us if it hasn't already been initialized and created/enabled 
    // the hook aswell as return log information to the debug stream so we can see if hooks fail to create/enable and where they failed

    template <typename R, typename... Args>
    R CallFunc(void* func, Args... args)
    {
        // Call the function and return the result.
        return ((R(*)(Args...))func)(args...);
    }

    template <unsigned int IIdx, typename TRet, typename... TArgs>
    static inline auto CallVFunc(void* thisptr, TArgs... argList) -> TRet {
        using Fn = TRet(__thiscall*)(void*, decltype(argList)...);
        // Call the function and return the result.
        return (*static_cast<Fn**>(thisptr))[IIdx](thisptr, argList...);
    }

    // template function that converts the specified value to a string using a stringstream
    template <typename T>
    std::string combine(T t)
    {
        std::stringstream ss;
        ss << t;
        return ss.str();
    }

    // template function that converts the specified values to a string using a stringstream
    template <typename T, typename... Args>
    std::string combine(T t, Args... args)
    {
        std::stringstream ss;
        ss << t << combine(args...);
        return ss.str();
    }

    // template function that creates a hook at the specified target function with the provided detour function and stores the original function in the specified location
    template <typename T>
    bool HookFunction(void* pTarget, T pDetour, void* pOriginal) {
        static bool initialized = false;
        if (!initialized)
        {
            initialized = true;
            if (MH_Initialize() != MH_OK)
            {
                //log("Error: Failed to initialize MinHook\n");
                return false;
            }
        }

        if (MH_CreateHook(pTarget, pDetour, (LPVOID*)pOriginal) != MH_OK)
        {
            //log("Error: Failed to create hook\n");
            return false;
        }

        if (MH_EnableHook(pTarget) != MH_OK)
        {
            //log("Error: Failed to enable hook\n");
            return false;
        }

        //log(combine("Success: Enabled hook for ", pTarget, "\n").c_str());

        return true;
    }

    // macro that checks if a value is within a range
#define INRANGE(x, a, b) (x >= a && x <= b)

// macro that converts two characters to a single byte
#define GET_BYTE(x) ((GET_BITS(x[0]) << 4) | GET_BITS(x[1]))

// macro that converts a character to a nibble (4 bits)
#define GET_BITS(x) (INRANGE((x & (~0x20)), 'A', 'F') ? ((x & (~0x20)) - 'A' + 0xa) : (INRANGE(x, '0', '9') ? (x - '0') : 0))

// This function searches for a pattern of bytes within the memory range specified by range_start and range_end
// The pattern to search for is specified as a string, where each byte in the pattern is represented by 2 hexadecimal characters
// For example, the pattern "90 90 90 90" would represent the byte sequence 0x90 0x90 0x90 0x90
// If the pattern is found within the specified memory range, a pointer to the start of the pattern is returned
// Otherwise, a null pointer is returne
    static void* findSig(const char* sig) {
        // pointer to the current position in the pattern
        const char* pattern = sig;

        // store the first matched address
        uintptr_t first_match = 0;

        // store the first matched address
        static const uintptr_t range_start = reinterpret_cast<uintptr_t>(GetModuleHandleA("Minecraft.Windows.exe"));

        // store information about the module
        static MODULEINFO mi_mod_info;

        // flag to initialize mi_mod_info only once
        static bool init = false;
        if (!init) {
            // initialize mi_mod_info
            init = true;
            GetModuleInformation(GetCurrentProcess(), reinterpret_cast<HMODULE>(range_start), &mi_mod_info, sizeof(MODULEINFO));
        }
        // calculate the end address of the module
        static const uintptr_t range_end = range_start + mi_mod_info.SizeOfImage;

        // store the current byte in the pattern as a BYTE
        BYTE pat_byte = GET_BYTE(pattern);

        // store the original position in the pattern
        const char* old_pat = pattern;

        // iterate through all addresses in the module
        for (uintptr_t p_cur = range_start; p_cur < range_end; p_cur++) {
            // if we reached the end of the pattern, return the first matched address
            if (!*pattern) {
                return reinterpret_cast<void*>(first_match);
            }

            // skip spaces in the pattern
            while (*(PBYTE)pattern == ' ') {
                pattern++;
            }

            // if we reached the end of the pattern, return the first matched address
            if (!*pattern) {
                return reinterpret_cast<void*>(first_match);
            }

            // if the pattern has changed from the last iteration, update the pat_byte value
            if (old_pat != pattern) {
                old_pat = pattern;
                // if the current byte in the pattern is not a wildcard '?', update pat_byte to the current byte value
                if (*(PBYTE)pattern != '\?') {
                    pat_byte = GET_BYTE(pattern);
                }
            }

            // if the current byte in the pattern is a wildcard or if the current byte in the pattern matches the current byte in the range,
            // increment the pattern by 2 and set first_match to the current address if it is not already set
            if (*(PBYTE)pattern == '\?' || *(BYTE*)p_cur == pat_byte) {
                if (!first_match) {
                    first_match = p_cur;
                }

                // if the next byte in the pattern is the end, return the first matched address
                if (!pattern[2]) {
                    return reinterpret_cast<void*>(first_match);
                }

                pattern += 2;
            }
            else { // if the current byte in the pattern does not match the current byte in the range, reset the pattern to the start and clear first_match
                pattern = sig;
                first_match = 0;
            }
        }
    }
#pragma endregion

#pragma region Folders and Assets
    // Get Minecraft's RoamingState folder location.
    std::string getRoamingStatePath(void) {
        char* path = NULL;
        size_t length;

        // _dupenv_s Is used to retrieve an environment variable’s value.
        _dupenv_s(&path, &length, "appdata");

        // Return Minecraft's "RoamingState" path.
        return std::string(path) + "\\..\\Local\\Packages\\Microsoft.MinecraftUWP_8wekyb3d8bbwe\\RoamingState";
    };

    // Get the client's folder or create one if it doesn't exist.
    std::string getClientPath(void) {
        // The Client's file location.
        std::string ClientFilePath = getRoamingStatePath() + "\\Nigger";

        // Convert ClientFilePath to a filesystem::path.
        auto folderpath = std::filesystem::path(ClientFilePath);

        // Check if FolderPath exists.
        if (!std::filesystem::exists(folderpath)) { // If folder doesn't exist.
            std::filesystem::create_directories(folderpath); // Create a new folder.
        }

        // Return the Client's folder location.
        return ClientFilePath;
    }

    // Get the resource folder inside the client's folder.
    std::string getResourcePath(void) {
        // The Resource's file location.
        std::string ResourcePath = getClientPath() + "\\Resource";

        // Convert ResourcePath to a filesystem::path.
        auto ResourceFolder = std::filesystem::path(ResourcePath);

        // Check if ResourcePath exists.
        if (!std::filesystem::exists(ResourceFolder)) // If folder doesn't exist.
            std::filesystem::create_directories(ResourceFolder); // Create a new folder.

        // Return the Resource folder location.
        return ResourcePath;
    }

    // Checks if the path exist or no.
    bool doesPathExist(std::string path) {
        // Returns a bool to check if path exists.
        return std::filesystem::exists(path);
    }

    // Download a file using urlmon.
    void downloadFile(std::string url, std::string path) {
        // If the path/location already exists.
        if (Utils::doesPathExist(path)) return; // Return to stop it from downloading.

        // Downloads a file from a URL link.
        URLDownloadToFileA(nullptr, url.c_str(), path.c_str(), 0, NULL);
    }

    void InitializeClientFolders() {
        Utils::downloadFile("https://raw.githubusercontent.com/xNotTozic/Shadow-Assets/main/Title.png", Utils::getResourcePath() + "\\LolNigger.png");
    }
#pragma endregion

#pragma region Utils

    static inline bool leftClick, leftDown, rightClick, rightDown, middleClick, middleDown;

    static void onMouseClick(int key, bool isDown)
    {
        switch (key)
        {
        case 1:
            leftClick = isDown;
            leftDown = isDown ? true : leftDown;
            break;
        case 2:
            rightClick = isDown;
            rightDown = isDown ? true : rightDown;
            break;
        case 3:
            middleClick = isDown;
            middleDown = isDown ? true : middleDown;
            break;
        }
    }

#pragma endregion
}