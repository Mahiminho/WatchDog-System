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

export default function Statistics() {
  const [data, setData] = useState([]);

  useEffect(() => {
    fetch("https://67db42341fd9e43fe4741621.mockapi.io/data")
      .then((response) => response.json())
      .then((json) => {
        const sliced = json.slice(-100).map((item, index) => ({
          id: index,
          temp: parseFloat(item.tempData),
        }));
        setData(sliced);
      })
      .catch((error) => console.error("Connection error!", error));
  }, []);

  return (
    <div className={styles.container}>
      <h2 className={styles.title}>Temperature</h2>
      <ResponsiveContainer width="100%" height={400}>
        <LineChart data={data}>
          <CartesianGrid strokeDasharray="3 3" stroke="#444" />
          <XAxis dataKey="id" stroke="#ccc" />
          <YAxis domain={[-10, 50]} stroke="#ccc" />
          <Tooltip
            contentStyle={{ backgroundColor: "#2c2c2c", border: "none" }}
            labelStyle={{ color: "#ccc" }}
            itemStyle={{ color: "#fff" }}
          />
          <Line
            type="monotone"
            dataKey="temp"
            stroke="#00c3ff"
            strokeWidth={2}
            dot={{ r: 3 }}
            activeDot={{ r: 5 }}
          />
        </LineChart>
      </ResponsiveContainer>
    </div>
  );
}