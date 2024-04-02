#include "ble_mesh_config_srv.h"


esp_ble_mesh_cfg_srv_t BLEmeshConfigSrv::config_server = {
    .net_transmit = ESP_BLE_MESH_TRANSMIT(2, 20),
    .relay = ESP_BLE_MESH_RELAY_NOT_SUPPORTED,
    .relay_retransmit = ESP_BLE_MESH_TRANSMIT(2, 20),
    .beacon = ESP_BLE_MESH_BEACON_DISABLED,
    .gatt_proxy = ESP_BLE_MESH_GATT_PROXY_NOT_SUPPORTED,
    .friend_state = ESP_BLE_MESH_FRIEND_DISABLED,
    .default_ttl = 7,
};
