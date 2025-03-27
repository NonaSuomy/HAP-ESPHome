#pragma once

#include <esphome/core/defines.h>
#ifdef USE_LOCK
#include <esphome/core/application.h>
#include <hap.h>
#include <hap_apple_servs.h>
#include <hap_apple_chars.h>
#include <map>
#ifdef USE_HOMEKEY
#include <nvs.h>
#include "const.h"
#include <HK_HomeKit.h>
#include <hkAuthContext.h>
#include <esphome/components/pn532/pn532.h>
#include <esphome/core/base_automation.h>
#include "automation.h"
#endif

namespace esphome {
namespace homekit {

enum class AInfo {
    NAME,
    MODEL,
    SN,
    MANUFACTURER,
    FW_REV
};

enum class HKFinish {
    TAN = 0,
    GOLD,
    SILVER,
    BLACK
};

class LockEntity {
private:
    std::map<AInfo, const char*> accessory_info = {
        {AInfo::NAME, NULL}, 
        {AInfo::MODEL, "HAP-LOCK"}, 
        {AInfo::SN, NULL}, 
        {AInfo::MANUFACTURER, "rednblkx"}, 
        {AInfo::FW_REV, "0.1"}
    };
    
    lock::Lock* ptrToLock;
    std::unique_ptr<hap_tlv8_val_t> hkFinishTlvData;

#ifdef USE_HOMEKEY
    static nvs_handle savedHKdata;
    static readerData_t readerData;
    static pn532::PN532* nfc_ctx;
#endif

public:
    LockEntity(lock::Lock* lockPtr);
    void setup();
    void setInfo(std::map<AInfo, const char*> info);
    void set_hk_hw_finish(HKFinish color);

protected:
    static int acc_identify(hap_acc_t* ha);
    static int lock_write(hap_write_data_t write_data[], int count, void* serv_priv, void* write_priv);
    void on_lock_update(lock::Lock* obj);
};

} // namespace homekit
} // namespace esphome
#endif
