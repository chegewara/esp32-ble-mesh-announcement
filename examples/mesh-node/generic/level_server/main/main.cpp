#include <stdio.h>

#include "esp_err.h"
#include "generic_level_models.h"
#include "ble_mesh_node.h"

class BLEmeshModelCallbacks : public BLEmeshModelCb
{
    void onSet(IBLEMeshModel *model, void *param) override;
    void onChange(IBLEMeshModel *model, void *param) override;
};

static auto levelSrv = new GenericLevelSrvModel("level 1s");
static auto levelSrvCb = new BLEmeshModelCallbacks();

extern "C" void app_main(void)
{
    auto device = BLEmeshNode::GetInstance();
    device->addPrimaryModel(levelSrv);
    device->init_ble();
    device->init_mesh();
    device->registerCallbacks();
    device->enableProvisioning();
    levelSrv->setCb(levelSrvCb);

    levelSrv->keys(0, 0); // hardcoded keys index for demo purpose; works with nrf mesh

    BLEmeshConfigSrv::proxy(1);
    BLEmeshConfigSrv::beacon(1);
    BLEmeshConfigSrv::relay(1);
}

void BLEmeshModelCallbacks::onSet(IBLEMeshModel *model, void *param)
{
    auto p = (esp_ble_mesh_generic_server_cb_param_t *)param;
    uint32_t lvl = (p->value.set.level.level + 32768);
    printf("%s callback set: %i => %lu\n", model->name(), p->value.set.level.level + 32768, lvl);
}

void BLEmeshModelCallbacks::onChange(IBLEMeshModel *model, void *param)
{
    auto p = (esp_ble_mesh_generic_server_cb_param_t *)param;
    uint32_t lvl = (p->value.state_change.level_set.level + 32768);
    printf("%s callback change: %i => %lu\n", model->name(), p->value.state_change.level_set.level, lvl);
}
