#include "generic_on_off_models.h"

void GenericOnOffSrvModel::init(void* p)
{
    (void)p;
    model_id = ESP_BLE_MESH_MODEL_ID_GEN_ONOFF_SRV;
    BLE_MESH_MODEL_PUB_DEFINE(&pub, 2 + 3, ROLE_NODE, __COUNTER__); /*!< msg len: 2 (opcode) + 3 (val + delay + trasition) */
    _model = new esp_ble_mesh_model_t(ESP_BLE_MESH_MODEL_GEN_ONOFF_SRV(&pub, &onoff_server));
    pub.msg->len = 5;
}

void GenericOnOffSrvModel::onEvent(uint32_t event, uint32_t op_code, void *params)
{
    ESP_LOGI(__func__, "%s event: %ld, OP code: 0x%04lx\n", name(), event, op_code);
    if(cb)
        cb->onEvent(this, event, params);

    esp_ble_mesh_generic_server_cb_param_t *param = (esp_ble_mesh_generic_server_cb_param_t *)params;

    switch (event)
    {
    case ESP_BLE_MESH_GENERIC_SERVER_RECV_GET_MSG_EVT:
    {
        if (!cb)
        {
            if (op_code == ESP_BLE_MESH_MODEL_OP_GEN_ONOFF_GET)
                cb->onGet(this, params);
        }
        sendStatus(param->ctx.addr, ESP_BLE_MESH_MODEL_OP_GEN_ONOFF_STATUS, &onoff_server.state.onoff);
        break;
    }
    case ESP_BLE_MESH_GENERIC_SERVER_STATE_CHANGE_EVT:
    {
        if (cb)
        {
            if (op_code == ESP_BLE_MESH_MODEL_OP_GEN_ONOFF_SET || op_code == ESP_BLE_MESH_MODEL_OP_GEN_ONOFF_SET_UNACK)
            {
                cb->onChange(this, params);
            }
        }
        break;
    }
    case ESP_BLE_MESH_GENERIC_SERVER_RECV_SET_MSG_EVT:
    {
        if (cb)
        {
            if (op_code == ESP_BLE_MESH_MODEL_OP_GEN_ONOFF_SET || op_code == ESP_BLE_MESH_MODEL_OP_GEN_ONOFF_SET_UNACK)
            {
                cb->onSet(this, params);
            }
        }

        if (op_code == ESP_BLE_MESH_MODEL_OP_GEN_ONOFF_SET)
        {
            sendStatus(param->ctx.addr, ESP_BLE_MESH_MODEL_OP_GEN_ONOFF_STATUS, &onoff_server.state.onoff);
        }

        publishStatus(ESP_BLE_MESH_MODEL_OP_GEN_ONOFF_STATUS, &onoff_server.state.onoff);
        break;
    }

    default:
        break;
    }
}
