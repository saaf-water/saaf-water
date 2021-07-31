import React from 'react';
import { Line } from 'react-chartjs-2';

const CombinedData = {
  labels: ['1', '2', '3', '4', '5', '6'],
  datasets: [
    {
      label: 'TDS in ppm',
      data: [12, 9, 2, 10, 8, 7],
      fill: false,
      fillOpacity: 0.5,
      backgroundColor: 'rgba(123, 209, 82, 0.3)',
      borderColor: 'rgb(5, 115, 74)',
    },
    {
      label: 'Turbidity in NTU',
      data: [4, 9, 12, 10, 7, 5],
      fill: false,
      fillOpacity: 0.5,
      backgroundColor: 'rgba(239, 126, 50, 0.5)',
      borderColor: 'rgb(222, 84, 44)',
    },
    {
      label: 'pH value',
      data: [3, 5, 5, 6, 4, 2],
      fill: false,
      fillOpacity: 0.5,
      backgroundColor: 'rgba(231, 227, 78, 0.8)',
      borderColor: 'rgb(234, 189, 59)',
    },
    {
      label: 'Electrical Conductivity',
      data: [12, 19, 3, 5, 2, 3],
      fill: false,
      fillOpacity: 0.5,
      backgroundColor: 'rgba(255, 10, 13, 0.1)',
      borderColor: 'rgba(235, 84, 140, 1.00)',
    },
    {
      label: 'Temperature in Celsius',
      data: [20, 25, 24, 20, 22, 21],
      fill: false,
      fillOpacity: 0.5,
      backgroundColor: 'rgba(26, 201, 230, 0.4)',
      borderColor: 'rgba(23, 107, 160, 1.00)',
    },

  ],
};

const options = {
  scales: {
    yAxes: [
      {
        ticks: {
          beginAtZero: true,
        },
      },
    ],
  },
};

const CombinedChart = () => (
  <>

    <Line data={CombinedData} options={options} />
  </>
);
export default CombinedChart;
