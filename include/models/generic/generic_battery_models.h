#pragma once


#include "ble_mesh_model.h"
#include "generic_model.h"


class BLEMeshBatterySrv : public IGenericSrvModel
{
private:
    esp_ble_mesh_gen_battery_srv_t server = {
        .rsp_ctrl = {
            .get_auto_rsp = get_auto_rsp,
            .set_auto_rsp = set_auto_rsp,
            .status_auto_rsp = status_auto_rsp,
        }
    };

public:
    using IGenericSrvModel::IGenericSrvModel;
    ~BLEMeshBatterySrv() {}

    virtual void init(void*) override;

    virtual void onEvent(uint32_t event, uint32_t op_code, void *params) override;

};


class BLEMeshBatteryCli : public IGenericCliModel
{
private:
    esp_ble_mesh_client_t _client = {};

public:
    using IGenericCliModel::IGenericCliModel;
    ~BLEMeshBatteryCli() {}

    virtual void init(void*) override;
};

