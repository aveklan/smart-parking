var distance;
var status;
var ts;

function callAPI() {
  var myHeaders = new Headers();
  var requestOptions = {
    method: "GET",
    headers: myHeaders,
  };

  var latestData;

  fetch(
    "https://2asbp47dg8.execute-api.eu-west-3.amazonaws.com/dev",
    requestOptions
  )
    .then((response) => response.text())
    .then((result) => storeData(JSON.parse(result)));
}

function storeData(data) {
  console.log(data);
  spot0 = data[0]["spot0"];
  spot1 = data[0]["spot1"];
  spot2 = data[0]["spot2"];
  spot3 = data[0]["spot3"];

  parkedCars = data[0]["parkedCars"];
  totalCarNumbers = data[0]["totalCarNumbers"];
  last_update = data[0]["timestamp"];

  if (spot0) {
    console.log("spot0");
    document.getElementById("spot0").classList.remove("free");
    document.getElementById("spot0").classList.add("filled");
  } else document.getElementById("spot0").classList.add("free");
  if (spot1) {
    document.getElementById("spot1").classList.add("filled");
    document.getElementById("spot1").classList.remove("free");
  } else document.getElementById("spot1").classList.add("free");
  if (spot2) {
    document.getElementById("spot2").classList.add("filled");
    document.getElementById("spot2").classList.remove("free");
  } else document.getElementById("spot2").classList.add("free");
  if (spot3) {
    document.getElementById("spot3").classList.add("filled");
    document.getElementById("spot3").classList.remove("free");
  } else document.getElementById("spot3").classList.add("free");
  document.getElementById("spot4").classList.add("free");
  document.getElementById("spot5").classList.add("free");

  document.getElementById("free_spots").textContent =
    "Free spots: " + (6 - parkedCars);

  document.getElementById("last_24h_cars").textContent =
    "Number of cars parked in the last 24 hours: " + totalCarNumbers;
  document.getElementById("last_update").textContent =
    "Last update: " + last_update;
}

function init() {
  callAPI();
}
