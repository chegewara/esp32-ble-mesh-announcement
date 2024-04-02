#include "ble_mesh_scene_models.h"

void BLEmeshSceneCli::init(void* p)
{
    (void)p;
    model_id = ESP_BLE_MESH_MODEL_ID_SCENE_CLI;
    BLE_MESH_MODEL_PUB_DEFINE(&pub, 2 + 3, ROLE_NODE, __COUNTER__); /*!< msg len: 2 (opcode) + 3 */
    _model = new esp_ble_mesh_model_t(ESP_BLE_MESH_MODEL_SCENE_CLI(&pub, &_data));
}