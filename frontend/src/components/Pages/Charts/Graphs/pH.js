/*pH.js contains the code for the PH component, it shows the pH of water.*/

import React from "react";
import Skeleton, { SkeletonTheme } from "react-loading-skeleton";
import { Line } from "react-chartjs-2";

const Graph = (graphData) => {

  const data = (canvas) => {
    const ctx = canvas.getContext("2d");
    var bord = '#4F4F4F'
    //1. Using gradient background. 
    let gradient = ctx.createLinearGradient(0, 0, 0, 700);
    if (listData[listData.length - 1] >= 6.5 && listData[listData.length - 1] <= 8.5) {
      gradient.addColorStop(0, 'rgba(0, 199,79, 0.33)');
      gradient.addColorStop(0.5, 'rgba(147, 255, 0, 0.2)');
      gradient.addColorStop(1, 'rgba(147, 255, 0, 0)');
      bord = '#00C74F';
    }
    else if (listData[listData.length - 1] < 6.5 || listData[listData.length - 1] > 8.5) {
      gradient.addColorStop(0, 'rgba(255, 0, 0, 0.35)');
      gradient.addColorStop(0.5, 'rgba(245, 19, 100, 0.22)');
      gradient.addColorStop(1, 'rgba(243, 22, 115, 0)');
      bord = '#FF0000';
    }
    return {
      labels: listLabel,
      datasets: [
        {
          label: 'pH',
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
          display: true
        },
        gridLines: {
          display: true,
          color: 'rgba(200, 200, 200, 0.09)'
        }
      }],
      yAxes: [{
        ticks: {
          display:true,
          beginAtZero: true,
          steps: 1,
          stepValue: 1,
          max: 14
        },
        gridLines: {
          display: true,
          color: 'rgba(20, 20, 20, 0.08)'
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
      display: true
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
    listData.push(item.ph);
    return 0;
  });
  graphData.graphData.hist.map((item) => {
    listLabel.push(item.date +" "+ item.time);
    return 0;
  });

  return <Line data={data} options={options} />;
};

const PHFull = ({ current, history }) => {
  return (
    <>
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
export default PHFull;
