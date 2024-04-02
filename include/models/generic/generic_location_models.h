#pragma once

#include "ble_mesh_model.h"
#include "generic_model.h"


/**
 * @brief 
 * 
 */
class GenericLocationSrvModel : public IGenericSrvModel
{
private:
    esp_ble_mesh_gen_location_state_t state;
    esp_ble_mesh_gen_location_srv_t loc_server = {
        .rsp_ctrl = {
            .get_auto_rsp = get_auto_rsp,
            .set_auto_rsp = set_auto_rsp,
            .status_auto_rsp = status_auto_rsp,
        },
        .state = &state
    };

public:
    using IGenericSrvModel::IGenericSrvModel;
    ~GenericLocationSrvModel() {}

    virtual void init(void*) override;

    virtual void onEvent(uint32_t event, uint32_t op_code, void *params) override;

    virtual esp_ble_mesh_gen_location_state_t* getState() {
        return &state;
    }

    virtual void updateLocalLocation(esp_ble_mesh_state_change_gen_loc_local_set_t& local) {
        loc_server.state->local_north = local.north;
        loc_server.state->local_east = local.east;
        loc_server.state->local_altitude = local.altitude;
        loc_server.state->floor_number = local.floor_number;
        loc_server.state->uncertainty = local.uncertainty;
    }

    virtual void updateGlobalLocation(esp_ble_mesh_state_change_gen_loc_global_set_t& global) {
        loc_server.state->global_latitude = global.latitude;
        loc_server.state->global_longitude = global.longitude;
        loc_server.state->global_altitude = global.altitude;
    }
};


/**
 * @brief 
 * 
 *
 */
class GenericLocationSetupModel : public IGenericSrvModel
{
private:
    GenericLocationSrvModel* srv_model = nullptr;
    esp_ble_mesh_gen_location_srv_t loc_server = {
        .rsp_ctrl = {
            .get_auto_rsp = get_auto_rsp,
            .set_auto_rsp = set_auto_rsp,
            .status_auto_rsp = status_auto_rsp,
        },
        .state = NULL
    };

public:
    using IGenericSrvModel::IGenericSrvModel;

    ~GenericLocationSetupModel() {}

    virtual void init(void*) override;

    virtual void onEvent(uint32_t event, uint32_t op_code, void *params) override;

    virtual void updateLocalLocation(esp_ble_mesh_state_change_gen_loc_local_set_t& local) {
        loc_server.state->local_north = local.north;
        loc_server.state->local_east = local.east;
        loc_server.state->local_altitude = local.altitude;
        loc_server.state->floor_number = local.floor_number;
        loc_server.state->uncertainty = local.uncertainty;
    }

    virtual void updateGlobalLocation(esp_ble_mesh_state_change_gen_loc_global_set_t& global) {
        loc_server.state->global_latitude = global.latitude;
        loc_server.state->global_longitude = global.longitude;
        loc_server.state->global_altitude = global.altitude;
    }
};


/**
 * @brief 
 * 
 */
class GenericLocationCliModel : public IGenericCliModel
{
private:
    esp_ble_mesh_client_t loc_client = {};

public:
    using IGenericCliModel::IGenericCliModel;
    ~GenericLocationCliModel() {}

    virtual void init(void*) override;

    virtual void getLocalLocation(uint16_t address, uint8_t ttl = 2);

    virtual void getGlobalLocation(uint16_t address, uint8_t ttl = 2);

    virtual void setLocalLocation(uint16_t address, esp_ble_mesh_gen_location_state_t* data, bool ack = false);

    virtual void setGlobalLocation(uint16_t address, esp_ble_mesh_gen_location_state_t* data, bool ack = false);

};
