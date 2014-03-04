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

#ifndef OZONE_WAYLAND_INPUT_TOUCHSCREEN_H_
#define OZONE_WAYLAND_INPUT_TOUCHSCREEN_H_

#include "ozone/wayland/display.h"
#include "ui/gfx/point.h"

namespace ozonewayland {

class EventConverterOzoneWayland;
class WaylandWindow;

class WaylandTouchscreen {
 public:
  WaylandTouchscreen();
  ~WaylandTouchscreen();

  void OnSeatCapabilities(wl_seat *seat, uint32_t caps);

 private:
  static void OnTouchDown(
      void *data,
      struct wl_touch *wl_touch,
      uint32_t serial,
      uint32_t time,
      struct wl_surface *surface,
      int32_t id,
      wl_fixed_t x,
      wl_fixed_t y);

  static void OnTouchUp(
      void *data,
      struct wl_touch *wl_touch,
      uint32_t serial,
      uint32_t time,
      int32_t id);

  static void OnTouchMotion(
      void *data,
      struct wl_touch *wl_touch,
      uint32_t time,
      int32_t id,
      wl_fixed_t x,
      wl_fixed_t y);

  static void OnTouchFrame(
      void *data,
      struct wl_touch *wl_touch);

  static void OnTouchCancel(
      void *data,
      struct wl_touch *wl_touch);

  EventConverterOzoneWayland* dispatcher_;
  gfx::Point pointer_position_;

  DISALLOW_COPY_AND_ASSIGN(WaylandTouchscreen);
};

}  // namespace ozonewayland

#endif  // OZONE_WAYLAND_INPUT_POINTER_H_
