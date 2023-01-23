var config = {
  apiKey: "AIzaSyBTKvFNDLdZmG9-hlkZiLdIUmR6k4lWW5U",
  authDomain: "rustic-restaurant.firebaseapp.com",
  databaseURL: "https://rustic-restaurant-default-rtdb.firebaseio.com",
  projectId: "rustic-restaurant",
  storageBucket: "rustic-restaurant.appspot.com",
  messagingSenderId: "771438659999",
  appId: "1:771438659999:web:d78efbe44b294a222455aa",
  measurementId: "G-NGPNFLK9RG",
};

var tablesId = [];
var tables = [];

firebase.initializeApp(config);

var database = firebase.database();

var target = document.querySelector(".tables");

database.ref("/rustic").on(
  "child_added",
  function (snapshot) {
    color = "";
    var tmp = {
      timestamp: String(snapshot.key),
      table: String(snapshot.val().slice(0, 20)),
      message: String(snapshot.val().slice(20)),
    };
    if (tablesId.includes(tmp.table) === false) {
      tables.push(tmp);
      tablesId.push(tmp.table);

      var p = document.createElement("p");
      p.setAttribute("id", tmp.table);
      p.innerHTML = tmp.message;
      var div = document.createElement("div");
      div.appendChild(p);
      var fragment = document.createDocumentFragment();
      fragment.appendChild(div);
      target.appendChild(fragment);
    }
    switch (tmp.message) {
      case "0001":
        tmp.message = "order";
        color = "orange";
        break;
      case "0010":
        tmp.message = "call";
        color = "red";
        break;
      case "0100":
        tmp.message = "pay";
        color = "green";
        break;
      case "1000":
        color = "grey";
        tmp.message = "cancel";
        break;
      default:
        color = "grey";
        tmp.message = String(tmp.message);
    }
    tables.find((record) => record.table === String(tmp.table)).message =
      tmp.message;
    document.getElementById(tmp.table).innerHTML = String(
      tmp.message
    ).toUpperCase();
    document.getElementById(tmp.table).style.backgroundColor = color;
  },
  function (errorObject) {
    console.log("The read failed: " + errorObject.code);
  }
);
