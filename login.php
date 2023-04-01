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