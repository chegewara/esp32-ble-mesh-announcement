#pragma once

// https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/nrf/libraries/bluetooth_services/mesh/light_hsl_srv.html#model-composition
#include "light_model.h"


class LightHueSrvModel : public ILightSrvModel
{

private:
    esp_ble_mesh_light_hsl_state_t hsl_state;
    esp_ble_mesh_light_hsl_srv_t hsl_server = {
        .rsp_ctrl = {
            .get_auto_rsp = get_auto_rsp,
            .set_auto_rsp = set_auto_rsp,
            .status_auto_rsp = status_auto_rsp,
        },
        .state = &hsl_state,
    };
public:
    using ILightSrvModel::ILightSrvModel;
    ~LightHueSrvModel() {}


    /// @note type
    /// ESP_BLE_MESH_LIGHT_HSL_STATE,
    /// ESP_BLE_MESH_LIGHT_HSL_LIGHTNESS_STATE,
    /// ESP_BLE_MESH_LIGHT_HSL_HUE_STATE,
    /// ESP_BLE_MESH_LIGHT_HSL_SATURATION_STATE,

    virtual void init(void* p) override
    {
        (void)p;
        model_id = ESP_BLE_MESH_MODEL_ID_LIGHT_HSL_HUE_SRV;
        BLE_MESH_MODEL_PUB_DEFINE(&pub, 2 + 7, ROLE_NODE, __COUNTER__); /*!< msg len: 2 (opcode) + 7 max data */
        _model = new esp_ble_mesh_model_t( ESP_BLE_MESH_MODEL_LIGHT_HSL_HUE_SRV(&pub, &hsl_server) );
    }

    void onEvent(uint32_t event, uint32_t op_code, void* params) override;
};

class LightSaturationSrvModel : public ILightSrvModel
{

private:
    esp_ble_mesh_light_hsl_state_t hsl_state;
    esp_ble_mesh_light_hsl_srv_t hsl_server = {
        .rsp_ctrl = {
            .get_auto_rsp = get_auto_rsp,
            .set_auto_rsp = set_auto_rsp,
            .status_auto_rsp = status_auto_rsp,
        },
        .state = &hsl_state,
    };
public:
    using ILightSrvModel::ILightSrvModel;
    ~LightSaturationSrvModel() {}


    /// @note type
    /// ESP_BLE_MESH_LIGHT_HSL_STATE,
    /// ESP_BLE_MESH_LIGHT_HSL_LIGHTNESS_STATE,
    /// ESP_BLE_MESH_LIGHT_HSL_HUE_STATE,
    /// ESP_BLE_MESH_LIGHT_HSL_SATURATION_STATE,
// #define ESP_BLE_MESH_MODEL_ID_LIGHT_HSL_HUE_SRV                     0x130a
// #define ESP_BLE_MESH_MODEL_ID_LIGHT_HSL_SAT_SRV                     0x130b

    virtual void init(void* p) override
    {
        (void)p;
        model_id = ESP_BLE_MESH_MODEL_ID_LIGHT_HSL_SAT_SRV;
        BLE_MESH_MODEL_PUB_DEFINE(&pub, 2 + 7, ROLE_NODE, __COUNTER__); /*!< msg len: 2 (opcode) + 7 max data */
        _model = new esp_ble_mesh_model_t( ESP_BLE_MESH_MODEL_LIGHT_HSL_SAT_SRV(&pub, &hsl_server) );
    }

    void onEvent(uint32_t event, uint32_t op_code, void* params) override;
};

class LightHSLSrvModel : public ILightSrvModel
{
private:
    esp_ble_mesh_light_hsl_state_t hsl_state;
    esp_ble_mesh_light_hsl_srv_t hsl_server = {
        .rsp_ctrl = {
            .get_auto_rsp = get_auto_rsp,
            .set_auto_rsp = set_auto_rsp,
            .status_auto_rsp = status_auto_rsp,
        },
        .state = &hsl_state,
    };
public:
    using ILightSrvModel::ILightSrvModel;
    ~LightHSLSrvModel() {}


    /// @note type
    /// ESP_BLE_MESH_LIGHT_HSL_STATE,
    /// ESP_BLE_MESH_LIGHT_HSL_LIGHTNESS_STATE,
    /// ESP_BLE_MESH_LIGHT_HSL_HUE_STATE,
    /// ESP_BLE_MESH_LIGHT_HSL_SATURATION_STATE,
// #define ESP_BLE_MESH_MODEL_ID_LIGHT_HSL_HUE_SRV                     0x130a
// #define ESP_BLE_MESH_MODEL_ID_LIGHT_HSL_SAT_SRV                     0x130b

    virtual void init(void* p) override
    {
        (void)p;
        model_id = ESP_BLE_MESH_MODEL_ID_LIGHT_HSL_SRV;
        BLE_MESH_MODEL_PUB_DEFINE(&pub, 2 + 7, ROLE_NODE, __COUNTER__); /*!< msg len: 2 (opcode) + 7 max data */
        _model = new esp_ble_mesh_model_t( ESP_BLE_MESH_MODEL_LIGHT_HSL_SRV(&pub, &hsl_server) );
    }

    void onEvent(uint32_t event, uint32_t op_code, void* params) override;
};

class LightHSLSetupModel : public IBLEMeshModel
{
private:
    esp_ble_mesh_light_hsl_state_t hsl_state;
    esp_ble_mesh_light_hsl_setup_srv_t hsl_setup_server = {
        .rsp_ctrl = {
            .get_auto_rsp = get_auto_rsp,
            .set_auto_rsp = set_auto_rsp,
            .status_auto_rsp = status_auto_rsp,
        },
        .state = &hsl_state,
    };
public:
    using IBLEMeshModel::IBLEMeshModel;
    ~LightHSLSetupModel() {}


    virtual void init(void* p) override
    {
        (void)p;
        model_id = ESP_BLE_MESH_MODEL_ID_LIGHT_HSL_SETUP_SRV;
        BLE_MESH_MODEL_PUB_DEFINE(&pub, 2 + 8, ROLE_NODE, __COUNTER__); /*!< msg len: 2 (opcode) + 8 max data */
        _model = new esp_ble_mesh_model_t( ESP_BLE_MESH_MODEL_LIGHT_HSL_SETUP_SRV(&pub, &hsl_setup_server) );
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

    void onEvent(uint32_t event, uint32_t op_code, void* params) override;
};

class LightHSLCliModel : public IBLEMeshModel
{
private:
    esp_ble_mesh_client_t hsl_client = {};

public:
    using IBLEMeshModel::IBLEMeshModel;
    ~LightHSLCliModel() {}


    virtual void init(void* p) override
    {
        (void)p;
        model_id = ESP_BLE_MESH_MODEL_ID_LIGHT_HSL_CLI;
        BLE_MESH_MODEL_PUB_DEFINE(&pub, 2 + 15, ROLE_NODE, __COUNTER__); /*!< msg len: 2 (opcode) + 3 data */
        _model = new esp_ble_mesh_model_t( ESP_BLE_MESH_MODEL_LIGHT_HSL_CLI(&pub, &hsl_client) );
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

    void setHSL(uint16_t hue, uint16_t sat, uint16_t light)
    {
        esp_ble_mesh_client_common_param_t common = {};
        auto ctx = getCtx();

        common.opcode = ESP_BLE_MESH_MODEL_OP_LIGHT_HSL_SET_UNACK;
        common.model = _model;
        common.ctx.net_idx = ctx->net_idx;
        common.ctx.app_idx = ctx->app_idx;
        // common.ctx.addr = addr;
        common.ctx.send_ttl = ctx->send_ttl;
        common.msg_timeout = 0; /* 0 indicates that timeout value from menuconfig will be used */

        esp_ble_mesh_light_client_set_state_t set = {};
        set.hsl_set.op_en = false;
        set.hsl_set.hsl_hue = hue;
        set.hsl_set.hsl_saturation = sat;
        set.hsl_set.hsl_lightness = light;
        set.hsl_set.tid = tid++;

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
