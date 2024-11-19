<?php
$servername = "localhost";
$username = "root";
$password = ""; 


$dbname1 = "dht22"; 
$conn1 = new mysqli($servername, $username, $password, $dbname1);

if ($conn1->connect_error) {
    die("Connection failed to dht22: " . $conn1->connect_error);
}

$dbname2 = "id22114529_for_grip"; 
$conn2 = new mysqli($servername, $username, $password, $dbname2);

if ($conn2->connect_error) {
    die("Connection failed to id22114529_for_grip: " . $conn2->connect_error);
}
?>