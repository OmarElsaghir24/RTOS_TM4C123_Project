# RTOS_TM4C123_Project
Developed an RTOS project using the TM4C123GH6PM microcontroller that goes over the core concepts of RTOS using FreeRTOS. Implemented priority scheduling, a shell interface, queues, semaphores, fault handlers, and task management.

![RTOS Circuit](https://github.com/user-attachments/assets/7781ff3c-c5ad-40c6-bd82-8853315bbc9e)

# Features
* Priority-Based Scheduling: each task is assigned a priority, where level 0 is the highest priority and level 5 is the lowest. Includes a total of 10 tasks.
* Shell Interface: provides user access to managing tasks. Includes deleting, monitoring cpu and memory usage, restarting, and suspending.

# Shell Interface Features
* `Reboot`: reboots the TM4C123 microcontroller.
* `Delete Yellow`: deletes yellow led task.
* `Green Delete`: deletes green led task.
* `Preempt on`: resumes all tasks by restarting the scheduler (enables preemptive scheduling).
* `Preempt off`: represents a similar implementation of cooperative scheduling. Suspends all running tasks, which pauses the scheduler.
* `Memory Info`: displays memory and cpu usage of each task currently running. Also shows free heap, shell stack remaining, and number of tasks running.

![Memory Information](https://github.com/user-attachments/assets/ab5ebf28-e825-4e6a-b89b-0e25acbe1ee9)

* Stack Monitor Task:

![Task Stack ](https://github.com/user-attachments/assets/aa74b35f-bb90-49e0-8f2b-acd2f7d6ac60)
