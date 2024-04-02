#pragma once

#include "ble_mesh_model.h"
#include "generic_model.h"

/**
 * @brief 
 * 
 */
class GenericOnOffSrvModel : public IGenericSrvModel
{
private:
    esp_ble_mesh_gen_onoff_srv_t onoff_server = {
        .rsp_ctrl = {
            .get_auto_rsp = get_auto_rsp,
            .set_auto_rsp = set_auto_rsp,
            .status_auto_rsp = status_auto_rsp,
        }};

public:
    using IGenericSrvModel::IGenericSrvModel;
    ~GenericOnOffSrvModel() {}

    virtual void init(void*) override;

    virtual void onEvent(uint32_t event, uint32_t op_code, void *params) override;

    virtual esp_err_t updateState(uint8_t val) {
        esp_ble_mesh_server_state_value_t value = {
            .gen_onoff = val
        };
        return esp_ble_mesh_server_model_update_state(_model, ESP_BLE_MESH_GENERIC_ONOFF_STATE, &value);
    }
};


/**
 * @brief 
 * 
 */
class GenericOnOffCliModel : public IGenericCliModel
{
private:
    esp_ble_mesh_client_t onoff_client = {};

public:
    using IGenericCliModel::IGenericCliModel;
    ~GenericOnOffCliModel() {}

    virtual void init(void*) override;

    virtual void turnOn(uint16_t address, bool ack = false);
    
    virtual void turnOff(uint16_t address, bool ack = false);

};
