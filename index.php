<?php
// Retrieve the form data sent via POST method
$username = $_POST['username'];
$password = $_POST['password'];
// Check if the user's credentials are valid
if ($username === 'test' && $password === 'test') {
    echo "Login successful!";
} else {
    echo "Invalid username or password.";
}
?>

<!DOCTYPE html>
<html>
<head>
	<title>Login Form</title>
</head>
<body>
	<h1>Login Form</h1>
	<form method="post" action="">
		<label for="username">Username:</label>
		<input type="text" id="username" name="username" required><br><br>
		<label for="password">Password:</label>
		<input type="password" id="password" name="password" required><br><br>
		<input type="submit" value="Submit">
	</form>
</body>
</html>