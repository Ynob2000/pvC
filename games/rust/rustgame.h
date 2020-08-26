#pragma once
#include "game.h"
#include "vmath.h"
#include <set>

#define gom 0x17A6AD8
#define networkable 0x28FFD18

static std::string to_string(const std::wstring& wstr)
{
	std::wstring_convert< std::codecvt_utf8<wchar_t>, wchar_t > converter;
	return converter.to_bytes(wstr);
}

enum entity_type
{
	BasePlayer,
	StorageContainer,
	SupplyDrop,
	VendingMachineMapMarker,
	ExcavatorArm,
	LightListener,
	BaseVehicleSeat,
	Boar,
	Bear,
	Deer,
	Chicken,
	Wolf,
	Horse,
	HotAirBalloon,
	MiniCopter,
	DroppedItemContainer,
	OreResourceEntity,
	ResourceEntity,
	StashContainer,
	DroppedItem,
	CollectibleEntity,
	BaseMelee,
	BaseProjectile,
	TreeEntity,
	OreHotSpot,
	LootContainer,
	FreeableLootContainer,
	Recycler,
	JunkPileWater,
	Keycard,
	HackableLockedCrate,
	ItemPickup,
	Undefined,
	WorldItem,
	BradleyAPC,
	PlayerCorpse,
	BaseHelicopter,
	Scientist,
	GunTrap,
	FlameTurret,
	Landmine,
	BearTrap,
	AutoTurret,
	Barricade,
	RidableHorse
};

class RustGame : public Game
{
public:
    RustGame(WinContext *ctx, WinProcess *InProcess, uint64_t InModuleBase)
    {
        ProcessContext = ctx;
        GameProcess = InProcess;
        ModuleBase = InModuleBase;
        unity_player = 0;
        camera_instance = 0;
        base_networkable = 0;
        local_player = 0;
        local_pos_component = 0;
    }

    uintptr_t unity_player, camera_instance;
	uintptr_t base_networkable, local_player, local_pos_component;

	uintptr_t buffer_list = NULL, object_list = NULL;

	bool get_networkable()
	{
		base_networkable = GameProcess->Read<uintptr_t>(ModuleBase + networkable);
		return base_networkable ? true : false;
	}

	bool get_buffer_list()
	{
		auto unk1 = GameProcess->Read<uintptr_t>(base_networkable + 0xB8);
		if (!unk1)
			return false;

		auto client_entities = GameProcess->Read<uintptr_t>(unk1);
		auto entity_realm = GameProcess->Read<uintptr_t>(client_entities + 0x10);
		buffer_list = GameProcess->Read<uintptr_t>(entity_realm + 0x28);
		return buffer_list ? true : false;
	}

	bool get_object_list()
	{
		object_list = GameProcess->Read<uintptr_t>(buffer_list + 0x18);
		return object_list ? true : false;
	}

	int get_object_list_size()
	{
		return GameProcess->Read<int>(buffer_list + 0x10);
	}

	uintptr_t get_base_player(uintptr_t object)
	{
		auto object_unk = GameProcess->Read<uintptr_t>(object + 0x18);
		if (!object_unk)
			return {};

		return GameProcess->Read<uintptr_t>(object_unk + 0x28);
	}

	std::string get_display_name(uintptr_t base_player)
	{
	    char buffer[14];
        GameProcess->Read(base_player + 0x638, &buffer, 14);

		return "TOCARD";
	}

	std::string get_class_name(uintptr_t object)
	{
		auto object_unk = GameProcess->Read<uintptr_t>(object);
		if (!object_unk)
			return {};

		char buffer[13];
		GameProcess->Read(GameProcess->Read<uintptr_t>(object_unk + 0x10), &buffer, 13);
		return buffer;
	}

	uintptr_t get_object_pos_component(uintptr_t entity)
	{
		auto player_visual = GameProcess->Read<uintptr_t>(entity + 0x8);
		if (!player_visual)
			return NULL;

		return GameProcess->Read<uintptr_t>(player_visual + 0x38);
	}	
	
	Vector3f get_object_pos(uintptr_t entity)
	{
		auto player_visual = GameProcess->Read<uintptr_t>(entity + 0x8);
		if (!player_visual)
			return { -1, -1, -1 };

		auto visual_state = GameProcess->Read<uintptr_t>(player_visual + 0x38);
		if (!visual_state)
			return { -1, -1, -1 };

		return GameProcess->Read<Vector3f>(visual_state + 0x90);
	}

	Matrix4f get_view_matrix()
	{
		if (!camera_instance)
		{
			auto gom_ = GameProcess->Read<uintptr_t>(unity_player + gom);
			if (!gom_)
				return {};

			auto tagged_objects = GameProcess->Read<uintptr_t>(gom_ + 0x8);
			if (!tagged_objects)
				return {};

			auto game_object = GameProcess->Read<uintptr_t>(tagged_objects + 0x10);
			if (!tagged_objects)
				return {};

			auto object_class = GameProcess->Read<uintptr_t>(game_object + 0x30);
			if (!tagged_objects)
				return {};

			camera_instance = GameProcess->Read<uintptr_t>(object_class + 0x18);
		}

		if (camera_instance)
			return GameProcess->Read<Matrix4f>(camera_instance + 0xDC);
		else
			return {};
	}

	bool world_to_screen(const Vector3f& entity_pos, Vector2f& screen_pos)
	{
		auto CameraMatrix = get_view_matrix();
	    CameraMatrix = CameraMatrix.transpose();

	    const Vector3f translation = {CameraMatrix.at(3, 0), CameraMatrix.at(3, 1), CameraMatrix.at(3, 2)};
	    const Vector3f up = {CameraMatrix.at(1, 0), CameraMatrix.at(1, 1), CameraMatrix.at(1, 2)};
	    const Vector3f right = {CameraMatrix.at(0, 0), CameraMatrix.at(0, 1), CameraMatrix.at(0, 2)};

	    const auto w = translation.dotProduct(entity_pos) + CameraMatrix.at(3,3);

	    if (w < 0.1f)
	        return false;

	    const auto x = right.dotProduct(entity_pos) + CameraMatrix.at(0, 3);
	    const auto y = up.dotProduct(entity_pos) + CameraMatrix.at(1, 3);

	    screen_pos.x = (1920) * (1.f + x / w) - (1920);
	    screen_pos.y = -((1080) * (1.f - y / w) - (1080));

	    return true;
	}


	uintptr_t get_active_item(uintptr_t _local_player)
	{
		return  GameProcess->Read<uintptr_t>(_local_player + 0x530);
	}

	int get_item_id(uintptr_t item)
	{
		return  GameProcess->Read<uintptr_t>(item + 0x28);
	}

	std::wstring read_wstring(uintptr_t String_address, size_t size)
	{
		const auto buffer = std::make_unique<wchar_t[]>(size);
		GameProcess->Read(String_address, buffer.get(), size * 2);
		return std::wstring(buffer.get());
	}

	std::wstring get_item_name(uintptr_t item)
	{
		auto unk = GameProcess->Read<uintptr_t>(item + 0x20);
		unk = GameProcess->Read<uintptr_t>(unk + 0x20);
		return read_wstring(unk + 0x14, 14);
	}

	bool get_recoil_properties(uintptr_t weapon, std::string name)
	{
		auto base_projectile = GameProcess->Read<uintptr_t>(weapon + 0x98);
		if (!base_projectile)
			return false;

		auto recoil_prop = GameProcess->Read<uintptr_t>(base_projectile + 0x2C0);
		if (!recoil_prop)
			return false;

		int yaw_min = GameProcess->Read<float>(recoil_prop + 0x18);
		int yaw_max = GameProcess->Read<float>(recoil_prop + 0x1C);

		int pitch_min = GameProcess->Read<float>(recoil_prop + 0x20);
		int pitch_max = GameProcess->Read<float>(recoil_prop + 0x24);

	}

	uintptr_t get_active_weapon(uintptr_t _local_player)
	{
		if (!_local_player)
			return false;

		auto inventory = GameProcess->Read<uintptr_t>(_local_player + 0x5C8);
		if (!inventory)
			return {};

		auto contianer_belt = GameProcess->Read<uintptr_t>(inventory + 0x28);
		auto contents = GameProcess->Read<uintptr_t>(contianer_belt + 0x38);
		auto size = GameProcess->Read<int>(contents + 0x18);
		contents = GameProcess->Read<uintptr_t>(contents + 0x10);

		try {
			for (int i = 0; i < size; i++)
			{
				static std::vector<std::wstring>recorded{};
				auto item = GameProcess->Read<uintptr_t>(contents + (0x20 + (i * 0x8)));
				if (get_item_id(item) == get_active_item(_local_player))
				{
					static const auto weps = { L"shotgun", L"pistol", L"rifle", L"smg" };
					const auto item_name = get_item_name(item);
					for (auto wep : weps)
					{
						if (item_name.find(wep) != std::string::npos)
						{
							std::string current_weapon = to_string(item_name);

							// check if we've iterated over this weapon already
							try {
								if (std::find(recorded.begin(), recorded.end(), item_name) == recorded.end())
								{
									get_recoil_properties(item, current_weapon);
									recorded.push_back(item_name);
								}
							}
							catch (const std::exception& exc) {
								std::cout << exc.what() << std::endl;
							}

							return item;
						}
					}
				}
			}
		}
		catch (const std::exception& exc) {
			std::cout << exc.what() << std::endl;
		}

		return {};
	}

	bool set_automatic(uintptr_t weapon)
	{
		auto base_projectile = GameProcess->Read<uintptr_t>(weapon + 0x98);
		if (!base_projectile)
			return false;

		GameProcess->Write<bool>(base_projectile + 0x270, true);
		return true;
	}

	bool set_recoil_props(uintptr_t weapon)
	{
		auto base_projectile = GameProcess->Read<uintptr_t>(weapon + 0x98);
		if (!base_projectile)
			return false;

		auto recoil_prop = GameProcess->Read<uintptr_t>(base_projectile + 0x2C0);
		if (!recoil_prop)
			return false;

		GameProcess->Write<float>(recoil_prop + 0x18, 0.f);
		GameProcess->Write<float>(recoil_prop + 0x1C, 0.f);

		GameProcess->Write<float>(recoil_prop + 0x20, 0.f);
		GameProcess->Write<float>(recoil_prop + 0x24, 0.f);
		return true;
	}
};
