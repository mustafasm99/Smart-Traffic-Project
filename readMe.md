# Smart Traffic Light System

This project is a Smart Traffic Light System that uses Python Django for the server-side application and C++ for the Arduino-based ESP32 microcontroller. The system is designed to manage traffic lights efficiently, especially for emergency vehicles.

## Features

- **Real-time Traffic Control**: The system can control traffic lights in real-time.
- **Emergency Vehicle Priority**: Emergency vehicles can send requests to change traffic lights, ensuring they reach their destination quickly.
- **Web Interface**: A Django-based web application for monitoring and controlling the traffic lights.

## Components

- **Python Django**: Used for the server-side application to handle requests and manage traffic light states.
- **C++ Arduino**: Used to program the ESP32 microcontroller that controls the traffic lights.
- **ESP32**: A microcontroller that interfaces with the traffic lights and communicates with the server.

## Installation

1. **Clone the repository**:
    ```bash
    git clone https://github.com/yourusername/smart-traffic-light.git
    cd smart-traffic-light
    ```

2. **Set up the Django server**:
    ```bash
    cd server
    pip install -r requirements.txt
    python manage.py migrate
    python manage.py runserver
    ```

3. **Upload the Arduino code to ESP32**:
    - Open the Arduino IDE.
    - Load the `traffic_light.ino` file.
    - Select the correct board and port.
    - Upload the code to the ESP32.

## Usage

1. **Start the Django server**:
    ```bash
    python manage.py runserver
    ```

2. **Access the web interface**:
    Open your web browser and go to `http://localhost:8000`.

3. **Emergency Vehicle Request**:
    - Emergency vehicles can send a request via the web interface or a dedicated API endpoint to change the traffic light.

## Contributing

Contributions are welcome! Please fork the repository and submit a pull request.

## License

This project is licensed under the MIT License.

## Contact

For any questions or suggestions, please contact [your email].
