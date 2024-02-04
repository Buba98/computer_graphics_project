# Computer Graphics Course Assignments 2022/2023

Welcome to the official repository for the Computer Graphics course assignments at Politecnico di Milano. This repository houses the solutions for all assignments issued during the academic year 2022/2023.

## Getting Started

### Prerequisites

Before you can run the "Sandro Run" project, you'll need to install some dependencies. For the basic dependencies, please refer to this [website](https://vulkan-tutorial.com/Development_environment).

### Building the Assignments

All assignments are based on a single Makefile. To build a specific assignment, execute the following commands:

```bash
make clean <ASSIGNMENT_FOLDER_NAME>
make shaders <ASSIGNMENT_FOLDER_NAME>
make compile <ASSIGNMENT_FOLDER_NAME>
```

### Running the Assignments

To run a specific assignment, execute the following command:

```bash
make run <ASSIGNMENT_FOLDER_NAME>
```

Alternatively, the Makefile also provides the capability to compile and run the assignments in a single command:

```bash
make all <ASSIGNMENT_FOLDER_NAME>
```

Please replace `<ASSIGNMENT_FOLDER_NAME>` with the name of the assignment folder you wish to build or run.