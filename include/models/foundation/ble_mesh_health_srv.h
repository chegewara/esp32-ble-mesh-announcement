#pragma once

#include "esp_ble_mesh_health_model_api.h"

#include "ble_mesh_model.h"

class BLEmeshHealthSrv : public IBLEMeshModel
{
private:
    uint8_t test_ids[1] = {0x00};
    esp_ble_mesh_health_srv_t health_server = {
        .health_test = {
            .id_count = 1,
            .test_ids = test_ids,
        },
    };
public:
    using IBLEMeshModel::IBLEMeshModel;
    ~BLEmeshHealthSrv() {}


    void init(void* p) override {
        (void)p;
        model_id = ESP_BLE_MESH_MODEL_ID_HEALTH_SRV;
        BLE_MESH_MODEL_PUB_DEFINE(&pub, 2 + 11, ROLE_NODE, __COUNTER__); /*!< msg len: 2 (opcode) + 11 (???) */
        _model = new esp_ble_mesh_model_t( ESP_BLE_MESH_MODEL_HEALTH_SRV(&health_server, &pub) );
    }

    esp_err_t registerCallback() override {
        return registerCallback(NULL);
    }

    virtual esp_err_t registerCallback(esp_ble_mesh_health_server_cb_t callback) {
        if (callback == NULL)
        {
            return esp_ble_mesh_register_health_server_callback(callback); /// @fixme
        }

        return esp_ble_mesh_register_health_server_callback(callback);
    }

    virtual void onEvent(uint32_t event, uint32_t op_code, void *params) override
    {
        ESP_LOGD(__func__, "%s event: %ld, OP code: 0x%04lx\n", name(), event, op_code);
        if(cb)
            cb->onEvent(this, event, params);
    }

};
