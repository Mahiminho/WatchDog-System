import React, { useState, useEffect } from "react";
import styles from "./Main.module.css";

import air from "../../assets/main-pics/air.svg";
import light from "../../assets/main-pics/light.svg";
import sound from "../../assets/main-pics/sound.svg";
import temp from "../../assets/main-pics/temp.svg";

export default function Main() {
  const [data, setData] = useState([]);

  useEffect(() => {
    fetch("https://67db42341fd9e43fe4741621.mockapi.io/data")
      .then((response) => response.json())
      .then((json) => setData(json))
      .catch((error) => console.error("Connection error!", error));
  }, []);

  return (
    <>
      {data.length > 0 ? (
        <div className={styles.container}>
          <div className={styles.row}>
            <div className={styles.card}>
              <img src={temp} alt="Temperature" className={styles.image} />
              <span className={styles.text}>
                Temperature: {data[0]?.tempData || "N/A"} °C
              </span>
            </div>
            <div className={styles.card}>
              <img src={light} alt="Light" className={styles.image} />
              <span className={styles.text}>
                Luminosity: {data[0]?.lightData || "N/A"} Lux
              </span>
            </div>
          </div>
          <div className={styles.row}>
            <div className={styles.card}>
              <img src={air} alt="Air Quality" className={styles.image} />
              <span className={styles.text}>
                Air quality: {data[0]?.airData || "N/A"} PPM
              </span>
            </div>
            <div className={styles.card}>
              <img src={sound} alt="Sound" className={styles.image} />
              <span className={styles.text}>
                Sound: {data[0]?.soundData || "N/A"} dB
              </span>
            </div>
          </div>
        </div>
      ) : (
        <p className="">Loading...</p>
      )}
    </>
  );
}