## Description

I am currently working on ble mesh C++ library for esp32 SoCs.
Library should help people like me, with minimum knowledge about ble mesh, to create ble mesh devices quick and easy.
I hope to make library flexible, which means it will have default callbacks registered in esp-idf, which will pass events to every class object which should receive event. And user can also override default callbacks and register its own.
This project is nothing like my other repositories. I want to make it mature, fully documented and ready to use by everyone.

### Implemented
This is only demo version or maybe alpha. Only few SIG models are implemented and most likely not finished yet.


#### foundation models
- config srv        - mostly done - missing heartbeat pub/sub
- config cli        - partially
- health srv        - partially
- health cli        - partially

#### generic models
- gen on/off srv    - partially
- gen on/off cli    - ready
- gen level srv     - partially
- gen level cli     - ready
- gen loc srv       - partially
- gen loc setup srv - partially
- gen loc cli       - ready
- gen battery srv   - not tested
- gen battery cli   - not tested

#### sensor models
- 

#### time and scene models
- time srv          - partially
- time setup srv    - partially
- time cli          - missing functions
- scene srv         - partially
- scene setup srv   - partially
- scene cli         - partially

`scene models needs to be added to other models, to handle scene store/recall/delete`

#### light models
- lightness srv - partially
- lightness cli - partially
- light hsl srv - partially
- light hsl cli - partially

#### vendor models
- vendor cli/srv - partially (there is only 1 model for cli and srv)


## Example code
Thats most basic code which allows to create mesh device with generic level server model. Also to control proxy and relay features on node.
```
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
    onOffSrv->setCb(levelSrvCb);
    BLEmeshConfigSrv::proxy(1);
    BLEmeshConfigSrv::beacon(1);
    BLEmeshConfigSrv::relay(1);
}

void BLEmeshModelCallbacks::onSet(IBLEMeshModel *model, void *param)
{
    auto p = (esp_ble_mesh_generic_server_cb_param_t *)param;
    uint32_t lvl = (p->value.set.level.level + 32768) / 256;
    printf("%s callback set: %i => %lu\n", model->name(), p->value.set.level.level + 32768, lvl);
}

void BLEmeshModelCallbacks::onChange(IBLEMeshModel *model, void *param)
{
    auto p = (esp_ble_mesh_generic_server_cb_param_t *)param;
    uint32_t lvl = (p->value.state_change.level_set.level + 32768) / 256;
    printf("%s callback change: %i => %lu\n", model->name(), p->value.state_change.level_set.level, lvl);
}


```

Thats most basic code which allows to create mesh device with generic level client model.
```
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
}

```
## Limitations
- it is working only with esp-idf 5.3 (master)
- it is bluedroid library, which means it requires a lot of ram and flash, but i can run it together with wifi and mqtt without issues (node gateway test app)

## Requests
Requests, questions and suggestions are welcome.

## License and publication date
Library will be published as MIT when i have most ble mesh v1.0 models working more or less and code will be cleaned up and documented (couple months maybe).

