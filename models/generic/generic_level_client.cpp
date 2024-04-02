#include "generic_level_models.h"
/*
void GenericLevelCliModel::setLevel(uint16_t address, int16_t level, bool ack)
{
    esp_ble_mesh_client_common_param_t common = {};
    auto ctx = getCtx();

    if(ack)
        common.opcode = ESP_BLE_MESH_MODEL_OP_GEN_LEVEL_SET;
    else
        common.opcode = ESP_BLE_MESH_MODEL_OP_GEN_LEVEL_SET_UNACK;

    common.model = _model;
    common.ctx.net_idx = ctx->net_idx;
    common.ctx.app_idx = ctx->app_idx;
    common.ctx.addr = address;
    common.ctx.send_ttl = ctx->send_ttl;
    common.msg_timeout = 0; 

    esp_ble_mesh_generic_client_set_state_t set = {};
    set.level_set.op_en = false;
    set.level_set.level = level;
    set.level_set.tid = tid++;

    esp_err_t err = esp_ble_mesh_generic_client_set_state(&common, &set);
    if (err)
    {
        ESP_LOGE("TAG", "Send Generic OnOff Set Unack failed");
        return;
    }
}
*/
void GenericLevelCliModel::level(uint16_t address)
{
    getState(address, ESP_BLE_MESH_MODEL_OP_GEN_LEVEL_GET);
}

void GenericLevelCliModel::init(void* p)
{
    (void)p;
    model_id = ESP_BLE_MESH_MODEL_ID_GEN_LEVEL_CLI;
    BLE_MESH_MODEL_PUB_DEFINE(&pub, 2 + 5, ROLE_NODE, __COUNTER__); /*!< msg len: 2 (opcode) + 5 (op + val*2 + delay + trasition) */
    _model = new esp_ble_mesh_model_t(ESP_BLE_MESH_MODEL_GEN_LEVEL_CLI(&pub, &_client));
}

void GenericLevelCliModel::level(uint16_t address, uint16_t lvl, bool ack)
{
    uint32_t opcode = 0;
    if(ack)
        opcode = ESP_BLE_MESH_MODEL_OP_GEN_LEVEL_SET;
    else
        opcode = ESP_BLE_MESH_MODEL_OP_GEN_LEVEL_SET_UNACK;
    sendState(address, opcode, &lvl);
}
