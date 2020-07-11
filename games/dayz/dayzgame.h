#pragma once
#include "game.h"
#include "Engine.h"
#include "vmath.h"
#include <set>

class DayzGame : public Game
{
public:
    float CameraSpeed = 1.0f;

    DayzGame(WinContext *ctx, WinProcess *InProcess, uint64_t InModuleBase)
    {
        ProcessContext = ctx;
        GameProcess = InProcess;
        ModuleBase = InModuleBase;
    }
    uint64_t GetWorld(){
        return GameProcess->Read<uint64_t>(
                ModuleBase + off_world);
    };
    uint64_t GetNetworkManager() {
        return ModuleBase + off_networkmanager;
    };
    uint64_t GetNetworkClient(){
        return GameProcess->Read<uint64_t>(
                DayzGame::GetNetworkManager() + off_networkmanager_networkclient);
    };
    uint64_t GetNetworkClientScoreBoard(){
        return GameProcess->Read<uint64_t>(
                DayzGame::GetNetworkClient() + off_networkclient_scoreboard);
    };
    string GetNetworkClientServerName(){
        return DayzGame::ReadArmaString(GameProcess->Read<uint64_t>(
                DayzGame::GetNetworkClient() + off_networkclient_servername));
    };
    uint32_t GetCountOfPlayers(){
        return GameProcess->Read<uint32_t>(
                DayzGame::GetNetworkClient() + off_networkclient_scoreboard + off_sortedobject_entity);
    };
    string GetPlayerName(uint64_t Entity){
        uint64_t playerIdentity = GetIdentity(GetNetworkId(Entity));
        return GetPlayerIdentityPlayerName(playerIdentity);
    };
    string GetItemName(uint64_t Entity){
        return DayzGame::ReadArmaString(ReadPtrChain(GameProcess, Entity, {0xE0, 0x450}));
    };
    int GetPlayerIdentityNetworkId(uint64_t Identity){
        return GameProcess->Read<uint64_t>(Identity + off_playeridentity_networkid);
    };
    string GetPlayerIdentityPlayerName(uint64_t Identity){
        return DayzGame::ReadArmaString(GameProcess->Read<uint64_t>(Identity + off_playeridentity_playername));
    };
    uint64_t GetIdentity(int networkId){
        uint32_t nbPlayers = DayzGame::GetCountOfPlayers();
        for (int i = 0; i < nbPlayers; ++i) {
            uint64_t PlayerIdentity = DayzGame::GetNetworkClientScoreBoard() + i * off_networkclient_playeridentity_size;
            int tempIdentity = DayzGame::GetPlayerIdentityNetworkId(PlayerIdentity);

            if (tempIdentity == networkId)
                return DayzGame::GetNetworkClientScoreBoard() + i * off_networkclient_playeridentity_size;
        }
    };
    uint64_t GetCamera(){
        return GameProcess->Read<uint64_t>(
                DayzGame::GetWorld() + off_world_camera);
    };
    uint64_t GetCameraEntry(){
        return GameProcess->Read<uint64_t>(
                DayzGame::GetLocalPlayer() + off_entity_cameraentry);
    };
    uint64_t GetCameraOn(){
        return GameProcess->Read<uint64_t>(
                DayzGame::GetWorld() + off_world_cameraon);
    };
    uint64_t GetLocalPlayer(){
        return GameProcess->Read<uint64_t>(
                DayzGame::GetCameraOn() + off_sortedobject_entity);
    };
    uint64_t GetLocalPlayerVisualState(){
        return GameProcess->Read<uint64_t>(
                DayzGame::GetLocalPlayer() + off_entity_futurevisualstate);
    };
    uint64_t GetInventory(uint64_t Entity){
        return GameProcess->Read<uint64_t>(Entity + off_entity_inventory);
    };
    string GetItemInHands(uint64_t Entity){
        return DayzGame::ReadArmaString(GameProcess->Read<uint64_t>(
                GameProcess->Read<uint64_t>(GameProcess->Read<uint64_t>(
                        DayzGame::GetInventory(Entity) + off_inventory_iteminhands)
                                            + off_entity_rendererentitytype) + off_entitytype_cleanname));
    };
    uint64_t GetEntity(uint64_t PlayerList, uint64_t SelectedPlayer){
        return GameProcess->Read<uint64_t>(PlayerList + SelectedPlayer * off_sortedobject_entity);
    };
    int GetNetworkId(uint64_t Entity){
        return GameProcess->Read<int>(Entity + off_entity_networkid);
    };
    string GetEntityTypeName(uint64_t Entity){
        string name = DayzGame::ReadArmaString(GameProcess->Read<uint64_t>(
                GameProcess->Read<uint64_t>(
                        Entity + off_entity_rendererentitytype)
                + off_entitytype_configname));
        return name;
    };
    uint64_t GetType(uint64_t Entity){
        return GameProcess->Read<uint64_t>(
                GameProcess->Read<uint64_t>(
                        GameProcess->Read<uint64_t>(
                                Entity + off_entity_rendererentitytype)
                        + off_entitytype_configname) + 0x18);
    };
    string GetTypeName(uint64_t Entity){
        return DayzGame::ReadArmaString(GameProcess->Read<uint64_t>(
                GameProcess->Read<uint64_t>(Entity + off_entity_rendererentitytype) + off_entitytype_typename));
    };
    string GetModelName(uint64_t Entity){
        return DayzGame::ReadArmaString(GameProcess->Read<uint64_t>(
                GameProcess->Read<uint64_t>(Entity + off_entity_rendererentitytype) + off_entitytype_modelname));
    };
    Vector3f GetCoordinate(uint64_t Entity){
        if (Entity == DayzGame::GetLocalPlayer()) {
            return GameProcess->Read<Vector3f>(
                    DayzGame::GetLocalPlayerVisualState() + off_visualstate_position);
        }
        else {
            return  GameProcess->Read<Vector3f>(
                    GameProcess->Read<uint64_t>(
                            Entity + off_entity_renderervisualstate) + off_visualstate_position);
        }
    };
    Vector3f GetHeadCoordinate(uint64_t Entity){
        if (Entity == DayzGame::GetLocalPlayer()) {
            return GameProcess->Read<Vector3f>(
                    DayzGame::GetLocalPlayerVisualState() + off_visualstate_position);
        }
        else {
            return  GameProcess->Read<Vector3f>(
                    GameProcess->Read<uint64_t>(
                            Entity + off_entity_renderervisualstate) + off_visualstate_headposition);
        }
    };
    float GetDistanceToMe(const Vector3f& Entity){
        return GetDistanceFromTo(Entity, DayzGame::GetCoordinate(DayzGame::GetLocalPlayer()));
    };
    static float GetDistanceFromTo(const Vector3f& From, const Vector3f& To){
        return (To - From).length();
    };
    uint64_t GetItemTable(int index = 0){
        return GameProcess->Read<uint64_t>(
                DayzGame::GetWorld() + off_world_itemtable + (index * 0x20));
    };
    uint64_t GetItemTableSize(int index = 0){
        return GameProcess->Read<uint64_t>(
                DayzGame::GetWorld() + off_world_itemtable + (index * 0x20) + off_length);
    };
    uint64_t NearEntityTable(){
        return GameProcess->Read<uint64_t>(
                DayzGame::GetWorld() + off_world_nearanimaltable);
    };
    int NearEntityTableSize(){
        return GameProcess->Read<int>(
                DayzGame::GetWorld() + off_world_nearanimaltable + off_length);
    };
    uint64_t FarEntityTable(){
        return GameProcess->Read<uint64_t>(
                DayzGame::GetWorld() + off_world_faranimaltable);
    };
    int FarEntityTableSize(){
        return GameProcess->Read<int>(
                DayzGame::GetWorld() + off_world_faranimaltable + off_length);
    };
    uint64_t BulletTable(){
        return GameProcess->Read<uint64_t>(
                DayzGame::GetWorld() + off_world_bullettable);
    };
    uint64_t BulletTableSize(){
        return GameProcess->Read<size_t>(
                DayzGame::BulletTable() + off_length);
    };
    vector<uint64_t> GetAllItems(){
        vector<uint64_t> arrayList;
        for (int i = 0; i < 12; ++i) {
            uint32_t tableSize = DayzGame::GetItemTableSize(i);

            for (uint64_t ItemId = 0; ItemId < 3 * tableSize; ++ItemId) {
                uint64_t entity = DayzGame::GetEntity(DayzGame::GetItemTable(i), ItemId);
                if (entity!=0)
                    arrayList.push_back(entity);
            }
        }
        return arrayList;
    };
    vector<uint64_t> GetNearEntities(){
        vector<uint64_t> arrayList;

        for (uint64_t playerId = 0; playerId < DayzGame::NearEntityTableSize(); ++playerId) {
            arrayList.push_back(DayzGame::GetEntity(DayzGame::NearEntityTable(), playerId));
        }
        return arrayList;
    };
    vector<uint64_t> GetFarEntities(){
        vector<uint64_t> arrayList;

        for (uint64_t playerId = NULL; playerId < DayzGame::FarEntityTableSize(); ++playerId) {
            arrayList.push_back(DayzGame::GetEntity(DayzGame::FarEntityTable(), playerId));
        }
        return arrayList;
    };
    vector<uint64_t> GetAllEntities(){
        vector<uint64_t> arrayList;
        int nearEntitySize = DayzGame::NearEntityTableSize();

        for (int playerId = NULL; playerId < nearEntitySize; ++playerId) {
            if (playerId != 0) { // check if entity != localplayer
                uint64_t targetentity = DayzGame::GetEntity(DayzGame::NearEntityTable(), playerId);

                if (DayzGame::GetEntityTypeName(targetentity) == "dayzplayer") {
                    arrayList.push_back(targetentity);
                }
            }
        }

        for (int playerId = NULL; playerId < DayzGame::FarEntityTableSize(); ++playerId) {
            uint64_t targetentity = DayzGame::GetEntity(DayzGame::FarEntityTable(), playerId);

            if (DayzGame::GetEntityTypeName(targetentity) == "dayzplayer") {
                arrayList.push_back(targetentity);
            }
        }

        return arrayList;
    };
    vector<uint64_t> GetAllBullets()
    {
        vector<uint64_t> arrayList;

        for (uint64_t playerId = NULL; playerId < DayzGame::BulletTableSize(); ++playerId) {
            arrayList.push_back(DayzGame::GetEntity(DayzGame::BulletTable(), playerId));
        }

        return arrayList;
    };
    bool WorldToScreen(const Vector3f& Position, Vector2f& output){
        if (!DayzGame::GetCamera())
            return false;

        Vector3f temp = Position - DayzGame::GetInvertedViewTranslation();

        float x = temp.dotProduct(DayzGame::GetInvertedViewRight());
        float y = temp.dotProduct(DayzGame::GetInvertedViewUp());
        float z = temp.dotProduct(DayzGame::GetInvertedViewForward());
        if (z < 0.1f)
            return false;
        Vector2f res = Vector2f(
                DayzGame::GetViewportSize().x * (1 + (x / DayzGame::GetProjectionD1().x / z)),
                DayzGame::GetViewportSize().y * (1 - (y / DayzGame::GetProjectionD2().y / z)));
        output.x = res.x;
        output.y = res.y;
        return true;
    };
    void MovCameraUp(){
        if (DayzGame::CameraSpeed <= 0) { DayzGame::CameraSpeed = 1.0f; }

        auto updown = GameProcess->Read<float>(
                DayzGame::GetCameraEntry() + off_cameraentry_movupdown);

        GameProcess->Write<float>(
                DayzGame::GetCameraEntry() + off_cameraentry_movupdown, updown + DayzGame::CameraSpeed);
    };
    void MovCameraDown(){
        if (DayzGame::CameraSpeed <= 0) { DayzGame::CameraSpeed = 1.0f; }

        auto updown = GameProcess->Read<float>(
                DayzGame::GetCameraEntry() + off_cameraentry_movupdown);

        GameProcess->Write<float>(
                DayzGame::GetCameraEntry() + off_cameraentry_movupdown, updown - DayzGame::CameraSpeed);
    };
    void MovCameraRight(){
        if (DayzGame::CameraSpeed <= 0) { DayzGame::CameraSpeed = 1.0f; }

        auto updown = GameProcess->Read<float>(
                DayzGame::GetCameraEntry() + off_cameraentry_movleftright);

        GameProcess->Write<float>(
                DayzGame::GetCameraEntry() + off_cameraentry_movleftright, updown + DayzGame::CameraSpeed);
    };
    void MovCameraLeft(){
        if (DayzGame::CameraSpeed <= 0) { DayzGame::CameraSpeed = 1.0f; }

        auto updown = GameProcess->Read<float>(
                DayzGame::GetCameraEntry() + off_cameraentry_movleftright);

        GameProcess->Write<float>(
                DayzGame::GetCameraEntry() + off_cameraentry_movleftright, updown - DayzGame::CameraSpeed);
    };
    void MovCameraForward(){
        if (DayzGame::CameraSpeed <= 0) { DayzGame::CameraSpeed = 1.0f; }

        auto updown = GameProcess->Read<float>(
                DayzGame::GetCameraEntry() + off_cameraentry_movforwardbackward);

        GameProcess->Write<float>(
                DayzGame::GetCameraEntry() + off_cameraentry_movforwardbackward, updown - DayzGame::CameraSpeed);
    };
    void MovCameraBackward(){
        if (DayzGame::CameraSpeed <= 0) { DayzGame::CameraSpeed = 1.0f; }

        auto updown = GameProcess->Read<float>(
                DayzGame::GetCameraEntry() + off_cameraentry_movforwardbackward);

        GameProcess->Write<float>(
                DayzGame::GetCameraEntry() + off_cameraentry_movforwardbackward, updown + DayzGame::CameraSpeed);
    };
    void SetTerrainGrid(float value){
        GameProcess->Write<float>(DayzGame::GetWorld() + off_world_terraingrid, value); //0 - remove 12 - set
    };
    bool SetPosition(uint64_t Entity, Vector3f TargetPosition){
        if (Entity == DayzGame::GetLocalPlayer()) {
            GameProcess->Write<Vector3f>(
                    GameProcess->Read<uint64_t>(
                            Entity + off_entity_futurevisualstate) + off_visualstate_position, TargetPosition);
        }
        else {
            GameProcess->Write<Vector3f>(
                    GameProcess->Read<uint64_t>(
                            Entity + off_entity_renderervisualstate) + off_visualstate_position, TargetPosition);
        }
        return true;
    };
    bool KillBySilentAim(uint64_t Entity){
        for (uint64_t playerId = NULL; playerId < DayzGame::BulletTableSize(); ++playerId) {
            Vector3f WorldPosition = DayzGame::GetCoordinate(Entity);
            DayzGame::SetPosition(DayzGame::GetEntity(DayzGame::BulletTable(), playerId), // tp ur bullet to entity head
                                  Vector3f(WorldPosition.x, WorldPosition.y + 1.0f, WorldPosition.z)); // body pos [+/-] :)
        }
        return true;
    };
    bool SetCameraMovSpeed(float Speed){
        DayzGame::CameraSpeed = Speed;
        return true;
    };
    Vector3f GetInvertedViewRight(){
        return Vector3f(GameProcess->Read<Vector3f>(
                DayzGame::GetCamera() + off_camera_viewright));
    };
    Vector3f GetInvertedViewUp(){
        return Vector3f(GameProcess->Read<Vector3f>(
                DayzGame::GetCamera() + off_camera_viewup));
    };
    Vector3f GetInvertedViewForward(){
        return Vector3f(GameProcess->Read<Vector3f>(
                DayzGame::GetCamera() + off_camera_viewforward));
    };
    Vector3f GetViewportSize(){
        return Vector3f(GameProcess->Read<Vector3f>(
                DayzGame::GetCamera() + off_camera_viewportsize));
    };
    Vector3f GetInvertedViewTranslation(){
        return Vector3f(GameProcess->Read<Vector3f>(
                DayzGame::GetCamera() + off_camera_viewtranslation));
    };
    Vector3f GetProjectionD1(){
        return Vector3f(GameProcess->Read<Vector3f>(
                DayzGame::GetCamera() + off_camera_projection_d1));
    };
    Vector3f GetProjectionD2(){
        return Vector3f(GameProcess->Read<Vector3f>(
                DayzGame::GetCamera() + off_camera_projection_d2));
    };
    string GetDirection(){
        Vector3f viewAnglesRadians, viewAngle;
        viewAnglesRadians.x = atan2(
                DayzGame::GetInvertedViewRight().z, DayzGame::GetInvertedViewRight().x);
        viewAngle.x = DayzGame::RadiansToDegrees(viewAnglesRadians.x);
        viewAnglesRadians.y = atan2(
                DayzGame::GetInvertedViewForward().y, DayzGame::GetInvertedViewUp().y);
        viewAngle.y = DayzGame::RadiansToDegrees(viewAnglesRadians.y);

        if (viewAngle.x >= 0 && viewAngle.x <= 90) {
            return "NW";
        }
        else if (viewAngle.x <= 0 && viewAngle.x >= -90) {
            return "NE";
        }
        else if (viewAngle.x >= 90 && viewAngle.x <= 180) {
            return "SW";
        }
        else if (viewAngle.x <= -90 && viewAngle.x >= -180) {
            return "SE";
        }
    };
    float RadiansToDegrees(float radians){
        float degrees;
        degrees = ((radians * 180) / 3.14159265358979323846f);
        return degrees;
    };

    std::set<std::string> food = {
            "Canned Bacon",
            "Canned",
            "Baked Beans",
            "Canned Peaches",
            "Canned Sardines",
            "Canned Spaghetti",
            "Canned Tuna",
            "Strawberry Jam",
            "Cereals",
            "Powdered Milk",
            "Rice",
            "Apple",
            "Pear",
            "Plum",
            "Tomato",
            "Pepper",
            "Potato",
            "Zucchini",
            "Field Mushroom",
            "Nota Cola",
            "Spite Lemonade",
            "Pipsi Cola",
            "Mad Monk Kvass"
    };

    std::set<std::string> gear = {
            "Night Vision Goggles",
            "Tactical Helmet",
            "PU Scope",
            "ATOG Scope",
            "Plate Carrier",
            "Pouches",
            "Attachable Pouches",
            "Holster",
            "Gun Holster"
    };

    std::set<std::string> items = {
            "IJ-70",
            ".380 ACP Rounds",
            "Boxed .380 ACP Rounds",
            "12rd IJ-70 Mag",
            "CR-75",
            "9x19mm Rounds",
            "Box of 9×19mm Rounds",
            "15rd CR-75 Mag",
            "Mlock-91",
            "15rd Mlock-91 Mag",
            "FX-45", ".45 ACP Rounds", "Box of 25 Rounds", "15rd FX-45 Mag",
            "Kolt 1911", "Engraved Kolt 1911", "7rd Kolt 1911 Mag",
            "MK II ",  ".22 LR Rounds", "Box of .22 Rounds", "10rd MK II Mag",
            "BK-133", "Boxed 12ga 00 Buckshots", "12ga 00 Buckshots",
            "BK 43", "Sawed-Off BK-43 Shotgun", "Boxed 12ga Rifled Slugs", "12ga Rifled Slugs",
            "Vaiga", "5rd Vaiga Mag", "8rd Vaiga Mag", "20rd Drum Vaiga Mag",
            "CR-61 Skorpion", "20rd CR-61 Mag",
            "SG5-K", "30rd SG5-K Mag",
            "USG-45", "25rd USG-45 Mag",
            "Sporter 22", "10rd Sporter 22 Mag", "30rd Sporter 22 Mag",
            "BK-18", "7.62x39mm Rounds",  "Box of 7.62x39mm Rounds", "7.62x39mm Tracer Rounds", "Boxed 7.62x39mm Tracer Rounds",
            "Sawed-off BK-18",
            "CR-527", "5rd CR-527 Mag",
            "SK 59/66",
            "Mosin 91/30", "Mosin 91/30 Geen", "Mosin 91/30 Black", "Mosin 91/30 Camo", "7.62x54mmR Rounds", "Box of 7.62x54mmR Rounds", "7.62x54mmR Tracer Rounds", "Boxed 7.62x54mmR Tracer Rounds",
            "Sawed-off Mosin 91/30",
            "M70 Tundra", "M70 Tundra Black", "M70 Tundra Green", ".308 WIN Rounds", "308 WIN Tracer Rounds", "Box of .308 WIN Rounds", "Boxed .308 WIN Tracer Rounds",
            "M4-A1", "5.56×45mm", "Box of 5.56mm Rounds", "5.56x45mm Tracer Rounds", "Boxed 5.56x45mm Tracer Rounds",  "30rd Standardized Mag", "30rd Coupled Standardized Mag", "40rd Mag-C Magazine", "30rd Mag-C Magazine", "20rd Mag-C Magazine",
            "KA-M", "30rd KA-M Mag", "75rd KA-M Drum Mag", "30rd KA-M Polymer Mag",
            "KA-101", "30rd KA-101 Mag", "30Rnd AK101 Magazine (Green)",
            "KA-74", "30rd KA-74 Mag", "Box of 5.45×39mm Rounds", "5.45×39mm Rounds", "5.45x39mm Tracer Rounds", "Boxed 5.45x39mm Tracer Rounds",
            "KAS-74U",
            "LAR", "20rd LAR Mag",
            "VSD", "10rd VSD Mag",
            "VSS", "10rd VSS Mag"
    };


private:
    string ReadArmaString(uint64_t address)
    {
        int length = GameProcess->Read<uint64_t>(address + off_length);
        char buffer[100];
        GameProcess->Read(address + off_text, buffer, length);
        return buffer;
    };
};
