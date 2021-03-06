/*
 * Copyright (C) 2019 The LineageOS Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "FingerprintInscreenService"

#include "FingerprintInscreen.h"
#include <android-base/logging.h>
#include <hidl/HidlTransportSupport.h>
#include <fstream>

#define FP_PRESS_PATH "/sys/kernel/oppo_display/notify_fppress"
#define DIM_AMOUNT_PATH "/sys/kernel/oppo_display/dim_alpha"

namespace vendor {
namespace lineage {
namespace biometrics {
namespace fingerprint {
namespace inscreen {
namespace V1_0 {
namespace implementation {

/*
 * Write value to path and close file.
 */
template <typename T>
static void set(const std::string& path, const T& value) {
    std::ofstream file(path);
    file << value;
}

template <typename T>
static T get(const std::string& path, const T& def) {
    std::ifstream file(path);
    T result;

    file >> result;
    return file.fail() ? def : result;
}

FingerprintInscreen::FingerprintInscreen() {
    this->mVendorFpService = IBiometricsFingerprint::getService();
}

Return<void> FingerprintInscreen::onStartEnroll() {
    return Void();
}

Return<void> FingerprintInscreen::onFinishEnroll() {
    return Void();
}

Return<void> FingerprintInscreen::onPress() {
    set(FP_PRESS_PATH, 1);

    return Void();
}

Return<void> FingerprintInscreen::onRelease() {
    set(FP_PRESS_PATH, 0);

    return Void();
}

Return<void> FingerprintInscreen::onShowFODView() {
    this->mVendorFpService->setScreenState(vendor::oppo::hardware::biometrics::fingerprint::V2_1::FingerprintScreenState::FINGERPRINT_SCREEN_ON);
    return Void();
}

Return<void> FingerprintInscreen::onHideFODView() {
    this->mVendorFpService->setScreenState(vendor::oppo::hardware::biometrics::fingerprint::V2_1::FingerprintScreenState::FINGERPRINT_SCREEN_OFF);
    set(FP_PRESS_PATH, 0);

    return Void();
}

Return<bool> FingerprintInscreen::handleAcquired(int32_t acquiredInfo, int32_t vendorCode) {
    LOG(ERROR) << "acquiredInfo: " << acquiredInfo << ", vendorCode: " << vendorCode << "\n";
    return false;
}

Return<bool> FingerprintInscreen::handleError(int32_t error, int32_t vendorCode) {
    LOG(ERROR) << "error: " << error << ", vendorCode: " << vendorCode << "\n";
    return false;
}

Return<void> FingerprintInscreen::setLongPressEnabled(bool) {
    return Void();
}

Return<int32_t> FingerprintInscreen::getDimAmount(int32_t) {
    int dimAmount = get(DIM_AMOUNT_PATH, 0);
    LOG(INFO) << "dimAmount = " << dimAmount;

    return dimAmount;
}

Return<bool> FingerprintInscreen::shouldBoostBrightness() {
    return false;
}

Return<void> FingerprintInscreen::setCallback(const sp<IFingerprintInscreenCallback>& callback) {
    (void) callback;
    return Void();
}

Return<int32_t> FingerprintInscreen::getPositionX() {
    return 560;
}

Return<int32_t> FingerprintInscreen::getPositionY() {
    return 2030;
}

Return<int32_t> FingerprintInscreen::getSize() {
    return 190;
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace inscreen
}  // namespace fingerprint
}  // namespace biometrics
}  // namespace lineage
}  // namespace vendor
