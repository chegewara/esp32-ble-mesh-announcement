#include <stdio.h>

#include "esp_err.h"
#include "generic_on_off_models.h"
#include "ble_mesh_node.h"

#include "driver/gpio.h"
#define LED_PIN GPIO_NUM_32

class BLEmeshModelCallbacks : public BLEmeshModelCb
{
    void onChange(IBLEMeshModel *model, void *param) override;
};

static auto onOffSrv = new GenericOnOffSrvModel("on-off 1s");
static auto onOffSrvCb = new BLEmeshModelCallbacks();

static void example_add_elements_models()
{
    auto device = BLEmeshNode::GetInstance();
    auto elem1 = new BLEmeshElement();
    auto primary = device->rootElement();

    {
        primary->addModel(onOffSrv);
        onOffSrv->setCb(onOffSrvCb);
    }
}

extern "C" void app_main(void)
{
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(LED_PIN, 1);
    vTaskDelay(100);
    gpio_set_level(LED_PIN, 0);

    auto device = BLEmeshNode::GetInstance();
    example_add_elements_models();

    device->init_ble();
    device->init_mesh();
    device->registerCallbacks();
    if (!esp_ble_mesh_node_is_provisioned()){
        device->enableProvisioning();
    }

    onOffSrv->keys(0, 0); // hardcoded keys index for demo purpose; works with nrf mesh

    BLEmeshConfigSrv::proxy(1);
    BLEmeshConfigSrv::beacon(1);
    BLEmeshConfigSrv::relay(1);
}

void BLEmeshModelCallbacks::onChange(IBLEMeshModel *model, void *param)
{
    auto p = (esp_ble_mesh_generic_server_cb_param_t *)param;
    printf("%s => callback change: %d\n", model->name(), p->value.state_change.onoff_set.onoff);
    gpio_set_level(LED_PIN, p->value.state_change.onoff_set.onoff);
}
