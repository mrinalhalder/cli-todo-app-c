# 📝 CLI To-Do List Application in C

A robust, terminal-based To-Do List manager written completely in pure C. This project demonstrates core low-level programming concepts, dynamic memory layouts, and secure memory operations.

---

## 🚀 Key Features

* **Data Persistence:** Automatically saves and reads tasks from a local `todo.txt` file.
* **Dynamic Memory Management:** Uses dynamic arrays (`malloc`/`realloc`) that scale dynamically as tasks grow.
* **Custom Array Shifting Algorithm:** Implements an optimized bubble-shifting logic to delete items safely without breaking array boundaries.
* **Robust Input Validation:** Prevents application crashes from infinite loops caused by invalid characters or overflow integers using custom buffer cleansers.
* **Edge-case Protection:** Free from memory leaks and out-of-bound errors.

---

## 🛠️ Tech Stack & Concepts Used

* **Language:** C (C11 Standard)
* **Concepts:** Custom structures (`typedef struct`), Dynamic Arrays, File I/O (Streaming pointers), Buffer Clearing, Boolean error status handling.

---

## 💻 How to Run This Project

### Prerequisites
Make sure you have a C compiler (like GCC) installed.

### Compilation
Open your terminal/command prompt in the project folder and run:
```bash
gcc main.c -o todo
./todo

