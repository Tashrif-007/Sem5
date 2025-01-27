<?php
    $host = "localhost";
    $username = "root";
    $password = "";
    $dbName = "test";

    $conn = new mysqli($host, $username, $password, $dbName);

    if($conn->connect_error) {
        die("Connection failed: ". $conn->connect_error);
    }
?>
