#pragma once
#include <vector>
#include "UIEngine/process/IProcess.h"

class ProcessManager
{
    std::vector<std::unique_ptr<IProcess>> processes;
public:
    IProcess* AddProcess(std::unique_ptr<IProcess> process) {
        processes.push_back(std::move(process));
        return processes.back().get();
    }

    void Update() {
        for (auto& p : processes) {
            p->Update();
        }
        // удалить завершённые
        processes.erase(
            std::remove_if(processes.begin(), processes.end(),
                [](const auto& p) { return p->IsFinished(); }),
            processes.end()
        );
    }
};