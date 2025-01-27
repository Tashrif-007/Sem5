<?php
    session_start();

    if(isset($_SESSION['user_id']) || isset($_COOKIE["user_id"])) {
        header("Location: index.php");
        exit;
    }

    require("loginaction.php");
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Login</title>
</head>
<body>
    <div>
        <form action="login.php" method="POST">
            <label>Username:</label>
            <input type="text" id="username" name="username" required>
            <br>
            <label>Password:</label>
            <input type="password" id="password" name="password" required>
            <br>
            <label>
                <input type="checkbox" id="rememberMe" name="remember">Remember me
            </label>

            <input type="submit" value="Login">
        </form>
        <?php if(isset($error)) echo "<p style='color: red;'>$error</p>";?>
        <p>Don't have an account? <a href="register.php">Register here</a>.</p>
    </div>
</body>
</html>
