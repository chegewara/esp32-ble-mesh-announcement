#pragma once

#include "ble_mesh_model.h"
#include "generic_model.h"

/**
 * @brief 
 * 
 */
class GenericLevelSrvModel : public IGenericSrvModel
{
private:
    esp_ble_mesh_gen_level_srv_t level_state = {
        .rsp_ctrl = {
            .get_auto_rsp = get_auto_rsp,
            .set_auto_rsp = set_auto_rsp,
            .status_auto_rsp = status_auto_rsp,
        }
    };
public:
    using IGenericSrvModel::IGenericSrvModel;
    ~GenericLevelSrvModel() { }

    virtual void init(void*) override;

    virtual int16_t level() {
        return level_state.state.level;
    }

    virtual void onEvent(uint32_t event, uint32_t op_code, void* params) override;

    virtual esp_err_t updateState(int16_t val) {
        esp_ble_mesh_server_state_value_t value = {
            .gen_level = val
        };
        return esp_ble_mesh_server_model_update_state(_model, ESP_BLE_MESH_GENERIC_LEVEL_STATE, &value);
    }

};

/**
 * @brief 
 * 
 */
class GenericLevelCliModel : public IGenericCliModel
{
private:
    esp_ble_mesh_client_t _client = {};
    
public:
    using IGenericCliModel::IGenericCliModel;
    ~GenericLevelCliModel() {}

    virtual void init(void *) override;

    // virtual void setLevel(uint16_t address, int16_t level, bool ack);

    virtual void level(uint16_t address);

    virtual void level(uint16_t address, uint16_t lvl, bool ack = false);

};
