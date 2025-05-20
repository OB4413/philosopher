# 🍽️ philosophers

> A concurrency and threading project simulating the Dining Philosophers problem. Part of the 1337 curriculum.

## 📌 Description

The **philosophers** project simulates the classic Dining Philosophers problem using processes or threads. The goal is to manage philosophers who alternately think, eat, and sleep while avoiding deadlocks and starvation.

## ⚙️ Project Requirements

- Create N philosopher threads/processes.
- Each philosopher alternates between thinking, eating, and sleeping.
- Use mutexes or semaphores to prevent race conditions.
- Avoid deadlocks and starvation.
- Parameters like time to die, eat, and sleep are given as arguments.
- The simulation ends when all philosophers have eaten a certain number of times or a philosopher dies.

## 📁 File Structure

```
philosophers/
├── philosophers.c
├── utils.c
├── philosopher.h
├── Makefile
└── README.md
```

## 🧪 Usage Example

```bash
$ ./philosophers 5 800 200 200
```

- Arguments:
  - Number of philosophers
  - Time to die (in milliseconds)
  - Time to eat (in milliseconds)
  - Time to sleep (in milliseconds)
  - [Optional] Number of times each philosopher must eat

## 🔧 Compilation

```bash
make
./philosophers 5 800 200 200
```

## ✅ Features

- Handles concurrency using processes or threads.
- Avoids deadlocks.
- Proper synchronization with mutexes/semaphores.
- Graceful program termination.

## 👨‍💻 Author

- GitHub: [OB4413](https://github.com/OB4413)
