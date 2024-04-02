#include <stdio.h>

#include "generic_on_off_models.h"
#include "ble_mesh_node.h"


static auto onOffCli = new GenericOnOffCliModel("on-off 1c");

extern "C" void app_main(void)
{
    auto device = BLEmeshNode::GetInstance();
    device->addPrimaryModel(onOffCli);
    device->init_ble();
    device->init_mesh();
    device->registerCallbacks();
    if (!esp_ble_mesh_node_is_provisioned()){
        device->enableProvisioning();
    }

    onOffCli->keys(0, 0); // hardcoded keys index for demo purpose; works with nrf mesh

    bool on_off = true;
    while (1)
    {
        if (esp_ble_mesh_node_is_provisioned()){
            if(on_off)onOffCli->turnOn(0xc000);
            else onOffCli->turnOff(0xc000);

            on_off = !on_off;
        }
        vTaskDelay(100);
    }
}

