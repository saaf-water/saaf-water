import React, { useState } from "react"
import { Redirect } from "react-router-dom"
import firebaseConfig from "../../config"
import coverimg from "./Asset/poster.webp"

/*SignUp.js builds the signup page of the SaafWater webapp, it also includes code for signing up as an admin */

/*Signup code*/
const SignUp = () => {
  const [currentUser, setCurrentUser] = useState(null);
  const handleSubmit = (e) => {
    e.preventDefault();
    const { email, password } = e.target.elements;
    try {
      firebaseConfig.auth().createUserWithEmailAndPassword(email.value, password.value).then((userCredential) => {
        userCredential.user.sendEmailVerification();
        firebaseConfig.auth().signOut();
        alert("Email Sent for verification");
      });
      setCurrentUser(true);
    } catch (error) {
      alert(error);
    }
  };
  if (currentUser) {
    return <Redirect to="/login" />;
  }

  /*Page setup*/
  return (
    <div class="font-roboto relative flex items-center justify-center h-screen mb-12 overflow-hidden p-2 ">
      <div class="relative z-40 backdrop-filter backdrop-blur-sm p-5 bg-gray-200 dark:bg-gray-900 bg-opacity-60 shadow-lg rounded-xl">
        <div className="container mx-auto">
          <div className="flex flex-row items-center sm:justify-center lg:justify-start py-2 pl-2 pr-1 sm:px-6 lg:px-8 " >
            <div className="max-w-md w-full space-y-8 ">
              <div>
                <div className="flex-1 flex items-center justify-center sm:items-stretch sm:justify-center">
                  <div className="flex-shrink-0 flex items-center">
                    <a
                      href="/"
                      className="text-4xl font-thin text-gray-600 dark:text-gray-300 lg:hidden h-8 w-auto"
                      alt="Saaf water">
                      <span class="text-4xl font-black text-indigo-600 dark:text-indigo-400">Saaf </span> water
                    </a>
                    <a
                      href="/"
                      className="text-5xl font-thin text-gray-600 dark:text-gray-300 hidden lg:block h-8 w-auto"
                      alt="Saaf water">
                      <span class="text-5xl font-black text-indigo-600 dark:text-indigo-400">Saaf </span> water
                    </a>
                  </div>
                </div>
                <h2 className="mt-6 text-center text-md md:text-xl font-extrabold text-gray-900 dark:text-gray-100">Create your new account</h2>
              </div>
              <form className="mt-8 space-y-6" onSubmit={handleSubmit}>
                <div className="rounded-md shadow-sm -space-y-px">
                  <div>
                    <label htmlFor="email-address" className="sr-only">
                      Email address
                    </label>
                    <input
                      id="email-address"
                      name="email"
                      type="email"
                      autoComplete="email"
                      required
                      className="appearance-none rounded-none relative block w-full px-3 py-2 bg-white dark:bg-gray-800 border border-gray-300 dark:border-gray-700 placeholder-gray-500 dark:placeholder-gray-600 text-gray-900 dark:text-gray-100 rounded-t-md focus:outline-none focus:ring-indigo-500 focus:border-indigo-500 focus:z-10 sm:text-sm"
                      placeholder="Email address"
                    />
                  </div>
                  <div>
                    <label htmlFor="password" className="sr-only">
                      Password
                    </label>
                    <input
                      id="password"
                      name="password"
                      type="password"
                      autoComplete="current-password"
                      required
                      className="appearance-none rounded-none relative block w-full px-3 py-2 bg-white dark:bg-gray-800 border border-gray-300 dark:border-gray-700 placeholder-gray-500 dark:placeholder-gray-600 text-gray-900 dark:text-gray-100 focus:outline-none focus:ring-indigo-500 focus:border-indigo-500 focus:z-10 sm:text-sm"
                      placeholder="Password"
                    />
                  </div>
                  <div>
                    <label htmlFor="pincode" className="sr-only">
                      Enter Area Pincode
                    </label>
                    <input
                      id="pincode"
                      name="pincode"
                      type="pincode"
                      required
                      className="appearance-none rounded-none relative block w-full px-3 py-2 bg-white dark:bg-gray-800 border border-gray-300 dark:border-gray-700 placeholder-gray-500 dark:placeholder-gray-600 text-gray-900 dark:text-gray-100 rounded-b-md focus:outline-none focus:ring-indigo-500 focus:border-indigo-500 focus:z-10 sm:text-sm"
                      placeholder="Area Pincode"
                    />
                  </div>
                </div>
                <div className="flex items-center justify-between">
                  <div className=" flex px-1 sm:px-5 items-center">
                    <input
                      id="remember_me"
                      name="remember_me"
                      type="checkbox"
                      className="h-4 w-4 text-indigo-600 focus:ring-indigo-500 bg-white dark:bg-gray-800 border  border-gray-300 rounded dark:border-gray-700"
                    />
                    <label htmlFor="remember_me" className="font-medium ml-2 block text-xs md:text-sm text-gray-900 dark:text-gray-100">
                      Remember me
                    </label>
                  </div>

                  <div className="text-sm px-1 sm:px-5 ">
                    <a href="/ForgotPassword" className="font-medium text-xs md:text-sm text-indigo-600 dark:text-indigo-400 hover:text-indigo-500 dark:hover:text-indigo-700">
                      Forgot your password?
                    </a>
                  </div>
                </div>
                <div>
                  <button
                    type="submit"
                    className="group relative w-full flex justify-center py-2 px-4 border border-transparent text-sm font-medium rounded-md text-white bg-indigo-600 dark:bg-indigo-500 hover:bg-indigo-700  dark:hover:bg-indigo-700 focus:outline-none focus:ring-2 focus:ring-offset-2 focus:ring-indigo-500"
                  >
                    <span className="absolute left-0 inset-y-0 flex items-center pl-3">

                    </span>
                    Sign Up
                  </button>
                </div>
                <div className="flex justify-center text-xs md:text-sm">
                  <a href="/Login" className="font-medium text-indigo-600 dark:text-indigo-400 hover:text-indigo-500 dark:hover:text-indigo-700    ">
                    Already have an account? Login!
                  </a>
                </div>
              </form>

            </div>
          </div>
        </div>

      </div>
      <video autoPlay poster={coverimg} loop muted className="filter brightness-75 absolute z-0 w-auto min-w-full min-h-full max-w-none">
        <source src="https://www.dropbox.com/s/q6pl6f5o0jap3dx/Saaf-water-low.webm?raw=1" type="video/mp4"></source>
        Your browser does not support the video tag.
      </video>
    </div>
  );
};

export default SignUp;