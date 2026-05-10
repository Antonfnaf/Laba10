#include "UIEngine/WindowManager.h"

ScreenLayout WindowManager::screen = ScreenLayout(std::make_unique<PaneLayout>(std::move(std::make_unique<NullWindow>())));
Bind WindowManager::binds;
Bind WindowManager::tempBinds;