#include "esp_ble_mesh_defs.h"
#include "esp_ble_mesh_common_api.h"
#include "esp_ble_mesh_networking_api.h"
#include "esp_ble_mesh_provisioning_api.h"
#include "esp_ble_mesh_config_model_api.h"
#include "esp_ble_mesh_generic_model_api.h"
#include "esp_ble_mesh_local_data_operation_api.h"

#include <string.h>
#include "mesh/buf.h"

void add_elements(esp_ble_mesh_elem_t elements[], esp_ble_mesh_elem_t* model, int count)
{
    memcpy(&elements[count], model, sizeof(esp_ble_mesh_elem_t));
}

struct net_buf_simple* get_net_buf(int size)
{
    struct net_buf_simple* buf = NET_BUF_SIMPLE(1);
    return buf;
}

void copy_memcpy(esp_ble_mesh_model_t* models, esp_ble_mesh_model_t* model)
{
    memcpy(models, model, sizeof(esp_ble_mesh_model_t));
}
