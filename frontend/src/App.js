import React from 'react'
import { BrowserRouter, Route, Switch } from 'react-router-dom'
import Charts from "./components/Pages/Charts/Charts"
import Dashboard from './components/Pages/Home/Dashboard'
//import Map from './components/Pages/Map/Map'
import Navbar from './components/Navbar'

/* The App.js file takes care of the routing of the webapp*/

// import LogIn from "./components/Login"
// import SignUp from "./components/SignUp"
// import { AuthProvider } from "./components/Auth"
// import ForgotPassword from "./components/ForgotPassword"

function App() {
  return (
    //<AuthProvider>

    <BrowserRouter>
      <div className="font-roboto bg-gray-200 dark:bg-gray-900 h-screen overflow-hidden">
        <Navbar />
        <Switch>
          <Route path="/" component={Dashboard} exact/>
          {/* <Route path="/Predictions" component={Dashboard}/> */}
          <Route path="/Charts" component={Charts}/>
          {/* <Route path="/labTest" component={Dashboard} /> */}
          {/* <Route path="/map" component={Map} /> */}
        </Switch>
      </div>
    </BrowserRouter>
    //</AuthProvider>
  );
}

export default App;