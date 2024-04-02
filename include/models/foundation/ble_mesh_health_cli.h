#pragma once

#include "esp_ble_mesh_health_model_api.h"

#include "ble_mesh_model.h"

class BLEmeshHealthCli : public IBLEMeshModel
{
private:
    esp_ble_mesh_client_t health_client = {};
public:
    using IBLEMeshModel::IBLEMeshModel;
    ~BLEmeshHealthCli() {}


    void init(void* p) override {
        (void)p;
        model_id = ESP_BLE_MESH_MODEL_ID_HEALTH_CLI;
        _model = new esp_ble_mesh_model_t( ESP_BLE_MESH_MODEL_HEALTH_CLI(&health_client) );
    }

    esp_err_t registerCallback() override {
        return registerCallback(NULL);
    }

    virtual esp_err_t registerCallback(esp_ble_mesh_health_client_cb_t callback) {
        if (callback == NULL)
        {
            return esp_ble_mesh_register_health_client_callback(callback); /// @fixme
        }

        return esp_ble_mesh_register_health_client_callback(callback);
    }

    virtual void onEvent(uint32_t event, uint32_t op_code, void *params) override
    {
        ESP_LOGD(__func__, "%s event: %ld, OP code: 0x%04lx\n", name(), event, op_code);
        if(cb)
            cb->onEvent(this, event, params);
    }

// esp_err_t esp_ble_mesh_health_client_get_state(esp_ble_mesh_client_common_param_t *params, esp_ble_mesh_health_client_get_state_t *get_state);
// esp_err_t esp_ble_mesh_health_client_set_state(esp_ble_mesh_client_common_param_t *params, esp_ble_mesh_health_client_set_state_t *set_state);

};
