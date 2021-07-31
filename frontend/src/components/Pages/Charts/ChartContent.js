import React, { useState, useEffect } from "react";
import useWebSocket from "react-use-websocket";
import Skeleton, { SkeletonTheme } from "react-loading-skeleton";

import ElectricalConductivityFull from "./Graphs/electricalConductivity";
import PHFull from "./Graphs/pH";
import TDSFull from "./Graphs/TDS";
import TemperatureFull from "./Graphs/Temperature";
import TurbidityFull from "./Graphs/Turbidity";

require('dotenv').config()
/* ChartContent.js communicates through websockets and sends props to other graph components*/

export default function ChartContent() {
    const [socketUrl] = useState(
        process.env.React_App_HISTORY_WEBSOCKET
    );

    const [socketCurrentUrl] = useState(
        process.env.React_App_PUMP_WEBSOCKET
    );

    const history = useWebSocket(socketUrl);
    const current = useWebSocket(socketCurrentUrl);

    useEffect(() => {
        //console.log("Sending Message on Component Mount");
        current.sendMessage("Get Data");
        setTimeout(() => {
            history.sendMessage("Get Data");
        }, 2000);

        //Every 30 Mins
        setInterval(() => {
            //console.log("Sending Message");
            current.sendMessage("Get Data");
            setTimeout(() => {
                history.sendMessage("Get Data");
            }, 2000);
        }, 1800000);
        // eslint-disable-next-line
    }, []);

    return (
        <div className="font-roboto flex-col pb-44 space-y-2 container px-5 py-5 mx-auto">
            <p className="font-light text-sm">Last Updated: {current.lastJsonMessage
                ? current.lastJsonMessage.date + ", " + current.lastJsonMessage.time : (
                    <SkeletonTheme className="py-1" color="#cfcfcf" highlightColor="#c4c4c4">
                        <p>
                            <Skeleton count={1} />
                        </p>
                    </SkeletonTheme>)}</p>
            <div className="flex flex-wrap justify-center -m-4">
                <div className=" w-full lg:w-2/5 m-4 relative border border-gray-300 dark:border-gray-700 rounded-xl bg-white dark:bg-gray-800" >
                    <div className="p-4">
                        <div className="p-5 justify-self-start content-center font-roboto font-extrabold text-black dark:text-white text-3xl pb-5">TDS</div>
                        <TDSFull current={current} history={history} />
                    </div>
                </div>
                <div className=" w-full lg:w-2/5 lg:h-1/4 m-4 relative border border-gray-300 dark:border-gray-700 rounded-xl bg-white dark:bg-gray-800" >
                    <div className="p-4">
                        <div className="p-5 justify-self-start content-center font-roboto font-extrabold text-black dark:text-white text-3xl pb-5">Turbidity</div>
                        <TurbidityFull current={current} history={history} />
                    </div>
                </div>
                <div className=" w-full lg:w-2/5 m-4 relative border border-gray-300 dark:border-gray-700 rounded-xl bg-white dark:bg-gray-800" >
                    <div className="p-4">
                        <div className="p-5 justify-self-start content-center font-roboto font-extrabold text-black dark:text-white text-3xl pb-5">ph</div>
                        <PHFull current={current} history={history} />
                    </div>
                </div>
                <div className=" w-full lg:w-2/5 m-4 relative border border-gray-300 dark:border-gray-700 rounded-xl bg-white dark:bg-gray-800" >
                    <div className="p-4">
                        <div className="p-5 justify-self-start content-center font-roboto font-extrabold text-black dark:text-white text-3xl pb-5">Electrical Conductivity</div>
                        <ElectricalConductivityFull current={current} history={history} />
                    </div>
                </div>
                <div className=" w-full lg:w-2/5 m-4 relative border border-gray-300 dark:border-gray-700 rounded-xl bg-white dark:bg-gray-800" >
                    <div className="p-4">
                        <div className="p-5 justify-self-start content-center font-roboto font-extrabold text-black dark:text-white text-3xl pb-5">Temperature</div>
                        <TemperatureFull current={current} history={history} />
                    </div>
                </div>
            </div>
        </div>
    );
}