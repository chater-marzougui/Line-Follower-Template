<a name="readme-top"></a>
<div align="center">

[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![MIT License][license-shield]][license-url]
[![LinkedIn][linkedin-shield]](https://www.linkedin.com/in/chater-marzougui-342125299/)
</div>


<br />
<div align="center">
    <h1 style="font-size:35px">Line Follower Template <br></h1>
    <br>
    <p style="font-size:20px" align="center">
        A comprehensive template for line follower robot competitions
    <br>
    <br>
    <a href="https://github.com/chater-marzougui/Line-Follower-Template/issues/new?labels=bug&template=bug-report---.md">Report Bug</a>
    ·
    <a href="https://github.com/chater-marzougui/Line-Follower-Template/issues/new?labels=enhancement&template=feature-request---.md">Request Feature</a>
  </p>
  <br><br>
  <a href="https://github.com/chater-marzougui/Line-Follower-Template">
  </a>
</div>


# Line Follower PID Template
<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li><a href="#about-the-project">About The Project</a></li>
    <li><a href="#features">Features</a></li>
    <li><a href="#project-overview">Project Overview</a></li>
    <li><a href="#directory-structure">Directory Structure</a></li>
    <li><a href="#components-and-functionality">Components and Functionality</a></li>
    <li><a href="#web-interface">Web Interface</a></li>
    <li><a href="#hardware-requirements">Hardware Requirements</a></li>
    <li><a href="#installation">Installation</a></li>
    <li><a href="#future-improvements">Future Improvements</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
  </ol>
</details>


<!-- ABOUT THE PROJECT -->
## About The Project
The **Line Follower Competition Project** provides a robust foundation for developing and competing in line follower robotics competitions. It includes a dynamic PID tuning system via serial communication, stabilization templates, and automated HTML-to-string conversion for ESP8266 devices.

## Features
- **Dynamic PID Tuning:** Adjust PID values in real-time for optimal performance.
- **Stabilization Templates:** Pre-built code templates to stabilize line follower robots.
- **HTML-to-String Conversion:** Python automation for converting HTML to ESP8266-compatible strings.
- **Comprehensive Setup:** Well-documented setup instructions for quick deployment.
- **Wireless Configuration:** Wi-Fi enabled interface for remote control and monitoring.
- **Real-time Feedback:** Instant visualization of sensor data and line position.
- **Modular Architecture:** Separate components for Arduino control and ESP8266 web interface.

<br/>
<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Project Overview

This template provides a complete framework to develop a line follower robot with a wireless interface for PID control configuration. The robot is driven by an Arduino microcontroller, while an ESP8266 handles web-based interaction.

The ESP8266 connects to Wi-Fi and hosts a local web interface through which users can:

- Update PID parameters (`Kp`, `Kd`, `baseSpeed`)
- View QTR sensor data and calculated line position
- Start/stop the robot remotely

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Directory Structure

```
chater-marzougui-line-follower-template/
├── readme.MD
├── Line_Follower_PID_Config_Arduino/
│   └── Line_Follower_PID_Config_Arduino.ino
├── Line_Follower_PID_Config_ESP/
│   ├── convert_html_to_espHtml.py
│   ├── index.html
│   └── Line_Follower_PID_Config_ESP.ino
└── LineFollowerTemplate/
    └── LineFollowerTemplate.ino
```

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Components and Functionality

### `LineFollowerTemplate/LineFollowerTemplate.ino`

- Implements the core PID-based line following logic.
- Uses QTR-8RC sensors for line tracking.
- Controls motors with PWM signals.
- Interfaces with ESP8266 via `SoftwareSerial`.
- Responds to `run`, `stop`, and `QTR8RC` commands.
- Includes calibration routine and support for dashed line handling.

### `Line_Follower_PID_Config_Arduino/Line_Follower_PID_Config_Arduino.ino`

- Receives PID parameters from ESP8266 in JSON.
- Sends current PID values back to ESP8266.
- Forwards QTR sensor data and calculated line position.
- Communicates with main logic using serial.

### `Line_Follower_PID_Config_ESP/Line_Follower_PID_Config_ESP.ino`

- Connects to Wi-Fi (`SSID: infinix7`, `PWD: chater123`).
- Hosts a web server and serves a configuration dashboard.
- Routes:
  - `/` - Main control page
  - `/update` - Updates PID values
  - `/getData` - Requests current PID values
  - `/getQTRData` - Polls QTR sensor data
  - `/toggleWorking` - Start/Stop robot

### `convert_html_to_espHtml.py`

- Converts `index.html` into a C++ string for embedding in the firmware.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Web Interface

- Hosted locally via MDNS (`http://pidconfig.local/`).
- Allows dynamic adjustment of:
  - `Kp`, `Kd`, and `baseSpeed`
  - Start/Stop functionality
  - QTR sensor polling
- JavaScript handles asynchronous communication with ESP8266.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Hardware Requirements

- Arduino Uno (or compatible)
- ESP8266 module (e.g., NodeMCU)
- QTR-8RC line sensor array
- Dual motor driver module
- Chassis with two motors and wheels
- Optional: side sensors, ultrasonic distance sensor

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Installation

1. **Clone this repository** to your local machine.
2. **Navigate to the project directory**.
3. **Upload the appropriate code** to your Arduino or ESP8266 device.
4. **Configure Wi-Fi settings** in the ESP8266 code.
5. **Connect the hardware** according to the pinout in the code comments.
6. **Access the web interface** via the defined MDNS address.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Future Improvements

- Add Ki to PID (integral term)
- Improve JSON parsing error handling
- Enhance UI with graphs and real-time plots
- Integrate side/distance sensors into the control loop

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Contributing

Contributions are what make the open source community such an amazing place to learn, inspire, and create. Any contributions you make are **greatly appreciated**.

If you have a suggestion that would make this better, please fork the repo and create a pull request. You can also simply open an issue with the tag "enhancement".
Don't forget to give the project a star! Thanks again!

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request
<p align="right">(<a href="#readme-top">back to top</a>)</p>

## License

This project is licensed under the **MIT License** - see the [LICENSE](LICENSE) file for details.
<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- CONTACT -->
## Contact

Chater Marzougui - [@Chater-marzougui][linkedin-url] - chater.mrezgui2002@gmail.com <br/>
<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[contributors-shield]: https://img.shields.io/github/contributors/chater-marzougui/Line-Follower-Template.svg?style=for-the-badge
[contributors-url]: https://github.com/chater-marzougui/Line-Follower-Template/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/chater-marzougui/Line-Follower-Template.svg?style=for-the-badge
[forks-url]: https://github.com/chater-marzougui/Line-Follower-Template/network/members
[stars-shield]: https://img.shields.io/github/stars/chater-marzougui/Line-Follower-Template.svg?style=for-the-badge
[stars-url]: https://github.com/chater-marzougui/Line-Follower-Template/stargazers
[issues-shield]: https://img.shields.io/github/issues/chater-marzougui/Line-Follower-Template.svg?style=for-the-badge
[issues-url]: https://github.com/chater-marzougui/Line-Follower-Template/issues
[license-shield]: https://img.shields.io/github/license/chater-marzougui/Line-Follower-Template.svg?style=for-the-badge
[license-url]: https://github.com/chater-marzougui/Line-Follower-Template/blob/master/LICENSE
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://linkedin.com/in/chater-marzougui-342125299
