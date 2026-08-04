#include "Task.h"
#include "Sensors/Sensor.h"
#include <stdio.h>

Task::Task(std::weak_ptr<Sensor> parentSensor) : m_parentSensor(parentSensor)
{}

bool Task::StartTask()
{
    printf("Starting Task\n");
    BaseType_t ok = xTaskCreate(&Task::PollTask, "sensor_poll", 2048, this, 5, &m_task);
    if(ok == pdPASS)
    {
        printf("Task Successfully Created");
        return true;
    }
    printf("Failed to create task");
    return false;
}

void Task::PollTask(void* param)
{
    printf("Entered Poll Task\n");

    //Static function, therefore cannot use member variables
    auto task = static_cast<Task*>(param);

    while (true) 
    {
        if(auto sensor = task->m_parentSensor.lock())
        {
            sensor->ReadSensor();
        }
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

void Task::EndTask()
{
    if (m_task != nullptr) {
        vTaskDelete(m_task);
        m_task = nullptr;
    }
}
