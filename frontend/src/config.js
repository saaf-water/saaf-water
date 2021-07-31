import firebase from "firebase/app";
import "firebase/auth";

const firebaseConfig = firebase.initializeApp({
  apiKey: " AIzaSyCtzbH-dRZEYBoPrkJMa3QzUFOpPTtNZLc",
  authDomain: "https://auth-saafwater.firebaseapp.com/",
  databaseURL: "https://auth-saafwater-default-rtdb.asia-southeast1.firebasedatabase.app/",
  projectId: "auth-saafwater",
  messagingSenderId: "160131231583",
  appId: "1:160131231583:web:8ecc250a240d0d88f0d0ea",
});

export default firebaseConfig;