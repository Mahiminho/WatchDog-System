import { useEffect, useState, useRef } from "react";
import styles from "./Logs.module.css";
import { LoadingDots } from "../Main/Main";

const SENSOR_TABS = [
  "Temperature",
  "Air Quality",
  "Light",
  "Sound",
  "Presence",
  "Motion",
  "Vibration",
];
const NODE_TABS = ["Fire", "Gas", "Human", "Damage"];
const MAX_LOGS = 30;

export default function Logs({
  envReport,
  envReportTimestamp,
  singleSensorInsights,
  combinedSensorInsights,
}) {
  const [smartLogs, setSmartLogs] = useState([]);
  const [sensorLogs, setSensorLogs] = useState({});
  const [activeSensor, setActiveSensor] = useState(SENSOR_TABS[0]);
  const [nodeLogs, setNodeLogs] = useState({});
  const [activeNode, setActiveNode] = useState(NODE_TABS[0]);
  const [splitX, setSplitX] = useState(
    Number(localStorage.getItem("logsSplitX")) || 50
  );
  const [splitY, setSplitY] = useState(
    Number(localStorage.getItem("logsSplitY")) || 50
  );
  const dragging = useRef(null);

  useEffect(() => {
    setSmartLogs(JSON.parse(localStorage.getItem("smartLogs") || "[]"));
    setSensorLogs(JSON.parse(localStorage.getItem("sensorLogs") || "{}"));
    setNodeLogs(JSON.parse(localStorage.getItem("nodeLogs") || "{}"));
  }, []);

  useEffect(() => {
    localStorage.setItem("logsSplitX", splitX);
  }, [splitX]);
  useEffect(() => {
    localStorage.setItem("logsSplitY", splitY);
  }, [splitY]);

  useEffect(() => {
    if (envReport && envReportTimestamp) {
      setSmartLogs((prev) => {
        if (
          prev.length &&
          prev[0].time === envReportTimestamp &&
          prev[0].text === envReport
        ) {
          return prev;
        }
        const newLog = {
          time: envReportTimestamp,
          text: envReport,
        };
        const updated = [newLog, ...prev].slice(0, MAX_LOGS);
        localStorage.setItem("smartLogs", JSON.stringify(updated));
        return updated;
      });
    }
  }, [envReport, envReportTimestamp]);

  useEffect(() => {
    if (singleSensorInsights && singleSensorInsights.length) {
      setSensorLogs((prev) => {
        const updated = { ...prev };
        SENSOR_TABS.forEach((tab, i) => {
          if (!updated[tab]) updated[tab] = [];
          if (singleSensorInsights[i]) {
            updated[tab] = [
              {
                time: singleSensorInsights[i].timestamp,
                text: singleSensorInsights[i].text,
              },
              ...updated[tab],
            ].slice(0, MAX_LOGS);
          }
        });
        localStorage.setItem("sensorLogs", JSON.stringify(updated));
        return updated;
      });
    }
  }, [singleSensorInsights]);

  useEffect(() => {
    if (combinedSensorInsights && combinedSensorInsights.length) {
      setNodeLogs((prev) => {
        const updated = { ...prev };
        NODE_TABS.forEach((tab, i) => {
          if (!updated[tab]) updated[tab] = [];
          if (combinedSensorInsights[i]) {
            updated[tab] = [
              {
                time: combinedSensorInsights[i].timestamp,
                text: combinedSensorInsights[i].text,
              },
              ...updated[tab],
            ].slice(0, MAX_LOGS);
          }
        });
        localStorage.setItem("nodeLogs", JSON.stringify(updated));
        return updated;
      });
    }
  }, [combinedSensorInsights]);

  const onDragX = (e) => {
    if (dragging.current === "x") {
      const percent = (e.clientX / window.innerWidth) * 100;
      setSplitX(Math.max(20, Math.min(80, percent)));
    }
  };
  const onDragY = (e) => {
    if (dragging.current === "y") {
      const percent = ((e.clientY - 60) / (window.innerHeight - 60)) * 100;
      setSplitY(Math.max(20, Math.min(80, percent)));
    }
  };
  useEffect(() => {
    const move = (e) => {
      if (dragging.current === "x") onDragX(e);
      if (dragging.current === "y") onDragY(e);
    };
    const up = () => (dragging.current = null);
    window.addEventListener("mousemove", move);
    window.addEventListener("mouseup", up);
    return () => {
      window.removeEventListener("mousemove", move);
      window.removeEventListener("mouseup", up);
    };
  }, []);

  const isLoading =
    envReport === null &&
    (!singleSensorInsights || singleSensorInsights.length === 0) &&
    (!combinedSensorInsights || combinedSensorInsights.length === 0);

  if (isLoading) {
    return <LoadingDots />;
  }

  return (
    <div className={styles.logsRoot}>
      <div className={styles.left} style={{ width: `${splitX}%` }}>
        <div className={styles.sectionLabel}>Smart Reports</div>
        <div className={styles.tableWrap}>
          <LogsTable logs={smartLogs} />
        </div>
      </div>
      <div
        className={styles.dragBarX}
        onMouseDown={() => (dragging.current = "x")}
      />
      <div className={styles.right} style={{ width: `${100 - splitX}%` }}>
        <div className={styles.top} style={{ height: `${splitY}%` }}>
          <div className={styles.sectionLabel}>Sensor Reports</div>
          <div className={styles.tabs}>
            {SENSOR_TABS.map((tab) => (
              <button
                key={tab}
                className={activeSensor === tab ? styles.activeTab : styles.tab}
                onClick={() => setActiveSensor(tab)}
              >
                {tab}
              </button>
            ))}
          </div>
          <div className={styles.tableWrap}>
            <LogsTable logs={sensorLogs[activeSensor] || []} />
          </div>
        </div>
        <div
          className={styles.dragBarY}
          onMouseDown={() => (dragging.current = "y")}
        />
        <div className={styles.bottom} style={{ height: `${100 - splitY}%` }}>
          <div className={styles.sectionLabel}>Node Reports</div>
          <div className={styles.tabs}>
            {NODE_TABS.map((tab) => (
              <button
                key={tab}
                className={activeNode === tab ? styles.activeTab : styles.tab}
                onClick={() => setActiveNode(tab)}
              >
                {tab}
              </button>
            ))}
          </div>
          <div className={styles.tableWrap}>
            <LogsTable logs={nodeLogs[activeNode] || []} />
          </div>
        </div>
      </div>
    </div>
  );
}

function LogsTable({ logs }) {
  return (
    <table className={styles.table}>
      <thead>
        <tr>
          <th style={{ width: "180px" }}>Time</th>
          <th>Report</th>
        </tr>
      </thead>
      <tbody>
        {logs.length === 0 ? (
          <tr>
            <td colSpan={2} style={{ textAlign: "center", color: "#888" }}>
              No data
            </td>
          </tr>
        ) : (
          logs.map((log, i) => (
            <tr key={i}>
              <td>
                {typeof log.time === "string"
                  ? log.time
                  : JSON.stringify(log.time)}
              </td>
              <td>
                {typeof log.text === "string"
                  ? log.text
                  : JSON.stringify(log.text)}
              </td>
            </tr>
          ))
        )}
      </tbody>
    </table>
  );
}
