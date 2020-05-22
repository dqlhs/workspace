/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2019. All rights reserved.
 * Description: Remote Control related data structure definitions.
 * Author: songchuan
 * Create: 2019-04-06
 */

#ifndef HWREMOTECTRLEVENT_H
#define HWREMOTECTRLEVENT_H

#include <stdint.h>

/* Remote Control header */
using HwRemoteCtrlHeader = struct RemoteCtrlHeader {
    uint16_t version : 3;
    uint16_t dFlag : 1;
    uint16_t wFlag : 1;
    uint16_t wr1Flag : 1;
    uint16_t wr2Flag : 1;
    uint16_t rFlag : 2;
    uint16_t inputCategory : 7;
    uint16_t len;
    uint8_t sessionId;
    uint8_t reserveData;
    uint8_t windowsId;
    uint8_t windowsStat;
    uint8_t windowsReserved1;
    uint8_t windowsReserved2;
};

using HwRemoteCtrlEventDirection = enum RemoteCtrlEventDirection {
    DIRECTION_FROM_SOURCE,
    DIRECTION_FROM_SINK,
    DIRECTION_UNKNOWN
};

/* HID data type */
using HwHidDataType = enum HidDataType {
    HID_REPORT,
    HID_REPORT_DESCRIPTOR,
    HID_INVALID_DATA,
};

const uint32_t BIT0 = 0x00000001;
const uint32_t BIT1 = 0x00000002;
const uint32_t BIT2 = 0x00000004;
const uint32_t BIT3 = 0x00000008;
const uint32_t BIT4 = 0x00000010;
const uint32_t BIT5 = 0x00000020;
const uint32_t BIT6 = 0x00000040;
const uint32_t BIT7 = 0x00000080;

const uint8_t REMOTECTRL_HW_VERSION = 0;
const uint8_t REMOTECTRL_HEAD_FLAG_D = 0;  // 0: from source to sink; 1: from sink to source

const uint8_t REMOTECTRL_INPUT_CATEGORY_GENERIC = 0;
const uint8_t REMOTECTRL_INPUT_CATEGORY_HIDC = 1;
const uint8_t REMOTECTRL_INPUT_CATEGORY_INPUT = 2;
const uint8_t REMOTECTRL_INPUT_CATEGORY_VIRTUALKEY = 3;
const uint8_t REMOTECTRL_INPUT_CATEGORY_MESSAGE = 4;

const uint8_t REMOTECTRL_GENERIC_TOUCH_DOWN = 0;
const uint8_t REMOTECTRL_GENERIC_TOUCH_UP = 1;
const uint8_t REMOTECTRL_GENERIC_TOUCH_MOVE = 2;
const uint8_t REMOTECTRL_GENERIC_KEY_DOWN = 3;
const uint8_t REMOTECTRL_GENERIC_KEY_UP = 4;
const uint8_t REMOTECTRL_GENERIC_ZOOM = 5;
const uint8_t REMOTECTRL_GENERIC_SCROLL_VERTICAL = 6;
const uint8_t REMOTECTRL_GENERIC_SCROLL_HORIZONTAL = 7;
const uint8_t REMOTECTRL_GENERIC_ROTATE = 8;
const uint8_t REMOTECTRL_GENERIC_MOUSE_DOWN = 9;
const uint8_t REMOTECTRL_GENERIC_MOUSE_UP = 10;
const uint8_t REMOTECTRL_GENERIC_MOUSE_MOVE = 11;
const uint8_t REMOTECTRL_GENERIC_WHEEL_VERTICAL = 12;
const uint8_t REMOTECTRL_GENERIC_WHEEL_HORIZONTAL = 13;

const uint8_t REMOTECTRL_VENDOR_INPUT_CONTENT = 0;
const uint8_t REMOTECTRL_VENDOR_INPUT_FOCUS = 1;
const uint8_t REMOTECTRL_VENDOR_VIRTUALKEY_BACK = 2;
const uint8_t REMOTECTRL_VENDOR_VIRTUALKEY_HOME = 3;
const uint8_t REMOTECTRL_VENDOR_VIRTUALKEY_RECENT_APP = 4;
const uint8_t REMOTECTRL_VENDOR_VIRTUALKEY_QUICK_SETTING = 5;
const uint8_t REMOTECTRL_VENDOR_MESSAGE_EXTEND = 6;
const uint8_t REMOTECTRL_VENDOR_MESSAGE_RESERVED = 7;

const uint8_t REMOTECTRL_MAX_TOUCH_INPUTS = 10;
/* as the total buffer size is 512 in phone side, we set 472 as the max length */
const uint16_t  REMOTECTRL_MAX_CONTENT_LEN = 472;

/* MetaState */
#if 0
namespace RemoteCtrlMetaState {
const uint32_t CAPS_MASK = 0x00100000;
const uint32_t ALT_LEFT_MASK = 0x00000010;
const uint32_t ALT_RIGHT_MASK = 0x00000020;
const uint32_t CTR_LEFT_MASK = 0x00002000;
const uint32_t CTR_RIGHT_MASK = 0x00004000;
const uint32_t SHIFT_LEFT_MASK = 0x00000040;
const uint32_t SHIFT_RIGHT_MASK = 0x00000080;
const uint32_t NUM_LOCK_MASK = 0x00200000;
const uint32_t SCROLL_LOCK_MASK = 0x00400000;
}
#endif

/* Touch event details */
using HwRemoteCtrlTouchEventType = enum RemoteCtrlTouchEventType {
    REMOTECTRL_TOUCH_DOWN,
    REMOTECTRL_TOUCH_UP,
    REMOTECTRL_TOUCH_MOVE
};

using HwRemoteCtrlTouchEventDetails = struct RemoteCtrlTouchEventDetails {
    HwRemoteCtrlTouchEventType type;                  // touch event type
    uint8_t numPointers;                              // Number of pointers of a multi-touch motion event.
    // When set to 0x01, it indicates a single-touch motion event
    uint8_t pointerId[REMOTECTRL_MAX_TOUCH_INPUTS];   // The identification number of this pointer.
    // The value lies in [0,1,бн]
    double coordinateX[REMOTECTRL_MAX_TOUCH_INPUTS];  // X-coordinate for touch down event
    double coordinateY[REMOTECTRL_MAX_TOUCH_INPUTS];  // Y-coordinate for touch down event
};

/* Mouse event details */
using HwRemoteCtrlMouseEventType = enum RemoteCtrlMouseEventType {
    REMOTECTRL_MOUSE_DOWN,
    REMOTECTRL_MOUSE_UP,
    REMOTECTRL_MOUSE_MOVE
};

using HwRemoteCtrlMouseEventDetails = struct RemoteCtrlMouseEventDetails {
    HwRemoteCtrlMouseEventType type;  // mouse event type
    uint8_t mouseStyle;               // 0:left button,1:middle button,2:right button
    double coordinateX;               // X-coordinate for mouse down event
    double coordinateY;               // Y-coordinate for mouse down event
};

/* Wheel details */
using HwRemoteCtrlWheelEventType = enum RemoteCtrlWheelEventType {
    REMOTECTRL_WHEEL_VERTICAL,
    REMOTECTRL_WHEEL_HORIZONTAL
};

/* Wheel distance */
using HwRemoteCtrlWheelInd = struct RemoteCtrlWheelInd {
    uint8_t direction : 1;   // Scroll Direction Indication bit;
    // 0b0:Scrolling to the right/Scrolling down, 0b1:Scrolling to the left/Scrolling up
    uint8_t scrollUnit : 2;  // Scroll Unit Indication bits;
    // 0b00:the unit is a pixel, 0b01:the unit is a mouse notch, 0b10-0b11:Reserved
};

using HwRemoteCtrlWheelEventDetails = struct RemoteCtrlWheelEventDetails {
    HwRemoteCtrlWheelEventType type;  // wheel direction
    HwRemoteCtrlWheelInd wheelInd;    // wheel Indication
    uint16_t wheelDis;                // wheel distance
    double coordinateX;               // X-coordinate when wheel button down
    double coordinateY;               // Y-coordinate when wheel button down
};

/* key board event details */
using HwRemoteCtrlKeyEventType = enum RemoteCtrlKeyEventType {
    REMOTECTRL_KEY_DOWN,
    REMOTECTRL_KEY_UP,
};

using HwRemoteCtrlKeyEventDetails = struct RemoteCtrlKeyEventDetails {
    HwRemoteCtrlKeyEventType type;  // key event type
    uint8_t reserved;
    uint16_t keyCode1;   // the first key code
    uint16_t keyCode2;   // the second key code
    uint32_t metaState;  // whether meta keys is pressed
};

/* Zoom event details */
using HwRemoteCtrlZoomEventDetails = struct RemoteCtrlZoomEventDetails {
    double coordinateX;            // The reference X-coordinate for the zoom operation
    double coordinateY;            // The reference Y-coordinate for the zoom operation
    uint8_t numTimesZoomInt;       // The unsigned integer portion of the number of times to zoom
    uint8_t numTimesZoomFraction;  // The fraction portion of the number of times to zoom.
    // The unit of the fractional part shall be 1/256,
    // and the sign of the fractional part is always positive
};

/* scroll event details */
using HwRemoteCtrlScrollEventType = enum RemoteCtrlScrollEventType {
    REMOTECTRL_SCROLL_VERTICAL,
    REMOTECTRL_SCROLL_HORIZONTAL
};

using HwRemoteCtrlScrollEventDetails = struct RemoteCtrlScrollEventDetails {
    HwRemoteCtrlScrollEventType type;  // scroll event type
    int16_t numPixelsScrolled;         // Number of pixels scrolled with respect to
    // the negotiated display resolution
    // For vertical scroll, a negative number
    // indicates to scroll up; a positive number
    // indicates to scroll down
    // For horizontal scroll, a negative number
    // indicates to scroll right;a positive number
    // indicates to scroll left
};

/* Rotate event Details */
using HwRemoteCtrlRotateEventDetails = struct RemoteCtrlRotateEventDetails {
    int8_t numRotateInt;        // The signed integer portion of the amount to rotate in units in radians.
    // A negative number indicates to rotate clockwise;
    // a positive number indicates to rotate counter-clockwise
    uint8_t numRotateFraction;  // The fraction portion of the amount to rotate in units of radians.
    // The unit of the fractional part shall be 1/256,
    // and the sign of the fractional part is always positive
};

/* Input event Details */
using HwRemoteCtrlInputEventType = enum RemoteCtrlInputEventType {
    REMOTECTRL_INPUT_CONTENT,
    REMOTECTRL_INPUT_FOCUS
};

using HwRemoteCtrlContentEvent = struct RemoteCtrlContentEvent {
    uint16_t msgLen;                             // the input text length
    char inputText[REMOTECTRL_MAX_CONTENT_LEN];  // the input text
};

using HwRemoteCtrlFocusEvent = struct RemoteCtrlFocusEvent {
    uint8_t focusStat;  // is focus on
    double cursorX1;    // The X position of the upper-left corner of the cursor
    double cursorY1;    // The Y position of the upper-left corner of the cursor
    double cursorX2;    // The X position of the lowwer-right corner of the cursor
    double cursorY2;    // The Y position of the lowwer-right corner of the cursor
};

using HwRemoteCtrlInputEvent = union RemoteCtrlInputEvent {
    HwRemoteCtrlContentEvent contentEvt;  // content event
    HwRemoteCtrlFocusEvent focusEvt;      // focus event
};

using HwRemoteCtrlInputEventDetails = struct RemoteCtrlInputEventDetails {
    HwRemoteCtrlInputEventType inputType;
    HwRemoteCtrlInputEvent inputDetail;
};

/* Virtual key event details */
using HwRemoteCtrlVirtualKeyEventType = enum RemoteCtrlVirtualKeyEventType {
    REMOTECTRL_VIRTUALKEY_BACK,
    REMOTECTRL_VIRTUALKEY_HOME,
    REMOTECTRL_VIRTUALKEY_RECENT_APP,
    REMOTECTRL_VIRTUALKEY_QUICK_SETTING
};

using HwRemoteCtrlVirtualKeyEventDetails = struct RemoteCtrlVirtualKeyEventDetails {
    HwRemoteCtrlVirtualKeyEventType type;  // virtual key event type
    double coordinateX;                    // X-coordinate for virtual key down
    double coordinateY;                    // Y-coordinate for virtual key down
};

/* Message event Details */
using HwRemoteCtrlMessageEventType = enum RemoteCtrlMessageEventType {
    REMOTECTRL_MESSAGE_EXTEND,
    REMOTECTRL_MESSAGE_RESERVED,
    REMOTECTRL_MESSAGE_UNKNOWN
};

using HwRemoteCtrlMessageDetails = struct RemoteCtrlMessageDetails {
    HwRemoteCtrlMessageEventType type;
    uint16_t msgLen;
    uint16_t msgType;
    char message[REMOTECTRL_MAX_CONTENT_LEN];
};

/* The remote control event types */
using HwRemoteCtrlEventType = enum RemoteCtrlEventType {
    REMOTECTRL_TOUCH,
    REMOTECTRL_KEY,
    REMOTECTRL_ZOOM,
    REMOTECTRL_SCROLL,
    REMOTECTRL_ROTATE,
    REMOTECTRL_HID_KEY,
    REMOTECTRL_MOUSE,
    REMOTECTRL_INPUT,
    REMOTECTRL_WHEEL,
    REMOTECTRL_VIRTUAL_KEY,
    REMOTECTRL_MESSAGE
};

using HwRemoteCtrlEventDetails = union RemoteCtrlEventDetails {
    HwRemoteCtrlTouchEventDetails touchEvt;
    RemoteCtrlKeyEventDetails keyEvt;
    HwRemoteCtrlZoomEventDetails zoomEvt;
    HwRemoteCtrlScrollEventDetails scrollEvt;
    HwRemoteCtrlRotateEventDetails rotateEvt;
    HwRemoteCtrlMouseEventDetails mouseEvt;
    HwRemoteCtrlInputEventDetails inputEvt;
    HwRemoteCtrlWheelEventDetails wheelEvt;
    HwRemoteCtrlVirtualKeyEventDetails virtualKeyEvt;
    HwRemoteCtrlMessageDetails msgEvt;
};

using HwRemoteCtrlEvent = struct RemoteCtrlEvent {
    HwRemoteCtrlHeader header;
    HwRemoteCtrlEventType evtType;       // remote control event type
    HwRemoteCtrlEventDetails evtDetail;  // remote control event detail
    uint16_t timestamp;                  // not used now
};

#endif
