import React, { useState, useEffect } from "react";
import styles from "./Main.module.css";

import air from "../../assets/main-pics/air.svg";
import light from "../../assets/main-pics/light.svg";
import sound from "../../assets/main-pics/sound.svg";
import temp from "../../assets/main-pics/temp.svg";

export default function Main() {
  const [data, setData] = useState(null);

  useEffect(() => {
    const fetchData = () => {
      fetch("http://localhost:8000/data")
        .then((response) => response.json())
        .then((json) => {
          setData(json); // Store just the latest data object
        })
        .catch((error) => console.error("Connection error!", error));
    };
  
    fetchData(); // Initial fetch
    const interval = setInterval(fetchData, 1000); // Poll every second
  
    return () => clearInterval(interval); // Cleanup interval
  }, []);

  return (
    <>
      {data ? (
        <div className={styles.container}>
          <div className={styles.row}>
            <div className={styles.card}>
              <img src={temp} alt="Temperature" className={styles.image} />
              <span className={styles.text}>
                Temperature: {data.tempData || "N/A"} °C
              </span>
            </div>
            <div className={styles.card}>
              <img src={light} alt="Light" className={styles.image} />
              <span className={styles.text}>
                Luminosity: {data.lightData || "N/A"} Lux
              </span>
            </div>
          </div>
          <div className={styles.row}>
            <div className={styles.card}>
              <img src={air} alt="Air Quality" className={styles.image} />
              <span className={styles.text}>
                Air quality: {data.airData || "N/A"} PPM
              </span>
            </div>
            <div className={styles.card}>
              <img src={sound} alt="Sound" className={styles.image} />
              <span className={styles.text}>
                Sound: {data.soundData || "N/A"} dB
              </span>
            </div>
          </div>
        </div>
      ) : (
        <p className={styles.loading}>Loading...</p>
      )}
    </>
  );
}