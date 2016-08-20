var xhrRequest = function (url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
};


function getWeather() {
   var url = "http://www.wticalumni.com/DHK/Wxsnoop/weather.json";


 // Send request to Weather
  xhrRequest(url, 'GET',
    function(responseText) {

      // responseText contains a JSON object with weather info

      var json = JSON.parse(responseText);

      // Temperature in Kelvin requires adjustment
      //var temperature = Math.round(json.main.temp - 273.15);

      //Outside Temp
      var temperature = json.site.properties.outdoorTemperature.values[0].value;
      console.log(temperature);
      
      //Inside Temp
      var temperature_inside = json.site.properties.indoorTemperature.values[0].value;
      console.log(temperature_inside);


      // Assemble dictionary using our keys
      var dictionary = {
        "0": temperature,
        "1": temperature_inside
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
