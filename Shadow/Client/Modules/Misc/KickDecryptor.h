#pragma once

class KickDecryptor : public Module
{
public:
    KickDecryptor(int keybind = Keys::NONE, bool enabled = false) :
        Module("KickDecryptor", "Misc", "Decrypts the hive error", keybind, enabled)
    {

    }
};