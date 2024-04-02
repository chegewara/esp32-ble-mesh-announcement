#include "ble_mesh_scene_models.h"

/// @brief external data top share between server and srv_setup
/// @todo check if other server-setup_srv data share is required
void BLEmeshSceneSrv::init(void* p)
{
    (void)p;
    model_id = ESP_BLE_MESH_MODEL_ID_SCENE_SRV;
    BLE_MESH_MODEL_PUB_DEFINE(&pub, 2 + 3, ROLE_NODE, __COUNTER__); /*!< msg len: 2 (opcode) + 3 */
    _model = new esp_ble_mesh_model_t(ESP_BLE_MESH_MODEL_SCENE_SRV(&pub, &_data));
    for (size_t i = 0; i < 16; i++)
    {
        scenes[i] = {
            .scene_value = get_net_buf(1),
        };
    }
    _scenes_state.scenes = scenes;
}

void BLEmeshSceneSetup::init(void* srv)
{
    assert(srv);
    model_id = ESP_BLE_MESH_MODEL_ID_SCENE_SETUP_SRV;
    BLE_MESH_MODEL_PUB_DEFINE(&pub, 2 + 2, ROLE_NODE, __COUNTER__); /*!< msg len: 2 (opcode) + 2 */
    _model = new esp_ble_mesh_model_t(ESP_BLE_MESH_MODEL_SCENE_SETUP_SRV(&pub, &_data));

    srv_model = (BLEmeshSceneSrv*)srv;
    _data.state = srv_model->getState();
}
