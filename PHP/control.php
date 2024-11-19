<?php
include 'db_connect.php';
header('Content-Type: application/json');

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $data = json_decode(file_get_contents("php://input"), true);
    $part = $data['part'] ?? '';
    $status = $data['status'] ?? null;

    if (in_array($part, ['motor', 'servodirection', 'servoaxis', 'servolift', 'servogrip'])) {
        $stmt = $conn2->prepare("UPDATE control_status SET status = ?, timestamp = CURRENT_TIMESTAMP WHERE name = ?");
        
        if ($stmt) {
            $stmt->bind_param("is", $status, $part);

            if ($stmt->execute()) {
                echo json_encode(['success' => true]);
            } else {
                echo json_encode(['success' => false, 'error' => $stmt->error]);
            }

            $stmt->close();
        } else {
            echo json_encode(['success' => false, 'error' => $conn2->error]);
        }
    } else {
        echo json_encode(['success' => false, 'error' => 'Invalid component']);
    }

    $conn2->close();
}

if (isset($_GET['get_status'])) {
    $part = $_GET['get_status'];

    if (in_array($part, ['motor', 'servodirection', 'servoaxis', 'servolift', 'servogrip'])) {
        $stmt = $conn2->prepare("SELECT status FROM control_status WHERE name = ?");
        
        if ($stmt) {
            $stmt->bind_param("s", $part);
            $stmt->execute();
            $result = $stmt->get_result();

            if ($row = $result->fetch_assoc()) {
                echo json_encode(['status' => (int)$row['status']]);
            } else {
                echo json_encode(['status' => 0]);
            }

            $stmt->close();
        } else {
            echo json_encode(['success' => false, 'error' => $conn2->error]);
        }
    } else {
        echo json_encode(['success' => false, 'error' => 'Invalid component']);
    }

    $conn2->close();
}
?>
