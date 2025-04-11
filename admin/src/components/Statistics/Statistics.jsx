import React, { useEffect, useState } from "react";
import styles from "./Statistics.module.css";
import {
  LineChart,
  Line,
  XAxis,
  YAxis,
  Tooltip,
  ResponsiveContainer,
  CartesianGrid,
} from "recharts";

const MAX_POINTS = 60;

export default function Statistics() {
  const [data, setData] = useState([]);

  useEffect(() => {
    const fetchData = () => {
      fetch("http://localhost:8000/data")
        .then((response) => response.json())
        .then((newData) => {
          setData((prevData) => {
            const updatedData = [
              ...prevData,
              {
                time: new Date().toLocaleTimeString("en-GB", {
                  minute: "2-digit",
                  second: "2-digit",
                }),
                tempData: newData.tempData,
                lightData: newData.lightData,
                airData: newData.airData,
                soundData: newData.soundData,
              },
            ];
            return updatedData.slice(-MAX_POINTS);
          });
        })
        .catch((error) => console.error("Connection error!", error));
    };

    fetchData();
    const interval = setInterval(fetchData, 1000);
    return () => clearInterval(interval);
  }, []);

  const chartColors = {
    temp: "#4d94ff",
    light: "#2962ff",
    air: "#7c4dff",
    sound: "#6200ea"
  };

  const renderChart = (title, dataKey, color, yDomain) => (
    <div className={styles.chartBox}>
      <h2 className={styles.title}>{title}</h2>
      <ResponsiveContainer width="100%" height="80%">
        <LineChart
          data={data}
          margin={{ top: 10, right: 30, left: -10, bottom: 5 }}
        >
          <CartesianGrid strokeDasharray="3 3" stroke="rgba(255,255,255,0.1)" />
          <XAxis
            dataKey="time"
            stroke="rgba(255,255,255,0.5)"
            tick={{ fill: "rgba(255,255,255,0.5)", fontSize: 11 }}
            height={30}
          />
          <YAxis
            domain={yDomain}
            stroke="rgba(255,255,255,0.5)"
            tick={{ fill: "rgba(255,255,255,0.5)", fontSize: 11 }}
            width={35}
          />
          <Tooltip
            contentStyle={{
              backgroundColor: "rgba(14, 17, 23, 0.95)",
              border: "1px solid rgba(255,255,255,0.2)",
              borderRadius: "4px",
              padding: "8px",
            }}
            labelStyle={{ color: "rgba(255,255,255,0.8)" }}
            itemStyle={{ color: "rgba(255,255,255,0.8)" }}
          />
          <Line
            type="monotone"
            dataKey={dataKey}
            stroke={color}
            strokeWidth={2}
            dot={false}
            isAnimationActive={false}
          />
        </LineChart>
      </ResponsiveContainer>
    </div>
  );

  return (
    <div className={styles.grid}>
      {renderChart("Temperature (°C)", "tempData", chartColors.temp, [-10, 55])}
      {renderChart("Light (Lux)", "lightData", chartColors.light, [0, 150])}
      {renderChart("Air Quality (PPM)", "airData", chartColors.air, [0, 150])}
      {renderChart("Sound (dB)", "soundData", chartColors.sound, [0, 150])}
    </div>
  );
}
