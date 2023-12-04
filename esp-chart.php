
text/x-generic esp-chart.php ( HTML document, ASCII text )
<?php

$servername = "localhost";

// REPLACE with your Database name
$dbname = "gdfzsrmy_WPXKD";
// REPLACE with Database user
$username = "gdfzsrmy_WPXKD";
// REPLACE with Database user password
$password = "!(w#xJLA[%J}KD?6U";

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
} 

$sql = "SELECT id, value1, value2, reading_time FROM dht11 order by reading_time desc limit 40";

$result = $conn->query($sql);

while ($data = $result->fetch_assoc()){
    $sensor_data[] = $data;
}

$readings_time = array_column($sensor_data, 'reading_time');

$readings_time = array_map(function($reading) {
    return gmdate("Y-m-d H:i:s", strtotime($reading));
}, $readings_time);

$value1 = json_encode(array_reverse(array_column($sensor_data, 'value1')), JSON_NUMERIC_CHECK);
$value2 = json_encode(array_reverse(array_column($sensor_data, 'value2')), JSON_NUMERIC_CHECK);
$reading_time = json_encode(array_reverse($readings_time), JSON_NUMERIC_CHECK);

/*echo $value1;
echo $value2;
echo $value3;
echo $reading_time;*/

$result->free();
$conn->close();
?>

<!DOCTYPE html>
<html>
  <head>
    <title>UC Humidity Station</title>
  </head>
<meta name="viewport" content="width=device-width, initial-scale=1">
  <script src="https://code.highcharts.com/highcharts.js"></script>
  <style>
    body {
    background-image: url(/univcbr.jpeg);
    background-size: cover;
    background-position: center;
    background-position-x: -130px;
    background-attachment: fixed;
      min-width: 310px;
    	max-width: 1280px;
    	height: 500px;
      margin: 0 auto;
    }
    h2 {
      font-family: Arial;
      font-size: 2.5rem;
      text-align: center;

    }
    .container {
      min-width: 310px;
      max-width: 1280px;
      height: 500px;
      margin: 50px auto;
    }
    .container_outside {
        width:150px;
        height:100px;
        }
  </style>
  <body>
    <h2>UC Humidity Station</h2>
    <div id="chart-temperature" class="container"></div>
    <div id="chart-humidity" class="container"></div>
<script>

var value1 = <?php echo $value1; ?>;
var value2 = <?php echo $value2; ?>;
//var value3 = <?php echo $value3; ?>;
var reading_time = <?php echo $reading_time; ?>;

var chartT = new Highcharts.Chart({
  chart:{ renderTo : 'chart-temperature', backgroundColor: 'rgba(255, 255, 255, 0.9)' },
  title: { text: 'Humidity Sensor 1' },
  series: [{
    showInLegend: false,
    data: value1
  }],
  plotOptions: {
    line: { animation: false,
      dataLabels: { enabled: true }
    },
    series: { color: '#059e8a' }
  },
  xAxis: { 
    type: 'datetime',
    categories: reading_time
  },
  yAxis: {
    title: { text: 'Humidity (%)' }
    //title: { text: 'Temperature (Fahrenheit)' }
  },
  credits: { enabled: false }
});

var chartH = new Highcharts.Chart({
  chart:{ renderTo:'chart-humidity', backgroundColor: 'rgba(255, 255, 255, 0.9)' },
  title: { text: 'Humidity Sensor 2' },
  series: [{
    showInLegend: false,
    data: value2
  }],
  plotOptions: {
    line: { animation: false,
      dataLabels: { enabled: true }
    }
  },
  xAxis: {
    type: 'datetime',
    //dateTimeLabelFormats: { second: '%H:%M:%S' },
    categories: reading_time
  },
  yAxis: {
    title: { text: 'Humidity (%)' }
  },
  credits: { enabled: false }
});

</script>
    <div class="container_outside"></div>
</body>
</html>
