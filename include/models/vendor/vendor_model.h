#pragma once

#include "ble_mesh_model.h"

#include "callbacks.h"

/**
 * @brief 
 * 
 */
class VendorSrvModel : public IBLEMeshModel
{
private:
esp_ble_mesh_client_t vendor_client = {
    .op_pair_size = 0,
    .op_pair = nullptr,
};

public:
    using IBLEMeshModel::IBLEMeshModel;
    ~VendorSrvModel() {}

    virtual void init(void* p) override {
        ESP_LOGW(TAG, "not supported in this model: %s", __func__);
    }

    void init(uint16_t cid, uint16_t vid, esp_ble_mesh_model_op_t *vnd_op, esp_ble_mesh_client_op_pair_t *vnd_op_pair = NULL , size_t count = 0);

    esp_err_t registerCallback() override {
        return registerCallback(NULL);
    }

    esp_err_t registerCallback(esp_ble_mesh_model_cb_t callback)
    {
        if (callback == NULL)
        {
            return esp_ble_mesh_register_custom_model_callback(_default_ble_mesh_custom_model_cb);
        }

        return esp_ble_mesh_register_custom_model_callback(callback);
    }

    virtual void onEvent(uint32_t event, uint32_t op_code, void *params) override;
};
