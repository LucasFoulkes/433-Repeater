const firebase = require("firebase");

var firabeseConfig = {
  apiKey: "AIzaSyBTKvFNDLdZmG9-hlkZiLdIUmR6k4lWW5U",
  authDomain: "rustic-restaurant.firebaseapp.com",
  databaseURL: "https://rustic-restaurant-default-rtdb.firebaseio.com",
  projectId: "rustic-restaurant",
  storageBucket: "rustic-restaurant.appspot.com",
  messagingSenderId: "771438659999",
  appId: "1:771438659999:web:d78efbe44b294a222455aa",
  measurementId: "G-NGPNFLK9RG",
};

firebase.initializeApp(firebaseConfig);

let database = firebase.database();

database.ref("/rustic/test").set(obj, function (error) {
  if (error) {
    console.log("Failed with error: " + error);
  } else {
    console.log("success");
  }
});
