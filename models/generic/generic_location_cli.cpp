#include "generic_location_models.h"

void GenericLocationCliModel::init(void* p)
{
    (void)p;
    model_id = ESP_BLE_MESH_MODEL_ID_GEN_LOCATION_CLI;
    BLE_MESH_MODEL_PUB_DEFINE(&pub, 2 + 1, ROLE_NODE, __COUNTER__); /*!< msg len: 2 (opcode) */
    _model = new esp_ble_mesh_model_t(ESP_BLE_MESH_MODEL_GEN_LOCATION_CLI(&pub, &loc_client));
}


void GenericLocationCliModel::getLocalLocation(uint16_t address, uint8_t ttl)
{
    auto ctx = getCtx();
    ctx->send_ttl = ttl;
    getState(address, ESP_BLE_MESH_MODEL_OP_GEN_LOC_LOCAL_GET);
}

void GenericLocationCliModel::getGlobalLocation(uint16_t address, uint8_t ttl)
{
    auto ctx = getCtx();
    ctx->send_ttl = ttl;
    getState(address, ESP_BLE_MESH_MODEL_OP_GEN_LOC_GLOBAL_GET);
}

void GenericLocationCliModel::setLocalLocation(uint16_t address, esp_ble_mesh_gen_location_state_t *data, bool ack)
{
    uint32_t opcode = 0;
    if(ack)
        opcode = ESP_BLE_MESH_MODEL_OP_GEN_LOC_LOCAL_SET;
    else
        opcode = ESP_BLE_MESH_MODEL_OP_GEN_LOC_LOCAL_SET_UNACK;

    sendState(address, opcode, data);
}

void GenericLocationCliModel::setGlobalLocation(uint16_t address, esp_ble_mesh_gen_location_state_t *data, bool ack)
{
    uint32_t opcode = 0;
    if(ack)
        opcode = ESP_BLE_MESH_MODEL_OP_GEN_LOC_GLOBAL_SET;
    else
        opcode = ESP_BLE_MESH_MODEL_OP_GEN_LOC_GLOBAL_SET_UNACK;

    sendState(address, opcode, data);
}
