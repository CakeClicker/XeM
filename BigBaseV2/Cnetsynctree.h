#pragma once
#include "Logger.hpp"
namespace Big
{
    class GenericPool
    {
    public:
        uint64_t poolStartAddress;
        uint8_t* byteArray;
        int  size;
        int  itemSize;

        inline bool isValid(int i)
        {
            assert(i >= 0);
            return mask(i) != 0;
        }

        inline uint64_t getAddress(int i)
        {
            assert(i >= 0);
            return mask(i) & (poolStartAddress + i * itemSize);
        }
    private:
        inline long long mask(int i)
        {
            assert(i >= 0);
            long long num1 = byteArray[i] & 0x80; // check for high bit.
            return ~((num1 | -num1) >> 63);
        }
    };
    class VehiclePool
    {
    public:
        uint64_t* listAddress;
        int  size;
        char _padding2[36];
        uint32_t* bitArray;
        char _padding3[40];
        int  itemCount;

        inline bool isValid(int i)
        {
            assert(i >= 0);
            return (bitArray[i >> 5] >> (i & 0x1F)) & 1;
        }

        inline uint64_t getAddress(int i)
        {
            return listAddress[i];
        }
    };
#pragma pack(push, 1)
    class CPed
    {
    public:
        char m_padding[0x10B8];
        CPlayerInfo* m_playerinfo;
    };
#pragma pack(pop)

    class CPedFactory
    {
    public:
        virtual ~CPedFactory() = default;
        CPed* m_local_ped;
    };

    class CPlayerSyncTree
    {
    public:
        // char pad_0000[8]; //0x0000
         //netObject* player_object; //0x0008
         //char pad_0010[256]; //0x0010
    }; //Size: 0x0110

    enum object_type : uint32_t
    {
        SCAR = 0,
        SBIKE = 1,
        SBOAT = 2,
        SDOOR = 3,
        SHELI = 4,
        SOBJECT = 5,
        SPED = 6,
        SPICKUP = 7,
        SPICKUP_PLACEMENT = 8,
        SPLANE = 9,
        SSUBMARINE = 10,
        SPLAYER = 11,
        STRAILER = 12,
        STRAIN = 13,
    };

    class CPlayerAppearanceDataNode
    {
    public:
        char pad_0000[24]; //0x0000
        CPlayerSyncTree* sync_tree; //0x0018
        char pad_0020[992]; //0x0020
        uint32_t model; //0x0400
        char pad_0404[124]; //0x0404
    }; //Size: 0x0480

    class CNetworkSyncDataULBase
    {
    public:
        virtual ~CNetworkSyncDataULBase() = default;

        inline virtual void m_8() { }
        inline virtual void m_10() { }
        inline virtual void m_18() { }
        inline virtual void m_20() { }
        inline virtual void m_28() { }
        inline virtual void m_30() { }
        inline virtual void m_38() { }
        inline virtual void m_40() { }
        inline virtual void m_48() { }
        inline virtual void m_50() { }
        inline virtual void SetCloningFrequency(int player, int frequency) { }

    public:
        uint8_t pad_10h[49];
        uint8_t ownerId;
        uint8_t nextOwnerId;
        uint8_t isRemote;
        uint8_t wantsToDelete : 1; // netobj+76
        uint8_t unk1 : 1;
        uint8_t shouldNotBeDeleted : 1;
        uint8_t pad_4Dh[3];
        uint8_t pad_50h[32];
        uint32_t creationAckedPlayers; // netobj+112
        uint32_t m64;
        uint32_t m68;
        uint32_t m6C;

    public:
        inline bool IsCreationAckedByPlayer(int index)
        {
            return (creationAckedPlayers & (1 << index)) != 0;
        }
    };

    class CNetSyncTree
    {
    public:
        uint32_t data[12]; //0x0000
        class netSyncTreeNodes* m_sync_tree_node; //0x0030
    }; //Size: 0x0038

    const int SYNC_TREE_DATA = 258;

    class netSyncTreeNodes
    {
    public:
        uint32_t data[SYNC_TREE_DATA];

        inline uint32_t model(uint16_t object_type)
        {
            uint32_t a = model_vehicle();

            switch (object_type)
            {
            case SDOOR: a = model_door(); break;
            case SOBJECT: a = model_object(); break;
            case SPED: a = model_ped(); break;
            case SPICKUP: a = model_pickup(); break;
            case SPLAYER: a = model_player(); break;
            }
            return a;
        }
        inline float* pos(uint16_t object_type)
        {
            float* a = pos_vehicle();

            switch (object_type)
            {
            case SDOOR: a = pos_door(); break;
            case SOBJECT: a = pos_object(); break;
            case SPED: a = pos_ped(); break;
            case SPICKUP: a = pos_pickup(); break;
            case SPLAYER: a = pos_player(); break;
            }
            return a;
        }

        inline uint32_t model_door() { return data[48]; };
        inline uint32_t model_object() { return data[84]; };
        inline uint32_t model_ped() { return data[49]; };
        inline uint32_t model_pickup() { return data[72]; };
        inline uint32_t model_player() { return data[48]; };
        inline uint32_t model_vehicle() { return data[50]; };

        inline float* pos_door() { return (float*)data + 52; };
        inline float* pos_object() { return (float*)data + 204; };
        inline float* pos_ped() { return (float*)data + 224; };
        inline float* pos_pickup() { return (float*)data + 200; };
        inline float* pos_player() { return (float*)data + 196; };
        inline float* pos_vehicle() { return (float*)data + 124; };

        inline uint32_t pickup() { return data[70]; };
        inline uint32_t weapon_component() { return data[74]; };
    };
    class netObject
    {
    public:
        int16_t object_type; //0x0008
        int16_t object_id; //0x000A
        char pad_000C[61]; //0x000C
        int8_t owner_id; //0x0049
        int8_t control_id; //0x004A
        int8_t next_owner_id; //0x004B
        bool is_remote; //0x004C
        bool wants_to_delete; //0x004D
        char pad_004E[1]; //0x004E
        bool should_not_be_deleted; //0x004F
        char pad_0050[32]; //0x0050
        uint32_t players_acked; //0x0070
        char pad_0074[116]; //0x0074

        virtual ~netObject() = 0;

        virtual void m_8() = 0;
        virtual void m_10() = 0;
        virtual void m_18() = 0;
        virtual void* m_20() = 0;
        virtual void m_28() = 0;
        virtual void m_38() = 0;
        virtual void m_40() = 0;
        virtual void m_48() = 0;
        virtual void m_50() = 0;
        virtual void m_58() = 0;
        virtual void m_60() = 0;
        virtual void m_68() = 0;
        virtual void m_70() = 0;
        virtual void m_78() = 0;
        virtual void m_88() = 0;
        virtual void m_90() = 0;
        virtual void m_98() = 0;
        virtual int GetObjectFlags() = 0;
        virtual void m_A8() = 0;
        virtual void m_B0() = 0;
        virtual void m_B8() = 0;
        virtual void m_C0() = 0;
        virtual void m_C8() = 0;
        virtual int GetSyncFrequency() = 0;
        virtual void m_D8() = 0;
        virtual void m_E0() = 0;
        virtual void m_E8() = 0;
        virtual void m_F0() = 0;
        virtual void m_F8() = 0;
        virtual void Update() = 0;
        virtual bool m_108_1604() = 0; // added in 1604
        virtual void m_108() = 0;
        virtual void m_110() = 0;
        virtual void m_118() = 0;
        virtual void m_120() = 0;
        virtual void m_128() = 0;
        virtual void m_130() = 0;
        virtual void m_138() = 0;
        virtual void m_140() = 0;
        virtual void m_148() = 0;
        virtual void m_150() = 0;
        virtual bool m_158(void* player, int type, int* outReason) = 0;
        virtual void m_160() = 0;
        virtual bool m_168(int* outReason) = 0;
        virtual void m_170() = 0;
        virtual void m_178() = 0;
        virtual void m_180() = 0;
        virtual void m_188() = 0;
        virtual void m_190() = 0;
        virtual void m_198() = 0;
        virtual void m_1A0() = 0;
        virtual void m_1A8() = 0;
        virtual void m_1B0() = 0;
        virtual void m_1B8() = 0;
        virtual void m_1C0() = 0;
        virtual void m_1C8() = 0;
        virtual void m_1D0() = 0;
        virtual void m_1D8() = 0;
        virtual void m_1E0() = 0;
        virtual void m_1E8() = 0;
        virtual void m_1F0() = 0;
        virtual void m_1F8() = 0;
        virtual void m_200() = 0;
        virtual void m_208() = 0;
        virtual void m_210() = 0;
        virtual void m_218() = 0;
        virtual void m_220() = 0;
        virtual void m_228() = 0;
        virtual void m_230() = 0;
        virtual void m_238() = 0;
        virtual void m_240() = 0;
        virtual void m_248() = 0;
        virtual void m_250() = 0;
        virtual void m_258() = 0;
        virtual void m_260() = 0;
        virtual void m_268() = 0;
        virtual void m_270() = 0;
        virtual void m_278() = 0;
        virtual void m_280() = 0;
        virtual void m_288() = 0;
        virtual void m_290() = 0;
        virtual void m_298() = 0;
        virtual void m_2A0() = 0;
        virtual void m_2A8() = 0;
        virtual void m_2B0() = 0;
        virtual void m_2B8() = 0;
        virtual void m_2C0() = 0;
        virtual void m_2C8() = 0;
        virtual void m_2D0() = 0;
        virtual void m_2D8() = 0;
        virtual void m_2E0() = 0;
        virtual void m_2E8() = 0;
        virtual void m_2F0() = 0;
        virtual void m_2F8() = 0;
        virtual void m_300() = 0;
        virtual void m_308() = 0;
        virtual void m_310() = 0;
        virtual void m_318() = 0;
        virtual void m_320() = 0;
        virtual void UpdatePendingVisibilityChanges() = 0;
    };

    class datBitBuffer;
    class CNetGamePlayer;

    class netObjectMgrBase
    {
    public:
        virtual ~netObjectMgrBase() = default;

        virtual void Initialize() = 0;
        virtual void Shutdown() = 0;
        virtual void Update(bool) = 0;

        virtual void AddEntity(void*, void*) = 0;
        virtual void UnregisterNetworkObject_(void* unk, bool force) = 0;
        virtual void UnregisterNetworkObject(netObject* object, int reason, bool force1, bool force2) = 0;

        virtual void ChangeOwner(netObject* object, CNetGamePlayer* player, int migrationType) = 0;

        virtual void HandleJoiningPlayer(CNetGamePlayer* player) = 0;
        virtual void HandleLeavingPlayer(CNetGamePlayer* player) = 0;

        virtual void _0x50(CNetGamePlayer* player) = 0;
        virtual void _0x58() = 0;

        virtual void RegisterNetworkObject(netObject* object) = 0;

        virtual void PackCloneCreate(netObject* object, CNetGamePlayer* player, datBitBuffer* buffer) = 0;
        virtual bool PackCloneRemove(netObject* object, CNetGamePlayer* player, bool) = 0;

        virtual void _0x78(netObject* object, void*) = 0;
        virtual void _0x80() = 0;
        virtual void _0x88() = 0;
        virtual const char* _0x90(int) = 0;

        virtual void _0xC8() = 0;
    };

    const DWORD CsyncCrashObjects[] =
    {
        0xceea3f4b, 0x2592b5cf, 0x9cf21e0f, 0xc1ce1183, 0x49863e9c, 0xcd93a7db, 0x82cac433, 0x74f24de,  0x1c725a1,  0x81fb3ff0,
        0x79b41171, 0x781e451d, 0xa5e3d471, 0x6a27feb1, 0x861d914d, 0x8c049d17, 0xffba70aa, 0xe65ec0e4, 0xc3c00861, 0x5f5dd65c,
        0xc07792d4, 0x53cfe80a, 0xd9f4474c, 0xcb2acc8,  0xc6899cde, 0xd14b5ba3, 0x32a9996c, 0x69d4f974, 0xc2e75a21, 0x1075651,
        0xe1aeb708, 0xcbbb6c39, 0x6fa03a5e, 0xcf7a9a9d, 0x34315488, 0x45ef7804, 0xac3de147, 0xcafc1ec3, 0xd971bbae, 0xe764d794,
        0xf51f7309, 0x1e78c9d,  0xa49658fc, 0x4f2526da, 0x576ab4f6, 0xd20b1778, 0x54bc1cd8, 0xce109c5c, 0xe049c0ce, 0x78de93d1,
        0xe5b89d31, 0x5850e7b3, 0x6aed0e4b, 0xc50a4285, 0xb648a502, 0x5e497511, 0x47c14801, 0xfd8bb397, 0xef541728, 0xc2cc99d8,
        0x8fb233a4, 0x24e08e1f, 0x337b2b54, 0x7367d224, 0x919d9255, 0x4484243f, 0x3c91d42d, 0x3353525a, 0xc175f658, 0x762657c6,
        0x94ac15b3, 0x28014a56, 0xe0a8bfc9, 0x3a559c31, 0x5fc8a70,  0x3b545487, 0xb9402f87, 0xCE5FF074, 0x573201B8, 0xAAB71F62,
        0xA8683715, 0x14EC17EA, 0x56E29962, 0xFCFA9E1E, 0x644AC75E, 0x18012A9F, 0xD86B5A95, 0x8BBAB455, 0x2FD800B7, 0x3C831724,
        0x6AF51FAF, 0x471BE4B2, 0x4E8F95A2, 0x8D8AC8B9, 0x1250D7BA, 0x64611296, 0xB11BAB56, 0x06A20728, 0x431D501C
    };



    enum sync_type : uint32_t
    {
        PACK = 0,                   //fake
        CREATE = 1,
        SYNC = 2,
        REMOVE = 3,
        TAKEOVER = 4,
        TIMESTAMP = 5,
        NETWORK = 6,                //fake
        END = 7,
    };

    const std::string name_network_event[] =
    {
        "OBJECT_ID_FREED_EVENT","OBJECT_ID_REQUEST_EVENT","ARRAY_DATA_VERIFY_EVENT","SCRIPT_ARRAY_DATA_VERIFY_EVENT","REQUEST_CONTROL_EVENT",
        "GIVE_CONTROL_EVENT","WEAPON_DAMAGE_EVENT","REQUEST_PICKUP_EVENT","REQUEST_MAP_PICKUP_EVENT","GAME_CLOCK_EVENT","GAME_WEATHER_EVENT",
        "RESPAWN_PLAYER_PED_EVENT","GIVE_WEAPON_EVENT","REMOVE_WEAPON_EVENT","REMOVE_ALL_WEAPONS_EVENT","VEHICLE_COMPONENT_CONTROL_EVENT",
        "FIRE_EVENT","EXPLOSION_EVENT","START_PROJECTILE_EVENT","UPDATE_PROJECTILE_TARGET_EVENT","REMOVE_PROJECTILE_ENTITY_EVENT",
        "BREAK_PROJECTILE_TARGET_LOCK_EVENT","ALTER_WANTED_LEVEL_EVENT","CHANGE_RADIO_STATION_EVENT","RAGDOLL_REQUEST_EVENT","PLAYER_TAUNT_EVENT",
        "PLAYER_CARD_STAT_EVENT","DOOR_BREAK_EVENT","SCRIPTED_GAME_EVENT","REMOTE_SCRIPT_INFO_EVENT","REMOTE_SCRIPT_LEAVE_EVENT",
        "MARK_AS_NO_LONGER_NEEDED_EVENT","CONVERT_TO_SCRIPT_ENTITY_EVENT","SCRIPT_WORLD_STATE_EVENT","CLEAR_AREA_EVENT","CLEAR_RECTANGLE_AREA_EVENT",
        "NETWORK_REQUEST_SYNCED_SCENE_EVENT","NETWORK_START_SYNCED_SCENE_EVENT","NETWORK_STOP_SYNCED_SCENE_EVENT","NETWORK_UPDATE_SYNCED_SCENE_EVENT",
        "INCIDENT_ENTITY_EVENT","GIVE_PED_SCRIPTED_TASK_EVENT","GIVE_PED_SEQUENCE_TASK_EVENT","NETWORK_CLEAR_PED_TASKS_EVENT","NETWORK_START_PED_ARREST_EVENT",
        "NETWORK_START_PED_UNCUFF_EVENT","NETWORK_SOUND_CAR_HORN_EVENT","NETWORK_ENTITY_AREA_STATUS_EVENT","NETWORK_GARAGE_OCCUPIED_STATUS_EVENT",
        "PED_CONVERSATION_LINE_EVENT","SCRIPT_ENTITY_STATE_CHANGE_EVENT","NETWORK_PLAY_SOUND_EVENT","NETWORK_STOP_SOUND_EVENT",
        "NETWORK_PLAY_AIRDEFENSE_FIRE_EVENT","NETWORK_BANK_REQUEST_EVENT","NETWORK_AUDIO_BARK_EVENT","REQUEST_DOOR_EVENT","NETWORK_TRAIN_REPORT_EVENT",
        "NETWORK_TRAIN_REQUEST_EVENT","NETWORK_INCREMENT_STAT_EVENT","MODIFY_VEHICLE_LOCK_WORD_STATE_DATA","MODIFY_PTFX_WORD_STATE_DATA_SCRIPTED_EVOLVE_EVENT",
        "REQUEST_PHONE_EXPLOSION_EVENT","REQUEST_DETACHMENT_EVENT","KICK_VOTES_EVENT","GIVE_PICKUP_REWARDS_EVENT","NETWORK_CRC_HASH_CHECK_EVENT",
        "BLOW_UP_VEHICLE_EVENT","NETWORK_SPECIAL_FIRE_EQUIPPED_WEAPON","NETWORK_RESPONDED_TO_THREAT_EVENT","NETWORK_SHOUT_TARGET_POSITION",
        "VOICE_DRIVEN_MOUTH_MOVEMENT_FINISHED_EVENT","PICKUP_DESTROYED_EVENT","UPDATE_PLAYER_SCARS_EVENT","NETWORK_CHECK_EXE_SIZE_EVENT","NETWORK_PTFX_EVENT",
        "NETWORK_PED_SEEN_DEAD_PED_EVENT","REMOVE_STICKY_BOMB_EVENT","NETWORK_CHECK_CODE_CRCS_EVENT","INFORM_SILENCED_GUNSHOT_EVENT","PED_PLAY_PAIN_EVENT",
        "CACHE_PLAYER_HEAD_BLEND_DATA_EVENT","REMOVE_PED_FROM_PEDGROUP_EVENT","REPORT_MYSELF_EVENT","REPORT_CASH_SPAWN_EVENT",
        "ACTIVATE_VEHICLE_SPECIAL_ABILITY_EVENT","BLOCK_WEAPON_SELECTION","NETWORK_CHECK_CATALOG_CRC"
    };

    const std::string name_object_type[] =
    {
        "CAR","BIKE","BOAT","DOOR","HELI","OBJECT","PED","PICKUP","PICKUP_PLACEMENT","PLANE","SUBMARINE","PLAYER","TRAILER","TRAIN",
    };

    const std::string name_sync_type[] =
    {
        "PACK","CREATE","SYNC","REMOVE","TAKEOVER","TIMESTAMP","NETWORK","END",
    };



}
