var xhrRequest = function (url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
};

function RoundString(StringIn) {
  var float_from_string = parseFloat(StringIn);
         // console.log("float_max_wind = ", float_max_wind);
      
      var Rounded_float =  Math.round(float_from_string);
          //console.log("Rounded_max_wind = ", Rounded_max_wind);
      
      return Rounded_float.toString();
          //console.log("char max_wind = ", max_wind);
  
}

function getWeather() {
   var url = "http://www.wticalumni.com/DHK/Wxsnoop/weather.json";


 // Send request to Weather
  xhrRequest(url, 'GET',
    function(responseText) {

      // responseText contains a JSON object with weather info

      var json = JSON.parse(responseText);
      
      // console.log("ResponseText:" , responseText);
      
       //Time
      var time_from_json = json.time;
      
           console.log("time from json = ", time_from_json);
      
      
      //Outside Temp
      var temperature_from_json = json.agent.properties.outdoorTemperature.values[0].value;
      var temperature = RoundString(temperature_from_json);

          console.log("outside temp from json = ", temperature, " rounded to ", temperature);
      
      //Inside Temp
      var temperature_inside_from_json = json.agent.properties.indoorTemperature.values[0].value;
      var temperature_inside = RoundString(temperature_inside_from_json);

          console.log("inside temp from json = ", temperature_inside_from_json, " rounded to ", temperature_inside);
      
      //Daily Rain
      var daily_rain = json.agent.properties.dayRain.values[0].value;
          console.log("daily rain = ", daily_rain);
      
      //Max Wind
      var max_wind_from_json = json.agent.properties.windSpeed.maxValueToday.values[0].value;
     
      var max_wind = RoundString(max_wind_from_json);
         console.log("max wind from json = ", max_wind_from_json, " rounded to ", max_wind);
      
     
      
      // Assemble dictionary using our keys
      var dictionary = {
        "TEMP_OUTSIDE": temperature,
        "TEMP_INSIDE": temperature_inside,
        "RAIN_DAILY": daily_rain,
        "WIND_MAX": max_wind,
        "TIME": time_from_json
      };


      // Send to Pebble
      Pebble.sendAppMessage(dictionary,
        function(e) {
          console.log("Weather info sent to Pebble successfully!");
        },
        function(e) {
          console.log("Error sending weather info to Pebble!");
        }
      );
    }    // End Function(responseText)
  );     // End xhrRequest
}        // End getWeather

// Listen for when the watchface is opened
Pebble.addEventListener('ready',
  function(e) {
    console.log("PebbleKit JS ready!");

    // Get the initial weather
    getWeather();
  }
);

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
  function(e) {
    console.log("AppMessage received!");
    getWeather();
  }
);
