// Opaque forward declarations of the internal-cheat framework types the
// `code` file references. Sufficient to get a C++ compiler past the
// initial parse of that snippet for editor tooling — this does NOT link
// and is NOT intended to.
//
// Real definitions of these would live inside a full CS:GO Source-1
// cheat SDK (see the README for context). Nothing here matches the real
// game's engine layout beyond having the right member names.

#pragma once

#include <cstdint>
#include <string>

namespace UserCmdFlags {
    constexpr int IN_JUMP = 1 << 1;
    constexpr int IN_DUCK = 1 << 2;
    constexpr int IN_ATTACK = 1 << 0;
}

struct QAngle {
    float x, y, z;
};

struct Vector {
    float x, y, z;
};

// The Source-1 user command struct — one per client tick, sent to server.
struct CUserCmd {
    int command_number;
    int tick_count;
    QAngle viewangles;
    Vector forwardmove;
    Vector sidemove;
    Vector upmove;
    int buttons;
    unsigned char impulse;
    int weaponselect;
    int weaponsubtype;
    int random_seed;
    short mousedx;
    short mousedy;
    bool hasbeenpredicted;
};

// A weapon entity. Definition minimal by design.
class Weapon {
public:
    int GetItemDefinitionIndex() const;
};

// Local player + world globals.
namespace Globals {
    class Player {
    public:
        Weapon* activeWeapon();
        bool isAlive() const;
        int movetype() const;
    };
    extern Player* localPlayer;
}

// Interface pointers to Source-1 engine subsystems.
namespace Interfaces {
    class IVEngineClient {
    public:
        void GetViewAngles(QAngle& out);
        void SetViewAngles(const QAngle& in);
    };
    class IClientEntityList {
    public:
        void* GetClientEntity(int index);
    };
    extern IVEngineClient* engine;
    extern IClientEntityList* entityList;
}

// Weapon item definition indices — a handful of CS:GO ones for reference.
// Real headers list ~100 of these.
namespace ItemIndex {
    constexpr int WEAPON_REVOLVER = 64;
    constexpr int WEAPON_AWP = 9;
    constexpr int WEAPON_SCAR20 = 38;
    constexpr int WEAPON_G3SG1 = 11;
}

// Config-tree lookup helper — real macro expands to something like
// `Config::Get<int>("Rage>AntiAim>Real>Method")`.
int CONFIGINT(const char* path);
bool CONFIGBOOL(const char* path);
float CONFIGFLOAT(const char* path);

// True when the LBY update timer is currently counting down; the real
// implementation reads it off the local player entity.
bool updatingLby();

// The AntiAim feature class the `code` file is a method of.
namespace Features {
    class AntiAim {
    public:
        void createMove(CUserCmd* cmd);
    };
}
