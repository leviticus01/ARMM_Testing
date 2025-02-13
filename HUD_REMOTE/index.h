const char webpage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
   <style type="text/css">
      body {
        background-color: #9fd9f9;
        font-family: Arial;
      }
      h1 {
        font-family: 'Arial';
        font-size: 5vw;
      }
      h2 {
        font-family: 'Arial';
        font-size: 4vw;
      }
      h3 {
        font-family: 'Arial';
        font-size: 3vw;
      }
      .button, .clearButton, .alertButton {
        font-family: Arial;
        font-weight: bold;
        border-radius: 10px;
        text-align: center;
        text-decoration: none;
        display: inline-block;
        width: 30vw;
        height: 20vw;
        font-size: 4vw;
        padding: 0px;
        color: black;
      }
      .button {
        background-color: #4CAF50;
        border: 4px outset #4CAF50;
      }
      .clearButton {
        background-color: #aeaeae;
        border: 4px outset #aeaeae;
      }
      .alertButton {
        background-color: #e28441;
        border: 4px outset #e28441;
      }
      .slider {
        -webkit-appearance: none;
        width: 80vw;
        height: 4vh;
        border-radius: 20px;
        background: #ffffff;
        border-radius: 5px;
        -webkit-transition: .2s;
        transition: opacity .2s;
      }
      .slider::-webkit-slider-thumb {
        -webkit-appearance: none;
        appearance: none;
        width: 10vw;
        height: 5vh;
        border-radius: 10px; 
        background: #065cd6;
        cursor: pointer;
      }
      .slider::-moz-range-thumb {
        width: 10vw;
        height: 5vh;
        border-radius: 10px; 
        background: #065cd6;
        cursor: pointer;
      }

   </style>
   <body>
      <center>
         <div>
            <h1>ARMM HUD TEST REMOTE</h1>
            <br>
            <div>
               <h2>NAVIGATION CONTROL</h2>
               <button class="button" onclick="updateNav(1)">&lt;<br>TURN LEFT</button>
               <button class="button" onclick="updateNav(3)">^<br>STRAIGHT</button>
               <button class="button" onclick="updateNav(2)">&gt;<br>TURN RIGHT</button>
               <br><br>
               <button class="clearButton" onclick="updateNav(0)">CLEAR<br>NAV DATA</button>
            </div>
            <br><br>
            <div>
              <h2>BLIND SPOT CONTROL</h2>
              <button class="alertButton" onclick="updateBlindSpot(1)">&lt;<br>ALERT LEFT</button>
              <button class="alertButton" onclick="updateBlindSpot(2)">&gt;<br>ALERT RIGHT</button>
              <br><br>
              <button class="clearButton" onclick="updateBlindSpot(0)">CLEAR<br>BLIND SPOT</button>
            </div>
            <br><br>
            <div class = "slidecontainer">
              <h2><label for="speedRange">SPEED CONTROL</label></h2>
              <input type="range" min="0" max="20" value="10" class="slider" id="speedRange">
            </div>
            <br><br>
            <div>
              <h2>ONBOARD DEVICE DATA</h2>
              <h3>
                Navigation State: <span id="navState">loading...</span><br>
                Blind Spot State: <span id="blindSpotState">loading...</span><br>
                Speed State: <span id="speedState">loading...</span><br>
             </h3>
            </div>
            <br><br><br><br><br>
            <div>Built by Levi Tucker & Team ARMM <br> February 2025</div>
         </div>

         <script>
          // Updates Nav when button pressed
          function updateNav(navState) { // Navigation: 0 = clear, 1 = left, 2 = right, 3 = straight
            var xhttp = new XMLHttpRequest();
            xhttp.onreadystatechange = function() {
              if (this.readyState == 4 && this.status == 200) {
                document.getElementById("navState").innerHTML = this.responseText;
              }
            };
            xhttp.open("GET", "set_nav?state="+navState, true);
            xhttp.send();
          }
          
          // Updates BlindSpot when button pressed
          function updateBlindSpot(blindSpotState) { // BlindSpot: 0 = clear, 1 = left, 2 = right
            var xhttp = new XMLHttpRequest();
            xhttp.onreadystatechange = function() {
              if (this.readyState == 4 && this.status == 200) {
                document.getElementById("blindSpotState").innerHTML = this.responseText;
              }
            };
            xhttp.open("GET", "set_blindspot?state="+blindSpotState, true);
            xhttp.send();
          }
          
          // Change slider value
          var slider = document.getElementById("speedRange");
          var speedDisplay = document.getElementById("speedState");
          slider.oninput = function() {
            //speedDisplay.innerHTML = this.value + " mph";
            var xhttp = new XMLHttpRequest();
            xhttp.onreadystatechange = function() {
              if (this.readyState == 4 && this.status == 200) {
                document.getElementById("speedState").innerHTML = this.responseText;
              }
            };
            xhttp.open("GET", "set_speed?state="+this.value, true);
            xhttp.send();
          }



            setInterval(function() 
            {
              getData();
            }, 1000); 
            function getData() {
              var xhttp = new XMLHttpRequest();
              xhttp.onreadystatechange = function() {
                if (this.readyState == 4 && this.status == 200) {
                  document.getElementById("buttonStatus").innerHTML =
                  this.responseText;
                }
              };
              xhttp.open("GET", "buttonread", true);
              xhttp.send();
            }
         </script>
      </center>
   </body>
</html>
)=====";