#include <stdio.h>

#include "esp_err.h"
#include "generic_level_models.h"
#include "ble_mesh_node.h"

static auto levelCli = new GenericLevelCliModel("level 1c");

extern "C" void app_main(void)
{
    auto device = BLEmeshNode::GetInstance();
    device->addPrimaryModel(levelCli);
    device->init_ble();
    device->init_mesh();
    device->registerCallbacks();
    device->enableProvisioning();

    levelCli->keys(0, 0); // hardcoded keys index for demo purpose; works with nrf mesh

    uint16_t level = 0;
    while (1)
    {
        if (esp_ble_mesh_node_is_provisioned()){
            levelCli->level(0xc000, level);

            level += 1000;
        }
        vTaskDelay(100);
    }
}
