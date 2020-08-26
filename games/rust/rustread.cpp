#include "rustread.h"


float MAX_RENDER_DISTANCE = 500.f;


void toWinPos(Vector2f& screenPos, float width, float height)
{
    screenPos.x = (screenPos.x + width) / 2;
    screenPos.y = (-screenPos.y + height) / 2;
}


bool RustReader::InGame()
{
	FILE *out = stdout;
    for (auto &i : game->ProcessContext->processList)
    {
        if (!strcasecmp("RustClient.exe", i.proc.name))
        {
            fprintf(out, "Game:\tLooping process %lx:%s\n", i.proc.dirBase, i.proc.name);

            for (auto &o : i.modules)
            {
                fprintf(out, "\t%.8lx\t%.8lx\t%lx\t%s\n", o.info.baseAddress, o.info.entryPoint, o.info.sizeOfModule, o.info.name);
                if (!strcmp("UnityPlayer.dll", o.info.name))
                {
                    game->unity_player = o.info.baseAddress;
                }
            }
        }
    }
	if (!game->get_networkable())
		return false;

	if (!game->get_buffer_list())
		return false;

    return game->get_object_list();

}

void RustReader::GetPlayers(ESPObjectArray *a, float width, float height, bool use_aimbot)
{
    WinProcess* GameProcess = game->GameProcess;
    clearArray(a);
	Vector2f ScreenPos;
	if (game->local_player) {
//        auto base_movement = GameProcess->Read<uintptr_t>(game->local_player + 0x5E8);
//        if (base_movement) {
//            GameProcess->Write<float>(base_movement + 0xAC, 0.f);
//            GameProcess->Write<float>(base_movement + 0xB0, 0.f);
//        }
    }
    for (auto i = 0; i < game->get_object_list_size(); i++)
	{
		const auto element = GameProcess->Read<uintptr_t>(game->object_list + (0x20 + (i * 0x8)));
		const auto element_name = game->get_class_name(element);

		const auto base_mono_object = GameProcess->Read<uintptr_t>(element + 0x10);
		if (!base_mono_object)
			continue;

		auto object = GameProcess->Read<uintptr_t>(base_mono_object + 0x30);
		if (!object)
			continue;

		object = GameProcess->Read<uintptr_t>(object + 0x30);
		if (!object)
			continue;

		ESPObject Object;
		bool render = false;
		float to_distance;
		if (element_name.find("BasePlayer") != std::string::npos)
		{
			const auto base_player = game->get_base_player(object);
			if (!base_player)
				continue;

			auto pos = GameProcess->Read<Vector3f>(game->get_object_pos_component(object) + 0x90);
			if (pos == Vector3f{ -1, -1, -1 }) // if it returns invalid positon, means the object was null, remove from list 
			{
				continue;
			}

			auto local_pos = GameProcess->Read<Vector3f>(game->local_pos_component + 0x90);

			if (!i && !game->local_player) // assign local player
			{
				game->local_player = object;
				game->local_pos_component = game->get_object_pos_component(object);
				std::cout << "[-] Local player: " << std::hex << game->local_player << std::endl;
			}

			to_distance = sqrtf(powf(local_pos.x - pos.x, 2) + powf(local_pos.y - pos.y, 2) + powf(local_pos.z - pos.z, 2) );

			if (to_distance > 500.f)
				continue;

			if (!game->world_to_screen(pos, ScreenPos))
				continue;

			render = true;
            Object.r = 255/255.f ;
            Object.g = 0 / 255.f;
            Object.b = 0 / 255.f;
            //strcpy(Object.pName, (game->get_display_name(base_player)).c_str());
		}
	
		else if (element_name.find("Scientist") != std::string::npos)
		{
			auto pos = GameProcess->Read<Vector3f>(game->get_object_pos_component(object) + 0x90);
			if (pos == Vector3f{ -1, -1, -1 }) // if it returns invalid positon, means the object was null, remove from list 
			{
				continue;
			}

			auto local_pos = GameProcess->Read<Vector3f>(game->local_pos_component + 0x90);

			to_distance = sqrtf(powf(local_pos.x - pos.x, 2) + powf(local_pos.y - pos.y, 2) + powf(local_pos.z - pos.z, 2) );

			if (to_distance > 500.f)
				continue;

			if (!game->world_to_screen(pos, ScreenPos))
				continue;

			render = true;
            Object.r = 0/255.f ;
            Object.g = 0 / 255.f;
            Object.b = 255 / 255.f;

		}			
		else if (element_name.find("StashContai") != std::string::npos)
		{
			auto pos = GameProcess->Read<Vector3f>(game->get_object_pos_component(object) + 0x90);
			if (pos == Vector3f{ -1, -1, -1 }) // if it returns invalid positon, means the object was null, remove from list 
			{
				continue;
			}

			auto local_pos = GameProcess->Read<Vector3f>(game->local_pos_component + 0x90);

			to_distance = sqrtf(powf(local_pos.x - pos.x, 2) + powf(local_pos.y - pos.y, 2) + powf(local_pos.z - pos.z, 2) );

			if (to_distance > 500.f)
				continue;

			if (!game->world_to_screen(pos, ScreenPos))
				continue;

			render = true;
            Object.r = 255/255.f ;
            Object.g = 255 / 255.f;
            Object.b = 0 / 255.f;
		}
		

		if (render)
		{
			toWinPos(ScreenPos, width, height);
			Object.x = ScreenPos.x;
			Object.y = ScreenPos.y;
	        Object.xHead = ScreenPos.x;
	        Object.yHead = ScreenPos.y;
	        Object.inGameDistance = to_distance;
	        Object.drawBones = false;

	        insertArray(a, Object);
		}

	}
}


void RustReader::GetLoot(ESPObjectArray *a, float width, float height)
{
    return;
}
