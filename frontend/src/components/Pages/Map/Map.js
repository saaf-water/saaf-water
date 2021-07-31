import React, { useState, useEffect } from 'react';
/* Map.js contains code which shows the map page*/
const useThemeDetector = () => {
  const getCurrentTheme = () => window.matchMedia("(prefers-color-scheme: dark)").matches;
  const [isDarkTheme, setIsDarkTheme] = useState(getCurrentTheme());
  const mqListener = (e => {
    setIsDarkTheme(e.matches);
  });

  useEffect(() => {
    const darkThemeMq = window.matchMedia("(prefers-color-scheme: dark)");
    darkThemeMq.addListener(mqListener);
    return () => darkThemeMq.removeListener(mqListener);
  }, []);
  return isDarkTheme;
}

function classNames(...classes) {
  return classes.filter(Boolean).join(' ')
}

export default function Map() {
  return (
    <div className="font-roboto">



      <div class="flex flex-row">
        <div class="flex-auto justify-center items-center lg:border  border-gray-300 dark:border-gray-700 lg:rounded-t-xl bg-white dark:bg-gray-800 h-screen relative overflow-auto no-scrollbar">
          <div className="pb-8 lg:pb-5 flex items-start justify-center  w-screen  h-full">
            <iframe
              class={classNames(useThemeDetector() ? 'block' : 'block', 'border-gray-300 lg:rounded-t-xl')}
              width="100%"
              height="95%"
              frameborder="0"
              scrolling="no"
              marginheight="0"
              marginwidth="0"
              title="Saaf-Water-light"
              src="//www.arcgis.com/apps/Embed/index.html?webmap=fbadf2d08dd141aa8fbfe60a227e189b&extent=73.5435,15.0755,74.5577,15.6462&zoom=true&previewImage=false&scale=true&search=true&searchextent=true&disable_scroll=true&theme=light">
            </iframe>
            {/* <iframe
              class={classNames(useThemeDetector() ? 'block' : 'hidden', 'border-gray-700 lg:rounded-t-xl')}
              width="100%"
              height="95%"
              frameborder="0"
              scrolling="no"
              marginheight="0"
              marginwidth="0"
              title="Saaf-Water-Dark"
              src="//www.arcgis.com/apps/Embed/index.html?webmap=3c0c2dc817994509b9d529f7000b3a85&extent=50.3459,3.4536,115.2531,38.1566&zoom=true&previewImage=false&scale=true&search=true&searchextent=true&disable_scroll=true&theme=dark">
            </iframe> */}
          </div>
        </div>

      </div>
    </div>
  );
}
