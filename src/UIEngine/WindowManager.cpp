#include "UIEngine/WindowManager.h"
ProcessManager WindowManager::processManager = ProcessManager();
std::unique_ptr<IWindow> WindowManager::nullwin = std::make_unique<NullWindow>();
ScreenLayout WindowManager::screen = ScreenLayout(
    std::make_unique<PaneLayout>(nullwin.get()),
    [](std::unique_ptr<IProcess> p) { WindowManager::processManager.AddProcess(std::move(p)); }
);
std::vector<std::unique_ptr<IWindow>> WindowManager::windows = std::vector<std::unique_ptr<IWindow>>();
std::map<std::string, IWindow*> WindowManager::bookmarks = std::map<std::string, IWindow*>();
Bind WindowManager::binds;
Bind WindowManager::tempBinds;

