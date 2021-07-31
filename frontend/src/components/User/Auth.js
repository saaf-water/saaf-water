import React, { useEffect, useState } from "react";
import firebaseConfig from "../../config.js";

/* The code in Auth.js is for authentication integration to SaafWater webapp*/
export const AuthContext = React.createContext();

export const AuthProvider = ({ children }) => {
  const [loading, setLoading] = useState(true);
  const [currentUser, setCurrentUser] = useState(null);
  useEffect(() => {
    firebaseConfig.auth().onAuthStateChanged((user) => {
      setCurrentUser(user);
      setLoading(false);
    });
  }, []);
  if (loading) {
    return(
    <div className="font-roboto align-center justify-center flex">Hang on, Please</div>
    );
     
  }
  return (
    <AuthContext.Provider value={{ currentUser }}>
      {children}
    </AuthContext.Provider>
  );
};