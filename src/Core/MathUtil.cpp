

#include "MathUtil.h"

namespace ShapeGame
{
    float AngleDifference(float Current, float Target)
    {
        float Diff = fmodf(Target - Current, 360.f);
        Diff = fmodf(Diff + 180.f, 360.f) - 180.f;
        return Diff;
    }

    float AngleLerp(float Current, float Target, float Speed, float DeltaTime)
    {
        float Remaining = AngleDifference(Current, Target);

        if (fabsf(Remaining) <= FLT_EPSILON)
        {
            return Target;
        }

        float Step = copysignf(fminf(fabsf(Remaining), Speed * DeltaTime), Remaining);
        return fmodf(Current + Step + 360.f, 360.f);
    }

    size_t GetSegmentVerticesWithThickness(
        std::vector<Vector2>& vertices,
        const glm::vec2& start,
        const glm::vec2& end,
        float thickness,
        int capSegCount)
    {
        size_t count = 0;
        const float halfThick = thickness * 0.5f;
        const glm::vec2 dir = glm::normalize(end - start);
        const glm::vec2 perpendicular = {-dir.y * halfThick, dir.x * halfThick};

        // ===== 线段主体（Triangle Strip） =====
        vertices.push_back(V2_Convert(start - perpendicular)); // 上起点
        count++;
        vertices.push_back(V2_Convert(start + perpendicular)); // 下起点
        count++;
        vertices.push_back(V2_Convert(end - perpendicular)); // 上终点
        count++;
        vertices.push_back(V2_Convert(end + perpendicular)); // 下终点
        count++;

        if (capSegCount > 0)
        { // ===== 端点圆形（Triangle Fan） =====
            auto addCircle = [&](const glm::vec2& center, float angleOffset)
            {
                vertices.push_back(V2_Convert(center)); // 中心点
                count++;
                for (int i = 0; i <= capSegCount; ++i)
                {
                    float angle = -(angleOffset + (i * PI) / capSegCount) + PI * 0.5f;
                    glm::vec2 offset = {cosf(angle) * halfThick, sinf(angle) * halfThick};
                    vertices.push_back(V2_Convert(center + offset));
                    count++;
                }
            };

            // 起点圆形（偏移PI/2保证连接方向正确）
            addCircle(start, atan2f(-perpendicular.x, -perpendicular.y));
            // 终点圆形（保持与线段方向一致）
            addCircle(end, atan2f(perpendicular.x, perpendicular.y));
        }
        return count;
    }
} // namespace ShapeGame