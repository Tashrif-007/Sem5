<?php
session_start();
require 'db.php'; 

if (!isset($_SESSION['user_id']) && !isset($_COOKIE['user_id'])) {
    header("Location: login.php");
    exit;
}

$user_id = $_SESSION['user_id'] ?? $_COOKIE['user_id'];

$sql = "SELECT username FROM userinfo WHERE id = $user_id";
$result = $conn->query($sql);

if ($result && $result->num_rows === 1) {
    $row = $result->fetch_assoc();
    $username = $row['username'];
} else {
    header("Location: login.php");
    exit;
}
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Welcome</title>
</head>
<body>
    <h1>Welcome, <?php echo htmlspecialchars($username); ?>!</h1>
    <a href="logout.php">Logout</a>
</body>
</html>
