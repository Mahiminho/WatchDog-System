import { useState, useRef, useEffect } from "react";
import styles from "./Data.module.css";
import {
  LineChart,
  Line,
  XAxis,
  YAxis,
  Tooltip,
  ResponsiveContainer,
  CartesianGrid,
} from "recharts";
import DatePicker from "react-datepicker";
import "react-datepicker/dist/react-datepicker.css";

const SENSOR_LABELS = [
  "Temperature",
  "Air Quality",
  "Light",
  "Presence",
  "Distance",
  "Motion",
  "Sound",
  "Vibration",
];
const SENSOR_KEYS = [
  "temperature",
  "airPPM",
  "lightLux",
  "presence",
  "presenceDistance",
  "motion",
  "noiseDB",
  "vibration",
];
const CHART_COLORS = ["#1e90ff", "#7c3aed", "#a78bfa", "#2563eb"];

function getNowDate() {
  const now = new Date();
  return now.toISOString().slice(0, 10);
}
function getNowTime() {
  const now = new Date();
  return now.toTimeString().slice(0, 8);
}
function formatTimestamp(date, time) {
  return `${date.replace(/-/g, "/")} ${time}`;
}

export default function Data({ API_URL }) {
  const [fromDate, setFromDate] = useState("");
  const [fromTime, setFromTime] = useState("");
  const [toDate, setToDate] = useState("");
  const [toTime, setToTime] = useState("");
  const [toActive, setToActive] = useState(false);

  const [data, setData] = useState(null);
  const [selectedSensor, setSelectedSensor] = useState(0);
  const [chartColorIdx, setChartColorIdx] = useState(0);

  const [fromDateObj, setFromDateObj] = useState(null);
  const [toDateObj, setToDateObj] = useState(null);

  const [splitX, setSplitX] = useState(
    Number(localStorage.getItem("dataSplitX")) || 50
  );
  const dragging = useRef(false);

  const [noData, setNoData] = useState(false);
  const [isDragging, setIsDragging] = useState(false);

  const [hoverRow, setHoverRow] = useState(null);
  const [hoverCol, setHoverCol] = useState(null);

  useEffect(() => {
    setFromDate("");
    setFromTime("");
    setToDate("");
    setToTime("");
    setToActive(false);
    setData(null);
    setSelectedSensor(0);
    setChartColorIdx(0);
    setNoData(false);
    setHoverRow(null);
    setHoverCol(null);
    setSplitX(Number(localStorage.getItem("dataSplitX")) || 50);
  }, []);

  useEffect(() => {
    const onDrag = (e) => {
      if (!dragging.current) return;
      setIsDragging(true);
      document.body.style.userSelect = "none";
      document.body.classList.add("no-pointer-events");
      const percent = (e.clientX / window.innerWidth) * 100;
      setSplitX(Math.max(20, Math.min(80, percent)));
    };
    const onUp = () => {
      dragging.current = false;
      setIsDragging(false);
      document.body.style.userSelect = "";
      document.body.classList.remove("no-pointer-events");
    };
    window.addEventListener("mousemove", onDrag);
    window.addEventListener("mouseup", onUp);
    return () => {
      window.removeEventListener("mousemove", onDrag);
      window.removeEventListener("mouseup", onUp);
      document.body.style.userSelect = "";
      document.body.classList.remove("no-pointer-events");
    };
  }, []);

  useEffect(() => {
    localStorage.setItem("dataSplitX", splitX);
  }, [splitX]);

  useEffect(() => {
    if (fromDate && fromTime) setToActive(true);
    else setToActive(false);
    setToDate("");
    setToTime("");
    setData(null);
    setNoData(false);
  }, [fromDate, fromTime]);

  const handleSensorTab = (idx) => {
    setSelectedSensor(idx);
    let next = Math.floor(Math.random() * CHART_COLORS.length);
    while (next === chartColorIdx)
      next = Math.floor(Math.random() * CHART_COLORS.length);
    setChartColorIdx(next);
  };

  let tableRows = [];
  if (data && data.timestamps) {
    const len = data.timestamps.length;
    for (let i = 0; i < len; ++i) {
      tableRows.push([
        data.timestamps[i],
        ...SENSOR_KEYS.map((k) => data[k][i]),
      ]);
    }
  }

  const chartData =
    data && data[SENSOR_KEYS[selectedSensor]] && data.timestamps
      ? data[SENSOR_KEYS[selectedSensor]].map((v, i) => ({
          value: typeof v === "boolean" ? (v ? 1 : 0) : v,
          time: formatTime(data.timestamps[i]),
        }))
      : [];

  function formatTime(ts) {
    if (!ts) return "";
    let norm = ts.replace(/\//g, "-").replace(" ", "T");
    const d = new Date(norm);
    if (isNaN(d.getTime())) return "";
    return (
      d.getHours().toString().padStart(2, "0") +
      ":" +
      d.getMinutes().toString().padStart(2, "0")
    );
  }

  const handleGetData = () => {
    setNoData(false);
    setData(null);
    const timestamp1 = formatTimestamp(fromDate, fromTime);
    const timestamp2 = formatTimestamp(toDate, toTime);

    fetch(
      `${API_URL}/sensors-range?timestamp1=${encodeURIComponent(
        timestamp1
      )}&timestamp2=${encodeURIComponent(timestamp2)}`
    )
      .then((res) => (res.ok ? res.json() : res.json()))
      .then((json) => {
        if (!json || json.error) {
          setNoData(true);
          setData(null);
        } else {
          setData(json);
        }
      })
      .catch(() => {
        setNoData(true);
        setData(null);
      });
  };

  const handleCloseNoData = () => {
    setNoData(false);
    setData(null);
    setFromDate("");
    setFromTime("");
    setToDate("");
    setToTime("");
    setToActive(false);
  };

  const minToDate = fromDate || "";
  const minToTime = fromTime || "";

  return (
    <div className={styles.dataRoot}>
      <div className={styles.rangePanel}>
        <div className={styles.rangeRow}>
          <div>
            <label>From:</label>
            <DatePicker
              selected={fromDateObj}
              onChange={(date) => {
                setFromDateObj(date);
                setFromDate(date ? date.toISOString().slice(0, 10) : "");
                setFromTime(date ? date.toTimeString().slice(0, 8) : "");
              }}
              showTimeSelect
              timeFormat="HH:mm:ss"
              timeIntervals={1}
              dateFormat="yyyy-MM-dd HH:mm:ss"
              placeholderText="Select date & time"
              className={styles.datePicker}
              maxDate={toDateObj}
            />
          </div>
          <span className={styles.rangeDash}>-</span>
          <div>
            <label>To:</label>
            <DatePicker
              selected={toDateObj}
              onChange={(date) => {
                setToDateObj(date);
                setToDate(date ? date.toISOString().slice(0, 10) : "");
                setToTime(date ? date.toTimeString().slice(0, 8) : "");
              }}
              showTimeSelect
              timeFormat="HH:mm:ss"
              timeIntervals={1}
              dateFormat="yyyy-MM-dd HH:mm:ss"
              placeholderText="Select date & time"
              className={styles.datePicker}
              minDate={fromDateObj}
              disabled={!toActive}
            />
          </div>
        </div>
        <button
          className={styles.getButton}
          onClick={handleGetData}
          disabled={!fromDate || !fromTime || !toDate || !toTime}
        >
          Get data
        </button>
      </div>

      {noData && (
        <div className={styles.noDataPopup}>
          <div className={styles.noDataBox}>
            <button className={styles.closeBtn} onClick={handleCloseNoData}>
              ×
            </button>
            <div className={styles.noDataText}>No data for this time range</div>
          </div>
        </div>
      )}

      <div className={styles.contentWrap}>
        <div className={styles.left} style={{ width: `${splitX}%` }}>
          <div className={styles.chartBoxCard}>
            <div className={styles.sensorTabs}>
              {SENSOR_LABELS.map((label, i) => (
                <button
                  key={label}
                  className={
                    i === selectedSensor
                      ? styles.sensorTabActive
                      : styles.sensorTab
                  }
                  onClick={() => handleSensorTab(i)}
                >
                  {label}
                </button>
              ))}
            </div>
            <div className={styles.chartBox}>
              {!data ||
              !data[SENSOR_KEYS[selectedSensor]] ||
              data[SENSOR_KEYS[selectedSensor]].length === 0 ? (
                <div className={styles.chartNoData}>No data</div>
              ) : (
                <ResponsiveContainer width="100%" height="100%">
                  <LineChart
                    data={chartData}
                    margin={{ top: 10, right: 30, left: -10, bottom: 5 }}
                  >
                    <CartesianGrid
                      strokeDasharray="3 3"
                      stroke="rgba(255,255,255,0.1)"
                    />
                    <XAxis
                      dataKey="time"
                      stroke="rgba(255,255,255,0.5)"
                      tick={{ fill: "rgba(255,255,255,0.5)", fontSize: 12 }}
                      interval="preserveStartEnd"
                      minTickGap={30}
                    />
                    <YAxis
                      stroke="rgba(255,255,255,0.5)"
                      tick={{ fill: "rgba(255,255,255,0.5)", fontSize: 12 }}
                      width={50}
                      allowDecimals={false}
                      interval="preserveStartEnd"
                    />
                    {/* <Tooltip /> */}
                    <Tooltip content={<CustomTooltip />} />
                    <Line
                      type="monotone"
                      dataKey="value"
                      stroke={
                        CHART_COLORS[selectedSensor % CHART_COLORS.length]
                      }
                      strokeWidth={2}
                      dot={false}
                    />
                  </LineChart>
                </ResponsiveContainer>
              )}
            </div>
          </div>
        </div>
        <div
          className={styles.dragBarX}
          style={{ left: `calc(${splitX}% - 4px)` }}
          onMouseDown={() => (dragging.current = true)}
        />
        <div className={styles.right} style={{ width: `${100 - splitX}%` }}>
          <div className={styles.tableWrap}>
            <table className={styles.table}>
              <thead>
                <tr>
                  <th style={{ minWidth: 120 }}>Timestamp</th>
                  {SENSOR_LABELS.map((label, i) => (
                    <th key={label}>{label}</th>
                  ))}
                </tr>
              </thead>
              <tbody>
                {tableRows.length === 0 ? (
                  <tr className={styles.noDataRow}>
                    <td colSpan={SENSOR_LABELS.length + 1}>
                      <span className={styles.tableEmpty}>No data</span>
                    </td>
                  </tr>
                ) : (
                  tableRows.map((row, i) => (
                    <tr
                      key={i}
                      onMouseEnter={() => setHoverRow(i)}
                      onMouseLeave={() => setHoverRow(null)}
                    >
                      {row.map((val, j) => (
                        <td
                          key={j}
                          className={
                            hoverRow === i || hoverCol === j
                              ? styles.cellHover
                              : ""
                          }
                          onMouseEnter={() => setHoverCol(j)}
                          onMouseLeave={() => setHoverCol(null)}
                        >
                          {j === 0
                            ? String(val)
                                .replace(/\.\d+$/, "")
                                .replace("T", " ")
                            : typeof val === "boolean"
                            ? val
                              ? "Yes"
                              : "No"
                            : val}
                        </td>
                      ))}
                    </tr>
                  ))
                )}
              </tbody>
            </table>
          </div>
        </div>
      </div>
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
