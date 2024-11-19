<?php
include 'db_connect.php';


if (isset($_GET['temperature']) && isset($_GET['humidity'])) {
    $temperature = $_GET['temperature'];
    $humidity = $_GET['humidity'];


    $sql = "INSERT INTO dht22_readings (temperature, humidity) VALUES ($temperature, $humidity)";

    if ($conn1->query($sql) === TRUE) {
        echo "New records created successfully";
    } else {
        echo "Error: " . $sql . "<br>" . $conn1->error;
    }
} else {

    $sql = "SELECT temperature, humidity FROM dht22_readings ORDER BY timestamp DESC LIMIT 1";
    $result = $conn1->query($sql);

    if ($result->num_rows > 0) {
        $row = $result->fetch_assoc();
        echo json_encode(['temperature' => $row['temperature'], 'humidity' => $row['humidity']]);
    } else {
        echo json_encode(['temperature' => 0, 'humidity' => 0]);
    }
}

$conn1->close();
?>