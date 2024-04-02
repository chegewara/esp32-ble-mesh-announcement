#pragma once

#include "esp_ble_mesh_defs.h"


class IBLEcontext
{
private:
    esp_ble_mesh_msg_ctx_t ctx = {
        .net_idx = 0xff,
        .app_idx = 0xff,
        .send_ttl = ESP_BLE_MESH_TTL_DEFAULT
    };
public:
    IBLEcontext() {}
    ~IBLEcontext() {}

    virtual esp_ble_mesh_msg_ctx_t* getCtx() {
        return &ctx;
    }

    virtual void netKey(uint16_t key = 0) {
        ctx.net_idx = key;
    }

    virtual void appKey(uint16_t key = 0) {
        ctx.app_idx = key;
    }

    virtual void keys(uint16_t net, uint16_t app) {
        ctx.net_idx = net;
        ctx.app_idx = app;
    }

    virtual void ttl(uint8_t ttl = ESP_BLE_MESH_TTL_DEFAULT) {
        ctx.send_ttl = ttl;
    }

    virtual void address(uint16_t addr) {
        ctx.addr = addr;
    }

    /// TODO add function to get params when recv context
};
