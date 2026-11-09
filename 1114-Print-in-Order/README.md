LeetCode 1114 — Print in Order

Difficulty: Easy
Language: C
Topic: Multithreading, Atomic Operations, Semaphore, Thread Synchronization
Status: Accepted — 36/36 Test Cases

1. Problem Description

Three threads are responsible for executing three functions:

first();
second();
third();

However, the operating system may schedule these threads in any order.

For example:

Thread C → third()
Thread B → second()
Thread A → first()

Regardless of the scheduling order, the output must always be:

firstsecondthird

The objective is to guarantee the execution order:

first() → second() → third()

⸻

2. Initial Approach — Shared State

My first thought was to use a Semaphore to control the execution order.

However, the initial concept was closer to a shared state machine:

State	Allowed Function
0	first()
1	second()
2	third()
3	Completed

The state transition is straightforward:

        counter = 0
             │
             ▼
          first()
             │
             ▼
        counter = 1
             │
             ▼
          second()
             │
             ▼
        counter = 2
             │
             ▼
          third()
             │
             ▼
        counter = 3

Each function must wait until the shared state reaches the expected value.

This led to the first implementation using C11 Atomic Operations.

⸻

3. Solution A — Atomic Operations

Implementation

#include <stdlib.h>
#include <stdatomic.h>
typedef struct {
    atomic_int counter;
} Foo;
void printFirst();
void printSecond();
void printThird();
Foo* fooCreate() {
    Foo* obj = malloc(sizeof(Foo));
    if (obj == NULL)
        return NULL;
    atomic_init(&obj->counter, 0);
    return obj;
}
void first(Foo* obj) {
    printFirst();
    atomic_store(&obj->counter, 1);
}
void second(Foo* obj) {
    while (atomic_load(&obj->counter) != 1) {
        // Busy waiting
    }
    printSecond();
    atomic_store(&obj->counter, 2);
}
void third(Foo* obj) {
    while (atomic_load(&obj->counter) != 2) {
        // Busy waiting
    }
    printThird();
    atomic_store(&obj->counter, 3);
}
void fooFree(Foo* obj) {
    free(obj);
}

How It Works

The counter variable represents the current execution stage.

first() can execute immediately because it has no dependency.

After first() finishes, it updates the state to 1, allowing second() to proceed.

Likewise, second() updates the state to 2, allowing third() to proceed.

The critical detail is the order of operations:

printSecond();
atomic_store(&obj->counter, 2);

The state must be updated after the function completes.

Otherwise, the next thread might begin executing before the previous function finishes.

Why Use Atomic?

A normal integer is not sufficient for safe communication between threads.

For example:

int counter = 0;

If one thread writes to counter while another reads it without synchronization, the program has a data race, resulting in undefined behavior under the C memory model.

C11 provides:

#include <stdatomic.h>

Three useful operations are:

API	Purpose
atomic_init()	Initialize an atomic variable
atomic_load()	Read an atomic variable
atomic_store()	Write an atomic variable

These operations provide atomic access and defined memory-ordering behavior.

The default ordering is memory_order_seq_cst (sequential consistency).

Atomic vs. Volatile

Atomic and volatile serve different purposes.

Feature	volatile	atomic
Prevent certain compiler optimizations of accesses	Yes	Not its primary purpose
Guarantee atomic operations	No	Yes
Provide thread synchronization	No	Yes
Suitable for memory-mapped registers	Yes	Usually not
Prevent data races on shared variables	No	Yes, when all conflicting accesses are atomic

Important: volatile is not a replacement for thread synchronization.

⸻

4. Busy Waiting

The Atomic implementation uses:

while (atomic_load(&obj->counter) != 1) {
}

This is called Busy Waiting, or Spin Waiting.

The thread repeatedly checks the shared variable until the condition becomes true.

Advantages

* Simple implementation.
* No operating-system blocking API required.
* Potentially low latency for extremely short waits.

Disadvantages

* Consumes CPU time while waiting.
* May compete with the thread responsible for changing the state.
* Can waste power.
* Performance depends on thread scheduling.

The operating system can still preempt a spinning thread, but the thread does not voluntarily block.

LeetCode Result

Accepted: 36/36
Runtime: 1437 ms
Memory: 9.41 MB

The implementation passed all test cases, but the runtime was relatively high.

This motivated a second implementation using POSIX Semaphores.

⸻

5. Solution B — POSIX Semaphore

The original idea was to use Semaphore synchronization.

Instead of continuously checking a shared state, threads can wait until a permit becomes available.

For this problem, two Semaphores are sufficient.

first()
   │
   ▼
sem_post(sem1)
   │
   ▼
sem_wait(sem1)
   │
   ▼
second()
   │
   ▼
sem_post(sem2)
   │
   ▼
sem_wait(sem2)
   │
   ▼
third()

Implementation

#include <stdlib.h>
#include <semaphore.h>
typedef struct {
    sem_t sem1;
    sem_t sem2;
} Foo;
void printFirst();
void printSecond();
void printThird();
Foo* fooCreate() {
    Foo* obj = malloc(sizeof(Foo));
    if (obj == NULL)
        return NULL;
    sem_init(&obj->sem1, 0, 0);
    sem_init(&obj->sem2, 0, 0);
    return obj;
}
void first(Foo* obj) {
    printFirst();
    sem_post(&obj->sem1);
}
void second(Foo* obj) {
    sem_wait(&obj->sem1);
    printSecond();
    sem_post(&obj->sem2);
}
void third(Foo* obj) {
    sem_wait(&obj->sem2);
    printThird();
}
void fooFree(Foo* obj) {
    sem_destroy(&obj->sem1);
    sem_destroy(&obj->sem2);
    free(obj);
}

This is the version used for the performance comparison. For production code, return values from sem_init(), sem_wait(), and other synchronization APIs should also be checked.

LeetCode Result

Accepted: 36/36
Runtime: 3 ms
Memory: 9.22 MB

The observed runtime improved significantly compared with the Atomic implementation.

However, these are individual LeetCode measurements. Thread scheduling and test-environment conditions can affect the results, so the observed difference should not be interpreted as a universal performance ratio.

⸻

6. Understanding Semaphore

A Semaphore manages a number of available Permits.

The two most important operations are:

sem_wait();
sem_post();

sem_wait()

Attempts to acquire one permit.

* If the count is greater than zero, decrement it and continue.
* If the count is zero, wait until a permit becomes available.

sem_post()

Provides one permit.

* If threads are waiting, one may be allowed to continue.
* Otherwise, the available permit count increases.

Unlike a simple notification, a Semaphore can retain permits until they are consumed.

Example: Counting Semaphore

Suppose a Semaphore starts with two permits:

sem_init(&sem1, 0, 2);

Thread B executes:

sem_wait(&sem1);
sem_wait(&sem1);
sem_wait(&sem1);

The behavior is:

Operation	Available Permits	Result
Initialization	2	Ready
First wait	1	Continue
Second wait	0	Continue
Third wait	0	Block

The third sem_wait() cannot complete until another thread provides a permit.

sem_post(&sem1);

Once a permit becomes available, the waiting thread can acquire it and continue.

⸻

7. Understanding sem_init()

Function declaration:

int sem_init(
    sem_t *sem,
    int pshared,
    unsigned int value
);

Example:

sem_init(&obj->sem1, 0, 0);

Parameter	Value	Meaning
sem	&obj->sem1	Semaphore to initialize
pshared	0	Shared between threads in the same process
value	0	Initial permit count

pshared = 0

The Semaphore is shared between threads belonging to the same process.

Process A
│
├── Thread A
├── Thread B
└── Thread C
       │
       ▼
    Semaphore

This matches the LeetCode 1114 scenario.

pshared != 0

The Semaphore is configured for synchronization between different processes.

Process A                 Process B
    │                         │
    └──── Shared Memory ──────┘
                │
            Semaphore

The Semaphore must reside in shared memory accessible to the participating processes.

Setting pshared to a nonzero value does not automatically make ordinary malloc() memory shared between processes.

Initial Value

For LeetCode 1114:

sem_init(&obj->sem1, 0, 0);
sem_init(&obj->sem2, 0, 0);

Both Semaphores start without available permits.

This ensures:

* second() waits for first().
* third() waits for second().

If sem1 started with one permit, second() could proceed before first() completed.

⸻

8. Atomic vs. Semaphore

Feature	Atomic + Busy Waiting	POSIX Semaphore
Shared data	Atomic integer	Semaphore objects
Synchronization	Repeated state checks	Permit acquisition
Waiting behavior	Spinning	Blocking when no permit
CPU usage while waiting	Potentially high	Usually low
OS scheduling involvement	Normal scheduling	Blocking and wake-up
Typical use case	Very short waits	Unpredictable or longer waits
LeetCode runtime observed	1437 ms	3 ms
LeetCode result	Accepted	Accepted

Important Observation

Lower latency and lower CPU usage are not the same thing.

Spin waiting can be efficient when the wait is extremely short and CPU resources are available.

Blocking synchronization can be more efficient when the waiting duration is uncertain or relatively long.

For this particular experiment, the Semaphore implementation produced a much shorter measured runtime.

⸻

9. POSIX Semaphore and FreeRTOS

The POSIX Semaphore API is conceptually similar to the synchronization mechanisms used in FreeRTOS.

POSIX	FreeRTOS
sem_init()	xSemaphoreCreateBinary() / xSemaphoreCreateCounting()
sem_wait()	xSemaphoreTake()
sem_post()	xSemaphoreGive()
sem_destroy()	vSemaphoreDelete()

These are conceptual correspondences, not identical APIs.

One notable difference:

A POSIX Semaphore initialized with a count of 1 is still a counting semaphore. Its count can increase beyond 1 through additional sem_post() calls, up to the implementation’s limit.

A FreeRTOS Binary Semaphore, by contrast, has a maximum count of 1.

⸻

10. Platform Notes

The Atomic implementation uses C11:

#include <stdatomic.h>

The Semaphore implementation uses POSIX:

#include <semaphore.h>

The POSIX sem_init() API is supported in the Linux environment used for this LeetCode submission.

However, macOS does not support unnamed POSIX Semaphores through sem_init().

For local macOS testing, alternatives include:

* Named POSIX Semaphores using sem_open().
* pthread_mutex_t combined with pthread_cond_t.

⸻

11. What I Learned

This problem started with a simple question:

How can three threads execute in a guaranteed order when their scheduling order is unpredictable?

My initial approach was to control execution through a shared state.

That led to learning about:

1. C11 Atomic Operations — safely accessing shared variables.
2. Busy Waiting — waiting by repeatedly checking a condition.
3. POSIX Semaphores — synchronizing threads through permits.
4. Blocking vs. Spinning — understanding the tradeoff between CPU usage and waiting latency.
5. Counting Semaphores — understanding how permits are acquired and released.
6. Process Sharing — understanding the pshared parameter of sem_init().

The most important realization was that thread synchronization is not necessarily about controlling which thread runs first.

Instead, it is about controlling which thread is allowed to proceed.

A thread can start early, but it must wait until its dependencies have been satisfied.

Final Takeaway

Atomic Operations make shared-state communication safe.

Semaphores provide a mechanism for coordinating execution through permits.

Choosing between spinning and blocking requires considering latency, CPU usage, power consumption, and scheduling behavior.

A seemingly simple Easy problem became an opportunity to explore how multithreaded systems coordinate execution.

LeetCode #1114 — Accepted.