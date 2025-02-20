const char webpage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
   <style type="text/css">
      body {
        font-size: 3vw;
        color: #999999;
        background-color: #2c2c2c;
        font-family: Arial;
      }
      h1 {
        font-family: 'Arial';
        font-size: 5vw;
      }
      h2, label{
        font-family: 'Arial';
        font-size: 4vw;
      }
      h3 {
        font-family: 'Arial';
        font-size: 3vw;
      }

      input {
        font-size: 3vw;
        padding: 2%;
        background-color: #e1e1e1;
        border-color: #000000;
        width: 10vw;
      }
      .button, .altButton, .alertButton {
        font-family: Arial;
        font-weight: bold;
        border-radius: 10px;
        text-align: center;
        text-decoration: none;
        display: inline-block;
        width: 30vw;
        height: 15vw;
        font-size: 4vw;
        padding: 0px;
        color: black;
      }
      .button {
        background-color: #4CAF50;
        border: 4px outset #4CAF50;
      }
      .altButton {
        background-color: #aeaeae;
        border: 4px outset #aeaeae;
        width: 50vw;
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
            <h1>ARMM GEN2 TESTING</h1>
            <br>
            <div>
               <button class="altButton" onclick="setNextTurn('NOW')">0 ft<br>OVERRIDE</button>
               <br><br>
               <button class="button" onclick="setNextTurn('LEFT')">&lt;<br>TURN LEFT</button>
               <button class="button" onclick="setNextTurn('STRAIGHT')">^<br>STRAIGHT</button>
               <button class="button" onclick="setNextTurn('RIGHT')">&gt;<br>TURN RIGHT</button>
               <br><br>
               <button class="altButton" onclick="endTest()">ARRIVED<br>(END TEST)</button>
            </div>
            <br><br>
            <div>
              <h2>SEGMENT CONTROL</h2>
              <br>
              <label for="distInput">Distance to Next Turn (ft):</label>
              <input class="numField" type="number" id="distInput">
              <br>
              <label for="timeInput">Time to Next Turn (s):</label>
              <input class="numField" type="number" id="timeInput">
              <br><br>
            </div>
            <div>
              <h2>ONBOARD DEVICE DATA</h2>
              <h3>
                Test Status: <span id="testStatus">loading...</span><br>
                Current Status: <span id="directionStatus">loading...</span><br>
             </h3>
            </div>
            <br><br><br><br><br>
            <div>Built by Levi Tucker & Team ARMM <br> February 2025</div>
         </div>

         <script>
          // Sends instructions when a new nav button is pressed
          var distToNextValue = 0;
          var timeToNextValue = 0;
          function setNextTurn(turn) { // Navigation: 0 = clear, 1 = left, 2 = right, 3 = straight

            if(turn == "NOW") {
              distToNextValue = 0;
              timeToNextValue = 0;
            } else {
              distToNextValue = document.getElementById("distInput").value;
              timeToNextValue = document.getElementById("timeInput").value;
            }

            var xhttp = new XMLHttpRequest();
            xhttp.onreadystatechange = function() {
              if (this.readyState == 4 && this.status == 200) {
                document.getElementById("directionStatus").innerHTML = this.responseText;
              }
            };
            xhttp.open("GET", "nextDirection?turn="+turn+"&dist="+distToNextValue+"&time="+timeToNextValue, true);
            xhttp.send();
          }
          
          // Tells system to end test
          function endTest() {
            var xhttp = new XMLHttpRequest();
            xhttp.onreadystatechange = function() {
              if (this.readyState == 4 && this.status == 200) {
                document.getElementById("navState").innerHTML = this.responseText;
              }
            };
            xhttp.open("GET", "end_test?state="+navState, true);
            xhttp.send();
          }

         </script>
      </center>
   </body>
</html>
)=====";