#pragma once

#include "ble_mesh_model.h"
#include "esp_ble_mesh_time_scene_model_api.h"

#include "callbacks.h"

extern "C" struct net_buf_simple* get_net_buf(int size);

class BLEmeshSceneSrv : public IBLEMeshModel
{
private:
    esp_ble_mesh_scene_register_t scenes[16];

    esp_ble_mesh_scenes_state_t _scenes_state = {
    .scene_count = 16,};

    esp_ble_mesh_scene_srv_t _data = {
        .rsp_ctrl = {
            .get_auto_rsp = get_auto_rsp,
            .set_auto_rsp = set_auto_rsp,
            .status_auto_rsp = status_auto_rsp,
        },
        .state = &_scenes_state};

public:
    using IBLEMeshModel::IBLEMeshModel;
    ~BLEmeshSceneSrv() {}

    virtual void init(void*) override;

    esp_err_t registerCallback() override {
        return registerCallback(NULL);
    }

    esp_err_t registerCallback(esp_ble_mesh_time_scene_server_cb_t callback)
    {
        if (callback == NULL)
        {
            return esp_ble_mesh_register_time_scene_server_callback(_default_ble_mesh_time_scene_server_cb);
        }

        return esp_ble_mesh_register_time_scene_server_callback(callback);
    }

    esp_ble_mesh_scenes_state_t* getState() {
        return &_scenes_state;
    }

    virtual void onEvent(uint32_t event, uint32_t op_code, void *params) override
    {
        ESP_LOGD(__func__, "%s event: %ld, OP code: 0x%04lx\n", name(), event, op_code);
        if(cb)
            cb->onEvent(this, event, params);
    }
};

class BLEmeshSceneSetup : public IBLEMeshModel
{
protected:
    BLEmeshSceneSrv* srv_model = nullptr;
    esp_ble_mesh_scene_setup_srv_t _data = {
        .rsp_ctrl = {
            .get_auto_rsp = get_auto_rsp,
            .set_auto_rsp = set_auto_rsp,
            .status_auto_rsp = status_auto_rsp,
        },
        .state = NULL
    };

public:
    using IBLEMeshModel::IBLEMeshModel;
    ~BLEmeshSceneSetup() {}

    virtual void init(void*) override;

    esp_err_t registerCallback() override {
        return ESP_OK;
    }

    virtual void onEvent(uint32_t event, uint32_t op_code, void *params) override
    {
        ESP_LOGD(__func__, "%s event: %ld, OP code: 0x%04lx\n", name(), event, op_code);
        if(cb)
            cb->onEvent(this, event, params);
    }
};

class BLEmeshSceneCli : public IBLEMeshModel
{
private:
    esp_ble_mesh_client_t _data = {};

public:
    using IBLEMeshModel::IBLEMeshModel;
    ~BLEmeshSceneCli() {}

    virtual void init(void*) override;

    esp_err_t registerCallback() override {
        return registerCallback(NULL);
    }

    esp_err_t registerCallback(esp_ble_mesh_time_scene_client_cb_t callback)
    {
        if (callback == NULL)
        {
            return esp_ble_mesh_register_time_scene_client_callback(callback); /// @fixme
        }

        return esp_ble_mesh_register_time_scene_client_callback(callback);
    }

    virtual void onEvent(uint32_t event, uint32_t op_code, void *params) override
    {
        ESP_LOGD(__func__, "%s event: %ld, OP code: 0x%04lx\n", name(), event, op_code);
        if(cb)
            cb->onEvent(this, event, params);
    }
};
