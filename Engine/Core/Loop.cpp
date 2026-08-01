#include "Loop.h"
#include "App.h"
#include "Engine.h"
#include "Time.h"
#include "Log.h"
#include "Platform/Window.h"
#include "Input/Input.h"
#include "UI/UIInput.h"

namespace Good
{

    void Loop::Run(Scope<App> app)
    {
        GOOD_LOG_INFO("Loop", "Starting: '{}'", app->GetName());
        app->OnStart();

        while (Engine::Get().IsRunning() && Window::Get().IsOpen())
        {
            // 1. СНАЧАЛА BeginFrame — сохраняем previous состояние
            Input::Get().BeginFrame();
            UIInput::Get().BeginFrame();

            // 2. ПОТОМ PollEvents — приходят новые события
            //    OnKey записывает в current, а previous уже сохранено
            Window::Get().PollEvents();

            // 3. Тик времени
            Time::Tick();

            // 4. Fixed update
            m_Accumulator += Time::UnscaledDelta();
            uint32 steps = 0;
            while (m_Accumulator >= Time::FixedDeltaTime() && steps < MaxFixedSteps)
            {
                app->OnFixedUpdate();
                m_Accumulator -= Time::FixedDeltaTime();
                ++steps;
            }

            // 5. Логика
            app->OnUpdate();

            // 6. Отрисовка
            Window::Get().BeginFrame();
            app->OnRender();
            app->OnUI();
            Window::Get().EndFrame();

            // 7. Конец кадра
            Input::Get().EndFrame();
            UIInput::Get().EndFrame();
        }

        app->OnStop();
        GOOD_LOG_INFO("Loop", "Ended. Frames: {}", Time::FrameCount());
    }

} // namespace Good