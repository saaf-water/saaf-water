/*TDS.js contains the code for TDS component, it shows the TDS level of water.*/
import React from "react";
import Skeleton, { SkeletonTheme } from "react-loading-skeleton";
import { Line } from "react-chartjs-2";

const Graph = (graphData) => {

  const data = (canvas) => {
    const ctx = canvas.getContext("2d");
    var bord = '#4F4F4F'
    //1. Using gradient background. 
    let gradient = ctx.createLinearGradient(0, 0, 0, 130);
    if (listData[listData.length - 1] >= 0 && listData[listData.length - 1] < 250) {
      gradient.addColorStop(0, 'rgba(0, 199,79, 0.33)');
      gradient.addColorStop(0.5, 'rgba(147, 255, 0, 0.2)');
      gradient.addColorStop(1, 'rgba(147, 255, 0, 0)');
      bord = '#00C74F';
    }
    else if (listData[listData.length - 1] >= 250 && listData[listData.length - 1] < 950) {
      gradient.addColorStop(0, 'rgba(247, 255, 32, 0.17)');
      gradient.addColorStop(0.5, 'rgba(247, 161, 32, 0.11)');
      gradient.addColorStop(1, 'rgba(243, 142, 22, 0)');
      bord = '#E0CA00';
    }
    else if (listData[listData.length - 1] >= 950) {
      gradient.addColorStop(0, 'rgba(255, 0, 0, 0.35)');
      gradient.addColorStop(0.5, 'rgba(245, 19, 100, 0.22)');
      gradient.addColorStop(1, 'rgba(243, 22, 115, 0)');
      bord = '#FF0000';
    }
    return {
      labels: listLabel,
      datasets: [
        {
          label: 'TDS',
          data: listData,
          fill: true,
          backgroundColor: gradient,
          pointBackgroundColor: 'white',
          borderWidth: 2,
          borderColor: bord,
        }
      ]
    }
  };

  const options = {
    responsive: true,
    maintainAspectRatio: true,
    animation: {
      easing: 'easeInOutQuad',
      duration: 1024
    },
    scales: {
      xAxes: [{
        ticks: {
          display: false
        },
        gridLines: {
          display: false,
          color: 'rgba(200, 200, 200, 0.05)'
        }
      }],
      yAxes: [{
        ticks: {
          display: false
        },
        gridLines: {
          display: false,
          color: 'rgba(20, 20, 20, 0)'
        }
      }]
    },
    layout: {
      padding: 5
    },
    elements: {
      line: {
        tension: 0.5
      }
    },
    legend: {
      display: false
    },
    point: {
      backgroundColor: 'white'
    },
    tooltips: {
      mode: 'index',
      intersect: false
   },
   hover: {
      mode: 'index',
      intersect: false
   }
  };
  var listData = [];
  var listLabel = [];
  graphData.graphData.hist.map((item) => {
    listData.push(item.tds);
    return 0;
  });
  graphData.graphData.hist.map((item) => {
    listLabel.push(item.date +" "+ item.time);
    return 0;
  });

  return <Line data={data} options={options} />;
};


const TDS = ({ current, history }) => {
  return (
    <>
      <div className="header text-black dark:text-white">
        <h5 className="title text-gray-500 dark:text-gray-400 font-bold">
          TDS
        </h5>
        <h1 className="font-extrabold text-xl xl:text-2xl 2xl:text-3xl">
          {current.lastJsonMessage
            ? current.lastJsonMessage.tds + " PPM"
            : "- PPM"}
        </h1>
        <div className="links"></div>
      </div>
      {history.lastJsonMessage ? (
        <Graph graphData={history.lastJsonMessage} />
      ) : (
        <SkeletonTheme className="py-5" color="#cfcfcf" highlightColor="#c4c4c4">
          <p>
            <Skeleton count={3} />
          </p>
        </SkeletonTheme>
      )}
    </>
  );
};
export default TDS;
