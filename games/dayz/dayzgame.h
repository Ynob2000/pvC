#pragma once
#include "game.h"
#include "Engine.h"
#include "vmath.h"

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
        return GameProcess->Read<uint64_t>(
                ModuleBase + off_networkmanager);
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
    uint64_t GetCountOfPlayers(){
        return GameProcess->Read<uint64_t>(
                DayzGame::GetNetworkClientScoreBoard() + off_sortedobject_entity);
    };
    string GetPlayerName(uint64_t Entity){
        return GetPlayerIdentityPlayerName(
                DayzGame::GetNetworkId(Entity));
    };
    string GetItemName(uint64_t Entity){
        return GetItemName(
                DayzGame::GetNetworkId(Entity));
    }; // my own
    uint64_t GetPlayerIdentityNetworkId(uint64_t Identity){
        return GameProcess->Read<uint64_t>(Identity + off_playeridentity_networkid);
    };
    string GetPlayerIdentityPlayerName(uint64_t Identity){
        return DayzGame::ReadArmaString(GameProcess->Read<uint64_t>(Identity + off_playeridentity_playername));
    };
    uint64_t GetIdentity(uint64_t networkId){
        for (uint64_t i = 0; i < DayzGame::GetCountOfPlayers(); i++) {
            uint64_t tempIdentity = DayzGame::GetPlayerIdentityNetworkId(
                    GameProcess->Read<uint64_t>(
                            DayzGame::GetNetworkClientScoreBoard() + i * off_networkclient_playeridentity_size));

            if (tempIdentity == networkId)
                return  tempIdentity;
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
    uint64_t GetNetworkId(uint64_t Entity){
        return GameProcess->Read<uint64_t>(Entity + off_entity_networkid);
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
    float GetDistanceToMe(Vector3f Entity){
        return GetDistanceFromTo(Entity, DayzGame::GetCoordinate(DayzGame::GetLocalPlayer()));
    };
    float GetDistanceFromTo(Vector3f From, Vector3f To){
        return (To - From).length();
    };
    uint64_t GetItemTable(){
        return GameProcess->Read<uint64_t>(
                DayzGame::GetWorld() + off_world_itemtable);
    };
    uint64_t GetItemTableSize(){
        return sizeof(DayzGame::GetItemTable()) * 3;
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

        for (uint64_t ItemId = NULL; ItemId < DayzGame::GetItemTableSize(); ++ItemId) {
            arrayList.push_back(DayzGame::GetEntity(DayzGame::GetItemTable(), ItemId));
        }

        return arrayList;
    };
    vector<uint64_t> GetNearEntityes(){
        vector<uint64_t> arrayList;

        for (uint64_t playerId = NULL; playerId < DayzGame::NearEntityTableSize(); ++playerId) {
            arrayList.push_back(DayzGame::GetEntity(DayzGame::NearEntityTable(), playerId));
        }
        return arrayList;
    };
    vector<uint64_t> GetFarEntityes(){
        vector<uint64_t> arrayList;

        for (uint64_t playerId = NULL; playerId < DayzGame::FarEntityTableSize(); ++playerId) {
            arrayList.push_back(DayzGame::GetEntity(DayzGame::FarEntityTable(), playerId));
        }
        return arrayList;
    };
    vector<uint64_t> GetAllEntityes(){
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
private:
    string ReadArmaString(uint64_t address)
    {
        int length = GameProcess->Read<uint64_t>(address + off_length);
        char buffer[100];
        GameProcess->Read(address + off_text, buffer, length);
        return buffer;
    };
};
