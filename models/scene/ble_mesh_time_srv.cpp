#include "ble_mesh_time_models.h"


void BLEmeshTimeSrv::init(void* p)
{
    (void)p;
    model_id = ESP_BLE_MESH_MODEL_ID_TIME_SRV;
    BLE_MESH_MODEL_PUB_DEFINE(&pub, 2 + 10, ROLE_NODE, __COUNTER__); /*!< msg len: 2 (opcode) + 5 + 5 (C.1) */
    _model = new esp_ble_mesh_model_t(ESP_BLE_MESH_MODEL_TIME_SRV(&pub, &_data));
    pub.msg->len = 10;
}

void BLEmeshTimeSetup::init(void* srv)
{
    assert(srv);
    model_id = ESP_BLE_MESH_MODEL_ID_TIME_SETUP_SRV;
    BLE_MESH_MODEL_PUB_DEFINE(&pub, 2 + 10, ROLE_NODE, __COUNTER__); /*!< msg len: 2 (opcode) + 10 */
    _model = new esp_ble_mesh_model_t(ESP_BLE_MESH_MODEL_TIME_SETUP_SRV(&_data));

    srv_model = (BLEmeshTimeSrv*)srv;
    _data.state = srv_model->getState();
}
