import React from "react";
import Skeleton, { SkeletonTheme } from "react-loading-skeleton";
/*Summary.js contains code for displaying water quality summary to users*/

const waterSummary = [
    { type: 'Worse', boilParam: 'Unfit for consumption', altUse: 'Water suitable for external use only!' },
    { type: 'Bad', boilParam: 'RO filteration and good boiling / UV filteration required before cosumption', altUse: 'Water suitable for domestic usage' },
    { type: 'Good', boilParam: 'Sedementary filteration followed by good boiling required before cosumption', altUse: 'Water suitable for domestic usage' },
    { type: 'Very Good', boilParam: 'Boiling required before cosumption', altUse: 'Water suitable for direct domestic usage' },
]

const Summary = ({ current }) => {
    if (current.lastJsonMessage) {
        if (current.lastJsonMessage.summary === 0) {
            return (
                <>
                    <div className="bg-gradient-to-br from-red-500 to-pink-300 dark:from-red-600 dark:to-pink-400 h-96 rounded-xl">
                        <div className="p-4">
                            <div className="justify-self-start font-roboto-semibold text-white text-xl py-5">Water Quality Summary </div>
                            <div className="justify-self-start content-center font-roboto font-extrabold text-white text-6xl pb-5">{waterSummary[current.lastJsonMessage.summary].type}</div>
                            <div className="justify-self-start text-black-900 font-bold p-2">+ {waterSummary[current.lastJsonMessage.summary].boilParam}</div>
                            <div className="justify-self-start text-black-900 font-bold p-2">+ {waterSummary[current.lastJsonMessage.summary].altUse}</div>
                        </div>
                    </div>
                </>
            )
        } else if (current.lastJsonMessage.summary === 1) {
            return (
                <div className="bg-gradient-to-br from-yellow-500 to-yellow-200 dark:from-yellow-600 dark:to-yellow-300 h-96 rounded-xl">
                    <div className="p-4">
                        <div className="justify-self-start font-roboto-semibold text-white text-xl py-5">Water Quality Summary </div>
                        <div className="justify-self-start content-center font-roboto font-extrabold text-white text-6xl pb-5">{waterSummary[current.lastJsonMessage.summary].type}</div>
                        <div className="justify-self-start text-black-900 font-bold p-2">+ {waterSummary[current.lastJsonMessage.summary].boilParam}</div>
                        <div className="justify-self-start text-black-900 font-bold p-2">+ {waterSummary[current.lastJsonMessage.summary].altUse}</div>
                    </div>
                </div>
            )
        } else if (current.lastJsonMessage.summary === 2) {
            return (
                <div className="bg-gradient-to-br from-green-400 to-yellow-200 dark:from-green-500 dark:via-green-400 dark:to-yellow-200 h-96 rounded-xl">
                    <div className="p-4">
                        <div className="justify-self-start font-roboto-semibold text-white text-xl py-5">Water Quality Summary </div>
                        <div className="justify-self-start content-center font-roboto font-extrabold text-white text-6xl pb-5">{waterSummary[current.lastJsonMessage.summary].type}</div>
                        <div className="justify-self-start text-black-900 font-bold p-2">+ {waterSummary[current.lastJsonMessage.summary].boilParam}</div>
                        <div className="justify-self-start text-black-900 font-bold p-2">+ {waterSummary[current.lastJsonMessage.summary].altUse}</div>
                    </div>
                </div>
            )
        } else if (current.lastJsonMessage.summary === 3) {
            return (
                <div className="bg-gradient-to-br from-green-400 via-green-300 to-blue-200 dark:from-green-500 dark:via-green-400 dark:to-blue-300 h-96 rounded-xl">
                    <div className="p-4">
                        <div className="justify-self-start font-roboto-semibold text-white text-xl py-5">Water Quality Summary </div>
                        <div className="justify-self-start content-center font-roboto font-extrabold text-white text-6xl pb-5">{waterSummary[current.lastJsonMessage.summary].type}</div>
                        <div className="justify-self-start text-black-900 font-bold p-2">+ {waterSummary[current.lastJsonMessage.summary].boilParam}</div>
                        <div className="justify-self-start text-black-900 font-bold p-2">+ {waterSummary[current.lastJsonMessage.summary].altUse}</div>
                    </div>
                </div>
            )
        }
    }
    else {
        return (
            <div className="border border-gray-300 dark:border-gray-700 bg-white dark:bg-gray-800 h-96 rounded-xl">
                <div className="p-4">
                    <div className="justify-self-start font-roboto-semibold text-white text-xl py-5">Water Quality Summary </div>
                    <div className="justify-self-start content-center font-roboto font-extrabold text-white text-6xl pb-5">Loading</div>
                    <div className="py-3"></div>
                    <SkeletonTheme className="py-5" color="#cfcfcf" highlightColor="#c4c4c4">
                        <p>
                            <Skeleton count={2} />
                        </p>
                    </SkeletonTheme>
                    <div className="py-2"></div>
                    <SkeletonTheme className="py-5" color="#cfcfcf" highlightColor="#c4c4c4">
                        <p>
                            <Skeleton count={2} />
                        </p>
                    </SkeletonTheme>
                </div>
            </div>
        )
    }
};
export default Summary;

