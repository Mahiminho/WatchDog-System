import { useMemo } from "react";
import styles from "./Statistics.module.css";
import { LoadingDots } from "../Main/Main";
import {
  LineChart,
  Line,
  XAxis,
  YAxis,
  Tooltip,
  ResponsiveContainer,
  CartesianGrid,
} from "recharts";

function getDomain(arr, key) {
  if (!arr.length) return [0, 1];
  const vals = arr.map((d) => d[key]);
  const min = Math.min(...vals);
  const max = Math.max(...vals);
  const pad = (max - min) * 0.15 || 5; // 15% padding
  return [Math.floor(min - pad), Math.ceil(max + pad)];
}

function formatTime(date) {
  const d = new Date(date);
  return (
    d.getHours().toString().padStart(2, "0") +
    ":" +
    d.getMinutes().toString().padStart(2, "0")
  );
}

export default function Statistics({ history, loading }) {
  if (loading || !history || history.length === 0) {
    return <LoadingDots />;
  }
  const chartData = useMemo(
    () =>
      history.map((d) => ({
        ...d,
        time: formatTime(d.time),
      })),
    [history]
  );

  const chartColors = {
    temp: "#4d94ff",
    light: "#2962ff",
    air: "#7c4dff",
    sound: "#6200ea",
  };

  const renderChart = (title, dataKey, color, yDomain) => (
    <div className={styles.chartBox} key={dataKey}>
      <h2 className={styles.title}>{title}</h2>
      <ResponsiveContainer width="100%" height="80%">
        <LineChart
          data={chartData}
          margin={{ top: 10, right: 30, left: -10, bottom: 5 }}
        >
          <CartesianGrid strokeDasharray="3 3" stroke="rgba(255,255,255,0.1)" />
          <XAxis
            dataKey="time"
            stroke="rgba(255,255,255,0.5)"
            tick={{ fill: "rgba(255,255,255,0.5)", fontSize: 12 }}
            height={30}
            interval="preserveStartEnd"
            minTickGap={30}
          />
          <YAxis
            domain={yDomain}
            stroke="rgba(255,255,255,0.5)"
            tick={{ fill: "rgba(255,255,255,0.5)", fontSize: 12 }}
            width={50}
            allowDecimals={false}
            interval="preserveStartEnd"
          />
          <Tooltip content={<CustomTooltip />} />
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
      {renderChart(
        "Temperature (°C)",
        "tempData",
        chartColors.temp,
        getDomain(history, "tempData")
      )}
      {renderChart(
        "Light (Lux)",
        "lightData",
        chartColors.light,
        getDomain(history, "lightData")
      )}
      {renderChart(
        "Air Quality (PPM)",
        "airData",
        chartColors.air,
        getDomain(history, "airData")
      )}
      {renderChart(
        "Sound (dB)",
        "soundData",
        chartColors.sound,
        getDomain(history, "soundData")
      )}
    </div>
  );
}

function CustomTooltip({ active, payload, label }) {
  if (active && payload && payload.length) {
    return (
      <div className={styles.chartTooltip}>
        <div style={{ color: "#fff", marginBottom: "0.3em" }}>{label}</div>
        <div>
          value: <b>{payload[0].value}</b>
        </div>
      </div>
    );
  }
  return null;
}
