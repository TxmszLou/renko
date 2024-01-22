#pragma once

#include "test.h"

namespace test {
    class TestClearColor : public Test
    {
    public:
        TestClearColor();
        ~TestClearColor();

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;

    private:
        // the color to clear screen with
        float m_ClearColor[4];
    };
}