/*Heatmap.js contains code for Heatmap component on homepage*/
import React, { useEffect, useState } from "react";
import CalendarHeatmap from "react-calendar-heatmap";
import ReactTooltip from "react-tooltip";

import "./Heatmap.css";

const today = new Date();

function shiftDate(date, numDays) {
  const newDate = new Date(date);
  newDate.setDate(newDate.getDate() + numDays);
  return newDate;
}

function getRange(count) {
  return Array.from({ length: count }, (_, i) => i);
}

function getRandomInt(min, max) {
  return Math.floor(Math.random() * (max - min + 1)) + min;
}

const Heatmap = ({ current }) => {
  const [heatMapSize, setHeatMapSize] = useState();

  function resizeMap() {
    //console.log(Math.max(document.documentElement.clientWidth, window.innerWidth || 0));
    if (Math.max(document.documentElement.clientWidth, window.innerWidth || 0) >= 1279) {
      setHeatMapSize(-132);
    } else if (Math.max(document.documentElement.clientWidth, window.innerWidth || 0) <= 1278 && Math.max(document.documentElement.clientWidth, window.innerWidth || 0) >= 808) {
      setHeatMapSize(-364);
    } else {
      setHeatMapSize(-132);
    }
  }

  let calenderData = [];
  // console.log(current.lastJsonMessage);
  
  // eslint-disable-next-line
  const randomValues = getRange(2000).map((index) => {
    return {
      date: shiftDate(today, -index),
      count: getRandomInt(0, 4)
    };
  });
  //console.log(randomValues);

  if (current.lastJsonMessage) {
    Object.keys(current.lastJsonMessage.histMax).map((item) =>
      calenderData.push({
        //date: shiftDate(today, -item), //Dummy Date
        date: current.lastJsonMessage.histMax[item].date,   //Websocket date
        count: current.lastJsonMessage.histMax[item].summary
      })
    );
  }

  useEffect(() => {
    resizeMap();
    window.addEventListener("resize", () => {
      resizeMap();
    });
  }, []);

  //console.log(calenderData);
  //console.log(window.innerWidth);
  
   return (
    <div>
      <CalendarHeatmap
        startDate={shiftDate(today, heatMapSize)}
        endDate={today}
        values={calenderData}
        classForValue={(value) => {
          if (!value) {
            return "color-empty";
          }
          return `color-beammp-${value.count}`;
        }}
        tooltipDataAttrs={(value) => {
          return {
            "data-tip": `Water Quality: ${value.count} on ${value.date}`               
          };
        }}
        showWeekdayLabels={false}
      />
      <ReactTooltip />
    </div>
  );
};
export default Heatmap;
