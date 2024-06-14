
<?php
// Database connection details
$servername = "localhost"; // Change if your database is hosted elsewhere
$username = "id22310817_geoscience"; // Replace with your database username
$password = "%Geosciences2023"; // Replace with your database password
$dbname = "id22310817_geoscienceproject_data"; // Replace with your database name

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);

// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

// Check if the POST request contains the expected data
if (isset($_POST['value1']) && isset($_POST['value2'])) {
    $value1 = intval($_POST['value1']);
    $value2 = intval($_POST['value2']);

    // Prepare and bind
    $stmt = $conn->prepare("INSERT INTO dht11 (value1, value2) VALUES (?, ?)");
    $stmt->bind_param("ii", $value1, $value2);

    // Execute the statement
    if ($stmt->execute()) {
        echo "New record created successfully";
    } else {
        echo "Error: " . $stmt->error;
    }

    // Close the statement
    $stmt->close();
} else {
    echo "Invalid data";
}

// Close the connection
$conn->close();
?>
