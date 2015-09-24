/*
   Copyright (c) 2014, The Linux Foundation. All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.
    * Neither the name of The Linux Foundation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
   WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
   ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
   BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
   BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
   OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
   IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdlib.h>
#include <stdio.h>

#include "vendor_init.h"
#include "property_service.h"
#include "log.h"
#include "util.h"

#include "init_msm.h"

void init_msm_properties(unsigned long msm_id, unsigned long msm_ver, char *board_type)
{
    char platform[PROP_VALUE_MAX];
    char radio[PROP_VALUE_MAX];
    char sku[PROP_VALUE_MAX];
    char carrier[PROP_VALUE_MAX];
    char device[PROP_VALUE_MAX];
    char devicename[PROP_VALUE_MAX];
    FILE *fp;
    int rc;

    UNUSED(msm_id);
    UNUSED(msm_ver);
    UNUSED(board_type);

    rc = property_get("ro.board.platform", platform);
    if (!rc || !ISMATCH(platform, ANDROID_TARGET))
        return;

    property_get("ro.boot.radio", radio);
    property_get("ro.boot.hardware.sku", sku);
    property_get("ro.boot.carrier", carrier);

    if (ISMATCH(carrier, "retgb") || ISMATCH(carrier, "reteu")) {
        // These are single SIM XT1562 devices
        property_set("ro.product.model", "XT1562");
        property_set("ro.product.name", "lux_reteu");
        property_set("ro.product.device", "lux");
        property_set("ro.build.description", "lux_reteu-user 5.1.1 LPD23.118-10 15 release-keys");
        property_set("ro.build.fingerprint", "motorola/lux_reteu/lux:5.1.1/LPD23.118-10/15:user/release-keys");
        property_set("ro.build.product", "lux");
        property_set("ro.mot.build.customerid", "reteu");
        property_set("ro.gsm.data_retry_config", "");
        property_set("persist.radio.mot_ecc_custid", "emea");
        property_set("persist.radio.mot_ecc_enabled", "1");
        property_set("persist.radio.process_sups_ind", "0");
    }
    else if (ISMATCH(sku, "XT1562") || ISMATCH(radio, "0x4")) {
        setMsim();
        property_set("ro.product.model", "XT1562");
        property_set("ro.product.name", "lux_retasia_ds");
        property_set("ro.product.device", "lux_uds");
        property_set("ro.build.description", "lux_retasia_ds-user 5.1.1 LPD23.118-10 14 release-keys");
        property_set("ro.build.fingerprint", "motorola/lux_retasia_ds/lux_uds:5.1.1/LPD23.118-10/14:user/release-keys");
        property_set("ro.build.product", "lux_uds");
        property_set("ro.mot.build.customerid", "retasiaall");
        property_set("ro.gsm.data_retry_config", "default_randomization=2000,max_retries=infinite,1000,1000,80000,125000,485000,905000");
        property_set("persist.radio.mot_ecc_custid", "emea");
        property_set("persist.radio.mot_ecc_enabled", "1");
        property_set("persist.radio.process_sups_ind", "0");
        property_set("persist.radio.plmn_name_cmp", "1");
    }
    else if (ISMATCH(carrier, "retbr")) {
        // These are dual SIM XT1563 devices
        setMsim();
        property_set("ro.product.model", "XT1563");
        property_set("ro.product.name", "lux_retla_ds");
        property_set("ro.product.device", "lux_uds");
        property_set("ro.build.description", "lux_retla_ds-user 5.1.1 LPD23.118-6.1 2 release-keys");
        property_set("ro.build.fingerprint", "motorola/lux_retla_ds/lux_uds:5.1.1/LPD23.118-6.1/2:user/release-keys");
        property_set("ro.build.product", "lux_uds");
        property_set("ro.mot.build.customerid", "retla");
        property_set("ro.gsm.data_retry_config", "default_randomization=2000,max_retries=infinite,1000,1000,80000,125000,485000,905000");
        property_set("persist.radio.mot_ecc_enabled", "1");
        property_set("persist.radio.process_sups_ind", "1");
        property_set("persist.radio.plmn_name_cmp", "1");
    }
    else if (ISMATCH(sku, "XT1563") || ISMATCH(radio, "0x8")) {
        property_set("ro.product.model", "XT1563");
        property_set("ro.product.name", "lux_retca");
        property_set("ro.product.device", "lux");
        property_set("ro.build.description", "lux_retca-user 5.1.1 LPD23.118-10 19 release-keys");
        property_set("ro.build.fingerprint", "motorola/lux_retca/lux:5.1.1/LPD23.118-10/19:user/release-keys");
        property_set("ro.build.product", "lux");
        property_set("ro.mot.build.customerid", "retca");
        property_set("ro.gsm.data_retry_config", "");
        property_set("persist.radio.process_sups_ind", "1");
    }

    property_get("ro.product.device", device);
    strlcpy(devicename, device, sizeof(devicename));
    INFO("Found radio id: %s data %s setting build properties for %s device\n", radio, sku, devicename);
}

void setMsim()
{
    property_set("persist.radio.force_get_pref", "1");
    property_set("persist.radio.multisim.config", "dsds");
}
