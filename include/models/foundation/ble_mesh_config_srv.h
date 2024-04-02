#pragma once

#include "esp_ble_mesh_config_model_api.h"

#include "ble_mesh_model.h"


#include "callbacks.h"

class BLEmeshConfigSrv : public IBLEMeshModel
{
protected:
    static esp_ble_mesh_cfg_srv_t config_server;

public:
    using IBLEMeshModel::IBLEMeshModel;
    ~BLEmeshConfigSrv() {}

    static esp_ble_mesh_cfg_srv_t *config()
    {
        return &config_server;
    }

    static void config(esp_ble_mesh_cfg_srv_t cfg)
    {
        config_server = cfg;
    }

    static void relay(bool en)
    {
        config_server.relay = en;
    }

    static void proxy(bool en)
    {
        config_server.gatt_proxy = en;
    }

    static void beacon(bool en)
    {
        config_server.beacon = en;
    }

    static void friends(bool en)
    {
        config_server.friend_state = en;
    }

    static void defaultTTL(uint8_t ttl) {
        config_server.default_ttl = ttl;
    }

    static void transmit(uint8_t cnt, uint8_t ms) {
        config_server.net_transmit = ESP_BLE_MESH_TRANSMIT(cnt, ms);
    }

    static void retransmit(uint8_t cnt, uint8_t ms) {
        config_server.relay_retransmit = ESP_BLE_MESH_TRANSMIT(cnt, ms);
    }

    void init(void* p) override
    {
        (void)p;
        model_id = ESP_BLE_MESH_MODEL_ID_CONFIG_SRV;
        _model = new esp_ble_mesh_model_t(ESP_BLE_MESH_MODEL_CFG_SRV(config()));
    }

    esp_err_t registerCallback() override {
        return registerCallback(NULL);
    }

    static esp_err_t registerCallback(esp_ble_mesh_cfg_server_cb_t callback) {
        if (callback == NULL)
        {
            return esp_ble_mesh_register_config_server_callback(_default_ble_mesh_config_server_cb);
        }

        return esp_ble_mesh_register_config_server_callback(callback);
    }

    void onEvent(uint32_t event, uint32_t op_code, void *params) override
    {
        ESP_LOGD(__func__, "%s event: %ld, OP code: 0x%04lx\n", name(), event, op_code);
        auto param = (esp_ble_mesh_cfg_server_cb_param_t *)params;
        if(cb)
            cb->onEvent(this, event, params);
    }
};
