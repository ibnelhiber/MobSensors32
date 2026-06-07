

#ifndef MAIN_TASKS_TASK_H_
#define MAIN_TASKS_TASK_H_

#include "Config.h"
#include <memory>

class Sensor;

class Task
{
private:
    TaskHandle_t m_task{nullptr};
    std::weak_ptr<Sensor> m_parentSensor;

public:
    Task(std::weak_ptr<Sensor> parentSensor);
    virtual ~Task() = default;
    static void PollTask(void* param);
    bool StartTask();    
    void EndTask();
    bool PauseTask();
};


#endif

