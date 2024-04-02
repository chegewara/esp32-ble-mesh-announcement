#include "generic_location_models.h"

void GenericLocationSrvModel::init(void* p)
{
    (void)p;
    model_id = ESP_BLE_MESH_MODEL_ID_GEN_LOCATION_SRV;
    BLE_MESH_MODEL_PUB_DEFINE(&pub, 0, ROLE_NODE, __COUNTER__); /*!< msg len: 1 or 2 (opcode) + 9 or 10 */
    _model = new esp_ble_mesh_model_t(ESP_BLE_MESH_MODEL_GEN_LOCATION_SRV(&pub, &loc_server));
}

/// @todo test it 
void GenericLocationSrvModel::onEvent(uint32_t event, uint32_t op_code, void *params)
{
    ESP_LOGD(__func__, "%s event: %ld, OP code: 0x%04lx\n", name(), event, op_code);

    if(cb)
        cb->onEvent(this, event, params);
}

void GenericLocationSetupModel::init(void* srv)
{
    assert(srv);
    model_id = ESP_BLE_MESH_MODEL_ID_GEN_LOCATION_SETUP_SRV;
    BLE_MESH_MODEL_PUB_DEFINE(&pub, 2 + 11, ROLE_NODE, __COUNTER__); /*!< msg len: 1 or 2 (opcode) + 9 or 10 */
    _model = new esp_ble_mesh_model_t(ESP_BLE_MESH_MODEL_GEN_LOCATION_SETUP_SRV(&pub, &loc_server));

    srv_model = (GenericLocationSrvModel*)srv;
    loc_server.state = srv_model->getState();
}

void GenericLocationSetupModel::onEvent(uint32_t event, uint32_t op_code, void *params)
{
    ESP_LOGD(__func__, "%s event: %ld, OP code: 0x%04lx\n", name(), event, op_code);

    if(cb)
        cb->onEvent(this, event, params);
}
