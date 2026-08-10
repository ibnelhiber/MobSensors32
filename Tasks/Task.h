

#ifndef MAIN_TASKS_TASK_H_
#define MAIN_TASKS_TASK_H_

#include "Buses/Adaptation/Adapt.h"
#include <memory>

class Sensor;

class Task
{
private:
    TaskHandle_t m_task{nullptr};
    std::weak_ptr<Sensor> m_parentSensor;
    int m_delayPeriod;

public:
    Task(std::weak_ptr<Sensor> parentSensor);
    virtual ~Task() = default;
    // Needs to be static to be passed to xTaskCreate()
    static void PollTask(void* param);
    bool StartTask(const int delayPeriod);    
    void EndTask();
    bool PauseTask();
};


#endif

