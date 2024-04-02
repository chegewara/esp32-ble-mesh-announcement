#pragma once

#include "ble_mesh_model.h"

#include "callbacks.h"

static uint16_t get_length(uint32_t opcode)
{
    switch (opcode)
    {
        case ESP_BLE_MESH_MODEL_OP_GEN_ONOFF_STATUS: return 1;
        case ESP_BLE_MESH_MODEL_OP_GEN_LOC_GLOBAL_STATUS: return 10;
        case ESP_BLE_MESH_MODEL_OP_GEN_LOC_LOCAL_STATUS: return 9;
        case ESP_BLE_MESH_MODEL_OP_GEN_BATTERY_STATUS: return 8;
        case ESP_BLE_MESH_MODEL_OP_GEN_LEVEL_STATUS:
        case ESP_BLE_MESH_MODEL_OP_GEN_DEF_TRANS_TIME_STATUS:
        case ESP_BLE_MESH_MODEL_OP_GEN_ONPOWERUP_STATUS:
        case ESP_BLE_MESH_MODEL_OP_GEN_POWER_LEVEL_STATUS:
        case ESP_BLE_MESH_MODEL_OP_GEN_POWER_LAST_STATUS:
        case ESP_BLE_MESH_MODEL_OP_GEN_POWER_DEFAULT_STATUS:
        case ESP_BLE_MESH_MODEL_OP_GEN_POWER_RANGE_STATUS:
        case ESP_BLE_MESH_MODEL_OP_GEN_MANUFACTURER_PROPERTIES_STATUS:
        case ESP_BLE_MESH_MODEL_OP_GEN_MANUFACTURER_PROPERTY_STATUS:
        case ESP_BLE_MESH_MODEL_OP_GEN_ADMIN_PROPERTIES_STATUS:
        case ESP_BLE_MESH_MODEL_OP_GEN_ADMIN_PROPERTY_STATUS:
        case ESP_BLE_MESH_MODEL_OP_GEN_USER_PROPERTIES_STATUS:
        case ESP_BLE_MESH_MODEL_OP_GEN_USER_PROPERTY_STATUS:
        case ESP_BLE_MESH_MODEL_OP_GEN_CLIENT_PROPERTIES_STATUS:
        default:
            ESP_LOGW(__func__, "opcode 0x%04lxnot implemented", opcode);
    }

    return 0;
}

/**
 * @brief
 *
 */
class IGenericSrvModel : public IBLEMeshModel
{

public:
    using IBLEMeshModel::IBLEMeshModel;
    ~IGenericSrvModel() {}

    esp_err_t registerCallback() override {
        return registerCallback(NULL);
    }

    esp_err_t registerCallback(esp_ble_mesh_generic_server_cb_t callback)
    {
        if (callback == NULL)
        {
            return esp_ble_mesh_register_generic_server_callback(_default_ble_mesh_generic_server_cb);
        }

        return esp_ble_mesh_register_generic_server_callback(callback);
    }

    /**
     * @brief
     * @note transition not implemented
     * @param address
     * @param data
     * @return esp_err_t
     */
    virtual esp_err_t sendStatus(uint16_t address, uint32_t opcode, uint8_t *data)
    {
        uint16_t status_msg_len = get_length(opcode);
        return sendMsg(address, opcode, data, status_msg_len);
    }

    /**
     * @brief
     * @note transition not implemented
     *
     * @param data
     * @return esp_err_t
     */
    virtual esp_err_t publishStatus(uint32_t opcode, uint8_t *data)
    {
        uint16_t status_msg_len = get_length(opcode);
        return publish(opcode, data, status_msg_len);
    }

};

class IGenericCliModel : public IBLEMeshModel
{
private:
public:
    using IBLEMeshModel::IBLEMeshModel;
    ~IGenericCliModel() {}

    esp_err_t registerCallback() override {
        return registerCallback(NULL);
    }

    esp_err_t registerCallback(esp_ble_mesh_generic_client_cb_t callback)
    {
        if (callback == NULL)
        {
            return esp_ble_mesh_register_generic_client_callback(_default_ble_mesh_generic_client_cb);
        }

        return esp_ble_mesh_register_generic_client_callback(callback);
    }

    virtual esp_err_t sendState(uint16_t address, uint32_t opcode, void *data) final;

    virtual esp_err_t getState(uint16_t address, uint32_t opcode) final;

    virtual void onEvent(uint32_t event, uint32_t op_code, void *params) override;

};
