char webpage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
<title>Detecting device orientation - Orientation_example - code sample</title>
</head>
<body>
<form action="config" method="get">
  <div>
    <label for="offsetL">offset Left</label>
    <input name="offsetL" id="offsetL" value="0">
  </div>
  <div>
    <label for="offsetR">offset Right</label>
    <input name="offsetR" id="offsetR" value="0">
  </div>
  <div>
    <button>Send offsets</button>
  </div>
</form>
</body>
</html>
)=====";
