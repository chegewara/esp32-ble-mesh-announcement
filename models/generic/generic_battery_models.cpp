#include "generic_battery_models.h"

void BLEMeshBatterySrv::init(void* p)
{
    (void)p;
    model_id = ESP_BLE_MESH_MODEL_ID_GEN_BATTERY_SRV;
    BLE_MESH_MODEL_PUB_DEFINE(&pub, 2 + 8, ROLE_NODE, __COUNTER__); /*!< msg len: 2 (opcode) + 8 */
    _model = new esp_ble_mesh_model_t(ESP_BLE_MESH_MODEL_GEN_BATTERY_SRV(&pub, &server));
}

void BLEMeshBatterySrv::onEvent(uint32_t event, uint32_t op_code, void *params)
{
    ESP_LOGI(__func__, "%s event: %ld, OP code: 0x%04lx\n", name(), event, op_code);
    if(cb)
        cb->onEvent(this, event, params);
}

void BLEMeshBatteryCli::init(void* p)
{
    (void)p;
    model_id = ESP_BLE_MESH_MODEL_ID_GEN_BATTERY_CLI;
    BLE_MESH_MODEL_PUB_DEFINE(&pub, 2 + 0, ROLE_NODE, __COUNTER__); /*!< msg len: 2 (opcode) */
    _model = new esp_ble_mesh_model_t(ESP_BLE_MESH_MODEL_GEN_BATTERY_CLI(&pub, &_client));
}
