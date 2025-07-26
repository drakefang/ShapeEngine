//
// Created by yuyzc on 2025/7/26.
//


#pragma once


namespace ShapeEngine
{
    struct GameTime
    {
        /**
         * @brief 从上一帧到当前帧所经过的真实时间（秒）。
         * 这个值不受时间缩放和暂停的影响。
         */
        float UnscaledDeltaTime = 0.0f;

        /**
         * @brief 从上一帧到当前帧所经过的游戏时间（秒）。
         * 这个值会受到全局时间缩放和暂停的影响。
         * 这是绝大多数游戏逻辑（如移动、动画）应该使用的时间。
         * (Calculated as: UnscaledDeltaTime * GlobalTimeDilation)
         */
        float DeltaTime = 0.0f;

        /**
         * @brief 从游戏开始到现在的总真实时间（秒）。
         */
        double TotalUnscaledTime = 0.0;

        /**
         * @brief 从游戏开始到现在的总游戏时间（秒）。
         */
        double TotalTime = 0.0;
    };
}