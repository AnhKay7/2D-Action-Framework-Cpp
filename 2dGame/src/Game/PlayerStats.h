#pragma once

// Đơn vị px, ms. Trong ngoặc: px/frame hoặc px/frame² ở 60 fps.
// Đây là số khởi đầu, chỉnh theo cảm giác của bạn.
namespace PlayerStats
{
    // ----- Chạy ngang -----
    constexpr float WALK_MAX = 0.090f;     // (1.5)  tốc độ đi bộ tối đa
    constexpr float RUN_MAX = 0.150f;     // (2.5)  giữ nút chạy
    constexpr float TOP_MAX = 0.180f;     // (3.0)  giữ nút chạy + thanh đầy
    constexpr float ACCEL = 0.000216f;  // (0.06) tăng tốc
    constexpr float FRICTION = 0.000180f;  // (0.05) thả phím trên mặt đất
    constexpr float SKID_DECEL = 0.000720f;  // (0.2)  bấm ngược chiều khi đang chạy
    constexpr float SKID_MIN_SPEED = 0.060f;     // chậm hơn mức này thì đổi hướng không hiện phanh

    // ----- Thanh tăng tốc -----
    constexpr float METER_MAX = 7.0f;      // 7 nấc
    constexpr float METER_FILL_TIME = 133.0f;    // ms để lên 1 nấc (8 frame)
    constexpr float METER_DRAIN_TIME = 400.0f;    // ms để tụt 1 nấc (24 frame)

    // ----- Nhảy -----
    constexpr float JUMP_SPEED_SLOW = 0.240f;     // (4.0)  h = 64 px = 4 ô
    constexpr float JUMP_SPEED_MID = 0.255f;     // (4.25) h = 72 px
    constexpr float JUMP_SPEED_FAST = 0.270f;     // (4.5)  h = 81 px
    constexpr float JUMP_MID_FROM = 0.060f;     // |vx| >= mức này: nhảy MID
    constexpr float JUMP_FAST_FROM = 0.150f;     // |vx| >= mức này: nhảy FAST
    constexpr float GRAVITY_HOLD = 0.00045f;   // (0.125)  đang bay lên VÀ giữ phím nhảy
    constexpr float GRAVITY = 0.001575f;  // (0.4375) còn lại
    constexpr float MAX_FALL = 0.240f;     // (4.0)

    constexpr float COYOTE_TIME = 60.0f;
    constexpr float JUMP_BUFFER_TIME = 100.0f;

    // ----- Kích thước (dạng nhỏ) -----
    constexpr float BBOX_W = 12.0f;
    constexpr float BBOX_H = 14.0f;
    constexpr float FEET_OFFSET = 8.0f;           // chân nằm dưới tâm sprite 8 px (ô 16x16)
    constexpr float SENSOR_SKIN = 1.0f;

    // ----- Đạp quái, bị thương -----
    constexpr float STOMP_BOUNCE = 0.21f;           // vy khi đạp trúng quái
    constexpr float HURT_INVULN_TIME = 1500.0f;     // ms bất tử sau khi bị thương
    constexpr int   STOMP_POINTS[] = { 100, 200, 400, 800, 1000, 2000, 4000, 8000 };
    constexpr int   STOMP_POINTS_COUNT = sizeof(STOMP_POINTS) / sizeof(STOMP_POINTS[0]);
}

#define PLAYER_START_X 40.0f
#define PLAYER_START_Y 100.0f