#include "generic_level_models.h"

void GenericLevelSrvModel::init(void* p)
{
    (void)p;
    model_id = ESP_BLE_MESH_MODEL_ID_GEN_LEVEL_SRV;
    BLE_MESH_MODEL_PUB_DEFINE(&pub, 2 + 5, ROLE_NODE, __COUNTER__); /*!< msg len: 2 (opcode) + 5 (op + levelx2 + delay + trasition) */
    _model = new esp_ble_mesh_model_t(ESP_BLE_MESH_MODEL_GEN_LEVEL_SRV(&pub, &level_state));
}

void GenericLevelSrvModel::onEvent(uint32_t event, uint32_t op_code, void *params)
{
    ESP_LOGD(__func__, "%s event: %ld, OP code: 0x%04lx\n", name(), event, op_code);
    if(cb)
        cb->onEvent(this, event, params);

    auto *param = (esp_ble_mesh_generic_server_cb_param_t *)params;
    auto srv = (esp_ble_mesh_gen_level_srv_t *)param->model->user_data;
    switch (event)
    {
    case ESP_BLE_MESH_GENERIC_SERVER_RECV_GET_MSG_EVT:
    {
        if (op_code == ESP_BLE_MESH_MODEL_OP_GEN_LEVEL_GET)
        {
            if (cb)
                cb->onGet(this, params);
        }
        sendStatus(param->ctx.addr, ESP_BLE_MESH_MODEL_OP_GEN_LEVEL_STATUS, (uint8_t *)&srv->state.level);
        break;
    }
    case ESP_BLE_MESH_GENERIC_SERVER_STATE_CHANGE_EVT:
    {
        if (op_code == ESP_BLE_MESH_MODEL_OP_GEN_LEVEL_SET || op_code == ESP_BLE_MESH_MODEL_OP_GEN_LEVEL_SET_UNACK)
        {
            if (cb)
                cb->onChange(this, params);
        }
        break;
    }
    case ESP_BLE_MESH_GENERIC_SERVER_RECV_SET_MSG_EVT:
    {
        if (op_code == ESP_BLE_MESH_MODEL_OP_GEN_LEVEL_SET || op_code == ESP_BLE_MESH_MODEL_OP_GEN_LEVEL_SET_UNACK)
        {
            if (cb)
                cb->onSet(this, params);
        }
        ESP_LOGW("", "%d => %d", srv->state.level, param->value.set.level.level);
        srv->state.level = param->value.set.level.level;
        if (op_code == ESP_BLE_MESH_MODEL_OP_GEN_LEVEL_SET)
        {
            sendStatus(param->ctx.addr, ESP_BLE_MESH_MODEL_OP_GEN_LEVEL_STATUS, (uint8_t *)&srv->state.level);
        }
        publishStatus(ESP_BLE_MESH_MODEL_OP_GEN_LEVEL_STATUS, (uint8_t *)&srv->state.level);
        break;
    }

    default:
        ESP_LOGW(name(), "event: %ld", event);
        break;
    }
}
