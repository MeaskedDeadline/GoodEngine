#pragma once
#include "Engine.h"
#include "App.h"

int main()
{
    Good::EngineConfig config;
    config.appName      = "GoodEngine";
    config.windowWidth  = 1280;
    config.windowHeight = 720;

    if (!Good::Engine::Get().Initialize(config))
        return -1;

    Good::Engine::Get().Run(Good::CreateApp());
    return 0;
}