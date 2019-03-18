char webpage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
<title>Config Gyrobot</title>
</head>
<body>
<form action="config" method="get">
  <div>
    <label for="offsetL">offset Left</label>
    <input name="offsetL" id="offsetL" value="$1$">
  </div>
  <div>
    <label for="offsetR">offset Right</label>
    <input name="offsetR" id="offsetR" value="$2$">
  </div>
  <div>
    <button>Set offsets</button>
  </div>
</form>
<br>
<a href="/">back</a> 
</body>
</html>
)=====";
