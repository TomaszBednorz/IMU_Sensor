#ifndef TASKS_APP_H_
#define TASKS_APP_H_

#include "main.h"


/*
 *  Functions prototypes
 */

void TasksApp_init(void);

/*
 *  Tasks
 */
void TasksApp_xsensRoutine(void* parameters);
void TasksApp_ethLoop(void* parameters);
void TasksApp_ethRx(void* parameters);

#endif /* TASKS_APP_H_ */
