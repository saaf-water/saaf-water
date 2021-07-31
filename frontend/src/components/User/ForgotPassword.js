import React from 'react'
import firebaseConfig from "../../config.js";

/*ForgotPassword.js builds the forgot password page to reset forgotten passwords*/

/*Password recovery code*/
function ForgotPassword() {
  const forgotSubmit = (e) => {
    e.preventDefault();
    const { email } = e.target.elements;
    try {
      firebaseConfig.auth().sendPasswordResetEmail(email.value);
    } catch (error) {
      alert(error);
    }
  };

  /*Forgot Password page setup code*/
  return (
    <div className="font-roboto min-h-screen bg-gray-100 dark:bg-gray-800 flex flex-row items-center justify-center py-2 pl-2 pr-1 sm:px-6 lg:px-8" >
      <div className="max-w-md w-full space-y-8 p-5">
        <div>
          <div className="flex-1 flex items-center justify-center">
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
          <h2 className="mt-6 p-2 text-center text-2xl font-bold text-gray-900 dark:text-gray-100">Don't worry, we will mail you a password reset link</h2>
        </div>
        <form className="mt-8 space-y-6" onSubmit={forgotSubmit}>
          <input type="hidden" name="remember" defaultValue="true" />
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
                className="appearance-none relative block w-full px-3 py-2 bg-white dark:bg-gray-700 border border-gray-300 dark:border-gray-700 placeholder-gray-500 dark:placeholder-gray-400 text-gray-900 dark:text-gray-100 rounded-md focus:outline-none focus:ring-indigo-500 focus:border-indigo-500 focus:z-10 sm:text-sm mb-8"
                placeholder="Email address"
              />
            </div>
            <div>
              <button
                type="submit"
                className="group relative w-full flex justify-center py-2 px-4 border border-transparent text-sm font-medium rounded-md text-white bg-indigo-600 dark:bg-indigo-500 hover:bg-indigo-700  dark:hover:bg-indigo-700 focus:outline-none focus:ring-2 focus:ring-offset-2 focus:ring-indigo-500"
              >
                Send email
              </button>
            </div>
            <div className="p-5 justify-center flex font-medium text-indigo-600 dark:text-indigo-400 hover:text-indigo-500 dark:hover:text-indigo-700    ">
              <a href="/login">Go back to Login</a>
            </div>
          </div>
        </form>
      </div>
    </div>
  );
};

export default ForgotPassword;
