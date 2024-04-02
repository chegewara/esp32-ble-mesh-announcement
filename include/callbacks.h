#pragma once

#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include "esp_gatt_common_api.h"
#include "esp_ble_mesh_defs.h"
#include "esp_ble_mesh_common_api.h"
#include "esp_ble_mesh_networking_api.h"
#include "esp_ble_mesh_provisioning_api.h"
#include "esp_ble_mesh_config_model_api.h"
#include "esp_ble_mesh_generic_model_api.h"
#include "esp_ble_mesh_local_data_operation_api.h"
#include "esp_ble_mesh_lighting_model_api.h"
#include "esp_ble_mesh_time_scene_model_api.h"
#include "esp_ble_mesh_sensor_model_api.h"

void _default_ble_mesh_provisioning_cb(esp_ble_mesh_prov_cb_event_t event, esp_ble_mesh_prov_cb_param_t *params);

void _default_ble_mesh_config_server_cb(esp_ble_mesh_cfg_server_cb_event_t event, esp_ble_mesh_cfg_server_cb_param_t *params);
void _default_ble_mesh_config_client_cb(esp_ble_mesh_cfg_client_cb_event_t event, esp_ble_mesh_cfg_client_cb_param_t *params);

void _default_ble_mesh_generic_server_cb(esp_ble_mesh_generic_server_cb_event_t event, esp_ble_mesh_generic_server_cb_param_t *params);
void _default_ble_mesh_generic_client_cb(esp_ble_mesh_generic_client_cb_event_t event, esp_ble_mesh_generic_client_cb_param_t *params);

void _default_ble_mesh_lighting_server_cb(esp_ble_mesh_lighting_server_cb_event_t event, esp_ble_mesh_lighting_server_cb_param_t *params);
void _default_ble_mesh_lighting_client_cb(esp_ble_mesh_light_client_cb_event_t event, esp_ble_mesh_light_client_cb_param_t *params);

void _default_ble_mesh_time_scene_client_cb(esp_ble_mesh_time_scene_client_cb_event_t event, esp_ble_mesh_time_scene_client_cb_param_t *params);
void _default_ble_mesh_time_scene_server_cb(esp_ble_mesh_time_scene_server_cb_event_t event, esp_ble_mesh_time_scene_server_cb_param_t *params);

void _default_ble_mesh_sensor_client_cb(esp_ble_mesh_sensor_client_cb_event_t event, esp_ble_mesh_sensor_client_cb_param_t *params);
void _default_ble_mesh_sensor_server_cb(esp_ble_mesh_sensor_server_cb_event_t event, esp_ble_mesh_sensor_server_cb_param_t *params);

void _default_ble_mesh_custom_model_cb(esp_ble_mesh_model_cb_event_t event, esp_ble_mesh_model_cb_param_t *params);
