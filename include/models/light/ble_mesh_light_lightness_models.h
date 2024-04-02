#pragma once


#include "light_model.h"


static esp_ble_mesh_light_lightness_state_t lightness_state;
class LightLightnessSrvModel : public ILightSrvModel
{
private:
    esp_ble_mesh_light_lightness_srv_t lightness_server = {
        .rsp_ctrl = {
            .get_auto_rsp = get_auto_rsp,
            .set_auto_rsp = set_auto_rsp,
            .status_auto_rsp = status_auto_rsp,
        },
        .state = &lightness_state,
    };
public:
    using ILightSrvModel::ILightSrvModel;
    ~LightLightnessSrvModel() {}


    virtual void init(void* p) override
    {
        (void)p;
        model_id = ESP_BLE_MESH_MODEL_ID_LIGHT_LIGHTNESS_SRV;
        BLE_MESH_MODEL_PUB_DEFINE(&pub, 2 + 5, ROLE_NODE, __COUNTER__); /*!< msg len: 2 (opcode) + 3 data */
        _model = new esp_ble_mesh_model_t( ESP_BLE_MESH_MODEL_LIGHT_LIGHTNESS_SRV(&pub, &lightness_server) );
    }
// esp_ble_mesh_lighting_server_cb_event_t
    void onEvent(uint32_t event, uint32_t op_code, void* params) override;
};

class LightLightnessSetupModel : public IBLEMeshModel
{
private:
    // esp_ble_mesh_light_lightness_state_t lightness_state;
    esp_ble_mesh_light_lightness_setup_srv_t lightness_setup_server = {
        .rsp_ctrl = {
            .get_auto_rsp = get_auto_rsp,
            .set_auto_rsp = set_auto_rsp,
            .status_auto_rsp = status_auto_rsp,
        },
        .state = &lightness_state,
    };
public:
    using IBLEMeshModel::IBLEMeshModel;
    ~LightLightnessSetupModel() {}


    virtual void init(void* p) override
    {
        (void)p;
        model_id = ESP_BLE_MESH_MODEL_ID_LIGHT_LIGHTNESS_SETUP_SRV;
        BLE_MESH_MODEL_PUB_DEFINE(&pub, 2 + 5, ROLE_NODE, __COUNTER__); /*!< msg len: 2 (opcode) + 4 data */
        _model = new esp_ble_mesh_model_t( ESP_BLE_MESH_MODEL_LIGHT_LIGHTNESS_SETUP_SRV(&pub, &lightness_setup_server) );
    }

    esp_err_t registerCallback() override {
        return registerCallback(NULL);
    }

    esp_err_t registerCallback(esp_ble_mesh_lighting_server_cb_t callback)
    {
        if (callback == NULL)
        {
            return esp_ble_mesh_register_lighting_server_callback(_default_ble_mesh_lighting_server_cb);
        }

        return esp_ble_mesh_register_lighting_server_callback(callback);
    }

    void onEvent(uint32_t event, uint32_t op_code, void* params) override {
        ESP_LOGD(__func__, "%s event: %ld, OP code: 0x%04lx\n", name(), event, op_code);
        if(cb)
            cb->onEvent(this, event, params);
    }
};

class LightLightnessCliModel : public IBLEMeshModel
{
private:
    esp_ble_mesh_client_t lightness_client = {};

public:
    using IBLEMeshModel::IBLEMeshModel;
    ~LightLightnessCliModel() {}


    virtual void init(void* p) override
    {
        (void)p;
        model_id = ESP_BLE_MESH_MODEL_ID_LIGHT_LIGHTNESS_CLI;
        BLE_MESH_MODEL_PUB_DEFINE(&pub, 2 + 3, ROLE_NODE, __COUNTER__); /*!< msg len: 2 (opcode) + 3 data */
        _model = new esp_ble_mesh_model_t( ESP_BLE_MESH_MODEL_LIGHT_LIGHTNESS_CLI(&pub, &lightness_client) );
    }

    esp_err_t registerCallback() override {
        return registerCallback(NULL);
    }

    esp_err_t registerCallback(esp_ble_mesh_light_client_cb_t callback)
    {
        if (callback == NULL)
        {
            return esp_ble_mesh_register_light_client_callback(_default_ble_mesh_lighting_client_cb);
        }

        return esp_ble_mesh_register_light_client_callback(callback);
    }

    void setLightness(uint16_t addr, uint16_t level)
    {
        esp_ble_mesh_client_common_param_t common = {};
        auto ctx = getCtx();

        common.opcode = ESP_BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_SET_UNACK;
        common.model = _model;
        common.ctx.net_idx = ctx->net_idx;
        common.ctx.app_idx = ctx->app_idx;
        common.ctx.addr = addr;
        common.ctx.send_ttl = ctx->send_ttl;
        common.msg_timeout = 0; /* 0 indicates that timeout value from menuconfig will be used */

        esp_ble_mesh_light_client_set_state_t set = {};
        set.lightness_set.op_en = false;
        set.lightness_set.lightness = level;
        set.lightness_set.tid = tid++;

        esp_err_t err = esp_ble_mesh_light_client_set_state(&common, &set);
        if (err)
        {
            ESP_LOGE("TAG", "Send Generic OnOff Set Unack failed");
            return;
        }
    }

    void onEvent(uint32_t event, uint32_t op_code, void* params) override {
        ESP_LOGD(__func__, "%s event: %ld, OP code: 0x%04lx\n", name(), event, op_code);
        if(cb)
            cb->onEvent(this, event, params);

        auto srv = ((esp_ble_mesh_lighting_server_cb_param_t*)params)->model->user_data;
        
    }
};
