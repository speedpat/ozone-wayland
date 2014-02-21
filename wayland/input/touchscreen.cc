// Copyright (c) 2014 Noser Engineering AG. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
// * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
// * Neither the name of Intel Corporation nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "ozone/wayland/input/touchscreen.h"

#include <linux/input.h>

#include "ozone/ui/events/event_converter_ozone_wayland.h"
#include "ozone/wayland/input/cursor.h"
#include "ozone/wayland/input_device.h"
#include "ozone/wayland/window.h"
#include "ui/events/event.h"

namespace ozonewayland {

WaylandTouchscreen::WaylandTouchscreen()
  : dispatcher_(NULL),
    pointer_position_(0, 0) {
}

WaylandTouchscreen::~WaylandTouchscreen() {
}

void WaylandTouchscreen::OnSeatCapabilities(wl_seat *seat, uint32_t caps) {
  static const struct wl_touch_listener kInputTouchListener = {
    WaylandTouchscreen::OnTouchDown,
    WaylandTouchscreen::OnTouchUp,
    WaylandTouchscreen::OnTouchMotion,
    WaylandTouchscreen::OnTouchFrame,
    WaylandTouchscreen::OnTouchCancel,
  };

  dispatcher_ = EventConverterOzoneWayland::GetInstance();

  if ((caps & WL_SEAT_CAPABILITY_TOUCH)) {
    wl_touch* input_touch = wl_seat_get_touch(seat);
    wl_touch_set_user_data(input_touch, this);
    wl_touch_add_listener(input_touch, &kInputTouchListener, this);
  }
}

void WaylandTouchscreen::OnTouchDown(void *data,
                                     struct wl_touch *wl_touch,
                                     uint32_t serial,
                                     uint32_t time,
                                     struct wl_surface *surface,
                                     int32_t id,
                                     wl_fixed_t x,
                                     wl_fixed_t y) {
  WaylandTouchscreen* device = static_cast<WaylandTouchscreen*>(data);
  WaylandDisplay::GetInstance()->SetSerial(serial);
  WaylandInputDevice* input = WaylandDisplay::GetInstance()->PrimaryInput();
  if (input->GetFocusWindowHandle() && input->GetGrabButton() == 0)
      input->SetGrabWindowHandle(input->GetFocusWindowHandle(), id);

  float sx = wl_fixed_to_double(x);
  float sy = wl_fixed_to_double(y);

  device->dispatcher_->Touch(ui::ET_TOUCH_PRESSED, sx, sy, id, time);
}

void WaylandTouchscreen::OnTouchUp(void *data,
                                   struct wl_touch *wl_touch,
                                   uint32_t serial,
                                   uint32_t time,
                                   int32_t id) {
  WaylandTouchscreen* device = static_cast<WaylandTouchscreen*>(data);
  WaylandDisplay::GetInstance()->SetSerial(serial);
  WaylandInputDevice* input = WaylandDisplay::GetInstance()->PrimaryInput();

  float sx = wl_fixed_to_double(x);
  float sy = wl_fixed_to_double(y);

  device->dispatcher_->Touch(ui::ET_TOUCH_RELEASED, sx, sy, id, time);

  if (input->GetGrabWindowHandle() && input->GetGrabButton() == id)
      input->SetGrabWindowHandle(0, 0);
}

void WaylandTochscreen::OnTouchMotion(void *data,
                                      struct wl_touch *wl_touch,
                                      uint32_t time,
                                      int32_t id,
                                      wl_fixed_t x,
                                      wl_fixed_t y) {
  WaylandTouchscreen* device = static_cast<WaylandTouchscreen*>(data);
  WaylandInputDevice* input = WaylandDisplay::GetInstance()->PrimaryInput();
  float sx = wl_fixed_to_double(x);
  float sy = wl_fixed_to_double(y);

  if (input->GetGrabWindowHandle() &&
      input->GetGrabWindowHandle() != input->GetFocusWindowHandle()) {
      return;
  }

  device->dispatcher_->Touch(ui::ET_TOUCH_MODED, sx, sy, id, time);
}

void WaylandTouchscreen::OnTouchFrame(void *data,
                                      struct wl_touch *wl_touch) {
  // todo what's this
}

void WaylandTouchscreen::OnTouchCancel(void *data,
                                       struct wl_touch *wl_touch) {
  WaylandTouchscreen* device = static_cast<WaylandTouchscreen*>(data);
  WaylandInputDevice* input = WaylandDisplay::GetInstance()->PrimaryInput();


  device->dispatcher_->Touch(ui::ET_TOUCH_CANCELLED, 0, 0, 0, 0);

  if (input->GetGrabWindowHandle() && input->GetGrabButton() != 0)
       input->SetGrabWindowHandle(0, 0);
}

}  // namespace ozonewayland
