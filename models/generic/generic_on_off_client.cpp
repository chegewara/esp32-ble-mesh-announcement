#include "generic_on_off_models.h"
#include "ble_mesh_prov.h"


void GenericOnOffCliModel::init(void* p)
{
    (void)p;
    model_id = ESP_BLE_MESH_MODEL_ID_GEN_ONOFF_CLI;
    BLE_MESH_MODEL_PUB_DEFINE(&pub, 2 + 3, ROLE_NODE, __COUNTER__); /*!< msg len: 2 (opcode) + 3 (val + delay + trasition) */
    _model = new esp_ble_mesh_model_t(ESP_BLE_MESH_MODEL_GEN_ONOFF_CLI(&pub, &onoff_client));
}

void GenericOnOffCliModel::turnOn(uint16_t address, bool ack)
{
    uint8_t val = 1;
    uint32_t opcode = 0;
    if(ack)
        opcode = ESP_BLE_MESH_MODEL_OP_GEN_ONOFF_SET;
    else
        opcode = ESP_BLE_MESH_MODEL_OP_GEN_ONOFF_SET_UNACK;
    sendState(address, opcode, &val);
}

void GenericOnOffCliModel::turnOff(uint16_t address, bool ack)
{
    uint8_t val = 0;
    uint32_t opcode = 0;
    if(ack)
        opcode = ESP_BLE_MESH_MODEL_OP_GEN_ONOFF_SET;
    else
        opcode = ESP_BLE_MESH_MODEL_OP_GEN_ONOFF_SET_UNACK;
    sendState(address, opcode, &val);
}
