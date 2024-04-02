#pragma once
#include "esp_sntp.h"
#include <time.h>
#include <sys/time.h>

#include "ble_mesh_model.h"
#include "esp_ble_mesh_time_scene_model_api.h"

#include "callbacks.h"

extern "C" struct net_buf_simple* get_net_buf(int size);


class BLEmeshTimeSrv : public IBLEMeshModel
{
private:
    esp_ble_mesh_time_state_t state = {};
    esp_ble_mesh_time_srv_t _data = {
        .rsp_ctrl = {
            .get_auto_rsp = get_auto_rsp,
            .set_auto_rsp = set_auto_rsp,
            .status_auto_rsp = status_auto_rsp,
        },
        .state = &state
    };

public:
    friend class BLEmeshTimeSetup;
    using IBLEMeshModel::IBLEMeshModel;
    ~BLEmeshTimeSrv() {}

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

    virtual esp_ble_mesh_time_state_t* getState() {
        return &state;
    }

    virtual void onEvent(uint32_t event, uint32_t op_code, void *params) override
    {
        ESP_LOGW(__func__, "%s event1: %ld, OP code: 0x%04lx\n", name(), event, op_code);
        if(cb)
            cb->onEvent(this, event, params);

        auto param = (esp_ble_mesh_time_scene_server_cb_param_t*)params;
        esp_ble_mesh_time_scene_server_cb_value_t value = param->value;
        auto addr = param->ctx.addr;

        if (op_code == ESP_BLE_MESH_MODEL_OP_TIME_GET)
        {
            uint32_t sec;
            uint32_t us;
            sntp_get_system_time(&sec, &us);
            printf("\tepoch: %ld\n", sec);
            updateTAIState(sec);
            sendMsg(addr, ESP_BLE_MESH_MODEL_OP_TIME_STATUS, (uint8_t*)_data.state, 10);
        } else {
            printf("\topcode: %ld\n", op_code);
        }
    }

    void update() {
        uint32_t sec;
        uint32_t us;
        sntp_get_system_time(&sec, &us);
        // printf("\tepoch: %ld\n", sec);
        updateTAIState(sec);

        bt_mesh_model_msg_init(pub.msg, ESP_BLE_MESH_MODEL_OP_TIME_STATUS);
        net_buf_simple_add_mem(_model->pub->msg, &state.time, 10);
    }

    virtual uint8_t timeRole() {
        return state.time_role;
    }

    virtual void timeRole(uint8_t val) {
        state.time_role = val;
    }

    virtual int16_t getUTCDelta() {
        return state.time.tai_utc_delta_curr - 255;
    }

private:
    virtual uint32_t getTAITime() {
        return ((*(uint64_t*)state.time.tai_seconds) & 0xFFFFFFFF);
    }

    virtual void updateTAIState(uint32_t t) {
        uint32_t _t = t; 
        if(t > 946684800) _t = t - 946684800;
        memcpy(state.time.tai_seconds, &_t, 4);
    }
};

class BLEmeshTimeSetup : public IBLEMeshModel
{
protected:
    BLEmeshTimeSrv* srv_model = nullptr;
    esp_ble_mesh_time_setup_srv_t _data = {
        .rsp_ctrl = {
            .get_auto_rsp = get_auto_rsp,
            .set_auto_rsp = set_auto_rsp,
            .status_auto_rsp = status_auto_rsp,
        },
        .state = NULL
    };

public:
    using IBLEMeshModel::IBLEMeshModel;
    ~BLEmeshTimeSetup() {}

    virtual void init(void*) override;

    esp_err_t registerCallback() override {
        return ESP_OK;
    }

    virtual void onEvent(uint32_t event, uint32_t op_code, void *params) override
    {
        ESP_LOGW(__func__, "%s event2: %ld, OP code: 0x%04lx\n", name(), event, op_code);
        if(cb)
            cb->onEvent(this, event, params);

        auto param = (esp_ble_mesh_time_scene_server_cb_param_t*)params;
        esp_ble_mesh_time_scene_server_cb_value_t value = param->value;
        auto addr = param->ctx.addr;

        if (op_code == ESP_BLE_MESH_MODEL_OP_TIME_SET)
        {
            uint32_t sec = srv_model->getTAITime() + 946684800 - srv_model->getUTCDelta();
            uint32_t us = 0;
            sntp_set_system_time(sec, us);
		setenv("TZ", "CET-1CEST", 1); // 1 means overwrite=true
		tzset();

            return;
        }
    }
};

class BLEmeshTimeCli : public IBLEMeshModel
{
private:
    esp_ble_mesh_client_t _data = {};

public:
    using IBLEMeshModel::IBLEMeshModel;
    ~BLEmeshTimeCli() {}

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

    // time_set
    // time_zone_set
    // tai_utc_delta_set
    // time_role_set
    virtual void setTAI() {
    }

    virtual void getTAI() {
    }

    virtual void onEvent(uint32_t event, uint32_t op_code, void *params) override
    {
        ESP_LOGD(__func__, "%s event: %ld, OP code: 0x%04lx\n", name(), event, op_code);
        if(cb)
            cb->onEvent(this, event, params);
    }

// esp_err_t esp_ble_mesh_time_scene_client_get_state(esp_ble_mesh_client_common_param_t *params, esp_ble_mesh_time_scene_client_get_state_t *get_state);
// esp_err_t esp_ble_mesh_time_scene_client_set_state(esp_ble_mesh_client_common_param_t *params, esp_ble_mesh_time_scene_client_set_state_t *set_state);

};

