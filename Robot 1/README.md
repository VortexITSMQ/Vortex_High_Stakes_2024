# **Robot 1**

## **Project Overview**

Robot 1 is a robotics project that consists of two main functionalities: autonomous programming and manual drive control. The project structure includes source code for both systems as well as relevant documentation.

---

## **Folder Structure**

The project is divided into three main folders:
1. **Autonomous Programming**: Contains the code and logic responsible for the robot's autonomous functions, such as object detection, path planning, and obstacle avoidance.
2. **Drive**: Contains the code that handles manual control of the robot, including drive logic and remote control configurations.
3. **Documentation**: Contains notes, instructions, and other relevant documents for understanding and running the project.

---

## **Version Control Overview**

This project uses a hybrid approach for version control, combining both version numbers and dates to track the evolution of prototypes. Each version is named using the format `v[MAJOR].[MINOR]_[YYYY_MM_DD]`.

- **Version Format:**
  - **Major Version (v[MAJOR])**: Significant changes or new features that affect the overall functionality.
  - **Minor Version (.[MINOR])**: Minor updates such as bug fixes, performance improvements, or small features.
  - **Date (YYYY_MM_DD)**: Indicates the release date of the version to track the progression over time.

---

## **Changelog**

### **v1.0_2024_10_08**

- **Initial Release**
  - Added basic autonomous system functionality.
  - Implemented manual drive control for the robot.
  - Set up communication protocols between the drive and autonomous subsystems.
  - Initial tests for both systems.

### **v1.1_2024_10_15**

- **Bug Fixes and Improvements**
  - Fixed an issue with sensor data synchronization in the autonomous mode.
  - Improved response time of manual drive control.
  - Enhanced battery management in autonomous mode to improve efficiency.

### **v2.0_2024_11_01**

- **Major Update: New Features Added**
  - Introduced a new collision avoidance algorithm in the autonomous system.
  - Improved the precision of the drive control for smoother operation.
  - Added a logging system in the **Documentation** folder to monitor key performance metrics.
  
---
## **Future Plans**
- Improve obstacle detection accuracy.
- Integrate machine learning for path planning in autonomous mode.
- Develop a more user-friendly manual control interface.
