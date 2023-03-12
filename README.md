<h1 align=center>SystemMonitor</h1>

# Description
This is system monitor for Linux. It can show general information about system usage by processes and threads that are running currently. To obtain nessesary information it uses **procfs**, so it can be used with other systems that support that technology.

# Usage
Output modes:
* p - process info
* t - thread info

Sorting modes (in descending order):
* m - memory consupiton
* c - cpu utilization

You can enter signaling mode by pressing k.
You can get help message by pressing h key.

# Documentation
Code documentation can be generated using **Doxygen**

# Building from source
To build this project you'll need **slint** library. If you already got it you can build the project as any other cmake project.
